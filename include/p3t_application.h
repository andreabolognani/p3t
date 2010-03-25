#ifndef __P3T_APPLICATION_H__
#define __P3T_APPLICATION_H__

typedef struct _p3t_application p3t_application;

p3t_application*  p3t_applicationNew      (void);
void              p3t_applicationDestroy  (p3t_application  *application);

void              p3t_applicationUpdate   (p3t_application  *application,
                                           int               input);

#endif /* __P3T_APPLICATION_H__ */
