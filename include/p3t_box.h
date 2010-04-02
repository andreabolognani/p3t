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
