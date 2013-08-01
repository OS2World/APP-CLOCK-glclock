
#ifndef ___DEFINE_H___
#define ___DEFINE_H___


#ifdef  NULL
#undef  NULL
#endif
#define NULL        0

#define for if (0) ; else for

#ifndef TRUE
#define TRUE        1
#endif
#ifndef FALSE
#define FALSE       0
#endif

#define ON          1
#define OFF         0

#define SUCCESS		0
#define FAILURE		-1

#define NONE  -999999

enum { WEST, EAST, NORTH, SOUTH } ;


//#define OVERRIDE_NEW_OPERATOR


#endif
