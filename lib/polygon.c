/*
Erfan Azad
Date: 29 Sep 2014
file: polygon.h
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "list.h"
#include "polygon.h"
#include "line.h"
#include "point.h"
#include "Image.h"
#include "color.h"

/****************************************
Start Scanline Fill
*****************************************/
typedef struct tEdge {
    float x0, y0;                   /* start point for the edge */
    float x1, y1;                   /* end point for the edge */
    int yStart, yEnd;               /* start row and end row */
    float xIntersect, dxPerScan;    /* where the edge intersects the current scanline and how it changes */
    /* we'll add more here later */
    struct tEdge *next;
} Edge;



/*
    This is a comparison function that returns a value < 0 if a < b, a
    value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
    Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart( const void *a, const void *b ) { 
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;

    return(ea->yStart - eb->yStart);
}


/*
    This is a comparison function that returns a value < 0 if a < b, a
    value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
    Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect( const void *a, const void *b ) {
    Edge *ea = (Edge *)a;
    Edge *eb = (Edge *)b;

    if( ea->xIntersect < eb->xIntersect )
        return(-1);
    else if(ea->xIntersect > eb->xIntersect )
        return(1);

    return(0);
}

/*
    Allocates, creates, fills out, and returns an Edge structure given
    the inputs.

    Current inputs are just the start and end location in image space.
    Eventually, the points will be 3D and we'll add color and texture
    coordinates.
 */
static Edge *makeEdgeRec( Point start, Point end, Image *src)
{
    Edge *edge;
    //float dscan = end.val[1] - start.val[1]; //y1 - y0
    
    edge = (Edge*)malloc(sizeof(Edge));
    edge->x0 = start.val[0];
    edge->y0 = start.val[1];
    edge->x1 = end.val[0];
    edge->y1 = end.val[1];
    edge->yStart = (int)(edge->y0 + 0.5);
    edge->yEnd = (int)(edge->y1 + 0.5)-1; 
    edge->dxPerScan = (edge->x1 - edge->x0)/(edge->y1 - edge->y0);

    //Correctly initializing xIntersect
    edge->xIntersect = edge->x0 + abs((edge->y0 - edge->yStart )) * edge->dxPerScan;

    //Clipping if the edge starts off the image or goes off image
    if(edge->y0 < 0){ //if edge starts below row 0
        printf("edge starts above the image");
        edge->xIntersect += -edge->y0 * edge->dxPerScan;
        edge->y0 = 0;
    }
    if(edge->yEnd > src->rows-1){ // if the edge starts inside the image but continues outside
        edge->yEnd = src->rows-1;
    }


    // if(edge->xIntersect<edge->x1 && edge->dxPerScan<0){
    //     edge->xIntersect = edge->x1; 
    // }
    // if(edge->xIntersect>edge->x1 && edge->dxPerScan>0){
    //     edge->xIntersect = edge->x1; 
    // }

    edge->next = NULL;
    return( edge );
}


/*
    Returns a list of all the edges in the polygon in sorted order by
    smallest row.
*/
static LinkedList *setupEdgeList( Polygon *p, Image *src) {
    LinkedList *edges = NULL;
    Point v1, v2;
    int i;

    // create a linked list
    edges = ll_new();

    // walk around the polygon, starting with the last point
    v1 = p->vertex[p->numVertex-1];

    for(i=0;i<p->numVertex;i++) {
        
        // the current point (i) is the end of the segment
        v2 = p->vertex[i];

        // if it is not a horizontal line
        if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
            Edge *edge;

            // if the first coordinate is smaller (top edge)
            if( v1.val[1] < v2.val[1] )
                edge = makeEdgeRec( v1, v2, src );
            else
                edge = makeEdgeRec( v2, v1, src );

            // insert the edge into the list of edges if it's not null
            if( edge )

                ll_insert( edges, edge, compYStart );
        }
        v1 = v2;
    }

    // check for empty edges (like nothing in the viewport)
    if( ll_empty( edges ) ) {
        ll_delete( edges, NULL );
        edges = NULL;
    }

    return(edges);
}

/*
    Draw one scanline of a polygon given the scanline, the active edges,
    a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan( int scan, LinkedList *active, Image *src, Color c ) {
  Edge *p1, *p2;
  int i;

    // loop over the list
  p1 = ll_head( active );
  while(p1) {
        // the edges have to come in pairs, draw from one to the next
      p2 = ll_next( active );
      if( !p2 ) {
          printf("bad bad bad (your edges are not coming in pairs)\n");
          break;
      }

        // if the xIntersect values are the same, don't draw anything.
        // Just go to the next pair.
      if( p2->xIntersect == p1->xIntersect ) {
          p1 = ll_next( active );
          printf("good good good pairs \n" );
          continue;
      }

      if(p1->xIntersect < 0){ // if starts to draw before the left of the image
        p1->xIntersect = 0;
      }
      if(p2->xIntersect > src->cols){ // if ends drawing beyound the right side of the image
        p2->xIntersect = src->cols;
      }
      int colStart = (int)(p1->xIntersect);
      int colEnd = (int)(p2->xIntersect+1);
      int row = scan;
      for (i=colStart; i< colEnd; i++){
        image_setColor(src, row, i, c);
      }

        // move ahead to the next pair of edges
      p1 = ll_next( active );
  }

    return;
}


/*
 Draw one scanline of a polygon given the scanline, the active edges,
 a DrawState, the image, and some Lights (for Phong shading only) using a bitmap
 */
static void fillScanBitmap( int scan, LinkedList *active, Image *src,Image *bitmap ) {
    Edge *p1, *p2;
    int i;
    
    // loop over the list
    p1 = ll_head( active );
    while(p1) {
        // the edges have to come in pairs, draw from one to the next
        p2 = ll_next( active );
        if( !p2 ) {
            printf("bad bad bad (your edges are not coming in pairs)\n");
            break;
        }
        
        // if the xIntersect values are the same, don't draw anything.
        // Just go to the next pair.
        if( p2->xIntersect == p1->xIntersect ) {
            p1 = ll_next( active );
            printf("good good good pairs \n" );
            continue;
        }
        
        /**** Your code goes here ****/
        if(p1->xIntersect < 0){
            p1->xIntersect = 0;
        }
        if(p2->xIntersect > src->cols){
            p2->xIntersect = src->cols;
        }
        int colStart = (int)(p1->xIntersect);
        int colEnd = (int)(p2->xIntersect+1);
        int row = scan;
        
        
        int incrementor=0;
        int bitmapRow=0;
        
        
        for (i=colStart; i< colEnd; i++){
            incrementor++;
            if (incrementor % 8 == 0) {
                bitmapRow++;
            }
            if (bitmapRow>bitmap->rows-1) {
                bitmapRow=0;
            }
            
            image_setf(src, row, i, bitmap->data[bitmapRow][incrementor]);
        }
        // identify the starting column
        // clip to the left side of the image
        // identify the ending column
        // clip to the right side of the image
        // loop from start to end and color in the pixels
        
        // move ahead to the next pair of edges
        p1 = ll_next( active );
    }
    
    return;
}

/* 
     Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList( LinkedList *edges, Image *src, Image *bitmap ,Color c ) {
    LinkedList *active = NULL;
    LinkedList *tmplist = NULL;
    LinkedList *transfer = NULL;
    Edge *current;
    Edge *tedge;
    int scan = 0;

    active = ll_new( );
    tmplist = ll_new( );
//
//    // get a pointer to the first item on the list and reset the current pointer
    current = ll_head( edges );
    // start at the first scanline and go until the active list is empty
    for(scan = current->yStart;scan < src->rows;scan++ ) {

        // grab all edges starting on this row
        while( current != NULL && current->yStart == scan ) {
            ll_insert( active, current, compXIntersect );
            current = ll_next( edges );
        }
//        // current is either null, or the first edge to be handled on some future scanline
//
        if( ll_empty(active) ) {
            break;
        }
//
////        checks to see if a bitmap has been added, defaults to regular scan if not
        if (bitmap == NULL) {
            fillScan( scan, active, src, c);
        }
//
////        if so, bitmap scan
        else{
            fillScanBitmap( scan, active, src, bitmap);
        }
//
//        // remove any ending edges and update the rest
        for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {

            // keep anything that's not ending
            if( tedge->yEnd > scan ) {
                float a = 1.0;

                // update the edge information with the dPerScan values
                tedge->xIntersect += tedge->dxPerScan;

                // adjust in the case of partial overlap
                if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
                    a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
                    tedge->xIntersect = tedge->x1;
                }
                else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
                    a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
                    tedge->xIntersect = tedge->x1;
                }

                ll_insert( tmplist, tedge, compXIntersect );
            }
        }
//
        transfer = active;
        active = tmplist;
        tmplist = transfer;

    }

    // get rid of the lists, but not the edge records
    ll_delete(active, NULL);
    ll_delete(tmplist, NULL);

    return(0);
}


/****************************************
End Scanline Fill
*****************************************/


// returns an allocated Polygon pointer initialized so that numVertex is 0 and vertex is NULL.
Polygon *polygon_create(){
	Polygon *p=malloc(sizeof(Polygon));
    if(NULL == p){
        printf("ERROR: polygon_create >> malloc failed!\n");
        exit(-1);
    }
    p->vertex=NULL;
    p->numVertex=0;
    p->zBuffer = 1;
    return p;
}

// returns an allocated Polygon pointer with the vertex list
//initialized to a copy of the points in vlist.
Polygon *polygon_createp(int numV, Point *vlist){
	Polygon *p=malloc(sizeof(Polygon));
    if(NULL == p){
        printf("ERROR: polygon_createp >> malloc failed!\n");
        exit(-1);
    }
    p->vertex=malloc(sizeof(Point)*numV);
    for (int i = 0; i < numV; i++){
        point_copy(&(p->vertex[i]),&vlist[i]);
    }
    p->numVertex=numV;
    p->zBuffer = 1;
    return p;
}

// frees the internal data for a Polygon and the Polygon pointer
void polygon_free(Polygon *p){
	if (p!=NULL) {
        if (p->vertex!=NULL) {
            free(p->vertex);
        }
        free(p);
    }
}

//initializes the existing Polygon to an empty Polygon.
void polygon_init(Polygon *p){
	p->vertex=NULL;
    p->numVertex=0;
    p->zBuffer = 1;
}

// initializes the vertex array to the points in vlist
void polygon_set(Polygon *p, int numV, Point *vlist){
	if (p->vertex!=NULL) {
        free(p->vertex);
    }
    p->vertex=malloc(sizeof(Point)*numV);
    for (int i = 0; i < numV; i++)
    {
        point_copy(&(p->vertex[i]),&vlist[i]);
    }
    p->numVertex = numV;
    p->zBuffer = 1;
}

// frees the internal data and resets the fields.
void polygon_clear(Polygon *p){
	if(p->vertex != NULL){
        free(p->vertex);
    }
    p->vertex = NULL;
    p->numVertex = 0;
    p->zBuffer = 1;
}

//sets the z-buffer flag to the given value
void polygon_zBuffer(Polygon *p, int flag){
	if(NULL != p){
        p->zBuffer = flag;
    }
}

//De-allocates/allocates space and copies the 
//vertex and color data from one polygon to the other
void polygon_copy(Polygon *to, Polygon *from){
	if(NULL != to && NULL != from){
        polygon_clear(to);
        to->vertex = malloc(sizeof(Point)*from->numVertex);
        int i;
        for (i=0; i < from->numVertex; i++)
        {
          to->vertex[i] = from->vertex[i];
        }
        to->numVertex = from->numVertex;
        to->zBuffer = from->zBuffer;
    }
}

// prints polygon data to the stream designated by the FILE pointer
void polygon_print(Polygon *p, FILE *fp){
	if(NULL != p && NULL != fp){
        int i;
        fprintf(fp, "%d\n",p->numVertex);//writes number of vertecies
        fprintf(fp, "%d\n",p->zBuffer);//writes the zBuffer value
        for(i=0; i<p->numVertex; i++){
            fprintf(fp, "%.3f %.3f %.3f %.3f\n",p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
        }
    }
}

// draw the outline of the polygon using color c.
void polygon_draw(Polygon *p, Image *src, Color colr){
	Line l;
    int i;
    for (i=0; i< p->numVertex-1; i++){
        line_set2D(&l,p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i+1].val[0], p->vertex[i+1].val[1]);
        line_draw(&l, src, colr);
    }
    line_set2D(&l, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[0].val[0], p->vertex[0].val[1]);
    line_draw(&l, src, colr);
}

/*
    Draws a filled polygon of the specified color into the image src.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c ) {
    LinkedList *edges = NULL;

    // set up the edge list
    edges = setupEdgeList( p, src );
    if( !edges )
        return;
    
    // process the edge list (should be able to take an arbitrary edge list)
    processEdgeList( edges, src, NULL, c);

    // clean up
    ll_delete( edges, (void (*)(const void *))free );

    return;
}


/*
 Draws a filled polygon of the specified color into the image src, allows for bitmap texture insert
 */
void polygon_drawFillBitmap(Polygon *p, Image *src, Image *bitmap ) {
    LinkedList *edges = NULL;
    
//    passing unitialized pointer, will not be used
    Color c;
    
    // set up the edge list
    edges = setupEdgeList( p, src );
    if( !edges )
        return;
    // process the edge list (should be able to take an arbitrary edge list)
    processEdgeList( edges, src, bitmap,c);
    
    // clean up
    ll_delete( edges, (void (*)(const void *))free );
    
    return;
}

/*
Auxilary function to be used with qsort
*/
int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

//draw the filled polygon using color c with the
//Barycentric coordinates algorithm.
void polygon_drawFillB(Polygon *p, Image *src, Color c){
	const float epsilon = 0.00001;
	float alpha, beta, gamma;
	float xlist[3] = {p->vertex[0].val[0], p->vertex[1].val[0], p->vertex[2].val[0]};
	float ylist[3] = {p->vertex[0].val[1], p->vertex[1].val[1], p->vertex[2].val[1]};
	qsort(xlist, 3, sizeof(float), compare);
	qsort(ylist, 3, sizeof(float), compare);
	int Xmin = (int)(xlist[0]+0.5);
	int Ymin = (int)(ylist[0]+0.5);
	int Xmax = (int)(xlist[2]+0.5);
	int Ymax = (int)(ylist[2]+0.5);

	for (int i = Ymin; i < Ymax; i++){
		for (int j = Xmin; j < Xmax; j++){
			beta = Aux_implicit_line(p->vertex[0], p->vertex[2], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[2],p->vertex[1].val[0], p->vertex[1].val[1]);
			gamma = Aux_implicit_line(p->vertex[0], p->vertex[1], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[1],p->vertex[2].val[0], p->vertex[2].val[1]);
			alpha = 1- beta - gamma; 

			if (beta<-epsilon || gamma <-epsilon || alpha<-epsilon){			
				continue;
			}
			else{
				image_setColor(src,i,j,c);
			}
		}
	}
}


/*
Draws a filled polygon (triangle) with color gradiant using Barycentric coordinates algorithm.
Parameters:
p : pointer to a polygon structure
src : pointer to an image structure
c1, c2, c3: Are color structures that will set the color of each corner of the triangle
*/
void polygon_drawFillB_Gradient(Polygon *p, Image *src, Color c1 ,Color c2, Color c3){
    const float epsilon = 0.00001;
    float alpha, beta, gamma;
    float xlist[3] = {p->vertex[0].val[0], p->vertex[1].val[0], p->vertex[2].val[0]};
    float ylist[3] = {p->vertex[0].val[1], p->vertex[1].val[1], p->vertex[2].val[1]};
    qsort(xlist, 3, sizeof(float), compare);
    qsort(ylist, 3, sizeof(float), compare);
    int Xmin = (int)(xlist[0]+0.5);
    int Ymin = (int)(ylist[0]+0.5);
    int Xmax = (int)(xlist[2]+0.5);
    int Ymax = (int)(ylist[2]+0.5);

    
    for (int i = Ymin; i < Ymax; i++){
        for (int j = Xmin; j < Xmax; j++){
            beta = Aux_implicit_line(p->vertex[0], p->vertex[2], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[2],p->vertex[1].val[0], p->vertex[1].val[1]);
            gamma = Aux_implicit_line(p->vertex[0], p->vertex[1], j,i)/ Aux_implicit_line(p->vertex[0], p->vertex[1],p->vertex[2].val[0], p->vertex[2].val[1]);
            alpha = 1- beta - gamma;
            
            if (beta<-epsilon || gamma <-epsilon || alpha<-epsilon){ //if outside of the triangle
                    continue;
            }
            else{ // if inside of the thriangle
                float im_red =(alpha*c1.rgb[0]) + (beta*c2.rgb[0]) + (gamma*c3.rgb[0]);
                float im_green =(alpha*c1.rgb[1]) + (beta*c2.rgb[1]) + (gamma*c3.rgb[1]);
                float im_blue =(alpha*c1.rgb[2]) + (beta*c2.rgb[2]) + (gamma*c3.rgb[2]);
                
                image_setc(src,i,j,0,im_red);
                image_setc(src,i,j,1,im_green);
                image_setc(src,i,j,2,im_blue);
            }
        }
    }
}

/*
Auxilary function to calculate the distance of point p_in from
the line segment p1p2
*/

float Aux_implicit_line(Point p1, Point p2, float x,float y){
	float x0=p1.val[0];
	float y0=p1.val[1];

	float x1=p2.val[0];
	float y1=p2.val[1];
	x += 0.5;
	y += 0.5;
	return (y0-y1)*x + (x1-x0)*y + x0*y1 - x1*y0;
}

/*
    normalizes the vectors of the polygon
*/
void polygon_normalize(Polygon *p){
    int i;
    for(i=0;i<p->numVertex;i++){
        p->vertex[i].val[0]/=p->vertex[i].val[3];
        p->vertex[i].val[1]/=p->vertex[i].val[3];
    }
}









