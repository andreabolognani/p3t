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

#ifndef __P3T_APPLICATION_H__
#define __P3T_APPLICATION_H__

#include <p3t_widget.h>

#define P3T_APPLICATION(x) ((p3t_application*) (x))

typedef struct _p3t_application p3t_application;
typedef struct _p3t_applicationPrivate p3t_applicationPrivate;

struct _p3t_application {
	p3t_widget               baseObject;
	p3t_applicationPrivate  *priv;
};

p3t_application*  p3t_applicationNew       (void);
void              p3t_applicationDestroy   (p3t_application  *application);

void              p3t_applicationUpdate    (p3t_application  *application);

/* To be used only for inheritance */
void             _p3t_applicationInit      (p3t_application  *application);
void             _p3t_applicationFinalize  (p3t_application  *application);

#endif /* __P3T_APPLICATION_H__ */
