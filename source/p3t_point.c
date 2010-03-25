#include <p3t_point.h>

#include <stdlib.h>

struct _p3t_point {
	int  x;
	int  y;
};

static void
init (p3t_point  *self,
      int         x,
      int         y)
{
	self->x = x;
	self->y = y;
}

static void
finalize (p3t_point *self)
{
	/* Nothing to do */
}

p3t_point*
p3t_pointNew (int  x,
              int  y)
{
	p3t_point *self;

	self = (p3t_point*) malloc (sizeof (p3t_point));
	init (self, x, y);

	return self;
}

void
p3t_pointDestroy (p3t_point *self)
{
	finalize (self);
	free (self);
}

int
p3t_pointGetX (p3t_point *self)
{
	return self->x;
}

int
p3t_pointGetY (p3t_point *self)
{
	return self->y;
}
