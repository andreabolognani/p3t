#ifndef __P3T_TIMERWIDGET_H__
#define __P3T_TIMERWIDGET_H__

#include <p3t_timer.h>
#include <p3t_widget.h>

#define P3T_TIMERWIDGET(x) ((p3t_timerWidget*) (x))

typedef struct _p3t_timerWidget p3t_timerWidget;
typedef struct _p3t_timerWidgetPrivate p3t_timerWidgetPrivate;

struct _p3t_timerWidget {
	p3t_widget               baseObject;
	p3t_timerWidgetPrivate  *priv;
};

p3t_timerWidget*  p3t_timerWidgetNew       (int               x,
                                            int               y,
                                            int               width,
                                            int               height);
void              p3t_timerWidgetDestroy   (p3t_timerWidget  *timerWidget);

void              p3t_timerWidgetSetTimer  (p3t_timerWidget  *timerWidget,
                                            p3t_timer        *timer);
p3t_timer*        p3t_timerWidgetGetTimer  (p3t_timerWidget  *timerWidget);

/* To be used only for inheritance */
void             _p3t_timerWidgetInit      (p3t_timerWidget  *timerWidget,
                                            int               x,
                                            int               y,
                                            int               width,
                                            int               height);
void             _p3t_timerWidgetFinalize  (p3t_timerWidget  *timerWidget);

#endif /* __P3T_TIMERWIDGET_H__ */
