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
/// @file       coroutine.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/10/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_CONCURRENT_COROUTINE_HPP
#define GEEK_TOOLKITS_CONCURRENT_COROUTINE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/toolkits/concurrent/config.hpp>
#include <geek/toolkits/concurrent/concurrent.hpp>
#include <geek/toolkits/pool/asio_alloc_handler.hpp>
#include <geek/toolkits/pool/freelist_allocator.hpp>
#include <boost/context/all.hpp>
#include <boost/asio/strand.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>

#if defined(GEEK_MSVC)
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif

namespace geek 
{
///////////////////////////////////////////////////////////////////////
/// @brief      coroutine ; 协程
///
/// @date       2011/10/23
///////////////////////////////////////////////////////////////////////
class coroutine
	: public boost::enable_shared_from_this<coroutine>
{
public:
	typedef boost::shared_ptr<coroutine> ptr_t;
	typedef boost::function<void (ptr_t)> fn_t;

public:
	~coroutine() {}

	/////////////////////////////////////////////////
	/// @brief     resume to yield point ; 恢复到挂起点 
	/////////////////////////////////////////////////
	inline void call() 
	{ 
		if (!started_)
		{
			started_ = true;
			ctx_.start();
		}
		else 
		{
			ctx_.resume();
		}
	}

	/////////////////////////////////////////////////
	/// @brief     yield for other coroutines to run ; 挂起当前协程 
	/////////////////////////////////////////////////
	inline void yield() 
	{
		ctx_.suspend(); 
	}

	/////////////////////////////////////////////////
	/// @brief     async resume to yield point ; 异步恢复到挂起点 
	/////////////////////////////////////////////////
	inline void schedule()
	{
		strand_.post(make_asio_alloc_handler(handler_allocator_, boost::bind(&coroutine::call, shared_from_this())));
	}

	/////////////////////////////////////////////////
	/// @brief     get io_service::strand ; 返回io_service::strand 
	/// @return io_service::strand ; 返回io_service::strand
	/////////////////////////////////////////////////
	inline boost::asio::io_service::strand& get_strand() 
	{
		return strand_; 
	}

	/////////////////////////////////////////////////
	/// @brief     get io_service ; 返回io_service 
	/// @return io_service ; 返回io_service
	/////////////////////////////////////////////////
	inline boost::asio::io_service& get_io_service()
	{
		return io_service_;
	}

private:
	friend void go(fn_t const&);
	friend void go(boost::asio::io_service&, fn_t const&);

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	friend void go(fn_t const&);

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	friend void go(boost::asio::io_service&, fn_t const&);

	template <typename tag>
	friend void go(fn_t const&);

	template <typename tag>
	friend void go(boost::asio::io_service&, fn_t const&);

	template <
		typename tag, 
		std::size_t n, 
		std::size_t grow_size
		>
	friend void go(fn_t const&);

	template <
		typename tag, 
		std::size_t n, 
		std::size_t grow_size
		>
	friend void go(boost::asio::io_service&, fn_t const&);

	template <class T, class A, class A1, class A2>
	friend  boost::shared_ptr<T> boost::allocate_shared(A const&, A1 const&, A2 const&);

	inline static ptr_t create(boost::asio::io_service& io_service, fn_t const& handler)
	{
		return ptr_t(new coroutine(io_service, handler));
	}

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	inline static ptr_t create(boost::asio::io_service& io_service, fn_t const& handler)
	{
		typedef freelist_allocator_mt<coroutine, thread_num, cache_num, page_size, page_grow_size, tag> coro_alloc_t;
		return boost::allocate_shared<coroutine, coro_alloc_t>(coro_alloc_t(), boost::ref(io_service), handler);
	}

	template <
		typename tag, 
		std::size_t n, 
		std::size_t grow_size
		>
	inline static ptr_t create(boost::asio::io_service& io_service, fn_t const& handler)
	{
		typedef freelist_allocator_st<coroutine, n, grow_size, tag> coro_alloc_t;
		return boost::allocate_shared<coroutine, coro_alloc_t>(coro_alloc_t(), boost::ref(io_service), handler);
	}

	inline coroutine(boost::asio::io_service& io_service, fn_t const& fn)
		: io_service_(io_service)
		, strand_(io_service_)
		, ctx_(
			&coroutine::trampoline, 
			this,
			boost::contexts::default_stacksize(),
			boost::contexts::no_stack_unwind, 
			boost::contexts::return_to_caller
			)
		, fn_(fn)
		, started_(false)
	{
	}

	inline void trampoline() 
	{
		fn_(shared_from_this());
	}

private:
	handler_allocator<1024> handler_allocator_;
	boost::asio::io_service& io_service_;
	boost::asio::io_service::strand strand_;
	boost::contexts::context ctx_;
	fn_t fn_;
	bool started_;
};

/// create a coroutine and schedule given function using geek concurrent ; 创建一个新的协程，并且使用geek的并发系统来调度指定的函数
inline void go(coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create(geek::concurrent::get_singleton().get_io_service(), fn));
	coro->schedule();
}

/// create a coroutine and schedule given function using given io_service; 创建一个新的协程，并且使用指定的io_service调度给定的函数
inline void go(boost::asio::io_service& io_service, coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create(io_service, fn));
	coro->schedule();
}

/// create a coroutine using given freelist and schedule given function using given io_service; 使用freelist创建一个新的协程，并且使用geek的并发系统来调度指定的函数
template <
	typename tag, 
	std::size_t thread_num, 
	std::size_t cache_num, 
	std::size_t page_size, 
	std::size_t page_grow_size
	>
inline void go(coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create<tag, thread_num, cache_num, page_size, page_grow_size>(geek::concurrent::get_singleton().get_io_service(), fn));
	coro->schedule();
}

/// create a coroutine using given freelist and schedule given function using given io_service; 使用freelist创建一个新的协程，并且使用指定的io_service调度给定的函数
template <
	typename tag, 
	std::size_t thread_num, 
	std::size_t cache_num, 
	std::size_t page_size, 
	std::size_t page_grow_size
	>
inline void go(boost::asio::io_service& io_service, coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create<tag, thread_num, cache_num, page_size, page_grow_size>(io_service, fn));
	coroutine->schedule();
}

/// create a coroutine using given freelist and schedule given function using given io_service; 使用freelist创建一个新的协程，并且使用geek的并发系统来调度指定的函数
template <typename tag>
inline void go(coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create<tag, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32>(geek::concurrent::get_singleton().get_io_service(), fn));
	coro->schedule();
}

/// create a coroutine using given freelist and schedule given function using given io_service; 使用freelist创建一个新的协程，并且使用指定的io_service调度给定的函数
template <typename tag>
inline void go(boost::asio::io_service& io_service, coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create<tag, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32>(io_service, fn));
	coroutine->schedule();
}

/// create a coroutine using given freelist and schedule given function using given io_service; 使用freelist创建一个新的协程，并且使用geek的并发系统来调度指定的函数
template <typename tag, std::size_t n, std::size_t grow_size>
inline void go(coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create<tag, n, grow_size>(geek::concurrent::get_singleton().get_io_service(), fn));
	coro->schedule();
}

/// create a coroutine using given freelist and schedule given function using given io_service; 使用freelist创建一个新的协程，并且使用指定的io_service调度给定的函数
template <typename tag, std::size_t n, std::size_t grow_size>
inline void go(boost::asio::io_service& io_service, coroutine::fn_t const& fn)
{
	BOOST_ASSERT(fn);

	coroutine::ptr_t coro(coroutine::create<tag, n, grow_size>(io_service, fn));
	coroutine->schedule();
}
}

#if defined(GEEK_MSVC)
#	pragma warning(pop)
#endif

#endif // GEEK_TOOLKITS_CONCURRENT_COROUTINE_HPP
