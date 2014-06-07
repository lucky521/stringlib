
#ifndef INCLUDED_STRINGLIB_H
#define INCLUDED_STRINGLIB_H

#include <string>
#include <vector>

namespace stringlib
{
    #define MAX_32BIT_INT 2147483647

    // 字符串属性判断
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断字符串是否仅由数字和字母组成
    ///
    bool isalnum( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断字符串是否仅由字母组成
    ///
    bool isalpha( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断字符串是否仅由数字组成
    ///
    bool isdigit( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断字符串是否仅由小写字母组成
    ///
    bool islower( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断字符串是否仅由大写字母组成
    ///
    bool isupper( const std::string & str );
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断字符串是否仅由空格符组成
    ///
    bool isspace( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断是否字符串中的每个单词都为首字符大写形式
    ///
    bool istitle( const std::string & str );
    
    
    // 子串与母串
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 返回子串的出现次数
    ///
    int count( const std::string & str, const std::string & substr, int start = 0, int end = MAX_32BIT_INT);

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判定是否为后缀子串
    ///
    bool endswith( const std::string & str, const std::string & suffix, int start = 0, int end = MAX_32BIT_INT );
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 判断是否为前缀子串
    ///
    bool startswith( const std::string & str, const std::string & prefix, int start = 0, int end = MAX_32BIT_INT );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 字符串匹配查找
    ///
    int find( const std::string & str, const std::string & sub, int start = 0, int end = MAX_32BIT_INT  );
    int index( const std::string & str, const std::string & sub, int start = 0, int end = MAX_32BIT_INT  );

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 字符串匹配查找（从右向左查找）
    ///
    int rfind( const std::string & str, const std::string & sub, int start = 0, int end = MAX_32BIT_INT );
    int rindex( const std::string & str, const std::string & sub, int start = 0, int end = MAX_32BIT_INT );
 
    
    
    //字符串形式变换
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 返回字符串的首字母大写形式
    ///
    std::string capitalize( const std::string & str );

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 字符串左侧补0
    ///
    std::string zfill( const std::string & str, int width );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 返回长度为width的居中形式
    ///
    std::string center( const std::string & str, int width );
   
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the string left justified in a string of length width. Padding is done using
    /// spaces. The original string is returned if width is less than str.size().
    ///
    std::string ljust( const std::string & str, int width );

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return the string right justified in a string of length width. Padding is done using
    /// spaces. The original string is returned if width is less than str.size().
    ///
    std::string rjust( const std::string & str, int width);
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string converted to lowercase.
    ///
    std::string lower( const std::string & str );

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Return a copy of the string converted to uppercase.
    ///
    std::string upper( const std::string & str );

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 去除字符串左右两侧的特定字符
    ///
    std::string strip( const std::string & str, const std::string & chars = "" );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 去除字符串左侧的特定字符
    ///
    std::string lstrip( const std::string & str, const std::string & chars = "" );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 去除字符串右侧的特定字符
    ///
    std::string rstrip( const std::string & str, const std::string & chars = "" );

    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 字符串字母大小写互换
    ///
    std::string swapcase( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 将字符串中的每个单词首字母大写
    ///
    std::string title( const std::string & str );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 字符串加密
    ///
    std::string translate( const std::string & str, const std::string & table, const std::string & deletechars = "");
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 替换字符串中的特定子串
    //
    std::string replace( const std::string & str, const std::string & oldstr, const std::string & newstr, int count = -1);
    
    
    
    //字符串连接和分割
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 将字符串重复连接n份
    ///
    std::string mul( const std::string & str, int n);
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 按分隔符将字符串划分为三部分：左串、分割串、右串
    ///
    void partition( const std::string & str, const std::string & sep, std::vector< std::string > & result );
    void rpartition( const std::string & str, const std::string & sep, std::vector< std::string > & result );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 按分隔符分割字符串，默认按空格符分割。
    ///
    void split( const std::string & str, std::vector< std::string > & result, const std::string & sep = "", int maxsplit = -1);
    void rsplit( const std::string & str, std::vector< std::string > & result, const std::string & sep = "", int maxsplit = -1);
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 字符串按行切割
    ///
    void splitlines(  const std::string & str, std::vector< std::string > & result, bool keepends = false );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 用指定的连接符将数组中的字符串连接起来
    ///
    std::string join( const std::string & str, const std::vector< std::string > & seq );
    
    //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief 根据指定下标切割出子串
    ///
    std::string slice( const std::string & str, int start = 0, int end = MAX_32BIT_INT);
        
} // namespace

#endif
