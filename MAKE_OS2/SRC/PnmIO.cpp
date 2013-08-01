#include <stdlib.h>
#include <ctype.h>

#include "PnmIO.H"


static int tokenize( FILE *fp, char *token, int n )
{
  int c;
  int end;
  int count = 0;

  c = 0;
  do {
    if ( c != '#' ) c = fgetc( fp );
    if ( c == '#' ) while ( !(end = feof( fp )) && (c = fgetc( fp )) != '\n' );
    if ( isspace( c ) ) while ( !(end = feof( fp )) && isspace( c = fgetc( fp ) ) );
  } while ( !(end = feof( fp )) && (isspace( c ) || c == '#') );

  if ( !end ) {
    do {
      token[count++] = c;
      if ( count == n - 1 || feof( fp ) ) break;
      c = fgetc( fp );
    } while ( !isspace( c ) && c != '#' );
    if ( c == '#' ) while ( !(end = feof( fp )) && (c = fgetc( fp )) != '\n' );
  }

  token[count] = '\0';

  return count;
}

static int readhead( FILE *fp, int *width, int *height, int *max )
{
//  int c;
  int mode;

  char token[256];
  int  length;

  length = tokenize( fp, token, 256 );

  if ( length == 2 && token[0] == 'P' ) {
    mode = token[1] - '0';
    if ( mode < 1 || mode > 6 ) return 0;
    length = tokenize( fp, token, 256 ); *width  = atoi( token );
    length = tokenize( fp, token, 256 ); *height = atoi( token );
    if ( !(mode == 1 || mode == 4) ) {
      length = tokenize( fp, token, 256 );
      *max = atoi( token );
    }
    else *max = 1;
  }
  else return 0;

  return mode;
}


TRUEIMAGE *finputpnmc( FILE *in, void (*call)( TRUEIMAGE* i, int y ) )
{
  int mode, width, height, max;
  long pixels, linebytes, i, j, count;
  unsigned char *data, *ptr, *temp;

  char token[256];

  TRUEIMAGE *image;

  if ( (mode = readhead( in, &width, &height, &max )) == 0 ) return NULL;
  if ( (image = (TRUEIMAGE*)malloc( sizeof( TRUEIMAGE ) ) ) == NULL ) return NULL;

  image->width  = width;
  image->height = height;
  pixels = width * height;
  linebytes = width * 3;

  if ( (image->dataptr = (unsigned char *)calloc( pixels, 3 )) == NULL ) return NULL;

  data = image->dataptr;

  switch ( mode ) {
  case 0:
    return NULL;
//    break;

  case 1:
    return NULL;
//    break;

  case 2:
    if ( max != 255 ) {
      double s = 255.0 / max;
      for ( i = 0, ptr = data; i < height; ++i ) {
	for ( j = 0; j < width && (count = tokenize( in, token, 256 )); ++j, ptr += 3 )
	  ptr[0] = ptr[1] = ptr[2] = (unsigned char)(atoi( token ) * s + 0.5);
	call( image, i );
      }
    }
    else {
      for ( i = 0, ptr = data; i < height; ++i ) {
	for ( j = 0; j < width && (count = tokenize( in, token, 256 )); ++j, ptr += 3 )
	  ptr[0] = ptr[1] = ptr[2] = (unsigned char)atoi( token );
	call( image, i );
      }
    }
    break;

  case 3:
    if ( max != 255 ) {
      double s = 255.0 / max;
      for ( i = 0, ptr = data; i < height; ++i ) {
	for ( j = 0; j < linebytes && (count = tokenize( in, token, 256 )); ++j, ++ptr )
	  *ptr = (unsigned char)(atoi( token ) * s + 0.5);
	call( image, i );
      }
    }
    else {
      for ( i = 0, ptr = data; i < height; ++i ) {
	for ( j = 0; j < linebytes && (count = tokenize( in, token, 256 )); ++j, ++ptr )
	  *ptr = (unsigned char)atoi( token );
	call( image, i );
      }
    }
    break;

  case 4:
    return NULL;
//    break;

  case 5:
    if ( (temp = (unsigned char*)malloc( width ) ) == NULL ) return NULL;
    for ( ptr = data, i = 0; i < height; ++i ) {
      fread( temp, 1, width, in );
      if ( max != 255 ) {
	double s = 255.0 / max;
	for ( j = 0; j < width; ++j, ptr += 3 )
	  ptr[0] = ptr[1] = ptr[2] = (unsigned char)(temp[j] * s + 0.5);
      }
      else for ( j = 0; j < width; ++j, ptr += 3 ) ptr[0] = ptr[1] = ptr[2] = temp[j];
      call( image, i );
    }
    free( temp );
    break;

  case 6:
    for ( i = 0, ptr = data; i < height; ++i, ptr += linebytes ) {
      fread( ptr, 3, width, in ); 
      if ( max != 255 ) {
	double s = 255.0 / max;
	ptr[0] = (unsigned char)(ptr[0] * s + 0.5);
	ptr[1] = (unsigned char)(ptr[1] * s + 0.5);
	ptr[2] = (unsigned char)(ptr[2] * s + 0.5);
      }
      call( image, i );
    }
    break;
  }

  return image;
}



TRUEIMAGE *finputpnm( FILE *in )
{
  int mode, width, height, max;
  long pixels, i, count;
  unsigned char *data, *ptr, *temp;

  char token[256];

  TRUEIMAGE *image;

  if ( (mode = readhead( in, &width, &height, &max )) == 0 ) return NULL;
  if ( (image = (TRUEIMAGE*)malloc( sizeof( TRUEIMAGE ) ) ) == NULL ) return NULL;

  image->width  = width;
  image->height = height;
  pixels  = width * height;

  if ( (image->dataptr = (unsigned char *)calloc( pixels, 3 )) == NULL ) return NULL;

  data = image->dataptr;

  switch ( mode ) {
  case 0:
    return NULL;
//    break;

  case 1:
    return NULL;
//    break;

  case 2:
    if ( max != 255 ) {
      double s = 255.0 / max;
      for ( i = 0, ptr = data;
	    i < pixels && (count = tokenize( in, token, 256 ));
	    ++i, ptr += 3 )
	ptr[0] = ptr[1] = ptr[2] = (unsigned char)(atoi( token ) * s + 0.5);
    }
    else {
      for ( i = 0, ptr = data;
	    i < pixels && (count = tokenize( in, token, 256 ));
	    ++i, ptr += 3 ) ptr[0] = ptr[1] = ptr[2] = (unsigned char)atoi( token );
    }
    break;

  case 3:
    if ( max != 255 ) {
      double s = 255.0 / max;
      for ( i = 0, ptr = data;
	    i < pixels * 3 && (count = tokenize( in, token, 256 ));
	    ++i, ++ptr ) *ptr = (unsigned char)(atoi( token ) * s + 0.5);
    }
    else {
      for ( i = 0, ptr = data;
	    i < pixels * 3 && (count = tokenize( in, token, 256 ));
	    ++i, ++ptr ) *ptr = (unsigned char)atoi( token );
    }
    break;

  case 4:
    return NULL;
//    break;

  case 5:
    if ( (temp = (unsigned char*)malloc( pixels ) ) == NULL ) return NULL;
    fread( temp, width, height, in );
    if ( max != 255 ) {
      double s = 255.0 / max;
      for ( i = 0, ptr = image->dataptr; i < pixels; ++i, ptr += 3 )
	ptr[0] = ptr[1] = ptr[2] = (unsigned char)(temp[i] * s + 0.5);
    }
    else for ( i = 0, ptr = image->dataptr; i < pixels; ++i, ptr += 3 )
      ptr[0] = ptr[1] = ptr[2] = temp[i];
    free( temp );
    break;

  case 6:
    fread( image->dataptr, width * 3, height, in );
    if ( max != 255 ) {
      double s = 255.0 / max;
      for ( i = 0, ptr = data; i < pixels; ++i, ptr += 3 ) {
	ptr[0] = (unsigned char)(ptr[0] * s + 0.5);
	ptr[1] = (unsigned char)(ptr[1] * s + 0.5);
	ptr[2] = (unsigned char)(ptr[2] * s + 0.5);
      }
    }
    break;
  }

  return image;
}


char* outputpgm( TRUEIMAGE *timage, FILE *out );


// by Masaki Kawase
char* outputppm( TRUEIMAGE *timage, FILE *out, const String& comment )
{
  if (!out || !timage || !timage->dataptr) return NULL ;
//  if (timage->width < 0 || timage->height <0) return NULL ;

  fprintf(out, "P6\n") ;

  if (comment != String(""))
    fprintf(out, "# " + comment + "\n") ;

  fprintf(out, IntToString(timage->width ) + " ") ;
  fprintf(out, IntToString(timage->height) + "\n") ;
  fprintf(out, "255\n") ;

  fwrite( timage->dataptr, timage->width * 3, timage->height, out) ;
  return (char *)EOF ;
}


void freeimage( TRUEIMAGE *timage )
{
  free( timage->dataptr );
  free( timage );
}
