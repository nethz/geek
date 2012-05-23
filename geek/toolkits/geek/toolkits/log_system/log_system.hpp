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
/// @file       log_system.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_LOG_SYSTEM_HPP
#define GEEK_TOOLKITS_LOG_SYSTEM_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/log_system/config.hpp>
#include <boost/thread/tss.hpp>
#include <boost/format.hpp>

namespace geek
{

class log_stream;

///////////////////////////////////////////////////////////////////////
/// @brief      log system ; 日志系统
///
/// @details    {英文翻译} ; 多线程安全
/// @date       2011/09/25
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_LOG_SYSTEM_DECL log_system
{
public:

	///////////////////////////////////////////////////////////////////////
	/// @brief      severity level ; 日志等级
	///
	/// @details    
	/// @date       2011/09/25
	///////////////////////////////////////////////////////////////////////
	enum severity_level
	{
		trace,
		normal,
		notification,
		warning,
		error,
		critical
	};

	typedef boost::log::sources::severity_logger<log_system::severity_level> log_type;
	typedef boost::log::sinks::sink<char> sink_type;
	typedef boost::shared_ptr<sink_type> sink_type_ptr;

public:
	
	/////////////////////////////////////////////////
	/// @brief     sample singleton ; 简单单件
	/// @details   
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	static log_system& get_instance(void);


	/////////////////////////////////////////////////
	/// @brief     sample singleton ; 简单单件
	/// @details   
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	static log_system* get_instance_ptr(void);


	/////////////////////////////////////////////////
	/// @brief     get stream ; 得到流
	/// @details   Thread safe ; 线程安全
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	std::ostream& stream(void);

	virtual ~log_system(void);


	log_system::log_type& get_global_log(void);



	/////////////////////////////////////////////////
	/// @brief     output log ;  输出日志 
	/// @details   
	/// @param message log ; 日志内容
	/// @param level log level ; 日志等级
	/// @param mask_debug olny debug? ; 只在debug模式下输出
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	void log_message(std::string const& message, log_system::severity_level level = log_system::warning, bool mask_debug = false);


	/// add sink ; 添加sink
	void add_sink(sink_type_ptr const& sink);

	///remove sink ; 移除sink
	void remove_sink(sink_type_ptr const& sink);


	/////////////////////////////////////////////////
	/// @brief     end the stream and output ; 结束并输出流
	/// @details   
	/// @param level log level ; 日志等级
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	void stream_endl(severity_level level);


private:
	log_system(void);
	void create_tss_log_stream();

	typedef boost::thread_specific_ptr<log_type> log_type_tss;
	typedef boost::thread_specific_ptr<log_stream> log_stream_tss;

private:
	log_type_tss global_log_;
	log_stream_tss log_stream_;
};


/// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, log_system::severity_level lvl)
{
	static const char* const str[] =
	{
		"Trace",
		"Normal",
		"Notification",
		"Warning",
		"Error",
		"Critical"
	};
	if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else                                                                                              
		strm << static_cast< int >(lvl);
	return strm;
}

	
	///////////////////////////////////////////////////////////////////////
	/// @brief     {英文翻译} ; 用于流输出结束
	///
	/// @details    
	/// @date       2011/09/25
	///////////////////////////////////////////////////////////////////////
	template <int level>
	class log_level
	{
	public:
		template<class _Elem,	class _Traits> inline
		static std::basic_ostream<_Elem, _Traits>& endl(std::basic_ostream<_Elem, _Traits>& _Ostr)
		{
			log_system::get_instance().stream_endl(static_cast<log_system::severity_level>(level));
			return _Ostr;
		}
	};
	
	
}


#ifdef GEEK_NO_LOG

#	define GEEK_LOG /##*
#	define GEEK_LOG_FILE_MESSAGE /##*
#	define GEEK_NORMAL *##/
#	define GEEK_NOTIFICATION *##/
#	define GEEK_WARNING *##/
#	define GEEK_TRACE *##/
#	define GEEK_ERROR *##/
#	define GEEK_CRITICAL *##/
#	define GEEK_LOG_MESSAGE(MSG, level) /##/

#else//GEEK_NO_LOG

#	define GEEK_LOG ( ::geek::log_system::get_instance().stream())
#	define GEEK_LOG_FILE_MESSAGE GEEK_LOG<<GEEK_FILE_MESSAGE
#	define GEEK_NORMAL ( ::geek::log_level< ::geek::log_system::normal>::endl)
#	define GEEK_NOTIFICATION ( ::geek::log_level< ::geek::log_system::notification>::endl)
#	define GEEK_WARNING ( ::geek::log_level< ::geek::log_system::warning>::endl)
#	define GEEK_TRACE ( ::geek::log_level< ::geek::log_system::trace>::endl)
#	define GEEK_ERROR ( ::geek::log_level< ::geek::log_system::error>::endl)
#	define GEEK_CRITICAL ( ::geek::log_level< ::geek::log_system::critical>::endl)
#	define GEEK_LOG_MESSAGE(MSG, level) (::geek::log_system::get_instance().log_message(MSG, level))

#endif//GEEK_NO_LOG


#define GEEK_FILE_MESSAGE ((boost::format("{FILE:%1%(%2%)} ")%__FILE__%__LINE__).str())

#define GEEK_LOG_TRACE_MESSAGE(MSG)  GEEK_LOG_MESSAGE((MSG), ::geek::log_system::trace) 
#define GEEK_LOG_NORMAL_MESSAGE(MSG)  GEEK_LOG_MESSAGE((MSG), ::geek::log_system::normal) 
#define GEEK_LOG_NOTIFICATION_MESSAGE(MSG)  GEEK_LOG_MESSAGE((MSG), ::geek::log_system::notification) 
#define GEEK_LOG_WARNING_MESSAGE(MSG)  GEEK_LOG_MESSAGE((MSG), ::geek::log_system::warning) 
#define GEEK_LOG_ERROR_MESSAGE(MSG)  GEEK_LOG_MESSAGE(GEEK_FILE_MESSAGE+ MSG, ::geek::log_system::error)
#define GEEK_LOG_CRITICAL_MESSAGE(MSG)  GEEK_LOG_MESSAGE(GEEK_FILE_MESSAGE + MSG, ::geek::log_system::critical)


#endif // GEEK_TOOLKITS_LOG_SYSTEM_HPP
