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
/// @file       freelist_allocator.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_FREELIST_ALLOCATOR_HPP
#define GEEK_TOOLKITS_FREELIST_ALLOCATOR_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <geek/toolkits/pool/singleton_freelist.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      boost::shared_ptr allocator st ; 使用freelist实现的单线程分配器
/// @date       2011/10/22
///////////////////////////////////////////////////////////////////////
struct freelist_allocator_st_tag {};
template <
	typename T, 
	std::size_t count,
	std::size_t grow_size = 32,
	typename tag = freelist_allocator_st_tag,
	typename user_alloc_t = boost::default_user_allocator_new_delete 
	>
class freelist_allocator_st
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type const* const_pointer;
	typedef value_type& reference;
	typedef value_type const& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	template <typename U>
	struct rebind
	{
		typedef freelist_allocator_st<U, count, grow_size, tag, user_alloc_t> other;
	};

public:
	freelist_allocator_st()
	{
		// Required to ensure construction of singleton_pool IFF an
		// instace of this allocator is constructed during global
		// initialization. See ticket #2359 for a complete explaination
		// ( http://svn.boost.org/trac/boost/ticket/2359 )
		singleton_freelist_st<tag, T, count, grow_size, user_alloc_t>::is_from(0);
	}

	// default copy constructor

	// default assignment operator

	// not explicit, mimicking std::allocator [20.4.1]
	template <typename U>
	freelist_allocator_st(freelist_allocator_st<U, count, grow_size, tag, user_alloc_t> const&)
	{
		// Required to ensure construction of singleton_pool IFF an
		// instace of this allocator is constructed during global
		// initialization. See ticket #2359 for a complete explaination
		// ( http://svn.boost.org/trac/boost/ticket/2359 )
		singleton_freelist_st<tag, T, count, grow_size, user_alloc_t>::is_from(0);
	}

	// default destructor

	static pointer address(reference r)
	{ 
		return &r; 
	}

	static const_pointer address(const_reference s)
	{ 
		return &s; 
	}

	static size_type max_size()
	{ 
		return (std::numeric_limits<size_type>::max)(); 
	}

	static void construct(pointer const ptr, value_type const& t)
	{ 
		new (ptr) T(t); 
	}

	static void destroy(pointer const ptr)
	{
		ptr->~T();
		(void) ptr; // avoid unused variable warning
	}

	bool operator==(freelist_allocator_st const&) const
	{ 
		return true; 
	}

	bool operator!=(freelist_allocator_st const&) const
	{ 
		return false; 
	}

	static pointer allocate(size_type const n)
	{
		return singleton_freelist_st<tag, T, count, grow_size, user_alloc_t>::allocate(n);
	}

	static pointer allocate(size_type const n, void const* const)
	{ 
		return allocate(n); 
	}

	static void deallocate(pointer const ptr, size_type const n)
	{
		singleton_freelist_st<tag, T, count, grow_size, user_alloc_t>::deallocate(ptr, n);
	}
};
///////////////////////////////////////////////////////////////////////
/// @brief      freelist allocator mt ; 使用freelist实现的多线程分配器
/// @date       2011/10/22
///////////////////////////////////////////////////////////////////////
struct freelist_allocator_mt_tag {};
template <
	typename T, 
	std::size_t thread_num,
	std::size_t cache_page_num, 
	std::size_t page_size,
	std::size_t page_grow_size = 32,
	typename tag = freelist_allocator_mt_tag,
	typename user_alloc_t = boost::default_user_allocator_new_delete
	>
class freelist_allocator_mt
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type const* const_pointer;
	typedef value_type& reference;
	typedef value_type const& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	template <typename U>
	struct rebind
	{
		typedef freelist_allocator_mt<U, thread_num, cache_page_num, page_size, page_grow_size, tag, user_alloc_t> other;
	};

public:
	freelist_allocator_mt()
	{
		// Required to ensure construction of singleton_pool IFF an
		// instace of this allocator is constructed during global
		// initialization. See ticket #2359 for a complete explaination
		// ( http://svn.boost.org/trac/boost/ticket/2359 )
		singleton_freelist_mt<tag, T, thread_num, cache_page_num, page_size, page_grow_size, user_alloc_t>::is_from(0);
	}

	// default copy constructor

	// default assignment operator

	// not explicit, mimicking std::allocator [20.4.1]
	template <typename U>
	freelist_allocator_mt(freelist_allocator_mt<U, thread_num, cache_page_num, page_size, page_grow_size, tag, user_alloc_t> const&)
	{
		// Required to ensure construction of singleton_pool IFF an
		// instace of this allocator is constructed during global
		// initialization. See ticket #2359 for a complete explaination
		// ( http://svn.boost.org/trac/boost/ticket/2359 )
		singleton_freelist_mt<tag, T, thread_num, cache_page_num, page_size, page_grow_size, user_alloc_t>::is_from(0);
	}

	// default destructor

	static pointer address(reference r)
	{ 
		return &r; 
	}

	static const_pointer address(const_reference s)
	{ 
		return &s; 
	}

	static size_type max_size()
	{ 
		return (std::numeric_limits<size_type>::max)(); 
	}

	static void construct(pointer const ptr, value_type const& t)
	{ 
		new (ptr) T(t); 
	}

	static void destroy(pointer const ptr)
	{
		ptr->~T();
		(void) ptr; // avoid unused variable warning
	}

	bool operator==(freelist_allocator_mt const&) const
	{ 
		return true; 
	}

	bool operator!=(freelist_allocator_mt const&) const
	{ 
		return false; 
	}

	static pointer allocate(size_type const n)
	{
		return singleton_freelist_mt<tag, T, thread_num, cache_page_num, page_size, page_grow_size, user_alloc_t>::allocate(n);
	}

	static pointer allocate(size_type const n, void const* const)
	{ 
		return allocate(n); 
	}

	static void deallocate(pointer const ptr, size_type const n)
	{
		singleton_freelist_mt<tag, T, thread_num, cache_page_num, page_size, page_grow_size, user_alloc_t>::deallocate(ptr, n);
	}
};
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_FREELIST_ALLOCATOR_HPP



