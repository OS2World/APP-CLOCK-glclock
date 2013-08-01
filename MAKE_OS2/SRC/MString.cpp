
#include "MString.H"


// コンストラクタ
String::String(int n)
{
	length = n ;

	if (n == 0)
	{
		str = NULL ;
		Clear() ;
	}
	else
	{
		allocated = shouldAlloc(length) ;
		str = new char[allocated] ;
		memset(str, ' ', length) ;
		str[length] = '\0' ;
	}
}

String::String(const char *c)
{
//      cout << "String::String from const char * OK!!!!!!!" << endl ;

    length = strlen(c) ;
	if (length == 0)
	{
		str = NULL ;
		Clear() ;
		return ;
	}

/*
	if (length < MIN_ALLOC_UNIT)
		allocated = MIN_ALLOC_UNIT ;
	else
		allocated = (length + 1) * 2 ;
*/
	allocated = shouldAlloc(length) ;
    str = new char[allocated] ;

	// 最初からヌルが有り得るので、ヌルは別途追加
    memcpy(str, c, length) ;
    str[length] = '\0' ; // 最後にヌル文字付化
}

String::String(const char *c, int n)
{
//      cout << "String::String from const *char, int OK!!!!!!!" << endl ;

	int slen = strlen(c) ;

    length = n ;
	allocated = shouldAlloc(length) ;
    str = new char[allocated] ;

    if (slen < length)
    {
        // コピー元の方が短い場合はあまりを空白で埋める
        memcpy(str, c, slen) ;
		memset(&(str[slen]) , ' ', length - slen) ;
    }
    else
    {
        // コピー元の方が長いか同じ場合は単純に必要分コピー
        memcpy(str, c, length) ;
    }

	// 最後にヌル文字追加
	str[length] = '\0' ;
}

String::String(const String& s)
{
//    cout << "String::String from String OK!!!!!!!" << endl ;
//	if (this == &s) return ;

    length = s.length ;
	allocated = s.allocated ;

    str = new char[allocated] ;
    memcpy(str, s.str, length) ;
    str[length] = '\0' ; // 最後にヌル文字付化

//    *this = String(s.str, s.length) ;
}

String::String(const String& s, int n)
{
//    cout << "String::String from String, int  OK!!!!!!!" << endl ;
    length = n ;
	allocated = shouldAlloc(length) ;
    str = new char[allocated] ;

    if (s.length < length)
    {
        // コピー元の方が短い場合はあまりを空白で埋める
        memcpy(str, s.str, s.length) ;
		memset(&(str[s.length]), ' ', length - s.length) ;
    }
    else
    {
        // コピー元の方が長いか同じ場合は単純に必要分コピー
        memcpy(str, s.str, length) ;
    }

	// 最後にヌル文字追加
	str[length] = '\0' ;

//    *this = String(s.str, n) ;
}

String::String(char c)
{
    str = NULL ;
    if (c == '\0')
	{
		str = NULL ;
		Clear() ;
	}
    else
    {
		length = 1 ;
		allocated = MIN_ALLOC_UNIT ;
		str = new char[allocated] ;
		str[0] = c ; str[1] = '\0' ;
    }
}


// 文字列の長さを変化（余った部分は ' '）
String& String::Length(int n)
{
	if (length == n)
		return *this ;
	else if (length > n)
	{
		// 短くなったとき
		Realloc(n) ;
		length = n ;
		return *this ;
	}
	else
	{
		// 長くなったときのみ、増えた分に空白をセット
		Realloc(n) ;
		memset(&(str[length]), ' ', n - length) ;
//		str[n] = '\0' ;
		length = n ;
		return *this ;
	}

//    return (*this = String(str, n)) ;
}


// n 文字目の１文字
char String::Character(int n) const
{
	if (n < 1 || n > length) return '\0' ;
	return str[n - 1] ;
}

// n 文字目の文字を変更
String& String::SetAt(int n, char c)
{
	if (n > 0 && n <= length)
		str[n - 1] = c ;
	return *this ;
}


String& String::ReplaceString(char s, char d)
{
	for (int n = 1 ; n <= length ; n ++)
	{
		if (str[n - 1] == s)
			str[n - 1] = d ;
	}

	return *this ;
}

String& String::ReplaceString(const String& s, const String& d)
{
	if (this == &s || this == &d) return *this ;

	int pos = 1 ;
	while (TRUE)
	{
		int start = Search(s) ;

		// 2000.06.08
		if (start < pos || !start)
			break ;

		int rlen = length - (start + s.length - 1) ;
		String right ;
		if (rlen > 0)
			right = ::Right(*this, rlen) ;

		if (start == 1)
		{
			*this = d + right ;
		}
		else
		{
			Length(start - 1) ;
			*this += d + right ;
		}

		pos = start + d.Length() ;
	}

	return *this ;
}


// Lower, Upper
String& String::Lower()
{
	for (int i = 0 ; i < length ; i ++)
	{
		if (isupper(str[i]))
			str[i] = tolower(str[i]) ;
	}

	return *this ;
}

String& String::Upper()
{
	for (int i = 0 ; i < length ; i ++)
	{
		if (islower(str[i]))
			str[i] = toupper(str[i]) ;
	}

	return *this ;
}


// フォーマット
String& String::Format(const char* format, ...)
{
	char buf[MAX_LINE_BUF] ;

	va_list args ;
	va_start(args, format) ;
//	int r = vsprintf(buf, format, args) ;
	vsprintf(buf, format, args) ;
	va_end(args) ;

	// 文字列セット
	*this = buf ;
	return *this ;
}

// フォーマット
String& String::AddFormat(const char* format, ...)
{
	char buf[MAX_LINE_BUF] ;

	va_list args ;
	va_start(args, format) ;
//	int r = vsprintf(buf, format, args) ;
	vsprintf(buf, format, args) ;
	va_end(args) ;

	// 文字列セット
	*this += buf ;
	return *this ;
}


// プライベート
String& String::ReadAllLines(FILE *fp)
{
	while (TRUE)
	{
		char buf[MAX_LINE_BUF] ;
		char *s ;

		s = fgets(buf, MAX_LINE_BUF, fp) ;
		if (!s)
			break ;

		*this += s ;
	}
	return *this ;
}


// テキストファイル読み込み
String& String::ReadFile(const String& fileName)
{
	FILE *fp ;
	fp = fopen(fileName, "r") ;

	if (!fp)
		*this = "" ;
	else
	{
		*this = ReadAllLines(fp) ;
		fclose(fp) ;
	}

	return *this ;
}


// トークン解析
// 返されたトークン（最初の文字列）は削除される
// String("") が返されるまで繰り返すこと
String String::Token(const String& delimiters, const String& comment)
{
	String	token ;

	// 次のトークンのスタート位置を検索
	int start ;
	for (start = 1 ; start <= Length() ; start ++)
	{
		// コメント行チェック
		if (::Mid(*this, start, comment.Length()) == comment)
		{
			// コメントなら行の最後まで飛ばす
			String right = ::Mid(*this, start, Length()) ;
			int next = right.Search('\n') ;

			if (next)
				start = next ;
			else	// 改行がないということは文字列の最後
				start = Length() ;

			continue ;
		}

		int flag = TRUE ;
		for (int c = 1 ; c <= delimiters.Length() ; c ++)
		{
			if (Character(start) == delimiters.Character(c))
			{
				flag = FALSE ;
				break ;
			}
		}

		// デリミタのどれともマッチしない場合は決定
		if (flag)
			break ;
	}

	Mid(start, Length()) ;

	// 次のトークンの終わりの位置を検索
	int length ;
	for (length = 1 ; length <= Length() ; length ++)
	{
		// コメントチェック（途中でコメントになっている場合はコメントの直前まで）
		if (::Mid(*this, length, comment.Length()) == comment)
			break ;

		// デリミタのどれか１つでもマッチすればその直前まで
		int flag = FALSE ;
		for (int c = 1 ; c <= delimiters.Length() ; c ++)
		{
			if (Character(length) == delimiters.Character(c))
			{
				flag = TRUE ;
				break ;
			}
		}

		if (flag)
			break ;
	}

	length -- ;
	token = ::Left(*this, length) ;
	Mid(length + 1, Length()) ;

	return token ;
}


// m 文字目から n 文字
String& String::Mid(int m, int n)
{
	// コピーの長さと位置を適正に
    if (m < 1) m = 1 ;
    if (m > length || n < 0) return (this->Clear()) ;
	if (m + n > length + 1) n = length + 1 - m ;

	// 重複する可能性があるため
    memmove(str, &(str[m - 1]), n) ;

	// 無駄があれば削る
	Realloc(n) ;
	length = n ;
	str[length] = '\0' ;

	return *this ;

/*
    if (m < 1) m = 1 ;
    if (m > length || n < 0) return (this->Clear()) ;
    if (m + n > length + 1) n = length + 1 - m ;

    String  w(n) ;
//    for (int i = 0 ; i < n ; i ++) w.str[i] = this->str[m + i - 1] ;
    memcpy(w.str, &(this->str[m - 1]), n) ;

    *this = w ;
    return *this ;
*/
}
String Mid(String s, int m, int n)
{
    return (s.Mid(m, n)) ;
}

// 左から n 文字
String& String::Left(int n)
{
    return (this->Mid(1, n)) ;
}
String Left(const String& s, int n)
{
    return (Mid(s, 1, n)) ;
}

// 右から n 文字
String& String::Right(int n)
{
    if (n > length) n = length ;
    this->Mid(length + 1 - n, n) ;
	return *this ;
}
String Right(const String& s, int n)
{
    String  w = s ;
    return (w.Right(n)) ;
}

// 指定されたファイル名のディレクトリ部分を返す
// 右から検索し、"/" もしくは "\" の手前までを返す
String String::FileDirectory() const
{
#ifdef WIN32
	char dpath = '\\' ;
#else
	char dpath = '/' ;
#endif

	String dir ;

	int locate = length - 1 ;
	while (locate > 0 && str[locate - 1] != dpath) locate -- ;
	if (locate <= 0)
		dir = "" ;
	else
		dir = ::Left(*this, locate - 1) ;

	return dir ;
}

// 指定されたファイル名のファイルネーム部分を返す
// 右から検索し、"/" もしくは "\" の後のみを返す
String String::FileNamePart() const
{
#ifdef WIN32
	char dpath = '\\' ;
#else
	char dpath = '/' ;
#endif

	String dir ;

	int locate = length - 1 ;
	while (locate > 0 && str[locate - 1] != dpath) locate -- ;
	if (locate <= 0)
		dir = str ;
	else
		dir = ::Right(*this, length - locate) ;

	return dir ;
}

// 文字列サーチ（最初に一致した場所を返す）
int String::Search(const String& key) const
{
    if ((!key.length) || (!length)) return FALSE ; // 文字列長が 0 なら FALSE

    // 文字列の長さからキーを引いたところまでサーチ
    for (int i = 0 ; i < length - (key.length - 1) ; i ++)
    {
		char tmp = key.str[0] ;
		if (tmp == str[i])
		{
		    // キーが１文字ならこの時点で確定
		    if (key.length == 1)
				return (i + 1) ;
		    // キーが２文字以上の場合はさらにサーチ
		    else
		    {
			int j ;
			for (j = 1 ; j < key.length ; j ++)
			    if (str[i + j] != key.str[j]) break ;

			// キーの文字数だけ繰り返していれば確定
			if (j == key.length)
			    return (i + 1) ;
			}
		}
	}

    return FALSE ;
}
int String::Search(const char *key) const
{
    return Search(String(key)) ;
}
int String::Search(const char key) const
{
    return Search(String(key)) ;
}


// オペレータ（/）つまりは、引数の文字列をサーチし、一致した回数を返す
int String::operator /(const String& key) const
{
    if ((!key.length) || (!length)) return FALSE ; // 文字列長が 0 なら FALSE

    int     n = 0 ;

    // 文字列の長さからキーを引いたところまでサーチ
    for (int i = 0 ; i < length - (key.length - 1) ; i ++)
    {
		char tmp = key.str[0] ;
		if (tmp == str[i])
		{
		    // キーが１文字ならこの時点で確定
		    if (key.length == 1)
				n ++ ;
			// キーが２文字以上の場合はさらにサーチ
		    else
		    {
				int j ;
				for (j = 1 ; j < key.length ; j ++)
				    if (str[i + j] != key.str[j]) break ;

				// キーの文字数だけ繰り返していれば確定
				if (j == key.length)
			    n ++ ;
		    }
		}
    }

    return n ;
}
int String::operator /(const char *key) const
{
    return *this / String(key) ;
}
int String::operator /(const char key) const
{
    return *this / String(key) ;
}

// 整数から String へ
String IntToString(int i)
{
    String  s ;
    if (i < 0)
    {
		s = '-' ;
		i = -i ;
    }

    for (int flg = OFF, d = MAX_DIV_DEF ; d > 1 ; d /= 10)
    {
		if (d > i)
		{
		    if (flg) s += '0' ;
		    continue ;
		}
		flg = ON ;
		s += char(i / d + '0') ;
		i %= d ;
    }
    return (s + char(i + '0')) ;
}

// オペレータ（=）代入演算子
String& String::operator =(const String& s)
{
/*
    delete [] str ;
    length = s.length ;
    str = new char[length + 1] ;


//    for (int i = 0 ; i < length ; i ++) str[i] = s.str[i] ;
//    str[length] = '\0' ; // 最後にヌル文字付化

    memcpy(str, s.str, length + 1) ;
    return *this ;
*/
	if (this == &s) return *this ;

	Copy(s) ;
	return *this ;
}
String& String::operator =(const char *c)
{
	int clen = strlen(c) ;
	Realloc(clen) ;
	length = clen ;
	memcpy(str, c, length + 1);
	return *this ;

//    return (*this = String(c)) ;
}
String& String::operator =(const char c)
{
	Realloc(1, FALSE) ;
	length = 1 ;
	str[0] = c ;
	str[1] = '\0' ;
	return *this ;

//	return (*this = String(c)) ;
}


// 文字比較（最初の n 文字が s と同じで、n より後が s と同じか途切れる）
// String str ;
// str.Equ("abcde",3) のように ("abcde", 3) で比較すると、
// str = "abc", str = "abcd", str = "abcde" が TRUE
// str が "abcde"（5 文字）より長い時は FALSE
// n <= 0 または 省略すると、n = 1 と同じ
int String::Equ(const String s, int n)
{
    if (n <= 0) n = 1 ;

    if (length > s.length || length < n) return FALSE ;
    String w = s ;
    w.Length(length) ;
    return (*this == w) ;
}

// n 文字大小比較（strncmp()と同じだが、n を省略すると、strcmp() と同じ）
int String::Cmp(const String s, int n)
{
    if (n < 0) return strcmp(str, s.str) ;
    else       return strncmp(str, s.str, n) ;
}

/*
// オペレータ（==, !=）比較演算子
int operator ==(const String& s1, const String& s2)
{
    if (!strcmp(s1.str, s2.str)) return TRUE ;
    else                         return FALSE ;
}
int operator ==(const char *c, const String& s2)
{
    if (!strcmp(c, s2.str)) return TRUE ;
    else                    return FALSE ;
}
int operator !=(const String& s1, const String& s2)
{
    return !(s1 == s2) ;
}
int operator !=(const char *c, const String& s2)
{
    return !(c == s2) ;
}
*/


// オペレータ（==, !=）比較演算子

// String 同士の比較
int String::operator ==(const String& s2) const
{
    if (!strcmp(str, s2.str)) return TRUE ;
    else                      return FALSE ;
}
int String::operator !=(const String& s2) const
{
    if (strcmp(str, s2.str)) return TRUE ;
    else                     return FALSE ;
//	{
//    if (strcmp(str, s2.str)) return TRUE ;
//    else                     return FALSE ;
//	}
}

// char * との比較
int String::operator ==(const char *c) const
{
    if (!strcmp(str, c)) return TRUE ;
    else                 return FALSE ;
}
int String::operator !=(const char *c) const
{
    if (strcmp(str, c)) return TRUE ;
    else                return FALSE ;
}

// char との比較
int String::operator ==(const char c) const
{
    if (length == 1 && str[0] == c) return TRUE ;
    else                            return FALSE ;
}
int String::operator !=(const char c) const
{
    return !(*this == c) ;
}

// 左辺が char * との比較
int operator ==(const char *c, const String& s2)
{
    if (!strcmp(c, s2.str)) return TRUE ;
    else                    return FALSE ;
}
int operator !=(const char *c, const String& s2)
{
    if (strcmp(c, s2.str)) return TRUE ;
    else                   return FALSE ;
}

// 左辺が char との比較
int operator ==(const char c, const String& s2)
{
    return (s2 == c) ;
}
int operator !=(const char c, const String& s2)
{
    return (s2 != c) ;
}


/*
// オペレータ（>, <, >=, <=）大小比較演算子
int operator >(const String& s1, const String& s2)
{
    if (strcmp(s1.str, s2.str) > 0) return TRUE ;
    else                            return FALSE ;
}
int operator >(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) > 0) return TRUE ;
    else                       return FALSE ;
}
int operator <(const String& s1, const String& s2)
{
    if (strcmp(s1.str, s2.str) < 0) return TRUE ;
    else                            return FALSE ;
}
int operator <(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) < 0) return TRUE ;
    else                            return FALSE ;
}
int operator >=(const String& s1, const String& s2)
{
    return (s1 > s2 || s1 == s2) ;
}
int operator >=(const char *c, const String& s2)
{
    return (c > s2 || c == s2) ;
}
int operator <=(const String& s1, const String& s2)
{
    return (s1 < s2 || s1 == s2) ;
}
int operator <=(const char *c, const String& s2)
{
    return (c < s2 || c == s2) ;
}
*/

// オペレータ（>, <, >=, <=）大小比較演算子

// String 同士の比較
int String::operator >(const String& s2) const
{
    if (strcmp(str, s2.str) > 0) return TRUE ;
    else                         return FALSE ;
}
int String::operator <(const String& s2) const
{
    if (strcmp(str, s2.str) < 0) return TRUE ;
    else                         return FALSE ;
}
int String::operator >=(const String& s2) const
{
    if (strcmp(str, s2.str) >= 0) return TRUE ;
    else                          return FALSE ;
}
int String::operator <=(const String& s2) const
{
    if (strcmp(str, s2.str) <= 0) return TRUE ;
    else                          return FALSE ;
}

// char * との比較
int String::operator >(const char *c) const
{
    if (strcmp(str, c) > 0) return TRUE ;
    else                    return FALSE ;
}
int String::operator <(const char *c) const
{
    if (strcmp(str, c) < 0) return TRUE ;
    else                    return FALSE ;
}
int String::operator >=(const char * c) const
{
    if (strcmp(str, c) >= 0) return TRUE ;
    else                     return FALSE ;
}
int String::operator <=(const char *c) const
{
    if (strcmp(str, c) <= 0) return TRUE ;
    else                     return FALSE ;
}

// char との比較
int String::operator >(const char c) const
{
    return (*this > String(c)) ;
}
int String::operator <(const char c) const
{
    return (*this < String(c)) ;
}
int String::operator >=(const char c) const
{
    return (*this >= String(c)) ;
}
int String::operator <=(const char c) const
{
    return (*this <= String(c)) ;
}

// 左辺が char* との比較
int operator >(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) > 0) return TRUE ;
    else                       return FALSE ;
}
int operator <(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) < 0) return TRUE ;
    else                       return FALSE ;
}
int operator >=(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) >= 0) return TRUE ;
    else                        return FALSE ;
}
int operator <=(const char *c, const String& s2)
{
    if (strcmp(c, s2.str) <= 0) return TRUE ;
    else                        return FALSE ;
}

// 左辺が char との比較
int operator >(const char c, const String& s2)
{
    return (String(c) > s2) ;
}
int operator <(const char c, const String& s2)
{
    return (String(c) < s2) ;
}
int operator >=(const char c, const String& s2)
{
    return (String(c) >= s2) ;
}
int operator <=(const char c, const String& s2)
{
    return (String(c) <= s2) ;
}


// オペレータ（()）代入演算子
String& String::operator ()(const String& s)
{
    return (*this = s) ;
}
String& String::operator ()(const char *c)
{
    return (*this = c) ;
}
String& String::operator ()(const char c)
{
    return (*this = c) ;
}


/*
// オペレータ（+, +=）
String operator +(const String& d, const String& s)
{
    String  w(d.str, d.length + s.length) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + d.length] = s.str[i] ;
    return w ;
}
String operator +(char d, const String& s)
{
    String wd = String(d), w(wd, s.length + 1) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + 1] = s.str[i] ;
    return w ;
}
String operator +(const String& d, char s)
{
    String ws(s), w(d.str, d.length + 1) ;
    for (int i = 0 ; i < ws.length ; i ++) w.str[i + d.length] = ws.str[i] ;
    return w ;
}
String& String::operator +=(const char c)
{
    return (*this = *this + c) ;
}
String& String::operator +=(const String& s)
{
    return (*this = *this + s) ;
}
*/

// オペレータ（+, +=）

// String 同士の連結
String String::operator +(const String& s) const
{
    String  w(str, length + s.length) ;

//    for (int i = 0 ; i < s.length ; i ++) w.str[i + length] = s.str[i] ;
    memcpy(&(w.str[length]), s.str, s.length) ;

    return w ;
}
// 右辺が char * との連結
String String::operator +(const char *s) const
{
    int slen = strlen(s) ;
    String  w(str, length + slen) ;

//    for (int i = 0 ; i < slen ; i ++) w.str[i + length] = s[i] ;
    memcpy(&(w.str[length]), s, slen) ;

    return w ;
}
// 右辺が char との連結
String String::operator +(const char s) const
{
    String w(str, length + 1) ;
    w.str[w.length - 1] = s ;
    return w ;
}

// 左辺が char * との連結
String operator +(const char *d, const String& s)
{
    int     dlen = strlen(d) ;
    String  w(d, dlen + s.length) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + dlen] = s.str[i] ;
    return w ;
}

// 左辺が char との連結
String operator +(const char d, const String& s)
{
    String wd(d), w(wd, s.length + 1) ;
    for (int i = 0 ; i < s.length ; i ++) w.str[i + 1] = s.str[i] ;
    return w ;
}

// 連結代入
String& String::operator +=(const String& s)
{
	if (this == &s)
	{
		*this += String(s) ;
		return *this ;
	}

	Realloc(length + s.length) ;
	memcpy(&(str[length]), s.str, s.length + 1) ;
	length += s.length ;

	return *this ;
/*
    int len = length ;
    Length(len + s.length) ;
    for (int i = 0 ; i < s.length ; i ++)
      str[i + len] = s.str[i] ;

    return (*this) ;
*/
}
String& String::operator +=(const char *s)
{
	int slen = strlen(s) ;
	Realloc(length + slen) ;
	memcpy(&(str[length]), s, slen + 1) ;
	length += slen ;

	return *this ;

//    return (*this = *this + s) ;
}
String& String::operator +=(const char c)
{
	Realloc(length + 1) ;
	length ++ ;
	str[length - 1] = c ;
	str[length] = '\0' ;

	return *this ;
/*
    Length(length + 1) ;
    str[length - 1] = c ;
    return (*this) ;
*/
}


// オペレータ（*, *=）
String String::operator *(int n)
{
	String s(length * n) ;
	for (int i = 0 ; i < n ; i ++)
		memcpy(&(str[length * i]), str, length) ;
	length *= n ;
	return s ;
/*
    String  s ;
    for (int i = 0 ; i < n ; i ++) s += *this ;
    return s ;
*/
}
String& String::operator *=(int n)
{
	Realloc(length * n) ;
	for (int i = 1 ; i < n ; i ++)
		memcpy(&(str[length * i]), str, length) ;
	length *= n ;
	return *this ;

//    return (*this = *this * n) ;
}

// 入出力オペレータ
ostream& operator <<(ostream& os, const String& s)
{
    os << s.str ;
    return os ;
}
istream& operator >>(istream& is, String& s)
{
    is >> s.str ;
    return is ;
}


// 拡張

// 文字列の切り詰め（右側から空白または改行またはTABを省く）
String& String::Cut()
{
    int  i ;
    for (i = length ; i >= 1 ; i --)
    {
//	if (Character(i) != ' '  &&
//	    Character(i) != '\n' &&
//	    Character(i) != '\t') break ;
	if (!isspace(Character(i))) break ;
    }
    Length(i) ; // 文字列の長さを切り詰める

    return *this ;
}


// 文字列の切り詰め（右側から空白または改行またはTABを省く）
String& Cut(String s)
{
    return s.Cut() ;
}

// 文字列の切り詰め（右側から空白または改行またはTABを省く）
String& Cut(const char *s)
{
    String tmp(s) ;
    return tmp.Cut() ;
}






#ifdef OVERRIDE_NEW_OPERATOR


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//      メモリノード構造体
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
typedef struct memnode_tag
{
        struct memnode_tag      *pNext;
        unsigned long                        dwSize;
        int                                     pMem[1];
} MEMNODE;

// 構造体のヘッダ部分のサイズ

#define MEMNODE_HDRSIZE (sizeof(MEMNODE*) + sizeof(unsigned long))



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//      ハッシュテーブル関連
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ハッシュテーブルの大きさ（２の累乗であることが必須）
//#define HASHTABLE_SIZE  0x1000
#define HASHTABLE_SIZE  0x1000

// ハッシュテーブル
static MEMNODE memtbl[HASHTABLE_SIZE];

// ポインタからハッシュキーを求めるインライン関数
static inline unsigned long GetHashKey( void* p )
{
        return (int)p & (HASHTABLE_SIZE - 1);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//      データ集計
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// アロケート要求回数（失敗含む）
static int nAllocCtr = 0;

// フリー要求回数（失敗含む）
static int nFreeCtr = 0;

// アロケート失敗回数
static int nAllocFail = 0;

// フリー失敗回数
static int nFreeFail = 0;

// NULL フリー回数
static int nFreeNull = 0;

// 現在のメモリ確保量（単純な総和）
static int nAllocAmt = 0;

// 最大時のメモリ確保量
static int nAllocAmtMax = 0;

// 明示的チェック終了
static int isChecked = FALSE ;


// 未開放メモリテーブル
#define MAX_UNFREE_TABLE 10000
static int nLeaks ;
static MEMNODE *unfree[MAX_UNFREE_TABLE] ;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//      オーバーライドしたnew演算子
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void* :: operator new( size_t n )
{
        MEMNODE*        pNewNode;
        MEMNODE*        pNode;
        unsigned long           nKey;

		if (!nAllocCtr)
		{
			for (int i = 0 ; i < HASHTABLE_SIZE ; i ++)
				memtbl[i].pNext = NULL ;
		}

        nAllocCtr++;

        // とりあえず領域を確保
        pNewNode = (MEMNODE*)malloc( n + sizeof(MEMNODE) );
        if ( pNewNode == NULL )
        {
                nAllocFail++;
                return NULL;
        }


/*
		else if (//((int)pNewNode) == (0xD82B40) ||
				 //((int)pNewNode) == (0xD82DE0) ||
//				 ((int)pNewNode) == (0xD83830) ||
				 ((int)pNewNode) == (0xD837B0))
		{
			pNewNode = pNewNode ;
//			inline int logprintf(const char* format, ...) ;
//			logprintf("pNewNode:%lX\n", pNewNode) ;
		}

//			inline int logprintf(const char* format, ...) ;
//			logprintf("pNewNode:%lX  pNewNode->pMem:%lX\n", (int)pNewNode, (int)pNewNode->pMem) ;
*/

        pNewNode->pNext = NULL;
        pNewNode->dwSize = n;

        // メモリ確保量の集計
        nAllocAmt += n;
        if ( nAllocAmt > nAllocAmtMax ) nAllocAmtMax = nAllocAmt;

        // ハッシュテーブルの先頭のエントリを得る
        nKey    = GetHashKey(pNewNode);
        pNode   = &memtbl[nKey];

        // 連結の最後尾をたぐる
        while ( pNode->pNext != NULL )
        {
                pNode = pNode->pNext;
        }

        // 連結の最後尾に pNewNode を追加
        pNode->pNext = pNewNode;

        // pMemが確保された領域となる
        return (void*)pNewNode->pMem;
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//      オーバーライドしたdelete演算子
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void :: operator delete( void* p )
{
        MEMNODE*        pDelNode;
        MEMNODE*        pNode;
        int                     nKey;

        nFreeCtr++;

		if (!p)
		{
			nFreeFail ++ ;
			nFreeNull ++ ;
			return ;
		}
        
        // ポインタpよりノードのポインタを算出する
        pDelNode = (MEMNODE*)((int)p - MEMNODE_HDRSIZE);

		if (isChecked)
		{
			for (int i = 0 ; i < nLeaks ; i ++)
			{
				if (unfree[i] == pDelNode)
				{
					unfree[i] = NULL ;
					free(pDelNode) ;
				}
			}

			FILE *fp = fopen("leakcheck.log", "w") ;
			if (fp)
			{
				for (int i = 0 ; i < nLeaks ; i ++)
				{
					if (unfree[i])
						fprintf(fp, "leak: address: 0x%lX\n", (int)unfree[i]) ;
				}
//				fprintf(fp, "already checked!  address: %lX  delete request: %ld\n", ((int)p - MEMNODE_HDRSIZE), nFreeCtr) ;
				fprintf(fp, "delete request: %ld\n", nFreeCtr) ;
				fclose(fp) ;
			}
			return ;
		}

		if ((int)pDelNode == 0x00d729d0)
		{
			pDelNode = pDelNode ;
		}

        // pDelNodeよりハッシュキーを求める
        nKey = GetHashKey(pDelNode);
        
        // ハッシュテーブルより，pDelNodeの一つ前のノードをたぐる
        pNode = &memtbl[nKey];
        while ( pNode->pNext != pDelNode && pNode->pNext != NULL )
        {
                pNode = pNode->pNext;
        }

        // ノードが見つからなかった場合の処理（エラー）
        if ( pNode->pNext == NULL )
        {
                nFreeFail++;
                return;
        }

        // 連結の繋ぎ直し
        pNode->pNext = pDelNode->pNext;

        // 領域の開放
        nAllocAmt -= pDelNode->dwSize;
        free(pDelNode);
}



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//      集計処理
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void MemDebugFinal()
{
        char    pStrBuf[256];
        int             i;
		int     leakBytes = 0 ;

		isChecked = TRUE ;
		for (i = 0 ; i < MAX_UNFREE_TABLE ; i ++)
			unfree[i] = NULL ;


//		inline int logprintf(const char* format, ...) ;

		nLeaks = 0 ;

        // ハッシュテーブルを走査し，未開放の領域があったら警告を表示。
        for ( i=0 ; i<HASHTABLE_SIZE ; i++ )
        {
                MEMNODE* pNode = memtbl[i].pNext;

                while ( pNode )
                {
                        MEMNODE* pNext = pNode->pNext;
//                        sprintf( pStrBuf, ">> Memory leak found!!  key: %d  address:%lX  pNext:%lX  size:%d bytes\r\n", i, (int)pNode, (int)pNode->pNext, (int)pNode->dwSize) ;
                        sprintf( pStrBuf, ">> Memory leak found!!  address:%lX  %d bytes\r\n", (int)pNode, (int)pNode->dwSize) ;
//                        logprintf( pStrBuf );
						leakBytes += pNode->dwSize ;
//                        free( pNode );

						unfree[nLeaks] = pNode ;
						nLeaks ++ ;

                        pNode = pNext;
                }
        }

/*
        // 集計結果を出力
        sprintf( pStrBuf, ">> new    request : %ld times\r\n", nAllocCtr );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> delete request : %ld times\r\n", nFreeCtr );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> new    failure : %ld times\r\n", nAllocFail );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> delete failure : %ld times\r\n", nFreeFail );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> delete NULL    : %ld times\r\n", nFreeNull );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> allocation max : %ld bytes\r\n", nAllocAmtMax );
        logprintf( pStrBuf );

        sprintf( pStrBuf, ">> total %d bytes leak\r\n", leakBytes );
        logprintf( pStrBuf );
*/
}


#endif	// #ifdef OVERRIDE_NEW_OPERATOR
