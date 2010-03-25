#include <p3t_widget.h>

#include <stdlib.h>

struct _p3t_widgetPrivate {
	p3t_box             *box;
	p3t_widgetCallback  *activateCallback;
	void                *activateCallbackData;
};

static void
init (p3t_widget *self)
{
	p3t_widgetPrivate *priv;

	priv = (p3t_widgetPrivate*) malloc (sizeof (p3t_widgetPrivate));

	priv->box = NULL;
	priv->activateCallback = NULL;
	priv->activateCallbackData = NULL;

	self->priv = priv;
}

static void
finalize (p3t_widget *self)
{
	if (self->priv->box != NULL) {
		p3t_boxDestroy (self->priv->box);
	}

	free (self->priv);
}

p3t_widget*
p3t_widgetNew (void)
{
	p3t_widget *self;

	self = (p3t_widget*) malloc (sizeof (p3t_widget));
	init (self);

	return self;
}

void
p3t_widgetDestroy (p3t_widget *self)
{
	finalize (self);
	free (self);
}

void
p3t_widgetSetActivateCallback (p3t_widget          *self,
                               p3t_widgetCallback  *callback,
                               void                *data)
{
	self->priv->activateCallback = callback;
	self->priv->activateCallbackData = data;
}

void
p3t_widgetActivate (p3t_widget *self)
{
	p3t_widgetCallback activateCallback;

	activateCallback = (*(self->priv->activateCallback));
	activateCallback (self, self->priv->activateCallbackData);
}

void
p3t_widgetTryActivate (p3t_widget  *self,
                       p3t_point   *point)
{
	if (p3t_boxContainsPoint (self->priv->box, point)) {
		p3t_widgetActivate (self);
	}
}

void
p3t_widgetSetBox (p3t_widget  *self,
                  p3t_box     *box)
{
	if (self->priv->box != NULL) {
		p3t_boxDestroy (self->priv->box);
	}

	self->priv->box = box;
}

p3t_box*
p3t_widgetGetBox (p3t_widget *self)
{
	return self->priv->box;
}
