
#include "glclock.H"


#ifdef WIN32
#include <crtdbg.h>
#endif


// �ᥤ��
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

	// �����꡼�󥻡��Х⡼�ɤǵ�ư��Τ�Τ������
	// ���ξ�ǽ�λ
	HWND win ;
	win = FindWindow("GLUT", "glclock screen saver") ;
	if (win) return EXIT_SUCCESS ;
#endif

#ifdef macintosh
	// argument ����� (mac �Ǥ� main ����ϲ����äƤ��ʤ���
	getOption(&argc, &argv);
	SetCursor(*GetCursor(watchCursor)) ;	// ���ץ���������ѹ�
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
// Windows ���ץ�ξ��


// Windows �ᥤ��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	int newFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) ;
	newFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF ; // | _CRTDBG_DELAY_FREE_MEM_DF ;
	_CrtSetDbgFlag(newFlag) ;
	_ASSERTE(_CrtCheckMemory()) ;
#endif

	// �����꡼�󥻡��Х⡼�ɤǵ�ư��Τ�Τ������
	// ���ξ�ǽ�λ
	HWND win ;
	win = FindWindow("GLUT", "glclock screen saver") ;
	if (win) return EXIT_FAILURE ;

	hInstanceGlClock = hInstance ;

	// �¹ԥե�����ѥ��� \Windows\System\glclock.ini �ե��������¸
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

	// dll ��Ȥ�ʤ����
	ret = glclock(__argc, __argv) ;

	if (ret)
		ret = FALSE ;
	else
		ret = TRUE ;

#else
	// glclock.dll ������Ū�˥�󥯤�����Ѥ�����

	// glclock.dll ���� glclock() ����
	// �ؿ����ɥ쥹�򥲥åȤǤ������ glclock ������

	HINSTANCE hLib_glclock = NULL ;
	hLib_glclock = LoadLibrary(_T(GLCLOCK_DLL)) ;
	if (hLib_glclock)
	{
		// DLL ���ɤ�����������
		// glclock() �� ����ȥ�ݥ���Ȥ����
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

		// DLL ����
		if (hLib_glclock)
			FreeLibrary(hLib_glclock) ;

		if (ret)
			ret = FALSE ;
		else
			ret = TRUE ;
	}
	else
	{
		// DLL �Υ��ɤ˼���
		MessageBox(NULL, _T("Failed to load glclock.dll"), _T("LoadLibrary Error"), MB_OK | MB_ICONSTOP) ;
		ret = FALSE ;
	}


#endif	// #ifndef GLCLOCK_DLL_EXPORT ... #else


	return ret ;
}

#endif	//#if !defined WIN32 || defined _CONSOLE ... else
