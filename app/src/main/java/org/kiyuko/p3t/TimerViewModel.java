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

import android.app.Application;
import android.arch.lifecycle.AndroidViewModel;
import android.arch.lifecycle.LiveData;

import java.util.HashMap;

public class TimerViewModel extends AndroidViewModel {

    private static String TAG = "app/TimerViewModel";

    private TimerRunner mRunner;
    private LiveData<HashMap<Integer, TimerState>> mAllTimerState;

    public TimerViewModel(Application application) {
        super(application);

        int count = application.getResources().getInteger(R.integer.timerCount);
        int targetTimeAsMinutes = application.getResources().getInteger(R.integer.timerTargetTimeAsMinutes);

        mRunner = TimerRunner.getRunner(application, count, targetTimeAsMinutes);
        mAllTimerState = mRunner.getAllTimerState();
    }

    public LiveData<HashMap<Integer, TimerState>> getAllTimerState() {
        return mAllTimerState;
    }

    public void up(TimerState state) {
        mRunner.up(state);
    }

    public void down(TimerState state) {
        mRunner.down(state);
    }

    public void action(TimerState state) {
        mRunner.action(state);
    }
}
