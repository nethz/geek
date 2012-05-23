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
/// @file       freelist_policy_mt.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/4
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_FREELIST_POLICY_MT_HPP
#define GEEK_TOOLKITS_FREELIST_POLICY_MT_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <geek/toolkits/pool/freelist_policy_st.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>
#include <boost/aligned_storage.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      freelist_allocator_mt ; 自由列表多线程分配器
/// @date       2011/10/4
///////////////////////////////////////////////////////////////////////
template <
	typename obj_t, 
	typename user_alloc_t = boost::default_user_allocator_new_delete 
	>
class freelist_policy_mt : private boost::noncopyable
{
public:
	explicit freelist_policy_mt(
		std::size_t const thread_num = 1,
		std::size_t const cache_page_num = 1, 
		std::size_t const page_size = 0, 
		std::size_t const page_grow_size = 32
		);

	~freelist_policy_mt();

public:
	// may throw std::bad_alloc; mostly O(1)
	obj_t* allocate(void);
	obj_t* allocate(std::size_t const n);

	// no throw; O(1)
	void deallocate(obj_t* o);
	void deallocate(obj_t* o, std::size_t const n);
	
	// no throw
	bool is_from(obj_t* o);
	bool is_from(obj_t* o, std::size_t const n);

private:
	typedef boost::uint8_t byte_t;
	typedef boost::uint16_t index_size_t;

	struct cache
	{
		struct node
		{
			byte_t storage[sizeof(index_size_t)*2 + sizeof(obj_t)];
		};

		struct array_node
		{
			byte_t storage[sizeof(index_size_t)*2];
		};

		typedef freelist_policy_st<obj_t, user_alloc_t> obj_alloc_st_t;
		typedef freelist_policy_st<node, user_alloc_t> node_alloc_st_t;

		struct page
		{
			inline page(std::size_t n, std::size_t grow_size, std::size_t i)
				: index(i)
				, obj_alloc_st(n, grow_size)
				, node_alloc_st(n, grow_size)
			{
			}

			boost::mutex mtx;
			std::size_t index;
			obj_alloc_st_t obj_alloc_st;
			node_alloc_st_t node_alloc_st;
		};

		inline page* select_page()
		{
			// may in multi-threads, so we using copy on write
			std::size_t index = 0;
			index = page_index;
			++index;
			if (index >= page_num)
			{
				index = 0;
			}
			page_index = index;
			return page_list[index].get();
		}

		typedef boost::shared_ptr<page> page_ptr_t;

		std::vector<page_ptr_t> page_list;
		std::size_t page_index;
		std::size_t page_num;
		std::size_t index;
	};

	struct thread_cache
	{
		cache* c;
	};

private:
	inline cache* get_cache()
	{
		thread_cache* tc = thread_cache_tss_.get();
		if (tc != NULL)
		{
			return tc->c;
		}

		std::size_t cache_index = 0;
		{
			boost::mutex::scoped_lock lock(cache_index_mtx_);
			++current_cache_index_;
			if (current_cache_index_ >= thread_num_)
			{
				current_cache_index_ = 0;
			}
			cache_index = current_cache_index_;
		}
		thread_cache_tss_.reset(new thread_cache());
		thread_cache_tss_->c = &cache_list_[cache_index];

		return thread_cache_tss_->c;
	}

	inline typename cache::page* get_page(obj_t* o)
	{
		byte_t* raw_ptr = (byte_t*)o - index_size_t_offsetX2_;
		index_size_t* cache_index = (index_size_t*)raw_ptr;
		index_size_t* page_index = (index_size_t*)(raw_ptr + index_size_t_offset_);

		return cache_list_[*cache_index].page_list[*page_index].get();
	}

	inline typename cache::page* get_page(obj_t* o, std::size_t const /*n*/)
	{
		byte_t* node = (byte_t*)(o - 1);
		index_size_t* cache_index = (index_size_t*)node;
		index_size_t* page_index = (index_size_t*)(node + index_size_t_offset_);

		return cache_list_[*cache_index].page_list[*page_index].get();
	}

private:
	std::size_t const thread_num_;
	std::size_t const cache_page_num_;
	std::size_t const page_size_;
	std::size_t const page_grow_size_;
	std::size_t const index_size_t_offset_;
	std::size_t const index_size_t_offsetX2_;

	boost::mutex cache_index_mtx_;
	std::size_t current_cache_index_;

	std::vector<cache> cache_list_;

	typedef boost::thread_specific_ptr<thread_cache> thread_cache_tss_t;
	thread_cache_tss_t thread_cache_tss_;

	BOOST_STATIC_ASSERT_MSG(sizeof(obj_t) >= sizeof(index_size_t)*2, "Requires sizeof(obj_t) >= 4");
};
//-------------------------------------------------------------------------------------------------
// freelist_policy_mt impl
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline freelist_policy_mt<obj_t, user_alloc_t>::freelist_policy_mt(
	std::size_t const thread_num,
	std::size_t const cache_page_num, 
	std::size_t const page_size, 
	std::size_t const page_grow_size
	)
	: thread_num_(thread_num)
	, cache_page_num_(cache_page_num)
	, page_size_(page_size)
	, page_grow_size_(page_grow_size)
	, index_size_t_offset_(sizeof(boost::uint16_t))
	, index_size_t_offsetX2_(sizeof(boost::uint16_t)*2)
	, current_cache_index_(0)
	, cache_list_(thread_num_)
{
	for (std::size_t i=0; i<thread_num_; ++i)
	{
		cache& c= cache_list_[i];
		c.page_list.resize(cache_page_num_);
		c.page_index = 0;
		c.page_num = cache_page_num_;
		c.index = i;

		for (std::size_t j=0; j<cache_page_num_; ++j)
		{
			c.page_list[j].reset(new typename cache::page(page_size_, page_grow_size_, j));
		}
	}
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline freelist_policy_mt<obj_t, user_alloc_t>::~freelist_policy_mt()
{
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline obj_t* freelist_policy_mt<obj_t, user_alloc_t>::allocate(void)
{
	cache* c = get_cache();
	typename cache::page* p = c->select_page();

	typename cache::node* node = NULL;
	{
		boost::mutex::scoped_lock lock(p->mtx);
		node = p->node_alloc_st.allocate();
	}

	byte_t* storage_address = (byte_t*)node;
	index_size_t* cache_index = (index_size_t*)storage_address;
	index_size_t* page_index = (index_size_t*)(storage_address + index_size_t_offset_);

	*cache_index = c->index;
	*page_index = p->index;

	return (obj_t*)(storage_address + index_size_t_offsetX2_);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline obj_t* freelist_policy_mt<obj_t, user_alloc_t>::allocate(std::size_t const n)
{
	if (n == 0)
	{
		return NULL;
	}

	if (n == 1)
	{
		return allocate();
	}

	cache* c = get_cache();
	typename cache::page* p = c->select_page();

	obj_t* o = NULL;
	{
		boost::mutex::scoped_lock lock(p->mtx);
		o = p->obj_alloc_st.allocate(n + 1);
	}

	byte_t* storage_address = (byte_t*)o;
	index_size_t* cache_index = (index_size_t*)storage_address;
	index_size_t* page_index = (index_size_t*)(storage_address + index_size_t_offset_);

	*cache_index = c->index;
	*page_index = p->index;

	return o + 1;
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline void freelist_policy_mt<obj_t, user_alloc_t>::deallocate(obj_t* o)
{
	if (o == NULL)
	{
		return;
	}

	typename cache::page* p = get_page(o);

	boost::mutex::scoped_lock lock(p->mtx);
	p->node_alloc_st.deallocate((typename cache::node*)((byte_t*)o - index_size_t_offsetX2_));
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline void freelist_policy_mt<obj_t, user_alloc_t>::deallocate(obj_t* o, std::size_t const n)
{
	if (n == 0)
	{
		return;
	}

	if (o == NULL)
	{
		return;
	}

	if (n == 1)
	{
		deallocate(o);
		return;
	}

	typename cache::page* p = get_page(o, n);

	boost::mutex::scoped_lock lock(p->mtx);
	p->obj_alloc_st.deallocate(o - 1, n + 1);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline bool freelist_policy_mt<obj_t, user_alloc_t>::is_from(obj_t* o)
{
	if (o == NULL)
	{
		return false;
	}

	typename cache::page* p = get_page(o);
	void* node = (byte_t*)o - index_size_t_offsetX2_;
	return p->node_alloc_st.is_from((typename cache::node*)node);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename user_alloc_t>
inline bool freelist_policy_mt<obj_t, user_alloc_t>::is_from(obj_t* o, std::size_t const n)
{
	if (n == 0)
	{
		return false;
	}

	if (o == NULL)
	{
		return false;
	}

	if (n == 1)
	{
		return is_from(o);
	}

	typename cache::page* p = get_page(o, n);
	return p->obj_alloc_st.is_from(o - 1, n + 1);
}
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_FREELIST_POLICY_MT_HPP



