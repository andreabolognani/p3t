#include <p3t_box.h>

#include <stdlib.h>

struct _p3t_boxPrivate {
	int  x;
	int  y;
	int  width;
	int  height;
	int  savedX;
	int  savedY;
};

void
_p3t_boxInit (p3t_box  *self,
             int       x,
             int       y,
             int       width,
             int       height)
{
	p3t_boxPrivate *priv;

	priv = (p3t_boxPrivate*) malloc (sizeof (p3t_boxPrivate));

	priv->x = x;
	priv->y = y;
	priv->width = width;
	priv->height = height;
	priv->savedX = x;
	priv->savedY = y;

	self->priv = priv;
}

void
_p3t_boxFinalize (p3t_box *self)
{
	free (self->priv);
}

p3t_box*
p3t_boxNew (int  x,
            int  y,
            int  width,
            int  height)
{
	p3t_box *self;

	self = (p3t_box*) malloc (sizeof (p3t_box));
	_p3t_boxInit (self, x, y, width, height);

	return self;
}

void
p3t_boxDestroy (p3t_box *self)
{
	_p3t_boxFinalize (self);
	free (self);
}

int
p3t_boxContainsPoint (p3t_box    *self,
                      p3t_point  *point)
{
	int pointX;
	int pointY;

	pointX = p3t_pointGetX (point);
	pointY = p3t_pointGetY (point);

	if (pointX < p3t_boxGetX (self)) {
		return 0;
	}
	if (pointY < p3t_boxGetY (self)) {
		return 0;
	}
	if (pointX > (p3t_boxGetX (self) + p3t_boxGetWidth (self))) {
		return 0;
	}
	if (pointY > (p3t_boxGetY (self) + p3t_boxGetHeight (self))) {
		return 0;
	}

	return 1;
}

int
p3t_boxContainsBox (p3t_box  *self,
                    p3t_box  *box)
{
	p3t_point *point;
	int pointX;
	int pointY;

	/* Check the top left corner is contained */
	pointX = p3t_boxGetX (box);
	pointY = p3t_boxGetY (box);
	point = p3t_pointNew (pointX, pointY);

	if (p3t_boxContainsPoint (self, point) == 0) {
		p3t_pointDestroy (point);
		return 0;
	}
	p3t_pointDestroy (point);

	/* Check the bottom right corner is contained */
	pointX += p3t_boxGetWidth (box);
	pointY += p3t_boxGetHeight (box);
	point = p3t_pointNew (pointX, pointY);

	if (p3t_boxContainsPoint (self, point) == 0) {
		p3t_pointDestroy (point);
		return 0;
	}
	p3t_pointDestroy (point);

	/* If both the top left and the bottom right corners are
	 * contained, the whole box is contained */
	return 1;
}

void
p3t_boxMakeAbsolute (p3t_box  *self,
                     p3t_box  *container)
{
	/* Move the top left corner of the box */
	self->priv->x += p3t_boxGetX (container);
	self->priv->y += p3t_boxGetY (container);
}

void
p3t_boxMakeRelative (p3t_box  *self,
                     p3t_box  *container)
{
	self->priv->x -= p3t_boxGetX (container);
	self->priv->y -= p3t_boxGetY (container);
}

void
p3t_boxMove (p3t_box  *self,
             int       x,
             int       y)
{
	self->priv->x = x;
	self->priv->y = y;
}

void
p3t_boxSavePosition (p3t_box *self)
{
	self->priv->savedX = p3t_boxGetX (self);
	self->priv->savedY = p3t_boxGetY (self);
}

void
p3t_boxRestorePosition (p3t_box *self)
{
	p3t_boxMove (self,
                 self->priv->savedX,
	             self->priv->savedY);
}

int
p3t_boxGetX (p3t_box *self)
{
	return self->priv->x;
}

int
p3t_boxGetY (p3t_box *self)
{
	return self->priv->y;
}

int
p3t_boxGetWidth (p3t_box *self)
{
	return self->priv->width;
}

int
p3t_boxGetHeight (p3t_box *self)
{
	return self->priv->height;
}
