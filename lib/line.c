//
//  line.c
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#include <stdlib.h>
#include <stdio.h>
#include "line.h"
#include "point.h"
#include "Image.h"
#include "color.h"


void line_set2D(Line *l, double x0, double y0, double x1, double y1){
    /*– initialize a 2D line.*/
    point_set2D(&l->a, x0, y0);
    point_set2D(&l->b, x1, y1);
    line_zBuffer(l, 1); // by default it should be initialized to 1
}

void line_set(Line *l, Point ta, Point tb){
    /*initialize a line to ta and tb.*/
    point_set(&l->a, ta.val[0], ta.val[1], ta.val[2], ta.val[3]);
    point_set(&l->b, tb.val[0], tb.val[1], tb.val[2], tb.val[3]);
    line_zBuffer(l, 1); // by default it should be initialized to 1
}

void line_zBuffer(Line *l, int flag){
/*set the z-buffer flag to the given value.*/
    if (l==NULL) {
        printf("ERROR: line_zBuffer >> pointer parameter is NULL\n");
        exit(-1);
    }
    l->zBuffer=flag;
}


void line_copy(Line *to, Line *from){
/*copy the line data structure.*/
    if (to == NULL || from == NULL) {
        printf("ERROR: line_copy >> pointer parameter is NULL\n");
        exit(-1);
    }
    *to=*from; // you can copy structures like this as long as there is no dynamic allocation
}


void line_draw(Line *l, Image *src, Color c){
/*draw the line into src using color c.*/
    if (l == NULL || src == NULL) {
        printf("ERROR: line_draw >> pointer parameter is NULL\n");
        exit(-1);
    }


    Line new = Liang_Barsky(l,src);
    if (new.a.val[0]==-1)
    {
       return;
    }

    // int y0 = l->a.val[0];
    // int x0 = l->a.val[1];
    // int y1 = l->b.val[0];
    // int x1 = l->b.val[1];
    int y0 = new.a.val[0];
    int x0 = new.a.val[1];
    int y1 = new.b.val[0];
    int x1 = new.b.val[1];





    int dx = abs(x1-x0);
    int dy = abs(y1-y0);
    int sx, sy, err, e2;
    if(x0<x1){
        sx = 1;
    }else{
        sx = -1;
    }
    if(y0<y1){
        sy = 1;
    }else{
        sy = -1;
    }
    if(dx>dy){
        err = dx/2;
    }else{
        err = -dy/2;
    }
    Point p;
    for(;;){
        point_set2D(&p,x0,y0);
        image_setColor(src,  p.val[0], p.val[1], c);
        // point_draw(&p,src,c);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }   
}

/*
    draws dash version of the lines
*/
void line_drawDash(Line *l, Image *src, Color c){
    /*draw the line into src using color c.*/
    if (l == NULL || src == NULL) {
        printf("ERROR: line_draw >> pointer parameter is NULL\n");
        exit(-1);
    }

    int y0 = l->a.val[0];
    int x0 = l->a.val[1];
    int y1 = l->b.val[0];
    int x1 = l->b.val[1];

     /*
    HERE WE PUT THE SPECIAL CASES FOR HORIZONTAL AND VERTICAL LINES
    */
    // if(x0 == x1){ // if vertical
    //     int dy = abs(y1 - y0);
    //     int i;
    //     int cols = src->cols;
    //     Point p;
    //     for(i=0; i<dy; i++){
    //         if(i%4 == 0){
    //             point_set2D(&p, x0, y0+(i*cols));
    //             point_draw(&p, src, c);
    //         }
    //     }
    //     return;
    // }

    // if(y0 == y1){ // if horizontal
    //     int dx = abs(x1 - x0);
    //     int j;
    //     Point p;
    //     for(j=0; j<dx; j++){
    //         if(j%4 == 0){
    //             point_set2D(&p, x0+j, y0);
    //             point_draw(&p, src, c);
    //         }
    //     }
    //     return;
    // }

    int dx = abs(x1-x0);
    int dy = abs(y1-y0);
    int sx, sy, err, e2;
    if(x0<x1){
        sx = 1;
    }else{
        sx = -1;
    }
    if(y0<y1){
        sy = 1;
    }else{
        sy = -1;
    }
    if(dx>dy){
        err = dx/2;
    }else{
        err = -dy/2;
    }
    Point p;
    int i;
    for(i=0;;i++){
        if(i%4 == 0){
            point_set2D(&p,x0,y0);
            point_draw(&p,src,c);
        }
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    } 
}


//normalizes Line l by the h coords
void line_normalize(Line* l){
    l->a.val[0] = l->a.val[0]/l->a.val[3];
    l->a.val[1] = l->a.val[1]/l->a.val[3];

    l->b.val[0] = l->b.val[0]/l->b.val[3];
    l->b.val[1] = l->b.val[1]/l->b.val[3];
}


Line  Liang_Barsky(Line *l, Image *src){
    float t0,t1,t2,tf,p1,p2,p3,p4,q1,q2,q3,q4;
    t0 = 0;
    tf = 1;
    Line ret;

    // boundaries 

    Point origin, end,p0,pf,nullP;
    point_set2D(&origin,0,0); 
    point_set2D(&end,(float)src->cols,(float)src->rows); 
    point_set2D(&nullP,-1,-1);
// loop 0
    p1 = -(l->b.val[0]-l->a.val[0]);
    q1 = l->a.val[0]-origin.val[0];
    t1=q1/p1;

    if (p1==0)
    {
        if(q1<0){
            line_set(&ret,nullP,nullP);
            // printf("q3 \n" );
            return ret;
        }
    }
    else{ 
        if (p1<0)
            {
                t0 = max(t0,t1);
            }
        else{
            tf = min(tf,t1);
        }
    }
    // loop 1
    p2 = l->b.val[0]-l->a.val[0];
    q2 = end.val[0]-l->a.val[0];
    t2 = q2/p2;

    if (p2==0)
    {
        if(q2<0){
            line_set(&ret,nullP,nullP);
            // printf("q3 \n" );
            return ret;
        }
    }
    else{ 
        if (p2<0)
            {
                t0 = max(t0,t2);
            }
        else{
            tf = min(tf,t2);
        }
    }
    // loop 2

    p3 = -(l->b.val[1]-l->a.val[1]);
    q3 = l->a.val[1]-origin.val[1];
    t2 = q3/p3;


    if (p3==0)
    {
        if(q3<0){
            line_set(&ret,nullP,nullP);
            // printf("q3 \n" );
            return ret;
        }
    }
    else{ 
        if (p3<0)
            {
                t0 = max(t0,t2);
            }
        else{
            tf = min(tf,t2);
        }
    }
    if (t0>=tf)
    {
            line_set(&ret,nullP,nullP);
            // printf("t0 %f   tf %f \n",t0,tf );
            return ret;
    }

        // loop 3

    p4 = l->b.val[1]-l->a.val[1];
    q4 = end.val[1]-l->a.val[1];
    t2 = q4/p4;

    if (p4==0)
    {
        if(q4 < 0){
            line_set(&ret,nullP,nullP);
            // printf("q3 \n" );
            return ret;
        }
    }
    else{ 
        if (p4<0)
            {
                t0 = max(t0,t2);
            }
        else{
            tf = min(tf,t2);
        }
    }


    // final calculation
    float p0x = l->a.val[0] + (t0*(l->b.val[0]-l->a.val[0]));
    float p0y = l->a.val[1] + (t0*(l->b.val[1]-l->a.val[1]));
    float pfx = l->a.val[0] + (tf*(l->b.val[0]-l->a.val[0]));
    float pfy = l->a.val[1] + (tf*(l->b.val[1]-l->a.val[1]));
    // printf("pox %f poy %f pfx %f pfy %f \n",p0x,p0y,pfx,pfy );
    // printf("t0 %f tf %f\n",t0,tf);

    point_set2D(&p0,p0x,p0y);
    point_set2D(&pf,pfx,pfy);

    line_set(&ret,p0,pf);
    // printf("pox %f poy %f pfx %f pfy %f \n",ret->a.val[0],ret->a.val[1],ret->b.val[0],ret->b.val[1] );
    return ret;

}

float max(float a, float b){
    if(a<b){
        return b;
    }
    else{
        return a;
    }
}

float min(float a, float b){
    if(a<b){
        return a;
    }
    else{
        return b;
    }
}




