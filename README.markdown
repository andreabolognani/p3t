P3T - Countdown timers for Android devices
==========================================

P3T provides a bunch of independent countdown timers, which can be
started, stopped and programmed individually. A sound is emitted when a
timer reaches the end of its countdown.


Usage
-----

After starting the application, you will be greeted by a screen containing
eight timers: each one is marked with a number and contains the remaining
time along with three control buttons.

The rightmost button, the one with a black circle in it, is the action
button: tap it when the timer is stopped, and it will start; tap it when
the timer is running, and it will pause; tap it when the timer has reached
the end of its countdown, and it will reset to its initial state.

The two remaining buttons can be used to increase or decrease the target
time by one minute at a time. If the timer is running when changing the
target time, it will be stopped.

You can turn off the screen at any time without affecting running timers;
switching applications, however, might result in P3T being terminated by
Android if the device is resource constrained, so it is better avoided.


Compatibility
-------------

P3T works on Android 4.1 and newer; the interface, however, has been
optimized for cheap 7" tablets with relatively low resolution screens
and might not display correctly on other devices.


Installation
------------

A signed .apk file is provided along with the source code for your
convenience; you will need to enable installation from unknown sources
on your device before you can proceed with the installation.


Source code
-----------

The lastest version of the P3T source code can be obtained by running
the following command:

    git clone https://git.kiyuko.org/p3t.git


License
-------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
