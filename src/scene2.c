/*
Ryan Brandt
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "module.h"
#include "drawState.h"
#include "line.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "circle.h"
#include "color.h"
#include "Image.h"
#include "view.h"
#include "bezier.h"
#include "matrix.h"



int main(int argc, char *argv[]) {
	Module *nose;
	Module *body;
	Module *Scene;
	Module *tail;
	Module *rotor;
	Module *Conningtower;
	BezierSurface bc;
	Point nosePoints[16];
	Point body1[16];
	Point tail1[16];
	Point rotor1[16];
	Point gradientP1[3];
	Point gradientP2[3];
	Point towerP[4];
	Point towerP2[5];
	View3D view;
	Matrix VTM, GTM;
	int rows = 500, cols = 500;
	Image *src = image_create(rows, cols);
	DrawState ds;
	Color red,white,topBlue,bottomBlue;
	Polygon *gradient1;
	Polygon *gradient2;
	Polyline *towerL2;
	Polyline *towerL;
	int division = 1;



	// color setting
	Color_set(&white, 1.0, 1.0, 1.0 );
	Color_set(&red, 1, 0, 0 );
	Color_set(&topBlue, 80.0/255.0, 177.0/255.0, 242.0/255.0 );
	Color_set(&bottomBlue, 35.0/255.0, 64.0/255.0, 255.0/255.0 );
	drawstate_setColor(&ds, white);
	image_fillrgb(src,17/255,50/255,240/255);

// background setting
	point_set2D(&gradientP1[0],0,0);
	point_set2D(&gradientP1[1],500,0);
	point_set2D(&gradientP1[2], 0,500);

	point_set2D(&gradientP2[0],500,500);
	point_set2D(&gradientP2[1],500,0);
	point_set2D(&gradientP2[2], 0,500);

// tower points
	point_set3D(&towerP[0],0,0,0);
	point_set3D(&towerP[1],0,1,0);
	point_set3D(&towerP[2], 1,1,0);
	point_set3D(&towerP[3], 1,0,0);

	point_set3D(&towerP2[0],0,1,0);
	point_set3D(&towerP2[1],1,1,0);
	point_set3D(&towerP2[2], 1,1,2);
	point_set3D(&towerP2[3], 0,1,2);
	point_set3D(&towerP2[4], 0,1,0);

	gradient1 = polygon_createp(3,gradientP1);
	gradient2 = polygon_createp(3,gradientP2);
	towerL2 = polyline_createp(5,towerP2);
	towerL =	polyline_createp(4,towerP);



	// points of the nose
	point_set3D(&nosePoints[0],0,0,0);
	point_set3D(&nosePoints[1],.25,0,0);
	point_set3D(&nosePoints[2],.5, 0,0);
	point_set3D(&nosePoints[3],.75,0,0);
	point_set3D(&nosePoints[4],0,.25,.25);
	point_set3D(&nosePoints[5],.25,.166,.25);
	point_set3D(&nosePoints[6],.5, .0833,.25);
	point_set3D(&nosePoints[7],.75,0,.25);
	point_set3D(&nosePoints[8],0,.5,.5);
	point_set3D(&nosePoints[9],.25,.332,.5);
	point_set3D(&nosePoints[10],.5, .166,.5);
	point_set3D(&nosePoints[11],.75,0,.5);
	point_set3D(&nosePoints[12],0,.75,.75);
	point_set3D(&nosePoints[13],.25,.5,.75);
	point_set3D(&nosePoints[14],.5, .25,.75);
	point_set3D(&nosePoints[15],.75,0,.75);


	// body
	point_set3D(&body1[0],0,.75,.75);
	point_set3D(&body1[1],.25,.5,.75);
	point_set3D(&body1[2],.5, .25,.75);
	point_set3D(&body1[3],.75,0,.75);

	point_set3D(&body1[4],0,.75,.1);
	point_set3D(&body1[5],.35,.5,1.25);
	point_set3D(&body1[6],.7, .5,1.5);
	point_set3D(&body1[7],.75,0,1.75);

	point_set3D(&body1[8],0,.75,2);
	point_set3D(&body1[9],.35,.5,2.25);
	point_set3D(&body1[10],.7, .5,2.5);
	point_set3D(&body1[11],.75,0,2.75);

	point_set3D(&body1[12],0,.75,3.75);
	point_set3D(&body1[13],.25,.5,3.75);
	point_set3D(&body1[14],.5, .5,3.75);
	point_set3D(&body1[15],.75,0,3.75);


	// tail
	point_set3D(&tail1[0],0,.75,.75);
	point_set3D(&tail1[1],.25,.5,.75);
	point_set3D(&tail1[2],.5, .5,.75);
	point_set3D(&tail1[3],.75,0,.75);

	point_set3D(&tail1[4],0,.7,.1);
	point_set3D(&tail1[5],.25,.45,1.25);
	point_set3D(&tail1[6],.5, .45,1.5);
	point_set3D(&tail1[7],.5,0,1.75);

	point_set3D(&tail1[8],0,.5,2);
	point_set3D(&tail1[9],.2,.25,2.25);
	point_set3D(&tail1[10],.45, .25,2.5);
	point_set3D(&tail1[11],.45,0,2.75);

	point_set3D(&tail1[12],0,.25,3.75);
	point_set3D(&tail1[13],.1,.125,3.75);
	point_set3D(&tail1[14],.15, .0625,3.75);
	point_set3D(&tail1[15],.2,0,3.75);

	// rotor
	point_set3D(&rotor1[0],0,0,0);
	point_set3D(&rotor1[1],.09,0,0);
	point_set3D(&rotor1[2],.18, 0,0);
	point_set3D(&rotor1[3],.27,0,0);

	point_set3D(&rotor1[4],0,.25,0.25);
	point_set3D(&rotor1[5],.09,0.25,.25);
	point_set3D(&rotor1[6],.18, 0.25,.25);
	point_set3D(&rotor1[7],.27,0.25,.25);

	point_set3D(&rotor1[8],0,0.25,0.5);
	point_set3D(&rotor1[9],.09,0.25,.5);
	point_set3D(&rotor1[10],.18, 0.25,.5);
	point_set3D(&rotor1[11],.27,0.25,.5);

	point_set3D(&rotor1[12],0,0.27,.75);
	point_set3D(&rotor1[13],.09,0.27,0.75);
	point_set3D(&rotor1[14],.18, 0.27,.75);
	point_set3D(&rotor1[15],.27,0.27,.75);


	bezierSurface_set(&bc,nosePoints);

	nose = module_create();
	module_bezierSurface(nose,&bc,division, 0);
	module_scale(nose,1,-1,1);
	module_bezierSurface(nose,&bc,division, 0);
	module_scale(nose,-1,1,1);
	module_bezierSurface(nose,&bc,division, 0);
	module_scale(nose,1,-1,1);
	module_bezierSurface(nose,&bc,division, 0);



	body = module_create();
	bezierSurface_set(&bc,body1);
	module_bezierSurface(body,&bc,division, 0);
	module_scale(body,1,-1,1);
	module_bezierSurface(body,&bc,division, 0);
	module_scale(body,-1,1,1);
	module_bezierSurface(body,&bc,division, 0);
	module_scale(body,1,-1,1);
	module_bezierSurface(body,&bc,division, 0);


	tail = module_create();
	bezierSurface_set(&bc,tail1);
	module_bezierSurface(tail,&bc,division, 0);
	module_scale(tail,1,-1,1);
	module_bezierSurface(tail,&bc,division, 0);
	module_scale(tail,-1,1,1);
	module_bezierSurface(tail,&bc,division, 0);
	module_scale(tail,1,-1,1);
	module_bezierSurface(tail,&bc,division, 0);

	rotor = module_create();
	module_scale(rotor,.5,2,1);
	bezierSurface_set(&bc,rotor1);
	module_bezierSurface(rotor,&bc,division, 0);
	module_rotateZ(rotor,cos(M_PI/2),sin(M_PI/2));
	module_translate(rotor,0,-.15,0);
	module_bezierSurface(rotor,&bc,division, 0);
	module_translate(rotor,0,.15,0);
	module_rotateZ(rotor,cos(M_PI/2),sin(M_PI/2));
	module_translate(rotor,.15,-.15,0);
	module_bezierSurface(rotor,&bc,division, 0);
	module_translate(rotor,-.15,0,0);
	module_rotateZ(rotor,cos(M_PI/2),sin(M_PI/2));
	module_bezierSurface(rotor,&bc,division, 0);

	Conningtower = module_create();
	module_polyline(Conningtower,towerL);
	module_translate(Conningtower,0,0,2);
	module_polyline(Conningtower,towerL);
	module_identity(Conningtower);
	module_polyline(Conningtower,towerL2);



	Scene = module_create();
	module_module(Scene,nose);
	module_module(Scene,body);
	module_translate(Scene,0,0,2.95);
	module_module(Scene,body);
	module_translate(Scene,0,0,2.95);
	module_module(Scene,body);
	module_translate(Scene,0,0,2.95);
	module_module(Scene,tail);


	vector_set( &(view.vup), 0.0, 1, 0);
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	point_set3D(&(view.vrp),10,0,2);
	vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1],-view.vrp.val[2] );
	// matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );
	int frame;
	// point_set3D(&center, 4, 0,4 );
	// initial fill
	polygon_drawFillB_Gradient(gradient1,src,topBlue,topBlue,bottomBlue);
	polygon_drawFillB_Gradient(gradient2,src,bottomBlue,topBlue,bottomBlue);
	for(frame=0;frame<225;frame++) {

		Scene = module_create();
		module_translate(Scene,0,0,5);
		module_translate(Scene,0,0,-frame*.1);
		module_module(Scene,nose);
		module_translate(Scene,0,.5,2);
		module_module(Scene,Conningtower);
		module_translate(Scene,0,-.5,-2);
		module_module(Scene,body);
		module_translate(Scene,0,0,2.95);
		module_module(Scene,body);
		module_translate(Scene,0,0,2.95);
		module_module(Scene,body);
		module_translate(Scene,0,0,2.95);
		module_module(Scene,tail);
		module_rotateZ(Scene,cos(frame*5*M_PI/180),sin(frame*5*M_PI/180));
		module_translate(Scene,0,0,2.95);
		module_module(Scene,rotor);

		char buffer[256];
		// point_copy(&(view.vrp),&poly.vertex[frame] );
		// vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1],-view.vrp.val[2] );
		matrix_setView3D( &VTM, &view );
		module_draw( Scene, &VTM, &GTM, &ds, src );
		sprintf(buffer, "scene2-frame%03d.ppm", frame);
		image_write(src, buffer);
		polygon_drawFillB_Gradient(gradient1,src,topBlue,topBlue,bottomBlue);
		polygon_drawFillB_Gradient(gradient2,src,bottomBlue,topBlue,bottomBlue);

	}



	image_free( src );

	polygon_free(gradient1);
	polygon_free(gradient2);
	polyline_free(towerL);
	polyline_free(towerL2);
	return(0);
}