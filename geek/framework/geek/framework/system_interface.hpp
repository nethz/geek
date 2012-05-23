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
/// @file       system_interface.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/10/01
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_FRAMEWORK_SYSTEM_INTERFACE_HPP
#define GEEK_FRAMEWORK_SYSTEM_INTERFACE_HPP
// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif
#include <geek/framework/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <geek/prerequisites.hpp>
namespace geek{


	class GEEK_FRAMEWORK_DECL system_interface: public singleton<system_interface>
	{

	public:
		system_interface(void);
		virtual ~system_interface(void);

		///启动系统循环
		virtual void run(void) = 0;

		///推出系统运行

		virtual void exit(void) = 0;

		virtual bool is_running(void) const = 0;

		///依次初始化每个子系统
		virtual bool init(void) = 0;
		///依次关闭每个子系统
		virtual void shutdown(void) = 0;
		///依次更新所有子系统，参数是两次调用的时间间隔
		virtual void update(geek::duration_type const& interval) = 0;

		virtual bool is_init(void) const = 0;	
		//得到两次调用的时间间隔
		virtual duration_type const& get_interval(void) = 0;
	
		///重置时间
		virtual void reset(void) = 0;
	};


}
#endif // GEEK_FRAMEWORK_SYSTEM_INTERFACE_HPP


