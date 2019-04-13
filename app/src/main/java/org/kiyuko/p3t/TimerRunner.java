/* P3T - Countdown timers for your device
 * Copyright (C) 2010-2019  Andrea Bolognani <eof@kiyuko.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package org.kiyuko.p3t;

import android.app.Application;
import android.arch.lifecycle.LiveData;
import android.content.Context;
import android.media.MediaPlayer;
import android.os.PowerManager;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class TimerRunner extends TimerTask {

    private static String TAG = "app/TimerRunner";

    private static int UPDATE_PERIOD = 100;

    private static TimerRunner INSTANCE;

    private Application mApplication;
    private MediaPlayer[] mMediaPlayers;
    private PowerManager.WakeLock[] mWakeLocks;

    private LiveDataWrapper mApplicationState;
    private long mLastUpdate;
    private Timer mTimer;

    public static TimerRunner getRunner(Application application, int count, int targetTimeAsMinutes) {
        if (INSTANCE == null) {
            synchronized (TimerRunner.class) {
                if (INSTANCE == null) {
                    INSTANCE = new TimerRunner(application, count, targetTimeAsMinutes);
                }
            }
        }
        return INSTANCE;
    }

    private TimerRunner(Application application, int count, int targetTimeAsMinutes) {
        mApplicationState = new LiveDataWrapper(count, targetTimeAsMinutes);

        mApplication = application;

        mMediaPlayers = new MediaPlayer[count + 1];
        for (int i = 1; i <= count; i++) {
            mMediaPlayers[i] = MediaPlayer.create(mApplication, R.raw.finish);
        }

        PowerManager powerManager = (PowerManager) mApplication.getSystemService(Context.POWER_SERVICE);

        mWakeLocks = new PowerManager.WakeLock[count + 1];
        for (int i = 1; i <= count; i++) {
            String tag = String.format("org.kiyuko.p3t:timer/%d", count);
            mWakeLocks[i] = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, tag);
        }

        mLastUpdate = new Date().getTime();
        mTimer = new Timer();
        mTimer.scheduleAtFixedRate(this, 0, UPDATE_PERIOD);
    }

    public LiveData<ApplicationState> getApplicationState() {
        return mApplicationState;
    }

    public void up(TimerState state) {
        state = mApplicationState.getValue().getAllTimerState().get(state.getId());

        stopSound(state);

        DisplayTime displayTime = new DisplayTime(state.getCurrentTime());
        int minutes = displayTime.getMinutes();

        // Rounding up to the closest minute always gives us the behavior we
        // want, for example 04:20 -> 05.00 and 10:00 -> 11:00
        minutes++;

        // Prevent unusable values
        if (minutes > 99) {
            minutes = 99;
        }

        state.setStatus(TimerState.Status.STOPPED);
        state.setTargetTimeAsMinutes(minutes);
        state.setCurrentTime(state.getTargetTime());

        mApplicationState.post();
    }

    public void down(TimerState state) {
        state = mApplicationState.getValue().getAllTimerState().get(state.getId());

        stopSound(state);

        DisplayTime displayTime = new DisplayTime(state.getCurrentTime());
        int minutes = displayTime.getMinutes();
        int seconds = displayTime.getSeconds();

        // Rounding down to the closest minute is only necessary if there are
        // no spare seconds, otherwise we can just discard the seconds and obtain
        // the behavior we want, that is 13:37 -> 13:00 and 11:00 -> 10:00
        if (seconds == 0) {
            minutes--;
        }

        // Prevent unusable values
        if (minutes < 1) {
            minutes = 1;
        }

        state.setStatus(TimerState.Status.STOPPED);
        state.setTargetTimeAsMinutes(minutes);
        state.setCurrentTime(state.getTargetTime());

        mApplicationState.post();
    }

    public void action(TimerState state) {
        state = mApplicationState.getValue().getAllTimerState().get(state.getId());

        stopSound(state);

        switch (state.getStatus()) {
            case STOPPED: {
                state.setStatus(TimerState.Status.RUNNING);
                mWakeLocks[state.getId()].acquire();
                break;
            }
            case RUNNING: {
                state.setStatus(TimerState.Status.PAUSED);
                mWakeLocks[state.getId()].release();
                break;
            }
            case PAUSED: {
                state.setStatus(TimerState.Status.RUNNING);
                mWakeLocks[state.getId()].acquire();
                break;
            }
            case FINISHED: {
                state.setStatus(TimerState.Status.STOPPED);
                state.setCurrentTime(state.getTargetTime());
                mWakeLocks[state.getId()].release();
                break;
            }
        }

        mApplicationState.post();
    }

    @Override
    public void run() {
        ApplicationState applicationState = mApplicationState.getValue();

        if (applicationState == null) {
            return;
        }

        long now = new Date().getTime();

        for (TimerState state: applicationState.getAllTimerState().values()) {
            if (state.getStatus() == TimerState.Status.RUNNING) {
                int currentTime = state.getCurrentTime();

                currentTime -= (now - mLastUpdate);
                state.setCurrentTime(currentTime);

                if (currentTime <= 0) {
                    state.setStatus(TimerState.Status.FINISHED);
                    state.setCurrentTime(0);

                    startSound(state);
                }
            }
        }

        mLastUpdate = now;

        mApplicationState.post();
    }

    private void startSound(TimerState state) {
        MediaPlayer mediaPlayer = mMediaPlayers[state.getId()];

        mediaPlayer.start();
    }

    private void stopSound(TimerState state) {
        MediaPlayer mediaPlayer = mMediaPlayers[state.getId()];

        if (mediaPlayer.isPlaying()) {
            try {
                mediaPlayer.stop();
                mediaPlayer.prepare();
            } catch (Exception e) {}
        }
    }

    private class LiveDataWrapper extends LiveData<ApplicationState> {

        private ApplicationState mApplicationState;

        public LiveDataWrapper(int count, int targetTimeAsMinutes) {
            mApplicationState = new ApplicationState(count, targetTimeAsMinutes);
            post();
        }

        public void post() {
            postValue(mApplicationState);
        }
    }
}
