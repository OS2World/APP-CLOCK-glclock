
#ifndef ___GL_OBJECT_H___
#define ___GL_OBJECT_H___

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// データ変換ウォーニングの無効化
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA
#endif

#include <iostream.h>

#include "GLextensions.H"
#include "PnmIO.H"


// for debug
//#define glNormal3f(x,y,z)	glTexCoord2f(x, y)
//#define glNormal3fv(v)		glTexCoord2fv(v)
//#define glNormal3f(x,y,z)
//#define glNormal3fv(v)


//#define	DEBUG_MATRIX_STACK


#define GRAY_RATIO_R	(0.30f)
#define GRAY_RATIO_G	(0.59f)
#define GRAY_RATIO_B	(0.11f)


#ifdef	DEBUG_MATRIX_STACK
#define glPushMatrix	DEBUG_glPushMatrix
#define glPopMatrix		DEBUG_glPopMatrix
void DEBUG_glPushMatrix() ;
void DEBUG_glPopMatrix() ;
#undef	GL_CONPILE
#define	GL_COMPILE	GL_COMPILE_AND_EXECUTE
#endif	// #ifdef	DEBUG_MATRIX_STACK


class Vector2d ;

// イメージフォーマットを RGB から RGBA に
int RGBtoRGBA(TRUEIMAGE *image, GLfloat alpha = 1.0f) ;

// イメージコピー
TRUEIMAGE *CreateCopyImage(TRUEIMAGE *src) ;

// 指定サイズの新たなイメージ生成（データは不明）
TRUEIMAGE *CreateNewImage(unsigned int w, unsigned int h = 0) ;

// イメージ補正関係
// radiusRatio に 0.0 以外を指定すると、
// 変更している部分の中心からの距離の radiusRatio 乗に比例して
// 補正レートが変化

// イメージクリア（デフォルトは黒）
void ClearImage(TRUEIMAGE *image, float r = 0.0f, float g = -1.0, float b = -1.0f, float radiusRatio = 0.0f,
				unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// イメージガンマ補正
void GammaCollection(TRUEIMAGE *image, float gamma, float radiusRatio = 0.0f,
					 unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// イメージの明るさ変更
void ChangeBrightness(TRUEIMAGE *image, float bright, float radiusRatio = 0.0f,
					  unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// イメージ明るさ変化（ただしＲＧＢ別々に）
void ModulateRGB(TRUEIMAGE *image, float r, float g, float b, float radiusRatio = 0.0f,
				 unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// イメージの彩度変更
void ChangeSaturation(TRUEIMAGE *image, float vivid, float radiusRatio = 0.0f,
					  unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// イメージのコントラスト変更
void ChangeContrast(TRUEIMAGE *image, float cont, float radiusRatio = 0.0f,
					unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// イメージの明るさ変更
void ChangeGammaBrightness(TRUEIMAGE *image, float gamma, float radiusRatio = 0.0f,
						   unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;

// image に image2 を加算（それぞれに対し重み付けが可能）
// image のみ、width, height 指定可能
void AddImage(TRUEIMAGE *image, TRUEIMAGE *image2, float weight = 1.0f, float weight2 = 1.0f, float radiusRatio = 0.0f,
			  unsigned int ix = 0, unsigned int iy = 0, unsigned int width = 0, unsigned int height = 0) ;


// イメージの上下左右反転
void ReverseCoordinates(TRUEIMAGE *image, int mirrorFlag, int flipFlag) ;

// 視野ピラミッド作成（ジッタ情報含）
void     Perspective(GLfloat scr_w, GLfloat scr_h, GLfloat n_clip, GLfloat f_clip,
				 int win_w = NONE, int win_h = NONE, GLfloat px = 0.0f, GLfloat py = 0.0f) ;

Vector2d *CreateSampleJitter(int sample) ;
Vector2d *CreateSampleDepthOfFieldJitter(int sample) ;
void     DeleteSampleJitter(Vector2d *jitter) ;

void     SetMaterial(GLenum surface, GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f, GLfloat metal = 0.0f,
					 GLfloat specular = 1.5f, GLfloat shininess = 64.0f, GLint mirror = FALSE, GLfloat m_ratio = 0.0f, GLfloat amb = NONE) ;

void     SetLightColor(GLenum light, GLfloat colorRGBA[4], GLfloat amb = 0.1f,
					   GLfloat specularFlag = 1.0f, GLfloat bright = 1.0f, GLfloat *modulate = NULL) ;
void     SetDirectionalLight(GLenum light, GLfloat dir[4], GLfloat colorRGBA[4], GLfloat amb = 0.1f,
							 GLfloat specularFlag = 1.0f, GLfloat bright = 1.0f, GLfloat *modulate = NULL) ;
void     SetPositionalLight(GLenum light, GLfloat pos[4], GLfloat colorRGBA[4],
							GLfloat amb = 0.1f, GLfloat specularFlag = 1.0f, GLfloat bright = 1.0f,
							GLfloat const_attn = 1.0f, GLfloat linear_attn = 0.0f, GLfloat quad_attn = 0.0f,
							GLfloat *dir = NULL, GLfloat spot_cutoff = 180.0f, GLfloat spot_exp = 0.0f,
							GLfloat *modulate = NULL) ;

// 文字列描画
#ifndef USE_OS2
void DrawString(String& str, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, void *font = GLUT_BITMAP_9_BY_15) ;
#else
void DrawString(String& str, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 0.0f, void *font = GLUT_BITMAP_8_BY_13) ;
#endif
// 文字列描画
void DrawString(String& str, void *font = GLUT_BITMAP_9_BY_15) ;

// １文字描画
#ifndef USE_OS2
inline void DrawCharacter(char chr, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z= 0.0f, void *font = GLUT_BITMAP_9_BY_15)
#else
inline void DrawCharacter(char chr, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z= 0.0f, void *font = GLUT_BITMAP_8_BY_13)
#endif
{
	glRasterPos3d(x, y, z) ;
	glutBitmapCharacter(font, chr) ;
}

class Vector2d
{
public:
	GLfloat x, y ;

	Vector2d() {} ;
	inline Vector2d(GLfloat vx, GLfloat vy) { x = vx ; y = vy ; }
	~Vector2d() {} ;

	inline Vector2d& operator ()(GLfloat vx, GLfloat vy) { x = vx ; y = vy ; return *this ; }
} ;


class Vector3d
{
public:
    GLfloat v[3] ;

    Vector3d() {} ;
    inline Vector3d(GLfloat x, GLfloat y, GLfloat z) { v[0] = x ; v[1] = y ; v[2] = z ; }
    inline Vector3d(const Vector3d& ov) { v[0] = ov.v[0] ; v[1] = ov.v[1] ; v[2] = ov.v[2] ; }
    inline Vector3d(GLfloat *ov) { v[0] = ov[0] ; v[1] = ov[1] ; v[2] = ov[2] ; }

    // アクセサ
    inline GLfloat& X() { return v[0] ; }
    inline GLfloat& Y() { return v[1] ; }
    inline GLfloat& Z() { return v[2] ; }

    inline const GLfloat& X() const { return v[0] ; }
    inline const GLfloat& Y() const { return v[1] ; }
    inline const GLfloat& Z() const { return v[2] ; }

    // メソッド
    Vector3d& Unit() ;
//    friend const Vector3d Unit(Vector3d& vec) ;
    friend Vector3d Unit(const Vector3d& vec) ;
//    friend const Vector3d UnitNormal(Vector3d& v1, Vector3d& v2) ;
//    friend const Vector3d UnitNormal(Vector3d& v1, const Vector3d& v2) ;
//    friend const Vector3d UnitNormal(const Vector3d& v1, Vector3d& v2) ;
    friend Vector3d UnitNormal(const Vector3d& v1, const Vector3d& v2) ;
    Vector3d& Transform(const GLfloat m[16]) ;
    Vector3d& Transform3x3(const GLfloat m[16]) ;
	inline float Abs() const { return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) ; }

	// Sphere Mapping 変換
	Vector3d& SphericalVertex() ;
	friend Vector3d SphericalVertex(Vector3d vec) ;

	// カラーと見なした際の彩度を変化
	Vector3d& ChangeSaturation(float vividness) ;
    friend Vector3d ChangeSaturation(Vector3d src, float vividness) ;

    // オペレータ
    inline const Vector3d& operator ()(GLfloat x, GLfloat y, GLfloat z)
      { v[0] = x ; v[1] = y ; v[2] = z ; return *this ; }

//    friend const Vector3d operator |(Vector3d& v1, Vector3d& v2) ;
//    friend const Vector3d operator |(Vector3d& v1, const Vector3d& v2) ;
//    friend const Vector3d operator |(const Vector3d& v1, Vector3d& v2) ;
    friend Vector3d operator |(const Vector3d& v1, const Vector3d& v2) ;

    // メンバ関数 X(),Y(),Z() が const でないため、引数は const に出来ない
//    friend int operator ==(Vector3d& v1, Vector3d& v2)
//      { return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2]) ; }
//    friend int operator ==(Vector3d& v1, const Vector3d& v2)
//      { return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2]) ; }
//    friend int operator ==(const Vector3d& v1, Vector3d& v2)
//      { return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2]) ; }
    inline friend int operator ==(const Vector3d& v1, const Vector3d& v2)
      { return (v1.v[0] == v2.v[0] && v1.v[1] == v2.v[1] && v1.v[2] == v2.v[2]) ; }

//    friend int operator !=(Vector3d& v1, Vector3d& v2)
//      { return !(v1 == v2) ; }
//    friend int operator !=(Vector3d& v1, const Vector3d& v2)
//      { return !(v1 == v2) ; }
//    friend int operator !=(const Vector3d& v1, Vector3d& v2)
//      { return !(v1 == v2) ; }
    inline friend int operator !=(const Vector3d& v1, const Vector3d& v2)
      { return !(v1 == v2) ; }

//    Vector3d& operator =(Vector3d& ov)
//      { v[0] = ov.v[0] ; v[1] = ov.v[1] ; v[2] = ov.v[2] ; return *this ; }

//    const Vector3d& operator =(Vector3d& ov)
//      { v[0] = ov.v[0] ; v[1] = ov.v[1] ; v[2] = ov.v[2] ; return *this ; }
    inline const Vector3d& operator =(const Vector3d& ov)
      { v[0] = ov.v[0] ; v[1] = ov.v[1] ; v[2] = ov.v[2] ; return *this ; }

//    const Vector3d& operator =(GLfloat ov[3])
//      { v[0] = ov[0] ; v[1] = ov[1] ; v[2] = ov[2] ; return *this ; }
    inline const Vector3d& operator =(const GLfloat ov[3])
      { v[0] = ov[0] ; v[1] = ov[1] ; v[2] = ov[2] ; return *this ; }

//    const Vector3d& operator +=(Vector3d& ov)
//      { v[0] += ov.v[0] ; v[1] += ov.v[1] ; v[2] += ov.v[2] ; return *this ; }
    inline const Vector3d& operator +=(const Vector3d& ov)
      { v[0] += ov.v[0] ; v[1] += ov.v[1] ; v[2] += ov.v[2] ; return *this ; }

//    friend const Vector3d operator +(Vector3d& v1, Vector3d& v2)
//      { return Vector3d(v1.v[0]+v2.v[0], v1.v[1]+v2.v[1], v1.v[2]+v2.v[2]) ; }
//    friend const Vector3d operator +(Vector3d& v1, const Vector3d& v2)
//      { return Vector3d(v1.v[0]+v2.v[0], v1.v[1]+v2.v[1], v1.v[2]+v2.v[2]) ; }
//    friend const Vector3d operator +(const Vector3d& v1, Vector3d& v2)
//      { return Vector3d(v1.v[0]+v2.v[0], v1.v[1]+v2.v[1], v1.v[2]+v2.v[2]) ; }
    friend Vector3d operator +(const Vector3d& v1, const Vector3d& v2)
      { return Vector3d(v1.v[0]+v2.v[0], v1.v[1]+v2.v[1], v1.v[2]+v2.v[2]) ; }

//    const Vector3d& operator -=(Vector3d& ov)
//      { v[0] -= ov.v[0] ; v[1] -= ov.v[1] ; v[2] -= ov.v[2] ; return *this ; }
    inline const Vector3d& operator -=(const Vector3d& ov)
      { v[0] -= ov.v[0] ; v[1] -= ov.v[1] ; v[2] -= ov.v[2] ; return *this ; }

//    friend const Vector3d operator -(Vector3d& v1, Vector3d& v2)
//      { return Vector3d(v1.v[0]-v2.v[0], v1.v[1]-v2.v[1], v1.v[2]-v2.v[2]) ; }
//    friend const Vector3d operator -(Vector3d& v1, const Vector3d& v2)
//      { return Vector3d(v1.v[0]-v2.v[0], v1.v[1]-v2.v[1], v1.v[2]-v2.v[2]) ; }
//    friend const Vector3d operator -(const Vector3d& v1, Vector3d& v2)
//      { return Vector3d(v1.v[0]-v2.v[0], v1.v[1]-v2.v[1], v1.v[2]-v2.v[2]) ; }
    inline friend Vector3d operator -(const Vector3d& v1, const Vector3d& v2)
      { return Vector3d(v1.v[0]-v2.v[0], v1.v[1]-v2.v[1], v1.v[2]-v2.v[2]) ; }
    inline friend Vector3d operator -(const Vector3d& v)
      { return Vector3d(-v.v[0], -v.v[1], -v.v[2]) ; }

    inline const Vector3d& operator *=(float d)
      { v[0] *= d ; v[1] *= d ; v[2] *= d ; return *this ; }
//    friend const Vector3d operator *(Vector3d& v, float d)
//      { return Vector3d(v.v[0] * d, v.v[1] * d, v.v[2] * d) ; }
    inline friend Vector3d operator *(const Vector3d& v, float d)
      { return Vector3d(v.v[0] * d, v.v[1] * d, v.v[2] * d) ; }

    const Vector3d& operator /=(float d)
      { v[0] /= d ; v[1] /= d ; v[2] /= d ; return *this ; }
//    friend const Vector3d operator /(Vector3d& v, float d)
//      { return Vector3d(v.v[0] / d, v.v[1] / d, v.v[2] / d) ; }
    inline friend Vector3d operator /(const Vector3d& v, float d)
      { return Vector3d(v.v[0] / d, v.v[1] / d, v.v[2] / d) ; }

    static const Vector3d white ;
} ;

ostream& operator <<(ostream& os, Vector3d& vec) ;

inline GLfloat* VectorToArray(GLfloat v[3], const Vector3d& vec)
{
	v[0] = vec.X() ;
	v[1] = vec.Y() ;
	v[2] = vec.Z() ;

	return v ;
}


class VPolygon
{
public:
    Vector3d vertex[4], normal[4], pNormal ;

	inline VPolygon& operator=(const VPolygon& v)
	{
		for (int i = 0 ; i < 4 ; i ++)
		{
			vertex[i] = v.vertex[i] ;
			normal[i] = v.normal[i] ;
		}
		pNormal = v.pNormal ;

		return *this ;
	}
} ;


class Object
{
public:
    int     nPolygons, fType, eType ;
    VPolygon *polygon ;

    inline Object(int nPol = 0)
	{
		if (nPol >= 0) polygon = new VPolygon[nPol] ;
		else { polygon = NULL ; nPol = 0 ; }
		nPolygons = nPol ;
	}

    ~Object() { delete [] polygon ; }

	inline Object& NPolygons(int nPol)
	{
		if (polygon) delete [] polygon ;
		if (nPol < 0) nPol = 0 ;
		polygon = new VPolygon[nPol] ;
		nPolygons = nPol ;
		return *this ;
	}

    Object& MakePolygon(int polNo, int v1, int v2, int v3, const GLfloat data[][3], const GLfloat m[16] = NULL) ;
    Object& MakePolygon(int polNo, int v1, int v2, int v3, int v4, const GLfloat data[][3], const GLfloat m[16] = NULL) ;
    Object& AverageNormal(int pNo, int vNo, int pNo1, int pNo2, int pNo3 = -1, int pNo4 = -1, int pNo5 = -1, int pNo6 = -1) ;
	Object& DrawTriangle(int p = 0, int depth = FALSE, int average = FALSE, int flatFlag = FALSE, int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;
	Object& DrawTriangles(int depth = FALSE, int average = TRUE, int flatFlag = FALSE, int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;
} ;


class RollingObject: public Object
{
public: 
	int fType, eType, uDiv, vDiv ;

	// コンストラクタ
	RollingObject(int nPol = 0): Object(nPol) { uDiv = vDiv = 0 ; }

	inline RollingObject& NPolygons(int nPol) { Object::NPolygons(nPol) ; return *this ; }
	RollingObject& MakeRollingPolygons(int u, int v, GLfloat data[][2], GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f) ;
	static int VFlag(int v, int ud, int vFlag, int vFlags[][2]) ;
	RollingObject& AverageNormals(int uFlag = TRUE, int vFlag = TRUE, int vFlags[][2] = NULL) ; //, int vNo = 0) ;

	// それぞれ、三角ポリゴン（頂点がオブジェクトの中心１点）の場合のみ対応
	RollingObject& TopNormal(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = 1.0f) ;
	RollingObject& BottomNormal(GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat z = -1.0f) ;

	void DrawObject(int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;
} ;


class PositivePolyhedron: public Object
{
public:
    int depth, average ;

    PositivePolyhedron(int nPols): Object(nPols) {} ;

	PositivePolyhedron& Smooth() ;
	inline PositivePolyhedron& Depth(int d) { depth = d ; return *this ; }
	inline PositivePolyhedron& Average(int b) { average = b ; return *this ; }

	inline PositivePolyhedron& Draw(int d = NONE, int a = NONE, int flatFlag = FALSE, int normalFlag = TRUE, float *s = NULL, float *t = NULL)
	{
		if (d >= 0) depth   = d ;
		if (a >= 0) average = a ;
		DrawTriangles(depth, average, flatFlag, normalFlag, s, t) ;
		return *this ;
	}
} ;


class Octahedron: public PositivePolyhedron
{
public:
	Octahedron& Make(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, int d = NONE, int a = NONE, int smooth = NONE) ;
    inline Octahedron(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, int d = FALSE, int a = TRUE, int smooth = FALSE): PositivePolyhedron(8)
	  { Make(x,y,z, d, a, smooth) ; }

	static void DrawPolyhedron(GLfloat r = 1.0, int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;

	static GLfloat o_v[][3] ;
	static int     o_o[][3] ;
} ;


class Icosahedron: public PositivePolyhedron
{
public:
	Icosahedron& Make(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, int d = NONE, int a = NONE, int smooth = NONE) ;
    inline Icosahedron(GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, int d = FALSE, int a = TRUE, int smooth = FALSE): PositivePolyhedron(20)
	  { Make(x,y,z, d, a, smooth) ; }

	static void DrawPolyhedron(GLfloat r = 1.0, int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;

	static GLfloat X, Z, i_v[][3] ;
	static int  i_o[][3] ;
} ;


// 屈折ベクトル、鏡面反射係数計算クラス
class Refraction
{
public:
	GLfloat  specular ;
	Vector3d refraction ;

	inline Refraction& operator =(const Refraction& r)
	{
		specular = r.specular ;
		refraction = r.refraction ;

		return *this ;
	}

	// n1, n2, eye, normal から specular および refraction を計算
	// 反射係数や透過係数の計算も行う
	inline Refraction& CalculateRefraction(GLfloat n, Vector3d&eye, Vector3d& normal)
	{
		float n2 = n ; // n2 / n1 ;
		float cos1 = -(eye.X() * normal.X() +
			       eye.Y() * normal.Y() +
			       eye.Z() * normal.Z()) ;
		float sin1 = 1.0f - cos1 * cos1 ;
		if (sin1 < 0.0f) specular = 1.0f ; // 完全鏡面反射
		else
		{
			float cos2 = sqrt(1.0f - sin1 / (n2 * n2)) ;
			float n2cos2cos1 = n2 * cos2 - cos1 ;

			float kss = Pow(n2cos2cos1 / (n2 * cos2 + cos1)) ;
			float ksp = Pow((n2 * cos1 - cos2) / (n2 * cos1 + cos2)) ;
			specular = 0.5f * (kss + ksp) ;

			refraction((eye.X() - normal.X() * n2cos2cos1) / n2,
				   (eye.Y() - normal.Y() * n2cos2cos1) / n2,
				   (eye.Z() - normal.Z() * n2cos2cos1) / n2) ;
		}

		return *this ;
	}

	// n1, n2, eye, normal から refraction を計算
	// 反射係数や透過係数の計算なしバージョン
	inline Refraction& CalculateRefraction2(GLfloat n, Vector3d&eye, Vector3d& normal)
	{
		float n2 = n ; // n2 / n1 ;
		float cos1 = -(eye.X() * normal.X() +
			       eye.Y() * normal.Y() +
			       eye.Z() * normal.Z()) ;
		float sin1_sq = 1.0f - cos1 * cos1 ;
		float root = n2 * n2 - sin1_sq ;
		if (root < 0.0f) root = 0.0f ;
		float sqrtn2sin = sqrt(root) - cos1 ;

		refraction = (eye - normal * sqrtn2sin) / n2 ;

//		cout << refraction << "   length: " << refraction.Abs() << endl ;

		return *this ;
	}


	// eye, normal から specular を計算
	// 反射係数や透過係数の計算を行うだけで、
	// 実際には視線ベクトルは屈折させない
	// フェイクを行う場合、n は入射角による反射透過の変化率を指定
	inline Refraction& CalculateFakeRefraction(GLfloat n, Vector3d&eye, Vector3d& normal)
	{
		float cos1 = -(eye.X() * normal.X() +
			       eye.Y() * normal.Y() +
			       eye.Z() * normal.Z()) ;

		// 視線ベクトルと法線ベクトルの角度から鏡面反射率を計算（もろフェイク）
		if (n == 1.0f) specular = 1.0f - cos1 ;
		else specular = 1.0f - Pow(cos1, n) ;
		return *this ;
	}

	Refraction() { } ;
} ;


// 屈折マッピング使用時の各頂点情報
// 頂点座標、法線、テクスチャ座標、鏡面反射係数、透過係数
class RefractVertex
{
public:
	Vector3d   vertex, normal, texture, specular_color, refraction_color ;
	GLfloat    n ;
	Refraction refraction ;

	RefractVertex() {} ;
	inline RefractVertex(Vector3d& v, Vector3d& norm, GLfloat n2)
	{
		vertex = v ;
		normal = norm ;
		n = n2 ;
	}

	inline RefractVertex& operator =(const RefractVertex& v)
	{
		vertex = v.vertex ;
		normal = v.normal ;
		texture = v.texture ;
		specular_color = v.specular_color ;
		refraction_color = v.refraction_color ;
		n = v.n ;
		refraction = v.refraction ;

		return *this ;
	}

	inline RefractVertex(const RefractVertex& v)
	{
		*this = v ;
	}

	// 透過率、反射率を考慮に入れた計算
	inline RefractVertex& CalculateVertex(const GLfloat matrix[16], const Vector3d& r_c, const Vector3d& s_c = Vector3d::white)
	{
		Vector3d eye, norm ;
		eye = vertex ;
		eye.Transform(matrix) ;

		if (eye.Z() >= 0.0f) return *this ;
		eye.Unit() ;

		norm = normal ;
		norm.Transform3x3(matrix) ;

		// 屈折ベクトル、反射係数計算
		refraction.CalculateRefraction(n, eye, norm) ;

		specular_color = s_c * refraction.specular ;
		refraction_color = r_c * (1.0f - refraction.specular) ;

		// 屈折ベクトルからテクスチャ座標計算
		GLfloat m = 2.0f * sqrt(Pow(refraction.refraction.X()) +
					Pow(refraction.refraction.Y()) +
					Pow(refraction.refraction.Z() + 1.0f)) ;

		if (m >= 0.0f)
			texture(refraction.refraction.X() / m + 0.5f,
				refraction.refraction.X() / m + 0.5f, 0.0f) ;
		else texture(0.0f, 0.5f, 0.0f) ;

		return *this ;
	}

	// 透過率、反射率は無関係（描画時に色は指定しない）
	// アプリケーション側で glColor*() で指定
	inline RefractVertex& CalculateVertex(const GLfloat matrix[16])
	{
		Vector3d eye, norm ;
		eye = vertex ;
		eye.Transform(matrix) ;

		if (eye.Z() >= 0.0f) return *this ;
		eye.Unit() ;

		norm = normal ;
		norm.Transform3x3(matrix) ;

		// 屈折ベクトル計算
		refraction.CalculateRefraction2(n, eye, norm) ;

		// 屈折ベクトルからテクスチャ座標計算
		GLfloat m = 2.0f * sqrt(Pow(refraction.refraction.X()) +
					Pow(refraction.refraction.Y()) +
					Pow(refraction.refraction.Z() + 1.0f)) ;
		if (m > 0.0f)
			texture(refraction.refraction.X() / m + 0.5f,
				refraction.refraction.X() / m + 0.5f, 0.0f) ;
		else texture(0.0f, 0.5f, 0.0f) ;

		return *this ;
	}


	// フェイク屈折...
	// 透過率、反射率を考慮に入れた計算
	inline RefractVertex& CalculateFakeVertex(const GLfloat matrix[16], const Vector3d& r_c, const Vector3d& s_c = Vector3d::white)
	{
		Vector3d eye, norm ;
		eye = vertex ;
		eye.Transform(matrix) ;

		if (eye.Z() >= 0.0f) return *this ;
		eye.Unit() ;

		norm = normal ;
		norm.Transform3x3(matrix) ;

		// 反射係数計算
		refraction.CalculateFakeRefraction(n, eye, norm) ;

		specular_color = s_c * refraction.specular ;
		refraction_color = r_c * (1.0f - refraction.specular) ;

		// 屈折ベクトル（この場合は視線ベクトルそのまま）からテクスチャ座標計算
		GLfloat m = 2.0f * sqrt(Pow(eye.X()) +
					Pow(eye.Y()) +
					Pow(eye.Z() + 1.0f)) ;

/*
		if (m >= 0.0f)
			texture(eye.X() / m + 0.5f,
				eye.X() / m + 0.5f, 0.0f) ;
		else texture(0.0f, 0.5f, 0.0f) ;
*/
		texture(eye.X() / m + 0.5f,
			eye.X() / m + 0.5f, 0.0f) ;

		return *this ;
	}

	// フェイク屈折...
	// 透過率、反射率も無視
	inline RefractVertex& CalculateFakeVertex(const GLfloat matrix[16])
	{
		Vector3d eye, norm ;
		eye = vertex ;
		eye.Transform(matrix) ;

		if (eye.Z() >= 0.0f) return *this ;
		eye.Unit() ;

		// 屈折ベクトル（この場合は視線ベクトルそのまま）からテクスチャ座標計算
		GLfloat m = 2.0f * sqrt(Pow(eye.X()) +
					Pow(eye.Y()) +
					Pow(eye.Z() + 1.0f)) ;

		texture(eye.X() / m + 0.5f,
			eye.X() / m + 0.5f, 0.0f) ;

		return *this ;
	}
} ;


// 屈折あり回転体オブジェクト
class RefractRollingObject : public RollingObject
{
public:
	RefractVertex *verticies ;

	// コンストラクタ
	inline RefractRollingObject(int nPol = 0): RollingObject(nPol) { verticies = NULL ; }

	// デストラクタ 頂点単位データが生成されているなら解放
	~RefractRollingObject() { if (verticies) delete [] verticies ; }

	// 頂点データ配列アクセサ（エラーチェック等一切なし）
	inline RefractVertex& Vertex(int u, int v) { return verticies[v * (uDiv + 1) + u] ; }

	// 屈折とか用の頂点単位のデータ作成
	// すでに回転体のポリゴンデータが存在していなければならない
	// 現段階では MakeRollingObject() 内からのみコールされる
	inline RefractRollingObject& MakeRefractVerticies(GLfloat n = 1.5f)
	{
		if (verticies) delete [] verticies ;
		verticies = new RefractVertex[(uDiv + 1) * vDiv] ;

		int u, v ;
		for (v = 0 ; v < vDiv - 1 ; v ++)
		{
			int pNo = v ;

			for (u = 0 ; u <= uDiv ; u ++)
			{
				if (u == uDiv) pNo = v ;

				// 念のため初期化
				Vertex(u, v).texture(0.0f, 0.0f, 0.0f) ;
				Vertex(u, v).specular_color(1.0f, 1.0f, 1.0f) ;
				Vertex(u, v).refraction_color(0.0f, 0.0f, 0.0f) ;

				// 屈折率
				Vertex(u, v).n = n ;

				// 座標および法線ベクトル
				Vertex(u, v).vertex = polygon[pNo].vertex[0] ;
				Vertex(u, v).normal = polygon[pNo].normal[0] ;

				pNo += vDiv - 1 ;
			}
		}

		// 底の頂点データ
		// 念のため初期化
		Vertex(0, vDiv - 1).texture(0.0f, 0.0f, 0.0f) ;
		Vertex(0, vDiv - 1).specular_color(1.0f, 1.0f, 1.0f) ;
		Vertex(0, vDiv - 1).refraction_color(0.0f, 0.0f, 0.0f) ;

		// 屈折率
		Vertex(0, vDiv - 1).n = n ;

		// 座標および法線ベクトル
		Vertex(0, vDiv - 1).vertex = polygon[vDiv - 1].vertex[0] ;
		Vertex(0, vDiv - 1).normal = polygon[vDiv - 1].normal[0] ;

		return *this ;
	}

	// 回転体ポリゴンと、屈折とか用の頂点データ作成
	inline RefractRollingObject& MakeRollingPolygons(int u, int v, GLfloat data[][2], GLfloat n = 1.5f, GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f)
	{
		// 回転体ポリゴン生成
		RollingObject::MakeRollingPolygons(u,v, data, x,y,z) ;

		// とりあえず面倒だから、屈折オブジェクトは全てスムーズで
		// 上下のポリゴンは三角形（トップとボトム頂点がz軸中央に１つずつだけ）
		AverageNormals() ;
		TopNormal() ;
		BottomNormal() ;

		// 屈折関係用頂点単位のデータ作成
		MakeRefractVerticies(n) ;

		return *this ;
	}

	// 屈折関係データを計算しながら OpenGL描画
	// 透過率、指定した色を考慮して描画
	RefractRollingObject& DrawRefractObject(const Vector3d& r_c, const Vector3d& s_c = Vector3d::white) ;

	// 屈折関係データを計算しながら OpenGL描画
	// 透過率、色は無視（アプリケーション側が自前で glColor*() を実行）
	RefractRollingObject& DrawRefractObject() ;

	// 我流フェイク屈折マップしながら OpenGL描画（屈折率無関係）
	// 透過率、指定した色を考慮して描画
	RefractRollingObject& DrawFakeRefractObject(const Vector3d& r_c, const Vector3d& s_c = Vector3d::white) ;

	// 我流フェイク屈折マップしながら OpenGL描画（屈折率無関係）
	// 透過率、色も無視（アプリケーション側が自前で glColor*() を実行）
	RefractRollingObject& DrawFakeRefractObject() ;
} ;


class WineGlass: public RollingObject
{
public:
	enum { N_VERTICIES = 40 } ;
	static GLfloat vertexes[N_VERTICIES][2] ;
	// numbers of polygons = div * ((N_VERTICIES - 2) * 2 = div * 76

	inline WineGlass(int divide = 32, GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f)
	: RollingObject(divide * (N_VERTICIES - 1))
	{
		MakeRollingPolygons(divide, N_VERTICIES, vertexes, x / 19.5f, y / 19.5f, z / 19.5f) ;
		AverageNormals(TRUE, TRUE) ;
		TopNormal() ;
		BottomNormal() ;
	}

	static void DrawPolyhedron(int divide = 32, GLfloat r = 1.0, int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;
	void DrawObject(int normalFlag = TRUE, float *s = NULL, float *t = NULL) ;
} ;

class RefractWineGlass: public RefractRollingObject
{
public:

	inline RefractWineGlass(int divide = 32, GLfloat n = 1.5f, GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f)
	: RefractRollingObject(divide * (WineGlass::N_VERTICIES - 1))
	{
		MakeRollingPolygons(divide, WineGlass::N_VERTICIES, WineGlass::vertexes, n, x / 19.5f, y / 19.5f, z / 19.5f) ;
//		AverageNormals(TRUE, TRUE) ;
//		TopNormal() ;
//		BottomNormal() ;
	}
} ;


// 環境マップから背景画像を描画する半球体（おわん型）オブジェクト
// 基本的に法線は内側に向ける
// 背景描画に使うメソッド内では法線の逆向きベクトルからテクスチャ座標を計算する
class HalfSphere: public RollingObject
{
public:
	enum { N_DIVIDE = 32, V_DIVIDE = 16, MAX_VERTICIES = 128 } ;

	inline HalfSphere(int divide = N_DIVIDE, int vDivide = V_DIVIDE, GLfloat x = 1.0f, GLfloat y = 1.0f, GLfloat z = 1.0f, GLfloat r = 1.0f, GLfloat g = 1.0f, GLfloat b = 1.0f)
	: RollingObject(divide * vDivide)
	{
		if (divide  < 3) divide  = 3 ;
		if (vDivide < 2) vDivide = 2 ;

		if (vDivide > MAX_VERTICIES - 1) vDivide = MAX_VERTICIES - 1 ;

		// ｖ頂点数は vDivide + 1
		int nVerticies = vDivide + 1 ;

		// 半球体座標生成
		GLfloat verticies[MAX_VERTICIES][2] ;

		for (int v = 0 ; v < nVerticies ; v ++)
		{
			GLdouble r = Rad(90.0 / vDivide * v) ;
			verticies[v][0] =  sin(r) ;
			verticies[v][1] = -cos(r) ;
		}
		// 中心だけはテクスチャ座標計算が不可能なので微妙にずらす
		verticies[0][0] = 0.0001f ;

		MakeRollingPolygons(divide, nVerticies, verticies, x, y, z) ;
		AverageNormals(TRUE, TRUE) ;

		// 最初の列（おわんの底）の法線ベクトルは真上に
//		TopNormal() ;

		// 最後の列（おわんの縁）の法線ベクトルは真横に
		// これをきちんと処理しないと極端な広角の場合に不具合（ほとんど問題なし）
	}

	// 半球体の描画（テクスチャ座標、頂点座標）
	void DrawObject() ;
	void DrawObjectWithNormalMapping() ;
	void DrawObjectWithNormalMapping2() ;
	static void DrawPolyhedron(int divide = N_DIVIDE, int vDivide = V_DIVIDE, GLfloat r = 1.0) ;
	static void DrawPolyhedronWithNormalMapping(int divide = N_DIVIDE, int vDivide = V_DIVIDE, GLfloat r = 1.0) ;
	static void DrawPolyhedronWithNormalMapping2(int divide = N_DIVIDE, int vDivide = V_DIVIDE, GLfloat r = 1.0) ;
} ;



// 回転ベクトル操作（単純な回転角度の蓄積: 0.0 から 360.0 をループ）
class RotateVector
{
public:
	Vector3d status, vector ;

	RotateVector(GLfloat sx = 0.0f, GLfloat sy = 0.0f, GLfloat sz = 0.0f,
		     GLfloat vx = 0.0f, GLfloat vy = 0.0f, GLfloat vz = 0.0f)
	: status(sx, sy, sz), vector(vx, vy, vz) { } ;

	inline void Transform()
	{
		status += vector ;

		if      (status.X() >= 360.0f) status.X() -= 360.0f ;
		else if (status.X() <=   0.0f) status.X() += 360.0f ;
		if      (status.Y() >= 360.0f) status.Y() -= 360.0f ;
		else if (status.Y() <=   0.0f) status.Y() += 360.0f ;
		if      (status.Z() >= 360.0f) status.Z() -= 360.0f ;
		else if (status.Z() <=   0.0f) status.Z() += 360.0f ;
	}
} ;


// テクスチャコーディネーション移動（0.0 から 1.0 をループ）
class TranslateTexture
{
public:
	Vector3d status, vector ;

	TranslateTexture(GLfloat sx = 0.0f, GLfloat sy = 0.0f, GLfloat sz = 0.0f,
			 GLfloat vx = 0.0f, GLfloat vy = 0.0f, GLfloat vz = 0.0f)
	: status(sx, sy, sz), vector(vx, vy, vz) { } ;

	inline void Transform()
	{
		status += vector ;

		if      (status.X() >= 1.0f) status.X() -= 1.0f ;
		else if (status.X() <= 0.0f) status.X() += 1.0f ;
		if      (status.Y() >= 1.0f) status.Y() -= 1.0f ;
		else if (status.Y() <= 0.0f) status.Y() += 1.0f ;
		if      (status.Z() >= 1.0f) status.Z() -= 1.0f ;
		else if (status.Z() <= 0.0f) status.Z() += 1.0f ;
	}
} ;


// テクスチャ・オブジェクト管理クラス
class TextureObjectCtrl
{
	enum { ALLOC_SIZE = 16 } ;

	GLuint	*texture ;
	int		nTextures ;
	int		nAlloc ;

public:
	inline void Initialize() { if (texture) delete [] texture ; nTextures = 0 ; nAlloc = 0 ; texture = NULL ; }
	inline TextureObjectCtrl() { texture = NULL ; Initialize() ; }

	static TextureObjectCtrl textureObjectCtrl ;

private:
	// テーブル追加（テクスチャ・オブジェクト数を返す）
	inline int AddTexture(GLint textureName)
	{
		if (nTextures >= nAlloc)
		{
			GLuint *tmp = NULL ;
			if (nTextures) tmp = new GLuint[nTextures] ;
			for (int i = 0 ; i < nTextures ; i ++)
				tmp[i] = texture[i] ;
			delete [] texture ;

			nAlloc += ALLOC_SIZE ;
			texture = new GLuint[nAlloc] ;

			for (int i = 0 ; i < nTextures ; i ++)
				texture[i] = tmp[i] ;
			delete [] tmp ;
		}

		nTextures ++ ;
		texture[nTextures - 1] = textureName ;

		return nTextures ;
	}

public:
	// アクセサ
	inline int NTextures() const { return nTextures ; }
	GLuint TextureName(int i) const
	{
		if (i >= 0 && i < nTextures)
			return texture[i] ;
		return (GLuint)0 ;
	}
	inline GLuint operator [](int i) const { return TextureName(i) ; }

	// メソッド
	inline GLuint AllocateATexture(GLuint& texture)
	{
		glGenTextures(1, &texture) ;
		if (texture)
			AddTexture(texture) ;

		return texture ;
	}

	~TextureObjectCtrl() { if (texture) delete [] texture ; }
} ;


// from nVIDIA cube mapping sample code.
// This "fastTeapot" routine is faster than glutSolidTeapot.
void fastSolidTeapot(GLint grid, GLdouble scale) ;


#ifdef WIN32

// Win32 専用
// ディスプレイモードコントロールクラス
class DisplaySettingsCtrl
{
	int     nSettings ;
	DEVMODE *devmode ;

public:
	DisplaySettingsCtrl() ;
	int EnumAllSettings() ;

	~DisplaySettingsCtrl() ;
} ;

#endif	// #ifdef WIN32


// インライン関数

// 映り込み度 alpha 〜 1,0f までを
// cosa の値（基本は 1.0f〜0.0f）から計算
//
// cosa == 0.0f つまり 視線と平行の時に 1.0f
// cosa == 1.0f つまり視線と正面向きの時に alpha
inline float ReflectAlphaByCos(float cosa, float alpha = 0.5f, float exp = 2.0f)
{
	return ((1.0f - alpha) * Pow((1.0f - cosa), exp) + alpha) ;
}

// 映り込み時の明るさ bright 〜 1.0f までを
// cosa の値（基本は 1.0f〜0.0f）から計算
//
// cosa == 0.0f つまり 視線と平行の時に 1.0f（オリジナルの明るさ）
// cosa == 1.0f つまり視線と正面向きの時に bright
inline float ReflectBrightByCos(float cosa, float bright = 0.01f, float exp = 2.0f)
{
	return ((1.0f - bright) * Pow((1.0f - cosa), exp) + bright) ;
}


#endif
