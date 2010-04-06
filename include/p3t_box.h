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

#ifndef __P3T_BOX_H__
#define __P3T_BOX_H__

#include <p3t_point.h>

#define P3T_BOX(x) ((p3t_box*) (x))

typedef struct _p3t_box p3t_box;
typedef struct _p3t_boxPrivate p3t_boxPrivate;

struct _p3t_box {
	p3t_boxPrivate *priv;
};

p3t_box*  p3t_boxNew              (int       x,
                                   int       y,
                                   int       width,
                                   int       height);
void      p3t_boxDestroy          (p3t_box  *box);

int       p3t_boxGetX             (p3t_box  *box);
int       p3t_boxGetY             (p3t_box  *box);
int       p3t_boxGetWidth         (p3t_box  *box);
int       p3t_boxGetHeight        (p3t_box  *box);

int       p3t_boxContainsPoint    (p3t_box    *box,
                                   p3t_point  *point);
int       p3t_boxContainsBox      (p3t_box    *box,
                                   p3t_box    *anotherBox);

void      p3t_boxMakeRelative     (p3t_box    *box,
                                   p3t_box    *container);
void      p3t_boxMakeAbsolute     (p3t_box    *box,
                                   p3t_box    *container);
void      p3t_boxMove             (p3t_box    *box,
                                   int         x,
                                   int         y);
void      p3t_boxSavePosition     (p3t_box    *box);
void      p3t_boxRestorePosition  (p3t_box    *box);

/* To be used only for inheritance */
void     _p3t_boxInit             (p3t_box  *box,
                                   int       x,
                                   int       y,
                                   int       width,
                                   int       height);
void     _p3t_boxFinalize         (p3t_box  *box);

#endif /* __P3T_BOX_H__ */
