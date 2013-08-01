
#ifndef __GL_EXTENSIONS_H__
#define __GL_EXTENSIONS_H__


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// データ変換ウォーニングの無効化
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA
#endif


#include <GL/gl.h>
#include <GL/glut.h>

#include "MString.H"
#include "Math3D.H"


//#define DONOT_USE_GL_VERSION_1_1


//#define DONOT_USE_ARB_multitexture
//#define DONOT_USE_SGIS_multitexture
//#define DONOT_USE_EXT_multitexture

//#define DONOT_USE_GLUT_GAME_MODE
//#define DONOT_USE_GLUT_KEYBOARD_STATUS
//#define DONOT_USE_GLUT_FULL_SCREEN
//#define DONOT_USE_GLUT_SET_CURSOR

#if (GLUT_API_VERSION >= 3)
#define	IS_SUPPORTED_GLUT_FULL_SCREEN
#define	IS_SUPPORTED_GLUT_SET_CURSOR
#endif

#if (GLUT_API_VERSION >= 4 || GLUT_XLIB_IMPLEMENTATION >= 13)
#define IS_SUPPORTED_GLUT_GAME_MODE
#define IS_SUPPORTED_GLUT_KEYBOARD_STATUS
#endif

#if defined DONOT_USE_GLUT_GAME_MODE && defined IS_SUPPORTED_GLUT_GAME_MODE
#undef IS_SUPPORTED_GLUT_GAME_MODE
#endif

#if defined DONOT_USE_GLUT_FULL_SCREEN && defined IS_SUPPORTED_GLUT_FULL_SCREEN
#undef IS_SUPPORTED_GLUT_FULL_SCREEN
#endif

#if defined DONOT_USE_GLUT_SET_CURSOR && defined IS_SUPPORTED_GLUT_SET_CURSOR
#undef IS_SUPPORTED_GLUT_SET_CURSOR
#endif


#if defined WIN32 && defined MESA
#ifndef __WIN32__
#define __WIN32__
#endif	// #ifndef __WIN32__
#endif	// #if defined WIN32 && defined MESA


// OpenGL-1.0 のみ使用
#if defined DONOT_USE_GL_VERSION_1_1 && defined GL_VERSION_1_1
#undef GL_VERSION_1_1

// OpenGL-1.1 以降のヘッダでしか定義されない識別子
#undef GL_POLYGON_OFFSET_FILL
#undef GL_POLYGON_OFFSET_POINT
#undef GL_POLYGON_OFFSET_LINE
#undef GL_POLYGON_OFFSET_FILL
#undef GL_POLYGON_OFFSET_FACTOR
#undef GL_POLYGON_OFFSET_UNITS

#undef GL_ALPHA4
#undef GL_ALPHA8
#undef GL_ALPHA12
#undef GL_ALPHA16
#undef GL_LUMINANCE4
#undef GL_LUMINANCE8
#undef GL_LUMINANCE12
#undef GL_LUMINANCE16
#undef GL_LUMINANCE4_ALPHA4
#undef GL_LUMINANCE6_ALPHA2
#undef GL_LUMINANCE8_ALPHA8
#undef GL_LUMINANCE12_ALPHA4
#undef GL_LUMINANCE12_ALPHA12
#undef GL_LUMINANCE16_ALPHA16
#undef GL_INTENSITY
#undef GL_INTENSITY4
#undef GL_INTENSITY8
#undef GL_INTENSITY12
#undef GL_INTENSITY16
#undef GL_R3_G3_B2
#undef GL_RGB4
#undef GL_RGB5
#undef GL_RGB8
#undef GL_RGB10
#undef GL_RGB12
#undef GL_RGB16
#undef GL_RGBA2
#undef GL_RGBA4
#undef GL_RGB5_A1
#undef GL_RGBA8
#undef GL_RGB10_A2
#undef GL_RGBA12
#undef GL_RGBA16

#undef GL_TEXTURE_RED_SIZE
#undef GL_TEXTURE_GREEN_SIZE
#undef GL_TEXTURE_BLUE_SIZE
#undef GL_TEXTURE_ALPHA_SIZE
#undef GL_TEXTURE_LUMINANCE_SIZE
#undef GL_TEXTURE_INTENSITY_SIZE
#undef GL_PROXY_TEXTURE_1D
#undef GL_PROXY_TEXTURE_2D

#undef GL_TEXTURE_INTERNAL_FORMAT

#endif	// #if defined DONOT_USE_GL_VERSION_1_1 && defined GL_VERSION_1_1


// Texture internal formats (OpenGL-1.0)
#ifndef GL_VERSION_1_1

#define GL_ALPHA4                         0x803B
#define GL_ALPHA8                         0x803C
#define GL_ALPHA12                        0x803D
#define GL_ALPHA16                        0x803E
#define GL_LUMINANCE4                     0x803F
#define GL_LUMINANCE8                     0x8040
#define GL_LUMINANCE12                    0x8041
#define GL_LUMINANCE16                    0x8042
#define GL_LUMINANCE4_ALPHA4              0x8043
#define GL_LUMINANCE6_ALPHA2              0x8044
#define GL_LUMINANCE8_ALPHA8              0x8045
#define GL_LUMINANCE12_ALPHA4             0x8046
#define GL_LUMINANCE12_ALPHA12            0x8047
#define GL_LUMINANCE16_ALPHA16            0x8048
#define GL_INTENSITY                      0x8049
#define GL_INTENSITY4                     0x804A
#define GL_INTENSITY8                     0x804B
#define GL_INTENSITY12                    0x804C
#define GL_INTENSITY16                    0x804D
#define GL_R3_G3_B2                       0x2A10
#define GL_RGB4                           0x804F
#define GL_RGB5                           0x8050
#define GL_RGB8                           0x8051
#define GL_RGB10                          0x8052
#define GL_RGB12                          0x8053
#define GL_RGB16                          0x8054
#define GL_RGBA2                          0x8055
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_RGBA12                         0x805A
#define GL_RGBA16                         0x805B

#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
#define GL_TEXTURE_LUMINANCE_SIZE         0x8060
#define GL_TEXTURE_INTENSITY_SIZE         0x8061
#define GL_PROXY_TEXTURE_1D               0x8063
#define GL_PROXY_TEXTURE_2D               0x8064

#define GL_TEXTURE_INTERNAL_FORMAT        0x1003	// = GL_TEXTURE_COMPONENTS

#endif



#ifdef WIN32
#define DEFAULT_OPENGL32_DLL	"opengl32.dll"
#endif	// #ifdef WIN32


// マルチテクスチャの使用には OpenGL-1.1 以降が必要
#ifndef GL_VERSION_1_1

#ifndef DONOT_USE_ARB_multitexture
#define DONOT_USE_ARB_multitexture
#endif	// DONOT_USE_SGIS_multitexture

#ifndef DONOT_USE_SGIS_multitexture
#define DONOT_USE_SGIS_multitexture
#endif	// DONOT_USE_SGIS_multitexture

#ifndef DONOT_USE_EXT_multitexture
#define DONOT_USE_EXT_multitexture
#endif	// DONOT_USE_SGIS_multitexture

#endif	// #ifndef GL_VERSION_1_1


// OpenGL バージョン（小数点以下１桁まで）
float GetOpenGLVersion() ;


// multitexture をサポートするバージョン（未定）
#ifdef  GL_VERSION_1_
#define GL_VERSION_multitexture
#endif
#define GL_VERSION_multitexture_F		9999.9f

// separate_specular_color をサポートするバージョン（1.2）
#ifdef  GL_VERSION_1_2
#define GL_VERSION_separate_specular_color
#endif
#define GL_VERSION_separate_specular_color_F	1.2f

// texture_filter_anisotropic をサポートするバージョン（未定）
#ifdef  GL_VERSION_1_
#define GL_VERSION_texture_filter_anisotropic
#endif
#define GL_VERSION_texture_filter_anisotropic_F	9999.9f

// texture object をサポートするバージョン（1.1）
#ifdef  GL_VERSION_1_1
#define GL_VERSION_texture_object
#endif
#define GL_VERSION_texture_object_F		1.1f

// polygon offset をサポートするバージョン（1.1）
#ifdef  GL_VERSION_1_1
#define GL_VERSION_polygon_offset
#endif
#define GL_VERSION_polygon_offset_F		1.1f

// texture_cube_map をサポートするバージョン（未定）
#ifdef  GL_VERSION_1_
#define GL_VERSION_texture_cube_map
#endif
#define GL_VERSION_texture_cube_map_F	9999.9f

// texture_compression をサポートするバージョン（未定）
#ifdef  GL_VERSION_1_
#define GL_VERSION_texture_compression
#endif
#define GL_VERSION_texture_compression_F	9999.9f

// texture_compression_s3tc をサポートするバージョン（サポートされる訳ない？）
#ifdef  GL_VERSION_1_
#define GL_VERSION_texture_compression_s3tc
#endif
#define GL_VERSION_texture_compression_s3tc_F	9999.9f

// texture_compression_FXT1 をサポートするバージョン（サポートされる訳ない？）
#ifdef  GL_VERSION_1_
#define GL_VERSION_texture_compression_FXT1
#endif
#define GL_VERSION_texture_compression_FXT1_F	9999.9f


// OpenGL Extension 関連（Windows WGL のみ）
#if defined(WIN32) && !defined(MESA)

// マルチテクスチャ関連

// ARB_multitexture
#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture

// define
#define GL_ACTIVE_TEXTURE_ARB               0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB        0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB            0x84E2
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_TEXTURE2_ARB                     0x84C2
#define GL_TEXTURE3_ARB                     0x84C3

#endif	// #ifndef GL_ARB_multitexture

// ARB_multitexture functions
typedef void (APIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void (APIENTRY * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (APIENTRY * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, const GLint *v);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (APIENTRY * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, const GLshort *v);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);

extern PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB ;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB ;


#ifndef GL_SGIS_multitexture
#define GL_SGIS_multitexture

// define from
// SGIS_multitexture (from 3Dfx OpenGL Beta 2.1 "gl.h")
#ifndef MESA
#define GL_TEXTURE0_SGIS	0x835E
#define GL_TEXTURE1_SGIS	0x835F

// Mesa
#else
#define GL_SELECTED_TEXTURE_SGIS			0x835C
#define GL_SELECTED_TEXTURE_COORD_SET_SGIS	0x835D
#define GL_MAX_TEXTURES_SGIS				0x835E
#define GL_TEXTURE0_SGIS					0x835F
#define GL_TEXTURE1_SGIS					0x8360
#define GL_TEXTURE2_SGIS					0x8361
#define GL_TEXTURE3_SGIS					0x8362
#define GL_TEXTURE_COORD_SET_SOURCE_SGIS	0x8363

#endif	// #ifndef MESA ... #else

#endif	// #ifndef GL_SGIS_multitexture

// SGIS_multitexture functions
typedef void (APIENTRY * PFNGLSELECTTEXTURESGISPROC)			(GLenum target);
typedef void (APIENTRY * PFNGLSELECTTEXTURECOORDSETSGISPROC)	(GLenum target);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FSGISPROC)			(GLenum target, GLfloat s, GLfloat t);

extern PFNGLSELECTTEXTURESGISPROC			glSelectTextureSGIS ;
extern PFNGLSELECTTEXTURECOORDSETSGISPROC	glSelectTextureCoordSetSGIS ;
extern PFNGLMULTITEXCOORD2FSGISPROC			glMultiTexCoord2fSGIS ;


// EXT_multitexture
#ifndef GL_EXT_multitexture
#define GL_EXT_multitexture

// define
#define GL_SELECTED_TEXTURE_EXT                0x83C0
#define GL_SELECTED_TEXTURE_COORD_SET_EXT      0x83C1
#define GL_SELECTED_TEXTURE_TRANSFORM_EXT      0x83C2
#define GL_MAX_TEXTURES_EXT                    0x83C3
#define GL_MAX_TEXTURE_COORD_SETS_EXT          0x83C4
#define GL_TEXTURE_ENV_COORD_SET_EXT           0x83C5
#define GL_TEXTURE0_EXT                        0x83C6
#define GL_TEXTURE1_EXT                        0x83C7
#define GL_TEXTURE2_EXT                        0x83C8
#define GL_TEXTURE3_EXT                        0x83C9

#endif	// #ifndef GL_EXT_multitexture

// EXT_multitexture functions
typedef void (APIENTRY * PFNGLSELECTTEXTUREEXTPROC)			(GLenum target);
typedef void (APIENTRY * PFNGLSELECTTEXTURECOORDSETEXTPROC)	(GLenum target);
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FEXTPROC)		(GLenum target, GLfloat s, GLfloat t);

extern PFNGLSELECTTEXTUREEXTPROC			glSelectTextureEXT ;
extern PFNGLSELECTTEXTURECOORDSETEXTPROC	glSelectTextureCoordSetEXT ;
extern PFNGLMULTITEXCOORD2FEXTPROC			glMultiTexCoord2fEXT ;


// EXT_texture_object
#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object

// define
#define GL_TEXTURE_PRIORITY_EXT		0x8066
#define GL_TEXTURE_RESIDENT_EXT		0x8067

#define GL_TEXTURE_1D_BINDING_EXT	0x8068
#define GL_TEXTURE_2D_BINDING_EXT	0x8069
#define GL_TEXTURE_3D_BINDING_EXT	0x806A

#endif	// #ifndef GL_EXT_texture_object

// EXT_texture_object functions
typedef void (APIENTRY * PFNGLGENTEXTURESEXTPROC)		(GLsizei n, GLuint *textures) ;
typedef void (APIENTRY * PFNGLDELETETEXTURESEXTPROC)	(GLsizei n, GLuint *textures) ;
typedef void (APIENTRY * PFNGLBINDTEXTUREEXTPROC)		(GLenum target, GLuint texture) ;
typedef GLboolean (APIENTRY * PFNGLISTEXTUREEXTPROC)	(GLuint texture) ;

extern PFNGLGENTEXTURESEXTPROC		glGenTexturesEXT ;
extern PFNGLDELETETEXTURESEXTPROC	glDeleteTexturesEXT ;
extern PFNGLBINDTEXTUREEXTPROC		glBindTextureEXT ;
extern PFNGLISTEXTUREEXTPROC		glIsTextureEXT ;


// EXT_polygon_offset
#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset

// define
#define GL_POLYGON_OFFSET_EXT			0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT	0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT		0x8039

#endif	// #ifndef GL_EXT_polygon_offset

// GL_EXT_polygon_offset functions
typedef void (APIENTRY * PFNGLPOLYGONOFFSETEXTPROC)	(GLfloat facter, GLfloat bias) ;

extern PFNGLPOLYGONOFFSETEXTPROC	glPolygonOffsetEXT ;



#endif	// #if defined(_WIN32) && !defined(MESA)


// マルチテクスチャを統合したインタフェース（識別子は勘 ^^;）

#ifdef WIN32
typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC)	(GLenum target) ;
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FPROC)	(GLenum target, GLfloat s, GLfloat t) ;
#else
typedef void (* PFNGLACTIVETEXTUREPROC)		(GLenum target) ;
typedef void (* PFNGLMULTITEXCOORD2FPROC)	(GLenum target, GLfloat s, GLfloat t) ;
#endif	// #ifdef WIN32 ... #else

extern PFNGLACTIVETEXTUREPROC	p_glActiveTexture ;
extern PFNGLMULTITEXCOORD2FPROC	p_glMultiTexCoord2f ;

extern GLenum DEF_GL_MAX_TEXTURE_UNITS ;
extern GLenum DEF_GL_TEXTURE0 ;
extern GLenum DEF_GL_TEXTURE1 ;
extern GLenum DEF_GL_TEXTURE2 ;
extern GLenum DEF_GL_TEXTURE3 ;

// OpenGL が multitexture をサポートしないバージョンの場合
#ifndef GL_VERSION_multitexture

#define glActiveTexture			(*p_glActiveTexture)
#define glMultiTexCoord2f		(*p_glMultiTexCoord2f)
#define GL_MAX_TEXTURE_UNITS 	DEF_GL_MAX_TEXTURE_UNITS
#define GL_TEXTURE0				DEF_GL_TEXTURE0
#define GL_TEXTURE1				DEF_GL_TEXTURE1
#define GL_TEXTURE2				DEF_GL_TEXTURE2
#define GL_TEXTURE3				DEF_GL_TEXTURE3

#endif	// #ifndef GL_VERSION_multitexture


// OpenGL Extension サポートチェック
int IsSupported_OpenGLExtension(const String& name) ;

// multitexture 関連サポートチェック
int IsSupported_ARB_multitexture() ;
int IsSupported_SGIS_multitexture() ;
int IsSupported_EXT_multitexture() ;
char *IsSupported_multitexture(int flag_ARB = TRUE, int flag_EXT = TRUE, int flag_SGIS = TRUE) ;

// サポート種別
#define CURRENT_NAME_GL_VERSION_multitexture	"OpenGL-1.? Multi-Texture"
#define CURRENT_NAME_GL_ARB_multitexture		"GL_ARB_multitexture"
#define CURRENT_NAME_GL_EXT_multitexture		"GL_EXT_multitexture"
#define CURRENT_NAME_GL_SGIS_multitexture		"GL_SGIS_multitexture"


// 関数追加がなく、パラメタ追加のみの拡張機能
// ランタイムサポートのみで使用可能

// フォンシェーディング

// WIN_phong_shading
// 関数の追加はなく、パラメタの追加のみであるため、ランタイムのみの対応で OK
#ifndef GL_WIN_phong_shading
#define GL_WIN_phong_shading
// define
#define GL_PHONG_WIN			((GLenum)0x80EA)
#define GL_PHONG_HINT_WIN		((GLenum)0x80EB)

#endif	// #ifndef GL_WIN_phong_shading


// Separate Specular Color

// OpenGL が separate_specular_color をサポートしないバージョン（1.2 未満）のみ
// 関数の追加はなく、パラメタの追加のみであるため、ランタイムのみの対応で OK
#ifndef GL_VERSION_separate_specular_color

#define GL_LIGHT_MODEL_COLOR_CONTROL		((GLenum)0x81F8)
#define GL_SINGLE_COLOR						((GLenum)0x81F9)
#define GL_SEPARATE_SPECULAR_COLOR			((GLenum)0x81FA)

#endif	// #ifndef GL_VERSION_separate_specular_color


// Separate Specular Color Extensions
// OpenGL バージョンに関わらずセット

// EXT_separate_specular_color
#ifndef GL_EXT_separate_specular_color
#define GL_EXT_separate_specular_color

// define
#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT	GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_SINGLE_COLOR_EXT					GL_SINGLE_COLOR
#define GL_SEPARATE_SPECULAR_COLOR_EXT		GL_SEPARATE_SPECULAR_COLOR

#endif	// #ifndef GL_EXT_separate_specular_color

// ARB_separate_specular_color
#ifndef GL_ARB_separate_specular_color
#define GL_ARB_separate_specular_color

// define
#define GL_LIGHT_MODEL_COLOR_CONTROL_ARB	GL_LIGHT_MODEL_COLOR_CONTROL
#define GL_SINGLE_COLOR_ARB					GL_SINGLE_COLOR
#define GL_SEPARATE_SPECULAR_COLOR_ARB		GL_SEPARATE_SPECULAR_COLOR

#endif	// #ifndef GL_ARB_separate_specular_color


// 暫定設定ここから！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

// Texture Filter Anisotropic

// OpenGL が texture_filter_anisotropic をサポートしないバージョン（未定）のみ
// 関数の追加はなく、パラメタの追加のみであるため、ランタイムのみの対応で OK
#ifndef GL_VERSION_texture_filter_anisotropic

// バージョンで正式サポートされた時点で、正式に決定（現在は未定なので暫定）
// もし正式の値がこの値と異なる場合は、変数で動的にセットするよう変更が必要なので注意
#define GL_TEXTURE_MAX_ANISOTROPY			((GLenum)0x84fe)
#define GL_MAX_TEXTURE_MAX_ANISOTROPY		((GLenum)0x84ff)

#endif	// #ifndef GL_VERSION_texture_filter_anisotropic


// Texture Filter Anisotropic Extensions
// OpenGL バージョンに関わらずセット

// EXT_texture_filter_anisotropic
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic

// define
#define GL_TEXTURE_MAX_ANISOTROPY_EXT		((GLenum)0x84fe)
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT	((GLenum)0x84ff)

#endif	// #ifndef GL_EXT_texture_filter_anisotropic


// Texture Reflection のパラメタ
// 現時点では固定で OK
// OpenGL が texture_cube_map をサポートしないバージョン（未定）のみ

// バージョンで正式サポートされた時点で、正式に決定（現在は未定なので暫定）
// もし正式の値がこの値と異なる場合は、変数で動的にセットするよう変更が必要なので注意
#ifndef GL_VERSION_texture_cube_map
#define GL_NORMAL_MAP				((GLenum)0x8511)
#define GL_REFLECTION_MAP			((GLenum)0x8512)
#endif	// #ifndef GL_VERSION_texture_cube_map

// texture_cube_map extensions
// OpenGL バージョンに関わらずセット

// ARB_texture_cube_map
#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map

#define GL_NORMAL_MAP_ARB					GL_NORMAL_MAP
#define GL_REFLECTION_MAP_ARB				GL_REFLECTION_MAP

#define GL_TEXTURE_CUBE_MAP_ARB				((GLenum)0x8513)
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB		((GLenum)0x8514)

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB	((GLenum)0x8515)
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB	((GLenum)0x8516)
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB	((GLenum)0x8517)
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB	((GLenum)0x8518)
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB	((GLenum)0x8519)
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB	((GLenum)0x851a)

#endif	// #ifndef GL_ARB_texture_cube_map

// EXT_texture_cube_map
#ifndef GL_EXT_texture_cube_map
#define GL_EXT_texture_cube_map

#define GL_NORMAL_MAP_EXT					GL_NORMAL_MAP
#define GL_REFLECTION_MAP_EXT				GL_REFLECTION_MAP

#define GL_TEXTURE_CUBE_MAP_EXT				((GLenum)0x8513)
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT		((GLenum)0x8514)

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT	((GLenum)0x8515)
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT	((GLenum)0x8516)
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT	((GLenum)0x8517)
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT	((GLenum)0x8518)
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT	((GLenum)0x8519)
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT	((GLenum)0x851a)

#endif	// #ifndef GL_EXT_texture_cube_map

// NV_texgen_reflection
// OpenGL バージョンに関わらずセット
#ifndef GL_NV_texgen_reflection
#define GL_NV_texgen_reflection

#define GL_NORMAL_MAP_NV					GL_NORMAL_MAP
#define GL_REFLECTION_MAP_NV				GL_REFLECTION_MAP

#endif	// #ifndef GL_NV_texgen_reflection

// GL_ARB_texture_cube_map サポートチェック
int IsSupported_ARB_texture_cube_map() ;

// GL_EXT_texture_cube_map サポートチェック
int IsSupported_EXT_texture_cube_map() ;

// GL_NV_texgen_reflection サポートチェック
int IsSupported_NV_texgen_reflection() ;

// texgen_reflection 関連サポートチェック
// GL_ARB_texture_cube_map もしくは
// GL_EXT_texture_cube_map もしくは
// GL_NV_texgen_reflection をサポートしているか
char *IsSupported_texgen_reflection(int flag_ARB_texture_cube_map = TRUE,
									int flag_EXT_texture_cube_map = TRUE,
									int flag_NV_texgen_reflection = TRUE) ;

// サポート種別
#define CURRENT_NAME_GL_ARB_texture_cube_map	"GL_ARB_texture_cube_map"
#define CURRENT_NAME_GL_EXT_texture_cube_map	"GL_EXT_texture_cube_map"
#define CURRENT_NAME_GL_NV_texgen_reflection	"GL_NV_texgen_reflection"


// texture_compression
// バージョンで正式サポートされた時点で、正式に決定（現在は未定なので暫定）
// もし正式の値がこの値と異なる場合は、変数で動的にセットするよう変更が必要なので注意
#ifndef GL_VERSION_texture_compression
#define GL_COMPRESSED_ALPHA				((GLenum)0x84E9)
#define GL_COMPRESSED_LUMINANCE			((GLenum)0x84EA)
#define GL_COMPRESSED_LUMINANCE_ALPHA	((GLenum)0x84EB)
#define GL_COMPRESSED_INTENSITY			((GLenum)0x84EC)
#define GL_COMPRESSED_RGB				((GLenum)0x84ED)
#define GL_COMPRESSED_RGBA				((GLenum)0x84EE)
#endif	// #ifndef GL_VERSION_texture_compression

// とりあえず OpenGL バージョンに関わらずセット
#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression

#define GL_COMPRESSED_ALPHA_ARB				GL_COMPRESSED_ALPHA
#define GL_COMPRESSED_LUMINANCE_ARB			GL_COMPRESSED_LUMINANCE
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB	GL_COMPRESSED_LUMINANCE_ALPHA
#define GL_COMPRESSED_INTENSITY_ARB			GL_COMPRESSED_INTENSITY
#define GL_COMPRESSED_RGB_ARB				GL_COMPRESSED_RGB
#define GL_COMPRESSED_RGBA_ARB				GL_COMPRESSED_RGBA

#endif	// #ifndef GL_ARB_texture_compression

// GL_ARB_texture_compression サポートチェック
int IsSupported_ARB_texture_compression_Enum() ;

// texgen_reflection 関連サポートチェック（ただしトークンのみで OK）
// 現在のところ GL_ARB_texture_compression （のランタイム）をサポートしているか
char *IsSupported_texture_compression_Enum(int flag_ARB_texture_compression) ;

// サポート種別
#define CURRENT_NAME_GL_VERSION_texture_compression	"OpenGL-1.? Texture Compression"
#define CURRENT_NAME_GL_ARB_texture_compression		"GL_ARB_texture_compression"

// texture_compression_s3tc
#ifndef GL_VERSION_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1			((GLenum)0x83F0)
#define GL_COMPRESSED_RGBA_S3TC_DXT1		((GLenum)0x83F1)
#define GL_COMPRESSED_RGBA_S3TC_DXT3		((GLenum)0x83F2)
#define GL_COMPRESSED_RGBA_S3TC_DXT5		((GLenum)0x83F3)
#endif	// #ifndef GL_VERSION_texture_compression_s3tc

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT		GL_COMPRESSED_RGB_S3TC_DXT1
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT	GL_COMPRESSED_RGBA_S3TC_DXT1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT	GL_COMPRESSED_RGBA_S3TC_DXT3
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT	GL_COMPRESSED_RGBA_S3TC_DXT5

#endif	// #ifndef GL_EXT_texture_compression_s3tc

// texture_compression_FXT1
#ifndef GL_VERSION_texture_compression_FXT1
#define GL_COMPRESSED_RGB_FXT1			((GLenum)0x86B0)
#define GL_COMPRESSED_RGBA_FXT1			((GLenum)0x86B1)
#endif	// #ifndef GL_VERSION_texture_compression_FXT1

#ifndef GL_3DFX_texture_compression_FXT1
#define GL_3DFX_texture_compression_FXT1

#define GL_COMPRESSED_RGB_FXT1_3DFX			GL_COMPRESSED_RGB_FXT1
#define GL_COMPRESSED_RGBA_FXT1_3DFX		GL_COMPRESSED_RGBA_FXT1

#endif	// #ifndef GL_EXT_texture_compression_s3tc


// GL_EXT_texture_compression_s3tc サポートチェック
int IsSupported_EXT_texture_compression_s3tc() ;

// S3TC 関連サポートチェック
char *IsSupported_texture_compression_s3tc(int flag_EXT_texture_compression_s3tc) ;

// サポート種別
#define CURRENT_NAME_GL_VERSION_texture_compression_s3tc	"OpenGL-1.? Texture Compression S3TC"	// サポートされる訳ない？
#define CURRENT_NAME_GL_EXT_texture_compression_s3tc		"GL_EXT_texture_compression_s3tc"


// GL_3DFX_texture_compression_FXT1 サポートチェック
int IsSupported_3DFX_texture_compression_FXT1() ;

// FXT1 関連サポートチェック
char *IsSupported_texture_compression_FXT1(int flag_3DFX_texture_compression_FXT1) ;

// サポート種別
#define CURRENT_NAME_GL_VERSION_texture_compression_FXT1	"OpenGL-1.? Texture Compression S3TC"	// サポートされる訳ない？
#define CURRENT_NAME_GL_3DFX_texture_compression_FXT1		"GL_3DFX_texture_compression_FXT1"



// 暫定設定ここまで！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！


// テクスチャ・オブジェクト統合インタフェース

#ifdef WIN32
typedef void (APIENTRY * PFNGLGENTEXTURESPROC)		(GLsizei n, GLuint *textures) ;
#ifdef MESA
typedef void (APIENTRY * PFNGLDELETETEXTURESPROC)	(GLsizei n, const GLuint *textures) ;
#else
typedef void (APIENTRY * PFNGLDELETETEXTURESPROC)	(GLsizei n, GLuint *textures) ;
#endif	// #ifdef MESA
typedef void (APIENTRY * PFNGLBINDTEXTUREPROC)		(GLenum target, GLuint texture) ;
typedef GLboolean (APIENTRY * PFNGLISTEXTUREPROC)	(GLuint texture) ;
#else
typedef void (* PFNGLGENTEXTURESPROC)				(GLsizei n, GLuint *textures) ;
typedef void (* PFNGLDELETETEXTURESPROC)			(GLsizei n, const GLuint *textures) ;
typedef void (* PFNGLBINDTEXTUREPROC)				(GLenum target, GLuint texture) ;
typedef GLboolean (* PFNGLISTEXTUREPROC)			(GLuint texture) ;
#endif	// #ifdef WIN32 ... #else

extern PFNGLGENTEXTURESPROC		p_glGenTextures ;
extern PFNGLDELETETEXTURESPROC	p_glDeleteTextures ;
extern PFNGLBINDTEXTUREPROC		p_glBindTexture ;
extern PFNGLISTEXTUREPROC		p_glIsTexture ;

// OpenGL が texture object をサポートしないバージョンの場合
#ifndef GL_VERSION_texture_object

#define glGenTextures		(*p_glGenTextures)
#define glDeleteTextures	(*p_glDeleteTextures)
#define glBindTexture		(*p_glBindTexture)
#define glIsTexture			(*p_glIsTexture)

#endif	// #ifndef GL_VERSION_texture_object

// texture object 関連サポートチェック
int IsSupported_EXT_texture_object() ;
char *IsSupported_texture_object(int flag_EXT = TRUE) ;

// サポート種別
#define CURRENT_NAME_GL_VERSION_texture_object	"OpenGL-1.1 Texture Object"
#define CURRENT_NAME_GL_EXT_texture_object		"GL_EXT_texture_object"


// ポリゴン・オフセット統合インタフェース

#ifdef WIN32
typedef void (APIENTRY * PFNGLPOLYGONOFFSETPROC)	(GLfloat facter, GLfloat units) ;
#else
typedef void (* PFNGLPOLYGONOFFSETPROC)				(GLfloat facter, GLfloat units) ;
#endif	// #ifdef WIN32 ... #else

extern PFNGLPOLYGONOFFSETPROC	p_glPolygonOffset ;
extern GLenum DEF_GL_POLYGON_OFFSET_POINT ;
extern GLenum DEF_GL_POLYGON_OFFSET_LINE ;
extern GLenum DEF_GL_POLYGON_OFFSET_FILL ;
extern GLenum DEF_GL_POLYGON_OFFSET_FACTOR ;
extern GLenum DEF_GL_POLYGON_OFFSET_UNITS ;

// OpenGL が polygon offset をサポートしないバージョンの場合
#ifndef GL_VERSION_polygon_offset

#define glPolygonOffset				(*p_glPolygonOffset)
#define GL_POLYGON_OFFSET_POINT		DEF_GL_POLYGON_OFFSET_POINT
#define GL_POLYGON_OFFSET_LINE		DEF_GL_POLYGON_OFFSET_LINE
#define GL_POLYGON_OFFSET_FILL		DEF_GL_POLYGON_OFFSET_FILL
#define GL_POLYGON_OFFSET_FACTOR	DEF_GL_POLYGON_OFFSET_FACTOR
#define GL_POLYGON_OFFSET_UNITS		DEF_GL_POLYGON_OFFSET_UNITS

#endif	// #ifndef GL_VERSION_polygon_offset

// polygon offset 関連サポートチェック
int IsSupported_EXT_polygon_offset() ;
char *IsSupported_polygon_offset(int flag_EXT = TRUE) ;

// サポート種別
#define CURRENT_NAME_GL_VERSION_polygon_offset	"OpenGL-1.1 Polygon Offset"
#define CURRENT_NAME_GL_EXT_polygon_offset		"GL_EXT_polygon_offset"


// multitexture coord 指定
//
// target を指定する場合は、事前に
// IsSupported_multitexture() による使用機能の登録が必要
// 
// IsSupported_SGIS_multitexture() は使用しない方が良い
// また、SGIS では基本的に
// GL_MAX_TEXTURE_UNITS は使用不可（かならず２ユニットとみなした方が良い）
//
// target の指定がない場合 glTexCoord2f() で代用
void TexCoord2f(GLfloat s, GLfloat t, int target = 0) ;


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
void Vertex3fWithTexAxis(GLfloat vx, GLfloat vy, GLfloat vz, GLfloat *s0 = NULL, GLfloat *t0 = NULL) ;

void Vertex3fWithTexAxis(GLfloat vx, GLfloat vy, GLfloat vz,
						 int target0,      GLfloat *s0 = NULL, GLfloat *t0 = NULL,
						 int target1 = -1, GLfloat *s1 = NULL, GLfloat *t1 = NULL,
						 int target2 = -1, GLfloat *s2 = NULL, GLfloat *t2 = NULL,
						 int target3 = -1, GLfloat *s3 = NULL, GLfloat *t3 = NULL) ;


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
void Vertex3fvWithTexAxis(GLfloat v[3], GLfloat *s0 = NULL, GLfloat *t0 = NULL) ;

void Vertex3fvWithTexAxis(GLfloat v[3],
						  int target0,      GLfloat *s0 = NULL, GLfloat *t0 = NULL,
						  int target1 = -1, GLfloat *s1 = NULL, GLfloat *t1 = NULL,
						  int target2 = -1, GLfloat *s2 = NULL, GLfloat *t2 = NULL,
						  int target3 = -1, GLfloat *s3 = NULL, GLfloat *t3 = NULL) ;


// multitexture 使用時の最大テクスチャユニット数
// カレントが SGIS_multitexture の場合は２
// IsSupported_*_multitexture() が未コールの場合ゼロ
GLint GetMaxTextureUnits() ;

// anisotropic filtering 使用時の最大 max anisotropy
GLfloat GetMaxTextureMaxAnisotropy() ;

// WIN_phong_shading サポートチェック
int IsSupported_WIN_phong_shading() ;

// separate_specular_color 関連サポートチェック
int IsSupported_ARB_separate_specular_color() ;
int IsSupported_EXT_separate_specular_color() ;
char *IsSupported_separate_specular_color(int flag_ARB = TRUE, int flag_EXT = TRUE) ;
// サポート種別
#define CURRENT_NAME_GL_VERSION_separate_specular_color	"OpenGL-1.2 Separate Specular Color"
#define CURRENT_NAME_GL_ARB_separate_specular_color		"GL_ARB_separate_specular_color"
#define CURRENT_NAME_GL_EXT_separate_specular_color		"GL_EXT_separate_specular_color"

// texture_filter_anisotropic 関連サポートチェック
int IsSupported_EXT_texture_filter_anisotropic() ;
char *IsSupported_texture_filter_anisotropic(int flag_EXT = TRUE) ;
// サポート種別
#define CURRENT_NAME_GL_VERSION_texture_filter_anisotropic	"OpenGL-1.? Texture Filter Anisotropic"
#define CURRENT_NAME_GL_EXT_texture_filter_anisotropic		"GL_EXT_texture_filter_anisotropic"


// 現在有効になっている統合インタフェース
String CurrentInterface_multitexture() ;
String CurrentInterface_separate_specular_color() ;
String CurrentInterface_texture_filter_anisotropic() ;
String CurrentInterface_texture_object() ;
String CurrentInterface_polygon_offset() ;
String CurrentInterface_texture_cube_map() ;
String CurrentInterface_texgen_reflection() ;
String CurrentInterfaceEnum_texture_compression() ;
String CurrentInterface_texture_compression_s3tc() ;
String CurrentInterface_texture_compression_FXT1() ;


#if defined(DONOT_USE_ARB_multitexture) && defined(GL_ARB_multitexture)
#undef GL_ARB_multitexture
#endif

#if defined(DONOT_USE_SGIS_multitexture) && defined(GL_SGIS_multitexture)
#undef GL_SGIS_multitexture
#endif

#if defined(DONOT_USE_EXT_multitexture) && defined(GL_EXT_multitexture)
#undef GL_EXT_multitexture
#endif


#ifdef WIN32

// Win32
// opengl32.dll をロード
// 現在のところ、Win32 で Mesa DLL を使用する場合用
class OpenGLDLLCtrl
{
public:
	HINSTANCE hLibOpenGL32 ;

	static OpenGLDLLCtrl openGLDLLCtrl ;

	void LoadLibrary(const String& openGL32_DLL = DEFAULT_OPENGL32_DLL)
	{
		if (!hLibOpenGL32 && openGL32_DLL != "")
			hLibOpenGL32 = ::LoadLibrary(openGL32_DLL) ;
	}

	void FreeLibrary()
	{
		if (hLibOpenGL32)
			::FreeLibrary(hLibOpenGL32) ;

		hLibOpenGL32 = NULL ;
	}

	// コンストラクタ
	OpenGLDLLCtrl(const String& openGL32_DLL = "")
	{
		hLibOpenGL32 = NULL ;
		LoadLibrary(openGL32_DLL) ;
	}

	void *GetProcAddress(const String& procedureName)
	{
		if (!hLibOpenGL32)
			LoadLibrary(DEFAULT_OPENGL32_DLL) ;

		if (hLibOpenGL32)
			return ::GetProcAddress(hLibOpenGL32, procedureName) ;
		else
			return NULL ;
	}

	~OpenGLDLLCtrl() { FreeLibrary() ; }

} ;

#endif	// #ifdef WIN32


// Texture Internal Formats
GLint InternalFormat(String str) ;
String InternalFormatString(GLint internalFormat) ;

#endif	// #ifndef __GL_EXTENSIONS_H__
