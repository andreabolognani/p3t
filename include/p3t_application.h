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

/* To be used only for inheritance */
void             _p3t_applicationInit      (p3t_application  *application);
void             _p3t_applicationFinalize  (p3t_application  *application);

#endif /* __P3T_APPLICATION_H__ */
