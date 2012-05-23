
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
#include "dyn_lib_impl.hpp"
#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/exception.hpp>

using namespace geek;



#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
#  define WIN32_LEAN_AND_MEAN
#  if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#  endif
#  include <windows.h>
#endif

#if GEEK_PLATFORM == GEEK_PLATFORM_APPLE || GEEK_PLATFORM == GEEK_PLATFORM_IPHONE
#   include <dlfcn.h>
#endif

#if GEEK_PLATFORM == GEEK_PLATFORM_IPHONE
// Basically a dummy function.  Dynamic libraries aren't supported on iPhone

#endif 


#if GEEK_PLATFORM == GEEK_PLATFORM_APPLE 
#include <geek/toolkits/platform/osx/mac_utils.hpp>
#endif


//-----------------------------------------------------------------------
dyn_lib_impl::dyn_lib_impl(std::string const& name)
{
	name_ = name;
	h_inst_ = NULL;
}

//-----------------------------------------------------------------------
dyn_lib_impl::~dyn_lib_impl()
{
}

//-----------------------------------------------------------------------
void dyn_lib_impl::load()
{
	// Log library load
	log_system::get_instance().log_message("Loading library " + name_, log_system::normal);
	//LogManager::getSingleton().logMessage("Loading library " + mName);

	std::string name = name_;
#if GEEK_PLATFORM == GEEK_PLATFORM_LINUX
	// dlopen() does not add .so to the filename, like windows does for .dll
	if (name.substr(name.length() - 3, 3) != ".so")
		name += ".so";
#elif GEEK_PLATFORM == GEEK_PLATFORM_APPLE
	// dlopen() does not add .dylib to the filename, like windows does for .dll
	if (name.substr(name.length() - 6, 6) != ".dylib")
		name += ".dylib";
#elif GEEK_PLATFORM == GEEK_PLATFORM_WIN32
	// Although LoadLibraryEx will add .dll itself when you only specify the library name,
	// if you include a relative path then it does not. So, add it to be sure.
	if (name.substr(name.length() - 4, 4) != ".dll")
		name += ".dll";

#	ifdef UNICODE
		std::wstring utf16;
		::utf8::utf8to16(name.begin(), name.end(), std::back_inserter(utf16));
		h_inst_ = (DYNLIB_HANDLE)DYNLIB_LOAD( utf16.c_str() );
#	else
		h_inst_ = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );
#	endif
#endif
	//!!h_inst_ = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );

	if( !h_inst_ )
		GEEK_THROW_EXCEPTION(
		geek::err_internal_error, 
		"Could not load dynamic library " + name_ + ".  System Error: " + dynlib_error()
		);
}

//-----------------------------------------------------------------------
void dyn_lib_impl::unload()
{
	// Log library unload

	log_system::get_instance().log_message("Unloading library " + name_, log_system::normal);
//	LogManager::getSingleton().logMessage("Unloading library " + _name);

	if( DYNLIB_UNLOAD( h_inst_ ) )
	{
		GEEK_THROW_EXCEPTION(
			geek::err_internal_error, 
			"Could not unload dynamic library " + name_ + ".  System Error: " + dynlib_error()
			);
	}

}

//-----------------------------------------------------------------------
void* dyn_lib_impl::get_symbol(std::string const& str_name ) const
{
	void* symbol = (void*)DYNLIB_GETSYM( h_inst_, str_name.c_str() );
	if (!symbol)
	{
		//GEEK_LOG /*<< "Could not get symbol: " << str_name << " in plugin: " << name_ << ". System Error: " << dynlib_error()*/ << GEEK_ERROR;
	}
	return symbol;
}
//-----------------------------------------------------------------------
std::string dyn_lib_impl::dynlib_error(void) const
{
#if GEEK_PLATFORM == GEEK_PLATFORM_WIN32
	LPVOID lpMsgBuf; 
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL, 
		GetLastError(), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR)& lpMsgBuf, 
		0, 
		NULL 
		); 
	std::string ret = (char*)lpMsgBuf;
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return ret;
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	return std::string(dlerror());
#else
	return std::string("");
#endif
}
