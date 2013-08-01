
#include "glclock.H"


#ifdef WIN32
#include <crtdbg.h>
#endif


// メイン
#if !defined WIN32 || defined _CONSOLE

int main(int argc, char** argv)
{
#ifdef WIN32

#ifdef _DEBUG
	int newFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) ;
	newFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF ;
	_CrtSetDbgFlag(newFlag) ;
	_ASSERTE(_CrtCheckMemory()) ;
#endif

	// スクリーンセーバモードで起動中のものがあれば
	// その場で終了
	HWND win ;
	win = FindWindow("GLUT", "glclock screen saver") ;
	if (win) return EXIT_SUCCESS ;
#endif

#ifdef macintosh
	// argument を獲得 (mac では main からは何もやってこない）
	getOption(&argc, &argv);
	SetCursor(*GetCursor(watchCursor)) ;	// 時計カーソルに変更
#endif

	int ret = glclock(argc, argv) ;
	return ret ;


// command option test...
/*
	String option ;
	if (argc > 1) option = argv[1] ;
	for (int i = 2 ; i < argc ; i ++)
		option += String(" ") + argv[i] ;

	int ret = glclock("-sign \"\" -TTTT") ;
	return ret ;
*/
}


#else
// Windows アプリの場合


// Windows メイン
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	int newFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) ;
	newFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF ; // | _CRTDBG_DELAY_FREE_MEM_DF ;
	_CrtSetDbgFlag(newFlag) ;
	_ASSERTE(_CrtCheckMemory()) ;
#endif

	// スクリーンセーバモードで起動中のものがあれば
	// その場で終了
	HWND win ;
	win = FindWindow("GLUT", "glclock screen saver") ;
	if (win) return EXIT_FAILURE ;

	hInstanceGlClock = hInstance ;

	// 実行ファイルパスを \Windows\System\glclock.ini ファイルに保存
	String glclockExePath, glclockIniPath ;
	glclockExePath = String(__argv[0]) ;

	char buf[MAX_PATH + 1] ;
	int len ;
	len = GetSystemDirectory(buf, MAX_PATH) ;
	if (len)
		glclockIniPath = String(buf) + '\\' + GLCLOCK_INI ;

	FILE *fpGlClockIni = fopen(glclockIniPath, "w") ;
	if (fpGlClockIni)
	{
//		fprintf(fpGlClockIni, "%s\n", (char *)glclockExePath) ;
		fprintf(fpGlClockIni, glclockExePath) ;
		fclose(fpGlClockIni) ;
	}


	int ret ;

#ifndef GLCLOCK_DLL_EXPORT

	// dll を使わない場合
	ret = glclock(__argc, __argv) ;

	if (ret)
		ret = FALSE ;
	else
		ret = TRUE ;

#else
	// glclock.dll に明示的にリンクする使用する場合

	// glclock.dll から glclock() をリンク
	// 関数アドレスをゲットできた場合 glclock コール

	HINSTANCE hLib_glclock = NULL ;
	hLib_glclock = LoadLibrary(_T(GLCLOCK_DLL)) ;
	if (hLib_glclock)
	{
		// DLL ロードに成功したら
		// glclock() の エントリポイントを取得
		PFNGLCLOCKARGPROC pglclock_arg ;
		pglclock_arg = (PFNGLCLOCKARGPROC)GetProcAddress(hLib_glclock, _T("_glclock_arg@8")) ;

		if (!pglclock_arg)
		{
			MessageBox(NULL, _T("Failed to get glclock_arg entry point"), _T("GetProcAddress Error"), MB_OK | MB_ICONSTOP) ;
			ret = EXIT_FAILURE ;
		}
		else
		{
#define glclock_arg (*pglclock_arg)
			ret = glclock_arg(__argc, __argv) ;
#undef glclock_arg
		}

		// DLL を開放
		if (hLib_glclock)
			FreeLibrary(hLib_glclock) ;

		if (ret)
			ret = FALSE ;
		else
			ret = TRUE ;
	}
	else
	{
		// DLL のロードに失敗
		MessageBox(NULL, _T("Failed to load glclock.dll"), _T("LoadLibrary Error"), MB_OK | MB_ICONSTOP) ;
		ret = FALSE ;
	}


#endif	// #ifndef GLCLOCK_DLL_EXPORT ... #else


	return ret ;
}

#endif	//#if !defined WIN32 || defined _CONSOLE ... else
