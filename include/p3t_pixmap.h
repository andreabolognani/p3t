#ifndef __P3T_PIXMAP_H__
#define __P3T_PIXMAP_H__

#include <p3t_box.h>
#include <nds.h>

#define P3T_PIXMAP(x) ((p3t_pixmap*) (x))

typedef struct _p3t_pixmap p3t_pixmap;
typedef struct _p3t_pixmapPrivate p3t_pixmapPrivate;

typedef enum {
	P3T_PIXMAP_TYPE_DIGIT      = 0,
	P3T_PIXMAP_TYPE_BACKGROUND = 1
} p3t_pixmapType;

enum {
	P3T_PIXMAP_DIGIT_0 = 0,
	P3T_PIXMAP_DIGIT_1 = 1,
	P3T_PIXMAP_DIGIT_2 = 2,
	P3T_PIXMAP_DIGIT_3 = 3,
	P3T_PIXMAP_DIGIT_4 = 4,
	P3T_PIXMAP_DIGIT_5 = 5,
	P3T_PIXMAP_DIGIT_6 = 6,
	P3T_PIXMAP_DIGIT_7 = 7,
	P3T_PIXMAP_DIGIT_8 = 8,
	P3T_PIXMAP_DIGIT_9 = 9
};

struct _p3t_pixmap {
	p3t_box             baseObject;
	p3t_pixmapPrivate  *priv;
};

p3t_pixmap*  p3t_pixmapGet        (p3t_pixmapType   type,
                                   int              identifier);
void         p3t_pixmapDraw       (p3t_pixmap      *pixmap,
                                   p3t_box         *box,
                                   u16             *video);

#endif /* __P3T_PIXMAP_H__ */
