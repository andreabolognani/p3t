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

p3t_button*  p3t_buttonNew       (int                    x,
                                  int                    y,
                                  int                    width,
                                  int                    height,
                                  p3t_pixmapButtonType   buttonType);
void         p3t_buttonDestroy   (p3t_button            *button);

/* To be used only for inheritance*/
void        _p3t_buttonInit      (p3t_button            *button,
                                  int                    x,
                                  int                    y,
                                  int                    width,
                                  int                    height,
                                  p3t_pixmapButtonType   buttonType);
void        _p3t_buttonFinalize  (p3t_button            *button);

#endif /* __P3T_BUTTON_H__ */
