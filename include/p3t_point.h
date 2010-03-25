#ifndef __P3T_POINT_H__
#define __P3T_POINT_H__

typedef struct _p3t_point p3t_point;

p3t_point*  p3t_pointNew      (int         x,
                               int         y);
void        p3t_pointDestroy  (p3t_point  *point);

int         p3t_pointGetX     (p3t_point  *point);
int         p3t_pointGetY     (p3t_point  *point);

#endif /* __P3T_POINT_H__ */
