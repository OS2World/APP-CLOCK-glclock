
// よく使う数学関数 & 定数

#ifndef ___MATH3D_H___
#define ___MATH3D_H___

#include <math.h>
#include <stdlib.h>

#include "Define.H"

#ifndef PAI
#undef  PAI
#endif
#define PAI 3.141592653589793238462643383279502884197169399375105820974944

#define INF     1.0e10
#define EPS     1.0e-10

#define Max(a, b)	((a) >= (b) ? (a) : (b))
#define Min(a, b)	((a) <= (b) ? (a) : (b))

// a を何乗すると b
#define Log(a, b)	(log(b) / log(a))

inline int Abs(int n) { return (n >= 0 ? n : -n) ; }
inline float Abs(float d) { return (d >= 0.0f ? d : -d) ; }
inline double Abs(double d) { return (d >= 0.0 ? d : -d) ; }
//#define Abs(n)	((n) >= 0 ? (n) : -(n))

inline int Equ(float s, float d) { return (Abs(s - d) < EPS) ; }
inline int Equ(double s, double d) { return (Abs(s - d) < EPS) ; }

inline int Sgn(int n) { return (n > 0 ? 1 : (n < 0 ? -1 : 0)) ; }
inline float Sgn(float d) { return (d>0.0f ? 1.0f : (d<0.0f ? -1.0f : 0.0f)) ; }
inline double Sgn(double d) { return (d>0.0 ? 1.0 : (d<0.0 ? -1.0 : 0.0)) ; }

inline int Square(int d) { return d * d ; }
inline float Square(float d) { return d * d ; }
inline double Square(double d) { return d * d ; }

inline int Cube(int d) { return d * d * d ; }
inline float Cube(float d) { return d * d * d ; }
inline double Cube(double d) { return d * d * d ; }

inline int Pow(int d) { return d * d ; }
inline float Pow(float d) { return d * d ; }
inline double Pow(double d) { return d * d ; }

inline float Pow(float d, float n)
{
    return (n == 0.0f ? 1.0f : (float)pow(d, n)) ;
}

inline double Pow(double d, double n)
{
    return (n == 0.0 ? 1.0 : pow(d, n)) ;
}

inline float Pow(float d, int n)
{
    if (n == 0) return 1.0 ;
    if (d == 0.0f) return 0.0f ;

    int     a = Abs(n) ;
    float  m = d ;
    for (int i = 1 ; i < a ; i ++) d *= m ;

    if (n < 0) return 1.0f / d ;
    return d ;
}

inline double Pow(double d, int n)
{
    if (n == 0) return 1.0 ;
    if (d == 0.0) return 0.0 ;

    int     a = Abs(n) ;
    double  m = d ;
    for (int i = 1 ; i < a ; i ++) d *= m ;

    if (n < 0) return 1.0 / d ;
    return d ;
}

inline int Pow(int d, int n)
{
    if (n == 0) return 1 ;
    else if (n < 0) return 0 ;

    int m = d ;
    for (int i = 1 ; i < n ; i ++) d *= m ;
    return d ;
}

// 三角関数関係
inline float Rad(float d) { return (d * (float)PAI / 180.0f) ; }
inline double Rad(double d) { return (d * PAI / 180.0) ; }
inline float Deg(float r) { return (r * 180.0f) / (float)PAI ; }
inline double Deg(double r) { return (r * 180.0) / PAI ; }

inline float Sin(float d) { return (float)sin(Rad(d)) ; }
inline double Sin(double d) { return sin(Rad(d)) ; }
inline float Cos(float d) { return (float)cos(Rad(d)) ; }
inline double Cos(double d) { return cos(Rad(d)) ; }

inline float ATan2(float y, float x) { return Deg((float)atan2(y, x)) ; }
inline double ATan2(double y, double x) { return Deg(atan2(y, x)) ; }

inline float Clump(float d, float l = 0.0, float h = 1.0)
{
	if      (d < l) return l ;
	else if (d > h) return h ;
	else            return d ;
}

inline double Clump(double d, double l = 0.0, double h = 1.0)
{
	if      (d < l) return l ;
	else if (d > h) return h ;
	else            return d ;
}

inline void UnitVector(float v[])
{
	float d ;

	if ((d = (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])) != 0.0f)
	{
		v[0] /= d ;
		v[1] /= d ;
		v[2] /= d ;
	}
}

inline void UnitVector(double v[])
{
	double d ;

	if ((d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])) != 0.0)
	{
		v[0] /= d ;
		v[1] /= d ;
		v[2] /= d ;
	}
}

inline void NormalVector(float v1[], float v2[], float out[])
{
	out[0] = v1[1]*v2[2] - v1[2]*v2[1] ;
	out[1] = v1[2]*v2[0] - v1[0]*v2[2] ;
	out[2] = v1[0]*v2[1] - v1[1]*v2[0] ;

	UnitVector(out) ;
}

inline void NormalVector(double v1[], double v2[], double out[])
{
	out[0] = v1[1]*v2[2] - v1[2]*v2[1] ;
	out[1] = v1[2]*v2[0] - v1[0]*v2[2] ;
	out[2] = v1[0]*v2[1] - v1[1]*v2[0] ;

	UnitVector(out) ;
}


#endif
