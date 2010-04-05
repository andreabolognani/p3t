#ifndef __P3T_WIDGET_H__
#define __P3T_WIDGET_H__

#include <p3t_point.h>
#include <p3t_box.h>

#define P3T_WIDGET(x) ((p3t_widget*) (x))

typedef struct _p3t_widget p3t_widget;
typedef struct _p3t_widgetPrivate p3t_widgetPrivate;

typedef void (*p3t_widgetActivateCallback) (p3t_widget*, p3t_point*, void*);
typedef void (*p3t_widgetPaintCallback)    (p3t_widget*, void*);

struct _p3t_widget {
	p3t_box             baseObject;
	p3t_widgetPrivate  *priv;
};

p3t_widget*  p3t_widgetNew                  (int                          x,
                                             int                          y,
                                             int                          width,
                                             int                          height);
void         p3t_widgetDestroy              (p3t_widget                  *widget);

void         p3t_widgetPaint                (p3t_widget                  *widget);
void         p3t_widgetActivate             (p3t_widget                  *widget);
void         p3t_widgetTryActivate          (p3t_widget                  *widget,
                                             p3t_point                   *point);

void         p3t_widgetSetPaintCallback     (p3t_widget                  *widget,
                                             p3t_widgetPaintCallback      callback,
                                             void                        *data);
void         p3t_widgetSetActivateCallback  (p3t_widget                  *widget,
                                             p3t_widgetActivateCallback   callback,
                                             void                        *data);

/* To be used only for inheritance */
void        _p3t_widgetInit                 (p3t_widget                  *widget,
                                             int                          x,
                                             int                          y,
                                             int                          width,
                                             int                          height);
void        _p3t_widgetFinalize             (p3t_widget                  *widget);

#endif /* __P3T_WIDGET_H__ */
