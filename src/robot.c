/*
Ryan Brandt
creates never-ending looping walking robot
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
	int frame;
	Color red,white;

	Point side[16];
	Point p[4];
	Point face[16];
	Point appendage[16];
	Point leg[16];
	Polygon poly;
	BezierSurface bc;
	DrawState ds;
	Module *curveBody;
	Module *quad;
	Module *scene;
	Module *body;
	Module *head;
	Module *appendageMod;
	Module *upperBody;
	Module *conningTower;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}
	printf("Creating Bezier curves with %d subdivisions\n", divisions);

	Color_set(&white, 1.0, 1.0, 1.0 );
	Color_set(&red, 1, 0, 0 );

	// set up the drawstate
	drawstate_setColor(&ds, white);

// body and head mapping
	point_set3D(&side[0],  0,  0.0000, 0 );
	point_set3D(&side[1],  1, 0, 0 );
	point_set3D(&side[2], 2, 0 , 0 );
	point_set3D(&side[3], 3, 0, 0);
	point_set3D(&side[4],  0,  1.5, 1.5 );
	point_set3D(&side[5],  1,1.5, 1.5 );
	point_set3D(&side[6],  2, 1.5, 1.5);
	point_set3D(&side[7],  3, 1.5, 1.5);
	point_set3D(&side[8],  0,  3, 1.5 );
	point_set3D(&side[9],  1, 3, 1.5 );
	point_set3D(&side[10],  2, 3, 1.5 );
	point_set3D(&side[11],  3, 3, 1.5 );
	point_set3D(&side[12],  0, 4.5, 0 );
	point_set3D(&side[13],  1, 4.5, 0 );
	point_set3D(&side[14],  2,  4.5, 0 );
	point_set3D(&side[15],  3, 4.5, 0 );

// face mapping
	point_set3D(&face[0],  0,  .5, 3 );
	point_set3D(&face[1],  -.2, 1, 3 );
	point_set3D(&face[2], -.5, 2 , 3 );
	point_set3D(&face[3], -.5, 3, 3);
	point_set3D(&face[4],  1,  .5, 3 );
	point_set3D(&face[5],  1,1, 0 );
	point_set3D(&face[6],  1, 2, 0);
	point_set3D(&face[7],  1, 3, 0);
	point_set3D(&face[8],  2,  .5, 3 );
	point_set3D(&face[9],  2, 1, 0 );
	point_set3D(&face[10],  2, 2, 0 );
	point_set3D(&face[11],  2, 3, 3 );
	point_set3D(&face[12],  3, .5, 3 );
	point_set3D(&face[13],  3.2, 1, 3 );
	point_set3D(&face[14],  3.5,  2, 3 );
	point_set3D(&face[15],  3.5, 3, 3 );

// appendage mapping
	point_set3D(&appendage[0],  0,  0, 0 );
	point_set3D(&appendage[1],  0, 0, 1 );
	point_set3D(&appendage[2], 0, 0 , 2 );
	point_set3D(&appendage[3], 0, 0, 3);
	point_set3D(&appendage[4],  1,  0, 0 );
	point_set3D(&appendage[5],  1,0, 1 );
	point_set3D(&appendage[6],  1, 0, 2);
	point_set3D(&appendage[7],  1, 0, 3);
	point_set3D(&appendage[8],  1,  1, 0 );
	point_set3D(&appendage[9],  1, 1, 1 );
	point_set3D(&appendage[10],  1, 1, 2 );
	point_set3D(&appendage[11],  1, 1, 3 );
	point_set3D(&appendage[12],  0, 1, 0 );
	point_set3D(&appendage[13],  0, 1, 1 );
	point_set3D(&appendage[14],  0,  1, 2 );
	point_set3D(&appendage[15],  0, 1, 3 );


	point_set3D(&leg[0],  0,  0, 0 );
	point_set3D(&leg[1],  0, 0, 1.5 );
	point_set3D(&leg[2], 0, 0 , 3 );
	point_set3D(&leg[3], 0, 0, 4.5);
	point_set3D(&leg[4],  1,  0, 0 );
	point_set3D(&leg[5],  1,0, 1.5 );
	point_set3D(&leg[6],  1, 0, 3);
	point_set3D(&leg[7],  1, 0, 4.5);
	point_set3D(&leg[8],  1,  1, 0 );
	point_set3D(&leg[9],  1, 1, 1.5 );
	point_set3D(&leg[10],  1, 1, 3 );
	point_set3D(&leg[11],  1, 1, 4.5 );
	point_set3D(&leg[12],  0, 1, 0 );
	point_set3D(&leg[13],  0, 1, 1.5 );
	point_set3D(&leg[14],  0,  1, 3 );
	point_set3D(&leg[15],  0, 1, 4.5 );

	point_set3D(&p[0],  0, 0, 0 );
	point_set3D(&p[1],  3, 0, 0 );
	point_set3D(&p[2],  3, 0, -1 );
	point_set3D(&p[3],  0,  0, -1 );
	polygon_init(&poly);

	bezierSurface_set(&bc,side);

	curveBody = module_create();
	module_bezierSurface(curveBody,&bc,divisions, 0);

	body = module_create();

	module_module(body,curveBody);

	module_scale(body, 1,1,-1);
	module_translate(body,0,0,-1);
	module_module(body,curveBody);
	polygon_set(&poly,4,p);
	module_identity(body);
	module_polygon(body,&poly);
	module_translate(body,0,4.5,0);
	module_polygon(body,&poly);

	head = module_create();
	module_scale(head,.5,.5,.5);
	module_rotateY(head,cos(M_PI/2),sin(M_PI/2));
	module_module(head,body);
	// module_identity(head);
	bezierSurface_set(&bc,face);
	module_rotateY(head,cos(M_PI/2),sin(M_PI/2));
	module_scale(head,.75,.75,.75);
	module_translate(head,.3,.25,1);
	module_bezierSurface(head,&bc,divisions,0);
	module_color(head,&red);
	module_rotateX(head,cos(M_PI/2),sin(M_PI/2));
	// module_scale(head,.5,.5,.5);
	module_translate(head,-.75,2,-.25);
	module_cylinder(head,8);
	module_translate(head,.75,0,0);
	module_cylinder(head,8);

	appendageMod = module_create();
	bezierSurface_set(&bc,appendage);
	module_bezierSurface(appendageMod,&bc,divisions,0);
	module_scale(appendageMod,-1,1,1);
	module_translate(appendageMod,.25,0,0);
	module_bezierSurface(appendageMod,&bc,divisions,0);

	quad = module_create();
	bezierSurface_set(&bc,leg);
	module_bezierSurface(quad,&bc,divisions,0);
	module_scale(quad,-1,1,1);
	module_translate(quad,.25,0,0);
	module_bezierSurface(quad,&bc,divisions,0);


	upperBody = module_create();
	module_module(upperBody,body);
	module_translate(upperBody,1.5,4.5,.5);
	module_module(upperBody,head);
	module_identity(upperBody);







	vector_set( &(view.vup), 0.0, 1, 0);
	view.d = 1.5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;
	matrix_identity( &GTM );

	Point center;
	point_set3D(&center, 0, 5,0 );

	float coefLeg1 = 90;
	int leg1Down = 1;
	int leg2Down = 0;
	float coefLeg2 = 90;


	float coefArm1 = 0;
	int arm1Down = 1;
	int arm2Down = 0;
	float coefArm2 = 0;
	view_rotate_circle(&poly,&center,45,5,-45,-45,-45);
int increment = 0;

for(frame=0;frame<90;frame++) {
		char buffer[256];
		if(increment>44){
			increment = 0;
		}

		// modifying the view
		point_copy(&(view.vrp),&poly.vertex[increment] );
		vector_set( &(view.vpn), -view.vrp.val[0], -view.vrp.val[1]+1,-view.vrp.val[2] );
		matrix_setView3D( &VTM, &view );
		increment++;	


		// creating scene and rotating appendages
		scene = module_create();
		module_translate(scene,0,0,1);
		module_module(scene,upperBody);
		module_identity(scene);
		// leg one
		module_rotateX(scene,cos(coefLeg1*M_PI/180),sin(coefLeg1*M_PI/180));
		if(coefLeg1>135 && leg1Down == 0){
			leg1Down = 1;
		}
		if(coefLeg1<45 && leg1Down == 1){
			leg1Down = 0;
		}
		if(leg1Down == 1){
			coefLeg1-=2;
		}
		if(leg1Down == 0){
			coefLeg1+=2;
		}
		module_translate(scene,.5,0,0);
		module_module(scene, quad);
		module_identity(scene);


		module_rotateX(scene,cos(coefLeg2*M_PI/180),sin(coefLeg2*M_PI/180));
		if(coefLeg2>135 && leg2Down == 0){
			leg2Down = 1;
		}
		if(coefLeg2<45 && leg2Down == 1){
			leg2Down = 0;
		}
		if(leg2Down == 1){
			coefLeg2-=2;
		}
		if(leg2Down == 0){
			coefLeg2+=2;
		}
		module_translate(scene,2.5,0,0);
		module_module(scene, quad);

// arms
		module_identity(scene);
		module_rotateY(scene,-cos(M_PI/2),-sin(M_PI/2));
		module_rotateZ(scene,cos(45*M_PI/180),sin(45*M_PI/180));
		module_rotateX(scene,cos(coefArm1*M_PI/180),sin(coefArm1*M_PI/180));
		module_translate(scene,.5,3,.5);


		if(coefArm1<-45 && arm1Down == 1){
			arm1Down = 0;
		}
		if(coefArm1>45 && arm1Down == 0){
			arm1Down = 1;
		}
		if(arm1Down == 1){
			coefArm1-=2;
		}
		if(arm1Down == 0){
			coefArm1+=2;
		}
		module_module(scene,appendageMod);



		module_identity(scene);
		module_rotateY(scene,-cos(M_PI/2),-sin(M_PI/2));
		module_rotateZ(scene,cos(45*M_PI/180),sin(45*M_PI/180));
		module_rotateX(scene,cos(coefArm2*M_PI/180),sin(coefArm2*M_PI/180));
		module_translate(scene,.5,3,.5);
		module_scale(scene,-1,1,1);
		module_translate(scene,3,0,0);


		if(coefArm2<-45 && arm2Down == 1){
			arm2Down = 0;
		}
		if(coefArm2>45 && arm2Down == 0){
			arm2Down = 1;
		}
		if(arm2Down == 1){
			coefArm2-=2;
		}
		if(arm2Down == 0){
			coefArm2+=2;
		}
		module_module(scene,appendageMod);

		module_draw( scene, &VTM, &GTM, &ds, src );

		sprintf(buffer, "robot-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);3
	}


	// clean up
	image_free( src );

	module_delete( scene );

	polygon_clear(&poly);

	return(0);
}