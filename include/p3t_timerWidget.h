/* P3T - Simple timers for Nintendo DS
 * Copyright (C) 2010  Andrea Bolognani <eof@kiyuko.org>
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
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __P3T_TIMERWIDGET_H__
#define __P3T_TIMERWIDGET_H__

#include <p3t_timer.h>
#include <p3t_widget.h>

#define P3T_TIMERWIDGET(x) ((p3t_timerWidget*) (x))

typedef struct _p3t_timerWidget p3t_timerWidget;
typedef struct _p3t_timerWidgetPrivate p3t_timerWidgetPrivate;

struct _p3t_timerWidget {
	p3t_widget               baseObject;
	p3t_timerWidgetPrivate  *priv;
};

p3t_timerWidget*  p3t_timerWidgetNew       (int               x,
                                            int               y,
                                            int               width,
                                            int               height);
void              p3t_timerWidgetDestroy   (p3t_timerWidget  *timerWidget);

void              p3t_timerWidgetSetTimer  (p3t_timerWidget  *timerWidget,
                                            p3t_timer        *timer);
p3t_timer*        p3t_timerWidgetGetTimer  (p3t_timerWidget  *timerWidget);

/* To be used only for inheritance */
void             _p3t_timerWidgetInit      (p3t_timerWidget  *timerWidget,
                                            int               x,
                                            int               y,
                                            int               width,
                                            int               height);
void             _p3t_timerWidgetFinalize  (p3t_timerWidget  *timerWidget);

#endif /* __P3T_TIMERWIDGET_H__ */
