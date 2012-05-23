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
#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/dyn_lib_manager/dyn_lib.hpp>
#include "dyn_lib_impl.hpp"
using namespace geek;


// void dyn_lib::load(void)
// {
// 	impl_->load();
// }
// /** Unload the library
// */
// void dyn_lib::unload(void)
// {
// 	impl_->unload();
// }

/** Default constructor - used by DynLibManager.
@warning
Do not call directly
*/
dyn_lib::dyn_lib(std::string const& name):impl_(new dyn_lib_impl(name))
{
	impl_->load();
}

//DynLib(const DynLib& dl);
//DynLib& operator = (const DynLib& dl);
//void swap(DynLib& dl) throw();

/** Default destructor.
*/
dyn_lib::~dyn_lib(void)
{
	try{

		impl_->unload();
	}catch(std::exception& e)
	{
		GEEK_LOG_ERROR_MESSAGE(e.what());
	}catch(...)
	{
		GEEK_LOG_ERROR_MESSAGE("can't unload dyn lib: "+ impl_->get_name());
	}
}


/// Get the name of the library
std::string const& dyn_lib::get_name(void) const
{
	return impl_->get_name();
}

/**
Returns the address of the given symbol from the loaded library.
@param
strName The name of the symbol to search for
@returns
If the function succeeds, the returned value is a handle to
the symbol.
@par
If the function fails, the returned value is <b>NULL</b>.

*/
void* dyn_lib::get_symbol(std::string const& str_name) const throw()
{
	return impl_->get_symbol(str_name);
}
