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

#ifndef __P3T_PIXMAP_H__
#define __P3T_PIXMAP_H__

#include <p3t_box.h>

#define P3T_PIXMAP(x) ((p3t_pixmap*) (x))

typedef struct _p3t_pixmap p3t_pixmap;
typedef struct _p3t_pixmapPrivate p3t_pixmapPrivate;

typedef enum {
	P3T_PIXMAP_TYPE_DIGIT,
	P3T_PIXMAP_TYPE_NUMBER,
	P3T_PIXMAP_TYPE_BUTTON,
	P3T_PIXMAP_TYPE_OUTLINE,
	P3T_PIXMAP_TYPE_BACKGROUND
} p3t_pixmapType;

typedef enum {
	P3T_PIXMAP_DIGIT_0,
	P3T_PIXMAP_DIGIT_1,
	P3T_PIXMAP_DIGIT_2,
	P3T_PIXMAP_DIGIT_3,
	P3T_PIXMAP_DIGIT_4,
	P3T_PIXMAP_DIGIT_5,
	P3T_PIXMAP_DIGIT_6,
	P3T_PIXMAP_DIGIT_7,
	P3T_PIXMAP_DIGIT_8,
	P3T_PIXMAP_DIGIT_9
} p3t_pixmapDigitType;

typedef enum {
	P3T_PIXMAP_NUMBER_1,
	P3T_PIXMAP_NUMBER_2,
	P3T_PIXMAP_NUMBER_3,
	P3T_PIXMAP_NUMBER_4,
	P3T_PIXMAP_NUMBER_5,
	P3T_PIXMAP_NUMBER_6,
	P3T_PIXMAP_NUMBER_7,
	P3T_PIXMAP_NUMBER_8
} p3t_pixmapNumberType;

typedef enum {
	P3T_PIXMAP_BUTTON_UP,
	P3T_PIXMAP_BUTTON_DOWN,
	P3T_PIXMAP_BUTTON_ACTION
} p3t_pixmapButtonType;

typedef enum {
	P3T_PIXMAP_OUTLINE_TIMERWIDGET,
	P3T_PIXMAP_OUTLINE_BUTTON
} p3t_pixmapOutlineType;

typedef enum {
	P3T_PIXMAP_BACKGROUND_DEFAULT
} p3t_pixmapBackgroundType;

struct _p3t_pixmap {
	p3t_box             baseObject;
	p3t_pixmapPrivate  *priv;
};

p3t_pixmap*  p3t_pixmapGet        (p3t_pixmapType   type,
                                   int              identifier);
void         p3t_pixmapDraw       (p3t_pixmap      *pixmap,
                                   p3t_box         *box);

#endif /* __P3T_PIXMAP_H__ */
