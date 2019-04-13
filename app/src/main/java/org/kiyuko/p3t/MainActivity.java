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

import android.arch.lifecycle.LiveData;
import android.arch.lifecycle.Observer;
import android.arch.lifecycle.ViewModelProviders;
import android.os.Build;
import android.os.PowerManager;
import android.support.annotation.Nullable;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.AppCompatImageButton;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static String TAG = "app/MainActivity";

    private TimerViewModel mViewModel;
    private LiveData<ApplicationState> mApplicationState;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mViewModel = ViewModelProviders.of(this).get(TimerViewModel.class);
        mApplicationState = mViewModel.getApplicationState();

        initializeInterface();

        mApplicationState.observe(this, new Observer<ApplicationState>() {
            @Override
            public void onChanged(@Nullable ApplicationState applicationState) {
                if (applicationState != null) {
                    updateInterface(applicationState);
                }
            }
        });

        // We need to use observeForever() here because otherwise we would not get updates
        // while the screen is off, which would completely defeat the purpose
        mApplicationState.observeForever(new Observer<ApplicationState>() {
            @Override
            public void onChanged(@Nullable ApplicationState applicationState) {
                if (applicationState != null) {
                    updateScreenStatus(applicationState);
                }
            }
        });

        hideSystemUI();
    }

    private void initializeInterface() {

        int count = getResources().getInteger(R.integer.timerCount);

        for (int i = 1; i <= count; i++) {

            String viewName = String.format("timer%d", i);
            int viewId = getResources().getIdentifier(viewName, "id", getPackageName());

            View view = findViewById(viewId);
            TextView label = view.findViewById(R.id.label);

            // Setting the view's ID to match the corresponding timer's will
            // allow us to look it up more conveniently later
            view.setId(i);
            label.setText(String.format("%d", i));

            AppCompatImageButton upButton = view.findViewById(R.id.upButton);
            upButton.setOnClickListener(this);

            AppCompatImageButton downButton = view.findViewById(R.id.downButton);
            downButton.setOnClickListener(this);

            AppCompatImageButton actionButton = view.findViewById(R.id.actionButton);
            actionButton.setOnClickListener(this);
        }
    }

    private void updateInterface(ApplicationState applicationState) {

        for (TimerState state: applicationState.getAllTimerState().values()) {

            View view = findViewById(state.getId());
            TextView time = view.findViewById(R.id.time);

            DisplayTime displayTime = new DisplayTime(state.getCurrentTime());
            time.setText(displayTime.toString());
        }
    }

    private void updateScreenStatus(ApplicationState applicationState) {

        int windowFlags = WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON;
        int wakeLockFlags = PowerManager.SCREEN_DIM_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP;

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O_MR1) {
            windowFlags = windowFlags |
                          WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED |
                          WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON;
        }

        if (applicationState.getUserAttentionNeeded()) {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
                setShowWhenLocked(true);
                setTurnScreenOn(true);
            }

            getWindow().addFlags(windowFlags);

            // We need to acquire a wakelock if we want the screen to actually turn on.
            // We can release it immediately afterwards, though
            PowerManager powerManager = (PowerManager) getSystemService(POWER_SERVICE);
            PowerManager.WakeLock wakeLock = powerManager.newWakeLock(wakeLockFlags, "org.kiyuko.p3t:screen");
            wakeLock.acquire();
            wakeLock.release();
        } else {

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O_MR1) {
                setShowWhenLocked(false);
                setTurnScreenOn(false);
            }

            getWindow().clearFlags(windowFlags);
        }
    }

    @Override
    public void onClick(View button) {
        View view = (View) button.getParent();

        TimerState state = new TimerState(view.getId());

        switch (button.getId()) {
            case R.id.upButton: {
                mViewModel.up(state);
                break;
            }
            case R.id.downButton: {
                mViewModel.down(state);
                break;
            }
            case R.id.actionButton: {
                mViewModel.action(state);
                break;
            }
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }

    private void hideSystemUI() {
        // Make the application fullscreen on Lollipop or newer
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {

            View decorView = getWindow().getDecorView();
            decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                // Set the content to appear under the system bars so that the
                // content doesn't resize when the system bars hide and show.
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                // Hide the nav bar and status bar
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_FULLSCREEN);
        }

        // Hide the action bar
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.hide();
        }
    }
}
