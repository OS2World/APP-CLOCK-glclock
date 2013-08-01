
// ======================================================================== //
//                                                                          //
//                         String クラス宣言ファイル                        //
//                                                                          //
//          December 1996 by Masaki Kawase (g93088@isc.chubu.ac.jp)         //
//                                                                          //
//                                                                          //
//                               クラス String                              //
//                                                                          //
//                                                                          //
//                           汎用 文字列操作クラス                          //
//                                                                          //
//                                                                          //
//                                                                          //
//  関連ファイル :                                                          //
//    "String.H" : クラスの宣言及びインライン関数定義（このファイル）       //
//                アプリケーション インクルード用ファイル                   //
//    "String.C"     : その他の関数定義                                     //
//                                                                          //
//  コンストラクタ :                                                        //
//    String() : ヌル文字で初期化                                           //
//    String(int n) : n 文字長のスペースで初期化                            //
//    String(char *c) : 文字列 c で初期化                                   //
//    String(char *c, int n) : 文字列 c で初期化、長さは強制的に n 文字     //
//        余った場合は、スペースで埋まる                                    //
//    String(String s, int n) : String オブジェクトを使う以外は上と同じ     //
//    String(String s) : String オブジェクトコピー                          //
//                                                                          //
//                                                                          //
//  st は String クラスのオブジェクトとする（String st ; ）                 //
//                                                                          //
//  メソッド :                                                              //
//    int st.Length() : 文字列長を返す                                      //
//    String& st.Length(n) : 文字列長をセット（余った場合はスペース）       //
//                                                                          //
//    st = String, st = (char*) で代入                                      //
//                                                                          //
//    以降のメソッドで、                                                    //
//    Mid(), Left(), Right() は、長過ぎて余った部分は詰められて短くなる     //
//    コンストラクタと違い、st の長さが 引数 n になるとは限らない           //
//                                                                          //
//    st.Mid(int m, n) : st を、m 文字目から n 文字分の文字列に変化         //
//    Mid(st, int m, n) : st の、m 文字目から n 文字分を取り出す            //
//                                                                          //
//    st.Left(int n) : st を、左から n 文字分の文字列に変化                 //
//        Length(n) との違いは、st からはみ出した部分はカットされること     //
//        Length(n) では 長さは必ず n に、はみ出した部分はスペースで埋まる  //
//    Left(st, n) : st の、左から n 文字分を取り出す                        //
//                                                                          //
//    st.Right(int n) : st を、右から n 文字分の文字列に変化                //
//    Right(st, n) : st の、右から n 文字分を取り出す                       //
//                                                                          //
//                                                                          //
//  比較演算子 :                                                            //
//    ==, !=, <, >, <=, <= : 文字列の比較                                   //
//        引数は、両方が String または、String と char* を取る              //
//                                                                          //
//  入出力オペレータ :                                                      //
//    <<, >>                                                                //
//                                                                          //
//                                                                          //
// ======================================================================== //


#ifndef ___STRING_H___
#define ___STRING_H___

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "Define.H"


//#define STRING_DEBUG

// ４バイト整数で収まる 10 のべき乗の最大値（IntToString() で使用）
#define MAX_DIV_DEF   1000000000


class String
{
	enum
	{
		MIN_ALLOC_UNIT = 64,
		MAX_ALLOC_UNIT = 4096,
		MAX_LINE_BUF   = 16384,
	} ;

    int     length ;
	int     allocated ;
    char    *str ;

	// 初期化時のクリア用
    inline String& Clear()
	{
		if (str) delete [] str ;
		length = 0 ;
		str = new char[1] ;
		str[0] = '\0' ;
		allocated = 1 ;
		return *this ;
	}

	// n 文字作成時、実際に確保すべきメモリ量
	// n は目的の文字列長
	inline static int shouldAlloc(int n)
	{
		if (n < MIN_ALLOC_UNIT)
			return MIN_ALLOC_UNIT ;
		else
		{
			if ((n + 1) * 2 > MAX_ALLOC_UNIT)
				return n + 1 + MAX_ALLOC_UNIT ;
			else
				return (n + 1) * 2 ;
		}
	}

	// 再割り当て
	// バッファが足らない場合
	//   確保してコピー
	//
	// 既に足りている場合
	//   必要量の倍以上ある場合は
	//   必要量の倍まで削る
	//   実データ長 length が減る可能性があるので注意
	//
	// n は、必要な文字列長
	// このコール後代入される文字列長
	//
	// guarantee に FALSE が指定されている場合は
	// データのコピーは実行しない
	inline void Realloc(int n, int guarantee = TRUE)
	{
#ifdef STRING_DEBUG
		cout << "String::Realloc(" << n << ", " << guarantee << ")" << endl ;
#endif
		if (n == length) return ;

		if (n + 1 > allocated)
		{
			// 必要量の倍確保する
			// ただし必要量の倍が MAX_ALLOC_UNIT を超える場合
			// 必要量＋MAX_ALLOC_UNIT 分確保する
			allocated = shouldAlloc(n) ;
//			if ((n + 1) * 2 > MAX_ALLOC_UNIT)
//				allocated = n + 1 + MAX_ALLOC_UNIT ;
//			else
//				allocated = (n + 1) * 2 ;

			char *w = new char[allocated] ;

			if (guarantee)
			{
				// 実データ（つまり文字列長＋１）
				// length + 1 バイトコピー
				// ただし length が n より多い場合は n だけで良い
				if (length > n)
				{
					memcpy(w, str, n) ;
					w[n] = '\0' ;
				}
				else
					memcpy(w, str, length + 1) ;
			}

			// 最後にバッファを交換
			delete [] str ;
			str = w ;
		}
		else
		{
			// バッファがあまる場合
			// 実際の必要量の倍を超えている場合は
			// 必要量の倍、ただし最低でも MIN_ALLOC_UNIT にバッファを削る
			//   実データ長 length が減る可能性があるので注意
			if ((n + 1) * 2 < allocated)
			{
				// 必要量の倍までデータを削る
				// ただし必要量の倍が MAX_ALLOC_UNIT を超える場合
				// 必要量＋MAX_ALLOC_UNIT 分確保する
				if ((n + 1) * 2 > MAX_ALLOC_UNIT)
					allocated = n + 1 + MAX_ALLOC_UNIT ;
				else if ((n + 1) * 2 < MIN_ALLOC_UNIT)
					allocated = MIN_ALLOC_UNIT ;
				else
					allocated = (n + 1) * 2 ;

				char *w = new char[allocated] ;

				if (guarantee)
				{
					// この後必要になるデータ（n）
					// すなわち n バイトコピー
					// ただし、もし確保量が n + 1 に足らない場合は
					// 足りる部分までをコピー
					//
					// n 文字の最後にはヌル文字追加
					if (n + 1 > allocated)
					{
						memcpy(w, str, allocated - 1) ;
						w[allocated - 1] = '\0' ;
					}
					else
					{
						memcpy(w, str, n) ;
						w[n] = '\0' ;
					}
				}

				// 最後にバッファを交換
				delete [] str ;
				str = w ;
			}
		}
	}

	// データのコピー
	// コピー先を元に再確保してからデータをコピー
	inline void Copy(const String& s)
	{
		if (this == &s) return ;

		// 必要な場合は再確保
		// 少なくともコピー元 s.length 分のデータは保証される
		// ただしデータのコピーは必要ないので FALSE を渡す
		Realloc(s.length, FALSE) ;

		// 実際のデータコピー
		length = s.length ;
		memcpy(str, s.str, s.length) ;

		// 最後は一応明示的にヌル文字追加
		str[length] = '\0' ;
	}

	String& ReadAllLines(FILE *fp) ;

  public:

    // コンストラクタ
    String()
	{
		str = NULL ;
		Clear() ;
	}

    String(int n) ;
//    String(char *c, int n = -1) ;
    String(const char *c) ;
    String(const char *c, int n) ;// = -1) ;
    String(const String &s) ;
    String(const String &s, int) ;// n = -1) ;
    String(char c) ;

    // デストラクタ
    ~String() { if (str) delete [] str ; }

    // キャスト演算子（char*）
//#ifdef WIN32
	inline operator char*() const { return str ; }
//#else
//	const operator char*() const { return str ; }
//#endif

    // アクセサ
    inline int Length() const { return length ; }

    // 文字列の長さを変化（余った部分はスペース ' '）
    String& Length(int n) ;

    // n 文字目の１文字
    char Character(int n = 1) const ;

	// n 文字目の文字を変更
	String& SetAt(int n, char c) ;

	String& ReplaceString(char s, char d) ;
	String& ReplaceString(const String& s, const String& d) ;
	inline String& ReplaceString(const String& s, char d) { ReplaceString(s, String(d)) ; return *this ; }
	inline String& ReplaceString(const char s, const String& d) { ReplaceString(String(s), d) ; return *this ; }

	// Lower, Upper
	String& Lower() ;
	inline friend String Lower(const String& s) { String w = s ; w.Lower() ; return w ; }
	String& Upper() ;
	inline friend String Upper(const String& s) { String w = s ; w.Upper() ; return w ; }

	// フォーマット
	String& Format(const char* format, ...) ;

	// フォーマットを追加
	String& AddFormat(const char* format, ...) ;

	// テキストファイル読み込み
	String& ReadFile(const String& fileName) ;

	// トークン解析
	// 返されたトークン（最初の文字列）は削除される
	// String("") が返されるまで繰り返すこと
	String Token(const String& delimiters = String(" \t\n"), const String& comment = String("//")) ;

    // m 文字目から n 文字
    String& Mid(int m, int n = 1) ;
    friend String Mid(String s, int m, int n = 1) ;

    // 左から n 文字
    String& Left(int n = 1) ;
    friend String Left(const String& s, int n = 1) ;

    // 右から n 文字
    String& Right(int n = 1) ;
    friend String Right(const String& s, int n = 1) ;

	String FileDirectory() const ;
	String FileNamePart() const ;

    // 文字列サーチ（最初に一致した場所を返す）
    int Search(const String& key) const ;
    int Search(const char   *key) const ;
    int Search(const char    key) const ;

    // オペレータ（/）つまりは、引数の文字列をサーチし、一致した回数を返す
    int operator /(const String& key) const ;
    int operator /(const char   *key) const ;
    int operator /(const char    key) const ;

    // オペレータ（=）代入演算子
    String& operator =(const String& s) ;
    String& operator =(const char   *c) ;
    String& operator =(const char    c) ;

    // 文字比較（最初の n 文字が s と同じで、n より後が s と同じか途切れる）
    // コマンドオプションのサーチなどで使用
    int Equ(const String s, int n = 1) ;

    // n 文字大小比較（strncmp()と同じだが、n を省略すると、strcmp() と同じ）
    int Cmp(const String s, int n = -1) ;

    // オペレータ（==, !=）比較演算子
    int operator ==(const String& s2) const ;
    int operator !=(const String& s2) const ;
    int operator ==(const char    *c) const ;
    int operator !=(const char    *c) const ;
    int operator ==(const char     c) const ;
    int operator !=(const char     c) const ;
    friend int operator ==(const char *c, const String& s2) ;
    friend int operator !=(const char *c, const String& s2) ;
    friend int operator ==(const char  c, const String& s2) ;
    friend int operator !=(const char  c, const String& s2) ;

    // オペレータ（>, <, >=, <=）比較演算子
    int operator > (const String& s2) const ;
    int operator < (const String& s2) const ;
    int operator >=(const String& s2) const ;
    int operator <=(const String& s2) const ;
    int operator > (const char    *c) const ;
    int operator < (const char    *c) const ;
    int operator >=(const char    *c) const ;
    int operator <=(const char    *c) const ;
    int operator > (const char     c) const ;
    int operator < (const char     c) const ;
    int operator >=(const char     c) const ;
    int operator <=(const char     c) const ;
    friend int operator > (const char *c, const String& s2) ;
    friend int operator < (const char *c, const String& s2) ;
    friend int operator >=(const char *c, const String& s2) ;
    friend int operator <=(const char *c, const String& s2) ;
    friend int operator > (const char  c, const String& s2) ;
    friend int operator < (const char  c, const String& s2) ;
    friend int operator >=(const char  c, const String& s2) ;
    friend int operator <=(const char  c, const String& s2) ;

    // オペレータ（()）代入演算子
    String& operator ()(const String& s) ;
    String& operator ()(const char   *c) ;
    String& operator ()(const char    c) ;

    // オペレータ（+, +=）
    String operator +(const String& s) const ;
    String operator +(const char   *s) const ;
    String operator +(const char    c) const ;
    friend String operator +(const char *d, const String& s) ;
    friend String operator +(const char  d, const String& s) ;
    String& operator +=(const String& s) ;
    String& operator +=(const char   *s) ;
    String& operator +=(const char    c) ;

    // オペレータ（*, *=）
    String operator *(int n) ;
    String& operator *=(int n) ;

    // 入出力オペレータ
    friend ostream& operator <<(ostream& os, const String& s) ;
    friend istream& operator >>(istream& is, String& s) ;


    // 拡張

    // 文字列の切り詰め（右側から空白または改行またはTABを省く）
    String& Cut() ;
    friend String& Cut(String      s) ;
    friend String& Cut(const char *s) ;
} ;

// 整数から String へ
String IntToString(int i) ;


#ifdef OVERRIDE_NEW_OPERATOR
void MemDebugFinal() ;
#endif	// #ifdef OVERRIDE_NEW_OPERATOR


#endif
