#ifndef __P3T_WIDGET_H__
#define __P3T_WIDGET_H__

#include <p3t_point.h>
#include <p3t_box.h>

#define P3T_WIDGET(x) ((p3t_widget*) (x))

typedef struct _p3t_widget p3t_widget;
typedef struct _p3t_widgetPrivate p3t_widgetPrivate;

typedef void (*p3t_widgetCallback) (p3t_widget*, void*);

struct _p3t_widget {
	p3t_widgetPrivate  *priv;
};

p3t_widget*  p3t_widgetNew                  (void);
void         p3t_widgetDestroy              (p3t_widget  *widget);
void         p3t_widgetInit                 (p3t_widget  *widget);
void         p3t_widgetFinalize             (p3t_widget  *widget);

void         p3t_widgetPaint                (p3t_widget          *widget);
void         p3t_widgetActivate             (p3t_widget          *widget);
void         p3t_widgetTryActivate          (p3t_widget          *widget,
                                             p3t_point           *point);

void         p3t_widgetSetPaintCallback     (p3t_widget          *widget,
                                             p3t_widgetCallback   callback,
                                             void                *data);
void         p3t_widgetSetActivateCallback  (p3t_widget          *widget,
                                             p3t_widgetCallback   callback,
                                             void                *data);
void         p3t_widgetSetBox               (p3t_widget  *widget,
                                             p3t_box     *box);
p3t_box*     p3t_widgetGetBox               (p3t_widget  *widget);

#endif /* __P3T_WIDGET_H__ */
