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
/// @file       asio_alloc_handler.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/7
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_ASIO_ALLOC_ALLOCATOR_HPP
#define GEEK_TOOLKITS_ASIO_ALLOC_ALLOCATOR_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <boost/aligned_storage.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      asio_alloc_handler ; asio handler 分配器
/// @note       come from boost.asio example: Allocation
/// @date       2011/10/7
///////////////////////////////////////////////////////////////////////
template <std::size_t storage_size>
class handler_allocator : private boost::noncopyable
{
public:
	handler_allocator()
		: in_use_(false)
	{
	}

	~handler_allocator()
	{
	}

	void* allocate(std::size_t size)
	{
		if (!in_use_ && size < storage_.size)
		{
			in_use_ = true;
			return storage_.address();
		}
		else
		{
			return ::operator new(size);
		}
	}

	void deallocate(void* pointer)
	{
		if (pointer == storage_.address())
		{
			in_use_ = false;
		}
		else
		{
			::operator delete(pointer);
		}
	}

private:
	// storage space used for handler-based custom memory allocation.
	boost::aligned_storage<storage_size> storage_;

	// whether the handler-based custom allocation storage has been used.
	bool in_use_;
};
///////////////////////////////////////////////////////////////////////
/// @brief      asio_alloc_handler ; asio handler 分配器
/// @date       2011/10/7
///////////////////////////////////////////////////////////////////////
template <typename handler_t, std::size_t storage_size = 1024>
class asio_alloc_handler
{
public:
	asio_alloc_handler(handler_allocator<storage_size>& alloc, handler_t const& handler)
		: allocator_(alloc)
		, handler_(handler)
	{
	}

	~asio_alloc_handler()
	{
	}

public:
	void operator()()
	{
		handler_();
	}

	void operator()() const
	{
		handler_();
	}

	template <typename A1>
	void operator()(A1 a1)
	{
		handler_(a1);
	}

	template <typename A1>
	void operator()(A1 a1) const
	{
		handler_(a1);
	}

	template <typename A1, typename A2>
	void operator()(A1 a1, A2 a2)
	{
		handler_(a1, a2);
	}

	template <typename A1, typename A2>
	void operator()(A1 a1, A2 a2) const
	{
		handler_(a1, a2);
	}

	template <typename A1, typename A2, typename A3>
	void operator()(A1 a1, A2 a2, A3 a3)
	{
		handler_(a1, a2, a3);
	}

	template <typename A1, typename A2, typename A3>
	void operator()(A1 a1, A2 a2, A3 a3) const
	{
		handler_(a1, a2, a3);
	}

	template <typename A1, typename A2, typename A3, typename A4>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4)
	{
		handler_(a1, a2, a3, a4);
	}

	template <typename A1, typename A2, typename A3, typename A4>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4) const
	{
		handler_(a1, a2, a3, a4);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
	{
		handler_(a1, a2, a3, a4, a5);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const
	{
		handler_(a1, a2, a3, a4, a5);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
	{
		handler_(a1, a2, a3, a4, a5, a6);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const
	{
		handler_(a1, a2, a3, a4, a5, a6);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
	{
		handler_(a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) const
	{
		handler_(a1, a2, a3, a4, a5, a6, a7);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7, typename A8>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
	{
		handler_(a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7, typename A8>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8) const
	{
		handler_(a1, a2, a3, a4, a5, a6, a7, a8);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7, typename A8, typename A9>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
	{
		handler_(a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7, typename A8, typename A9>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9) const
	{
		handler_(a1, a2, a3, a4, a5, a6, a7, a8, a9);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7, typename A8, typename A9, typename A10>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
	{
		handler_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, 
			  typename A7, typename A8, typename A9, typename A10>
	void operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10) const
	{
		handler_(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
	}

public:
	friend void* asio_handler_allocate(std::size_t size, asio_alloc_handler<handler_t, storage_size>* this_handler)
	{
		return this_handler->allocator_.allocate(size);
	}

	friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/, asio_alloc_handler<handler_t, storage_size>* this_handler)
	{
		this_handler->allocator_.deallocate(pointer);
	}

private:
	handler_allocator<storage_size>& allocator_;
	handler_t handler_;
};
///////////////////////////////////////////////////////////////////////
/// @brief      Helper function to wrap a handler object to add custom allocation.
/// @date       2011/10/7
///////////////////////////////////////////////////////////////////////
template <typename handler_t, std::size_t storage_size>
inline asio_alloc_handler<handler_t, storage_size> make_asio_alloc_handler(handler_allocator<storage_size>& a, handler_t const& h)
{
	return asio_alloc_handler<handler_t, storage_size>(a, h);
}
}
#endif // GEEK_TOOLKITS_ASIO_ALLOC_ALLOCATOR_HPP



