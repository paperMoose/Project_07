/*
Erfan Azad
Date: 23 Oct 2014
File: erf_scene2d.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "line.h"
#include "view.h"
#include "color.h"
#include "circle.h"

int main(int argc, char* argv[]){
	const int rows = 1000;
	const int cols = 1000;
	View2D view;
	Matrix vtm;
	Matrix ltm;
	Line line[5];
	Line tLine;
	Circle head;
	double R_head;
	Point p[8];
	Point tcenter;
	Color color[4];
	int i;
	char filename[256];
	Image *src;

	//setting the colors for the scene
	Color_set(&color[0], 0.56, 0.93, 0.56);//green
	Color_set(&color[1], 0.80, 0.52, 0.24);//brown
	Color_set(&color[2], 1.0, 1.0, 1.0);//white
	Color_set(&color[3], 0.0, 0.0, 0.0);//black

	//setting the points
	point_set2D(&p[0], 500, 500);//head center
	point_set2D(&p[1], 500, 550);//body start
	point_set2D(&p[2], 500, 620);//body end
	point_set2D(&p[3], 500, 540);//first split
	point_set2D(&p[4], 530, 500);//right hand
	point_set2D(&p[5], 470, 500);//left hand
	point_set2D(&p[6], 520, 660);//right leg
	point_set2D(&p[7], 480, 660);//left leg

	//setting the line using points
	line_set(&line[0], p[1], p[2]);//body
	line_set(&line[1], p[3], p[4]);//right hand
	line_set(&line[2], p[3], p[5]);//left hand
	line_set(&line[3], p[2], p[6]);//right leg
	line_set(&line[4], p[2], p[7]);//left leg

	//setting up the view
	point_set2D( &(view.vrp), 500, 500);
	view.dx = 1000.0;
	vector_set( &(view.x), 1.0, 0.0, 0.0);
	view.screenx = cols;
  	view.screeny = rows;
  	matrix_setView2D( &vtm, &view );

  	src = image_create( rows, cols );
  	image_fillrgb(src, 1.0, 1.0, 1.0); // white back ground

  	for(i=0; i<2; i++){
  		if(i==0){
  			point_copy(&tcenter, &p[0]);
  			//setting the circle
			R_head = 50;
			circle_set(&head, tcenter, R_head);

			matrix_xformPoint(&vtm, &tcenter, &tcenter);
			circle_draw(&head, src, color[3]);
  		}
  		line_copy(&tLine, &line[i]);
  		matrix_xformLine(&vtm, &tLine);
  		line_draw(&tLine,src, color[3]);
  	}

  	printf("writing the image...\n");
    image_write( src, "stickman.ppm" );

	



	//cleanup
	image_free(src);

}