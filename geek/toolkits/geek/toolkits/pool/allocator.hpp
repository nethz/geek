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
/// @file       allocator.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/4
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_ALLOCATOR_HPP
#define GEEK_TOOLKITS_ALLOCATOR_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <boost/pool/pool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/exception/all.hpp>
#include <boost/static_assert.hpp>
#include <boost/foreach.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      allocator ; 标准C++分配器
/// @date       2011/10/4
///////////////////////////////////////////////////////////////////////
struct fast_alloc_pool_t {};
struct basic_alloc_pool_t {};

template <
	typename T,
	typename user_alloc_t = boost::default_user_allocator_new_delete,
	typename pool_alloc_t = fast_alloc_pool_t
	>
class allocator
{
public:
	typedef T value_type;
	typedef user_alloc_t user_allocator;
	typedef value_type* pointer;
	typedef value_type const* const_pointer;
	typedef value_type& reference;
	typedef value_type const& const_reference;
	typedef boost::pool<user_alloc_t> pool_t;
	typedef boost::shared_ptr<pool_t> pool_ptr_t;
	typedef typename pool_t::size_type size_type;
	typedef typename pool_t::difference_type difference_type;

	template <typename U>
	struct rebind
	{
		typedef allocator<U, user_alloc_t, pool_alloc_t> other;
	};

private:
	struct fast_pool_wrap
	{
		fast_pool_wrap(pool_t& pool)
			: pool_(pool)
		{
		}

		void* malloc(size_type const n) 
		{ 
			return (n == 1) ? pool_.malloc() : pool_.ordered_malloc(n); 
		}
		void free(void* chuck, size_type const n) 
		{ 
			(n == 1) ? pool_.free(chuck) : pool_.free(chuck, n); 
		}

		pool_t& pool_;
	};

	struct basic_pool_wrap
	{
		basic_pool_wrap(pool_t& pool)
			: pool_(pool)
		{
		}

		void* malloc(size_type const n) 
		{ 
			return pool_.ordered_malloc(n); 
		}
		void free(void* chuck, size_type const n) 
		{ 
			pool_.ordered_free(chuck, n); 
		}

		pool_t& pool_;
	};

	typedef typename boost::mpl::if_<boost::is_same<pool_alloc_t, fast_alloc_pool_t>, fast_pool_wrap, basic_pool_wrap>::type pool_alloc_wrap_t;

	struct null_deleter
	{
		void operator()(pool_t* /*pool*/)
		{
			// do nothing, leave it to user
		}
	};

public:
	explicit allocator(size_type n = 0, size_type next_size = 32)
		: pool_(new pool_t(sizeof(value_type), next_size))
		, pool_wrap_(*pool_)
	{
		// reserve
		if (n > 0)
		{
			std::vector<void*> tmp(n);
			BOOST_FOREACH(void*& v, tmp)
			{
				v = pool_->malloc();
			}
			BOOST_REVERSE_FOREACH(void* v, tmp)
			{
				pool_->free(v);
			}
		}
	}

	explicit allocator(pool_t& pool)
		: pool_(&pool, null_deleter())
		, pool_wrap_(*pool_)
	{
	}

	explicit allocator(pool_ptr_t pool)
		: pool_(pool)
		, pool_wrap_(*pool_)
	{
	}

	template <typename U>
	allocator(allocator<U, user_alloc_t, pool_alloc_t> const&)
		: pool_(new pool_t(sizeof(T)))
		, pool_wrap_(*pool_)
	{
	}

	template<typename U>
	allocator<T>& operator=(allocator<U> const&)
	{	
		// assign from a related allocator (do nothing)
		return (*this);
	}
	
	~allocator() 
	{
	}

	pointer address(reference r)
	{ 
		return &r; 
	}

	const_pointer address(const_reference s)
	{ 
		return &s; 
	}

	size_type max_size() const
	{ 
		return (std::numeric_limits<size_type>::max)(); 
	}

	void construct(pointer const ptr, value_type const& t)
	{ 
		new (ptr) T(t); 
	}

	void destroy(pointer const ptr)
	{
		ptr->~T();
		(void)ptr; // avoid unused variable warning
	}

	bool operator==(const allocator&) const
	{ 
		return true; 
	}

	bool operator!=(const allocator&) const
	{ 
		return false; 
	}

	pointer allocate(size_type const n)
	{
		pointer const ret = static_cast<pointer>(pool_wrap_.malloc(n));
		if (ret == 0)
		{
			boost::throw_exception(std::bad_alloc());
		}
		return ret;
	}

	pointer allocate(size_type const n, void const* const)
	{
		return allocate(n);
	}

	void deallocate(pointer const ptr, size_type const n)
	{
#ifdef BOOST_NO_PROPER_STL_DEALLOCATE
		if (ptr == 0 || n == 0)
		{
			return;
		}
#endif
		pool_wrap_.free(ptr, n);
	}

private:
	pool_ptr_t pool_;
	pool_alloc_wrap_t pool_wrap_;
};
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_SINGLETON_CONFIG_HPP



