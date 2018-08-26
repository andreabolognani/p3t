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

public class TimerState {

    public enum Status {
        STOPPED,
        RUNNING,
        PAUSED,
        FINISHED;
    }

    private int mId;
    private int mTargetTime;

    private Status mStatus;
    private int mCurrentTime;

    public TimerState(int id) {
        mId = id;
    }

    public int getId() {
        return mId;
    }

    public int getCurrentTime() {
        return mCurrentTime;
    }

    public void setCurrentTime(int currentTime) {
        mCurrentTime = currentTime;
    }

    public Status getStatus() {
        return mStatus;
    }

    public void setStatus(Status status) {
        mStatus = status;
    }

    public int getTargetTime() {
        return mTargetTime;
    }

    public void setTargetTimeAsMinutes(int targetTimeAsMinutes) {
        mTargetTime = targetTimeAsMinutes * 60 * 1000;
    }
}
