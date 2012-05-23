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
/// @file       plugins_manager.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/24
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_DYN_LIB_MANAGER_PLUGINS_MANAGER_HPP
#define GEEK_TOOLKITS_DYN_LIB_MANAGER_PLUGINS_MANAGER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/toolkits/dyn_lib_manager/config.hpp>
#include <geek/toolkits/singleton.hpp>
#include <boost/scoped_ptr.hpp>
namespace geek {
	class dyn_lib;

	typedef boost::shared_ptr<dyn_lib> dyn_lib_ptr;
	class GEEK_TOOLKITS_DYN_LIB_MANAGER_DECL plugins_manager : public singleton<plugins_manager>
	{

		typedef std::vector<dyn_lib_ptr> dyn_lib_list_t;
	public:
		plugins_manager(void);
		~plugins_manager(void);

	public:
		bool init(void);

		void shutdown(void);

	public:
		void clear(void);

		void load_plugin_from_file(std::string const& file_name);
		void load_plugin(std::string const& plugin_name);
		void unload_plugin(std::string const& plugin_name);

	private:
		bool find_plugin_by_name(dyn_lib_ptr plugin, std::string const* plugin_name);

	private:
		dyn_lib_list_t plugin_lib_list_;
		bool is_initialised_;
	};

}
#endif // GEEK_TOOLKITS_DYN_LIB_MANAGER_PLUGINS_MANAGER_HPP



