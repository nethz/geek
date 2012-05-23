/*
-----------------------------------------------------------------------------
This source file is a part of geek
(Game Engine Extensible Kit)
For the latest info, see http://gdgeek.com/

GEEK (www.gdgeek.com) is made available under the MIT License.

Copyright (c) 2010-2011 http://gdgeek.com/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

////////////////////////////////////////////////////////////////////////
///  
/// @file       exception.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_EXCEPTION_HPP
#define GEEK_TOOLKITS_EXCEPTION_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/exception/config.hpp>
#include <boost/version.hpp>
//#include <boost/exception/diagnostic_information.hpp>


namespace geek
{
//{英文翻译};用于异常储存的类型和文本
typedef boost::error_info<struct tag_errno_type, std::string> geek_errno_type;
typedef boost::error_info<struct tag_errno_text, std::string> geek_errno_info_text;




//{英文翻译};用于适配不同的boost版本区别
template<class T, int Branch = static_cast<int>(BOOST_VERSION >= 103900) >
class exception_return{};

template<class T>
class exception_return<T, true>
{
public: typedef T*  type;
};

template<class T>
class exception_return<T, false>
{
public: typedef boost::shared_ptr<T> type;
};




///////////////////////////////////////////////////////////////////////
/// @brief      excetion codes; 异常类型
///
/// @details    {翻译英文}; 这个枚举用于区别异常类型 
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////
enum exception_codes {
	err_cannot_write_to_file,//无法写入文件
	err_invalid_state,//错误的状态
	err_invalid_params,//错误的参数
	err_duplicate_item,//错误的副本
	err_item_not_found,//没有找到资源
	err_file_not_found,//没有找到文件
	err_internal_error,//内部错误
	err_rt_assertion_failed,//运行时断言失败
	err_not_implemented,//没有执行
	err_no_definition,//没定义
};


///////////////////////////////////////////////////////////////////////
/// @brief    {翻译英文}; 异常类型映射到字符串
///
/// @details  {翻译英文}; 通过模板特化来影射.
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////
template <int ErrCode = err_no_definition> 
class exception_codes_text
{
public:
	static std::string const& text(void)
	{
		static std::string const text("no definition");
		return text;
	}
};

#define GEEK_EXCEPTION_CODE_TEXT(Enum, Text)  \
template <> \
class exception_codes_text<Enum>\
{\
public:\
	static std::string const& text(void)\
	{\
		static std::string const text(Text);\
		return text;\
	}\
};\

GEEK_EXCEPTION_CODE_TEXT(err_cannot_write_to_file, "cannot write to file")
GEEK_EXCEPTION_CODE_TEXT(err_invalid_state, "invalid state")
GEEK_EXCEPTION_CODE_TEXT(err_invalid_params, "invalid params")
GEEK_EXCEPTION_CODE_TEXT(err_duplicate_item, "duplicate item")
GEEK_EXCEPTION_CODE_TEXT(err_item_not_found, "item not found")
GEEK_EXCEPTION_CODE_TEXT(err_file_not_found, "file not found")
GEEK_EXCEPTION_CODE_TEXT(err_internal_error, "internal error")
GEEK_EXCEPTION_CODE_TEXT(err_rt_assertion_failed, "run time assertion failed")
GEEK_EXCEPTION_CODE_TEXT(err_not_implemented, "not implemented")



///////////////////////////////////////////////////////////////////////
/// @brief      geek exception class; geek异常基类
///
/// @details    {翻译英文}; 实现了std::exception的what函数
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////
class geek_exception : public boost::exception, public std::exception
{
public:

	geek_exception(void)
	{
	}

	virtual ~geek_exception(void) throw()
	{
	}


	/////////////////////////////////////////////////
	/// @brief     {英文翻译}:重载 std::exception::what
	/// @details   
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	const char* what(void) const throw()
	{	
		//{英文翻译}; boost提供了适配what的函数 
		return diagnostic_information_what(*this);
	}
	
	/////////////////////////////////////////////////
	/// @brief     {英文翻译}:保证再次抛出类型正确
	/// @details   
	/// @param text {英文翻译}:再次抛出时候附加的信息
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	virtual void throw_again(std::string const& text){}
protected:
	std::string type_;
	mutable std::string text_;
};

///////////////////////////////////////////////////////////////////////
/// @brief      exception template class; 异常模板类
///
/// @details    {翻译英文}; 末班参数是 enum exception_codes,但可以扩展
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////
template <int ErrCode = err_no_definition> 
class exception: public geek_exception
{

public:

	
	exception(void)
	{
		///{英文翻译}; 记录类型信息
		(*this)<<geek_errno_type(exception_codes_text<ErrCode>::text());
	}
/*
	virtual ~exception(void) thorw()
	{

	}
*/
	/////////////////////////////////////////////////
	/// @brief     {英文翻译}:保证再次抛出类型正确
	/// @details   
	/// @param text {英文翻译}:再次抛出时候附加的信息
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	virtual void throw_again(std::string const& text)
	{
		if(geek::exception_return<std::string const>::type err = boost::get_error_info<geek_errno_info_text>(*this) )
		{
			(*this) << ::geek::geek_errno_info_text(*err + text);
		}
		boost::throw_exception(*this);
	}
	

};




}

//////////////////////////////////////////////////////////////////////////
///{英文翻译}; 用于抛出异常的宏
#ifndef GEEK_THROW_EXCEPTION
#define GEEK_THROW_EXCEPTION(type , text)\
{\
	::geek::exception<type> e;\
	e << ::geek::geek_errno_info_text(text);\
	BOOST_THROW_EXCEPTION(e);\
}
#endif

///{英文翻译}; 在已抛出一场中加入新的信息再次抛出的宏
#ifndef GEEK_THROW_EXCEPTION_AGAIN
#define GEEK_THROW_EXCEPTION_AGAIN(e , text)\
{\
	geek_exception* p = dynamic_cast<geek_exception* >(&(e));\
	if(p)\
	{\
		p->throw_again(text);\
	}\
	else\
	{\
		throw e;\
	}\
}

#endif


#endif // GEEK_TOOLKITS_EXCEPTION_HPP
