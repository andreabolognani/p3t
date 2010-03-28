#include <p3t_timerWidget.h>
#include <p3t_application.h>

#include <stdlib.h>

struct _p3t_timerWidgetPrivate {
	p3t_timer  *timer;
};

static void
activateCallback (p3t_widget  *widget,
                  void        *data)
{
	p3t_timerWidget *self;
	p3t_timer *timer;

	self = P3T_TIMERWIDGET (widget);
	timer = p3t_timerWidgetGetTimer (self);

	if (timer != NULL) {
		switch (p3t_timerGetState (timer)) {

			case P3T_TIMER_STATE_STOPPED:
			case P3T_TIMER_STATE_PAUSED:

				p3t_timerStart (timer);
				break;

			case P3T_TIMER_STATE_RUNNING:

				p3t_timerPause (timer);
				break;

			case P3T_TIMER_STATE_FINISHED:

				p3t_timerStop (timer);
				break;
		}
	}
}

void
p3t_timerWidgetInit (p3t_timerWidget *self)
{
	p3t_timerWidgetPrivate *priv;

	p3t_widgetInit (P3T_WIDGET (self));

	priv = malloc (sizeof (p3t_timerWidgetPrivate));

	priv->timer = NULL;

	self->priv = priv;

	p3t_widgetSetActivateCallback (P3T_WIDGET (self),
	                               activateCallback,
	                               NULL);
}

void
p3t_timerWidgetFinalize (p3t_timerWidget *self)
{
	if (self->priv->timer != NULL) {
		p3t_timerDestroy (self->priv->timer);
	}

	p3t_widgetFinalize (P3T_WIDGET (self));
}

p3t_timerWidget*
p3t_timerWidgetNew (void)
{
	p3t_timerWidget *self;

	self = malloc (sizeof (p3t_timerWidget));
	p3t_timerWidgetInit (self);

	return self;
}

void
p3t_timerWidgetDestroy (p3t_timerWidget *self)
{
	p3t_timerWidgetFinalize (self);
	free (self);
}

void
p3t_timerWidgetSetTimer (p3t_timerWidget  *self,
                         p3t_timer        *timer)
{
	if (self->priv->timer != NULL) {
		p3t_timerDestroy (self->priv->timer);
	}

	self->priv->timer = timer;
}

p3t_timer*
p3t_timerWidgetGetTimer (p3t_timerWidget *self)
{
	return self->priv->timer;
}
