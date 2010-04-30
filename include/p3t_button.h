/* P3T - Coundown timers for Nintendo DS
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

#ifndef __P3T_BUTTON_H__
#define __P3T_BUTTON_H__

#include <p3t_widget.h>
#include <p3t_pixmap.h>

#define P3T_BUTTON(x) ((p3t_button*) (x))

typedef struct _p3t_button p3t_button;
typedef struct _p3t_buttonPrivate p3t_buttonPrivate;

struct _p3t_button {
	p3t_widget          baseObject;
	p3t_buttonPrivate  *priv;
};

p3t_button*            p3t_buttonNew       (int                    x,
                                            int                    y,
                                            int                    width,
                                            int                    height,
                                            p3t_pixmapButtonType   buttonType);
void                   p3t_buttonDestroy   (p3t_button            *button);

p3t_pixmapButtonType   p3t_buttonGetType   (p3t_button            *button);

/* To be used only for inheritance*/
void                  _p3t_buttonInit      (p3t_button            *button,
                                            int                    x,
                                            int                    y,
                                            int                    width,
                                            int                    height,
                                            p3t_pixmapButtonType   buttonType);
void                  _p3t_buttonFinalize  (p3t_button            *button);

#endif /* __P3T_BUTTON_H__ */
