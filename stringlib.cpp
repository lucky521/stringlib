#include "stringlib.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>

namespace stringlib
{
    /* fixup start/end slice values */
#define ADJUST_INDICES(start, end, len)         \
if (end > len)                          \
    end = len;                          \
else if (end < 0) {                     \
    end += len;                         \
if (end < 0)                        \
    end = 0;                        \
}                                       \
if (start < 0) {                        \
    start += len;                       \
if (start < 0)                      \
    start = 0;                      \
}
    
    namespace { //内部匿名空间
        
		//////////////////////////////////////////////////////////////////////////////////////////////
		/// 将vector中的string倒序放置
		///
		void reverse_strings( std::vector< std::string > & result)
		{
			for (std::vector< std::string >::size_type i = 0; i < result.size() / 2; i++ )
			{
				std::swap(result[i], result[result.size() - 1 - i]);
			}
		}
        
		//////////////////////////////////////////////////////////////////////////////////////////////
		/// 依空格将一个字符串分割为若干个子串放入vector，最多分maxsplit份。
		///
		void split_whitespace( const std::string & str, std::vector< std::string > & result, int maxsplit )
		{
			std::string::size_type i, j, len = str.size();
			for (i = j = 0; i < len; )
			{
                
				while ( i < len && ::isspace( str[i] ) ) i++;
				j = i;
                
				while ( i < len && ! ::isspace( str[i]) ) i++;
                
                
				if (j < i)
				{
					if ( maxsplit-- <= 0 ) break;
                    
					result.push_back( str.substr( j, i - j ));
                    
					while ( i < len && ::isspace( str[i])) i++;
					j = i;
				}
			}
			if (j < len)
			{
				result.push_back( str.substr( j, len - j ));
			}
		}
        
        
		//////////////////////////////////////////////////////////////////////////////////////////////
		/// 与split_whitespace功能一致，但从右向左进行判断
		///
		void rsplit_whitespace( const std::string & str, std::vector< std::string > & result, int maxsplit )
		{
			std::string::size_type len = str.size();
			std::string::size_type i, j;
			for (i = j = len; i > 0; )
			{
                
				while ( i > 0 && ::isspace( str[i - 1] ) ) i--;
				j = i;
                
				while ( i > 0 && ! ::isspace( str[i - 1]) ) i--;
                
				if (j > i)
				{
					if ( maxsplit-- <= 0 ) break;
                    
					result.push_back( str.substr( i, j - i ));
                    
					while ( i > 0 && ::isspace( str[i - 1])) i--;
					j = i;
				}
			}
			if (j > 0)
			{
				result.push_back( str.substr( 0, j ));
			}
            reverse_strings( result );
		}
        
        
        /* Matches the end (direction >= 0) or start (direction < 0) of self
         * against substr, using the start and end arguments. Returns
         * -1 on error, 0 if not found and 1 if found.
         */
        int _string_tailmatch(const std::string & self, const std::string & substr,
                              std::string::size_type start, std::string::size_type end,
                              int direction)
        {
            std::string::size_type len = self.size();
            std::string::size_type slen = substr.size();
            
            const char* sub = substr.c_str();
            const char* str = self.c_str();
            
            ADJUST_INDICES(start, end, len);
            
            if (direction < 0) {
                // startswith
                if (start+slen > len)
                    return 0;
            } else {
                // endswith
                if (end-start < slen || start > len)
                    return 0;
                if (end-slen > start)
                    start = end - slen;
            }
            if (end-start >= slen)
                return (!std::memcmp(str+start, sub, slen));
            
            return 0;
        }
        
        
	} //内部匿名空间
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 根据自定义的分隔符，将原字符串分割
    ///
    void split( const std::string & str, std::vector< std::string > & result, const std::string & sep, int maxsplit )
    {
        result.clear();
        
        if ( maxsplit < 0 ) maxsplit = MAX_32BIT_INT;
        
        
        if ( sep.size() == 0 )
        {
            split_whitespace( str, result, maxsplit );
            return;
        }
        
        std::string::size_type i,j, len = str.size(), n = sep.size();
        
        i = j = 0;
        
        while ( i+n <= len )
        {
            if ( str[i] == sep[0] && str.substr( i, n ) == sep )
            {
                if ( maxsplit-- <= 0 ) break;
                
                result.push_back( str.substr( j, i - j ) );
                i = j = i + n;
            }
            else
            {
                i++;
            }
        }
        
        result.push_back( str.substr( j, len-j ) );
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    void rsplit( const std::string & str, std::vector< std::string > & result, const std::string & sep, int maxsplit )
    {
        if ( maxsplit < 0 )
        {
            split( str, result, sep, maxsplit );
            return;
        }
        
        result.clear();
        
        if ( sep.size() == 0 )
        {
            rsplit_whitespace( str, result, maxsplit );
            return;
        }
        
        std::string::size_type i, j, len = str.size(), n = sep.size();
        
        i = j = len;
        
        while ( i >= n )
        {
            if ( str[i - 1] == sep[n - 1] && str.substr( i - n, n ) == sep )
            {
                if ( maxsplit-- <= 0 ) break;
                
                result.push_back( str.substr( i, j - i ) );
                i = j = i - n;
            }
            else
            {
                i--;
            }
        }
        
        result.push_back( str.substr( 0, j ) );
        reverse_strings( result );
    }
    
   
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 将字符串两侧的特定字符或字符串去掉
    ///
    #define LEFTSTRIP 0
    #define RIGHTSTRIP 1
    #define BOTHSTRIP 2
    std::string do_strip( const std::string & str, int striptype, const std::string & chars  )
    {
        
        std::string::size_type len = str.size(), i, j, charslen = chars.size();
        
        if ( charslen == 0 )
        {
            i = 0;
            if ( striptype != RIGHTSTRIP )
            {
                while ( i < len && ::isspace( str[i] ) )
                {
                    i++;
                }
            }
            
            j = len;
            if ( striptype != LEFTSTRIP )
            {
                do
                {
                    j--;
                }
                while (j >= i && ::isspace(str[j]));
                
                j++;
            }
        }
        else
        {
            const char * sep = chars.c_str();
            
            i = 0;
            if ( striptype != RIGHTSTRIP )
            {
                while ( i < len && memchr(sep, str[i], charslen) )
                {
                    i++;
                }
            }
            
            j = len;
            if (striptype != LEFTSTRIP)
            {
                do
                {
                    j--;
                }
                while (j >= i &&  memchr(sep, str[j], charslen)  );
                j++;
            }
        }
        
        if ( i == 0 && j == len )
        {
            return str;
        }
        else
        {
            return str.substr( i, j - i );
        }
        
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    std::string strip( const std::string & str, const std::string & chars )
    {
        return do_strip( str, BOTHSTRIP, chars );
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    std::string lstrip( const std::string & str, const std::string & chars )
    {
        return do_strip( str, LEFTSTRIP, chars );
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    std::string rstrip( const std::string & str, const std::string & chars )
    {
        return do_strip( str, RIGHTSTRIP, chars );
    }

    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 依据sep将字符串划分为三部分
    ///
    void partition( const std::string & str, const std::string & sep, std::vector< std::string > & result )
    {
        result.resize(3);
        int index = find( str, sep );
        if ( index < 0 )
        {
            result[0] = str;
            result[1] = "";
            result[2] = "";
        }
        else
        {
            result[0] = str.substr( 0, index );
            result[1] = sep;
            result[2] = str.substr( index + sep.size(), str.size() );
        }
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    void rpartition( const std::string & str, const std::string & sep, std::vector< std::string > & result )
    {
        result.resize(3);
        int index = rfind( str, sep );
        if ( index < 0 )
        {
            result[0] = "";
            result[1] = "";
            result[2] = str;
        }
        else
        {
            result[0] = str.substr( 0, index );
            result[1] = sep;
            result[2] = str.substr( index + sep.size(), str.size() );
        }
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 用连接符str把seq中的字符串连接到一起
    ///
    std::string join( const std::string & str, const std::vector< std::string > & seq )
    {
        std::vector< std::string >::size_type seqlen = seq.size(), i;
        
        if ( seqlen == 0 ) return "";
        if ( seqlen == 1 ) return seq[0];
        
        std::string result( seq[0] );
        
        for ( i = 1; i < seqlen; ++i )
        {
            result += str + seq[i];
            
        }
        
        return result;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    bool endswith( const std::string & str, const std::string & suffix, int start, int end )
    {
        int result = _string_tailmatch(str, suffix,
                                       (std::string::size_type) start, (std::string::size_type) end, +1);
        //if (result == -1) // TODO: Error condition
        
        return static_cast<bool>(result);
    }
    
    
    bool startswith( const std::string & str, const std::string & prefix, int start, int end )
    {
        int result = _string_tailmatch(str, prefix,
                                       (std::string::size_type) start, (std::string::size_type) end, -1);
        //if (result == -1) // TODO: Error condition
        
        return static_cast<bool>(result);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// Check if string is alphanumeric
    ///
    bool isalnum( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        if ( len == 0 ) return false;
        
        //调用cctype中的isalnum
        for ( i = 0; i < len; ++i )
        {
            if ( !::isalnum( str[i] ) )
                return false;
        }
        return true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// Check if string is alphabetic
    ///
    bool isalpha( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        if ( len == 0 ) return false;
        //调用cctype中的isalpha
        for ( i = 0; i < len; ++i )
        {
            if ( !::isalpha( (int) str[i] ) ) return false;
        }
        return true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// Check if string is decimal digit
    ///
    bool isdigit( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        if ( len == 0 ) return false;
        //调用cctype中的isdigit
        for ( i = 0; i < len; ++i )
        {
            if ( ! ::isdigit( str[i] ) ) return false;
        }
        return true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    bool islower( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        if ( len == 0 ) return false;
        
        for ( i = 0; i < len; ++i )
        {
            if ( !::islower( str[i] ) ) return false;
        }
        return true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    bool isupper( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        if ( len == 0 ) return false;
        
        for ( i = 0; i < len; ++i )
        {
            if ( !::isupper( str[i] ) ) return false;
        }
        return true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    bool isspace( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        if ( len == 0 ) return false;
        if( len == 1 ) return ::isspace( str[0] );
        
        for ( i = 0; i < len; ++i )
        {
            if ( !::isspace( str[i] ) ) return false;
        }
        return true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 所有的单词拼写首字母是否为大写，且其他字母为小写
    ///
    bool istitle( const std::string & str )
    {
        std::string::size_type len = str.size(), i;
        
        if ( len == 0 ) return false;
        if ( len == 1 ) return ::isupper( str[0] );
        
        bool wait_new_word = true; //状态：是否在等待单词首字母
        
        for ( i = 0; i < len; ++i )
        {
            if ( ::isupper( str[i] ) )
            {
                if ( !wait_new_word )
                    return false;
                wait_new_word = false;
            }
            else if ( ::islower( str[i] ) )
            {
                if ( wait_new_word )
                    return false;
            }
            else //非字母
            {
                wait_new_word = true;
            }
        }
        return true;
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 返回字符串的首字母大写化形式
    ///
    std::string capitalize( const std::string & str )
    {
        std::string s( str );
        std::string::size_type len = s.size(), i;
        
        for ( i = 0; i < len; ++i )
        {
            if (::isupper(s[i])) s[i] = (char) ::tolower( s[i] );
        }
        
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 返回字符串的小写化形式
    ///
    std::string lower( const std::string & str )
    {
        std::string s( str );
        std::string::size_type len = s.size(), i;
        
        for ( i = 0; i < len; ++i )
        {
            if ( ::isupper( s[i] ) ) s[i] = (char) ::tolower( s[i] );
        }
        
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 返回字符串的大写化形式
    ///
    std::string upper( const std::string & str )
    {
        std::string s( str ) ;
        std::string::size_type len = s.size(), i;
        
        for ( i = 0; i < len; ++i )
        {
            if ( ::islower( s[i] ) ) s[i] = (char) ::toupper( s[i] );
        }
        
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 大小写形式反化
    ///
    std::string swapcase( const std::string & str )
    {
        std::string s( str );
        std::string::size_type len = s.size(), i;
        
        for ( i = 0; i < len; ++i )
        {
            if ( ::islower( s[i] ) ) s[i] = (char) ::toupper( s[i] );
            else if (::isupper( s[i] ) ) s[i] = (char) ::tolower( s[i] );
        }
        
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 将所有首字母大写、其余字母小写化
    ///
    std::string title( const std::string & str )
    {
        std::string s( str );
        std::string::size_type len = s.size(), i;
        
        bool wait_new_word = true; //状态：是否在等待单词首字母
        
        for ( i = 0; i < len; ++i )
        {
            if ( wait_new_word )
            {
                if ( ::islower(s[i]) )
                {
                    s[i] = (char) ::toupper(s[i]);
                }
                wait_new_word = false;
            }
            else
            {
                if ( ::isupper(s[i]) )
                {
                    s[i] = (char) ::tolower(s[i]);
                }
                else if( ::islower(s[i]) )
                {
                }
                else
                    wait_new_word = true;
            }
        }
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    std::string translate( const std::string & str, const std::string & table, const std::string & deletechars )
    {
        std::string s;
        std::string::size_type len = str.size(), dellen = deletechars.size();
        
        if ( table.size() != 256 )
        {
            // raise exception or do nothing
            return str;
        }
        
        //若没有删除字符，则直接映射
        if ( dellen == 0 )
        {
            s = str;
            for ( std::string::size_type i = 0; i < len; ++i )
            {
                s[i] = table[ s[i] ];
            }
            return s;
        }
        
        int trans_table[256]; //0~255
        for ( int i = 0; i < 256; i++)
        {
            trans_table[i] = table[i];
        }
        
        for ( std::string::size_type i = 0; i < dellen; i++)
        {
            trans_table[(int) deletechars[i] ] = -1; //从映射表中去除要删除的字符
        }
        
        for ( std::string::size_type i = 0; i < len; ++i )
        {
            if ( trans_table[ (int) str[i] ] != -1 )
            {
                s += table[ str[i] ];
            }
        }
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 左侧填充0
    ///
    std::string zfill( const std::string & str, int width )
    {
        int len = (int)str.size();
        
        if ( len >= width )
        {
            return str;
        }
        
        std::string s( str );
        
        int fill = width - len;
        
        s = std::string( fill, '0' ) + s;
        
        if ( s[fill] == '+' || s[fill] == '-' )
        {
            s[0] = s[fill];
            s[fill] = '0';
        }
        
        return s;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 左对齐
    ///
    std::string ljust( const std::string & str, int width )
    {
        std::string::size_type len = str.size();
        if ( (( int ) len ) >= width ) return str;
        return str + std::string( width - len, ' ' );
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 右对齐
    ///
    std::string rjust( const std::string & str, int width )
    {
        std::string::size_type len = str.size();
        if ( (( int ) len ) >= width ) return str;
        return std::string( width - len, ' ' ) + str;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 居中对齐
    ///
    std::string center( const std::string & str, int width )
    {
        int len = (int) str.size();
        int marg, left;
        
        if ( len >= width ) return str;
        
        marg = width - len;
        left = marg / 2 + (marg & width & 1);
        
        return std::string( left, ' ' ) + str + std::string( marg - left, ' ' );
        
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 切割出子串
    ///
    std::string slice( const std::string & str, int start, int end )
    {
        ADJUST_INDICES(start, end, (int) str.size());
        if ( start >= end ) return "";
        return str.substr( start, end - start );
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 在start和end之间查找
    /// 当sub为空字符串“”时，返回值会是0.
    int find( const std::string & str, const std::string & sub, int start, int end  )
    {
        ADJUST_INDICES(start, end, (int) str.size());
        
        std::string::size_type result = str.find( sub, start );
        
        // If we cannot find the string, or if the end-point of our found substring is past
        // the allowed end limit, return that it can't be found.
        if( result == std::string::npos ||
           (result + sub.size() > (std::string::size_type)end) )
        {
            return -1;
        }
        
        return (int) result;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    int index( const std::string & str, const std::string & sub, int start, int end  )
    {
        return find( str, sub, start, end );
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    int rfind( const std::string & str, const std::string & sub, int start, int end )
    {
        ADJUST_INDICES(start, end, (int) str.size());
        
        std::string::size_type result = str.rfind( sub, end );
        
        if( result == std::string::npos ||
           result < (std::string::size_type)start  ||
           (result + sub.size() > (std::string::size_type)end))
            return -1;
        
        return (int)result;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    int rindex( const std::string & str, const std::string & sub, int start, int end )
    {
        return rfind( str, sub, start, end );
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    int count( const std::string & str, const std::string & substr, int start, int end )
    {
        int nummatches = 0;
        int cursor = start;
        
        while ( 1 )
        {
            cursor = find( str, substr, cursor, end );
            
            if ( cursor < 0 ) break;
            
            cursor += (int) substr.size();
            nummatches += 1;
        }
        
        return nummatches;
        
        
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 将字符串中的某种子串替换为新子串
    ///
    
    std::string replace( const std::string & str, const std::string & oldstr, const std::string & newstr, int count )
    {
        int sofar = 0;
        int cursor = 0;
        std::string s( str );
        
        std::string::size_type oldlen = oldstr.size(), newlen = newstr.size();
        
        cursor = find( s, oldstr, cursor );
        
        while ( cursor != -1 && cursor <= (int)s.size() )
        {
            if ( count > -1 && sofar >= count )
            {
                break;
            }
            
            s.replace( cursor, oldlen, newstr );
            cursor += (int) newlen;
            
            if ( oldlen != 0)
            {
                cursor = find( s, oldstr, cursor );
            }
            else //对于oldstr为空字符串的情况
            {
                ++cursor;
            }
            
            ++sofar;
        }
        return s;
    }
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 将字符串按行切分
    ///
    void splitlines(  const std::string & str, std::vector< std::string > & result, bool keepends )
    {
        result.clear();
        std::string::size_type len = str.size(), i, j, eol;
        
        for (i = j = 0; i < len; )
        {
            while (i < len && str[i] != '\n' && str[i] != '\r')
                i++;
            
            eol = i;
            if (i < len)
            {
                if (str[i] == '\r' && i + 1 < len && str[i+1] == '\n')
                {
                    i += 2;
                }
                else
                {
                    i++;
                }
                if (keepends)
                    eol = i;
                
            }
            result.push_back( str.substr( j, eol - j ) );
            j = i;
        }
        
        if (j < len)
        {
            result.push_back( str.substr( j, len - j ) );
        }
        
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// 重复n份连接起来
    ///
    std::string mul( const std::string & str, int n )
    {
        if (n <= 0) return "";
        if (n == 1) return str;
        
        std::ostringstream os;
        for(int i=0; i<n; ++i)
            os << str;
        return os.str();
    }
    
}//namespace stringlib


