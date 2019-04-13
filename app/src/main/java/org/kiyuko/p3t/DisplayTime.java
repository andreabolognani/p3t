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

public class DisplayTime {

    private static String TAG = "app/DisplayTime";

    private int mMinutes;
    private int mSeconds;

    public DisplayTime(int time) {

        int milliseconds = time % 1000;
        time = (time - milliseconds) / 1000;
        mSeconds = time % 60;
        time = (time - mSeconds) / 60;
        mMinutes = time;

        if (milliseconds > 0) {

            // We need to round up to the closest second for the timer to work as
            // expected: if we didn't do this, the interface would display 00:00
            // before the timer is actually finished running
            mSeconds++;

            // However, we have to be careful when rounding up or we might end up
            // displaying something silly like 05:60
            if (mSeconds > 59) {
                mMinutes++;
                mSeconds = 0;
            }
        }
    }

    public int getMinutes() {
        return mMinutes;
    }

    public int getSeconds() {
        return mSeconds;
    }

    public String toString() {
        return String.format("%02d:%02d", mMinutes, mSeconds);
    }
}
