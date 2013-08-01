
#include "glclockmark.H"

#ifdef WIN32
#include <crtdbg.h>
#endif

// 出力ストリーム
static FILE *outstream = NULL ;

static String title ;
static String tableEx ;
static String specTestEnv ;
static String specRefEnv ;

static int onlyFormat ;
static int addMode ;
static int donotOptimizeLog ;

#ifdef WIN32
static int donotShowResults ;
#endif

static String debugCGI ;

static String fileName ;
static String file_LOG ;
static String file_TXT ;
static String file_HTML ;
static String file_REF_LOG ;

static String outputLog ;
static String windowMode ;

static String title_VariousLighting		= "Various Lighting" ;
static String ditto_VariousLighting		= "ditto           " ;

static String title_SingleColor			= "(Diffuse + Specular) * Texture (Single Color Blending)" ;
static String ditto_SingleColor			= "ditto                                                 " ;
static String title_SeparateSpeculer	= "Diffuse * Texture + Specular (Separate Specular Color Blending)" ;
static String ditto_SeparateSpeculer	= "ditto                                                          " ;

//static String title_FogEquation			= "Various Fogging Equation" ;
//static String ditto_FogEquation			= "ditto                   " ;

static String title_VariousFiltering	= "Various Texture Filtering" ;
static String ditto_VariousFiltering	= "ditto                    " ;

static String title_SphereMapping		= "Sphere Mapping" ;
static String ditto_SphereMapping		= "ditto         " ;
static String title_TransSphereMapping	= "Double Sided Transparency Sphere Mapping without Z-Buffer" ;
static String ditto_TransSphereMapping	= "ditto                                                    " ;

static String title_4AddMotionBlur		= "4 Sampling Additive-Blended Object Motion-Blur without Z-Buffer (total 4-pass)" ;
static String ditto_4AddMotionBlur		= "ditto                                                                         " ;
static String title_4AlphaAntiAlias		= "4 Sampling Alpha-Blended Scene Anti-Aliasing with Z-Buffer Mask (total 7-pass)" ;
static String ditto_4AlphaAntiAlias		= "ditto                                                                         " ;
static String title_7AlphaDepthOfField	= "7 Sampling Alpha-Blended Depth of Field Blur with Z-Buffer Mask (total 13-pass)" ;
static String ditto_7AlphaDepthOfField	= "ditto                                                                          " ;

static String title_MaxTriColoring		= "Simple Coloring" ;
static String ditto_MaxTriColoring		= "ditto          " ;
static String title_MaxTriTexturing		= "Modulate Texture Mapping" ;
static String ditto_MaxTriTexturing		= "ditto                   " ;
//static String title_MaxTriLightTexture	= "5 Directional Lighted Texture Mapping" ;
//static String ditto_MaxTriLightTexture	= "ditto                                " ;
//static String title_MaxTriLightTexFog	= "5 Directional Lighted Texture with Fogging Exponent 2" ;
//static String ditto_MaxTriLightTexFog	= "ditto                                                " ;

static String title_OpenGLPractical		= "5 Directional Lights, Multi-Texture (Solid Texture, Specular Map), Blending, Linear Fog" ;
static String ditto_OpenGLPractical		= "ditto                                                                                  " ;
static String title_OpenGLPracticalHigh = "5 Point Lights with local Viewer, Multi-Texture (Solid Texture, Specular Map), Blending, Reflection, 1 Exponential Fog, High Detail" ;
static String ditto_OpenGLPracticalHigh	= "ditto                                                                                                                              " ;

String infoOpenGLRenderer ;

// glclock Mark 総合データ
OverAllMark testedMark ;
OverAllMark referenceMark ;

GlClockMark glclockMark ;

// ハイスコアフラグ（ハイスコアは最初に一致した１つのみ有効）
int flag_highestMark ;
int flag_highestFPS ;
int flag_highestKTPS ;

int flag_singleBuffering ;
int flag_doubleBuffering ;

int flag_eachMark[7] ;

// Number of loop
int loop ;

int barLength ;

// 共通オプション
static String options ;
static String firstOptions ;
static String vgaoption ;

static String exeCommonOptions = " -NO_GL_ERR" ;
//static String allCommonOptions = " -RGBA -NOACCUM" ;

#ifdef macintosh
static String allCommonOptions = " -xp 20 -yp 50 -NOM -NOACCUM"; 
#else
static String allCommonOptions = " -xp 0 -yp 0 -NOM -NOACCUM" ;
#endif

// 共通に追加する任意のオプション
static String optionLowQuality		= " -q 5 -d 32" ;
static String optionMidiumQuality	= " -q 5 -d 128" ;
static String optionHighQuality		= " -q 8 -d 256 -g 3" ;
static String optionMaximumQuality	= " -MAX -d 2048 -me 1 -bit 0 -hed 1 -go -1 -g 3" ;
static String optionFogNone			= " -fog 0" ;
static String optionFogLinear		= " -fog 1" ;
static String optionFogExponent		= " -fog 2" ;
static String optionFogExponent2	= " -fog 3" ;

// 各テスト専用オプション
static String eachOptions[N_MARK_EXECUTE] =
{
	"-mark 1.0 -title \"" + title_VariousLighting + ".\"" + optionMidiumQuality + optionFogNone,

	"-mark 2.0 -title \"" + title_SingleColor      + ".\"" + optionMidiumQuality + optionFogNone,
	"-mark 2.1 -title \"" + title_SeparateSpeculer + ".\"" + optionMidiumQuality + optionFogNone,

	"-mark 3.0 -title \"" + title_VariousFiltering + ".\"" + optionHighQuality + optionFogNone,

	"-B -pre \"4.01 " + title_SphereMapping      + ".\" -title \"" + title_SphereMapping      + ".\"" + optionMidiumQuality + optionFogNone + " -e 1",
	"-B -pre \"4.02 " + title_TransSphereMapping + ".\" -title \"" + title_TransSphereMapping + ".\"" + optionMidiumQuality + optionFogNone + " -C",

	"-B -pre \"5.01 " + title_4AddMotionBlur     + ".\" -title \"" + title_4AddMotionBlur     + ".\"" + optionLowQuality + optionFogNone + " -C -m 4",
	"-B -pre \"5.02 " + title_4AlphaAntiAlias    + ".\" -title \"" + title_4AlphaAntiAlias    + ".\"" + optionLowQuality + optionFogNone + " -e 1 -a 4",
	"-B -pre \"5.03 " + title_7AlphaDepthOfField + ".\" -title \"" + title_7AlphaDepthOfField + ".\"" + optionLowQuality + optionFogNone + " -e 1 -f 7 -FOCUS",

	"-mark 6.0 -title \"" + title_MaxTriColoring  + ".\"" + optionMaximumQuality + " -l 0",
	"-mark 6.1 -title \"" + title_MaxTriTexturing + ".\"" + optionMaximumQuality + " -s 1",

	"-B -pre \"7.01 " + title_OpenGLPractical     + ".\" -title \"" + title_OpenGLPractical     + ".\"" + optionMidiumQuality + optionFogLinear   + " -r 2",
	"-B -pre \"7.02 " + title_OpenGLPracticalHigh + ".\" -title \"" + title_OpenGLPracticalHigh + ".\"" + optionHighQuality   + optionFogExponent + " -r 2 -R -lt 1 -LO -sf marble -sf wood",
} ;


// 各テストのマークＮｏ
int markNo[N_MARK_EXECUTE] =
{
	1, 2,2, 3, 4,4, 5,5,5, 6,6, 7,7,
} ;

// 各マーク毎の標準出力メッセージ
static String chapter[N_MARK_EXECUTE * 2] =
{
	"1. - Per-Vertex Lighting Mark -\n\n",
	"",

	"2. - Per-Vertex Lighted Texturing Mark -\n\n",
	"",
	"",
	"",

	"3. - Texture Filtering Mark -\n\n",
	"",

	"4. - Environment Mapping Mark (Per-Texel Specular Lighting) -\n\n",
//	"4. - Environment Mapping Mark (Dynamical Texture Coordinates Generation) -\n\n",
	"",
	"",
	"",

	"5. - Super Sampled Blending Mark (without Accum-Buffer) -\n\n",
	"",
	"",
	"",
	"",
	"",

	"6. - Maximum Triangles Mark -\n\n",
	"",
	"",
	"",

	"7. - OpenGL Practical Rendering Mark -\n\n",
	"",
	"",
	"",
} ;

static String chapterName[8] =
{
	"1. Per-Vertex Lighting Mark.",
	"2. Per-Vertex Lighted Texturing Mark.",
	"3. Texture Filtering Mark.",
	"4. Environment Mapping Mark.",
	"5. Super Sampled Blending Mark.",
	"6. Maximum Triangles Mark.",
	"7. OpenGL Practical Rendering Mark.",

	"Other Comparison.",
} ;

static String nAllMark(N_ALL_MARK) ;

// 各テスト毎の標準出力メッセージ
static String message[N_MARK_EXECUTE * 2] =
{
	"  1 / " + nAllMark + " - " + title_VariousLighting,
	"  2 / " + nAllMark + " - " + ditto_VariousLighting,

	"  3 / " + nAllMark + " - " + title_SingleColor,
	"  4 / " + nAllMark + " - " + ditto_SingleColor,
	"  5 / " + nAllMark + " - " + title_SeparateSpeculer,
	"  6 / " + nAllMark + " - " + ditto_SeparateSpeculer,

	"  7 / " + nAllMark + " - " + title_VariousFiltering,
	"  8 / " + nAllMark + " - " + ditto_VariousFiltering,

	"  9 / " + nAllMark + " - " + title_SphereMapping,
	" 10 / " + nAllMark + " - " + ditto_SphereMapping,
	" 11 / " + nAllMark + " - " + title_TransSphereMapping,
	" 12 / " + nAllMark + " - " + ditto_TransSphereMapping,

	" 13 / " + nAllMark + " - " + title_4AddMotionBlur,
	" 14 / " + nAllMark + " - " + ditto_4AddMotionBlur,
	" 15 / " + nAllMark + " - " + title_4AlphaAntiAlias,
	" 16 / " + nAllMark + " - " + ditto_4AlphaAntiAlias,
	" 17 / " + nAllMark + " - " + title_7AlphaDepthOfField,
	" 18 / " + nAllMark + " - " + ditto_7AlphaDepthOfField,

	" 19 / " + nAllMark + " - " + title_MaxTriColoring,
	" 20 / " + nAllMark + " - " + ditto_MaxTriColoring,
	" 21 / " + nAllMark + " - " + title_MaxTriTexturing,
	" 22 / " + nAllMark + " - " + ditto_MaxTriTexturing,

	" 23 / " + nAllMark + " - " + title_OpenGLPractical,
	" 24 / " + nAllMark + " - " + ditto_OpenGLPractical,
	" 25 / " + nAllMark + " - " + title_OpenGLPracticalHigh,
	" 26 / " + nAllMark + " - " + ditto_OpenGLPracticalHigh,
} ;


// ログ用ストリームに書式付出力（バッファフラッシュ付）
inline int logprintf(const char* format, ...)
{
	int r ;
	char buf[4096] ;

	va_list args ;
	va_start(args, format) ;
	r = vsprintf(buf, format, args) ;
	va_end(args) ;

	// そのままストリームへ出力
	fprintf(outstream, buf) ;
	fflush(outstream) ;

	return r ;
}


String MakeWindowSizeOption(Option& option)
{
	int width, height ;

	if (option.Get("-VGA", 2))
	{
		windowMode = "VGA" ;

		width  = 640 ;
		height = 480 ;
	}
	else if (option.Get("-SVGA", 3))
	{
		windowMode = "SVGA" ;

		width  = 800 ;
		height = 600 ;
	}
	else if (option.Get("-XGA", 2))
	{
		windowMode = "XGA" ;

		width  = 1024 ;
		height =  768 ;
	}
	else
	{
		// いずれの指定も無かった場合
		width  = option.GetInt("-width",  WIDTH,  2) ;
		height = option.GetInt("-height", HEIGHT, 2) ;
	}

	String opt ;
	opt = "-w " + IntToString(width) ;
	opt += " -h " + IntToString(height) ;

	opt += allCommonOptions ;

	return opt ;
}


// 各マーク単位 ON/OFF
void SetEachMarkTestFlag(Option& option)
{
	for (int i = 0 ; i < 7 ; i ++ )
		flag_eachMark[i] = -1 ;

	int plusFlag = FALSE ;

	for (int i = 0 ; i < 7 ; i ++)
	{
		char opt = (char)i + '1' ;

		if (option.Get(String('+') + opt, 2))
		{
			plusFlag = TRUE ;
			flag_eachMark[i] = 1 ;
		}
		if (option.Get(String('-') + opt, 2))
			flag_eachMark[i] = 0 ;
	}

	for (int i = 0 ; i < 7 ; i ++)
	{
		if (flag_eachMark[i] == -1)
		{
			if (plusFlag)
				flag_eachMark[i] = 0 ;
			else
				flag_eachMark[i] = 1 ;
		}
	}
}

void Initialize(Option& option)
{
	// Windows では、カレントディレクトリを実行ファイルのあるディレクトリに移動
#ifdef WIN32
	{
		String work ;
		work = option.ArgV()[0] ;
		work = work.FileDirectory() ;
		SetCurrentDirectory(work) ;
	}
#endif

#ifndef CGI_MODE

	// 第一引数に "*.log" が指定されている場合は
	// 無条件 -NOTEST を指定し、
	// HTML の生成のみを実行
	// モード指定オプション等は無視
	if (option.ArgC() >= 2)
	{
		String argv1 = option.ArgV()[1] ;
		if (Right(argv1, 4) == ".log" || Right(argv1, 4) == ".LOG")
		{
#ifdef WIN32
			WIN32_FIND_DATA file ;
			HANDLE hFindFile ;
			hFindFile = FindFirstFile(argv1, &file) ;
			if (hFindFile != INVALID_HANDLE_VALUE)
			{
				argv1 = argv1.FileDirectory() ;
				if (argv1 != "")
					argv1 += String("\\") + file.cFileName ;
				else
					argv1 = file.cFileName ;

				FindClose(hFindFile) ;
			}
#endif
			fileName = Left(argv1, argv1.Length() - 4) ;	// ".log" をカット
			title = "glclock Mark Results" ;	// 暫定
			onlyFormat = TRUE ;	// HTML 生成のみ

//			MessageBox(NULL, String(option.ArgV()[0]) + " " + fileName, _T("glclockmark.exe path"), MB_OK | MB_ICONSTOP) ;
		}
	}

	// 第二引数に "*.log" が指定されている場合は
	// -ref とみなす
	if (option.ArgC() >= 3)
	{
		String argv2 = option.ArgV()[2] ;
		if (Right(argv2, 4) == ".log" || Right(argv2, 4) == ".LOG")
		{
#ifdef WIN32
			WIN32_FIND_DATA file ;
			HANDLE hFindFile ;
			hFindFile = FindFirstFile(argv2, &file) ;
			if (hFindFile != INVALID_HANDLE_VALUE)
			{
				argv2 = argv2.FileDirectory() ;
				if (argv2 != "")
					argv2 += String("\\") + file.cFileName ;
				else
					argv2 = file.cFileName ;

				FindClose(hFindFile) ;
			}
#endif
			file_REF_LOG = argv2 ;
		}
	}
	else
		file_REF_LOG = "" ;

	barLength = option.GetInt("-barlength", BASE_BAR_LENGTH, 4) ;

#else	// #ifndef CGI_MODE

	// CGI mode
	onlyFormat = TRUE ;

	String logfiles ;
	char *buf = getenv("QUERY_STRING") ;
	if (buf && strlen(buf))
	{
		debugCGI += "Get mode.\n" ;
		debugCGI += String("QUERY_STRING: \"") + buf + "\"\n" ;
		logfiles = buf ;
	}
	else
	{
		debugCGI += "Post mode.\n" ;
		char *env = getenv("CONTENT_LENGTH") ;
		debugCGI += String("CONTENT_LENGTH: \"") + env + "\"\n" ;

		int len = atoi(env) ;
		debugCGI += "len: " + IntToString(len) + "\n" ;

		buf = new char[len] ;
		fread(buf,len, 1, stdin) ;
		logfiles = buf ;
		delete [] buf ;
	}

	if (logfiles != "")
	{
		debugCGI += "logFiles: \"" + logfiles + "\"\n" ;
		int len ;

		// Log
		int start = logfiles.Search("log=") ;
		if (start)
		{
			start += 4 ;
			len = logfiles.Search("&") ;
			if (len > start)
				len = len - start ;
			else
				len = logfiles.Length() - start + 1 ;

			fileName = Mid(logfiles, start, len) ;

			len = fileName.Search(".log") ;
			if (!len)
				len = fileName.Search(".LOG") ;

			if (len)
				fileName.Length(len + 3) ;

			fileName = Left(fileName, fileName.Length() - 4) ;	// ".log" をカット
			fileName.ReplaceString("%40", '@') ;
		}
		else
			fileName = "" ;

		// Ref
		start = logfiles.Search("ref=") ;
		if (start)
		{
			start += 4 ;
			len = logfiles.Search("&") ;
			if (len > start)
				len = len - start ;
			else
				len = logfiles.Length() - start + 1 ;

			file_REF_LOG = Mid(logfiles, start, len) ;

			len = file_REF_LOG.Search(".log") ;
			if (!len)
				len = file_REF_LOG.Search(".LOG") ;

			if (len)
				file_REF_LOG.Length(len + 3) ;
			file_REF_LOG.ReplaceString("%40", '@') ;
		}
		else
			file_REF_LOG = FILE_REF_LOG ;


		// Bar length
		start = logfiles.Search("bar=") ;
		if (start)
		{
			start += 4 ;
			len = logfiles.Search("&") ;
			if (len > start)
				len = len - start ;
			else
				len = logfiles.Length() - start + 1 ;

			barLength = atoi(Mid(logfiles, start, len)) ;

			if (barLength < 10 || barLength > 1000)
				barLength = BASE_BAR_LENGTH ;
		}
		else
			barLength = BASE_BAR_LENGTH ;
	}
	else
	{
		fileName = "" ;
		file_REF_LOG = FILE_REF_LOG ;

		barLength = BASE_BAR_LENGTH ;
	}

	debugCGI += "log: \"" + fileName + "\"\n" ;
	debugCGI += "ref: \"" + file_REF_LOG + "\"\n" ;
	debugCGI += "barLlength: " + IntToString(barLength) + "\n" ;

#endif	// #ifndef CGI_MODE ... #else


	barLength = (int)((double)barLength * SCALE_BAR_LENGTH) ;

	if (!onlyFormat)
	{
		vgaoption = MakeWindowSizeOption(option) ;
		options = vgaoption ;

		// モードによって出力ファイル、ログファイル等を変化
		if (windowMode == "")
		{
			fileName = FILE_NAME ;
			title = "glclock Mark Results" ;
		}
		else
		{
			fileName = String(FILE_NAME) + "_" + windowMode ;
			title = String("glclock ") + windowMode + " Mark Results" ;
		}

		// テスト無効（結果フォーマットのみ）オプション
		onlyFormat = option.Get("-NOTEST", 4) ;

		// テスト内容制限オプション
		flag_singleBuffering = option.Get("-SINGLE", 7) ;
		flag_doubleBuffering = option.Get("-DOUBLE", 7) ;

		// どちらも指定なし or どちらも指定されている場合はすべて ON
		if (flag_singleBuffering == flag_doubleBuffering)
		{
			flag_doubleBuffering = TRUE ;
			flag_singleBuffering = TRUE ;
		}
		else if (flag_singleBuffering) flag_doubleBuffering = FALSE ;
		else if (flag_doubleBuffering) flag_singleBuffering = FALSE ;

		// 各マーク単位 ON/OFF
		SetEachMarkTestFlag(option) ;

		// Loop Option
		loop = option.GetInt("-loop", 1, 5) ;
		if (loop <= 0)
			loop = 1 ;
		else if (loop > 8)
			loop = 8 ;

		addMode = option.Get("-ADDMODE", 4) ;
	}

	tableEx = String(HTML_TABLE_BORDER) + HTML_TABLE_EX ;
	specTestEnv = SPEC_TESTED_ENV ;
	specRefEnv = SPEC_REF_ENV ;

	file_LOG  = fileName + SUFFIX_LOG ;
	file_TXT  = fileName + SUFFIX_TXT ;
	file_HTML = fileName + SUFFIX_HTML ;

	if (addMode)
	{
		FILE *fp = fopen(file_LOG, "r") ;
		if (fp)
			fclose(fp) ;
		else
			addMode = FALSE ;	// glclockmark.log file must be exist.
	}

	// ストリームセット
	outstream = stdout ;

	options += " -fo " + file_LOG ;
	options += exeCommonOptions ;

	// Windows 版のみ
	// 結果表示（ブラウザ起動）抑制
#ifdef WIN32
	donotShowResults = option.Get("-DONOTSHOWRESULT", 10) ;
#endif

	donotOptimizeLog = option.Get("-DONOTOPTIMIZELOG", 9) ;

#ifdef CGI_MODE
	donotOptimizeLog = TRUE ;
#endif


	// 参照環境ログファイル
	if (file_REF_LOG == "")
		file_REF_LOG = option.GetString("-reffile", FILE_REF_LOG, 4) ;

	// 解析不能な全オプションはそのまま glclock に渡す
	options += " " + option.GetUnusedString() ;
	firstOptions = " " + option.GetUnusedString() ;

	// スタートアップメッセージ
#ifndef CGI_MODE
	logprintf("glclock mark.\n") ;
#endif
}


#ifdef WIN32

// プロセスの生成
int SimpleCreateProcess(const String& appName, const String& options, const String& current, int mode)
{
	char *opt ;
	opt = new char[options.Length() + 1] ;
	strcpy(opt, options) ;

	STARTUPINFO startupInfo ;
	PROCESS_INFORMATION processInfo ;

	memset(&processInfo, 0, sizeof(processInfo)) ;
	memset(&startupInfo, 0, sizeof(startupInfo)) ;
	startupInfo.cb = sizeof(STARTUPINFO) ;

	int result ;
	result = CreateProcess(appName, appName + " " + opt,
						   NULL, NULL, FALSE,
						   CREATE_NEW_CONSOLE, NULL,
						   current,
						   &startupInfo, &processInfo) ;

	if (mode == _P_WAIT)
		WaitForSingleObject(processInfo.hThread, INFINITE) ;

	CloseHandle(processInfo.hThread) ;
	CloseHandle(processInfo.hProcess) ;

	delete [] opt ;
	return result ;
}

HINSTANCE SimpleShellExecuteOpen(const String& appName, const String& options = "", const String& current = "")
{
	HINSTANCE hInst ;
	hInst = ShellExecute(NULL, "open", appName, options, current, SW_SHOWNORMAL) ;

	return hInst ;
}

#endif	// #ifdef WIN32


int ExecuteAndWait(const String& appName, const String& options)
{
#ifdef WIN32
//	return _spawnlp(_P_WAIT, appName, appName, options, NULL) ;

	char work[_MAX_PATH + 1] ;
	GetCurrentDirectory(_MAX_PATH, work) ;
	return SimpleCreateProcess(String(work) + "\\" + appName, options, work, _P_WAIT) ;

//	String path = appName + " " + options ;
//	_flushall() ;
//	return system(appName) ;

#else	// #ifdef WIN32

#ifdef macintosh
	extern int OpenWithOptions(const String& options) ;
	return OpenWithOptions(options) ;

#else
	String path = "./" + appName + " " + options ;
	return system(path) ;
#endif

#endif	// #ifdef WIN32 ... #else
}


// ログファイルからタイマー精度を取得
// データなしなら -1
int GetTimerResolutionFromLogFile(const String& logFile)
{
	OverAllMark data ;
	int ret = data.SetTableFromLogFile(logFile) ;

	if (ret == -1)
		return ret ;

	::infoOpenGLRenderer = data.infoOpenGLRenderer ;

	return data.GetTimerResolution() ;
}


void ExecMarks()
{
	// OpenGL 情報ゲットし、ログファイルを新規作成
	if (!addMode)
	{
		ExecuteAndWait(GLCLOCK_EXECUTE, String("-B  -foutgl ") + file_LOG + " " + vgaoption + firstOptions) ;

#ifdef WIN32
		Sleep(1000) ;
#endif
	}

	String timerOption, dbgTimerOption ;
	int timerResolution = GetTimerResolutionFromLogFile(file_LOG) ;
	if (timerResolution > 0)
		timerOption = "-tres " + IntToString(timerResolution) ;

	if (infoOpenGLRenderer.Search("GeForce"))
		dbgTimerOption = "-tres 1000" ;
	else
		dbgTimerOption = timerOption ;

	int firstFlag = TRUE ;

	for (int l = 0 ; l < loop ; l ++)
	{
		if (loop >= 2)
			logprintf("\nTest Loop: %d\n", l + 1) ;

		for (int i = 0 ; i < N_MARK_EXECUTE * 2 ; i ++)
		{
			int testNo = i / 2 ;
			if (!flag_eachMark[markNo[testNo] - 1])
				continue ;

			String opt = eachOptions[testNo] ;
			int titleNo = i ;
			String buffer ;

			if ((i % 2) == 1)
			{
				if (!flag_singleBuffering)
					continue ;

				if (!flag_doubleBuffering)
					titleNo = i - 1 ;

				opt += " -S" ;
				buffer = " - Single Buffering" ;
			}

			if ((i % 2 ) == 0)
			{
				if (!flag_doubleBuffering)
					continue ;

				buffer = " - Double Buffering" ;
			}

			if (firstFlag)
			{
				opt += " -wfps 6000" ;
				firstFlag = FALSE ;
			}
			else
			{
				opt += " -wfps 3600" ;
			}

			opt += String(" ") + options ;

			if (i == 19 || i == 21)	// 6. Single Buffer for GeForce
				opt += " " + dbgTimerOption ;
			else
				opt += " " + timerOption ;

			if (chapter[titleNo] != "")
				logprintf("\n") ;

			logprintf(chapter[titleNo] + message[titleNo] + buffer + "\n") ;
			ExecuteAndWait(GLCLOCK_EXECUTE, opt) ;
		}
	}
}


int LoadAllLines(FILE *fp, MarkLogTable& logTable)
{
	// ログからすべての情報を行単位で String テーブルにロード
	logTable.Clear() ;	// 一度クリア

	char buf[MAX_LINE_BUF] ;

	// ファイルの最後まで読み出し、テーブルに追加
	while (fgets(buf, MAX_LINE_BUF, fp))
		logTable.AddLine(buf) ;

	int l = 0 ;
	int delFlag = FALSE ;
	MarkTable markTable ;

	while (l < logTable.nLine)
	{
		// Search first '#'
		while (l < logTable.nLine && logTable.line[l].Character(1) != '#') l ++ ;
		if (l >= logTable.nLine) break ;

		Mark work ;
		work.Set(logTable.line[l], logTable.line[l + 1], logTable.line[l + 2]) ;
		work.other = l ;	// Save line no

		int i ;
		for (i = 0 ; i < markTable.nMark ; i ++)
		{
			if (markTable.mark[i].title     == work.title &&
				markTable.mark[i].no        == work.no    &&
				markTable.mark[i].extension == work.extension)
				break ;
		}

		// not exist yet
		if (i >= markTable.nMark)
		{
			markTable.AddMark(work) ;
		}
		else
		{
			delFlag = TRUE ;
			int delLineNo = logTable.nLine ;

			// exist
			if (work.fps > markTable.mark[i].fps)
			{
				// Delete old line
				delLineNo = markTable.mark[i].other ;

				// New
				markTable.mark[i] = work ;
			}
			else
			{
				// Delete new line
				delLineNo = l ;
			}

			// Delete Lines
			logTable.line[delLineNo    ] = "" ;
			logTable.line[delLineNo + 1] = "" ;
			logTable.line[delLineNo + 2] = "" ;

			delLineNo += 3 ;
			while (delLineNo < logTable.nLine)
			{
//				if (Cut(logTable.line[delLineNo]) == "")
				if (logTable.line[delLineNo] == "\n")
				{
					logTable.line[delLineNo] = "" ;
					break ;
				}

				delLineNo ++ ;
			}
		}

		l += 3 ;
	}

	return delFlag ;
}


void WriteAllLines(FILE *fp, MarkLogTable& logTable)
{
	for (int l = 0 ; l < logTable.nLine ; l ++)
	{
		if (logTable.line[l] != "")
			fputs(logTable.line[l], fp) ;
	}
}


// 成功時：TRUE
// エラー：FALSE
int LoadLogFile(const String& file, MarkLogTable& logTable)
{
	FILE *fp ;

	fp = fopen(file, "r") ;
	if (!fp)
	{
		// オープンエラー
//#ifndef CGI_MODE
		logprintf("Failed to open log file " + file + ".\n") ;
#ifdef WIN32
		MessageBox(NULL, "Failed to open log file " + file + ".", _T("Load Log file error"), MB_OK | MB_ICONSTOP) ;
#endif

//#endif	// #ifdef CGI_MODE ... #else
		return FALSE ;
	}

	int delFlag = LoadAllLines(fp, logTable) ;
	fclose(fp) ;

	if (delFlag && !donotOptimizeLog)
	{
		fp = fopen(file, "w") ;
		if (!fp)
		{
			// オープンエラー
//#ifndef CGI_MODE
			logprintf("Failed to optimize log file " + file + ".\n") ;
#ifdef WIN32
			MessageBox(NULL, "Failed to open log file " + file + ".", _T("Optimize Log file error"), MB_OK | MB_ICONSTOP) ;
#endif

//#endif	// #ifdef CGI_MODE ... #else
			return FALSE ;
		}

		WriteAllLines(fp, logTable) ;
		fclose(fp) ;
	}

	return TRUE ;
}

String HTMLBar(const String& src, int width, int height, const String& link = "")
{
	if (width < 1)
		width = 1 ;
	if (height < 1)
		height = 1 ;

	String alt ;

	alt.Length(width / 8) ;
	if (alt.Length() < 1)
		alt.Length(1) ;
	else if (alt.Length() > 2048)
		alt.Length(2048) ;

	if (width > 16383)
		width = 16383 ;

	for (int i = 1 ; i <= alt.Length() ; i ++)
		alt.SetAt(i, '*') ;
	alt = " ALT=\"" + alt + "\"" ;

	char buf[4096] ;
	sprintf(buf, "<IMG SRC=\"%s\" BORDER=\"0\" WIDTH=\"%d\" HEIGHT=\"%d\"" + alt + ">", (char *)src, width, height) ;
	String html = buf ;

	if (link != "")
		html = "<A HREF=\"#" + link + "\">" + html + "</A>" ;

	return html ;
}

String HTMLChapterHR()
{
//	String html = "<P><HR><HR><P>\n" ;
	String html = "<P><HR SIZE=\"8\"><P>\n" ;
	return html ;
}

String HTMLSectionHR()
{
	String html ;
//	html += "<P>" ;
	html += "<HR SIZE=\"4\" WIDTH=\"90%\"><P>\n" ;
	return html ;
}

String HTMLMiniHR()
{
	String html = "<HR SIZE=\"2\" WIDTH=\"70%\">\n" ;
	return html ;
}

String HTMLName(const String& name)
{
	if (name == "") return String("") ;
	String html ;
	html = String("<A NAME=\"") + name + "\"></A>\n" ;
	return html ;
}

String HTMLLinkToName(const String& name, const String str)
{
	String html ;
	if (name == "")
		html = str ;
	else
		html = "<A HREF=\"#" + name + "\">" + str + "</A>\n" ;
	return html ;
}

String HTMLMiniLinkToName(const String& name, const String str)
{
	String html = "<FONT SIZE=2>" ;
	if (name == "")
		html += str ;
	else
		html += "<A HREF=\"#" + name + "\">" + str + "</A>\n" ;

	html += "</FONT>\n" ;
	return html ;
}

String HTMLLinkToEachMark(const String& name, const String str)
{
	String html = "<FONT SIZE=1>" ;
	if (name == "")
		html += str ;
	else
		html += "<A HREF=\"#" + name + "\">" + str + "</A>\n" ;

	html += "</FONT>\n" ;
	return html ;
}

String HTMLLinkToURL(const String& url, const String str)
{
	String html ;
	if (url == "")
		html = str ;
	else
		html = "<A HREF=\"" + url + "\"><FONT SIZE=2>" + str + "</FONT></A>\n" ;
	return html ;
}

String HTMLLinkToMasaHP()
{
	return (String("<FONT SIZE=2><I>") + HTMLLinkToURL(URL_MASA_HP, LINK_STRING_MASA_HP) + "</I></FONT>") ;
}

String HTMLLinkToGLClockMarkResultsDatabase()
{
	return (String("<I>") + HTMLLinkToURL(URL_MARK_DB, LINK_STRING_MARK_DB) + "</I>") ;
}

String HTMLMailToMasa()
{
	String html ;
	html = String("<A HREF=\"mailto:") + MAIL_MASA + "\"><FONT SIZE=2><I>" + MAIL_STRING_MASA + "</I></FONT></A>\n" ;
	return html ;
}

String HTMLChapterTitle(const String& title)
{
	String html ;
	html += "<FONT SIZE=5><CENTER><B>" + title + "</B></CENTER></FONT>\n" ;
	return html ;
}

String HTMLLargeTitle(const String& title)
{
	String html ;
	html += "<FONT SIZE=5><CENTER><B>" + title + "</B></CENTER></FONT>\n" ;
	return html ;
}

String HTMLBR(int size)
{
	String html ;
	char c = (char)size + '0' ;
	html += String("<FONT SIZE=") + c + "><BR></FONT>\n" ;
	return html ;
}

String HTMLMiniBR()
{
	String html ;
//	html += "<FONT SIZE=1><BR></FONT>\n" ;
	html += HTMLBR(1) ;
	return html ;
}

String HTMLLinkToMainSectionsAndTop()
{
	String html ;
	html = "<DIV ALIGN=\"RIGHT\"><FONT SIZE=2>" + HTMLLinkToName(LINK_DETAILS, LINK_STRING_CONTENTS) + HTMLLinkToName(LINK_TOP, "Return to the Top") + "</FONT></DIV>" ;
	return html ;
}


/*
String HTMLHighestScoreBar(double fps, double refFPS, const String& format = "%7.2f FPS ")
{
	String html ;
	String TD_KTPS    = "<TD WIDTH=\"100\">" ;
	String textSize   = "3" ;
	double length = BAR_LENGTH_LEVEL_2 ;
	int    height = BAR_HEIGHT_LEVEL_1 ;

//	double percent ;
//	if (refFPS > 0.0)
//		percent = fps / refFPS ;

	html += "<TABLE " + tableEx + ">\n" ;
	{
		// Test
		html += "<TR>\n" ;
		if (fps > 0.0 && refFPS > 0.0)
		{
			html.AddFormat(TD_KTPS + "<FONT COLOR=\"#A0FFA0\" SIZE=" + textSize + "><B>" + format + "</B></FONT></TD>\n", fps) ;
			html.AddFormat("<TD><FONT COLOR=\"#A0FFA0\" SIZE=" + textSize + "><B>%9.2f%%</B></FONT></TD>\n", fps / refFPS * 100.0) ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GREEN, (int)(fps / refFPS * glclockMark.barLength * barLength * length), height) ;
			html += "</TD>\n" ;
		}
		else
		{
			html.AddFormat(TD_KTPS + "<FONT COLOR=\"#A0FFA0\" SIZE=" + textSize + "><B>N/A </B></FONT></TD>\n", fps) ;
			html.AddFormat("<TD><FONT COLOR=\"#A0FFA0\" SIZE=" + textSize + "><B> </B></FONT></TD>\n", fps / refFPS * 100.0) ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GRAY, (int)(1.0 * glclockMark.barLength * barLength * length), height) ;
			html += "</TD>\n" ;
		}

		html += "</TR>\n" ;

		// Reference
		html += "<TR>\n" ;
		if (refFPS > 0.0)
		{
			html.AddFormat(TD_KTPS + "<FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + "><B>" + format + "</B></FONT></TD>\n", refFPS) ;
			html.AddFormat("<TD><FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + "><B>%9.2f%%</B></FONT></TD>\n", 1.0 * 100.0) ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_RED, (int)(1.0 * glclockMark.barLength * barLength * length), height) ;
			html += "</TD>\n" ;
		}
		else
		{
			html.AddFormat(TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=" + textSize + "><B>N/A </B></FONT></TD>\n", fps) ;
			html.AddFormat("<TD><FONT COLOR=\"#808080\" SIZE=" + textSize + "><B> </B></FONT></TD>\n", fps / refFPS * 100.0) ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GRAY, (int)(1.0 * glclockMark.barLength * barLength * length), height) ;
			html += "</TD>\n" ;
		}
		html += "</TR>\n" ;
	}
	html += "</TABLE>\n" ;

	return html ;
}
*/

/*
String HTMLHighestScoresTest()
{
	String html ;

	html += "<CENTER><FONT SIZE=4><B>Highest Frames/Sec.</B></FONT></CENTER>" ;
	if (glclockMark.test.highestFPS > 0.0)
		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%7.2f FPS</I></FONT></CENTER>", glclockMark.test.highestFPS) ;
	else
		html += "<CENTER><FONT SIZE=6 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += "<CENTER>" ;
	html += HTMLHighestScoreBar(glclockMark.test.highestFPS, glclockMark.reference.highestFPS, "%7.2f FPS ") ;
	html += "</CENTER>\n" ;
	html += "<P>\n" ;

//	html += HTMLMiniBR() ;

	html += "<CENTER><FONT SIZE=4><B>Highest 1000s Triangles/Sec.</B></FONT></CENTER>" ;
	if (glclockMark.test.highestKTPS > 0.0)
		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%10.3f kTPS</I></FONT></CENTER>", glclockMark.test.highestKTPS) ;
	else
		html += "<CENTER><FONT SIZE=6 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += "<CENTER>" ;
	html += HTMLHighestScoreBar(glclockMark.test.highestKTPS, glclockMark.reference.highestKTPS, "%10.3f kTPS ") ;
	html += "</CENTER>\n" ;
	html += "<P>\n" ;

	return html ;
}
*/

String HTMLHighestScores()
{
	String html ;

	html += "<CENTER>" ;
	html += "<FONT SIZE=\"4\"><B>Highest Scores</B></FONT>\n" ;
	html += "</CENTER>\n" ;

	html += "<CENTER>" ;
	html += "<TABLE " + tableEx + ">\n" ;
	{
		html += "<TR>\n" ;
		{
			html += "<TD ALIGN=\"CENTER\">\n" ;
				html += "<FONT SIZE=3><B> Mark </B></FONT>" ;
			html += "</TD>\n" ;

			html += "<TD ALIGN=\"CENTER\">\n" ;
				html += "<FONT SIZE=3><B> Frames/Sec. </B></FONT>" ;
			html += "</TD>\n" ;

			html += "<TD ALIGN=\"CENTER\">\n" ;
				html += "<FONT SIZE=3><B> kilo-Triangles/Sec. </B></FONT>" ;
			html += "</TD>\n" ;
		}
		html += "</TR>\n" ;

		html += "<TR>\n" ;
		{
			html += "<TD ALIGN=\"CENTER\">\n" ;
			{
				if (glclockMark.test.highestMark > 0.0)
					html.AddFormat(String("<A HREF=\"#") + LINK_HIGHEST_MARK + "\"><FONT SIZE=5 COLOR=\"#FFFFA0\"><B><I> %9.2f%% </I></B></FONT></A>", glclockMark.test.highestMark * 100.0) ;
				else
					html += "<FONT SIZE=5 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>" ;
			}
			html += "</TD>\n" ;

			html += "<TD ALIGN=\"CENTER\">\n" ;
			{
				if (glclockMark.test.highestFPS > 0.0)
					html.AddFormat(String("<A HREF=\"#") + LINK_HIGHEST_FPS + "\"><FONT SIZE=5 COLOR=\"#FFFFA0\"><B><I> %7.2f FPS </I></B></FONT></A>", glclockMark.test.highestFPS) ;
				else
					html += "<FONT SIZE=5 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>" ;
			}
			html += "</TD>\n" ;

			html += "<TD ALIGN=\"CENTER\">\n" ;
			{
				if (glclockMark.test.highestKTPS > 0.0)
					html.AddFormat(String("<A HREF=\"#") + LINK_HIGHEST_KTPS + "\"><FONT SIZE=5 COLOR=\"#FFFFA0\"><B><I> %10.3f kTPS </I></B></FONT></A>", glclockMark.test.highestKTPS) ;
				else
					html += "<FONT SIZE=5 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>" ;
			}
			html += "</TD>\n" ;
		}
		html += "</TR>\n" ;

		html += "<TR>\n" ;
		{
			html += "<TD ALIGN=\"CENTER\">\n" ;
			{
				if (glclockMark.reference.highestMark > 0.0)
					html.AddFormat("<FONT SIZE=4 COLOR=\"#FFA0A0\"><B><I> %9.2f%% </I></B></FONT>", glclockMark.reference.highestMark * 100.0) ;
				else
					html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>" ;
			}
			html += "</TD>\n" ;

			html += "<TD ALIGN=\"CENTER\">\n" ;
			{
				if (glclockMark.reference.highestFPS > 0.0)
					html.AddFormat("<FONT SIZE=4 COLOR=\"#FFA0A0\"><B><I> %7.2f FPS</I></B></FONT>", glclockMark.reference.highestFPS) ;
				else
					html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>" ;
			}
			html += "</TD>\n" ;

			html += "<TD ALIGN=\"CENTER\">\n" ;
			{
				if (glclockMark.reference.highestKTPS > 0.0)
					html.AddFormat("<FONT SIZE=4 COLOR=\"#FFA0A0\"><B><I> %10.3f kTPS </I></B></FONT>", glclockMark.reference.highestKTPS) ;
				else
					html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>" ;
			}
			html += "</TD>\n" ;
		}
		html += "</TR>\n" ;

/*
		html += "<TR>\n" ;
		{
			html += "<TD>\n" ;
			{
//				html += "<CENTER>\n" ;
				{
					html += "<P ALIGN=\"CENTER\"><FONT SIZE=4><B>Highest Frames/Sec. </B></FONT></P>\n" ;

					html += "<BR>\n" ;
					if (glclockMark.test.highestFPS > 0.0)
						html.AddFormat("<P ALIGN=\"CENTER\"><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%7.2f FPS</I></FONT></P>\n", glclockMark.test.highestFPS) ;
					else
						html += "<P ALIGN=\"CENTER\"><FONT SIZE=6 COLOR=\"#808080\"><I>N/A</I></FONT></P>\n" ;

					html += "<BR>\n" ;
					if (glclockMark.reference.highestFPS > 0.0)
						html.AddFormat("<P ALIGN=\"CENTER\"><FONT SIZE=4 COLOR=\"#FFA0A0\"><B><I>%7.2f FPS</I></B></FONT></P>\n", glclockMark.reference.highestFPS) ;
					else
						html += "<P ALIGN=\"CENTER\"><FONT SIZE=4 COLOR=\"#808080\"><B><I>N/A</I></B></FONT></P>\n" ;
				}
//				html += "</CENTER>\n" ;
			}
			html += "</TD>\n" ;

			html += "<TD>\n" ;
			{
//				html += "<CENTER>\n" ;
				{
					html += "<FONT SIZE=4><B> Highest kilo-Triangles/Sec.</B></FONT>\n" ;

					html += "<BR>\n" ;
					if (glclockMark.test.highestKTPS > 0.0)
						html.AddFormat("<FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%10.3f kTPS</I></FONT>\n", glclockMark.test.highestKTPS) ;
					else
						html += "<FONT SIZE=6 COLOR=\"#808080\"><I> N/A </I></FONT>\n" ;

					html += "<BR>" ;
					if (glclockMark.reference.highestKTPS > 0.0)
						html.AddFormat("<FONT SIZE=4 COLOR=\"#FFA0A0\"><B><I>%10.3f kTPS</I></B></FONT>\n", glclockMark.reference.highestKTPS) ;
					else
						html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A </I></B></FONT>\n" ;
				}
//				html += "</CENTER>\n" ;
			}
			html += "</TD>\n" ;

		}
		html += "</TR>\n" ;
*/
	}
	html += "</TABLE>\n" ;
	html += "</CENTER>\n" ;

	return html ;
}


String HTMLglclockMark()
{
	String html ;

	// specTestEnv テスト環境に OpenGL Renderer をセット
	specTestEnv = glclockMark.test.infoOpenGLRenderer + " " + glclockMark.test.infoColorBuffer ;

	// -ref オプションで参照環境のログが指定されている場合は、参照環境に OpenGL Renderer をセット
	if (file_REF_LOG != FILE_REF_LOG)
		specRefEnv = glclockMark.reference.infoOpenGLRenderer + " " + glclockMark.reference.infoColorBuffer ;

	// glclock Mark グラフ表示
	html += "<CENTER><TABLE " + tableEx + ">\n" ;
	{
		// Test
		html.AddFormat("<TR>\n") ;
		if (glclockMark.test.glclockMark > 0.0)
		{
			html.AddFormat("<TD ALIGN=\"RIGHT\"><FONT COLOR=\"#A0FFA0\" SIZE=4><B>%9.2f%%</B></FONT></TD>\n", glclockMark.test.glclockMark * 100.0) ;

			html.AddFormat("<TD>\n") ;
				html.AddFormat(HTMLBar(BAR_FILE_GREEN, (int)(glclockMark.test.glclockMark * glclockMark.barLength * barLength), 16)) ;
				html.AddFormat("<BR><FONT COLOR=\"A0FFA0\" SIZE=2><B>" + specTestEnv + "</B></FONT>\n") ;
			html.AddFormat("</TD>\n") ;
		}
		else
		{
			html += "<TD ALIGN=\"RIGHT\"><FONT COLOR=\"#808080\" SIZE=4><B> N/A</B></FONT></TD>\n" ;

			html.AddFormat("<TD>\n") ;
				html.AddFormat(HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength), 16)) ;
				html.AddFormat("<BR><FONT COLOR=\"#808080\" SIZE=2><B>" + specTestEnv + "</B></FONT>\n") ;
			html.AddFormat("</TD>\n") ;
		}
		html.AddFormat("</TR>\n") ;

		// Reference
		html.AddFormat("<TR>\n") ;
		if (glclockMark.reference.glclockMark)
		{
			html.AddFormat("<TD ALIGN=\"RIGHT\"><FONT COLOR=\"#FFA0A0\" SIZE=4><B>%9.2f%%</B></FONT></TD>\n", glclockMark.reference.glclockMark * 100.0) ;

			html.AddFormat("<TD>\n") ;
				html.AddFormat(HTMLBar(BAR_FILE_RED, (int)(glclockMark.reference.glclockMark * glclockMark.barLength * barLength), 16)) ;
				html.AddFormat("<BR><FONT COLOR=\"FFA0A0\" SIZE=2><B>" + specRefEnv + "</B></FONT>\n") ;
			html.AddFormat("</TD>\n") ;
		}
		else
		{
			html += "<TD ALIGN=\"RIGHT\"><FONT COLOR=\"#808080\" SIZE=4><B> N/A</B></FONT></TD>\n" ;

			html.AddFormat("<TD>\n") ;
				html.AddFormat(HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength), 16)) ;
				html.AddFormat("<BR><FONT COLOR=\"808080\" SIZE=2><B>" + specRefEnv + "</B></FONT>\n") ;
			html.AddFormat("</TD>\n") ;
		}
		html.AddFormat("</TR>\n") ;
	}
	html.AddFormat("</TABLE></CENTER>\n") ;

	return html ;
}

String HTMLEachSectionMarkFPS(int sectionNo,
							  double mark, double refMark,
							  double fps, double refFPS,
							  const String& link = "")
{
	String html ;
	double length = BAR_LENGTH_LEVEL_2 ;
	int    height = BAR_HEIGHT_LEVEL_2 ;

	String TD_KTPS = TD_KTPS_LEVEL_1 ;

	// ハイスコアの項目はカラーを金色に
	String barSrc = BAR_FILE_GREEN ;
	String color = "COLOR=\"#A0FFA0\"" ;
	if (Abs(mark - glclockMark.test.highestSectionMark) < 0.00001)	// 念のため誤差修正
	{
		color = "COLOR=\"#FFFFA0\"" ;
		barSrc = BAR_FILE_GOLD ;
	}

	// % 表示
//	TD_KTPS = "<TD>" ;

	// セクション
	html += "<FONT SIZE=4><B>" + HTMLLinkToName(link, chapterName[sectionNo]) + "</B></FONT>\n" ;
	if (mark > 0.0)
		html.AddFormat("<FONT " + color + " SIZE=\"4\"><B><I>%9.2f%%</I></B></FONT>\n", mark * 100.0) ;
	else
		html.AddFormat("<FONT COLOR=\"#808080\" SIZE=\"4\"><B><I> N/A</I></B></FONT>\n") ;

	html += "<UL>\n" ;
	{
		// Mark グラフ表示
		html.AddFormat("<TABLE " + tableEx + ">\n") ;
		{
			// Test
			html += "<TR>\n" ;
			if (fps > 0.0)
			{
				html.AddFormat(TD_KTPS + "<FONT " + color + " SIZE=3><B>%7.2f FPS </B></FONT></TD>\n", fps) ;
				html += "<TD>\n" ;
				html += HTMLBar(barSrc, (int)(mark * glclockMark.barLength * barLength * length), height, link) ;
				html += "</TD>\n" ;
			}
			else
			{
				html += TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=3><B> N/A </B></FONT></TD>\n" ;
				html += "<TD>\n" ;
				html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length), height) ;
				html += "</TD>\n" ;
			}

			html += "</TR>\n" ;

			// Reference
			html += "<TR>\n" ;
			if (refFPS > 0.0)
			{
				html.AddFormat(TD_KTPS + "<FONT COLOR=\"#FFA0A0\" SIZE=3><B>%7.2f FPS </B></FONT></TD>\n", refFPS) ;
//				html.AddFormat(TD_KTPS + "<P ALIGN=\"RIGHT\"><FONT COLOR=\"#FFA0A0\" SIZE=3><B>%9.2f%% </B></FONT></P></TD>\n", refMark * 100.0) ;
				html += "<TD>\n" ;
				html += HTMLBar(BAR_FILE_RED, (int)(refMark * glclockMark.barLength * barLength * length), height, link) ;
				html += "</TD>\n" ;
			}
			else
			{
				html += TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=3><B> N/A </B></FONT></TD>\n" ;
				html += "<TD>\n" ;
				html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length), height) ;
				html += "</TD>\n" ;
			}
			html += "</TR>\n" ;
		}
		html += "</TABLE>\n" ;
	}
	html += "</UL>\n" ;

	return html ;
}

String HTMLEachSectionMarkKTPS(int sectionNo,
							   double mark, double refMark,
							   double kTPS, double refKTPS,
							   const String& link = "")
{
	String html ;
	double length = BAR_LENGTH_LEVEL_2 ;
	int    height = BAR_HEIGHT_LEVEL_2 ;

	String TD_KTPS = TD_KTPS_LEVEL_1 ;

	// ハイスコアの項目はカラーを金色に
	String barSrc = BAR_FILE_GREEN ;
	String color = "COLOR=\"#A0FFA0\"" ;
	if (Abs(mark - glclockMark.test.highestSectionMark) < 0.00001)	// 念のため誤差修正
	{
		color = "COLOR=\"#FFFFA0\"" ;
		barSrc = BAR_FILE_GOLD ;
	}

	// % 表示
//	TD_KTPS = "<TD>" ;

	// セクション
	html += "<FONT SIZE=4><B>" + HTMLLinkToName(link, chapterName[sectionNo]) + "</B></FONT>\n" ;
	if (mark > 0.0)
		html.AddFormat("<FONT " + color + " SIZE=\"4\"><B><I>%9.2f%%</I></B></FONT>\n", mark * 100.0) ;
	else
		html.AddFormat("<FONT COLOR=\"#808080\" SIZE=\"4\"><B><I> N/A</I></B></FONT>\n") ;
//	html += " " + HTMLLinkToName(link, " (more Details)") ;

	html += "<UL>\n" ;
	{
		// Mark グラフ表示
		html.AddFormat("<TABLE " + tableEx + ">\n") ;
		{
			// Test
			html += "<TR>\n" ;
			if (kTPS > 0.0)
			{
				html.AddFormat(TD_KTPS + "<FONT " + color + " SIZE=3><B>%10.3f kTPS </B></FONT></TD>\n", kTPS) ;
				html += "<TD>\n" ;
				html += HTMLBar(barSrc, (int)(mark * glclockMark.barLength * barLength * length), height, link) ;
				html += "</TD>\n" ;
			}
			else
			{
				html += TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=3><B> N/A </B></FONT></TD>\n" ;
				html += "<TD>\n" ;
				html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length), height) ;
				html += "</TD>\n" ;
			}
			html += "</TR>\n" ;

			// Reference
			html += "<TR>\n" ;
			if (refKTPS > 0.0)
			{
				html.AddFormat(TD_KTPS + "<FONT COLOR=\"#FFA0A0\" SIZE=3><B>%10.3f kTPS </B></FONT></TD>\n", refKTPS) ;
//				html.AddFormat(TD_KTPS + "<P ALIGN=\"RIGHT\"><FONT COLOR=\"#FFA0A0\" SIZE=3><B>%9.2f%% </B></FONT></P></TD>\n", refMark * 100.0) ;
				html += "<TD>\n" ;
				html += HTMLBar(BAR_FILE_RED, (int)(refMark * glclockMark.barLength * barLength * length), height, link) ;
				html += "</TD>\n" ;
			}
			else
			{
				html += TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=3><B> N/A </B></FONT></TD>\n" ;
				html += "<TD>\n" ;
				html += HTMLBar(BAR_FILE_RED, (int)(glclockMark.barLength * barLength * length), height) ;
				html += "</TD>\n" ;
			}
			html += "</TR>\n" ;
		}
		html += "</TABLE>\n" ;
	}
	html += "</UL>\n" ;

	return html ;
}


String HTMLDetails()
{
	String html ;

//	html += "<FONT SIZE=6><CENTER>Details</CENTER></FONT>\n" ;
	html += HTMLChapterTitle(TITLE_CONTENTS) ;
//	html += HTMLMiniBR() ;
	html += "<CENTER><FONT SIZE=4><B>click to go each details</B></FONT></CENTER>" + HTMLMiniBR() ;

	html += "<UL>\n" ;
	{
		// Per-Vertex Lighting Mark
//		html += "<P>\n" ;
		html += HTMLEachSectionMarkFPS(0, glclockMark.test.lightingMark.lightingMark,
										  glclockMark.reference.lightingMark.lightingMark,
										  glclockMark.test.lightingMark.averageFPS,
										  glclockMark.reference.lightingMark.averageFPS, LINK_LIGHTING_MARK) ;
//		html += HTMLMiniBR() ;

		// Per-Vertex Lighted Texturing Mark
		html += HTMLEachSectionMarkFPS(1, glclockMark.test.lightingTexMark.lightingTexMark,
										  glclockMark.reference.lightingTexMark.lightingTexMark,
										  glclockMark.test.lightingTexMark.averageFPS,
										  glclockMark.reference.lightingTexMark.averageFPS, LINK_LIGHTED_TEX_MARK) ;
//		html += HTMLMiniBR() ;

		// Texture Filtering Mark
		html += HTMLEachSectionMarkFPS(2, glclockMark.test.filteringMark.filteringMark,
										  glclockMark.reference.filteringMark.filteringMark,
										  glclockMark.test.filteringMark.averageFPS,
										  glclockMark.reference.filteringMark.averageFPS, LINK_FILTERING_MARK) ;
//		html += HTMLMiniBR() ;

		// Environment Mapping Mark
		html += HTMLEachSectionMarkFPS(3, glclockMark.test.envMapMark.envMapMark,
										  glclockMark.reference.envMapMark.envMapMark,
										  glclockMark.test.envMapMark.averageFPS,
										  glclockMark.reference.envMapMark.averageFPS, LINK_ENV_MAPPING_MARK) ;
//		html += HTMLMiniBR() ;

		// Super Sampled Blending Mark
		html += HTMLEachSectionMarkFPS(4, glclockMark.test.superSampleMark.superSampleMark,
										  glclockMark.reference.superSampleMark.superSampleMark,
										  glclockMark.test.superSampleMark.averageFPS,
										  glclockMark.reference.superSampleMark.averageFPS, LINK_SUPER_SAMPLE_MARK) ;
//		html += HTMLMiniBR() ;

		// Maximum Triangles Mark
		html += HTMLEachSectionMarkKTPS(5, glclockMark.test.maxTriMark.maxTriMark,
										   glclockMark.reference.maxTriMark.maxTriMark,
										   glclockMark.test.maxTriMark.averageKTPS,
										   glclockMark.reference.maxTriMark.averageKTPS, LINK_MAX_TRIANGLES_MARK) ;
//		html += HTMLMiniBR() ;

		// OpenGL Practical Rendering Mark
		html += HTMLEachSectionMarkFPS(6, glclockMark.test.practicalMark.practicalMark,
										  glclockMark.reference.practicalMark.practicalMark,
										  glclockMark.test.practicalMark.averageFPS,
										  glclockMark.reference.practicalMark.averageFPS, LINK_PRACTICAL_MARK) ;
	}
	html += "</UL>\n" ;

	return html ;
}


void SetLengthAndHeightLevel(int level, double& length, int& height, String& textSize,
							 String *TD_FPS = NULL, String *TD_KTPS = NULL, String *TD_PERCENT = NULL)
{
	String work_TD_FPS ;
	String work_TD_KTPS ;
	String work_TD_PERCENT ;

	switch (level)
	{
	case 0:
		length = 1.0 ; height = 16 ;
		work_TD_FPS = TD_FPS_LEVEL_1 ; work_TD_KTPS = TD_KTPS_LEVEL_1 ; work_TD_PERCENT = TD_PERCENT_LEVEL_1 ;
		break ;
	case 1:
		length = BAR_LENGTH_LEVEL_1 ; height = BAR_HEIGHT_LEVEL_1 ; textSize = BAR_TEXT_SIZE_LEVEL_1 ;
		work_TD_FPS = TD_FPS_LEVEL_1 ; work_TD_KTPS = TD_KTPS_LEVEL_1 ; work_TD_PERCENT = TD_PERCENT_LEVEL_1 ;
		break ;
	case 2:
		length = BAR_LENGTH_LEVEL_2 ; height = BAR_HEIGHT_LEVEL_2 ; textSize = BAR_TEXT_SIZE_LEVEL_2 ;
		work_TD_FPS = TD_FPS_LEVEL_2 ; work_TD_KTPS = TD_KTPS_LEVEL_2 ; work_TD_PERCENT = TD_PERCENT_LEVEL_2 ;
		break ;
	case 3:
		length = BAR_LENGTH_LEVEL_3 ; height = BAR_HEIGHT_LEVEL_3 ; textSize = BAR_TEXT_SIZE_LEVEL_3 ;
		work_TD_FPS = TD_FPS_LEVEL_3 ; work_TD_KTPS = TD_KTPS_LEVEL_3 ; work_TD_PERCENT = TD_PERCENT_LEVEL_3 ;
		break ;
	case 4:
		length = BAR_LENGTH_LEVEL_4 ; height = BAR_HEIGHT_LEVEL_4 ; textSize = BAR_TEXT_SIZE_LEVEL_4 ;
		work_TD_FPS = TD_FPS_LEVEL_4 ; work_TD_KTPS = TD_KTPS_LEVEL_4 ; work_TD_PERCENT = TD_PERCENT_LEVEL_4 ;
		break ;
	case 5:
		length = BAR_LENGTH_LEVEL_5 ; height = BAR_HEIGHT_LEVEL_5 ; textSize = BAR_TEXT_SIZE_LEVEL_5 ;
		work_TD_FPS = TD_FPS_LEVEL_5 ; work_TD_KTPS = TD_KTPS_LEVEL_5 ; work_TD_PERCENT = TD_PERCENT_LEVEL_5 ;
		break ;
	}

	if (TD_FPS    ) *TD_FPS = work_TD_FPS ;
	if (TD_KTPS   ) *TD_FPS = work_TD_KTPS ;
	if (TD_PERCENT) *TD_FPS = TD_PERCENT_LEVEL_5 ;
}


// barBaseModeLength
// 各章の詳細情報では、バーの長さは mark ではなく、
// 各章での平均 FPS(kTPS) に対する参照環境の当該 FPS の割合となる
// 参照環境の当該 FPS(kTPS) をセットしてコール
String HTMLMarkBarFPS(double mark, double refMark,
					  double fps, double refFPS,
					  int level, double barBaseModeLength = 0.0, const String& link = "")
{
	String html ;
	double length ;
	int    height ;
	String textSize ;
	SetLengthAndHeightLevel(level, length, height, textSize) ;

	if (barBaseModeLength != 0.0)
		barBaseModeLength = refFPS / barBaseModeLength ;
	else
		barBaseModeLength = 1.0 ;

	double percentage = 0.0 ;
	if (refFPS > 0.0)
	{
		if (level != 1)
			percentage = fps / refFPS ;
		else
			percentage = mark ;	// レベル１（章タイトルのバー）の場合は、バーの長さの基準はパーセンテージとする
	}

	// レベル１（各章タイトル）で、ハイスコアと一致の項目はカラーを金色に
	String barSrc = BAR_FILE_GREEN ;
	String color = "COLOR=\"#A0FFA0\"" ;
	if (level == 1 && Abs(percentage - glclockMark.test.highestSectionMark) < 0.00001)
	{
		color = "COLOR=\"#FFFFA0\"" ;
		barSrc = BAR_FILE_GOLD ;
	}

	html += "<TABLE " + tableEx + ">\n" ;
	{
		// Test
		html += "<TR>\n" ;
//		if (fps > 0.0)
		if (mark > 0.0)
		{
			html.AddFormat("<TD NOWRAP ALIGN=\"RIGHT\"><FONT " + color + " SIZE=" + textSize + "><B>%7.2f FPS </B></FONT></TD>\n", fps) ;
			html += "<TD>\n" ;
			html += HTMLBar(barSrc, (int)(percentage * glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}
		else
		{
			html += "<TD NOWRAP ALIGN=\"RIGHT\"><FONT COLOR=\"#808080\" SIZE=" + textSize + "><B> N/A </B></FONT></TD>\n" ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}

		html += "</TR>\n" ;

		// Reference
		html += "<TR>\n" ;
//		if (refFPS > 0.0)
		if (refMark > 0.0)
		{
			html.AddFormat("<TD NOWRAP ALIGN=\"RIGHT\"><FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + "><B>%7.2f FPS </B></FONT></TD>\n", refFPS) ;
//			html.AddFormat("<TD><P ALIGN=\"RIGHT\"><FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + "><B>%9.2f%% </B></FONT></P></TD>\n", refMark * 100.0) ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_RED, (int)(1.0 * glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}
		else
		{
			html += "<TD NOWRAP ALIGN=\"RIGHT\"><FONT COLOR=\"#808080\" SIZE=" + textSize + "><B> N/A </B></FONT></TD>\n" ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}
		html += "</TR>\n" ;
	}
	html += "</TABLE>\n" ;

	return html ;
}

String HTMLMarkBarKTPS(double mark, double refMark,
					   double kTPS, double refKTPS,
					   int level, double barBaseModeLength = 0.0, const String& link = "")
{
	String html ;
	double length ;
	int    height ;
	String textSize ;
	SetLengthAndHeightLevel(level, length, height, textSize) ;

	if (barBaseModeLength != 0.0)
		barBaseModeLength = refKTPS / barBaseModeLength ;
	else
		barBaseModeLength = 1.0 ;

	double percentage = 0.0 ;
	if (refKTPS > 0.0)
	{
		if (level != 1)
			percentage = kTPS / refKTPS ;
		else
			percentage = mark ;	// レベル１（章タイトルのバー）の場合は、バーの長さの基準はパーセンテージとする
	}

	// レベル１（各章タイトル）で、ハイスコアと一致の項目はカラーを金色に
	String barSrc = BAR_FILE_GREEN ;
	String color = "COLOR=\"#A0FFA0\"" ;
	if (level == 1 && Abs(percentage - glclockMark.test.highestSectionMark) < 0.00001)
	{
		color = "COLOR=\"#FFFFA0\"" ;
		barSrc = BAR_FILE_GOLD ;
	}

	html += "<TABLE " + tableEx + ">\n" ;
	{
		// Test
		html += "<TR>\n" ;
		if (kTPS > 0.0)
		{
			html.AddFormat("<TD NOWRAP ALIGN=\"RIGHT\"><FONT " + color + " SIZE=" + textSize + "><B>%10.3f kTPS </B></FONT></TD>\n", kTPS) ;
			html += "<TD>\n" ;
			html += HTMLBar(barSrc, (int)(percentage * glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}
		else
		{
			html += "<TD NOWRAP ALIGN=\"RIGHT\"><FONT COLOR=\"#808080\" SIZE=" + textSize + "><B> N/A </B></FONT></TD>\n" ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}

		html += "</TR>\n" ;

		// Reference
		html += "<TR>\n" ;
		if (refKTPS > 0.0)
		{
			html.AddFormat("<TD NOWRAP ALIGN=\"RIGHT\"><FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + "><B>%10.3f kTPS </B></FONT></TD>\n", refKTPS) ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_RED, (int)(1.0 * glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}
		else
		{
			html += "<TD NOWRAP ALIGN=\"RIGHT\"><FONT COLOR=\"#808080\" SIZE=" + textSize + "><B> N/A </B></FONT></TD>\n" ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_GRAY, (int)(glclockMark.barLength * barLength * length * barBaseModeLength), height, link) ;
			html += "</TD>\n" ;
		}
		html += "</TR>\n" ;
	}
	html += "</TABLE>\n" ;

	return html ;
}


String HTMLChapterMarkBarFPS(double mark, double refMark,
							 double fps, double refFPS)
{
	String html ;

	html += "<CENTER>" ;
	html += HTMLMarkBarFPS(mark, refMark, fps, refFPS, 1) ;
	html += "</CENTER>\n" ;

	return html ;
}


String HTMLChapterMarkBarKTPS(double mark, double refMark,
							  double kTPS, double refKTPS)
{
	String html ;

	html += "<CENTER>" ;
	html += HTMLMarkBarKTPS(mark, refMark, kTPS, refKTPS, 1) ;
	html += "</CENTER>\n" ;

	return html ;
}


// fps, refFPS, kTPS, refKTPS には 必ず有効な値をセットする必要あり
String HTMLSubMarkBar(const String& title,
					  const String& extension, const String& refExtension,
					  int flagFPS, int flagKTPS,
					  double fps, double refFPS,
					  double kTPS, double refKTPS,
					  int level, double barBaseModeLength = 0.0)
{
	flagFPS = TRUE ;
	flagKTPS = TRUE ;


	String html ;

	double length ;
	int    height ;
	String textSize ;
	String mTextSize ;
	String lTextSize ;
	String TD_FPS ;
	String TD_KTPS ;
	String TD_PERCENT ;
	SetLengthAndHeightLevel(level, length, height, textSize) ;	// , &TD_FPS, &TD_KTPS, &TD_PERCENT) ;

	String barB ;
	String barBE ;
	if (level == 4 || level == 5)
	{
		barB = "<B>" ;
		barBE = "</B>" ;
	}

	TD_FPS = "<TD NOWRAP ALIGN=\"RIGHT\">" ;
	TD_KTPS = "<TD NOWRAP ALIGN=\"RIGHT\">" ;
	TD_PERCENT = "<TD NOWRAP ALIGN=\"RIGHT\">" ;

	// %
	double percentage = 0.0 ;
//	if (!flagKTPS || (flagFPS && flagKTPS))
	{
		// kTPS フラグが指定されていない場合、もしくは両方指定されている場合は FPS ベースで計算
		if (fps > 0 && refFPS > 0)
			percentage = fps / refFPS ;
	}
//	else	// kTPS のみ指定されている場合は kTPS ベースで計算
//	{
//		if (kTPS > 0 && refKTPS > 0)
//			percentage = kTPS / refKTPS ;
//	}

	// ハイスコアの項目はカラーを金色に
	String barSrc = BAR_FILE_GREEN ;
	String color = "COLOR=\"#A0FFA0\"" ;

	// ハイスコアの場合簡単な説明を追加
	String highScoreSpecs ;
	String highScoreMark ;
	String highScoreFPS ;
	String highScoreKTPS ;

	if (flag_highestMark && Abs(percentage - glclockMark.test.highestMark) < 0.00001)
	{
		flag_highestMark = FALSE ;
		color = "COLOR=\"#FFFFA0\"" ;
		html += HTMLName(LINK_HIGHEST_MARK) ;
		barSrc = BAR_FILE_GOLD ;
		highScoreMark = HIGH_SCORE_MARK_SPEC ;
//		highScoreMark.Format("Highest: %9.2f%%", percentage * 100.0) ;
	}
	if (flag_highestFPS && fps == glclockMark.test.highestFPS)
	{
		flag_highestFPS = FALSE ;
		color = "COLOR=\"#FFFFA0\"" ;
		html += HTMLName(LINK_HIGHEST_FPS) ;
		barSrc = BAR_FILE_GOLD ;
//		highScoreFPS = HIGH_SCORE_FPS_SPEC ;
		highScoreFPS.Format("Highest: %7.2f FPS", fps) ;
	}
	if (flag_highestKTPS && kTPS == glclockMark.test.highestKTPS)
	{
		flag_highestKTPS = FALSE ;
		color = "COLOR=\"#FFFFA0\"" ;
		html += HTMLName(LINK_HIGHEST_KTPS) ;
		barSrc = BAR_FILE_GOLD ;
//		highScoreKTPS = HIGH_SCORE_KTPS_SPEC ;
		highScoreKTPS.Format("Highest: %10.3f kTPS", kTPS) ;
	}

	if (highScoreMark != "" || highScoreFPS != "" || highScoreKTPS != "")
	{
		highScoreSpecs = highScoreMark ;

		if (highScoreFPS != "")
		{
			if (highScoreSpecs != "")
				highScoreSpecs += ", " + highScoreFPS ;
			else
				highScoreSpecs = highScoreFPS ;
		}

		if (highScoreKTPS != "")
		{
			if (highScoreSpecs != "")
				highScoreSpecs += ", " + highScoreKTPS ;
			else
				highScoreSpecs = highScoreKTPS ;
		}

		highScoreSpecs = " (" + highScoreSpecs + ")" ;
	}

	mTextSize = textSize ;
	if (mTextSize.Character(1) >= '2' && mTextSize.Character(1) <= '7')
		mTextSize.SetAt(1, mTextSize.Character(1) - 1) ;

	lTextSize = textSize ;
	if (lTextSize.Character(1) <= '6' && lTextSize.Character(1) >= '1')
		lTextSize.SetAt(1, lTextSize.Character(1) + 1) ;

	if (barBaseModeLength != 0.0)
		barBaseModeLength = refFPS / barBaseModeLength ;
	else
		barBaseModeLength = 1.0 ;

	String str ;
	String refStr ;
	if (flagFPS)
	{
		if (fps > 0.0)
			str.AddFormat(TD_FPS + "<FONT " + color + " SIZE=" + textSize + ">" + barB + "%7.2f FPS " + barBE + "</FONT></TD>\n", fps) ;
		else
			str += TD_FPS + "<FONT COLOR=\"#808080\" SIZE=" + textSize + ">" + barB + "N/A  " + barBE + "</FONT></TD>\n" ;

		if (refFPS > 0.0)
			refStr.AddFormat(TD_FPS + "<FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + ">" + barB + "%7.2f FPS " + barBE + "</FONT></TD>\n", refFPS) ;
		else
			refStr += TD_FPS + "<FONT COLOR=\"#808080\" SIZE=" + textSize + ">" + barB + "N/A " + barBE + "</FONT></TD>\n" ;
	}
	if (flagKTPS)
	{
		if (kTPS > 0.0)
			str.AddFormat(TD_KTPS + "<FONT " + color + " SIZE=" + textSize + ">" + barB + "%10.3f kTPS " + barBE + "</FONT></TD>\n", kTPS) ;
		else
			str += TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=" + textSize + ">" + barB + "N/A " + barBE + "</FONT></TD>\n" ;

		if (refKTPS > 0.0)
			refStr.AddFormat(TD_KTPS + "<FONT COLOR=\"#FFA0A0\" SIZE=" + textSize + ">" + barB + "%10.3f kTPS " + barBE + "</FONT></TD>\n", refKTPS) ;
		else
			refStr += TD_KTPS + "<FONT COLOR=\"#808080\" SIZE=" + textSize + ">" + barB + "N/A " + barBE + "</FONT></TD>\n" ;
	}

/*
	if (level != 4 && level != 5)
		html += "<FONT SIZE=" + textSize + "><B>" + title + "</B></FONT><BR>\n" ;
	else
		html += "<FONT SIZE=" + textSize + "><FONT SIZE=+1><B>" + title + "</B></FONT></FONT><BR>\n" ;
*/

//	html += "<FONT SIZE=" + lTextSize + "><B>" + title + "</B></FONT><BR>\n" ;

	html += "<FONT SIZE=" + lTextSize + "><B>" + title + "</B></FONT>\n" ;
	if (percentage > 0.0)
		html.AddFormat("<FONT SIZE=" + lTextSize + " " + color + "><B><I> %9.2f%%" + highScoreSpecs + "</I></B></FONT><BR>\n", percentage * 100.0) ;
	else
		html += "<FONT SIZE=" + lTextSize + " COLOR=\"#808080\"><B><I> N/A</I></B></FONT><BR>\n" ;

	html += "<UL>" ;
	{
		html += "<TABLE " + tableEx + ">\n" ;
		{
			// Test
			html += "<TR>\n" ;
			html += str ;
			html += "<TD>\n" ;

			if (fps > 0.0)
				html += HTMLBar(barSrc, (int)(percentage * glclockMark.barLength * barLength * length * barBaseModeLength), height) ;
			else
				html += HTMLBar(BAR_FILE_GRAY, (int)(1.0 * glclockMark.barLength * barLength * length * barBaseModeLength), height) ;

			html += "<BR><FONT " + color + " SIZE=" + mTextSize + ">" + extension + "</FONT>\n" ;
			html += "</TD>\n" ;
			html += "</TR>\n" ;

			// Reference
			html += "<TR>\n" ;
			html += refStr ;
			html += "<TD>\n" ;
			html += HTMLBar(BAR_FILE_RED, (int)(1.0 * glclockMark.barLength * barLength * length * barBaseModeLength), height) ;
			html += "<BR><FONT COLOR=\"#FFA0A0\" SIZE=" + mTextSize + ">" + refExtension + "</FONT>\n" ;
			html += "</TD>\n" ;
			html += "</TR>\n" ;
		}
		html += "</TABLE>\n" ;
	}
	html += "</UL>" ;
//	html += HTMLMiniBR() ;

	return html ;
}

String HTMLPerVertexLightingSubMark(int no, int detailFlag = FALSE)
{
	String html ;
	double fps = glclockMark.test.lightingMark.FPS[no] ;
	double refFPS = glclockMark.reference.lightingMark.FPS[no] ;
	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.lightingMark.averageFPS ;

//	if (refFPS) percent = fps / refFPS ;
	percent = glclockMark.test.lightingMark.percentage[no] ;

	if (!detailFlag)
	{
		html += "<FONT SIZE=4><B>" + HTMLLinkToName(LightingMark::HTMLname[no], LightingMark::title[no]) + "</B></FONT>\n" ;
		if (fps > 0.0)
			html.AddFormat("<FONT SIZE=4 COLOR=\"#A0FFA0\"><B><I> %9.2f%%</I></B></FONT>", percent * 100.0) ;
		else
			html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A</I></B></FONT>" ;

		html += "<UL>" ;
			html += HTMLMarkBarFPS(percent, 1.0, fps, refFPS, 3, refAverageFPS, LightingMark::HTMLname[no]) ;	// バーの基準は、章全体の参照環境の平均に合わせる
		html += "</UL>" ;
	}
	else
	{
		html += "<CENTER>\n" ;
		{
			html += "<FONT SIZE=4><B>" + LightingMark::title[no] + "</B></FONT>\n" ;
			html += HTMLMiniBR() ;
			if (fps > 0.0)
				html.AddFormat("<BR><FONT SIZE=5 COLOR=\"#A0FFA0\"><B><I> %9.2f%%</I></B></FONT>", percent * 100.0) ;
			else
				html += "<BR><FONT SIZE=5 COLOR=\"#808080\"><B><I> N/A</I></B></FONT>" ;
		}
		html += "</CENTER>\n" ;

		html += "<CENTER>\n" ;
//		html += "<UL>" ;
			html += HTMLMarkBarFPS(percent, 1.0, fps, refFPS, 3, refAverageFPS) ;	// バーの基準は、章全体の参照環境の平均に合わせる
//		html += "</UL>" ;
		html += "</CENTER>\n" ;

		// さらに内部（ライト１，２，４，８）の詳細情報
		html += "<P>" ;

		html += "<UL>" ;
		{
			for (int i = 0 ; i < 4 ; i ++)
			{
				int n = no * 4 + i ;
				int mode ;
				if (glclockMark.test.lightingMark.mark[n][0].fps >= glclockMark.test.lightingMark.mark[n][1].fps)
					mode = 0 ;
				else
					mode = 1 ;

				int refMode ;
				if (glclockMark.reference.lightingMark.mark[n][0].fps >= glclockMark.reference.lightingMark.mark[n][1].fps)
					refMode = 0 ;
				else
					refMode = 1 ;

				String title = glclockMark.test.lightingMark.mark[n][mode].title ;
				if (title == "")
					title = glclockMark.reference.lightingMark.mark[n][refMode].title ;

				html += HTMLSubMarkBar(title,
									   glclockMark.test.lightingMark.mark[n][mode].extension,
									   glclockMark.reference.lightingMark.mark[n][refMode].extension,
									   TRUE, FALSE,
									   glclockMark.test.lightingMark.mark[n][mode].fps,
									   glclockMark.reference.lightingMark.mark[n][refMode].fps,
									   glclockMark.test.lightingMark.mark[n][mode].kTPS,
									   glclockMark.reference.lightingMark.mark[n][refMode].kTPS,
									   5, refAverageFPS) ;
//				html += HTMLMiniBR() ;
			}
			html += HTMLMiniBR() ;
		}

		html += "</UL>" ;
		html += "<DIV ALIGN=\"RIGHT\">" + HTMLLinkToEachMark(LINK_LIGHTING_MARK, "Return to Per-Vertex Lighting Mark") + "</DIV>" ;
	}

	return html ;
}


String HTMLPerVertexLightedTexturingSubMark(int no, int detailFlag = FALSE)
{
	String html ;
	double fps ;
	double refFPS ;
	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.lightingTexMark.averageFPS ;
	Mark *mark ;
	Mark *refMark ;

	if (no == 0)
	{
		fps = glclockMark.test.lightingTexMark.singleColorFPS ;
		refFPS = glclockMark.reference.lightingTexMark.singleColorFPS ;

		//
		percent = glclockMark.test.lightingTexMark.singleColorPercentage ;
	}
	else
	{
		no = 1 ;
		fps = glclockMark.test.lightingTexMark.separateSpecularFPS ;
		refFPS = glclockMark.reference.lightingTexMark.separateSpecularFPS ;

		//
		percent = glclockMark.test.lightingTexMark.separateSpecularPercentage ;
	}

//	if (refFPS) percent = fps / refFPS ;

	if (!detailFlag)
	{
		html += "<FONT SIZE=4><B>" + HTMLLinkToName(LightingTexMark::HTMLname[no], LightingTexMark::title[no]) + "</B></FONT>\n" ;
		if (fps > 0.0)
			html.AddFormat("<FONT SIZE=4 COLOR=\"#A0FFA0\"><B><I> %9.2f%%</I></B></FONT>", percent * 100.0) ;
		else
			html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A</I></B></FONT>" ;

		html += "<UL>" ;
			html += HTMLMarkBarFPS(percent, 1.0, fps, refFPS, 3, refAverageFPS, LightingTexMark::HTMLname[no]) ;	// バーの基準は、章全体の参照環境の平均に合わせる
		html += "</UL>" ;
	}
	else
	{
		html += "<CENTER>\n" ;
		{
			html += "<FONT SIZE=4><B>" + LightingTexMark::detailTitle[no] + "</B></FONT>\n" ;
			html += HTMLMiniBR() ;
			if (fps > 0.0)
				html.AddFormat("<BR><FONT SIZE=5 COLOR=\"#A0FFA0\"><B><I> %9.2f%%</I></B></FONT>", percent * 100.0) ;
			else
				html += "<BR><FONT SIZE=5 COLOR=\"#808080\"><B><I> N/A</I></B></FONT>" ;
		}
		html += "</CENTER>\n" ;

		html += "<CENTER>\n" ;
//		html += "<UL>" ;
			html += HTMLMarkBarFPS(percent, 1.0, fps, refFPS, 3, refAverageFPS) ;	// バーの基準は、章全体の参照環境の平均に合わせる
//		html += "</UL>" ;
		html += "</CENTER>\n" ;

		// さらに内部（ライトモデル６種類）の詳細情報
		html += "<P>" ;

		html += "<UL>" ;
		{
			for (int i = 0 ; i < 6 ; i ++)
			{

				if (no == 0)
				{
					mark = glclockMark.test.lightingTexMark.singleColorMark[i] ;
					refMark = glclockMark.reference.lightingTexMark.singleColorMark[i] ;
				}
				else
				{
					mark = glclockMark.test.lightingTexMark.separateSpecularMark[i] ;
					refMark = glclockMark.reference.lightingTexMark.separateSpecularMark[i] ;
				}

				int mode ;
				if (mark[0].fps >= mark[1].fps)
					mode = 0 ;
				else
					mode = 1 ;

				int refMode ;
				if (refMark[0].fps >= refMark[1].fps)
					refMode = 0 ;
				else
					refMode = 1 ;

				String title = mark[mode].title ;
				if (title == "")
					title = refMark[refMode].title ;

				html += HTMLSubMarkBar(title,
									   mark[mode].extension,
									   refMark[refMode].extension,
									   TRUE, FALSE,
									   mark[mode].fps,
									   refMark[refMode].fps,
									   mark[mode].kTPS,
									   refMark[refMode].kTPS,
									   5, refAverageFPS) ;
//				html += HTMLMiniBR() ;
			}
			html += HTMLMiniBR() ;
		}
		html += "</UL>" ;
		html += "<DIV ALIGN=\"RIGHT\">" + HTMLLinkToEachMark(LINK_LIGHTED_TEX_MARK, "Return to Per-Vertex Lighted Texturing Mark") + "</DIV>" ;
	}

	return html ;
}

String HTMLMaximumTrianglesSubMark(int no, int detailFlag = FALSE)
{
	String html ;
	double fps ;
	double refFPS ;
	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.maxTriMark.averageKTPS ;
	Mark *mark ;
	Mark *refMark ;

	if (no == 0)
	{
		fps = glclockMark.test.maxTriMark.coloringKTPS ;
		refFPS = glclockMark.reference.maxTriMark.coloringKTPS ;

		//
		percent = glclockMark.test.maxTriMark.coloringPercentage ;
	}
	else
	{
		no = 1 ;
		fps = glclockMark.test.maxTriMark.texturingKTPS ;
		refFPS = glclockMark.reference.maxTriMark.texturingKTPS ;

		//
		percent = glclockMark.test.maxTriMark.texturingPercentage ;
	}

//	if (refFPS) percent = fps / refFPS ;

	if (!detailFlag)
	{
		html += "<FONT SIZE=4><B>" + HTMLLinkToName(MaxTriMark::HTMLname[no], MaxTriMark::title[no]) + "</B></FONT>\n" ;
		if (fps > 0.0)
			html.AddFormat("<FONT SIZE=4 COLOR=\"#A0FFA0\"><B><I> %9.2f%%</I></B></FONT>", percent * 100.0) ;
		else
			html += "<FONT SIZE=4 COLOR=\"#808080\"><B><I> N/A</I></B></FONT>" ;

		html += "<UL>" ;
			html += HTMLMarkBarKTPS(percent, 1.0, fps, refFPS, 3, refAverageFPS, MaxTriMark::HTMLname[no]) ;	// バーの基準は、章全体の参照環境の平均に合わせる
		html += "</UL>" ;
	}
	else
	{
		html += "<CENTER>\n" ;
		{
			html += "<FONT SIZE=4><B>" + MaxTriMark::title[no] + "</B></FONT>\n" ;
			html += HTMLMiniBR() ;
			if (fps > 0.0)
				html.AddFormat("<BR><FONT SIZE=5 COLOR=\"#A0FFA0\"><B><I> %9.2f%%</I></B></FONT>", percent * 100.0) ;
			else
				html += "<BR><FONT SIZE=5 COLOR=\"#808080\"><B><I> N/A</I></B></FONT>" ;
		}
		html += "</CENTER>\n" ;

		html += "<CENTER>\n" ;
		{
//			html += "<UL>" ;
				html += HTMLMarkBarKTPS(percent, 1.0, fps, refFPS, 3, refAverageFPS) ;	// バーの基準は、章全体の参照環境の平均に合わせる
//			html += "</UL>" ;
//			html += HTMLMiniBR() ;
//			html += "<FONT SIZE=4><B>" + MaxTriMark::detailTitle[no] + "</B></FONT>\n" ;
		}
		html += "</CENTER>\n" ;

		// さらに内部（フォグ方程式４種類）の詳細情報
		html += "<P>" ;

		html += "<UL>" ;
		{
			for (int i = 0 ; i < 4 ; i ++)
			{
				if (no == 0)
				{
					mark = glclockMark.test.maxTriMark.coloringMark[i] ;
					refMark = glclockMark.reference.maxTriMark.coloringMark[i] ;
				}
				else
				{
					mark = glclockMark.test.maxTriMark.texturingMark[i] ;
					refMark = glclockMark.reference.maxTriMark.texturingMark[i] ;
				}

				int mode ;
				if (mark[0].kTPS >= mark[1].kTPS)
					mode = 0 ;
				else
					mode = 1 ;

				int refMode ;
				if (refMark[0].kTPS >= refMark[1].kTPS)
					refMode = 0 ;
				else
					refMode = 1 ;

				String title = mark[mode].title ;
				if (title == "")
					title = refMark[refMode].title ;

				html += HTMLSubMarkBar(title,
									   mark[mode].extension,
									   refMark[refMode].extension,
									   FALSE, TRUE,
									   mark[mode].fps,
									   refMark[refMode].fps,
									   mark[mode].kTPS,
									   refMark[refMode].kTPS,
									   5, glclockMark.reference.maxTriMark.averageFPS) ;
//				html += HTMLMiniBR() ;
			}
			html += HTMLMiniBR() ;
		}
		html += "</UL>" ;
		html += "<DIV ALIGN=\"RIGHT\">" + HTMLLinkToEachMark(LINK_MAX_TRIANGLES_MARK, "Return to Maximum Triangles Mark") + "</DIV>" ;
	}

	return html ;
}


String HTMLTextureFilteringSubMark()
{
	String html ;
//	double fps ;
//	double refFPS ;
//	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.filteringMark.averageFPS ;
	Mark *mark ;
	Mark *refMark ;

	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int i = 0 ; i < 5 ; i ++)
		{
			mark = glclockMark.test.filteringMark.mark[i] ;
			refMark = glclockMark.reference.filteringMark.mark[i] ;

			int mode ;
			if (mark[0].fps >= mark[1].fps)
				mode = 0 ;
			else
				mode = 1 ;

			int refMode ;
			if (refMark[0].fps >= refMark[1].fps)
				refMode = 0 ;
			else
				refMode = 1 ;

			String title = mark[mode].title ;
			if (title == "")
				title = refMark[refMode].title ;

			html += HTMLSubMarkBar(title,
								   mark[mode].extension,
								   refMark[refMode].extension,
								   TRUE, FALSE,
								   mark[mode].fps,
								   refMark[refMode].fps,
								   mark[mode].kTPS,
								   refMark[refMode].kTPS,
								   4, refAverageFPS) ;
//			html += HTMLMiniBR() ;
		}
//		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


String HTMLEnvironmentMappingSubMark()
{
	String html ;
//	double fps ;
//	double refFPS ;
//	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.envMapMark.averageFPS ;
	Mark *mark ;
	Mark *refMark ;

	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int i = 0 ; i < 2 ; i ++)
		{
			if (i == 0)
			{
				mark = glclockMark.test.envMapMark.sphereMapMark ;
				refMark = glclockMark.reference.envMapMark.sphereMapMark ;
			}
			else
			{
				mark = glclockMark.test.envMapMark.transSphereMapMark ;
				refMark = glclockMark.reference.envMapMark.transSphereMapMark ;
			}

			int mode ;
			if (mark[0].fps >= mark[1].fps)
				mode = 0 ;
			else
				mode = 1 ;

			int refMode ;
			if (refMark[0].fps >= refMark[1].fps)
				refMode = 0 ;
			else
				refMode = 1 ;

			String title = mark[mode].title ;
			if (title == "")
				title = refMark[refMode].title ;

			html += HTMLSubMarkBar(title,
								   mark[mode].extension,
								   refMark[refMode].extension,
								   TRUE, FALSE,
								   mark[mode].fps,
								   refMark[refMode].fps,
								   mark[mode].kTPS,
								   refMark[refMode].kTPS,
								   4, refAverageFPS) ;
//			html += HTMLMiniBR() ;
		}
//		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


String HTMLSuperSampledBlendingSubMark()
{
	String html ;
//	double fps ;
//	double refFPS ;
//	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.superSampleMark.averageFPS ;
	Mark *mark ;
	Mark *refMark ;

	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int i = 0 ; i < 3 ; i ++)
		{
			mark = glclockMark.test.superSampleMark.mark[i] ;
			refMark = glclockMark.reference.superSampleMark.mark[i] ;

			int mode ;
			if (mark[0].fps >= mark[1].fps)
				mode = 0 ;
			else
				mode = 1 ;

			int refMode ;
			if (refMark[0].fps >= refMark[1].fps)
				refMode = 0 ;
			else
				refMode = 1 ;

			String title = mark[mode].title ;
			if (title == "")
				title = refMark[refMode].title ;

			html += HTMLSubMarkBar(title,
								   mark[mode].extension,
								   refMark[refMode].extension,
								   TRUE, FALSE,
								   mark[mode].fps,
								   refMark[refMode].fps,
								   mark[mode].kTPS,
								   refMark[refMode].kTPS,
								   4, refAverageFPS) ;
//			html += HTMLMiniBR() ;
		}
//		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


String HTMLOpenGLPracticalRenderingSubMark()
{
	String html ;
//	double fps ;
//	double refFPS ;
//	double percent = 0.0 ;
	double refAverageFPS = glclockMark.reference.practicalMark.averageFPS ;
	Mark *mark ;
	Mark *refMark ;

	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int i = 0 ; i < 2 ; i ++)
		{
			mark = glclockMark.test.practicalMark.mark[i] ;
			refMark = glclockMark.reference.practicalMark.mark[i] ;

			int mode ;
			if (mark[0].fps >= mark[1].fps)
				mode = 0 ;
			else
				mode = 1 ;

			int refMode ;
			if (refMark[0].fps >= refMark[1].fps)
				refMode = 0 ;
			else
				refMode = 1 ;

			String title = mark[mode].title ;
			if (title == "")
				title = refMark[refMode].title ;

			html += HTMLSubMarkBar(title,
								   mark[mode].extension,
								   refMark[refMode].extension,
								   TRUE, FALSE,
								   mark[mode].fps,
								   refMark[refMode].fps,
								   mark[mode].kTPS,
								   refMark[refMode].kTPS,
								   4, refAverageFPS) ;
//			html += HTMLMiniBR() ;
		}
//		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


String HTMLChapterMark(double mark)
{
	String html ;

	// ハイスコアの項目はカラーを金色に
	String color = "COLOR=\"#A0FFA0\"" ;
	if (Abs(mark - glclockMark.test.highestSectionMark) < 0.00001)	// 念のため誤差修正
		color = "COLOR=\"#FFFFA0\"" ;

	if (mark > 0.0)
		html.AddFormat("<CENTER><FONT SIZE=6 " + color + "><B><I>%9.2f%%</I></B></FONT></CENTER>", mark * 100.0) ;
	else
		html += "<CENTER><FONT SIZE=6 COLOR=\"#808080\"><B><I> N/A </I></B></FONT></CENTER>" ;

	return html ;
}


// Per-Vertex Lighting Mark
String HTMLPerVertexLightingMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[0]) ;
	html += "<P>" ;
//	html += "<CENTER><FONT SIZE=4><B>Average</B></FONT></CENTER>" ;

	html += HTMLChapterMark(glclockMark.test.lightingMark.lightingMark) ;
//	if (glclockMark.test.lightingMark.lightingMark > 0.0)
//		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%9.2f%%</I></FONT></CENTER>", glclockMark.test.lightingMark.lightingMark * 100.0) ;
//	else
//		html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += HTMLChapterMarkBarFPS(glclockMark.test.lightingMark.lightingMark,
								  glclockMark.reference.lightingMark.lightingMark,
								  glclockMark.test.lightingMark.averageFPS,
								  glclockMark.reference.lightingMark.averageFPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Lighting</B></FONT></CENTER>" ;
	html += "<CENTER><FONT SIZE=3><B>click to go more details</B></FONT></CENTER>" ;
	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int lightMode = 0 ; lightMode < 6 ; lightMode ++)
		{
			html += HTMLPerVertexLightingSubMark(lightMode) ;
		}

		html += HTMLMiniBR() ;
		html += HTMLLinkToMainSectionsAndTop() ;

		html += HTMLSectionHR() ;
//		html += "<CENTER><FONT SIZE=5><B>Each Details</B></FONT></CENTER>" ;
//		html += "<P>\n" ;

		// 更に詳細
		for (int lightMode = 0 ; lightMode < 6 ; lightMode ++)
		{
			if (lightMode != 0)
				html += HTMLMiniHR() ;

			html += HTMLName(glclockMark.test.lightingMark.HTMLname[lightMode]) ;
			html += HTMLPerVertexLightingSubMark(lightMode, TRUE) ;
		}
		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


// Per-Vertex Lighted Texturing Mark
String HTMLPerVertexLightedTexturingMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[1]) ;
	html += "<P>" ;
//	html += "<CENTER><FONT SIZE=4><B>Average</B></FONT></CENTER>" ;

	html += HTMLChapterMark(glclockMark.test.lightingTexMark.lightingTexMark) ;

	html += HTMLChapterMarkBarFPS(glclockMark.test.lightingTexMark.lightingTexMark,
								  glclockMark.reference.lightingTexMark.lightingTexMark,
								  glclockMark.test.lightingTexMark.averageFPS,
								  glclockMark.reference.lightingTexMark.averageFPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Texture Blending</B></FONT></CENTER>" ;
	html += "<CENTER><FONT SIZE=3><B>click to go more details</B></FONT></CENTER>" ;
	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int lightMode = 0 ; lightMode < 2 ; lightMode ++)
		{
			html += HTMLPerVertexLightedTexturingSubMark(lightMode) ;
		}

		html += HTMLMiniBR() ;
		html += HTMLLinkToMainSectionsAndTop() ;

		html += HTMLSectionHR() ;
//		html += "<CENTER><FONT SIZE=5><B>Each Details</B></FONT></CENTER>" ;
//		html += "<P>\n" ;

		// 更に詳細
		for (int lightMode = 0 ; lightMode < 2 ; lightMode ++)
		{
			if (lightMode != 0)
				html += HTMLMiniHR() ;

			html += HTMLName(glclockMark.test.lightingTexMark.HTMLname[lightMode]) ;
			html += HTMLPerVertexLightedTexturingSubMark(lightMode, TRUE) ;
		}
		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


// Maximum Triangles Mark
String HTMLMaximumTrianglesMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[5]) ;
	html += "<P>" ;
//	html += "<CENTER><FONT SIZE=4><B>Average</B></FONT></CENTER>" ;

	html += HTMLChapterMark(glclockMark.test.maxTriMark.maxTriMark) ;
//	if (glclockMark.test.maxTriMark.maxTriMark > 0.0)
//		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%9.2f%%</I></FONT></CENTER>", glclockMark.test.maxTriMark.maxTriMark * 100.0) ;
//	else
//		html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += HTMLChapterMarkBarKTPS(glclockMark.test.maxTriMark.maxTriMark,
								   glclockMark.reference.maxTriMark.maxTriMark,
								   glclockMark.test.maxTriMark.averageKTPS,
								   glclockMark.reference.maxTriMark.averageKTPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Coloring</B></FONT></CENTER>" ;
	html += "<CENTER><FONT SIZE=3><B>click to go more details</B></FONT></CENTER>" ;
	html += "<P>" ;

	html += "<UL>" ;
	{
		for (int renderMode = 0 ; renderMode < 2 ; renderMode ++)
		{
			html += HTMLMaximumTrianglesSubMark(renderMode) ;
//			if (renderMode < 1)
		}
		html += HTMLMiniBR() ;
		html += HTMLLinkToMainSectionsAndTop() ;

		html += HTMLSectionHR() ;
//		html += "<CENTER><FONT SIZE=5><B>Each Details</B></FONT></CENTER>" ;
//		html += "<P>\n" ;

		// 更に詳細
		for (int renderMode = 0 ; renderMode < 2 ; renderMode ++)
		{
			if (renderMode != 0)
				html += HTMLMiniHR() ;

			html += HTMLName(glclockMark.test.maxTriMark.HTMLname[renderMode]) ;
			html += HTMLMaximumTrianglesSubMark(renderMode, TRUE) ;
//			if (renderMode < 1)
//				html += HTMLMiniBR() ;
		}
		html += HTMLMiniBR() ;
	}
	html += "</UL>" ;

	return html ;
}


// Texture Filtering Mark
String HTMLTextureFilteringMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[2]) ;
	html += "<P>" ;
//	html += "<CENTER><FONT SIZE=4><B>Average</B></FONT></CENTER>" ;

	html += HTMLChapterMark(glclockMark.test.filteringMark.filteringMark) ;
//	if (glclockMark.test.filteringMark.filteringMark > 0.0)
//		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%9.2f%%</I></FONT></CENTER>", glclockMark.test.filteringMark.filteringMark * 100.0) ;
//	else
//		html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += HTMLChapterMarkBarFPS(glclockMark.test.filteringMark.filteringMark,
								  glclockMark.reference.filteringMark.filteringMark,
								  glclockMark.test.filteringMark.averageFPS,
								  glclockMark.reference.filteringMark.averageFPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Filtering</B></FONT></CENTER>" ;

	html += "<P>" ;
	html += HTMLTextureFilteringSubMark() ;

	return html ;
}


// Environment Mapping Mark
String HTMLEnvironmentMappingMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[3]) ;
	html += "<CENTER><FONT SIZE=5><B>- Per-Texel Specular Lighting -</B></FONT></CENTER>" ;
//	html += "<CENTER><FONT SIZE=5><B>- Dynamical Texture Coordinates Generation -</B></FONT></CENTER>" ;
	html += "<P>" ;
//	html += "<CENTER><FONT SIZE=4><B>Average</B></FONT></CENTER>" ;

	html += HTMLChapterMark(glclockMark.test.envMapMark.envMapMark) ;
//	if (glclockMark.test.envMapMark.envMapMark > 0.0)
//		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%9.2f%%</I></FONT></CENTER>", glclockMark.test.envMapMark.envMapMark * 100.0) ;
//	else
//		html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += HTMLChapterMarkBarFPS(glclockMark.test.envMapMark.envMapMark,
								  glclockMark.reference.envMapMark.envMapMark,
								  glclockMark.test.envMapMark.averageFPS,
								  glclockMark.reference.envMapMark.averageFPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Rendering</B></FONT></CENTER>" ;

	html += "<P>" ;
	html += HTMLEnvironmentMappingSubMark() ;

	return html ;
}


// Super Sampled Blending Mark
String HTMLSuperSampledBlendingMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[4]) ;
	html += "<CENTER><FONT SIZE=5><B>- without Accum-Buffer, Low Detail -</B></FONT></CENTER>" ;
	html += "<P>" ;

	html += HTMLChapterMark(glclockMark.test.superSampleMark.superSampleMark) ;
//	if (glclockMark.test.superSampleMark.superSampleMark > 0.0)
//		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%9.2f%%</I></FONT></CENTER>", glclockMark.test.superSampleMark.superSampleMark * 100.0) ;
//	else
//		html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += HTMLChapterMarkBarFPS(glclockMark.test.superSampleMark.superSampleMark,
								  glclockMark.reference.superSampleMark.superSampleMark,
								  glclockMark.test.superSampleMark.averageFPS,
								  glclockMark.reference.superSampleMark.averageFPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Sampling</B></FONT></CENTER>" ;

	html += "<P>" ;
	html += HTMLSuperSampledBlendingSubMark() ;

	return html ;
}


// OpenGL Practical Rendering Mark
String HTMLOpenGLPracticalRenderingMark()
{
	String html ;

	html += HTMLChapterTitle(chapterName[6]) ;
	html += "<P>" ;
//	html += "<CENTER><FONT SIZE=4><B>Average</B></FONT></CENTER>" ;

	html += HTMLChapterMark(glclockMark.test.practicalMark.practicalMark) ;
//	if (glclockMark.test.practicalMark.practicalMark > 0.0)
//		html.AddFormat("<CENTER><FONT SIZE=6 COLOR=\"#A0FFA0\"><I>%9.2f%%</I></FONT></CENTER>", glclockMark.test.practicalMark.practicalMark * 100.0) ;
//	else
//		html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><I> N/A </I></FONT></CENTER>" ;

	html += HTMLChapterMarkBarFPS(glclockMark.test.practicalMark.practicalMark,
								  glclockMark.reference.practicalMark.practicalMark,
								  glclockMark.test.practicalMark.averageFPS,
								  glclockMark.reference.practicalMark.averageFPS) ;
	html += "<P>" ;
	html += "<CENTER><FONT SIZE=4><B>Difference of Complexity</B></FONT></CENTER>" ;

	html += "<P>" ;
	html += HTMLOpenGLPracticalRenderingSubMark() ;

	return html ;
}


// Other Comparison
String HTMLOtherComparison()
{
	String html ;

	html += HTMLChapterTitle(chapterName[7]) ;
	html += "<P>" ;

	return html ;
}


String HTMLFormat()
{
	String html ;

//	html = "Content-type: text/html\n" ;

	// ハイスコアフラグセット
	flag_highestMark = TRUE ;
	flag_highestFPS  = TRUE ;
	flag_highestKTPS = TRUE ;

	// ログファイルからの内容に従ってタイトルを再設定
	if (glclockMark.test.infoColorBuffer.Search(VIEWPORT_VGA))
		windowMode = "VGA" ;
	else if (glclockMark.test.infoColorBuffer.Search(VIEWPORT_SVGA))
		windowMode = "SVGA" ;
	else if (glclockMark.test.infoColorBuffer.Search(VIEWPORT_XGA))
		windowMode = "XGA" ;
	else	// if (glclockMark.test.infoColorBuffer.Search(VIEWPORT_DEFAULT))
		windowMode = "" ;

	if (glclockMark.test.infoGlClock == "")
		title = "glclock" ;
	else
		title = glclockMark.test.infoGlClock ;

	title += String(" ") + windowMode + " Mark Results" ;

	html += "<HTML>\n" ;
	{
		html += "<HEAD>\n" ;
		{
			html += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=iso-8859-1\">\n" ;
//			html += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html\">\n" ;
			html += String("<TITLE>") + title + "</TITLE>\n" ;
		}
		html += "</HEAD>\n" ;

		html += String("<BODY TEXT=\"#E0E0E0\"\n") +
				       "      BACKGROUND=\"background.gif\"\n" +
				       "      BGCOLOR=\"#191919\"\n" +
					   "      LINK=\"#B0B0FF\"\n" +
					   "      VLINK=\"#8080F0\"\n" +
					   "      ALINK=\"#0000FF\">\n" ;

		html += "<FONT SIZE=3>\n" ;
		{
			// トップリンク用ネーム
			html += HTMLName(LINK_TOP) ;

			// タイトル
			html += HTMLLargeTitle(title) ;
//			html += "<P>" ;

			if (glclockMark.test.infoColorBuffer != "")
			{
				html += "<CENTER><FONT SIZE=4><B>\n" ;
				{
					html += glclockMark.test.infoColorBuffer ;
				}
				html += "</B></FONT></CENTER>\n" ;
			}

/*
			html += HTMLMiniBR() ;

			html += "<CENTER><FONT SIZE=4><B>\n" ;
			{
				html += "OpenGL-" + glclockMark.test.infoOpenGLVersion + "<BR>\n" ;
				html += glclockMark.test.infoOpenGLRenderer + "<BR>\n" ;
				html += glclockMark.test.infoOpenGLVender + "<BR>\n" ;
			}
			html += "</B></FONT></CENTER>\n" ;
*/

//			html += "<P>" ;
			html += HTMLMiniBR() ;

//			html += "<CENTER><FONT SIZE=4><B>Overall Mark</B></FONT></CENTER>" ;
//			html += HTMLMiniBR() ;

			// glclock Mark(%)
			if (glclockMark.test.glclockMark > 0.0)
				html.AddFormat("<CENTER><FONT SIZE=7 COLOR=\"#A0FFA0\"><B><I>%9.2f%%</I></B></FONT></CENTER>", glclockMark.test.glclockMark * 100.0) ;
			else
				html += "<CENTER><FONT SIZE=7 COLOR=\"#808080\"><B><I> N/A </I></B></FONT></CENTER>" ;

			// バーの説明
/*
			html.AddFormat("<TABLE>\n") ;
			{
				html.AddFormat("<TR>\n") ;
				html.AddFormat("<TD>" + HTMLBar(BAR_FILE_GREEN, 80, 6) + "</TD>\n") ;
				html.AddFormat(String("<TD><FONT SIZE=-1 COLOR=\"#A0FFA0\">") + specTestEnv + "</FONT></TD>\n") ;
				html.AddFormat("</TR>\n") ;

				html.AddFormat("<TR>\n") ;
				html.AddFormat("<TD>" + HTMLBar(BAR_FILE_RED, 80, 6) + "</TD>\n") ;
				html.AddFormat(String("<TD><FONT SIZE=-1 COLOR=\"#FFA0A0\">") + specRefEnv + "</FONT></TD>\n") ;
				html.AddFormat("</TR>\n") ;
			}
			html.AddFormat("</TABLE>\n") ;
*/

			// glclock Mark グラフ表示
//			html += "<BR>\n" ;
			html += HTMLglclockMark() ;

			html += HTMLMiniBR() ;

			// ハイスコア表示
			html += HTMLHighestScores() ;

			// OpenGL Infos
			if (glclockMark.test.infoOpenGLVersion != "")
			{
				html += HTMLMiniBR() ;
				html += "<CENTER><FONT SIZE=4><B>\n" ;
				{
					html += "OpenGL-" + glclockMark.test.infoOpenGLVersion + " (" + glclockMark.test.infoOpenGLVender + ")<BR>\n" ;
					html += glclockMark.test.infoOpenGLRenderer + "<BR>\n" ;
				}
				html += "</B></FONT></CENTER>\n" ;
//				html += HTMLMiniBR() ;
			}

//			html += "<P>" ;
			html += HTMLMiniBR() ;
			html += "<CENTER>" ;
				html += HTMLMiniLinkToName(LINK_DETAILS, TITLE_CONTENTS) + HTMLMiniLinkToName(LINK_ENVIRONMENT, CHAPTER_OPEN_GL_INFO) ;
				html += HTMLLinkToMasaHP() + HTMLLinkToGLClockMarkResultsDatabase() + HTMLMailToMasa() ;
			html += "</CENTER>" ;

			html += HTMLSectionHR() ;

			// Main Sections
			html += HTMLName(LINK_DETAILS) ;
			html += HTMLDetails() ;
			html += "<P ALIGN=\"RIGHT\">" + HTMLMiniLinkToName(LINK_TOP, "Return to the Top") + "</P>" ;

			html += HTMLChapterHR() ;

			// Per-Vertex Lighting Mark
			html += HTMLName(LINK_LIGHTING_MARK) ;
			html += HTMLPerVertexLightingMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// Per-VertexLighted Texturing Mark
			html += HTMLName(LINK_LIGHTED_TEX_MARK) ;
			html += HTMLPerVertexLightedTexturingMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// Texture Filtering Mark
			html += HTMLName(LINK_FILTERING_MARK) ;
			html += HTMLTextureFilteringMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// Environment Mapping Mark
			html += HTMLName(LINK_ENV_MAPPING_MARK) ;
			html += HTMLEnvironmentMappingMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// Super Sampled Blending Mark
			html += HTMLName(LINK_SUPER_SAMPLE_MARK) ;
			html += HTMLSuperSampledBlendingMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// Maximum Triangles Mark
			html += HTMLName(LINK_MAX_TRIANGLES_MARK) ;
			html += HTMLMaximumTrianglesMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// OpenGL Practical Rendering Mark
			html += HTMLName(LINK_PRACTICAL_MARK) ;
			html += HTMLOpenGLPracticalRenderingMark() ;
			html += HTMLLinkToMainSectionsAndTop() ;

			html += HTMLChapterHR() ;

			// Tested Environment
			html += HTMLName(LINK_ENVIRONMENT) ;
			html += HTMLChapterTitle(CHAPTER_OPEN_GL_INFO) ;
			html += "<FONT SIZE=2><B><PRE>\n" + glclockMark.test.infoOpenGL + "\n</PRE></B></FONT>" ;
			html += "<CENTER>" ;
				html += HTMLMiniLinkToName(LINK_TOP, "Return to the Top") + HTMLLinkToMasaHP()
					  + HTMLLinkToGLClockMarkResultsDatabase() + HTMLMailToMasa() ;
			html += "</CENTER>\n" ;

			// オリジナルのログを最後に追加（コメント形式）
			html += "\n\n\n<!--\n\n---- Original Log Start ----\n\n" ;
			html += glclockMark.test.originalLog ;
			html += "\n\n---- Original Log End ----\n\n-->\n\n\n" ;
		}
		html += "</FONT>\n" ;
		html += "</BODY>\n" ;
	}
	html.AddFormat("</HTML>\n") ;

	return html ;
}


// 成功時：TRUE
// エラー：FALSE
int FormatProc()
{
	int ret ;

	// ログファイルから全項目のテスト結果をテーブルにセットし、合計値などを計算
#ifndef CGI_MODE
	logprintf(String("\nGenerating \"") + file_HTML + "\"\n") ;

#else
	logprintf("Content-type: text/html\n") ;
	logprintf(String(CGI_LAST_MODIFIED) + "\n") ;
	logprintf("\n") ;

	file_LOG = CGI_PATH + file_LOG ;
	file_REF_LOG = CGI_PATH + file_REF_LOG ;

#ifdef DEBUG_CGI_MODE

	debugCGI += "file_LOG: \"" + file_LOG + "\"\n" ;
	debugCGI += "file_REF_LOG: \"" + file_REF_LOG + "\"\n" ;
	logprintf(debugCGI) ;

#endif	// #ifdef DEBUG_CGI_MODE

#endif	// #ifndef CGI_MODE

	ret = glclockMark.SetupMark(file_LOG, file_REF_LOG) ;
	if (!ret)
		return FALSE ;	// エラー

	// HTML 出力ファイルオープン
	FILE *fpHTML ;

#ifdef CGI_MODE
	fpHTML = stdout ;

#else	// #ifdef CGI_MODE

	fpHTML = fopen(file_HTML, "w") ;
	if (!fpHTML)
	{
		// オープンエラー
		logprintf("Failed to open log file " + file_HTML + ".\n") ;
#ifdef WIN32
		MessageBox(NULL, "Failed to open log file " + file_HTML + ".", _T("Genrate HTML file error"), MB_OK | MB_ICONSTOP) ;
#endif
		return FALSE ;
	}

#endif	// #ifdef CGI_MODE ... #else

	// HTML 形式で結果を出力
	fputs(HTMLFormat(), fpHTML) ;

	// HTML 出力ファイルクローズ
#ifndef CGI_MODE
	fclose(fpHTML) ;
#endif

#ifndef CGI_MODE
#ifdef WIN32
	// Windows ではブラウザで起動
	if (!donotShowResults)
		SimpleShellExecuteOpen(file_HTML) ;
#endif
#endif	// #ifndef CGI_MODE

	return TRUE ;
}

int EndProc()
{
	return EXIT_SUCCESS ;
}

int MainProc(int argc, char** argv)
{
	Option option(argc, argv) ;

	Initialize(option) ;

#ifndef CGI_MODE
	if (!onlyFormat)
		ExecMarks() ;
#endif

	int ret ;
	ret = FormatProc() ;

	if (!ret)
		return EXIT_FAILURE ;

	return EndProc() ;
}


int main(int argc, char** argv)
{
#if defined WIN32 && defined _DEBUG
	int newFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) ;
	newFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF ;
//	_CrtSetDbgFlag(newFlag) ;
//	_ASSERTE(_CrtCheckMemory()) ;
#endif

	int ret ;

#ifdef macintosh
	// argument を獲得 (mac では main からは何もやってこない）
	getOption(&argc, &argv);
#endif	// #ifdef macintosh

	ret = MainProc(argc, argv) ;

#ifdef OVERRIDE_NEW_OPERATOR
	MemDebugFinal() ;
#endif	// #ifdef OVERRIDE_NEW_OPERATOR

	exit(ret) ;
	return ret ;
}


void OverAllMark::SetOpenGLInformations()
{
	int start ;

	// infoOpenGL 中の "<" ">" を "[" "]" に変換
	for (int c = 1 ; c <= infoOpenGL.Length() ; c ++)
	{
		if      (infoOpenGL.Character(c) == '<') infoOpenGL.SetAt(c, '[') ;
		else if (infoOpenGL.Character(c) == '>') infoOpenGL.SetAt(c, ']') ;
	}

	// infoOpenGL 中、"glclock-" にヒットする文字から行末まで（数値の間）
	infoGlClock = "" ;
	start = infoOpenGL.Search("glclock-") ;
	if (start)
	{
		while (start < infoOpenGL.Length() && infoOpenGL.Character(start) != '\n')
		{
			infoGlClock += infoOpenGL.Character(start) ;
			start ++ ;
		}
	}

	// infoOpenGL 中、"Color buffer" にヒットする文字から行末まで
	infoColorBuffer = "" ;
	start = infoOpenGL.Search("Color buffer") ;
	if (start)
	{
		while (start < infoOpenGL.Length() && infoOpenGL.Character(start) != '\n')
		{
			infoColorBuffer += infoOpenGL.Character(start) ;
			start ++ ;
		}

		// 最初の部分をカット
		start = infoColorBuffer.Search(":") + 1 ;
		while (start <= infoColorBuffer.Length() && infoColorBuffer.Character(start) == ' ') start ++  ;	// 最初の空白をカット
		if (start <= infoColorBuffer.Length())
			infoColorBuffer.Right(infoColorBuffer.Length() + 1 - start) ;
		else
			infoColorBuffer = "" ;
	}

	// infoOpenGL 中、"Timer resolution" にヒットする文字から行末まで
	infoTimerResolution = "" ;
	start = infoOpenGL.Search("Timer resolution") ;
	if (start)
	{
		while (start < infoOpenGL.Length() && infoOpenGL.Character(start) != '\n')
		{
			infoTimerResolution += infoOpenGL.Character(start) ;
			start ++ ;
		}

		// 最初の部分をカット
		start = infoTimerResolution.Search(":") + 1 ;
		while (start <= infoTimerResolution.Length() && infoTimerResolution.Character(start) == ' ') start ++  ;	// 最初の空白をカット
		if (start <= infoTimerResolution.Length())
			infoTimerResolution.Right(infoTimerResolution.Length() + 1 - start) ;
		else
			infoTimerResolution = "" ;
	}


	// OpenGL Server Informations

	// infoOpenGL 中、"Vendor" にヒットする文字から行末まで
	// ただし、"Vendor : " 部分はカット
	infoOpenGLVender = "" ;
	start = infoOpenGL.Search("Vendor") ;
	if (start)
	{
		while (start < infoOpenGL.Length() && infoOpenGL.Character(start) != '\n')
		{
			infoOpenGLVender += infoOpenGL.Character(start) ;
			start ++ ;
		}

		// 最初の部分をカット
		start = infoOpenGLVender.Search(":") + 1 ;
		while (start <= infoOpenGLVender.Length() && infoOpenGLVender.Character(start) == ' ') start ++  ;	// 最初の空白をカット
		if (start <= infoOpenGLVender.Length())
			infoOpenGLVender.Right(infoOpenGLVender.Length() + 1 - start) ;
		else
			infoOpenGLVender = "" ;
	}

	// infoOpenGL 中、"Renderer" にヒットする文字から行末まで
	infoOpenGLRenderer = "" ;
	start = infoOpenGL.Search("Renderer") ;
	if (start)
	{
		while (start < infoOpenGL.Length() && infoOpenGL.Character(start) != '\n')
		{
			infoOpenGLRenderer += infoOpenGL.Character(start) ;
			start ++ ;
		}

		// 最初の部分をカット
		start = infoOpenGLRenderer.Search(":") + 1 ;
		while (start <= infoOpenGLRenderer.Length() && infoOpenGLRenderer.Character(start) == ' ') start ++ ;	// 最初の空白をカット
		if (start <= infoOpenGLRenderer.Length())
			infoOpenGLRenderer.Right(infoOpenGLRenderer.Length() + 1 - start) ;
		else
			infoOpenGLRenderer = "" ;
	}

	// infoOpenGL 中、"Version" にヒットする文字から行末まで
	infoOpenGLVersion = "" ;
	start = infoOpenGL.Search("Version") ;
	if (start)
	{
		while (start < infoOpenGL.Length() && infoOpenGL.Character(start) != '\n')
		{
			infoOpenGLVersion += infoOpenGL.Character(start) ;
			start ++ ;
		}

		// 最初の部分をカット
		start = infoOpenGLVersion.Search(":") + 1 ;
		while (start <= infoOpenGLVersion.Length() && infoOpenGLVersion.Character(start) == ' ') start ++  ;	// 最初の空白をカット
		if (start <= infoOpenGLVersion.Length())
			infoOpenGLVersion.Right(infoOpenGLVersion.Length() + 1 - start) ;
		else
			infoOpenGLVersion = "" ;
	}
}


// ログファイルから全項目のテスト結果をバッファにロード
// さらに各々の詳細データを分離してテーブルにセット
//
// 成功時：テーブル要素数
// エラー：-1
int OverAllMark::SetTableFromLogFile(const String& logFile)
{
	MarkLogTable markLogTable ;

	int ret ;
	ret = LoadLogFile(logFile, markLogTable) ;
	if (!ret)
		return -1 ;	// エラー

	// originalLog にすべて保存
	originalLog = "" ;
	for (int l = 0 ; l < markLogTable.nLine ; l ++)
		originalLog += markLogTable.line[l] ;

	// markLogTable の要素のうち、'#' で始まる最初の要素の前までが infoOpenGL
	int glend = 0 ;
	infoOpenGL = "" ;

	for (glend = 0 ; glend < markLogTable.nLine ; glend ++)
	{
		if (markLogTable.line[glend].Character(1) == '#')
			break ;

		infoOpenGL += markLogTable.line[glend] ;
	}

	// glend は OpenGL Info の最後（テストの最初の行）

	SetOpenGLInformations() ;

	// テストデータテーブル作成
	markTable.Clear() ;	// 一度すべてクリア

	int nMark = 0 ;
	while (glend < markLogTable.nLine - 2)
	{
		Mark tmp ;
		tmp.Set(markLogTable.line[glend], markLogTable.line[glend + 1], markLogTable.line[glend + 2]) ;
		nMark = markTable.AddMark(tmp) ;

		// 次のデータを検索
		glend += 2 ;	// データの入っていた分２行進める
		for ( ; glend < markLogTable.nLine ; glend ++)
		{
			if (markLogTable.line[glend].Character(1) == '#')
				break ;
		}

		// 次のデータがなければ終了
		if (glend >= markLogTable.nLine)
			break ;
	}

	return nMark ;
}


// ログファイルから全項目のテスト結果をバッファにロード
// さらに各々の詳細データを分離してテーブルにセット
// 平均 FPS 等をセット
//
// 成功時：テーブル要素数
// エラー：-1
int OverAllMark::SetupMark(const String& logFile)
{
	int nMark ;
	nMark = SetTableFromLogFile(logFile) ;

	if (nMark == -1)
		return nMark ;	// エラー

	SetupFromMarkTable() ;

	return nMark ;
}


int GlClockMark::SetupMark(const String& testLog, const String& referenceLog)
{
	int ret ;

	// ログファイルから結果をセット
	ret = test.SetupMark(testLog) ;	// テスト環境
//	if (ret < 0)
//		return FALSE ;	// エラー

	ret = reference.SetupMark(referenceLog) ;	// 参照環境
	if (ret < 0)
		return FALSE ;	// エラー

	// テスト環境および参照環境の値から glclock Mark 値を計算
	test.CalculateMark(reference) ;
	reference.CalculateReferenceMark() ;

	// グラフのバーの長さを計算
	if (test.glclockMark > 1.0f)
	{
//		double abs = log(4.0) / log(test.glclockMark * 4.0) ;
		double abs = log(test.glclockMark * 4.0) / log(4.0) ;
		barLength = abs / test.glclockMark ;
	}
	else
		barLength = 1.0 ;

	return TRUE ;
}


String LightingMark::title[6] =
{
	"Directional Light(s).",
	"Directional Light(s) with local Viewer.",
	"Point Light(s).",
	"Point Light(s) with local Viewer.",
	"Spot Light(s).",
	"Spot Light(s) with local Viewer.",
} ;

String LightingMark::HTMLname[6] =
{
	"LightingDir",
	"LightingDirPos",
	"LightingPoint",
	"LightingPointPos",
	"LightingSpot",
	"LightingSpotPos",
} ;

String LightingTexMark::title[2] =
{
	"Single Color Blending.",
	"Separate Specular Color Blending.",
} ;

String LightingTexMark::detailTitle[2] =
{
	"Single Color Blending.<BR>(Diffuse + Specular) * Texture",
	"Separate Specular Color Blending.<BR>Diffuse * Texture + Specular",
} ;

String LightingTexMark::HTMLname[2] =
{
	"LightTexSingle",
	"LightTexSeparate",
} ;

String MaxTriMark::title[2] =
{
	"Simple Coloring.",
	"Modulate Texture Mapping.",
} ;

String MaxTriMark::detailTitle[2] =
{
	"Difference of Fog Equation",
	"Difference of Fog Equation",
} ;

String MaxTriMark::HTMLname[2] =
{
	"MaxTriColor",
	"MaxTriTexture",
} ;


double LightingMark::weight_nLight[LightingMark::NM_WEIGHT_N_LIGHT] =
{
#ifdef ENABLE_SCORE_WEIGHT
	1.0,	// 1 light
	2.0,	// 2 lights
	3.0,	// 4 lights
	4.0,	// 8 lights
#else
	1.0, 1.0, 1.0, 1.0,
#endif
} ;


double LightingMark::weight_lightModel[LightingMark::NM_WEIGHT_LIGHT_MODEL] =
{
#ifdef ENABLE_SCORE_WEIGHT
	2.0,	// Directional Lights
	3.0,	// Directional Lights with Local Viewer
	4.0,	// Point Lights
	4.0,	// Point Lights with Local Viewer
	4.0,	// Spot Lights
	4.0,	// Spot Lights with Local Viewer
#else
	1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
#endif
} ;


double LightingTexMark::weight_lightModel[LightingTexMark::NM_WEIGHT_LIGHT_MODEL] =
{
#ifdef ENABLE_SCORE_WEIGHT
	2.0,	// Directional Lights
	3.0,	// Directional Lights with Local Viewer
	4.0,	// Point Lights
	4.0,	// Point Lights with Local Viewer
	4.0,	// Spot Lights
	4.0,	// Spot Lights with Local Viewer
#else
	1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
#endif
} ;

double LightingTexMark::weight_texBlend[LightingTexMark::NM_WEIGHT_TEX_BLEND] =
{
#ifdef ENABLE_SCORE_WEIGHT
	2.0,	// Single Color
	3.0,	// Separate Specular Color
#else
	1.0, 1.0,
#endif
} ;


double FilteringMark::weight_filter[FilteringMark::NM_WEIGHT_FILTER] =
{
#ifdef ENABLE_SCORE_WEIGHT
	1.0,	// Point Sampling
	2.0,	// Mip-Mapping
	2.0,	// Bi-Linear Filtering
	4.0,	// Bi-Linear Filtering with Mip-Mapping
	5.0,	// Tri-Linear Filtering
#else
	1.0, 1.0, 1.0, 1.0, 1.0,
#endif
} ;


double EnvMapMark::weight_render[EnvMapMark::NM_WEIGHT_RENDER] =
{
#ifdef ENABLE_SCORE_WEIGHT
	1.0,	// Sphere-Mapping
	1.0,	// 2-Sided Transparency Sphere-Mapping
#else
	1.0, 1.0,
#endif
} ;


double SuperSampleMark::weight_sample[SuperSampleMark::NM_WEIGHT_SAMPLE] =
{
#ifdef ENABLE_SCORE_WEIGHT
	3.0,	// Additive-Blending (4-pass)
	7.0,	// Alpha-Blending (7-pass)
	13.0,	// Alpha-Blending (13-pass)
#else
	1.0, 1.0, 1.0,
#endif
} ;


double MaxTriMark::weight_fog[MaxTriMark::NM_WEIGHT_FOG] =
{
#ifdef ENABLE_SCORE_WEIGHT
	2.0,	// None
	3.0,	// Linear Fog
	4.0,	// Exponential Fog
	4.0,	// Exponential^2 Fog
#else
	1.0, 1.0, 1.0, 1.0,
#endif
} ;

double MaxTriMark::weight_color[MaxTriMark::NM_WEIGHT_COLOR] =
{
#ifdef ENABLE_SCORE_WEIGHT
	2.0,	// Coloring
	3.0,	// Modulate Texturing
#else
	1.0, 1.0,
#endif
} ;


double PracticalMark::weight_complex[PracticalMark::NM_WEIGHT_COMPLEX] =
{
#ifdef ENABLE_SCORE_WEIGHT
	1.0,	// Simple
	3.0,	// Complex
#else
	1.0, 1.0,
#endif
} ;


double OverAllMark::weight_mark[OverAllMark::NM_WEIGHT_MARK] =
{
#ifdef ENABLE_SCORE_WEIGHT
	1.0,	// Lighting Mark
	1.0,	// Lighted Texturing Mark
	1.0,	// Filtering Mark
	1.0,	// Environment Mapping Mark
	1.0,	// Super Sampling Mark
	1.0,	// Maxmum Triangles Mark
	1.0,	// Practical Rendering Mark
#else
	1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
#endif
} ;
