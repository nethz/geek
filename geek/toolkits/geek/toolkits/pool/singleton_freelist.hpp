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
/// @file       singleton_freelist.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/22
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_SINGLETON_FREELIST_HPP
#define GEEK_TOOLKITS_SINGLETON_FREELIST_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <geek/toolkits/pool/freelist.hpp>
#include <geek/toolkits/pool/freelist_policy_mt.hpp>
#include <geek/toolkits/pool/freelist_policy_st.hpp>
#include <boost/pool/detail/singleton.hpp>

namespace geek
{
template <
	typename tag,
	typename obj_t, 
	std::size_t n,
	std::size_t grow_size,
	typename user_alloc_t = boost::default_user_allocator_new_delete 
	>
struct singleton_freelist_st
{
private:
	template <typename tag_t>
	struct freelist_type
	{
		freelist_type()
			: p(n, grow_size) 
			, fl(p)
		{ 
		}

		typedef freelist_policy_st<obj_t, user_alloc_t> freelist_policy_t;
		freelist_policy_t p;
		freelist<obj_t, freelist_policy_t> fl;
	};

	typedef boost::details::pool::singleton_default<freelist_type<tag> > singleton;

	singleton_freelist_st();

public:
	static obj_t* construct(void)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct();
	}

	static obj_t* construct(std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct(n);
	}

	template <typename arg_t>
	static obj_t* construct(typename boost::call_traits<arg_t>::param_type arg)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct(arg);
	}

	template <typename arg_t>
	static obj_t* construct(typename boost::call_traits<arg_t>::param_type arg, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct(arg, n);
	}

	static void destruct(obj_t* o)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.destruct(o);
	}

	static void destruct(obj_t* o, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.destruct(o, n);
	}

	static obj_t* allocate()
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.allocate();
	}

	static obj_t* allocate(std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.allocate(n);
	}

	static void deallocate(obj_t* o)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.deallocate(o);
	}

	static void deallocate(obj_t* o, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.deallocate(o, n);
	}

	static bool is_from(obj_t* o)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.is_from(o);
	}

	static bool is_from(obj_t* o, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.is_from(o, n);
	}
};
//-------------------------------------------------------------------------------------------------
// singleton_freelist_mt
//-------------------------------------------------------------------------------------------------
template <
	typename tag,
	typename obj_t, 
	std::size_t thread_num,
	std::size_t cache_page_num, 
	std::size_t page_size,
	std::size_t page_grow_size,
	typename user_alloc_t = boost::default_user_allocator_new_delete 
	>
struct singleton_freelist_mt
{
private:
	template <typename tag_t>
	struct freelist_type
	{
		freelist_type()
			: p(thread_num, cache_page_num, page_size, page_grow_size) 
			, fl(p)
		{ 
		}

		typedef freelist_policy_mt<obj_t, user_alloc_t> freelist_policy_t;
		freelist_policy_t p;
		freelist<obj_t, freelist_policy_t> fl;
	};

	typedef boost::details::pool::singleton_default<freelist_type<tag> > singleton;

	singleton_freelist_mt();

public:
	static obj_t* construct(void)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct();
	}

	static obj_t* construct(std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct(n);
	}

	template <typename arg_t>
	static obj_t* construct(typename boost::call_traits<arg_t>::param_type arg)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct(arg);
	}

	template <typename arg_t>
	static obj_t* construct(typename boost::call_traits<arg_t>::param_type arg, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.construct(arg, n);
	}

	static void destruct(obj_t* o)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.destruct(o);
	}

	static void destruct(obj_t* o, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.destruct(o, n);
	}

	static obj_t* allocate()
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.allocate();
	}

	static obj_t* allocate(std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.allocate(n);
	}

	static void deallocate(obj_t* o)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.deallocate(o);
	}

	static void deallocate(obj_t* o, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.deallocate(o, n);
	}

	static bool is_from(obj_t* o)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.is_from(o);
	}

	static bool is_from(obj_t* o, std::size_t const n)
	{
		freelist_type<tag>& fl = singleton::instance();
		return fl.fl.is_from(o, n);
	}
};
}
#endif // GEEK_TOOLKITS_SINGLETON_FREELIST_HPP



