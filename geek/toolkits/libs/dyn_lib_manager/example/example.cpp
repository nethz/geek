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


#include "stable_headers.hpp"
#include "example.hpp"
#include <geek/toolkits/log_system.hpp>
int main(void)
{
	try{
		using namespace geek;
		//typedef geek::singleton2<geek::plugins_manager> plugins_manager_singleton;
		boost::shared_ptr<geek::plugins_manager> plugins_mgr(new plugins_manager());

#ifdef _DEBUG
		plugins_manager::get_singleton().load_plugin(std::string("test_dyn_lib_manager_plugin") + std::string(_DEBUG_POSTFIX));
#else
		plugins_manager::get_singleton().load_plugin(std::string("test_dyn_lib_manager_plugin"));
#endif
		plugins_manager::get_singleton_ptr()->init();
		plugins_mgr->shutdown();
		plugins_mgr.reset();
	}
	catch(std::exception& e)
	{
		GEEK_LOG<<e.what()<<GEEK_ERROR;
	}

}
