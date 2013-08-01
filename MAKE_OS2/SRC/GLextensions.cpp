
#include "GLextensions.H"


static char		*current_MultiTextureInterfaceName = NULL ;
static String	current_MultiTextureInterfaceNameString ;
static GLint	current_maxTextureUnits = 1 ;

static char		*current_SeparateSpecularColorInterfaceName = NULL ;
static String	current_SeparateSpecularColorInterfaceNameString ;

static char		*current_TextureFilterAnisotropicInterfaceName = NULL ;
static String	current_TextureFilterAnisotropicInterfaceNameString ;

static char		*current_TextureObjectInterfaceName = NULL ;
static String	current_TextureObjectInterfaceNameString ;

static char		*current_PolygonOffsetInterfaceName = NULL ;
static String	current_PolygonOffsetInterfaceNameString ;

static char		*current_TextureCubeMapInterfaceName = NULL ;
static String	current_TextureCubeMapInterfaceNameString ;
static char		*current_TexgenReflectionInterfaceName = NULL ;
static String	current_TexgenReflectionInterfaceNameString ;

static char		*current_TextureCompressionInterfaceEnumName = NULL ;
static String	current_TextureCompressionInterfaceEnumNameString ;

static char		*current_TextureCompressionS3TCInterfaceName = NULL ;
static String	current_TextureCompressionS3TCInterfaceNameString ;

static char		*current_TextureCompressionFXT1InterfaceName = NULL ;
static String	current_TextureCompressionFXT1InterfaceNameString ;


// OpenGL Extension 関連
#if defined(WIN32) && !defined(MESA)

// ARB_multitexture
PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB   = NULL ;
PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB = NULL ;

// SGIS_multitexture
PFNGLSELECTTEXTURESGISPROC			glSelectTextureSGIS         = NULL ;
PFNGLSELECTTEXTURECOORDSETSGISPROC	glSelectTextureCoordSetSGIS = NULL ;
PFNGLMULTITEXCOORD2FSGISPROC		glMultiTexCoord2fSGIS       = NULL ;

// EXT_multitexture
PFNGLSELECTTEXTUREEXTPROC			glSelectTextureEXT         = NULL ;
PFNGLSELECTTEXTURECOORDSETEXTPROC	glSelectTextureCoordSetEXT = NULL ;
PFNGLMULTITEXCOORD2FEXTPROC			glMultiTexCoord2fEXT       = NULL ;

// EXT_texture_object
PFNGLGENTEXTURESEXTPROC				glGenTexturesEXT    = NULL ;
PFNGLDELETETEXTURESEXTPROC			glDeleteTexturesEXT = NULL ;
PFNGLBINDTEXTUREEXTPROC				glBindTextureEXT    = NULL ;
PFNGLISTEXTUREEXTPROC				glIsTextureEXT      = NULL ;

// EXT_polygon_offset
PFNGLPOLYGONOFFSETEXTPROC			glPolygonOffsetEXT  = NULL ;

#endif	// #if defined(WIN32) && !defined(MESA)


// OpenGL が multitexture をサポートしないバージョンの場合（1.3 でサポート？）
//#ifndef GL_VERSION_multitexture

// マルチテクスチャを統合したインタフェース（識別子は勘 ^^;）
PFNGLACTIVETEXTUREPROC   p_glActiveTexture   = NULL ;
PFNGLMULTITEXCOORD2FPROC p_glMultiTexCoord2f = NULL ;

GLenum DEF_GL_MAX_TEXTURE_UNITS = (GLenum)0 ;

GLenum DEF_GL_TEXTURE0 = (GLenum)0 ;
GLenum DEF_GL_TEXTURE1 = (GLenum)0 ;
GLenum DEF_GL_TEXTURE2 = (GLenum)0 ;
GLenum DEF_GL_TEXTURE3 = (GLenum)0 ;

//#endif	// #ifndef GL_VERSION_multitexture


// テクスチャ・オブジェクト統合インタフェース
PFNGLGENTEXTURESPROC	p_glGenTextures    = NULL ;
PFNGLDELETETEXTURESPROC	p_glDeleteTextures = NULL ;
PFNGLBINDTEXTUREPROC	p_glBindTexture    = NULL ;
PFNGLISTEXTUREPROC		p_glIsTexture      = NULL ;


// ポリゴン・オフセット統合インタフェース
PFNGLPOLYGONOFFSETPROC	p_glPolygonOffset = NULL ;

GLenum DEF_GL_POLYGON_OFFSET_POINT  = (GLenum)0 ;
GLenum DEF_GL_POLYGON_OFFSET_LINE   = (GLenum)0 ;
GLenum DEF_GL_POLYGON_OFFSET_FILL   = (GLenum)0 ;
GLenum DEF_GL_POLYGON_OFFSET_FACTOR = (GLenum)0 ;
GLenum DEF_GL_POLYGON_OFFSET_UNITS  = (GLenum)0 ;


// OpenGL バージョン（小数点以下１桁、つまり左から３桁）
float GetOpenGLVersion()
{
//	static float OpenGL_Version = -1.0f ;
	float OpenGL_Version = -1.0f ;

	if (OpenGL_Version < 1.0f)
	{
		const char *str = (const char *)glGetString(GL_VERSION) ;
		if (str)
		{
			String ver(str, 3) ;	// 左から３桁
			OpenGL_Version = strtod(ver, NULL) ;
		}
		else
			OpenGL_Version = 1.0f ;	// デフォルトは 1.0

		// error?
		if (OpenGL_Version < 1.0f)
			OpenGL_Version = 1.0f ;
	}

	return OpenGL_Version ;
}


// OpenGL Extension サポートチェック
int IsSupported_OpenGLExtension(const String& name)
{
//	static String extensions ;
	String extensions ;

	if (extensions == String(""))
	{
		const char *str = (const char *)glGetString(GL_EXTENSIONS) ;
		if (str)
			extensions = str ;
	}

	if (strstr(extensions, name))
		return TRUE ;
	else
		return FALSE ;
}


/* 使用例
	int has_ARB_multitexture = IsSupported_ARB_multitexture() ;

#ifdef GL_ARB_multitexture
	if (has_ARB_multitexture)
		glActiveTextureARB(GL_TEXTURE0_ARB) ;
#endif
*/
int IsSupported_ARB_multitexture()
{
#ifndef GL_ARB_multitexture
	return FALSE ;

#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_ARB_multitexture))
		return FALSE ;	// ARB_multitexture not supported

	// Get proc address (WGL only)
#if defined(WIN32) && !defined(MESA)
	if (!(glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB")) &&
		!(glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glActiveTextureARB")) &&		//	上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glActiveTextureARB@4")))	//	上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;

	if (!(glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB")) &&
		!(glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glMultiTexCoord2fARB")) &&	//	上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glMultiTexCoord2fARB@12")))	//	上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;
#endif	// #if defined(WIN32) && !defined(MESA)

	p_glActiveTexture   = glActiveTextureARB ;
	p_glMultiTexCoord2f = glMultiTexCoord2fARB ;

	DEF_GL_MAX_TEXTURE_UNITS = GL_MAX_TEXTURE_UNITS_ARB ;

	DEF_GL_TEXTURE0 = GL_TEXTURE0_ARB ;
	DEF_GL_TEXTURE1 = GL_TEXTURE1_ARB ;
	DEF_GL_TEXTURE2 = GL_TEXTURE2_ARB ;
	DEF_GL_TEXTURE3 = GL_TEXTURE3_ARB ;

	current_MultiTextureInterfaceName = CURRENT_NAME_GL_ARB_multitexture ;
	current_MultiTextureInterfaceNameString = current_MultiTextureInterfaceName ;
	return TRUE ;

#endif	// #ifndef GL_ARB_multitexture ... #else
}


/* 使用例
	int has_SGIS_multitexture = IsSupported_SGIS_multitexture() ;

#ifdef GL_SGIS_multitexture
	if (has_SGIS_multitexture)
		glSelectTextureSGIS(GL_TEXTURE0_SGIS) ;
#endif
*/
// Mesa では値が違っていたりするため、
// IsSupported_SGIS_multitexture() は直接コールせず
// IsSupported_multitexture() でまとめた方が無難
// ARB, EXT の方が優先されるため、Mesa では ARB, EXT のいずれかが自動選択されるはず
int IsSupported_SGIS_multitexture()
{
#ifndef GL_SGIS_multitexture
	return FALSE ;

#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_SGIS_multitexture))
		return FALSE ;	// SGIS_multitexture not supported

	// Get proc address (WGL only)
#if defined(WIN32) && !defined(MESA)
	if (!(glSelectTextureSGIS = (PFNGLSELECTTEXTURESGISPROC)wglGetProcAddress("glSelectTextureSGIS")) &&
		!(glSelectTextureSGIS = (PFNGLSELECTTEXTURESGISPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glSelectTextureSGIS")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glSelectTextureSGIS = (PFNGLSELECTTEXTURESGISPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glSelectTextureSGIS@4")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;

	// glMTexCoord* と glMultiTexCoord* の２種類存在するため双方をチェック
	if (!(glMultiTexCoord2fSGIS = (PFNGLMULTITEXCOORD2FSGISPROC)wglGetProcAddress("glMultiTexCoord2fSGIS")) &&
		!(glMultiTexCoord2fSGIS = (PFNGLMULTITEXCOORD2FSGISPROC)wglGetProcAddress("glMTexCoord2fSGIS"    )) &&
		!(glMultiTexCoord2fSGIS = (PFNGLMULTITEXCOORD2FSGISPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glMultiTexCoord2fSGIS"    )) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glMultiTexCoord2fSGIS = (PFNGLMULTITEXCOORD2FSGISPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glMultiTexCoord2fSGIS@12")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glMultiTexCoord2fSGIS = (PFNGLMULTITEXCOORD2FSGISPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glMTexCoord2fSGIS"        )) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glMultiTexCoord2fSGIS = (PFNGLMULTITEXCOORD2FSGISPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glMTexCoord2fSGIS@12"    )))		// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;
#endif	// #if defined(WIN32) && !defined(MESA)

	p_glActiveTexture   = glSelectTextureSGIS ;
	p_glMultiTexCoord2f = glMultiTexCoord2fSGIS ;

#ifndef MESA
	// Mesa 以外
	DEF_GL_TEXTURE0 = GL_TEXTURE0_SGIS ;
	DEF_GL_TEXTURE1 = GL_TEXTURE1_SGIS ;

#else
	// Mesa では何故か定数が異なる
	// いずれにせよ IsSupported_SGIS_multitexture() は直接コールせず
	// IsSupported_multitexture() でまとめた方が無難
	// ARB, EXT の方が優先されるため、Mesa では ARB, EXT のいずれかが自動選択される
	DEF_GL_MAX_TEXTURE_UNITS = GL_MAX_TEXTURES_SGIS ;

	DEF_GL_TEXTURE0 = GL_TEXTURE0_SGIS ;
	DEF_GL_TEXTURE1 = GL_TEXTURE1_SGIS ;
	DEF_GL_TEXTURE2 = GL_TEXTURE2_SGIS ;
	DEF_GL_TEXTURE3 = GL_TEXTURE3_SGIS ;

#endif	// #ifndef MESA ... #else

	current_MultiTextureInterfaceName = CURRENT_NAME_GL_SGIS_multitexture ;
	current_MultiTextureInterfaceNameString = current_MultiTextureInterfaceName ;
	return TRUE ;

#endif	// #ifndef GL_SGIS_multitexture ... #else
}


/* 使用例
	int has_EXT_multitexture = IsSupported_EXT_multitexture() ;

#ifdef GL_EXT_multitexture
	if (has_EXT_multitexture)
		SelectTextureEXT(GL_TEXTURE0_EXT) ;
#endif
*/
int IsSupported_EXT_multitexture()
{
#ifndef GL_EXT_multitexture
	return FALSE ;

#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_multitexture))
		return FALSE ;	// EXT_multitexture not supported

	// Get proc address (WGL only)
#if defined(WIN32) && !defined(MESA)
	if (!(glSelectTextureEXT = (PFNGLSELECTTEXTUREEXTPROC)wglGetProcAddress("glSelectTextureEXT")) &&
		!(glSelectTextureEXT = (PFNGLSELECTTEXTUREEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glSelectTextureEXT")) &&		// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glSelectTextureEXT = (PFNGLSELECTTEXTUREEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glSelectTextureEXT@4")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;

	if (!(glMultiTexCoord2fEXT = (PFNGLMULTITEXCOORD2FEXTPROC)wglGetProcAddress("glMultiTexCoord2fEXT")) &&
		!(glMultiTexCoord2fEXT = (PFNGLMULTITEXCOORD2FEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glMultiTexCoord2fEXT")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glMultiTexCoord2fEXT = (PFNGLMULTITEXCOORD2FEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glMultiTexCoord2fEXT@12")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
			return FALSE ;
#endif	// #if defined(WIN32) && !defined(MESA)

	p_glActiveTexture   = glSelectTextureEXT ;
	p_glMultiTexCoord2f = glMultiTexCoord2fEXT ;

	DEF_GL_MAX_TEXTURE_UNITS = GL_MAX_TEXTURES_EXT ;

	DEF_GL_TEXTURE0 = GL_TEXTURE0_EXT ;
	DEF_GL_TEXTURE1 = GL_TEXTURE1_EXT ;
	DEF_GL_TEXTURE2 = GL_TEXTURE2_EXT ;
	DEF_GL_TEXTURE3 = GL_TEXTURE3_EXT ;

	current_MultiTextureInterfaceName = CURRENT_NAME_GL_EXT_multitexture ;
	current_MultiTextureInterfaceNameString = current_MultiTextureInterfaceName ;
	return TRUE ;

#endif	// #ifndef GL_EXT_multitexture ... #else
}


// 内部専用
// multitexture 使用時の最大テクスチャユニット数
// カレントが Mesa 以外の SGIS_multitexture の場合は２固定
// IsSupported_*_multitexture() が未コールの場合１
GLint __GetMaxTextureUnits()
{
	if (current_MultiTextureInterfaceNameString == String(""))
	{
		current_maxTextureUnits = 1 ;
		return current_maxTextureUnits ;
	}

#ifndef MESA
	if (current_MultiTextureInterfaceNameString == String(CURRENT_NAME_GL_SGIS_multitexture))
	{
		current_maxTextureUnits = 2 ;	// Mesa 以外の SGIS_multitexture では、無条件２とみなす（この時点で処理終了）
		return current_maxTextureUnits ;
	}
#endif	// #ifndef MESA

	// ARB_multitexture or EXT_multitexture、もしくは Mesa の SGIS_multitexture なら、
	// OpenGL から最大テクスチャユニット数を取得
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &current_maxTextureUnits) ;	// テクスチャの最大ユニット数

	return current_maxTextureUnits ;
}


// 何らかの multitexture をサポートするか
// リターン値は サポートする multitexture の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.? Multi-Texture" …………………… OpenGL によるネイティブサポート（OpenGL-1.3 以降？）
// "GL_ARB_multitexture"
// "GL_EXT_multitexture"
// "GL_SGIS_multitexture"
// NULL
//
// コンパイル環境が multitexture を正式サポートするバージョンで、
// かつ、ランタイムが正式サポートしていないバージョンの場合
// ランタイム時にリンクできないため、実行不可能
//
/* 使用例
	char *multitex = IsSupported_multitexture() ;
	if (multitex)
		glActiveTexture(GL_TEXTURE0) ;
*/
char *IsSupported_multitexture(int flag_ARB, int flag_EXT, int flag_SGIS)
{
	current_MultiTextureInterfaceName = NULL ;

#ifdef GL_VERSION_multitexture
	// コンパイル・実行環境双方で、multitexture をサポートするバージョン以降なら
	if (GetOpenGLVersion() >= GL_VERSION_multitexture_F)
		current_MultiTextureInterfaceName = CURRENT_NAME_GL_VERSION_multitexture ;

#endif	// #ifdef GL_VERSION_multitexture

	// ARB_multitexture
	if (!current_MultiTextureInterfaceName && flag_ARB)
		IsSupported_ARB_multitexture() ;

	// EXT_multitexture
	if (!current_MultiTextureInterfaceName && flag_EXT)
		IsSupported_EXT_multitexture() ;

	// SGIS_multitexture
	if (!current_MultiTextureInterfaceName && flag_SGIS)
		IsSupported_SGIS_multitexture() ;

	if (current_MultiTextureInterfaceName)
		current_MultiTextureInterfaceNameString = current_MultiTextureInterfaceName ;

	current_maxTextureUnits = __GetMaxTextureUnits() ;

	return current_MultiTextureInterfaceName ;
}


// multitexture coord 指定
//
// target を指定する場合は、必ず事前に
// IsSupported_multitexture() による使用機能の登録が必要
// 
// IsSupported_SGIS_multitexture() は使用しない方が良い
// また、SGIS では基本的に
// GL_MAX_TEXTURE_UNITS は使用不可（かならず２ユニットとみなした方が良い）
//
// target の指定がない場合 glTexCoord2f() で代用
void TexCoord2f(GLfloat s, GLfloat t, int target)
{
#ifdef GL_VERSION_1_1
	if (target && GL_TEXTURE0)
		glMultiTexCoord2f((GLenum)((int)GL_TEXTURE0 + target), s, t) ;
	else
		glTexCoord2f(s, t) ;
#else
	glTexCoord2f(s, t) ;
#endif
}


// glVertex3f() の代わりにコール可
//
// s, t による座標指定が有効な場合は、
// glVetex() の前に glTexCoord2f() コール
//
// target が指定された場合、
// 各 target の示すテクスチャユニットにテクスチャ座標セット
//
// target を指定する場合は、事前に
// IsSupported_multitexture() による使用機能の登録が必要
// 
// IsSupported_SGIS_multitexture() は使用しない方が良い
// また、SGIS では基本的に
// GL_MAX_TEXTURE_UNITS は使用不可（かならず２ユニットとみなした方が良い）
void Vertex3fWithTexAxis(GLfloat vx, GLfloat vy, GLfloat vz, GLfloat *s0, GLfloat *t0)
{
	Vertex3fWithTexAxis(vx, vy, vz, -1, s0, t0) ;
}

void Vertex3fWithTexAxis(GLfloat vx, GLfloat vy, GLfloat vz,
						 int target0, GLfloat *s0, GLfloat *t0,
						 int target1, GLfloat *s1, GLfloat *t1,
						 int target2, GLfloat *s2, GLfloat *t2,
						 int target3, GLfloat *s3, GLfloat *t3)
{
	// 暫定
	static const int MAX_TEX_UNITS = 4 ;

	GLfloat *s[MAX_TEX_UNITS] ;
	GLfloat *t[MAX_TEX_UNITS] ;
	int      u[MAX_TEX_UNITS] ;

	s[0] = s0 ; t[0] = t0 ; u[0] = target0 ;
	s[1] = s1 ; t[1] = t1 ; u[1] = target1 ;
	s[2] = s2 ; t[2] = t2 ; u[2] = target2 ;
	s[3] = s3 ; t[3] = t3 ; u[3] = target3 ;

	// 各テクスチャユニットに対しテクスチャ座標をセット
	for (int i = 0 ; i < MAX_TEX_UNITS && i < current_maxTextureUnits ; i ++)
	{
		if (s[i] && t[i])
		{
#ifdef GL_VERSION_1_1
			// OpenGL-1.1 以上のみ有効
			if (u[i] > 0 && GL_TEXTURE0)
			{
/*
				if ((int)GL_TEXTURE0 + u[i] != 0x84C0 && (int)GL_TEXTURE0 + u[i] != 0x84C1)
				{
					DEF_GL_TEXTURE0 = GL_TEXTURE0 ;
					DEF_GL_TEXTURE1 = GL_TEXTURE1 ;
				}
*/

				// ターゲット指定されている場合
				glMultiTexCoord2f((GLenum)((int)GL_TEXTURE0 + u[i]),
								  s[i][0] * vx + s[i][1] * vy + s[i][2] * vz,
								  t[i][0] * vx + t[i][1] * vy + t[i][2] * vz) ;
			}
			else
#endif
			{
				// ターゲット指定されていない、もしくは OpenGL-1.0 の場合
				glTexCoord2f(s[i][0] * vx + s[i][1] * vy + s[i][2] * vz,
							 t[i][0] * vx + t[i][1] * vy + t[i][2] * vz) ;

				if (u[i] < 0)
					break ;
			}
		}

#ifndef GL_VERSION_1_1
		// OpenGL-1.0 の場合最初の１つのみ有効
		break ;
#endif
	}

	// 最後に頂点座標セット
	glVertex3f(vx, vy, vz) ;

/*
	if (GL_TEXTURE0 != 0x84C0 || GL_TEXTURE1 != 0x84C1)
	{
			DEF_GL_TEXTURE0 = GL_TEXTURE0 ;
			DEF_GL_TEXTURE1 = GL_TEXTURE1 ;
	}
*/
}


// glVertex3fv() の代わりにコール可
//
// s, t による座標指定が有効な場合は、
// glVetex() の前に glTexCoord2f() コール
//
// target が指定された場合、
// 各 target の示すテクスチャユニットにテクスチャ座標セット
//
// target を指定する場合は、事前に
// IsSupported_multitexture() による使用機能の登録が必要
// 
// IsSupported_SGIS_multitexture() は使用しない方が良い
// また、SGIS では基本的に
// GL_MAX_TEXTURE_UNITS は使用不可（かならず２ユニットとみなした方が良い）
void Vertex3fvWithTexAxis(GLfloat v[3], GLfloat *s0, GLfloat *t0)
{
	Vertex3fvWithTexAxis(v, -1, s0, t0) ;
}

void Vertex3fvWithTexAxis(GLfloat v[3],
						  int target0, GLfloat *s0, GLfloat *t0,
						  int target1, GLfloat *s1, GLfloat *t1,
						  int target2, GLfloat *s2, GLfloat *t2,
						  int target3, GLfloat *s3, GLfloat *t3)
{
	// 暫定
	static const int MAX_TEX_UNITS = 4 ;

	GLfloat *s[MAX_TEX_UNITS] ;
	GLfloat *t[MAX_TEX_UNITS] ;
	int      u[MAX_TEX_UNITS] ;

	s[0] = s0 ; t[0] = t0 ; u[0] = target0 ;
	s[1] = s1 ; t[1] = t1 ; u[1] = target1 ;
	s[2] = s2 ; t[2] = t2 ; u[2] = target2 ;
	s[3] = s3 ; t[3] = t3 ; u[3] = target3 ;


	// 各テクスチャユニットに対しテクスチャ座標をセット
	for (int i = 0 ; i < MAX_TEX_UNITS && i < current_maxTextureUnits ; i ++)
	{
		if (s[i] && t[i])
		{
#ifdef GL_VERSION_1_1
			// OpenGL-1.1 以上のみ有効
			if (u[i] > 0 && GL_TEXTURE0)
			{
/*
				if ((int)GL_TEXTURE0 + u[i] != 0x84C0 && (int)GL_TEXTURE0 + u[i] != 0x84C1)
				{
					DEF_GL_TEXTURE0 = GL_TEXTURE0 ;
					DEF_GL_TEXTURE1 = GL_TEXTURE1 ;
				}
*/

				// ターゲット指定されている場合
				glMultiTexCoord2f((GLenum)((int)GL_TEXTURE0 + u[i]),
								  s[i][0]*v[0] + s[i][1]*v[1] + s[i][2]*v[2],
								  t[i][0]*v[0] + t[i][1]*v[1] + t[i][2]*v[2]) ;
			}
			else
#endif
			{
				// ターゲット指定されていない、もしくは OpenGL-1.0 の場合
				glTexCoord2f(s[i][0]*v[0] + s[i][1]*v[1] + s[i][2]*v[2],
							 t[i][0]*v[0] + t[i][1]*v[1] + t[i][2]*v[2]) ;

				if (u[i] < 0)
					break ;
			}
		}

#ifndef GL_VERSION_1_1
		// OpenGL-1.0 の場合最初の１つのみ有効
		break ;
#endif
	}

	// 最後に頂点座標セット
	glVertex3fv(v) ;

/*
	if (GL_TEXTURE0 != 0x84C0 || GL_TEXTURE1 != 0x84C1)
	{
			DEF_GL_TEXTURE0 = GL_TEXTURE0 ;
			DEF_GL_TEXTURE1 = GL_TEXTURE1 ;
	}
*/
}


// multitexture 使用時の最大テクスチャユニット数
// カレントが Mesa 以外の SGIS_multitexture の場合は２固定
// IsSupported_*_multitexture() が未コールの場合１
GLint GetMaxTextureUnits()
{
	return current_maxTextureUnits ;
}


// multitexture 使用時の最大テクスチャユニット数
// カレントが Mesa 以外の SGIS_multitexture の場合は２固定
// IsSupported_*_multitexture() が未コールの場合１
/*
GLint GetMaxTextureUnits()
{
	static String pre_MultiTextureInterfaceName ;

	if (!current_MultiTextureInterfaceName)
		maxTextureUnits = 1 ;

	if (current_MultiTextureInterfaceNameString == pre_MultiTextureInterfaceName)
		return maxTextureUnits ;

	// 新たにマルチテクスチャインタフェースをセットされている場合は
	// glGet*() で取得
	pre_MultiTextureInterfaceName = current_MultiTextureInterfaceNameString ;

#ifndef MESA
	if (current_MultiTextureInterfaceNameString == CURRENT_NAME_GL_SGIS_multitexture)
	{
		maxTextureUnits = 2 ;	// Mesa 以外の SGIS_multitexture では、無条件２とみなす（この時点で処理終了）
		return maxTextureUnits ;
	}
#endif	// #ifndef MESA

	// ARB_multitexture or EXT_multitexture、もしくは Mesa の SGIS_multitexture なら、
	// OpenGL から最大テクスチャユニット数を取得
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits) ;	// テクスチャの最大ユニット数

	return maxTextureUnits ;
}
*/


// anisotropic filtering 使用時の最大 max anisotropy
GLfloat GetMaxTextureMaxAnisotropy()
{
	if (current_TextureFilterAnisotropicInterfaceNameString == String(""))
		return 1.0f ;

	GLfloat maxTextureMaxAnisotropy ;
	GLenum errCode = glGetError() ;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxTextureMaxAnisotropy) ;	// maximum degree of anisotropy

	// for fixing nVIDIA old driver's anisotropic filtering error...
	// ignore "Invalid Enum" Error
	errCode = glGetError() ;
	if (errCode == GL_INVALID_ENUM)
		maxTextureMaxAnisotropy = 1.0f ;

	return maxTextureMaxAnisotropy ;
}


// フォン・シェーディング
// WIN_phong_shading
// 関数の追加はなく、パラメタの追加のみであるため、ランタイムのみの対応で OK
//
/* 使用例
	int phongshade = IsSupported_WIN_phong_shading() ;

	// 関数の追加がないため #ifdef は不要
	if (phongshade)
		glShadeModel(GL_PHONG_WIN) ;
*/
int IsSupported_WIN_phong_shading()
{
//#ifndef GL_WIN_phong_shading
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension("GL_WIN_phong_shading"))
		return FALSE ;	// WIN_phong_shading not supported

	return TRUE ;

//#endif	// #ifndef GL_WIN_phong_shading ... #else
}


// GL_ARB_texture_cube_map サポートチェック
//
/* 使用例
	int has_ARB_texture_cube_map = IsSupported_ARB_texture_cube_map() ;

	// 関数の追加がないため #ifdef は不要
	if (has_ARB_texture_cube_map)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_ARB) ;

	if (has_ARB_texture_cube_map)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB) ;
*/
int IsSupported_ARB_texture_cube_map()
{
//#ifndef GL_EXT_texture_cube_map
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_ARB_texture_cube_map))
		return FALSE ;	// GL_ARB_texture_cube_map not supported

	current_TextureCubeMapInterfaceName = CURRENT_NAME_GL_ARB_texture_cube_map ;
	current_TextureCubeMapInterfaceNameString = current_TextureCubeMapInterfaceName ;

	current_TexgenReflectionInterfaceName = CURRENT_NAME_GL_ARB_texture_cube_map ;
	current_TexgenReflectionInterfaceNameString = current_TexgenReflectionInterfaceName ;

	return TRUE ;
//#endif	// #ifndef GL_ARB_texture_cube_map
}

// GL_EXT_texture_cube_map サポートチェック
//
/* 使用例
	int has_EXT_texture_cube_map = IsSupported_EXT_texture_cube_map() ;

	// 関数の追加がないため #ifdef は不要
	if (has_EXT_texture_cube_map)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT) ;

	if (has_EXT_texture_cube_map)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT) ;
*/
int IsSupported_EXT_texture_cube_map()
{
//#ifndef GL_EXT_texture_cube_map
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_texture_cube_map))
		return FALSE ;	// GL_EXT_texture_cube_map not supported

	current_TextureCubeMapInterfaceName = CURRENT_NAME_GL_EXT_texture_cube_map ;
	current_TextureCubeMapInterfaceNameString = current_TextureCubeMapInterfaceName ;

	current_TexgenReflectionInterfaceName = CURRENT_NAME_GL_EXT_texture_cube_map ;
	current_TexgenReflectionInterfaceNameString = current_TexgenReflectionInterfaceName ;

	return TRUE ;
//#endif	// #ifndef GL_EXT_texture_cube_map
}


// GL_NV_texgen_reflection サポートチェック
//
/* 使用例
	int has_NV_texgen_reflection = IsSupported_NV_texgen_reflection() ;

	// 関数の追加がないため #ifdef は不要
	if (has_NV_texgen_reflection)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_NV) ;

	if (has_NV_texgen_reflection)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_NV) ;
*/
int IsSupported_NV_texgen_reflection()
{
//#ifndef GL_NV_texgen_reflection
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_NV_texgen_reflection))
		return FALSE ;	// GL_NV_texgen_reflection not supported

	current_TexgenReflectionInterfaceName = CURRENT_NAME_GL_NV_texgen_reflection ;
	current_TexgenReflectionInterfaceNameString = current_TexgenReflectionInterfaceName ;
	return TRUE ;
//#endif	// #ifndef GL_NV_texgen_reflection
}


// texgen_reflection 関連サポートチェック
// GL_ARB_texture_cube_map もしくは
// GL_EXT_texture_cube_map もしくは
// GL_NV_texgen_reflection をサポートしているか
//
/* 使用例
	char *texgen_reflection = IsSupported_texgen_reflection() ;

	// 関数の追加がないため #ifdef は不要
	if (texgen_reflection)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP) ;

	if (texgen_reflection)
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP) ;
*/
char *IsSupported_texgen_reflection(int flag_ARB_texture_cube_map,
									int flag_EXT_texture_cube_map,
									int flag_NV_texgen_reflection)
{
	// GL_ARB_texture_cube_map
	if (flag_ARB_texture_cube_map && IsSupported_ARB_texture_cube_map())
		return current_TexgenReflectionInterfaceName ;

	// GL_EXT_texture_cube_map
	if (flag_EXT_texture_cube_map && IsSupported_EXT_texture_cube_map())
		return current_TexgenReflectionInterfaceName ;

	// GL_NV_texgen_reflection
	if (flag_NV_texgen_reflection && IsSupported_NV_texgen_reflection())
		return current_TexgenReflectionInterfaceName ;

	// not supported
	return NULL ;
}


// Separate Specular Color 関連
// 仕様が分からないため保留
// 現時点では、OpenGL-1.2 のパラメタをそのまま使用

// ARB_separate_specular_color
/* 使用例
	int has_ARB_separate_specular_color = IsSupported_ARB_separate_specular_color() ;

	// 関数の追加がないため #ifdef は不要
	if (has_ARB_separate_specular_color)
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_ARB, GL_SEPARATE_SPECULAR_COLOR_ARB) ;

	if (has_ARB_separate_specular_color)
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_ARB, GL_SINGLE_COLOR_ARB) ;
*/
int IsSupported_ARB_separate_specular_color()
{
//#ifndef GL_ARB_separate_specular_color
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_ARB_separate_specular_color))
		return FALSE ;	// ARB_separate_specular_color not supported

	current_SeparateSpecularColorInterfaceName = CURRENT_NAME_GL_ARB_separate_specular_color ;
	current_SeparateSpecularColorInterfaceNameString = current_SeparateSpecularColorInterfaceName ;
	return TRUE ;

//#endif	// #ifndef GL_ARB_separate_specular_color ... #else
}

// EXT_separate_specular_color
/* 使用例
	int has_EXT_separate_specular_color = IsSupported_EXT_separate_specular_color() ;

	// 関数の追加がないため #ifdef は不要
	if (has_EXT_separate_specular_color)
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SEPARATE_SPECULAR_COLOR_EXT) ;

  if (has_EXT_separate_specular_color)
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL_EXT, GL_SINGLE_COLOR_EXT) ;
*/
int IsSupported_EXT_separate_specular_color()
{
//#ifndef GL_EXT_separate_specular_color
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_separate_specular_color))
		return FALSE ;	// EXT_separate_specular_color not supported

	current_SeparateSpecularColorInterfaceName = CURRENT_NAME_GL_EXT_separate_specular_color ;
	current_SeparateSpecularColorInterfaceNameString = current_SeparateSpecularColorInterfaceName ;
	return TRUE ;

//#endif	// #ifndef GL_EXT_separate_specular_color ... #else
}


// 何らかの separate_specular_color をサポートするか
// リターン値は サポートする separate_specular_color の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.2 Separate Specular Ccolor" ……… OpenGL によるネイティブサポート（OpenGL-1.2 以降）
// "GL_ARB_separate_specular_color"
// "GL_EXT_separate_specular_color"
// NULL
//
/* 使用例
	char *separate_specular_color = IsSupported_separate_specular_color() ;

	// 関数の追加がないため #ifdef は不要
	if (separate_specular_color)
		glLightModeli(GL_COLOR_LIGHT_MODEL, GL_SEPARATE_SPECULAR_COLOR) ;

	if (separate_specular_color)
		glLightModeli(GL_COLOR_LIGHT_MODEL, GL_SINGLE_COLOR) ;
*/
char *IsSupported_separate_specular_color(int flag_ARB, int flag_EXT)
{
//#ifdef GL_VERSION_separate_specular_color	// コンパイル・実行環境双方で、separate_specular_color をサポートするバージョン以降なら

	// ランタイムが separate_specular_color をサポートするバージョン以降なら OK
	// パラメタの追加のみで良いため
	if (GetOpenGLVersion() >= GL_VERSION_separate_specular_color_F)
	{
		current_SeparateSpecularColorInterfaceName = CURRENT_NAME_GL_VERSION_separate_specular_color ;
		current_SeparateSpecularColorInterfaceNameString = current_SeparateSpecularColorInterfaceName ;
		return current_SeparateSpecularColorInterfaceName ;
	}

//#endif	// #ifdef GL_VERSION_separate_specular_color	// チェック不要

	// ARB_separate_specular_color
	if (flag_ARB && IsSupported_ARB_separate_specular_color())
		return current_SeparateSpecularColorInterfaceName ;

	// EXT_separate_specular_color
	if (flag_EXT && IsSupported_EXT_separate_specular_color())
		return current_SeparateSpecularColorInterfaceName ;

	// not supported
	return NULL ;
}


// Texture Filter Anisotropic 関連

// EXT_texture_filter_anisotropic
/* 使用例
	int has_EXT_texture_filter_anisotropic = IsSupported_EXT_texture_filter_anisotropic() ;

	// 関数の追加がないため #ifdef は不要
	if (has_EXT_texture_filter_anisotropic)
	{
		float max_anisotropy ;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy) ;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy) ;
	}
*/
int IsSupported_EXT_texture_filter_anisotropic()
{
//#ifndef GL_EXT_texture_filter_anisotropic
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_texture_filter_anisotropic))
		return FALSE ;	// EXT_texture_filter_anisotropic not supported

	current_TextureFilterAnisotropicInterfaceName = CURRENT_NAME_GL_EXT_texture_filter_anisotropic ;
	current_TextureFilterAnisotropicInterfaceNameString = current_TextureFilterAnisotropicInterfaceName ;
	return TRUE ;

//#endif	// #ifndef GL_EXT_texture_filter_anisotropic ... #else
}


// 何らかの texture_filter_anisotropic をサポートするか
// リターン値は サポートする texture_filter_anisotropic の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.? Texture Filter Anisotropic" ……… OpenGL によるネイティブサポート（OpenGL-未定 以降）
// "GL_EXT_texture_filter_anisotropic"
// NULL
//
/* 使用例
	char *texture_filter_anisotropic = IsSupported_texture_filter_anisotropic() ;

	// 関数の追加がないため #ifdef は不要
	if (texture_filter_anisotropic)
	{
		float max_anisotropy ;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max_anisotropy) ;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max_anisotropy) ;
	}
*/
char *IsSupported_texture_filter_anisotropic(int flag_EXT)
{
//#ifdef GL_VERSION_texture_filter_anisotropic	// コンパイル・実行環境双方で、separate_specular_color をサポートするバージョン以降なら

	// ランタイムが texture_filter_anisotropic をサポートするバージョン以降なら OK
	// パラメタの追加のみで良いため
	if (GetOpenGLVersion() >= GL_VERSION_texture_filter_anisotropic_F)
	{
		current_TextureFilterAnisotropicInterfaceName = CURRENT_NAME_GL_VERSION_texture_filter_anisotropic ;
		current_TextureFilterAnisotropicInterfaceNameString = current_TextureFilterAnisotropicInterfaceName ;
		return current_TextureFilterAnisotropicInterfaceName ;
	}

//#endif	// #ifdef GL_VERSION_texture_filter_anisotropic	// チェック不要

	// EXT_texture_filter_anisotropic
	if (flag_EXT && IsSupported_EXT_texture_filter_anisotropic())
		return current_TextureFilterAnisotropicInterfaceName ;

	// not supported
	return NULL ;
}


// Texture Object 関連

// EXT_texture_object
/* 使用例
	int has_EXT_texture_object = IsSupported_EXT_texture_object() ;

#ifdef GL_EXT_texture_object
	if (has_EXT_texture_object)
		BindTextureEXT(GL_TEXTURE_2D, texture) ;
#endif
*/
int IsSupported_EXT_texture_object()
{
#ifndef GL_EXT_texture_object
	return FALSE ;

#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_texture_object))
		return FALSE ;	// GL_EXT_texture_object not supported

	// Get proc address (WGL only)
#if defined(WIN32) && !defined(MESA)
	if (!(glGenTexturesEXT = (PFNGLGENTEXTURESEXTPROC)wglGetProcAddress("glGenTexturesEXT")) &&
		!(glGenTexturesEXT = (PFNGLGENTEXTURESEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glGenTexturesEXT")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glGenTexturesEXT = (PFNGLGENTEXTURESEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glGenTexturesEXT@8")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;

	if (!(glDeleteTexturesEXT = (PFNGLDELETETEXTURESEXTPROC) wglGetProcAddress("glDeleteTexturesEXT")) &&
		!(glDeleteTexturesEXT = (PFNGLDELETETEXTURESEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glDeleteTexturesEXT")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glDeleteTexturesEXT = (PFNGLDELETETEXTURESEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glDeleteTexturesEXT@8")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;

	if (!(glBindTextureEXT = (PFNGLBINDTEXTUREEXTPROC)wglGetProcAddress("glBindTextureEXT")) &&
		!(glBindTextureEXT = (PFNGLBINDTEXTUREEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glBindTextureEXT")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glBindTextureEXT = (PFNGLBINDTEXTUREEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glBindTextureEXT@8")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;

	if (!(glIsTextureEXT = (PFNGLISTEXTUREEXTPROC)wglGetProcAddress("glIsTextureEXT")) &&
		!(glIsTextureEXT = (PFNGLISTEXTUREEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glIsTextureEXT")) &&	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glIsTextureEXT = (PFNGLISTEXTUREEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glIsTextureEXT@4")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;
#endif	// #if defined(WIN32) && !defined(MESA)

	p_glGenTextures    = glGenTexturesEXT ;
	p_glDeleteTextures = glDeleteTexturesEXT ;
	p_glBindTexture    = glBindTextureEXT ;
	p_glIsTexture      = glIsTextureEXT ;

	current_TextureObjectInterfaceName = CURRENT_NAME_GL_EXT_texture_object ;
	current_TextureObjectInterfaceNameString = current_TextureObjectInterfaceName ;
	return TRUE ;

#endif	// #ifndef GL_EXT_texture_object ... #else
}


// 何らかの texture_object をサポートするか
// リターン値は サポートする texture_object の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.1 Texture Object" ……… OpenGL によるネイティブサポート（OpenGL-1.1 以降）
// "GL_EXT_texture_object"
// NULL
//
/* 使用例
	char *texture_object = IsSupported_texture_object() ;

	if (texture_object)
		glBindTexture(GL_TEXTURE_2D, texture) ;
*/
char *IsSupported_texture_object(int flag_EXT)
{
#ifdef GL_VERSION_texture_object
	// コンパイル・実行環境双方で、texture_object をサポートするバージョン以降なら
	if (GetOpenGLVersion() >= GL_VERSION_texture_object_F)
	{
		current_TextureObjectInterfaceName = CURRENT_NAME_GL_VERSION_texture_object ;
		current_TextureObjectInterfaceNameString = current_TextureObjectInterfaceName ;
		return current_TextureObjectInterfaceName ;
	}

#endif	// #ifdef GL_VERSION_separate_specular_color

	// EXT_texture_object
	if (flag_EXT && IsSupported_EXT_texture_object())
		return current_TextureObjectInterfaceName ;

	// not supported
	return NULL ;
}


// Polygon Offset 関連

// EXT_polygon_offset
/* 使用例
	int has_EXT_polygon_offset = IsSupported_EXT_polygon_offset() ;

#ifdef GL_EXT_polygon_offset
	if (has_EXT_polygon_offset)
	{
		glPolygonOffsetEXT(units, bias) ;
		glEnable(GL_POLYGON_OFFSET_EXT) ;

		glGetFloatv(GL_POLYGON_OFFSET_FACTOR_EXT, &facter) ;
		glGetFloatv(GL_POLYGON_OFFSET_BIAS_EXT, &bias) ;
	}
#endif
*/
int IsSupported_EXT_polygon_offset()
{
#ifndef GL_EXT_polygon_offset
	return FALSE ;

#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_texture_object))
		return FALSE ;	// GL_EXT_polygon_offset not supported

	// Get proc address (WGL only)
#if defined(WIN32) && !defined(MESA)
	if (!(glPolygonOffsetEXT = (PFNGLPOLYGONOFFSETEXTPROC)wglGetProcAddress("glPolygonOffsetEXT")) &&
		!(glPolygonOffsetEXT = (PFNGLPOLYGONOFFSETEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("glPolygonOffsetEXT")) &&		// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		!(glPolygonOffsetEXT = (PFNGLPOLYGONOFFSETEXTPROC)OpenGLDLLCtrl::openGLDLLCtrl.GetProcAddress("_glPolygonOffsetEXT@8")))	// 上で取得できなかったときは、DLL からインポート（Mesa DLL 用）
		return FALSE ;
#endif	// #if defined(WIN32) && !defined(MESA)

	p_glPolygonOffset = glPolygonOffsetEXT ;

	DEF_GL_POLYGON_OFFSET_POINT  = GL_POLYGON_OFFSET_EXT ;	// EXT_polygon_offset では、すべて共通（有効なのは FILL のみ？）
	DEF_GL_POLYGON_OFFSET_LINE   = GL_POLYGON_OFFSET_EXT ;
	DEF_GL_POLYGON_OFFSET_FILL   = GL_POLYGON_OFFSET_EXT ;
	DEF_GL_POLYGON_OFFSET_FACTOR = GL_POLYGON_OFFSET_FACTOR_EXT ;
	DEF_GL_POLYGON_OFFSET_UNITS  = GL_POLYGON_OFFSET_BIAS_EXT ;

	current_PolygonOffsetInterfaceName = CURRENT_NAME_GL_EXT_polygon_offset ;
	current_PolygonOffsetInterfaceNameString = current_PolygonOffsetInterfaceName ;
	return TRUE ;

#endif	// #ifndef GL_EXT_polygon_offset ... #else
}


// 何らかの polygon_offset をサポートするか
// リターン値は サポートする polygon_offset の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.1 Polygon Offset" ……… OpenGL によるネイティブサポート（OpenGL-1.1 以降）
// "GL_EXT_polygon_offset"
// NULL
//
/* 使用例
	char *polygon_offset = IsSupported_polygon_offset() ;

	if (polygon_offset)
	{
		glPolygonOffset(facter, units) ;
		glEnable(GL_POLYGON_OFFSET_FILL) ;	// EXT_polygon_offset の場合 POINT, LINE も共通

		glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &facter) ;
		glGetFloatv(GL_POLYGON_OFFSET_UNITS, &units) ;
	}
*/
char *IsSupported_polygon_offset(int flag_EXT)
{
#ifdef GL_VERSION_polygon_offset
	// コンパイル・実行環境双方で、polygon_offset をサポートするバージョン以降なら
	if (GetOpenGLVersion() >= GL_VERSION_polygon_offset_F)
	{
		current_PolygonOffsetInterfaceName = CURRENT_NAME_GL_VERSION_polygon_offset ;
		current_PolygonOffsetInterfaceNameString = current_PolygonOffsetInterfaceName ;
		return current_PolygonOffsetInterfaceName ;
	}

#endif	// #ifdef GL_VERSION_separate_specular_color

	// EXT_polygon_offset
	if (flag_EXT && IsSupported_EXT_polygon_offset())
		return current_PolygonOffsetInterfaceName ;

	// not supported
	return NULL ;
}



// Texture Compression Enum 関連

// GL_ARB_texture_compression
/* 使用例
	int hasEnum_ARB_texture_compression = IsSupportedEnum_ARB_texture_compression() ;

	// 関数の追加がないため #ifdef は不要
	if (hasEnum_ARB_texture_compression)
	{
		glTexImage2D( ..., GL_COMPRESSED_RGB_ARB, ...) ;
	}
*/
int IsSupported_ARB_texture_compression_Enum()
{
//#ifndef GL_ARB_texture_compression
//	return FALSE ;
//
//#else
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_ARB_texture_compression))
		return FALSE ;	// GL_ARB_texture_compression not supported

	current_TextureCompressionInterfaceEnumName = CURRENT_NAME_GL_ARB_texture_compression ;
	current_TextureCompressionInterfaceEnumNameString = current_TextureCompressionInterfaceEnumName ;
	return TRUE ;

//#endif	// #ifndef GL_ARB_texture_compression ... #else
}


// 何らかの texture_compression をサポートするか
// リターン値は サポートする texture_compression の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.? Texture Compression" ……… OpenGL によるネイティブサポート（OpenGL-未定 以降）
// "GL_ARB_texture_compression"
// NULL
//
/* 使用例
	char *enum_texture_compression = IsSupportedEnum_texture_compression() ;

	// 関数の追加がないため #ifdef は不要
	if (enum_texture_compression)
	{
		glTexImage2D( ..., GL_COMPRESSED_RGB, ...) ;
	}
*/
char *IsSupported_texture_compression_Enum(int flag_ARB)
{
//#ifdef GL_VERSION_texture_compression	// コンパイル・実行環境双方で、texture_compression をサポートするバージョン以降なら

	// ランタイムが texture_compression をサポートするバージョン以降なら OK
	// パラメタの追加のみで良いため
	if (GetOpenGLVersion() >= GL_VERSION_texture_compression_F)
	{
		current_TextureCompressionInterfaceEnumName = CURRENT_NAME_GL_VERSION_texture_compression ;
		current_TextureCompressionInterfaceEnumNameString = current_TextureCompressionInterfaceEnumName ;
		return current_TextureCompressionInterfaceEnumName ;
	}

//#endif	// #ifdef GL_VERSION_texture_compression	// チェック不要

	// ARB_texture_compression
	if (flag_ARB && IsSupported_ARB_texture_compression_Enum())
		return current_TextureCompressionInterfaceEnumName ;

	// not supported
	return NULL ;
}


// Texture Compression S3TC 関連

// GL_EXT_texture_compression_s3tc
/* 使用例
	int has_EXT_texture_compression_s3tc = IsSupported_EXT_texture_compression_s3tc() ;

	// 関数の追加がないため #ifdef は不要
	if (has_EXT_texture_compression_s3tc)
	{
		glTexImage2D( ..., GL_COMPRESSED_RGB_S3TC_DXT1_EXT, ...) ;
	}
*/
int IsSupported_EXT_texture_compression_s3tc()
{
//#ifndef GL_EXT_texture_compression_s3tc
//	return FALSE ;
//
//#else
	// GL_ARB_texture_compression 必須
	// GL_ARB_texture_compression 必須
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_ARB_texture_compression) ||
		!IsSupported_OpenGLExtension(CURRENT_NAME_GL_EXT_texture_compression_s3tc))
		return FALSE ;	// GL_EXT_texture_compression_s3tc not supported

	current_TextureCompressionS3TCInterfaceName = CURRENT_NAME_GL_EXT_texture_compression_s3tc ;
	current_TextureCompressionS3TCInterfaceNameString = current_TextureCompressionS3TCInterfaceName ;
	return TRUE ;

//#endif	// #ifndef GL_EXT_texture_compression_s3tc ... #else
}


// 何らかの texture_compression_s3tc をサポートするか
// リターン値は サポートする texture_compression_s3tc の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.? Texture Compression S3TC" ……… OpenGL によるネイティブサポート（される訳ない？）
// "GL_EXT_texture_compression_s3tc"
// NULL
//
/* 使用例
	char *texture_compression_S3TC = IsSupported_texture_compression_s3tc() ;

	// 関数の追加がないため #ifdef は不要
	if (texture_compression_S3TC)
	{
		glTexImage2D( ..., GL_COMPRESSED_RGB_S3TC_DXT1, ...) ;
	}
*/
char *IsSupported_texture_compression_s3tc(int flag_EXT)
{
	// ランタイムが texture_compression_s3tc をサポートするバージョン以降なら OK
	// パラメタの追加のみで良いため
	if (GetOpenGLVersion() >= GL_VERSION_texture_compression_s3tc_F)
	{
		current_TextureCompressionS3TCInterfaceName = CURRENT_NAME_GL_VERSION_texture_compression_s3tc ;
		current_TextureCompressionS3TCInterfaceNameString = current_TextureCompressionS3TCInterfaceName ;
		return current_TextureCompressionS3TCInterfaceName ;
	}

	// EXT_texture_compression_s3tc
	if (flag_EXT && IsSupported_EXT_texture_compression_s3tc())
		return current_TextureCompressionS3TCInterfaceName ;

	// not supported
	return NULL ;
}


// Texture Compression FXT1 関連

// GL_3DFX_texture_compression_FXT1
/* 使用例
	int has_3DFX_texture_compression_FXT1 = IsSupported_3DFX_texture_compression_FXT1() ;

	// 関数の追加がないため #ifdef は不要
	if (has_3DFX_texture_compression_FXT1)
	{
		glTexImage2D( ..., GL_COMPRESSED_RGB_FXT1_3DFX, ...) ;
	}
*/
int IsSupported_3DFX_texture_compression_FXT1()
{
//#ifndef GL_3DFX_texture_compression_FXT1
//	return FALSE ;
//
//#else
	// GL_ARB_texture_compression 必須
	if (!IsSupported_OpenGLExtension(CURRENT_NAME_GL_ARB_texture_compression) ||
		!IsSupported_OpenGLExtension(CURRENT_NAME_GL_3DFX_texture_compression_FXT1))
		return FALSE ;	// GL_EXT_texture_compression_s3tc not supported

	current_TextureCompressionFXT1InterfaceName = CURRENT_NAME_GL_3DFX_texture_compression_FXT1 ;
	current_TextureCompressionFXT1InterfaceNameString = current_TextureCompressionFXT1InterfaceName ;
	return TRUE ;

//#endif	// #ifndef GL_3DFX_texture_compression_FXT1 ... #else
}


// 何らかの texture_compression_FXT1 をサポートするか
// リターン値は サポートする texture_compression_FXT1 の名称ポインタまたは NULL
// 優先順位
// "OpenGL-1.? Texture Compression FXT1" ……… OpenGL によるネイティブサポート（される訳ない？）
// "GL_3DFX_texture_compression_FXT1"
// NULL
//
/* 使用例
	char *texture_compression_FXT1 = IsSupported_texture_compression_FXT1() ;

	// 関数の追加がないため #ifdef は不要
	if (texture_compression_FXT1)
	{
		glTexImage2D( ..., GL_COMPRESSED_RGB_FXT1, ...) ;
	}
*/
char *IsSupported_texture_compression_FXT1(int flag_3DFX)
{
	// ランタイムが texture_compression_FXT1 をサポートするバージョン以降なら OK
	// パラメタの追加のみで良いため
	if (GetOpenGLVersion() >= GL_VERSION_texture_compression_FXT1_F)
	{
		current_TextureCompressionFXT1InterfaceName = CURRENT_NAME_GL_VERSION_texture_compression_FXT1 ;
		current_TextureCompressionFXT1InterfaceNameString = current_TextureCompressionFXT1InterfaceName ;
		return current_TextureCompressionFXT1InterfaceName ;
	}

	// 3DFX_texture_compression_FXT1
	if (flag_3DFX && IsSupported_3DFX_texture_compression_FXT1())
		return current_TextureCompressionFXT1InterfaceName ;

	// not supported
	return NULL ;
}



// 現在有効になっている統合インタフェース
String CurrentInterface_multitexture()
{
	return current_MultiTextureInterfaceNameString ;
}

String CurrentInterface_separate_specular_color()
{
	return current_SeparateSpecularColorInterfaceNameString ;
}

String CurrentInterface_texture_object()
{
	return current_TextureObjectInterfaceNameString ;
}

String CurrentInterface_polygon_offset()
{
	return current_PolygonOffsetInterfaceNameString ;
}

String CurrentInterface_texture_cube_map()
{
	return current_TextureCubeMapInterfaceNameString ;
}

String CurrentInterface_texgen_reflection()
{
	return current_TexgenReflectionInterfaceNameString ;
}

String CurrentInterfaceEnum_texture_compression()
{
	return current_TextureCompressionInterfaceEnumNameString ;
}

String CurrentInterface_texture_compression_s3tc()
{
	return current_TextureCompressionS3TCInterfaceNameString ;
}


// Texture Internal Formats
// Default: 3
GLint InternalFormat(String str)
{
	GLint format ;

	format = (int)(str.Character(1) - '0') ;
	if (format < 0 || format > 4)
		format = 0 ;

	str.Upper() ;
	if (!format && Left(str, 3) != String("GL_"))
		str = "GL_" + str ;

	// "_ALPHA" がなく "_A" があり "RGB" が無い場合
	// !"RGB" は、 RGB5_A1 等に反応させない為
	if (!(str / "_ALPHA") && (str / "_A") && !(str / "RGB"))
		str.ReplaceString("_A", "_ALPHA") ;

	if (!(str / "_LUMINANCE"))	str.ReplaceString("_L", "_LUMINANCE") ;
	if (!(str / "_INTENSITY"))	str.ReplaceString("_I", "_INTENSITY") ;
	if (!(str / "_COMPRESSED"))	str.ReplaceString("_C", "_COMPRESSED") ;

	if (GetOpenGLVersion() >= 1.1f)
	{
		format = GL_RGB ;	// default internal format = GL_RGB

		// Is texture_compression available
		if (current_TextureCompressionInterfaceEnumName)
		{
			// Is texture_compression_s3tc available
			if (current_TextureCompressionS3TCInterfaceName)
			{
				if      (str / "GL_COMPRESSED_RGB_S3TC_DXT1")	format = GL_COMPRESSED_RGB_S3TC_DXT1 ;
				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT1")	format = GL_COMPRESSED_RGBA_S3TC_DXT1 ;
				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT3")	format = GL_COMPRESSED_RGBA_S3TC_DXT3 ;
				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT5")	format = GL_COMPRESSED_RGBA_S3TC_DXT5 ;
			}
			else
			{
				if      (str / "GL_COMPRESSED_RGB_S3TC_DXT1")	format = GL_COMPRESSED_RGB ;
				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT")	format = GL_COMPRESSED_RGBA ;	// RGBA_DXT?
//				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT1")	format = GL_COMPRESSED_RGBA ;
//				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT3")	format = GL_COMPRESSED_RGBA ;
//				else if (str / "GL_COMPRESSED_RGBA_S3TC_DXT5")	format = GL_COMPRESSED_RGBA ;
			}

			// Is texture_compression_FXT1 available
			if (current_TextureCompressionFXT1InterfaceName)
			{
				if      (str / "GL_COMPRESSED_RGB_FXT1")	format = GL_COMPRESSED_RGB_FXT1 ;
				else if (str / "GL_COMPRESSED_RGBA_FXT1")	format = GL_COMPRESSED_RGBA_FXT1 ;
			}
			else
			{
				if      (str / "GL_COMPRESSED_RGB_FXT1")	format = GL_COMPRESSED_RGB ;
				else if (str / "GL_COMPRESSED_RGBA_FXT1")	format = GL_COMPRESSED_RGBA ;
			}

			if      (str == String("GL_COMPRESSED_ALPHA")           || str == String("GL_COMPRESSED_ALPHA_ARB"))			format = GL_COMPRESSED_ALPHA ;
			else if (str == String("GL_COMPRESSED_LUMINANCE")       || str == String("GL_COMPRESSED_LUMINANCE_ARB"))		format = GL_COMPRESSED_LUMINANCE ;
			else if (str == String("GL_COMPRESSED_LUMINANCE_ALPHA") || str == String("GL_COMPRESSED_LUMINANCE_ALPHA_ARB"))	format = GL_COMPRESSED_LUMINANCE_ALPHA ;
			else if (str == String("GL_COMPRESSED_INTENSITY")       || str == String("GL_COMPRESSED_INTENSITY_ARB"))		format = GL_COMPRESSED_INTENSITY ;
			else if (str == String("GL_COMPRESSED_RGB")             || str == String("GL_COMPRESSED_RGB_ARB"))				format = GL_COMPRESSED_RGB ;
			else if (str == String("GL_COMPRESSED_RGBA")            || str == String("GL_COMPRESSED_RGBA_ARB"))				format = GL_COMPRESSED_RGBA ;
		}
		else
		{
			if      (str / "GL_COMPRESSED_ALPHA")			format = GL_ALPHA ;
			else if (str / "GL_COMPRESSED_LUMINANCE")		format = GL_LUMINANCE ;
			else if (str / "GL_COMPRESSED_LUMINANCE_ALPHA")	format = GL_LUMINANCE_ALPHA ;
			else if (str / "GL_COMPRESSED_INTENSITY")		format = GL_INTENSITY ;
			else if (str / "GL_COMPRESSED_RGB")				format = GL_RGB ;
			else if (str / "GL_COMPRESSED_RGBA")			format = GL_RGBA ;
		}

		if      (str == String("GL_ALPHA"))		format = GL_ALPHA ;
		else if (str == String("GL_ALPHA4"))	format = GL_ALPHA4 ;
		else if (str == String("GL_ALPHA8"))	format = GL_ALPHA8 ;
		else if (str == String("GL_ALPHA12"))	format = GL_ALPHA12 ;
		else if (str == String("GL_ALPHA16"))	format = GL_ALPHA16 ;

		else if (str == String("1"))				format = 1 ;
		else if (str == String("GL_LUMINANCE"))		format = GL_LUMINANCE ;
		else if (str == String("GL_LUMINANCE4"))	format = GL_LUMINANCE4 ;
		else if (str == String("GL_LUMINANCE8"))	format = GL_LUMINANCE8 ;
		else if (str == String("GL_LUMINANCE12"))	format = GL_LUMINANCE12 ;
		else if (str == String("GL_LUMINANCE16"))	format = GL_LUMINANCE16 ;

		else if (str == String("2"))						format = 2 ;
		else if (str == String("GL_LUMINANCE_ALPHA"))		format = GL_LUMINANCE_ALPHA ;
		else if (str == String("GL_LUMINANCE4_ALPHA4"))		format = GL_LUMINANCE4_ALPHA4 ;
		else if (str == String("GL_LUMINANCE6_ALPHA2"))		format = GL_LUMINANCE6_ALPHA2 ;
		else if (str == String("GL_LUMINANCE8_ALPHA8"))		format = GL_LUMINANCE8_ALPHA8 ;
		else if (str == String("GL_LUMINANCE12_ALPHA4"))	format = GL_LUMINANCE12_ALPHA4 ;
		else if (str == String("GL_LUMINANCE12_ALPHA12"))	format = GL_LUMINANCE12_ALPHA12 ;
		else if (str == String("GL_LUMINANCE16_ALPHA16"))	format = GL_LUMINANCE16_ALPHA16 ;

		else if (str == String("GL_INTENSITY"))		format = GL_INTENSITY ;
		else if (str == String("GL_INTENSITY4"))	format = GL_INTENSITY4 ;
		else if (str == String("GL_INTENSITY8"))	format = GL_INTENSITY8 ;
		else if (str == String("GL_INTENSITY12"))	format = GL_INTENSITY12 ;
		else if (str == String("GL_INTENSITY16"))	format = GL_INTENSITY16 ;

		else if (str == String("3"))			format = 3 ;
		else if (str == String("GL_RGB"))		format = GL_RGB ;
		else if (str == String("GL_R3_G3_B2"))	format = GL_R3_G3_B2 ;
		else if (str == String("GL_RGB4"))		format = GL_RGB4 ;
		else if (str == String("GL_RGB5"))		format = GL_RGB5 ;
		else if (str == String("GL_RGB8"))		format = GL_RGB8 ;
		else if (str == String("GL_RGB10"))		format = GL_RGB10 ;
		else if (str == String("GL_RGB12"))		format = GL_RGB12 ;
		else if (str == String("GL_RGB16"))		format = GL_RGB16 ;

		else if (str == String("4"))			format = 4 ;
		else if (str == String("GL_RGBA"))		format = GL_RGBA ;
		else if (str == String("GL_RGBA2"))		format = GL_RGBA2 ;
		else if (str == String("GL_RGBA4"))		format = GL_RGBA4 ;
		else if (str == String("GL_RGB5_A1"))	format = GL_RGB5_A1 ;
		else if (str == String("GL_RGBA8"))		format = GL_RGBA8 ;
		else if (str == String("GL_RGB10_A2"))	format = GL_RGB10_A2 ;
		else if (str == String("GL_RGBA12"))	format = GL_RGBA12 ;
		else if (str == String("GL_RGBA16"))	format = GL_RGBA16 ;
	}
	else
	{
		format = 3 ;	// default component = 3 (GL_RGB)

		if      (str == String("1"))				format = 1 ;
		else if (str == String("GL_LUMINANCE"))		format = 1 ;
		else if (str == String("GL_LUMINANCE4"))	format = 1 ;
		else if (str == String("GL_LUMINANCE8"))	format = 1 ;
		else if (str == String("GL_LUMINANCE12"))	format = 1 ;
		else if (str == String("GL_LUMINANCE16"))	format = 1 ;

		else if (str == String("2"))						format = 2 ;
		else if (str == String("GL_LUMINANCE_ALPHA"))		format = 2 ;
		else if (str == String("GL_LUMINANCE4_ALPHA4"))		format = 2 ;
		else if (str == String("GL_LUMINANCE6_ALPHA2"))		format = 2 ;
		else if (str == String("GL_LUMINANCE8_ALPHA8"))		format = 2 ;
		else if (str == String("GL_LUMINANCE12_ALPHA4"))	format = 2 ;
		else if (str == String("GL_LUMINANCE12_ALPHA12"))	format = 2 ;
		else if (str == String("GL_LUMINANCE16_ALPHA16"))	format = 2 ;

		else if (str == String("3"))			format = 3 ;
		else if (str == String("GL_RGB"))		format = 3 ;
		else if (str == String("GL_R3_G3_B2"))	format = 3 ;
		else if (str == String("GL_RGB4"))		format = 3 ;
		else if (str == String("GL_RGB5"))		format = 3 ;
		else if (str == String("GL_RGB8"))		format = 3 ;
		else if (str == String("GL_RGB10"))		format = 3 ;
		else if (str == String("GL_RGB12"))		format = 3 ;
		else if (str == String("GL_RGB16"))		format = 3 ;

		else if (str == String("4"))			format = 4 ;
		else if (str == String("GL_RGBA"))		format = 4 ;
		else if (str == String("GL_RGBA2"))		format = 4 ;
		else if (str == String("GL_RGBA4"))		format = 4 ;
		else if (str == String("GL_RGB5_A1"))	format = 4 ;
		else if (str == String("GL_RGBA8"))		format = 4 ;
		else if (str == String("GL_RGB10_A2"))	format = 4 ;
		else if (str == String("GL_RGBA12"))	format = 4 ;
		else if (str == String("GL_RGBA16"))	format = 4 ;
	}

	return format ;
}


String InternalFormatString(GLint internalFormat)
{
	String str ;

	switch (internalFormat)
	{
	case 1:	str = "1 component (GL_LUMINANCE)" ;		break ;
	case 2:	str = "2 components (GL_LUMINANCE_ALPHA)" ;	break ;
	case 3:	str = "3 components (GL_RGB)" ;				break ;
	case 4:	str = "4 components (GL_RGBA)" ;			break ;

	case GL_ALPHA:		str = "GL_ALPHA" ;		break ;
	case GL_ALPHA4:		str = "GL_ALPHA4" ;		break ;
	case GL_ALPHA8:		str = "GL_ALPHA8" ;		break ;
	case GL_ALPHA12:	str = "GL_ALPHA12" ;	break ;
	case GL_ALPHA16:	str = "GL_ALPHA16" ;	break ;

	case GL_LUMINANCE:		str = "GL_LUMINANCE" ;		break ;
	case GL_LUMINANCE4:		str = "GL_LUMINANCE4" ;		break ;
	case GL_LUMINANCE8:		str = "GL_LUMINANCE8" ;		break ;
	case GL_LUMINANCE12:	str = "GL_LUMINANCE12" ;	break ;
	case GL_LUMINANCE16:	str = "GL_LUMINANCE16" ;	break ;

	case GL_LUMINANCE_ALPHA:		str = "GL_LUMINANCE_ALPHA" ;		break ;
	case GL_LUMINANCE4_ALPHA4:		str = "GL_LUMINANCE4_ALPHA4" ;		break ;
	case GL_LUMINANCE6_ALPHA2:		str = "GL_LUMINANCE6_ALPHA2" ;		break ;
	case GL_LUMINANCE8_ALPHA8:		str = "GL_LUMINANCE8_ALPHA8" ;		break ;
	case GL_LUMINANCE12_ALPHA4:		str = "GL_LUMINANCE12_ALPHA4" ;		break ;
	case GL_LUMINANCE12_ALPHA12:	str = "GL_LUMINANCE12_ALPHA12" ;	break ;
	case GL_LUMINANCE16_ALPHA16:	str = "GL_LUMINANCE16_ALPHA16" ;	break ;

	case GL_INTENSITY:		str = "GL_INTENSITY" ;		break ;
	case GL_INTENSITY4:		str = "GL_INTENSITY4" ;		break ;
	case GL_INTENSITY8:		str = "GL_INTENSITY8" ;		break ;
	case GL_INTENSITY12:	str = "GL_INTENSITY12" ;	break ;
	case GL_INTENSITY16:	str = "GL_INTENSITY16" ;	break ;

	case GL_RGB:		str = "GL_RGB" ;		break ;
	case GL_R3_G3_B2:	str = "GL_R3_G3_B2" ;	break ;
	case GL_RGB4:		str = "GL_RGB4" ;		break ;
	case GL_RGB5:		str = "GL_RGB5" ;		break ;
	case GL_RGB8:		str = "GL_RGB8" ;		break ;
	case GL_RGB10:		str = "GL_RGB10" ;		break ;
	case GL_RGB12:		str = "GL_RGB12" ;		break ;
	case GL_RGB16:		str = "GL_RGB16" ;		break ;

	case GL_RGBA:		str = "GL_RGBA" ;		break ;
	case GL_RGBA2:		str = "GL_RGBA2" ;		break ;
	case GL_RGBA4:		str = "GL_RGBA4" ;		break ;
	case GL_RGB5_A1:	str = "GL_RGB5_A1" ;	break ;
	case GL_RGBA8:		str = "GL_RGBA8" ;		break ;
	case GL_RGB10_A2:	str = "GL_RGB10_A2" ;	break ;
	case GL_RGBA12:		str = "GL_RGBA12" ;		break ;
	case GL_RGBA16:		str = "GL_RGBA16" ;		break ;

	// compressed format
	case GL_COMPRESSED_ALPHA:			str = "GL_COMPRESSED_ALPHA" ;			break ;
	case GL_COMPRESSED_LUMINANCE:		str = "GL_COMPRESSED_LUMINANCE" ;		break ;
	case GL_COMPRESSED_LUMINANCE_ALPHA:	str = "GL_COMPRESSED_LUMINANCE_ALPHA" ;	break ;
	case GL_COMPRESSED_INTENSITY:		str = "GL_COMPRESSED_INTENSITY" ;		break ;
	case GL_COMPRESSED_RGB:				str = "GL_COMPRESSED_RGB" ;				break ;
	case GL_COMPRESSED_RGBA:			str = "GL_COMPRESSED_RGBA" ;			break ;

	// s3tc format
	case GL_COMPRESSED_RGB_S3TC_DXT1:	str = "GL_COMPRESSED_RGB_S3TC_DXT1" ;	break ;
	case GL_COMPRESSED_RGBA_S3TC_DXT1:	str = "GL_COMPRESSED_RGBA_S3TC_DXT1" ;	break ;
	case GL_COMPRESSED_RGBA_S3TC_DXT3:	str = "GL_COMPRESSED_RGBA_S3TC_DXT3" ;	break ;
	case GL_COMPRESSED_RGBA_S3TC_DXT5:	str = "GL_COMPRESSED_RGBA_S3TC_DXT5" ;	break ;

	// FXT1 format
	case GL_COMPRESSED_RGB_FXT1:	str = "GL_COMPRESSED_RGB_FXT1" ;	break ;
	case GL_COMPRESSED_RGBA_FXT1:	str = "GL_COMPRESSED_RGBA_FXT1" ;	break ;

	default:
		str = "Unknown format" ; break ;
	}

	return str ;
}


// Win32
// opengl32.dll をロード
// 現在のところ、Win32 で Mesa DLL を使用する場合用
#ifdef WIN32
OpenGLDLLCtrl OpenGLDLLCtrl::openGLDLLCtrl ;
#endif	// #ifdef WIN32
