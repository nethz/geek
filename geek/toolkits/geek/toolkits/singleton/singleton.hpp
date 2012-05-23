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
/// @file       singleton.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_SINGLETON_HPP
#define GEEK_TOOLKITS_SINGLETON_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/singleton/config.hpp>



namespace geek{

	///////////////////////////////////////////////////////////////////////
	/// @brief      {英文翻译}:一个可以手动控制生命期的单价模式
	///
	/// @details    
	/// @date       2011/09/24
	///////////////////////////////////////////////////////////////////////
	template<typename T> class singleton//: private boost::noncopyable
	{
	protected:
		static T* singleton_;
	public:
		singleton(void)
		{
			assert( !singleton_ );
#if defined( _MSC_VER ) && _MSC_VER < 1200
			int offset = (int)(T*)1 - (int)(singleton <T>*)(T*)1;
			singleton_ = (T*)((int)this + offset);
#else
			singleton_ = static_cast< T* >( this );
#endif
		}


		virtual ~singleton(void)
		{
			assert(singleton_);
			singleton_ = NULL;
		}

		///{英文翻译}:得到单件引用
		static T& get_singleton(void)
		{
			assert(singleton_);
			return(*singleton_);
		}

		///{英文翻译}:得到单件指针
		static T* get_singleton_ptr(void)
		{
			return (singleton_);
		}

	};

	///////////////////////////////////////////////////////////////////////
	/// @brief      {英文翻译}:一个非侵入的，可以手动空直生命期的单价模式
	///
	/// @details    
	/// @date       2011/09/24
	///////////////////////////////////////////////////////////////////////
	template<typename T>
	class singleton_non_invasive: public geek::singleton<singleton_non_invasive<T> >
	{
	public:
		inline singleton_non_invasive(T * obj):obj_(obj)
		{

		}
		virtual ~singleton_non_invasive(void)
		{

		}
		T& obj(void) const
		{
			return *obj_ptr();
		}

		T* obj_ptr(void) const
		{
			return obj_.get();
		}
	private:
		boost::scoped_ptr<T> obj_;
	};
}
#endif // GEEK_TOOLKITS_SINGLETON_CONFIG_HPP
