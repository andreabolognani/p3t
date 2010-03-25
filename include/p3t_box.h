#ifndef __P3T_BOX_H__
#define __P3T_BOX_H__

#include <p3t_point.h>

typedef struct _p3t_box p3t_box;

p3t_box*  p3t_boxNew            (int       x,
                                 int       y,
                                 int       width,
                                 int       height);
void      p3t_boxDestroy        (p3t_box  *box);

int       p3t_boxGetX           (p3t_box  *box);
int       p3t_boxGetY           (p3t_box  *box);
int       p3t_boxGetWidth       (p3t_box  *box);
int       p3t_boxGetHeight      (p3t_box  *box);

int       p3t_boxContainsPoint  (p3t_box    *box,
                                 p3t_point  *point);

#endif /* __P3T_BOX_H__ */
