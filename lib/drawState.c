#include "drawState.h"
#include <stdlib.h>
#include <stdio.h>
#include "module.h"
#include "line.h"
#include "point.h"
#include "polyline.h"
#include "polygon.h"
#include "circle.h"
#include "color.h"
#include "Image.h"



DrawState *drawstate_create( void ){ //–create a new DrawState structure and initialize the fields.
	// printf("drawstate\n");
	DrawState *state = malloc(sizeof(DrawState));
	Color white;
	Color_set(&white, 1.0,1.0,1.0);
	state->color = white;
	state->surfaceCoeff = 0;
	state->zBufferFlag = 1;
	return state;
}

void drawstate_setColor( DrawState *s, Color c ){ //– set the color field to c.
	if(s != NULL){
		s->color = c;
	}
}

void drawstate_setBody( DrawState *s, Color c ){ //– set the body field to c.
	if(s != NULL){
		s->body = c;
	}
}

void drawstate_setSurface( DrawState *s, Color c ){ //– set the surface field to c.
	if(s != NULL){
		s->surface = c;
	}
}

void drawstate_setSurfaceCoeff( DrawState *s, float f ){ //– set the surfaceCoeff field to f.
	if(s != NULL){
		s->surfaceCoeff = f;
	}
}

void drawstate_copy( DrawState *to, DrawState *from ){ //– copy the DrawState data
	if(to != NULL && from != NULL){
		*to = *from;
	}
	else{
		printf("problem in drawstate_copy");
	}
}



