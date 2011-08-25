/* P3T - Coundown timers for Nintendo DS
 * Copyright (C) 2010-2011  Andrea Bolognani <eof@kiyuko.org>
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

#ifndef __P3T_WIDGET_H__
#define __P3T_WIDGET_H__

#include <p3t_point.h>
#include <p3t_box.h>

#define P3T_WIDGET(x) ((p3t_widget*) (x))

typedef struct _p3t_widget p3t_widget;
typedef struct _p3t_widgetPrivate p3t_widgetPrivate;

typedef void (*p3t_widgetActivateCallback) (p3t_widget*, p3t_point*, void*);
typedef void (*p3t_widgetPaintCallback)    (p3t_widget*, void*);

struct _p3t_widget {
	p3t_box             baseObject;
	p3t_widgetPrivate  *priv;
};

p3t_widget*  p3t_widgetNew                  (int                          x,
                                             int                          y,
                                             int                          width,
                                             int                          height);
void         p3t_widgetDestroy              (p3t_widget                  *widget);

void         p3t_widgetPaint                (p3t_widget                  *widget);
void         p3t_widgetActivate             (p3t_widget                  *widget);
void         p3t_widgetTryActivate          (p3t_widget                  *widget,
                                             p3t_point                   *point);

void         p3t_widgetSetPaintCallback     (p3t_widget                  *widget,
                                             p3t_widgetPaintCallback      callback,
                                             void                        *data);
void         p3t_widgetSetActivateCallback  (p3t_widget                  *widget,
                                             p3t_widgetActivateCallback   callback,
                                             void                        *data);

/* To be used only for inheritance */
void        _p3t_widgetInit                 (p3t_widget                  *widget,
                                             int                          x,
                                             int                          y,
                                             int                          width,
                                             int                          height);
void        _p3t_widgetFinalize             (p3t_widget                  *widget);

#endif /* __P3T_WIDGET_H__ */
