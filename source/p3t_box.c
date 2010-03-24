#include <p3t_box.h>

#include <stdlib.h>

struct _p3t_box{
	int  x;
	int  y;
	int  width;
	int  height;
};

static void
init (p3t_box  *self,
      int       x,
      int       y,
      int       width,
      int       height)
{
	self->x = x;
	self->y = y;
	self->width = width;
	self->height = height;
}

p3t_box*
p3t_boxNew (int  x,
            int  y,
            int  width,
            int  height)
{
	p3t_box *self;

	self = (p3t_box*) malloc (sizeof (p3t_box));
	init (self, x, y, width, height);

	return self;
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
p3t_boxGetX (p3t_box *self)
{
	return self->x;
}

int
p3t_boxGetY (p3t_box *self)
{
	return self->y;
}

int
p3t_boxGetWidth (p3t_box *self)
{
	return self->width;
}

int
p3t_boxGetHeight (p3t_box *self)
{
	return self->height;
}
