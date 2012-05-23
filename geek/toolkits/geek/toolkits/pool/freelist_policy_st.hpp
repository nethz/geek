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
/// @file       freelist_policy_st.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/21
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_FREELIST_POLICY_ST_HPP
#define GEEK_TOOLKITS_FREELIST_POLICY_ST_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <geek/toolkits/pool/allocator.hpp>
#include <boost/foreach.hpp>
#include <boost/static_assert.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      freelist_policy_st ; 自由列表单线程池分配器
/// @date       2011/10/21
///////////////////////////////////////////////////////////////////////
template <
	typename obj_t, 
	typename user_alloc_t = boost::default_user_allocator_new_delete 
	>
class freelist_policy_st : private boost::noncopyable
{
public:
	typedef obj_t value_type;
	typedef user_alloc_t user_allocator;
	typedef value_type* pointer;
	typedef boost::pool<user_allocator> pool_t;
	typedef typename pool_t::size_type size_type;

public:
	explicit freelist_policy_st(
		size_type const n = 0, 
		size_type const grow_size = 32
		);

	~freelist_policy_st();

public:
	// no throw, if grow_size_ > 0 may throw std::bad_alloc; if grow_size_ == 0, O(1)
	obj_t* allocate(void);
	obj_t* allocate(std::size_t const n);

	// no throw; O(1)
	void deallocate(obj_t* o);
	void deallocate(obj_t* o, std::size_t const n);

	// no throw
	bool is_from(obj_t* o);
	bool is_from(obj_t* o, std::size_t const n);

private:
	pool_t pool_;
};
//-------------------------------------------------------------------------------------------------
// freelist_policy_st impl
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline freelist_policy_st<obj_t, user_alloc_t>::freelist_policy_st(
	size_type const n, 
	size_type const grow_size
	)
	: pool_(sizeof(obj_t), grow_size)
{
	// reserve
	if (n > 0)
	{
		std::vector<void*> tmp(n);
		BOOST_FOREACH(void*& v, tmp)
		{
			v = pool_.malloc();
		}
		BOOST_REVERSE_FOREACH(void* v, tmp)
		{
			pool_.free(v);
		}
	}
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline freelist_policy_st<obj_t, user_alloc_t>::~freelist_policy_st()
{
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline obj_t* freelist_policy_st<obj_t, user_alloc_t>::allocate(void)
{
	return static_cast<pointer>(pool_.malloc());
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline obj_t* freelist_policy_st<obj_t, user_alloc_t>::allocate(std::size_t const n)
{
	if (n == 0)
	{
		return NULL;
	}

	if (n == 1)
	{
		return allocate();
	}

	return static_cast<pointer>(pool_.ordered_malloc(n));
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline void freelist_policy_st<obj_t, user_alloc_t>::deallocate(obj_t* o)
{
	pool_.free(o);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline void freelist_policy_st<obj_t, user_alloc_t>::deallocate(obj_t* o, std::size_t const n)
{
	if (n == 0)
	{
		return;
	}

	if (n == 1)
	{
		deallocate(o);
		return;
	}

	pool_.free(o, n);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline bool freelist_policy_st<obj_t, user_alloc_t>::is_from(obj_t* o)
{
	return pool_.is_from(o);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline bool freelist_policy_st<obj_t, user_alloc_t>::is_from(obj_t* o, std::size_t const /*n*/)
{
	return pool_.is_from(o);
}
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_FREELIST_POLICY_ST_HPP



