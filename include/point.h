//
//  point.h
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#ifndef __POINT_H__
#define __POINT_H__

#include "Image.h"
#include "color.h"


typedef struct{
    double val[4];
} Point;



void point_set2D(Point *p, double x, double y);
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
void point_copy(Point *to, Point *from);
void point_draw(Point *p, Image *src, Color c);
void point_drawf(Point *p, Image *src, FPixel c);
void point_print(Point *p, FILE *fp);
void point_normalize(Point *p);

#endif /* defined(__POINT_H__) */
