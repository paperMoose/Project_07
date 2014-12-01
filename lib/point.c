//
//  point.c
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#include <stdlib.h>
#include <stdio.h>
#include "point.h"
#include "color.h"
#include "Image.h"

void point_set2D(Point *p, double x, double y){
/*set the first two values of the vector to x and y. Set the third value to 0 and the fourth value to 1 .*/
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=0.0;
    p->val[3]=1.0;
}

void point_set3D(Point *p, double x, double y, double z){
    /*set the point’s values to x and y and z. Set the homogeneous coordinate to 1 .*/
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=z;
    p->val[3]=1;
}

void point_set(Point *p, double x, double y, double z, double h){
/*set the four values of the vector to x, y, z, and h, respectively.*/
    p->val[0]=x;
    p->val[1]=y;
    p->val[2]=z;
    p->val[3]=h;
}


void point_copy(Point *to, Point *from){
/*copy the point data structure.*/
    if (to==NULL||from ==NULL) {
        printf("ERROR: point_copy >> pointer parameter is NULL\n");
        exit(-1);
    }
    *to = *from; // you can copy structuresd like this as long as there is no dynamic allocation
}


void point_draw(Point *p, Image *src, Color c){
/*draw the point into src using color c.*/
    if (p==NULL||src ==NULL) {
        printf("ERROR: point_draw >> pointer parameter is NULL\n");
        exit(-1);
    }
    int x=(int)p->val[1];
    int y=(int)p->val[0];
    image_setc(src, x, y, 0, c.rgb[0]);
    image_setc(src, x, y, 1, c.rgb[1]);
    image_setc(src, x, y, 2, c.rgb[2]);
    image_setz(src, x, y, p->val[2]);
}


void point_drawf(Point *p, Image *src, FPixel c){
/*draw the p into src using FPixel c.*/
    if (p==NULL||src ==NULL) {
        printf("ERROR: point_drawf >> pointer parameter is NULL\n");
        exit(-1);
    }
    double x=p->val[1];
    double y=p->val[0];
    image_setf(src, x, y, c);
    image_setz(src, x, y, p->val[2]);
}

void point_print(Point *p, FILE *fp){
    if(NULL!=p && NULL!=fp){
        fprintf(fp, "( %.3f %.3f %.3f %.3f )\n",p->val[0], p->val[1], p->val[2], p->val[3]);
        fprintf(fp, "\n");
    }
}

//normalizes the point p by the h coords.
void point_normalize(Point *p){
    p->val[0] = p->val[0]/ p->val[3];
    p->val[1] = p->val[1]/ p->val[3];
}
