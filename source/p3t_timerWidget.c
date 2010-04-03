#include <p3t_timerWidget.h>

#include <stdlib.h>

struct _p3t_timerWidgetPrivate {
	p3t_timer  *timer;
};

void
_p3t_timerWidgetInit (p3t_timerWidget  *self,
                     int               x,
                     int               y,
                     int               width,
                     int               height)
{
	p3t_timerWidgetPrivate *priv;

	_p3t_widgetInit (P3T_WIDGET (self), x, y, width, height);

	priv = malloc (sizeof (p3t_timerWidgetPrivate));

	priv->timer = NULL;

	self->priv = priv;
}

void
_p3t_timerWidgetFinalize (p3t_timerWidget *self)
{
	if (self->priv->timer != NULL) {
		p3t_timerDestroy (self->priv->timer);
	}
	free (self->priv);

	_p3t_widgetFinalize (P3T_WIDGET (self));
}

p3t_timerWidget*
p3t_timerWidgetNew (int  x,
                    int  y,
                    int  width,
                    int  height)
{
	p3t_timerWidget *self;

	self = (p3t_timerWidget*) malloc (sizeof (p3t_timerWidget));
	_p3t_timerWidgetInit (self, x, y, width, height);

	return self;
}

void
p3t_timerWidgetDestroy (p3t_timerWidget *self)
{
	_p3t_timerWidgetFinalize (self);
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
