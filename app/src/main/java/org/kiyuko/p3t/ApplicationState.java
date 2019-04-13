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

import java.util.HashMap;

public class ApplicationState {

    private static String TAG = "app/ApplicationState";

    private HashMap<Integer, TimerState> mAllTimerState;

    public ApplicationState(int count, int targetTimeAsMinutes) {
        mAllTimerState = new HashMap<>();

        for (int i = 1; i <= count; i++) {
            TimerState state = new TimerState(i);

            state.setStatus(TimerState.Status.STOPPED);
            state.setTargetTimeAsMinutes(targetTimeAsMinutes);
            state.setCurrentTime(state.getTargetTime());

            mAllTimerState.put(i, state);
        }
    }

    public HashMap<Integer, TimerState> getAllTimerState() {
        return mAllTimerState;
    }
}
