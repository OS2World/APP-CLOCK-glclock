#ifndef __PNMIO_H__
#define __PNMIO_H__

#include <stdio.h>
#include "MString.H"

#define   RED_PIXEL( i, x, y ) ((i)->dataptr[((y) * (i)->width + (x)) * 3 + 0])
#define GREEN_PIXEL( i, x, y ) ((i)->dataptr[((y) * (i)->width + (x)) * 3 + 1])
#define  BLUE_PIXEL( i, x, y ) ((i)->dataptr[((y) * (i)->width + (x)) * 3 + 2])


typedef struct {
  unsigned short width, height;
  unsigned char *dataptr;
} TRUEIMAGE;


TRUEIMAGE *finputpnm( FILE *in );
TRUEIMAGE *finputpnmc( FILE *in, void (*)( TRUEIMAGE* i, int y ) );

char* outputpgm( TRUEIMAGE *timage, FILE *out );

// by Masaki Kawase
char* outputppm( TRUEIMAGE *timage, FILE *out, const String& comment = "" );

void freeimage( TRUEIMAGE *timage );


#endif
