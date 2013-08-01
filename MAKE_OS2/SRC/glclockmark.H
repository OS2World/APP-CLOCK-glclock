
#ifndef _GLCLOCK_MARK_H_
#define _GLCLOCK_MARK_H_

// Compile for CGI
//#define	CGI_MODE
//#define	DEBUG_CGI_MODE

#if defined DEBUG_CGI_MODE && !defined CGI_MODE
#define CGI_MODE
#endif

#define CGI_PATH	""
#define CGI_LAST_MODIFIED	"Sat, 10 Jun 2000 06:45:23 GMT"	// GNU shelutils: date -u -R

// スコア計算のウェイトを有効に
#define ENABLE_SCORE_WEIGHT

// for Win32
#ifdef   WIN32
#define  WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#include <shellapi.h>
#include <tchar.h>

// for UNIX etc.
#else	// #ifdef   WIN32
#include <unistd.h>
#endif	// #ifdef   WIN32 ... #else

// for Mac
#ifdef macintosh
#include "macsupport.h"
#endif	// #ifdef macintosh

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdarg.h>

#include "MString.H"
#include "Option.H"
#include "Math3D.H"

// 出力ファイル
#define FILE_NAME		"glclockmark"
#define SUFFIX_LOG		".log"
#define SUFFIX_TXT		".txt"
#define SUFFIX_HTML		".html"

// FPS, kTPS の平均から計算するか
// パーセンテージの平均から計算するか
// GLCLOCK_MARK_BASE_FPS_KTPS を定義すれば FPS, kTPS から計算
#define GLCLOCK_MARK_BASE_FPS_KTPS

// モード識別子
#define VIEWPORT_DEFAULT	"320 x 320"
#define VIEWPORT_VGA		"640 x 480"
#define VIEWPORT_SVGA		"800 x 600"
#define VIEWPORT_XGA		"1024 x 768"

// 参照環境ログファイル
#define FILE_REF_LOG	"glclockmark_refdata.log" 
/*
#ifdef macintosh
#define FILE_REF_LOG	"glclockmark_refdata.log" 
#else
#define FILE_REF_LOG	"glclockmark_ref_Win98-PII233-TNT-PCI16MB.log"
//#define FILE_REF_LOG	"glclockmark_ref_Win98-PIII500-TNT2U-AGP32MB.log"
#endif
*/

// 参照環境
#define SPEC_REF_ENV	"PII 233MHz Win98 + TNT PCI-16MB (320 x 320 x 16bpp)"
//#define SPEC_REF_ENV	"P!!! 500MHz Win98 + TNT2 Ultra AGP-32MB (320 x 320 x 16bpp)"
#define SPEC_TESTED_ENV	"Tested Environment"

// グラフのバーの基準の長さ
#define BASE_BAR_LENGTH		100
#define SCALE_BAR_LENGTH	1.92

// 各章タイトル
#define BAR_LENGTH_LEVEL_1		0.9
#define BAR_HEIGHT_LEVEL_1		13
#define BAR_TEXT_SIZE_LEVEL_1	"3"
#define TD_FPS_LEVEL_1			"<TD NOWRAP WIDTH=\"80\" ALIGN=\"RIGHT\">"
#define TD_KTPS_LEVEL_1			"<TD NOWRAP WIDTH=\"120\" ALIGN=\"RIGHT\">"
#define TD_PERCENT_LEVEL_1		"<TD NOWRAP WIDTH=\"70\" ALIGN=\"RIGHT\">"

// Contents の各バー
#define BAR_LENGTH_LEVEL_2		0.8
#define BAR_HEIGHT_LEVEL_2		10
#define BAR_TEXT_SIZE_LEVEL_2	"2"
#define TD_FPS_LEVEL_2			"<TD NOWRAP WIDTH=\"80\" ALIGN=\"RIGHT\">"
#define TD_KTPS_LEVEL_2			"<TD NOWRAP WIDTH=\"100\" ALIGN=\"RIGHT\">"
#define TD_PERCENT_LEVEL_2		"<TD NOWRAP WIDTH=\"70\" ALIGN=\"RIGHT\">"

// 各マークの小節
#define BAR_LENGTH_LEVEL_3		0.7
#define BAR_HEIGHT_LEVEL_3		9
#define BAR_TEXT_SIZE_LEVEL_3	"2"
#define TD_FPS_LEVEL_3			"<TD NOWRAP WIDTH=\"80\" ALIGN=\"RIGHT\">"
#define TD_KTPS_LEVEL_3			"<TD NOWRAP WIDTH=\"100\" ALIGN=\"RIGHT\">"
#define TD_PERCENT_LEVEL_3		"<TD NOWRAP WIDTH=\"70\" ALIGN=\"RIGHT\">"

// 各マーク詳細レベル１（一次）
#define BAR_LENGTH_LEVEL_4		0.65
#define BAR_HEIGHT_LEVEL_4		8
#define BAR_TEXT_SIZE_LEVEL_4	"2"
#define TD_FPS_LEVEL_4			"<TD NOWRAP WIDTH=\"80\" ALIGN=\"RIGHT\">"
#define TD_KTPS_LEVEL_4			"<TD NOWRAP WIDTH=\"100\" ALIGN=\"RIGHT\">"
#define TD_PERCENT_LEVEL_4		"<TD NOWRAP WIDTH=\"70\" ALIGN=\"RIGHT\">"
//#define TD_FPS_LEVEL_4			"<TD WIDTH=\"50\" ALIGN=\"RIGHT\">"
//#define TD_KTPS_LEVEL_4			"<TD WIDTH=\"65\" ALIGN=\"RIGHT\">"
//#define TD_PERCENT_LEVEL_4		"<TD WIDTH=\"43\" ALIGN=\"RIGHT\">"

// 各マーク詳細レベル２（二次）
#define BAR_LENGTH_LEVEL_5		0.5
#define BAR_HEIGHT_LEVEL_5		5
#define BAR_TEXT_SIZE_LEVEL_5	"1"
#define TD_FPS_LEVEL_5			"<TD NOWRAP WIDTH=\"50\" ALIGN=\"RIGHT\">"
#define TD_KTPS_LEVEL_5			"<TD NOWRAP WIDTH=\"65\" ALIGN=\"RIGHT\">"
#define TD_PERCENT_LEVEL_5		"<TD NOWRAP WIDTH=\"43\" ALIGN=\"RIGHT\">"
//#define TD_FPS_LEVEL_5			"<TD WIDTH=\"40\" ALIGN=\"RIGHT\">"
//#define TD_KTPS_LEVEL_5			"<TD WIDTH=\"50\" ALIGN=\"RIGHT\">"
//#define TD_PERCENT_LEVEL_5		"<TD WIDTH=\"35\" ALIGN=\"RIGHT\">"


#define BAR_FILE_GREEN	"bar_green.jpg"
#define BAR_FILE_GOLD	"bar_gold.jpg"
#define BAR_FILE_RED	"bar_red.jpg"
#define BAR_FILE_GRAY	"bar_gray.jpg"

#define TITLE_CONTENTS			"Contens"
#define LINK_STRING_CONTENTS	"return to Contens"

#define URL_MASA_HP			"http://www.daionet.gr.jp/~masa/"
#define LINK_STRING_MASA_HP	"Masa's Web Site"
#define URL_MARK_DB			"http://www.daionet.gr.jp/~masa/glclock/glclockmark/"
#define LINK_STRING_MARK_DB	"Results Database"
#define MAIL_MASA			"masa@daionet.gr.jp"
#define MAIL_STRING_MASA	"E-Mail to Masa"

#define LINK_TOP			"Top"
#define LINK_DETAILS		"Details"
#define LINK_ENVIRONMENT	"Environment"

#define LINK_LIGHTING_MARK		"LightingMark"
#define LINK_LIGHTED_TEX_MARK	"LightedTexMark"
#define LINK_FILTERING_MARK		"FilteringMark"
#define LINK_ENV_MAPPING_MARK	"EnvMappingMark"
#define LINK_SUPER_SAMPLE_MARK	"SuperSampleMark"
#define LINK_MAX_TRIANGLES_MARK	"MaxTrianglesMark"
#define LINK_PRACTICAL_MARK		"PracticalMark"

#define LINK_HIGHEST_MARK		"HighestMark"
#define LINK_HIGHEST_FPS		"HighestFPS"
#define LINK_HIGHEST_KTPS		"HighestKTPS"

#define CHAPTER_OPEN_GL_INFO	"Informations"


#define HIGH_SCORE_MARK_SPEC	"Highest Mark"
#define HIGH_SCORE_FPS_SPEC		"Highest FPS"
#define HIGH_SCORE_KTPS_SPEC	"Highest kTPS"

//#define FLAG_TABLE_BORDER

#ifdef FLAG_TABLE_BORDER
#define HTML_TABLE_BORDER	"BORDER "
#else
#define HTML_TABLE_BORDER	""
#endif
//#define HTML_TABLE_EX	"CELLSPACING=1 CELLPADDING=2 BGCOLOR=\"#191919\" BACKGROUND=\"background.gif\""
#define HTML_TABLE_EX	" CELLPADDING=0"

#define N_MARK_EXECUTE	13
#define N_ALL_MARK		"26"	// N_MARK_EXECUTE * 2

// 番号の誤差考慮
#define NO_THRETHOLD	0.001

#define WIDTH	320
#define HEIGHT	320

#ifdef WIN32
#define GLCLOCK_EXECUTE	"glclock.exe"
#else	// #ifdef   WIN32
#define GLCLOCK_EXECUTE	"glclock"
#endif	// #ifdef   WIN32 ... #else

#define MAX_LINE_BUF	1024

class MarkLogTable
{
	int nAlloc ;
public:
	int nLine ;
	String *line ;

	void Clear()
	{
		if (line) delete [] line ;
		line = NULL ;
		nAlloc = 0 ;
		nLine = 0 ;
	}

	// デフォルトコンストラクタ
	MarkLogTable() { line = NULL ; Clear() ; }

	// テーブルに文字列追加（追加文字列の最後は必ず改行が必要）
	// 結果の行数（テーブルの要素数）を返す
	int AddLine(const String& str)
	{
		// 最初の一回
		if (!nAlloc)
		{
			nAlloc = 16 ;
			line = new String[nAlloc] ;
		}

		// ライン追加
		if (nLine >= nAlloc)
		{
			// 再アロケート
			String *work = new String[nAlloc] ;
			for (int i = 0 ; i < nAlloc ; i ++)
				work[i] = line[i] ;

			delete [] line ;
			line = new String[nAlloc * 2] ;

			for (int i = 0 ; i < nAlloc ; i ++)
				line[i] = work[i] ;

			delete [] work ;
			nAlloc *= 2 ;
		}

		line[nLine] = str ;
		nLine ++ ;
		return nLine ;
	}

	~MarkLogTable()
	{
		if (line) delete [] line ;
	}
} ;


// １項目のテスト結果
class Mark
{
public:
	String title ;		// "5 Directional ...
	double no ;			// 1.01 etc.
	String extension ;	// "Single Buffering ...
	double  fps ;
	double  kTPS ;

	int other ;

	// デフォルトコンストラクタ
	Mark() { no = 0.0 ; fps = 0.0 ; kTPS = 0.0 ; }

	// コピーコンストラクタ
	Mark(const Mark& src)
	{
		title = src.title ;
		no = src.no ;
		extension = src.extension ;
		fps = src.fps ;
		kTPS = src.kTPS ;
	}

	// 代入演算子
	Mark& operator =(const Mark& src)
	{
		title = src.title ;
		no = src.no ;
		extension = src.extension ;
		fps = src.fps ;
		kTPS = src.kTPS ;

		other = src.other ;

		return *this ;
	}

	// テキストから計測値をセット
	void Set(const String& line1, const String& line2, const String& line3)
	{
		// タイトルは '#*.** ' をカット
		title = Right(line1, line1.Length() - 6) ;
		no = atof(Mid(line1, 2, 4)) ;	// 1.01 etc.

		// 拡張部はそのままコピー
		extension = line2 ;

		// line3 から FPS, kTPS を取得
		{
			char *str = new char[strlen(line3) + 1] ;
			char *token ;
			int  recLen = 0, count = 0 ;
			String tok[32] ;

			strcpy(str, line3) ;
			token = strtok(str, " ") ;

			while(token)
			{
				tok[count] = token ;
				count ++ ;
				token = strtok(NULL, " ") ;
			}

			delete [] str ;

			// FPS の直前、kTPS の直前の数値
			for (int i = 0 ; i < count ; i ++)
			{
				if (tok[i] == "FPS" && i >= 1)
					fps = atof(tok[i - 1]) ;
				else if (tok[i] == "kTPS" && i >= 1)
					kTPS = atof(tok[i - 1]) ;
			}

			if (fps < 0.0 || fps >= 1000000.0)	// 限界１００万
				fps = 0.0 ;

			if (kTPS < 0.0 || kTPS >= 100000000.0)	// 限界１０００億
				kTPS = 0.0 ;
		}
/*
		// FPS
		int end = line3.Search("FPS") ;
		if (!end)
			fps = 0.0 ;
		else
			fps = atof(Left(line3, end - 1)) ;
		if (fps < 0.0 || fps >= 10000.0)
			fps = 0.0 ;

		// kTPS
		end = line3.Search("kTPS") ;
		if (!end)
			kTPS = 0.0 ;
		else
			kTPS = atof(Mid(line3, end - 14, 13)) ;
		if (kTPS < 0.0 || kTPS >= 1000000.0)
			kTPS = 0.0 ;
*/
	}
} ;


class MarkTable
{
	int nAlloc ;
public:
	int nMark ;
	Mark *mark ;

	void Clear()
	{
		if (mark) delete [] mark ;
		mark = NULL ;
		nAlloc = 0 ;
		nMark = 0 ;
		mark = NULL ;
	}

	// デフォルトコンストラクタ
	MarkTable() { mark = NULL ; Clear() ; }

	// テーブルにテスト項目追加（追加文字列の最後は必ず改行が必要）
	// 追加後の要素数を返す
	int AddMark(const Mark& aMark)
	{
		// 最初の一回
		if (!nAlloc)
		{
			nAlloc = 16 ;
			mark = new Mark[nAlloc] ;
		}

		// ライン追加
		if (nMark >= nAlloc)
		{
			// 再アロケート
			Mark *work = new Mark[nAlloc] ;
			for (int i = 0 ; i < nAlloc ; i ++)
				work[i] = mark[i] ;

			delete [] mark ;
			mark = new Mark[nAlloc * 2] ;

			for (int i = 0 ; i < nAlloc ; i ++)
				mark[i] = work[i] ;

			delete [] work ;
			nAlloc *= 2 ;
		}

		mark[nMark] = aMark ;
		nMark ++ ;
		return nMark ;
	}

	~MarkTable()
	{
		if (mark) delete [] mark ;
	}
} ;


class LightingMark
{
public:
	double lightingMark ;
	double percentage[6] ;
	double detailPercentage[24] ;

	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double averageFPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	double FPS[6] ;		// ６種類の組み合わせの平均
	Mark mark[24][2] ;

	static String title[6] ;
	static String HTMLname[6] ;

	enum
	{
		NM_WEIGHT_N_LIGHT = 4,
		NM_WEIGHT_LIGHT_MODEL = 6,
	} ;

	double weight_sum_nLight ;
	double weight_sum_lightModel ;
	static double weight_nLight[NM_WEIGHT_N_LIGHT] ;
	static double weight_lightModel[NM_WEIGHT_LIGHT_MODEL] ;

	LightingMark()
	{
		lightingMark = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		averageFPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		for (int i = 0 ; i < 6 ; i ++)
			FPS[i] = 0.0 ;

		weight_sum_nLight = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_N_LIGHT ; i ++)
			weight_sum_nLight += weight_nLight[i] ;

		weight_sum_lightModel = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_LIGHT_MODEL ; i ++)
			weight_sum_lightModel += weight_lightModel[i] ;
	}

	// Lighting Mark セットアップ
	void Set(const MarkTable& markTable)
	{
		for (int i = 0 ; i < 24 ; i++)
		{
			mark[i][0].fps = 0.0 ;
			mark[i][1].fps = 0.0 ;
		}

		for (int i = 0 ; i < 24 ; i ++)
		{
			double no = 1.01 + i * 0.01 ;	// 1.01 〜 1.24

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single") && mark[i][1].fps < markTable.mark[l].fps)	// Single Buffer
						mark[i][1] = markTable.mark[l] ;
					if (markTable.mark[l].extension.Search("Double") && mark[i][0].fps < markTable.mark[l].fps)	// Double Buffer or ???
						mark[i][0] = markTable.mark[l] ;
				}
			}
		}

		// セットしたデータから、FPS 平均値を計算
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;
		for (int i = 0 ; i < 6 ; i ++)
			FPS[i] = 0.0 ;

		for (int i = 0 ; i < 24 ; i ++)
		{
			double wn = weight_nLight[i % 4] ;
			double wm = weight_lightModel[i / 4] ;

			double fast ;
			fast = Max(mark[i][0].fps, mark[i][1].fps) ;

			averageFPS += fast * wn * wm ;

			highestFPS = Max(highestFPS, fast) ;
			highestKTPS = Max(highestKTPS, Max(mark[i][0].kTPS, mark[i][1].kTPS)) ;

			FPS[i / 4] += fast * wn ;

			dBufferFPS += mark[i][0].fps * wn * wm ;
			sBufferFPS += mark[i][1].fps * wn * wm ;
		}

//		averageFPS /= 18 ;	// ４ライト以上のみの平均
		averageFPS /= weight_sum_nLight * weight_sum_lightModel ;
		dBufferFPS /= weight_sum_nLight * weight_sum_lightModel ;
		sBufferFPS /= weight_sum_nLight * weight_sum_lightModel ;

		for (int i = 0 ; i < 6 ; i ++)
			FPS[i] /= weight_sum_nLight ;	// 1,2,4,8 光源平均
	}

	// Lighting Mark 計算
	void CalculateMark(LightingMark& reference)
	{
		// 各テスト毎にパーセンテージを計算し、その平均値を Lighting Mark とする
		for (int i = 0 ; i < 6 ; i ++)
			percentage[i] = 0.0 ;

		highestMark = 0.0 ;

		for (int i = 0 ; i < 24 ; i ++)
		{
			double ref = Max(reference.mark[i][0].fps, reference.mark[i][1].fps) ;
			if (ref > 0.0)
				detailPercentage[i] = Max(mark[i][0].fps, mark[i][1].fps) / ref ;
			else
				detailPercentage[i] = 0.0 ;

			highestMark = Max(highestMark, detailPercentage[i]) ;

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
			percentage[i / 4] += detailPercentage[i] ;
#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		}

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		lightingMark = 0.0 ;
		for (int i = 0 ; i < 6 ; i ++)
		{
			percentage[i] /= 4 ;
			lightingMark += percentage[i] ;
		}

		lightingMark /= 6 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS

		for (int i = 0 ; i < 6 ; i ++)
		{
			if (reference.FPS[i] > 0.0)
				percentage[i] = FPS[i] / reference.FPS[i] ;
			else
				percentage[i] = 0.0 ;
		}

		if (reference.averageFPS > 0.0)
			lightingMark = averageFPS / reference.averageFPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		for (int i = 0 ; i < 6 ; i ++)
			percentage[i] = 1.0 ;

		lightingMark = 1.0 ;
		highestMark = 1.0 ;
	}
} ;

class LightingTexMark
{
public:
	double lightingTexMark ;
	double singleColorPercentage ;
	double separateSpecularPercentage ;
	double detailSingleColorPercentage[6] ;
	double detailSeparateSpecularPercentage[6] ;

	double averageFPS ;
	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double singleColorFPS ;
	double separateSpecularFPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	Mark singleColorMark[6][2] ;
	Mark separateSpecularMark[6][2] ;

	static String title[2] ;
	static String detailTitle[2] ;
	static String HTMLname[2] ;

	enum
	{
		NM_WEIGHT_LIGHT_MODEL = 6,
		NM_WEIGHT_TEX_BLEND = 2,
	} ;

	double weight_sum_lightModel ;
	double weight_sum_texBlend ;
	static double weight_lightModel[NM_WEIGHT_LIGHT_MODEL] ;
	static double weight_texBlend[NM_WEIGHT_TEX_BLEND] ;

	LightingTexMark()
	{
		lightingTexMark = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		singleColorFPS = 0.0 ;
		separateSpecularFPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_lightModel = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_LIGHT_MODEL ; i ++)
			weight_sum_lightModel += weight_lightModel[i] ;

		weight_sum_texBlend = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_TEX_BLEND ; i ++)
			weight_sum_texBlend += weight_texBlend[i] ;
	}

	// Lighting Texture Mark セットアップ
	void Set(const MarkTable& markTable)
	{
		for (int i = 0 ; i < 6 ; i ++)
		{
			// Single Color Blending
			double no = 2.01 + i * 0.01 ;	// 2.01 〜 2.06

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						singleColorMark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						singleColorMark[i][0] = markTable.mark[l] ;
				}
			}

			// Separate Specular Color Blending
			no = 2.11 + i * 0.01 ;	// 2.11 〜 2.16

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						separateSpecularMark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						separateSpecularMark[i][0] = markTable.mark[l] ;
				}
			}
		}

		// セットしたデータから、FPS 平均値を計算
		// セットしたデータから、FPS 平均値を計算
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		singleColorFPS = 0.0 ;
		separateSpecularFPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		double wSingle = weight_texBlend[0] ;
		double wSeparete = weight_texBlend[1] ;

		for (int i = 0 ; i < 6 ; i ++)
		{
			double wm = weight_lightModel[i] ;

			// Single, Double のうち速い方
			double single, separate ;

			single = Max(singleColorMark[i][0].fps, singleColorMark[i][1].fps) ;
			singleColorFPS += single * wm ;

			separate = Max(separateSpecularMark[i][0].fps, separateSpecularMark[i][1].fps) ;
			separateSpecularFPS += separate * wm ;

			highestFPS = Max(highestFPS, Max(single, separate)) ;

			double singleKTPS = Max(singleColorMark[i][0].kTPS, singleColorMark[i][1].kTPS) ;
			double separateKTPS = Max(separateSpecularMark[i][0].kTPS, separateSpecularMark[i][1].kTPS) ;

			highestKTPS = Max(highestKTPS, Max(singleKTPS, separateKTPS)) ;

			dBufferFPS += (singleColorMark[i][0].fps * wSingle +
						   separateSpecularMark[i][0].fps * wSeparete) * wm ;
			sBufferFPS += (singleColorMark[i][1].fps * wSingle +
						   separateSpecularMark[i][1].fps * wSeparete) * wm ;
		}

		singleColorFPS /= weight_sum_lightModel ;
		separateSpecularFPS /= weight_sum_lightModel ;
		averageFPS = (singleColorFPS * wSingle + separateSpecularFPS * wSeparete) / weight_sum_texBlend ;
		dBufferFPS /= weight_sum_lightModel * weight_sum_texBlend ;
		sBufferFPS /= weight_sum_lightModel * weight_sum_texBlend ;
	}

	// Lighted Texturing Mark 計算
	void CalculateMark(LightingTexMark& reference)
	{
		// 各テスト毎にパーセンテージを計算し、その平均値を Lighted Texturing Mark とする
		singleColorPercentage = 0.0 ;
		separateSpecularPercentage = 0.0 ;

		highestMark = 0.0 ;

		for (int i = 0 ; i < 6 ; i ++)
		{
			double refSingleFPS   = Max(reference.singleColorMark[i][0].fps, reference.singleColorMark[i][1].fps) ;
			double refSeparateFPS = Max(reference.separateSpecularMark[i][0].fps, reference.separateSpecularMark[i][1].fps) ;

			if (refSingleFPS > 0.0)
				detailSingleColorPercentage[i] = Max(singleColorMark[i][0].fps, singleColorMark[i][1].fps) / refSingleFPS ;
			else
				detailSingleColorPercentage[i] = 1.0 ;

			if (refSeparateFPS > 0.0)
				detailSeparateSpecularPercentage[i] = Max(separateSpecularMark[i][0].fps, separateSpecularMark[i][1].fps) / refSeparateFPS ;
			else
				detailSeparateSpecularPercentage[i] = 1.0 ;

			singleColorPercentage += detailSingleColorPercentage[i] ;
			separateSpecularPercentage += detailSeparateSpecularPercentage[i] ;

			highestMark = Max(highestMark, Max(detailSingleColorPercentage[i], detailSeparateSpecularPercentage[i])) ;
		}

		singleColorPercentage /= 6 ;
		separateSpecularPercentage /= 6 ;

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		lightingTexMark = (singleColorPercentage + separateSpecularPercentage) / 2 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		singleColorPercentage = 0.0 ;
		separateSpecularPercentage = 0.0 ;

		if (reference.singleColorFPS > 0.0)
			singleColorPercentage = singleColorFPS / reference.singleColorFPS ;

		if (reference.separateSpecularFPS > 0.0)
			separateSpecularPercentage = separateSpecularFPS / reference.separateSpecularFPS ;

		if (0.0 < reference.averageFPS)
			lightingTexMark = averageFPS / reference.averageFPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		singleColorPercentage = 1.0 ;
		separateSpecularPercentage = 1.0 ;

		lightingTexMark = 1.0 ;
		highestMark = 1.0 ;
	}
} ;

class FilteringMark
{
public:
	double filteringMark ;
	double percentage[5] ;

	double averageFPS ;
	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	Mark mark[5][2] ;

	enum
	{
		NM_WEIGHT_FILTER = 5,
	} ;

	double weight_sum_filter ;
	static double weight_filter[NM_WEIGHT_FILTER] ;

	FilteringMark()
	{
		filteringMark = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_filter = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_FILTER ; i ++)
			weight_sum_filter += weight_filter[i] ;
	}

	// Filtering Mark セットアップ
	void Set(const MarkTable& markTable)
	{
		for (int i = 0 ; i < 5 ; i ++)
		{
			double no = 3.01 + i * 0.01 ;	// 3.01 〜 3.05

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						mark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						mark[i][0] = markTable.mark[l] ;
				}
			}
		}

		// セットしたデータから、FPS 平均値を計算
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		for (int i = 0 ; i < 5 ; i ++)
		{
			// 平均 FPS は Single, Double のうち速い方
			double fast ;
			fast = Max(mark[i][0].fps, mark[i][1].fps) ;
			averageFPS += fast * weight_filter[i] ;

			highestFPS = Max(highestFPS, fast) ;
			highestKTPS = Max(highestKTPS, Max(mark[i][0].kTPS, mark[i][1].kTPS)) ;

			dBufferFPS += mark[i][0].fps * weight_filter[i] ;
			sBufferFPS += mark[i][1].fps * weight_filter[i] ;
		}

		averageFPS /= weight_sum_filter ;
		dBufferFPS /= weight_sum_filter ;
		sBufferFPS /= weight_sum_filter ;
	}

	// Filtering Mark 計算
	void CalculateMark(FilteringMark& reference)
	{
		filteringMark = 0.0 ;
		highestMark = 0.0 ;

		for (int i = 0 ; i < 5 ; i ++)
		{
			double refFPS ;
			refFPS = Max(reference.mark[i][0].fps, reference.mark[i][1].fps) ;
			if (refFPS > 0.0)
				percentage[i] = Max(mark[i][0].fps, mark[i][1].fps) / refFPS ;
			else
				percentage[i] = 0.0 ;

			filteringMark += percentage[i] ;

			highestMark = Max(highestMark, percentage[i]) ;
		}

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		filteringMark /= 5 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		if (reference.averageFPS > 0.0)
			filteringMark = averageFPS / reference.averageFPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		for (int i = 0 ; i < 5 ; i ++)
			percentage[i] = 1.0 ;

		filteringMark = 1.0 ;
		highestMark = 1.0 ;
	}
} ;

class EnvMapMark
{
public:
	double envMapMark ;
	double sphereMapPercentage ;
	double transSphereMapPercentage ;

	double averageFPS ;
	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double sphereMapFPS ;
	double transSphereMapFPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	Mark sphereMapMark[2] ;
	Mark transSphereMapMark[2] ;

	enum
	{
		NM_WEIGHT_RENDER = 2,
	} ;

	double weight_sum_render ;
	static double weight_render[NM_WEIGHT_RENDER] ;

	EnvMapMark()
	{
		envMapMark = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		sphereMapFPS = 0.0 ;
		transSphereMapFPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_render = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_RENDER ; i ++)
			weight_sum_render += weight_render[i] ;
	}

	// Environment Mapping Mark セットアップ
	void Set(const MarkTable& markTable)
	{
		double no = 4.01 ;
		// 項目No. に一致するデータを検索
		for (int l = 0 ; l < markTable.nMark ; l ++)
		{
			// 該当するデータが存在する場合
			if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
			{
				// Double buffer か Single Buffer どちらか？
				if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
					sphereMapMark[1] = markTable.mark[l] ;
				else	// Double Buffer or ???
					sphereMapMark[0] = markTable.mark[l] ;
			}
		}

		no = 4.02 ;
		// 項目No. に一致するデータを検索
		for (int l = 0 ; l < markTable.nMark ; l ++)
		{
			// 該当するデータが存在する場合
			if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
			{
				// Double buffer か Single Buffer どちらか？
				if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
					transSphereMapMark[1] = markTable.mark[l] ;
				else	// Double Buffer or ???
					transSphereMapMark[0] = markTable.mark[l] ;
			}
		}

		// セットしたデータから、FPS 平均値を計算

		// 平均 FPS は Single, Double のうち速い方
		sphereMapFPS      = Max(sphereMapMark[0].fps, sphereMapMark[1].fps) ;
		transSphereMapFPS = Max(transSphereMapMark[0].fps, transSphereMapMark[1].fps) ;
		highestFPS = Max(sphereMapFPS, transSphereMapFPS) ;

		highestKTPS = Max(Max(sphereMapMark[0].kTPS, sphereMapMark[1].kTPS),
						  Max(transSphereMapMark[0].kTPS, transSphereMapMark[1].kTPS)) ;

		averageFPS = (sphereMapFPS * weight_render[0] + transSphereMapFPS * weight_render[1]) / weight_sum_render ;

		dBufferFPS = (sphereMapMark[0].fps * weight_render[0] + transSphereMapMark[0].fps * weight_render[1]) / weight_sum_render ;
		sBufferFPS = (sphereMapMark[1].fps * weight_render[0] + transSphereMapMark[1].fps * weight_render[1]) / weight_sum_render ;
	}

	// Environment Mapping Mark 計算
	void CalculateMark(EnvMapMark& reference)
	{
		double ref = Max(reference.sphereMapMark[0].fps, reference.sphereMapMark[1].fps) ;
		if (ref > 0.0)
			sphereMapPercentage = Max(sphereMapMark[0].fps, sphereMapMark[1].fps) / ref ;
		else
			sphereMapPercentage = 0.0 ;

		ref = Max(reference.transSphereMapMark[0].fps, reference.transSphereMapMark[1].fps) ;
		if (ref > 0.0)
			transSphereMapPercentage = Max(transSphereMapMark[0].fps, transSphereMapMark[1].fps) / ref ;

		highestMark = Max(sphereMapPercentage, transSphereMapPercentage) ;

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		envMapMark = (sphereMapPercentage + transSphereMapPercentage) / 2 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		if (0.0 < reference.averageFPS)
			envMapMark = averageFPS / reference.averageFPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		sphereMapPercentage = 1.0 ;
		transSphereMapPercentage = 1.0 ;

		envMapMark = 1.0 ;
		highestMark = 1.0 ;
	}
} ;

class SuperSampleMark
{
public:
	double superSampleMark ;
	double percentage[3] ;

	double averageFPS ;
	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	Mark mark[3][2] ;

	enum
	{
		NM_WEIGHT_SAMPLE = 3,
	} ;

	double weight_sum_sample ;
	static double weight_sample[NM_WEIGHT_SAMPLE] ;

	SuperSampleMark()
	{
		superSampleMark = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_sample = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_SAMPLE ; i ++)
			weight_sum_sample += weight_sample[i] ;
	}

	// Super Sampling Mark セットアップ
	void Set(const MarkTable& markTable)
	{
		for (int i = 0 ; i < 3 ; i ++)
		{
			double no = 5.01 + i * 0.01 ;	// 5.01 〜 5.03

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						mark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						mark[i][0] = markTable.mark[l] ;
				}
			}
		}

		// セットしたデータから、FPS 平均値を計算
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		for (int i = 0 ; i < 3 ; i ++)
		{
			// 平均 FPS は Single, Double のうち速い方
			double fast = Max(mark[i][0].fps, mark[i][1].fps) ;
			averageFPS += fast * weight_sample[i] ;
			highestFPS = Max(highestFPS, fast) ;

			highestKTPS = Max(highestFPS, Max(mark[i][0].kTPS, mark[i][1].kTPS)) ;

			dBufferFPS += mark[i][0].fps * weight_sample[i] ;
			sBufferFPS += mark[i][1].fps * weight_sample[i] ;
		}

		averageFPS /= weight_sum_sample ;
		dBufferFPS /= weight_sum_sample ;
		sBufferFPS /= weight_sum_sample ;
	}

	// Super Sampling Mark 計算
	void CalculateMark(SuperSampleMark& reference)
	{
		superSampleMark = 0.0 ;
		highestMark = 0.0 ;

		for (int i = 0 ; i < 3 ; i ++)
		{
			double refFPS ;
			refFPS = Max(reference.mark[i][0].fps, reference.mark[i][1].fps) ;
			if (refFPS > 0.0)
				percentage[i] = Max(mark[i][0].fps, mark[i][1].fps) / refFPS ;
			else
				percentage[i] = 0.0 ;

			superSampleMark += percentage[i] ;
			highestMark = Max(highestMark, percentage[i]) ;
		}

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		superSampleMark /= 3 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		if (reference.averageFPS > 0.0)
			superSampleMark = averageFPS / reference.averageFPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		for (int i = 0 ; i < 3 ; i ++)
			percentage[i] = 1.0 ;

		superSampleMark = 1.0 ;
		highestMark = 1.0 ;
	}
} ;

class MaxTriMark
{
public:
	double maxTriMark ;
	double coloringPercentage ;
	double texturingPercentage ;
	double detailColoringPercentage[4] ;
	double detailTexturingPercentage[4] ;

	double averageKTPS ;
	double averageFPS ;
	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double coloringKTPS ;
	double coloringFPS ;
	double texturingKTPS ;
	double texturingFPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	Mark coloringMark[4][2] ;
	Mark texturingMark[4][2] ;

	static String title[2] ;
	static String detailTitle[2] ;
	static String HTMLname[2] ;

	enum
	{
		NM_WEIGHT_FOG = 4,
		NM_WEIGHT_COLOR = 2,
	} ;

	double weight_sum_fog ;
	double weight_sum_color ;
	static double weight_fog[NM_WEIGHT_FOG] ;
	static double weight_color[NM_WEIGHT_COLOR] ;

	MaxTriMark()
	{
		maxTriMark = 0.0 ;
		averageKTPS = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		coloringKTPS = 0.0 ;
		coloringFPS = 0.0 ;
		texturingKTPS = 0.0 ;
		texturingFPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_fog = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_FOG ; i ++)
			weight_sum_fog += weight_fog[i] ;

		weight_sum_color = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_COLOR ; i ++)
			weight_sum_color += weight_color[i] ;
	}

	// Maximum Triangles MArk セットアップ
	void Set(const MarkTable& markTable)
	{
		for (int i = 0 ; i < 4 ; i ++)
		{
			double no = 6.01 + i * 0.01 ;	// 6.01 〜 6.04

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						coloringMark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						coloringMark[i][0] = markTable.mark[l] ;
				}
			}

			no = 6.11 + i * 0.01 ;	// 6.11 〜 6.14

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (Abs(no - markTable.mark[l].no) <= NO_THRETHOLD)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						texturingMark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						texturingMark[i][0] = markTable.mark[l] ;
				}
			}
		}

		// セットしたデータから、FPS 平均値を計算
		averageKTPS = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		coloringFPS = 0.0 ;
		coloringKTPS = 0.0 ;
		texturingFPS = 0.0 ;
		texturingKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		for (int i = 0 ; i < 4 ; i ++)
		{
			// 平均 FPS および kTPS は Single, Double のうち速い方
			double fastColorFPS = Max(coloringMark[i][0].fps, coloringMark[i][1].fps) ;
			double fastColorKTPS = Max(coloringMark[i][0].kTPS, coloringMark[i][1].kTPS) ;
			coloringFPS += fastColorFPS * weight_fog[i] ;
			coloringKTPS += fastColorKTPS * weight_fog[i] ;

			double fastTextureFPS = Max(texturingMark[i][0].fps, texturingMark[i][1].fps) ;
			double fastTextureKTPS = Max(texturingMark[i][0].kTPS, texturingMark[i][1].kTPS) ;
			texturingFPS += fastTextureFPS * weight_fog[i] ;
			texturingKTPS += fastTextureKTPS * weight_fog[i] ;

			highestFPS = Max(highestFPS, Max(fastColorFPS, fastTextureFPS)) ;
			highestKTPS = Max(highestKTPS, Max(fastColorKTPS, fastTextureKTPS)) ;

			dBufferFPS += (coloringMark[i][0].fps * weight_color[0] +
						   texturingMark[i][0].fps * weight_color[1]) * weight_fog[i] ;
			sBufferFPS += (coloringMark[i][1].fps * weight_color[0] +
						   texturingMark[i][1].fps * weight_color[1]) * weight_fog[i] ;
		}

		coloringFPS  /= weight_sum_fog ;
		coloringKTPS /= weight_sum_fog ;
		texturingFPS  /= weight_sum_fog ;
		texturingKTPS /= weight_sum_fog ;
		averageKTPS = (coloringKTPS * weight_color[0] + texturingKTPS * weight_color[1]) / weight_sum_color ;
		averageFPS = (coloringFPS * weight_color[0] + texturingFPS * weight_color[1]) / weight_sum_color ;
		dBufferFPS /= weight_sum_fog * weight_sum_color ;
		sBufferFPS /= weight_sum_fog * weight_sum_color ;
	}

	// Maximum Triangles Mark 計算
	void CalculateMark(MaxTriMark& reference)
	{
		// 各テスト毎にパーセンテージを計算し、その平均値を Maximum Triangles Mark とする
		coloringPercentage = 0.0 ;
		texturingPercentage = 0.0 ;

		highestMark = 0.0 ;

		for (int i = 0 ; i < 4 ; i ++)
		{
			double refColoringFPS  = Max(reference.coloringMark[i][0].fps, reference.coloringMark[i][1].fps) ;
			double refTexturingFPS = Max(reference.texturingMark[i][0].fps, reference.texturingMark[i][1].fps) ;

			if (refColoringFPS > 0.0)
				detailColoringPercentage[i] = Max(coloringMark[i][0].fps, coloringMark[i][1].fps) / refColoringFPS ;
			else
				detailColoringPercentage[i] = 0.0 ;

			if (refTexturingFPS > 0.0)
				detailTexturingPercentage[i] = Max(texturingMark[i][0].fps, texturingMark[i][1].fps) / refTexturingFPS ;
			else
				detailTexturingPercentage[i] = 0.0 ;

			coloringPercentage += detailColoringPercentage[i] ;
			texturingPercentage += detailTexturingPercentage[i] ;

			highestMark = Max(highestMark, Max(detailColoringPercentage[i], detailTexturingPercentage[i])) ;
		}

		coloringPercentage /= 4 ;
		texturingPercentage /= 4 ;

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		maxTriMark = (coloringPercentage + texturingPercentage) / 2 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		coloringPercentage = 0.0 ;
		texturingPercentage = 0.0 ;

		if (reference.coloringKTPS > 0.0)
			coloringPercentage = coloringKTPS / reference.coloringKTPS ;

		if (reference.texturingKTPS > 0.0)
			texturingPercentage = texturingKTPS / reference.texturingKTPS ;

		if (0.0 < reference.averageKTPS)
			maxTriMark = averageKTPS / reference.averageKTPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		coloringPercentage = 1.0 ;
		texturingPercentage = 1.0 ;

		maxTriMark = 1.0 ;
		highestMark = 1.0 ;
	}
} ;

class PracticalMark
{
public:
	double practicalMark ;
	double percentage[2] ;

	double averageFPS ;
	double highestMark ;
	double highestFPS ;
	double highestKTPS ;
	double dBufferFPS ;
	double sBufferFPS ;
	Mark mark[2][2] ;

	enum
	{
		NM_WEIGHT_COMPLEX = 2,
	} ;

	double weight_sum_complex ;
	static double weight_complex[NM_WEIGHT_COMPLEX] ;

	PracticalMark()
	{
		practicalMark = 0.0 ;
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_complex = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_COMPLEX ; i ++)
			weight_sum_complex += weight_complex[i] ;
	}

	// OpenGL Practical Rendering Mark セットアップ
	void Set(const MarkTable& markTable)
	{
		for (int i = 0 ; i < 2 ; i ++)
		{
			double no = 7.01 + i * 0.01 ;	// 7.01 〜 7.02

			// 項目No. に一致するデータを検索
			for (int l = 0 ; l < markTable.nMark ; l ++)
			{
				// 該当するデータが存在する場合
				if (no == markTable.mark[l].no)
				{
					// Double buffer か Single Buffer どちらか？
					if (markTable.mark[l].extension.Search("Single"))	// Single Buffer
						mark[i][1] = markTable.mark[l] ;
					else	// Double Buffer or ???
						mark[i][0] = markTable.mark[l] ;
				}
			}
		}

		// セットしたデータから、FPS 平均値を計算
		averageFPS = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		for (int i = 0 ; i < 2 ; i ++)
		{
			// 平均 FPS は Single, Double のうち速い方
			double fast = Max(mark[i][0].fps, mark[i][1].fps) ;
			averageFPS += fast * weight_complex[i] ;

			highestFPS = Max(highestFPS, fast) ;
			highestKTPS = Max(highestKTPS, Max(mark[i][0].kTPS, mark[i][1].kTPS)) ;

			dBufferFPS += mark[i][0].fps * weight_complex[i] ;
			sBufferFPS += mark[i][1].fps * weight_complex[i] ;
		}

		averageFPS /= weight_sum_complex ;
		dBufferFPS /= weight_sum_complex ;
		sBufferFPS /= weight_sum_complex ;
	}

	// OpenGL Practical Mark 計算
	void CalculateMark(PracticalMark& reference)
	{
		practicalMark = 0.0 ;
		highestMark = 0.0 ;

		for (int i = 0 ; i < 2 ; i ++)
		{
			double refFPS ;
			refFPS = Max(reference.mark[i][0].fps, reference.mark[i][1].fps) ;
			if (refFPS > 0.0)
				percentage[i] = Max(mark[i][0].fps, mark[i][1].fps) / refFPS ;
			else
				percentage[i] = 0.0 ;

			practicalMark += percentage[i] ;
			highestMark = Max(highestMark, percentage[i]) ;
		}

#ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		practicalMark /= 2 ;

#else	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS
		if (reference.averageFPS > 0.0)
			practicalMark = averageFPS / reference.averageFPS ;

#endif	// #ifndef GLCLOCK_MARK_BASE_FPS_KTPS ... #else
	}

	// 参照用にすべて１００％にセット
	void SetReferenceMarkTo100()
	{
		for (int i = 0 ; i < 2 ; i ++)
			percentage[i] = 1.0 ;

		practicalMark = 1.0 ;
		highestMark = 0.0 ;
	}
} ;


// 具体的な結果データ
class OverAllMark
{
public:
	double			glclockMark ;

	double			dBufferFPS ;
	double			sBufferFPS ;

	double			highestSectionMark ;
	double			highestMark ;
	double			highestFPS ;
	double			highestKTPS ;

	String			originalLog ;
	String			infoOpenGL ;
	String			infoGlClock ;
	String			infoColorBuffer ;
	String			infoTimerResolution ;
//	String			infoViewport ;
//	String			infoNBitsRGB ;
	String			infoOpenGLVender ;
	String			infoOpenGLRenderer ;
	String			infoOpenGLVersion ;
	MarkTable		markTable ;		// 元データ

	LightingMark	lightingMark ;
	LightingTexMark	lightingTexMark ;
//	FoggingMark		foggingMark ;
	FilteringMark	filteringMark ;
	EnvMapMark		envMapMark ;
	SuperSampleMark	superSampleMark ;
	MaxTriMark		maxTriMark ;
	PracticalMark	practicalMark ;

	enum
	{
		NM_WEIGHT_MARK = 7,
	} ;

	double weight_sum_mark ;
	static double weight_mark[NM_WEIGHT_MARK] ;

	OverAllMark()
	{
		glclockMark = 0.0 ;
		highestFPS = 0.0 ;
		highestKTPS = 0.0 ;
		dBufferFPS = 0.0 ;
		sBufferFPS = 0.0 ;

		weight_sum_mark = 0.0 ;
		for (int i = 0 ; i < NM_WEIGHT_MARK ; i ++)
			weight_sum_mark += weight_mark[i] ;
	}

private:
	void SetupFromMarkTable()
	{
		lightingMark.Set(markTable) ;
		lightingTexMark.Set(markTable) ;
//		foggingMark.Set(markTable) ;
		filteringMark.Set(markTable) ;
		envMapMark.Set(markTable) ;
		superSampleMark.Set(markTable) ;
		maxTriMark.Set(markTable) ;
		practicalMark.Set(markTable) ;
	}

	void CalculateFPS()
	{
		dBufferFPS =
			(lightingMark.dBufferFPS	* weight_mark[0] +
			 lightingTexMark.dBufferFPS	* weight_mark[1] +
			 filteringMark.dBufferFPS	* weight_mark[2] +
			 envMapMark.dBufferFPS		* weight_mark[3] +
			 superSampleMark.dBufferFPS	* weight_mark[4] +
			 maxTriMark.dBufferFPS		* weight_mark[5] +
			 practicalMark.dBufferFPS	* weight_mark[6]) / weight_sum_mark ;

		sBufferFPS =
			(lightingMark.sBufferFPS	* weight_mark[0] +
			 lightingTexMark.sBufferFPS	* weight_mark[1] +
			 filteringMark.sBufferFPS	* weight_mark[2] +
			 envMapMark.sBufferFPS		* weight_mark[3] +
			 superSampleMark.sBufferFPS	* weight_mark[4] +
			 maxTriMark.sBufferFPS		* weight_mark[5] +
			 practicalMark.sBufferFPS	* weight_mark[6]) / weight_sum_mark ;

		// 最高スコア計算
		highestSectionMark = 0.0 ;
		highestSectionMark = Max(lightingMark.lightingMark, lightingTexMark.lightingTexMark) ;
		highestSectionMark = Max(highestSectionMark, filteringMark.filteringMark) ;
		highestSectionMark = Max(highestSectionMark, envMapMark.envMapMark) ;
		highestSectionMark = Max(highestSectionMark, superSampleMark.superSampleMark) ;
		highestSectionMark = Max(highestSectionMark, maxTriMark.maxTriMark) ;
		highestSectionMark = Max(highestSectionMark, practicalMark.practicalMark) ;

		highestMark = 0.0 ;
		highestMark = Max(lightingMark.highestMark, lightingTexMark.highestMark) ;
		highestMark = Max(highestMark, filteringMark.highestMark) ;
		highestMark = Max(highestMark, envMapMark.highestMark) ;
		highestMark = Max(highestMark, superSampleMark.highestMark) ;
		highestMark = Max(highestMark, maxTriMark.highestMark) ;
		highestMark = Max(highestMark, practicalMark.highestMark) ;

		highestFPS = 0.0 ;
		highestFPS = Max(lightingMark.highestFPS, lightingTexMark.highestFPS) ;
		highestFPS = Max(highestFPS, filteringMark.highestFPS) ;
		highestFPS = Max(highestFPS, envMapMark.highestFPS) ;
		highestFPS = Max(highestFPS, superSampleMark.highestFPS) ;
		highestFPS = Max(highestFPS, maxTriMark.highestFPS) ;
		highestFPS = Max(highestFPS, practicalMark.highestFPS) ;

		highestKTPS = 0.0 ;
		highestKTPS = Max(lightingMark.highestKTPS, lightingTexMark.highestKTPS) ;
		highestKTPS = Max(highestKTPS, filteringMark.highestKTPS) ;
		highestKTPS = Max(highestKTPS, envMapMark.highestKTPS) ;
		highestKTPS = Max(highestKTPS, superSampleMark.highestKTPS) ;
		highestKTPS = Max(highestKTPS, maxTriMark.highestKTPS) ;
		highestKTPS = Max(highestKTPS, practicalMark.highestKTPS) ;
	}

public:
	void CalculateMark(OverAllMark& reference)
	{
		lightingMark.CalculateMark(reference.lightingMark) ;
		lightingTexMark.CalculateMark(reference.lightingTexMark) ;
		filteringMark.CalculateMark(reference.filteringMark) ;
		envMapMark.CalculateMark(reference.envMapMark) ;
		superSampleMark.CalculateMark(reference.superSampleMark) ;
		maxTriMark.CalculateMark(reference.maxTriMark) ;
		practicalMark.CalculateMark(reference.practicalMark) ;

		glclockMark =
			(lightingMark.lightingMark			* weight_mark[0] +
			 lightingTexMark.lightingTexMark	* weight_mark[1] +
			 filteringMark.filteringMark		* weight_mark[2] +
			 envMapMark.envMapMark				* weight_mark[3] +
			 superSampleMark.superSampleMark	* weight_mark[4] +
			 maxTriMark.maxTriMark				* weight_mark[5] +
			 practicalMark.practicalMark		* weight_mark[6]) / weight_sum_mark ;

		CalculateFPS() ;
	}

	void CalculateReferenceMark()
	{
		lightingMark.SetReferenceMarkTo100() ;
		lightingTexMark.SetReferenceMarkTo100() ;
		filteringMark.SetReferenceMarkTo100() ;
		envMapMark.SetReferenceMarkTo100() ;
		superSampleMark.SetReferenceMarkTo100() ;
		maxTriMark.SetReferenceMarkTo100() ;
		practicalMark.SetReferenceMarkTo100() ;

		glclockMark = 1.0 ;

		CalculateFPS() ;
	}

	void SetOpenGLInformations() ;

	// ログファイルから全項目のテスト結果をバッファにロード
	// さらに各々の詳細データを分離してテーブルにセット
	//
	// 成功時：テーブル要素数
	// エラー：-1
	int SetTableFromLogFile(const String& logFile) ;

	// ログファイルから全項目のテスト結果をバッファにロード
	// さらに各々の詳細データを分離してテーブルにセット
	// 平均 FPS 等をセット
	//
	// 成功時：テーブル要素数
	// エラー：-1
	int SetupMark(const String& logFile) ;

	// データテーブルからタイマー精度を取得
	// データなしなら -1
	int GetTimerResolution()
	{
		if (infoTimerResolution == "")
			return -1 ;

		return atoi(infoTimerResolution) ;
	}
} ;


// glclock mark
class GlClockMark
{
public:
	OverAllMark	test ;
	OverAllMark	reference ;
	double		barLength ;

	void CalculateMark() ;
	int SetupMark(const String& testLog, const String& referenceLog) ;
} ;


#endif	// #ifndef _GLCLOCK_MARK_H_
