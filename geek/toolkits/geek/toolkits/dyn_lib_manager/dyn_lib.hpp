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
/// @file       dyn_lib.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/24
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_DYN_LIB_MANAGER_DYN_LIB_HPP
#define GEEK_TOOLKITS_DYN_LIB_MANAGER_DYN_LIB_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/toolkits/dyn_lib_manager/config.hpp>
#include <boost/scoped_ptr.hpp>
namespace geek {

	class dyn_lib_impl;
//	template class GEEK_TOOLKITS_DYN_LIB_MANAGER_DECL boost::scoped_ptr<dyn_lib_impl>;
	/** Resource holding data about a dynamic library.
	@remarks
	This class holds the data required to get symbols from
	libraries loaded at run-time (i.e. from DLL's for so's)
	@author
	Adrian Cearn鉼 (cearny@cearny.ro)
	@since
	27 January 2002
	@see
	Resource
	*/
	class GEEK_TOOLKITS_DYN_LIB_MANAGER_DECL dyn_lib//: public boost::noncopyable
	{
	public:


// 		/** Load the library
// 		*/
// 		void load(void);
// 		/** Unload the library
// 		*/
// 		void unload(void);

		/** Default constructor - used by DynLibManager.
		@warning
		Do not call directly
		*/
		dyn_lib(std::string const& name);

		//DynLib(const DynLib& dl);
		//DynLib& operator = (const DynLib& dl);
		//void swap(DynLib& dl) throw();

		/** Default destructor.
		*/
		~dyn_lib(void);


		/// Get the name of the library
		std::string const& get_name(void) const;

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
		void*  get_symbol(std::string const& str_name ) const throw();

	private:



		boost::scoped_ptr<dyn_lib_impl> impl_;
	};
}
#endif // GEEK_TOOLKITS_DYN_LIB_MANAGER_DYN_LIB_HPP



