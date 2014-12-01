//
//  color.c
//  
//
//  Created by Ryan Brandt on 9/22/14.
//
//

#include <stdlib.h>
#include <stdio.h>
#include "color.h"

//– copies the Color data.
void Color_copy(Color *to, Color *from){
    if (to==NULL||from ==NULL) {
        printf("ERROR: Color-copy >> pointer parameter is NULL\n");
        exit(-1);
    }
    *to = *from; // you can copy structuresd like this as long as there is no dynamic allocation
}
//– sets the Color data.
void Color_set(Color *to, float r, float g, float b){
    if (to==NULL) {
        printf("ERROR: Color-copy >> pointer parameter is NULL\n");
        exit(-1);
    }
    to->rgb[0]=r;
    to->rgb[1]=g;
    to->rgb[2]=b;
    
}
