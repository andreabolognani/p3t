/* P3T - Countdown timers for your device
 * Copyright (C) 2010-2018  Andrea Bolognani <eof@kiyuko.org>
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

import android.arch.lifecycle.LiveData;

import java.util.Date;
import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

public class TimerRunner extends TimerTask {

    private static int UPDATE_PERIOD = 100;

    private static TimerRunner INSTANCE;

    private LiveDataWrapper mAllTimerState;
    private long mLastUpdate;
    private Timer mTimer;

    public static TimerRunner getRunner(int count, int targetTimeAsMinutes) {
        if (INSTANCE == null) {
            synchronized (TimerRunner.class) {
                if (INSTANCE == null) {
                    INSTANCE = new TimerRunner(count, targetTimeAsMinutes);
                }
            }
        }
        return INSTANCE;
    }

    private TimerRunner(int count, int targetTimeAsMinutes) {
        mAllTimerState = new LiveDataWrapper(count, targetTimeAsMinutes);

        mLastUpdate = new Date().getTime();
        mTimer = new Timer();
        mTimer.scheduleAtFixedRate(this, 0, UPDATE_PERIOD);
    }

    public LiveData<HashMap<Integer, TimerState>> getAllTimerState() {
        return mAllTimerState;
    }

    public void up(TimerState state) {
        state = mAllTimerState.getValue().get(state.getId());

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

        mAllTimerState.post();
    }

    public void down(TimerState state) {
        state = mAllTimerState.getValue().get(state.getId());

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

        mAllTimerState.post();
    }

    public void action(TimerState state) {
        state = mAllTimerState.getValue().get(state.getId());

        switch (state.getStatus()) {
            case STOPPED: {
                state.setStatus(TimerState.Status.RUNNING);
                break;
            }
            case RUNNING: {
                state.setStatus(TimerState.Status.PAUSED);
                break;
            }
            case PAUSED: {
                state.setStatus(TimerState.Status.RUNNING);
                break;
            }
            case FINISHED: {
                state.setStatus(TimerState.Status.STOPPED);
                state.setCurrentTime(state.getTargetTime());
                break;
            }
        }

        mAllTimerState.post();
    }

    @Override
    public void run() {
        HashMap<Integer, TimerState> allTimerState = mAllTimerState.getValue();

        if (allTimerState == null) {
            return;
        }

        long now = new Date().getTime();

        for (TimerState state: allTimerState.values()) {
            if (state.getStatus() == TimerState.Status.RUNNING) {
                int currentTime = state.getCurrentTime();

                currentTime -= (now - mLastUpdate);
                state.setCurrentTime(currentTime);

                if (currentTime <= 0) {
                    state.setStatus(TimerState.Status.FINISHED);
                    state.setCurrentTime(0);
                }
            }
        }

        mLastUpdate = now;

        mAllTimerState.post();
    }

    private class LiveDataWrapper extends LiveData<HashMap<Integer, TimerState>> {
        private HashMap<Integer, TimerState> mAllTimerState;

        public LiveDataWrapper(int count, int targetTimeAsMinutes) {
            mAllTimerState = new HashMap<>();

            for (int i = 1; i <= count; i++) {
                TimerState state = new TimerState(i);

                state.setStatus(TimerState.Status.STOPPED);
                state.setTargetTimeAsMinutes(targetTimeAsMinutes);
                state.setCurrentTime(state.getTargetTime());

                mAllTimerState.put(i, state);
            }

            post();
        }

        public void post() {
            postValue(mAllTimerState);
        }
    }
}
