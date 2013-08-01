
#ifndef _GLCLOCK_H_
#define _GLCLOCK_H_


#define TITLE_VER		"glclock-6.0 Beta 6.0"
#define AUTHOR			"June 2000 by Masa (Masaki Kawase)\n<E-Mail: masa@daionet.gr.jp>\n<http://www.daionet.gr.jp/~masa/>"
#define SIGN			"Presented by Masa."


// for Win32
#ifdef   WIN32
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h >
// glclock.ini
#define GLCLOCK_INI		"glclock.ini"
//#define _CONSOLE

// for UNIX etc.
#else	// #ifdef   WIN32
#include <unistd.h>
#endif

// for Mac
#ifdef macintosh
#define NO_SLEEP
#include "macsupport.h"
#endif	// #ifdef macintosh


#define NO_SELECT
//#define NO_SLEEP

//#define INCLUDE_SYS_TIME_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <time.h>

#ifdef INCLUDE_SYS_TIME_H
#include <sys/time.h>
#endif	// #ifdef INCLUDE_SYS_TIME_H

#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>

#include "GLObject.H"
#include "Option.H"


// Debug
//#define GL_COMPILE	GL_COMPILE_AND_EXECUTE

// -e 2 のディティールテクスチャブレンドを
// 環境マッピングの後でブレンド
//#define DETAIL_TEXTURE_BLEND_AFTER_ENV

// ディティールテクスチャ使用時の輝度補正を
// Diffuse 係数によって調整（ただし -r 1 はテクスチャによる調整）
#define DETAIL_TEXTURE_BRIGHT_BY_DIFFUSE

//#define CULL_REF_SKY_BY_ANGLE

#define ________SOLID_SHADED_CLOCK

//#if defined WIN32 && !defined _CONSOLE

// Windows で dll 作成
//#define GLCLOCK_DLL_EXPORT

// Windows で ウィンドウマネジメントに glut を使わず Win32API と wgl を使用
//#define DONOT_USE_GLUT_TO_MAGAGE_WINDOW

//#endif	// #if defined WIN32 && !defined _CONSOLE

#define OPENGL32_DLL	"opengl32.dll"

// FPS 出力フォーマット
#define FORMAT_FPS		"%7.2f FPS"
#define FORMAT_KTPS		" %10.3f kTPS"
#define FORMAT_KTPF		"   at %8.3f kTPF"

#define FORMAT_LOG_FPS		"%12.5f FPS"
#define FORMAT_LOG_KTPS		" %15.6f kTPS"
#define FORMAT_LOG_KTPF		"   at %8.3f kTPF"

// デフォルトログ出力ファイル名
#ifdef WIN32
#define OUTPUT_LOG_FILE		"glclock.log"
#else
#define OUTPUT_LOG_FILE		""
#endif

// スクリーンショット出力ファイル名
#define FILE_NAME_SCREEN_SHOT	"glclock_shot"
#define SIGNATURE_SCREEN_SHOT	"glclock screen shot"

// -R 指定時、空の映り込みの場所（奥行き操作）
#ifdef  CULL_REF_SKY_BY_ANGLE
#define CULL_REF_SKY_AXIS		0.24f
#define REF_SKY_Z_TRANSLATE		-4.2f

#else	// #ifdef CULL_REF_SKY_BY_ANGLE
#define REF_SKY_Z_TRANSLATE	-3.8f

#endif	// #ifdef CULL_REF_SKY_BY_ANGLE ... #else


#define CULL_OBJECTS_BY_ANGLE

#define CULL_SHELL_Z_AXIS	0.44f
#define CULL_BOARD_Z_AXIS	-0.03f


// glclock.dll 作成スイッチ
#if defined GLCLOCK_DLL_EXPORT && defined WIN32

#define DLL_EXPORT	__declspec(dllexport)
#define CCONV		_stdcall

#else	// #if defined GLCLOCK_DLL_EXPORT && defined WIN32

#define DLL_EXPORT
#define CCONV

#endif	// #if defined GLCLOCK_DLL_EXPORT && defined WIN32 ... #else

//#define GLCLOCK_RETURN_LONGJMP
#define	GLCLOCK_RET_LONGJMP_SUCCESS		1
#define	GLCLOCK_RET_LONGJMP_FAILURE		-1


#ifdef WIN32

// glclock.dll
typedef int (APIENTRY * PFNGLCLOCKOPTPROC)(char *) ;
typedef int (APIENTRY * PFNGLCLOCKARGPROC)(int, char **) ;
#define GLCLOCK_DLL		"glclock.dll"

#define  NM_RESOLUTION	12

#endif	// #ifdef WIN32


// デフォルトで Voodoo モード（フルスクリーン、-w 640 -h 480 等）
//#define VOODOO_MODE


// 環境マップ加工時に、中心からの距離によって変化（距離のｎ乗）
#define RADIUS_RATIO_SPECULAR	4.0f
#define RADIUS_RATIO_CHROME		8.0f
#define RADIUS_RATIO_CRYSTAL	8.0f

// カリングステータス
#define CULL_STATUS_NONE	0x0000
#define CULL_STATUS_SHELL	0x0002
#define CULL_STATUS_FRONT	0x0004

// -env 2

// シェーディング＋テクスチャブレンドによるハイライト（１パス）用
// 環境テクスチャのブライト
#define CHROME_ENV2_ENV_BRIGHT		0.5f

// 金属マテリアルの金属係数
#define ENV2_MATERIAL_METAL_RATIO	0.82f
// 金属マテリアルのスペキュラー
#define ENV2_MATERIAL_SPECULAR		0.9f
// 金属マテリアルのハイライト指数
#define ENV2_MATERIAL_SHININESS		64.0f


// 金属テクスチャ全般
// ハイライト加算時の環境マップの強さ
#define CHROME_BRIGHT			0.82f
//#define CHROME_BRIGHT			0.85f

// 金属テクスチャ全般
// ライトマップの強さ
#define CHROME_ADDING_LIGHT_RATIO	1.0f

// スペキュラーテクスチャ全般
// ライトマップの強さ
#define SPECULAR_ADDING_LIGHT_RATIO	1.0f

// 色付き専用テクスチャの明るさ（金色等）
#define COLOR_GOLD_TEXTURE		1.5f
#define COLOR_GOLD_TEXTURE_R	1.0f
#define COLOR_GOLD_TEXTURE_G	0.7f
#define COLOR_GOLD_TEXTURE_B	0.15f

// シェルの少し黄色がかった金属専用テクスチャの色
#define COLOR_CHROME2_TEXTURE	1.16f
#define COLOR_CHROME2_TEXTURE_R	1.0f
#define COLOR_CHROME2_TEXTURE_G	1.0f
#define COLOR_CHROME2_TEXTURE_B	0.75f

// ライトグリーン金属専用テクスチャの色
#define COLOR_GREEN_TEXTURE		1.2f
#define COLOR_GREEN_TEXTURE_R	0.5f
#define COLOR_GREEN_TEXTURE_G	1.0f
#define COLOR_GREEN_TEXTURE_B	0.6f

// ライトブルー金属専用テクスチャの色
#define COLOR_BLUE_TEXTURE		1.3f
#define COLOR_BLUE_TEXTURE_R	0.8f
#define COLOR_BLUE_TEXTURE_G	0.8f
#define COLOR_BLUE_TEXTURE_B	1.0f

// レッド金属専用テクスチャの色
#define COLOR_RED_TEXTURE		1.6f
#define COLOR_RED_TEXTURE_R		1.0f
#define COLOR_RED_TEXTURE_G		0.2f
#define COLOR_RED_TEXTURE_B		0.3f

// ピンク金属専用テクスチャの色
#define COLOR_PINK_TEXTURE		1.0f
#define COLOR_PINK_TEXTURE_R	1.0f
#define COLOR_PINK_TEXTURE_G	0.6f
#define COLOR_PINK_TEXTURE_B	0.6f


// クロームテクスチャ用 針の色
#define COLOR_CHROME_HOUR_R		0.8f
#define COLOR_CHROME_HOUR_G		0.8f
#define COLOR_CHROME_HOUR_B		1.0f

#define COLOR_CHROME_MINUTE_R	0.5f
#define COLOR_CHROME_MINUTE_G	1.0f
#define COLOR_CHROME_MINUTE_B	0.6f

#define COLOR_CHROME_SECOND_R	1.0f
#define COLOR_CHROME_SECOND_G	0.75f
#define COLOR_CHROME_SECOND_B	0.05f

// その他の時計針の色
#define COLOR_HOUR_R	0.8f
#define COLOR_HOUR_G	0.8f
#define COLOR_HOUR_B	1.0f

// -solid 専用
#define COLOR_HOUR_SOLID_R	0.6f
#define COLOR_HOUR_SOLID_G	0.5f
#define COLOR_HOUR_SOLID_B	1.0f

#define COLOR_MINUTE_R	0.5f
#define COLOR_MINUTE_G	1.0f
#define COLOR_MINUTE_B	0.6f

#define COLOR_SECOND_R	1.0f
#define COLOR_SECOND_G	0.75f
#define COLOR_SECOND_B	0.05f


// RGBA フォーマット使用時のアルファ値
#define DEFAULT_ALPHA		1.0f

#ifndef VOODOO_MODE
#define SPEED		160.0f
#else
#define SPEED		160.0f
#endif	// #ifndef VOODOO_MODE

//#define GIFF_DEMO
#define GIFF_DEMO_HOUR		3
#define GIFF_DEMO_MINUTE	55
#define GIFF_DEMO_SECOND	32
#define GIFF_SPIN_X			-10.0f

// glclock イメージパス指定環境変数
#define ENV_IMAGE_PATH		"GLCLOCK_IMAGE_PATH"

// glut Game Mode String
#define DEFAULT_GAME_MODE_STRING	""

// デフォルトイメージパス
#define IMAGE_PATH			""

// スピン関係初期値
#define SPIN_X		-18.0f
#define SPIN_Y		0.0f

#define	SPIN_X_BENCH	-45.0f

#ifdef GIFF_DEMO
#define ROTATE_Y	0.0f
#define ROTATE_X	0.0f
#else
#define ROTATE_Y		-30.0f
#define ROTATE_X		-14.0f
#endif
#define SAVER_ROTATE_Y	17.0f
#define BENCH_ROTATE_Y	0.0f

#define N_ANTI					8
#define N_MOTION				6
#define N_FIELD					7
#define MAX_MOTION_N_SAMPLING	64

#define METAL_RATIO	2.0f

#define IS_SET_WIDTH	FALSE
#define IS_SET_HEIGHT	FALSE

// glclockmark ステータス
#define	GLCLOCK_MARK	0.0f

// デフォルトのパース
#define ZOOM		0.5f

// スクリーンセーバ時の回転速度
#define SPIN_SPEED	1.0f
#define SPIN_RATIO	212.0f
#define SPIN_VELOC	3.5f

// スクリーンセーバ時の時計サイズ
#define CLOCK_SIZE		0.6f

// 画像に対して初期処理
#define R_SPECULAR_MAP_GAMMA			0.3f	// -r の スペキュラー専用
#define R_GLASS_SPECULAR_MAP_GAMMA		0.5f	// -r の ハイライトイメージ
#define SPECULAR_MAP_GAMMA				0.5f

#define CRYSTAL_SPECULAR_MAP_GAMMA		0.3f
#define CRYSTAL_LIGHT_MAP_GAMMA			0.8f	// クリスタルはライトマップも変化？
#define CRYSTAL_SPECULAR_ADDING_ENV_RATIO	1.0f
#define CRYSTAL_SPECULAR_ADD				0.7f	// クリスタル本体の描画の明るさ
#define CRYSTAL_NEEDLE_SPECULAR_ADD			1.0f	// クリスタル針の描画の明るさ（他に合わせると針が暗くなり過ぎるため）

#define IS_SET_DETAIL_MAP_GAMMA		0.0f
#define DETAIL_MAP_GAMMA_REAL		1.0f
#define DETAIL_MAP_GAMMA_ENV1		3.0f	// detail: 4
#define DETAIL_MAP_GAMMA_ENV2		0.4f	// detail: 5


// マルチテクスチャ使用時、予め輝度を減衰
#define REALITY_SPECULAR_NA		1.0f	// == REALITY_SPECULAR_ONE_MINUS_COLOR

// 金属一般テクスチャの最大彩度減衰値
#define CHROME_IMAGE_VIVIDNESS_RATIO	0.2f

// 以下はレンダリング時カラーとしてセット
#define MASTER_ENV_GLASS_BRIGHT_ADD		1.2f

//#define ENV_GLASS_BRIGHT		(0.50f * MASTER_ENV_GLASS_BRIGHT_ADD)
//#define ENV_GLASS_BRIGHT_R		(0.37f * MASTER_ENV_GLASS_BRIGHT_ADD)
//#define ENV_GLASS_BRIGHT_REF	(0.53f * MASTER_ENV_GLASS_BRIGHT_ADD)
#define ENV_GLASS_BRIGHT		(0.60f * MASTER_ENV_GLASS_BRIGHT_ADD)
#define ENV_GLASS_BRIGHT_R		(0.45f * MASTER_ENV_GLASS_BRIGHT_ADD)
#define ENV_GLASS_BRIGHT_REF	(0.64f * MASTER_ENV_GLASS_BRIGHT_ADD)

#define ENV_GLASS_BRIGHT_ONE_MINUS_COLOR		0.7f
#define ENV_GLASS_BRIGHT_REF_ONE_MINUS_COLOR	0.8f

// クリスタルではADDブレンドだけ少し暗めに
#define CRYSTAL_GLASS_ADD_BRIGHT_RATIO	0.7f

// -r 指定時のデフォルトのスペキュラー（テクスチャに対する割合）
#define REALITY_SPECULAR					0.5f
#define REALITY_SPECULAR_ONE_MINUS_COLOR	REALITY_SPECULAR_NA


// ADD の時の光源色の変化に対する割合
// 時間による光源色の変化に対し、
// スペキュラーの変化を抑える
#define SPECULAR_TIME_GRAD_RATIO_REAL		0.3f
#define SPECULAR_TIME_GRAD_RATIO_ENV2		0.6f
#define SPECULAR_TIME_GRAD_RATIO_TEX_ENV	0.9f


// -R オプションの反射体の明るさ（オリジナルに対する割合）
//#define REFLECT_BLIGHT	0.6f


// FPS 計算に入るまでの時間（ミリ秒）
#define FPS_CALC_WAIT			1000
#define FPS_CALC_WAIT_BENCH		3000

struct FPSInfo
{
	int frameCount ;
	int mSecond ;
	int polygonCount ;

	double fps ;
	double kTPS ;

	double weightFPS ;
	double weightKTPS ;
} ;

// マルチテクスチャ時のテクスチャユニット数制限
#define LIMIT_MAX_TEXTURE_UNITS		0

// デフォルトテクスチャサイズ（２のべき乗）
#define DEFAULT_TEXTURE_SIZE			6
#define DEFAULT_TEXTURE_SIZE_MIPMAPTEST	8

// ベンチマークテストの項目数
#define N_GLCLOCK_MARK_LIGHT_TEST				24
#define NO_GLCLOCK_MARK_LIGHT_TEST				1.0

#define N_GLCLOCK_MARK_LIGHT_TEX_TEST			6
#define NO_GLCLOCK_MARK_LIGHT_TEX_TEST			2.0
#define NO_GLCLOCK_MARK_LIGHT_TEX_SPECULAR_TEST	2.1

#define N_GLCLOCK_MARK_FILTER_TEST		5
#define NO_GLCLOCK_MARK_FILTER_TEST		3.0

#define N_GLCLOCK_MARK_FOG_TEST			4
#define NO_GLCLOCK_MARK_FOG_TEST		6.0
#define NO_GLCLOCK_MARK_FOG_TEX_TEST	6.1


// ディティールテクスチャ時の光源は明るく
//#define BRIGHT_DETAIL_MODE		1.0987f
//#define BRIGHT_DETAIL_MODE		1.1457813f

#define DRAW_DETAIL_FIRST_WALL	1

#define BITS_RADIUS	11.2f
#define HDRN_RADIUS	11.0f

#define ROOF_RAD	150.0f
#define ROOF_MOVE	11.0f

#define QUALITY		5
#define ANTI		0
#define FIELD		0
#define DEPTH_OF_FIELD_RANGE	1.0f
#define MOTION		0
#define S_MOTION	0
#define SHUTTER		0.999f
#define S_SHUTTER	1.0f

#define S_MOTION_ALPHA	(2.0f - (2.0f / s_motion))
#define PRE_S_MOTION_A	(2.0f - (2.0f / pre_s_motion))

#define BG_COLOR	0
#define DITHER		1

#define SOLID_SATURATION		1.0f
#define SOLID_CONTRAST			1.0f

#define LIGHT					5
#define LIGHT_VIVIDNESS			1.0f
#define LIGHT_MASTER_VIVIDNESS	1.5f
#define LIGHT_MARK_VIVIDNESS	4.0f

enum
{
	LIGHT_TYPE_DIRECTIONAL = 0,
	LIGHT_TYPE_POINT       = 1,
	LIGHT_TYPE_SPOT        = 2
} ;

#define LIGHT_TYPE	LIGHT_TYPE_DIRECTIONAL

#define POSITIONAL_LIGHT_DISTANCE		200.0f
#define POSITIONAL_LIGHT_CONST_ATTN		(0.0f)
#define POSITIONAL_LIGHT_LINEAR_ATTN	(0.0f / (POSITIONAL_LIGHT_DISTANCE))
#define POSITIONAL_LIGHT_QUAD_ATTN		(1.0f / ((POSITIONAL_LIGHT_DISTANCE) * (POSITIONAL_LIGHT_DISTANCE)))
#define SPOT_LIGHT_CUTOFF				60.0f
#define SPOT_LIGHT_EXPONENT				2.4f

#define LOCAL_LIGHT_AMBIENT_RATIO	0.2f

// 回転ライトフラグ（-1：自動、0：OFF、1：ON）
#define ROTATE_LIGHT	-1

#define ENV_FILE	"sky.ppm"
#define LIGHT_FILE	"light.ppm"
#define SOLID_FILE	"wood.ppm"
#define SLD2_FILE	""

#define IS_SET_DETAIL_FILE	""
#define DETAIL_FILE_REAL	"detail.pgm"
#define DETAIL_FILE_ENV		"detail2.pgm"

#define FILTER_TEST_FILE	"filter.pgm"
#define ENV_TEX		0
#define SOLID_TEX	0
#define MIN_FILTER	"LL"

#define MAX_ANISOTROPY	-1.0f

#define SPECULAR		1.0f
#define MASTER_BRIGHT	1.0f
#define COLLECT_MASTER_BRIGHT	0.8f
#define COLLECT_NORMAL_BRIGHT	1.25f	// 通常時計のみ少し明るく

#define CRYSTAL		0
#define CRYSTAL_A	0.5f
#define POL_MODE	"F"
#define REALITY		0
#define REFLECTION	-1
#define DETAIL_TEX	-1
#define PERS_CORR	"D"
#define FOG_HINT	"D"

#define TEX_SCALE	32.0f
#define FILTER_TEST_TEX_SCALE	6.4f
#define MIPMAP_TEST_TEX_SCALE	2.0f

#define IS_SET_DETAIL_TEX_SCALE	0.0f
#define DETAIL_TEX_SCALE_REAL	6.0f
#define DETAIL_TEX_SCALE_ENV	8.0f
//#define DETAIL_TEX_SCALE		4.0f

// ソリッドテクスチャの座標系回転度数
#define TEX_ROTATE				20.0f

#define ORG_IMAGE_TEX_ROTATE	0.0f
#define ORG_IMAGE_TEX_SCALE		29.0f

#define TEX_SIZE	0
#define M_SLEEP		0
#define SLEEP_RATIO	0.0f

#define U_SMOOTH	-1
#define V_SMOOTH	-1
#define G_SMOOTH	-1

#define N_D_WIDTH	1.6f
#define N_WIDTH		1.0f

#define N_HOUR_WIDTH		1.2f
#define N_MINUTE_WIDTH		0.7f
#define N_SECOND_WIDTH		0.35f

//#define N_MINUTE_HEIGHT		0.6f
//#define N_SECOND_HEIGHT		1.5f
#define N_HOUR_HEIGHT		1.5f
#define N_MINUTE_HEIGHT		1.2f
#define N_SECOND_HEIGHT		1.1f

#define N_HOUR_LENGTH		1.0f
#define N_MINUTE_LENGTH		1.4f
#define N_SECOND_LENGTH		1.7f

#define N_HOUR_TRANS_Y		1.5f
#define N_MINUTE_TRANS_Y	2.3f
#define N_SECOND_TRANS_Y	0.5f
//#define N_HOUR_TRANS_Y		3.0f
//#define N_MINUTE_TRANS_Y	5.0f
//#define N_SECOND_TRANS_Y	4.0f

#define N_HOUR_TRANS_Z		0.0f
#define N_MINUTE_TRANS_Z	1.2f
#define N_SECOND_TRANS_Z	1.8f
//#define N_SECOND_TRANS_Z	1.5f

#define N_MIRROR_HEIGHT_RATIO	0.3f

#define FOUT_FPS	""
#define FOUT_GL		""

#define PRE_FPS_MESSAGE	""

#define TEX_FORMAT					"GL_RGB8"
#define TEX_FORMAT_BENCHMARK		"GL_RGB"
#define TEX_FORMAT_COMPRESSED		"GL_COMPRESSED_RGB"
#define TEX_FORMAT_COMPRESSED_S3TC	"GL_COMPRESSED_RGB_S3TC_DXT1"
#define TEX_FORMAT_COMPRESSED_FXT1	"GL_COMPRESSED_RGB_FXT1"

#define Z_OFFSET	0.0f
#define Z_OFFSET_TNT_FIX	0.1f

#define SHELL_VTX	21
#define SHELL2_VTX	13
#define ROOF_VTX	7
#define ROOF2_VTX	5
#define M_VTX		9

#define B_DIST		-80.0f
#define T_RATIO		0.2f
#define I_RATIO		0.8f
#define T_MAX		40.0f
#define P_MIN		0.05f
#define P_MAX		20.0f
#define IR_RATIO	0.95f

#define BPPIXEL		0

#define U_SMOOTH_FLAG	(u_smooth == -1 ? (divide >= 12) : u_smooth)
#define V_SMOOTH_T(t)	(v_smooth == -1 ? (t) : NULL)

#define IS_SET_M_RATIO	(texture == 2 || solid)
#define SOLID_M_RATIO	(0.75f - solid * 0.1f)
#define METAL_R_M_RATIO	(reality ? 0.1f : SOLID_M_RATIO)

//#define IS_SET_M_RATIO	(mode_env == 2 || mode_solid)
//#define SOLID_M_RATIO	(0.75f - mode_solid * 0.1f)
//#define METAL_R_M_RATIO	(mode_real ? 0.1f : SOLID_M_RATIO)

// 高速スーパサンプリングフラグ
#define	WITHOUT_ACCUM_BUFFER_SUPER_SAMPLING	(mode_solid == 1 || mode_env == 1 || mode_crystal)

#define	ADDITIVE_ALPHA_SUPER_SAMPLING	(mode_crystal)

// 法線設定フラグ（-solid 1 または、normal でライト数０の場合 FALSE）
#define	FLAG_GL_NORMAL	(!(mode_solid == 1 || (mode_normal && !light)))

// 表面ガラスのテクスチャ座標指定（-solid 1 の場合のみ TRUE）
#define	FLAG_GLASS_TEX_COORD	(mode_solid == 1)

// シェーディングのテクスチャ座標指定（殻、蓋、文字盤）（-s 2, 3(13) -r 2(12)）
#define	FLAG_SHADE_TEX_COORD	(mode_solid ==2 || mode_solid == 3 || mode_solid == 13 || mode_real == 2 || mode_real == 12)

// シェーディングのテクスチャ座標指定（金飾り、針、回転体）（-s 2, 3）
#define	FLAG_SHADE_TEX_COORD_ONLY_SOLID	(mode_solid ==2 || mode_solid == 3 || mode_solid == 13)

// モデュレートのテクスチャ座標指定フラグ（-solid 1）
#define	FLAG_MODULATE_TEX_COORD	(mode_solid == 1 || mode_real == 1)


// Detail Blend
#define DETAIL_BLEND	-1
enum
{
	DETAIL_BLEND_MULTIPLY_DST_BASE = 0,
	DETAIL_BLEND_MULTIPLY_SRC_BASE = 1,
} ;

// Specular Blend
#define SPECULAR_BLEND	-1
enum
{
	SPECULAR_BLEND_ADD_SMOOTH	= 0,
	SPECULAR_BLEND_ADD			= 1,
} ;

// フォグ
enum
{
	FOG_NONE	= 0,
	FOG_LINEAR	= 1,
	FOG_EXP		= 2,
	FOG_EXP2	= 3,
} ;

//#define FOG_MODE		FOG_NONE
#define FOG_MODE		FOG_LINEAR
#define FOG_DENSITY		0.001f
#define FOG_MAX_RATIO	10.0f
#define FOG_START		0.0f
#define FOG_END			2048.0f

#define FOG_MODE_SAVAGE2000		FOG_EXP

// ６０ループまたは１秒を超えたらＦＰＳ計算（ベンチマークモード以外）
#define F_COUNT			60
#define F_TIME_COUNT	1


// OpenGLドライバおよびビデオカード種別
enum
{
	GENERIC, MESA_LIB,
	VOODOO_AUTO, VOODOO_MESA, VOODOO_MESA_OLD,
	VOODOO, VOODOO2, VOODOO2_SLI,
	VOODOO_RUSH, VOODOO_BANSHEE, VOODOO_3DFX_ICD,
	PERMEDIA2, SAVAGE2000, G400,
	RIVA, RIVA_TNT_FIX, WIN95_GDI_FIX,
} ;


struct TimeAngle
{
	float	hour, minute, second ;
} ;

// 時間計算用構造体
struct TimeData
{
	time_t	t ;
	struct	tm	*t_area ;
	int	n_hour, n_minute, n_second, p_hour, p_minute, p_second, fp_hour, fp_minute, fp_second ;
	TimeAngle angle ;
} ;

struct ClockState
{
	TimeAngle tm ;
	GLfloat  h_rotate, g_spin, pers, clock_z, roof, y_pers ;

	// 秒針だけのモーションブラー用スタート角度とステップ角度
	GLfloat  start_angle, step_angle ;
} ;


#define TIME_RESOLUTION		50

// ミリ秒単位時間ゲット
inline int GetMiliSeconds()
{
#ifdef WIN32
//	return glutGet((GLenum)GLUT_ELAPSED_TIME) ;
	return timeGetTime() ;

#else	// #ifdef WIN32
	return glutGet((GLenum)GLUT_ELAPSED_TIME) ;

#endif	// #ifdef WIN32 ... #else
}


#ifdef WIN32
extern HINSTANCE hInstanceGlClock ;
#define ID_ICON_RESOURCE	100
#define	ICON_NO				0
#endif	// #ifdef WIN32


extern "C"
{
#ifdef WIN32
DLL_EXPORT BOOL CCONV DllMain(HANDLE, ULONG, LPVOID) ;
#endif

DLL_EXPORT int CCONV glclock_opt(char *opt) ;
DLL_EXPORT int CCONV glclock_arg(int argc, char** argv) ;
}	// extern "C"

int glclock(char *opt) ;
int glclock(int argc, char** argv) ;


// for fake sphere-mapping
// 行列モード切替等は一切なし
inline void ScaleTranslateMatrixForFake()
{
	glScalef(0.5f, 0.5f, 1.0f) ;
	glTranslatef(1.0f, 1.0f, 0.0f) ;
}


#endif	// #ifndef _GLCLOCK_H_
