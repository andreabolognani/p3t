#ifndef __P3T_APPLICATION_H__
#define __P3T_APPLICATION_H__

#include <nds.h>

#define P3T_APPLICATION(x) ((p3t_application*) (x))

typedef struct _p3t_application p3t_application;

p3t_application*  p3t_applicationNew      (void);
void              p3t_applicationDestroy  (p3t_application  *application);

void              p3t_applicationUpdate   (p3t_application  *application,
                                           int               input);

u16*  p3t_applicationGetBackgroundBuffer  (p3t_application  *application);
u16*  p3t_applicationGetWidgetsBuffer     (p3t_application  *application);

#endif /* __P3T_APPLICATION_H__ */
