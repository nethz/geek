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
/// @file       channel.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/09/26
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_CONCURRENT_CHANNEL_HPP
#define GEEK_TOOLKITS_CONCURRENT_CHANNEL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/toolkits/concurrent/config.hpp>
#include <geek/toolkits/concurrent/coroutine.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>

#if defined(GEEK_MSVC)
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif

namespace geek 
{
///////////////////////////////////////////////////////////////////////
/// @brief      channel ; 管道
///
/// @date       2011/09/26
///////////////////////////////////////////////////////////////////////
template <typename T>
class channel
{
public:
	typedef boost::shared_ptr<channel<T> > ptr_t;

	static ptr_t create();
	static ptr_t create(boost::asio::io_service& io_service);

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	static ptr_t create();

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	static ptr_t create(boost::asio::io_service& io_service);

	template <typename tag>
	static ptr_t create();

	template <typename tag>
	static ptr_t create(boost::asio::io_service& io_service);

	template <
		typename tag,
		std::size_t n,
		std::size_t grow_size
		>
	static ptr_t create();

	template <
		typename tag,
		std::size_t n,
		std::size_t grow_size
		>
	static ptr_t create(boost::asio::io_service& io_service);

	~channel();

public:
	/////////////////////////////////////////////////
	/// @brief     recv data ; 接受数据
	/// @param coroutine current coroutine ; 当前协程
	/// @param t the data will recv ; 要接受的数据
	/// @note      block current coroutine until recv success or error; 阻塞当前协程，直到数据接收成功
	/////////////////////////////////////////////////
	void recv(coroutine::ptr_t coroutine, T& t);

	/////////////////////////////////////////////////
	/// @brief     send data ; 发送数据
	/// @param coroutine current coroutine ; 当前协程
	/// @param t the data will be sent ; 要发送的数据
	/// @note      block current coroutine until send success or error; 阻塞当前协程，直到数据发送成功
	/////////////////////////////////////////////////
	void send(coroutine::ptr_t coroutine, T const& t);

private:
	channel();
	explicit channel(boost::asio::io_service& io_service);

	template <class T, class A>
	friend  boost::shared_ptr<T> boost::allocate_shared(A const&);

	template <class T, class A, class A1>
	friend  boost::shared_ptr<T> boost::allocate_shared(A const&, A1 const&);

private:
	void handle_recv_request(boost::system::error_code& ec, T& t, coroutine::ptr_t coroutine)
	{
		if (send_queue_.empty())
		{
			recv_node n(ec, t, coroutine);
			recv_queue_.push(n);
		}
		else
		{
			send_node n = send_queue_.front();
			send_queue_.pop();
			ec = n.ec;
			t = n.t;
			coroutine->schedule();
			n.coroutine->schedule();
		}
	}

	void handle_send_request(boost::system::error_code& ec, T const& t, coroutine::ptr_t coroutine)
	{
		if (recv_queue_.empty())
		{
			send_node n(ec, t, coroutine);
			send_queue_.push(n);
		}
		else
		{
			recv_node n = recv_queue_.front();
			recv_queue_.pop();
			n.ec = ec;
			n.t = t;
			coroutine->schedule();
			n.coroutine->schedule();
		}
	}

	template <typename freelist_t>
	struct deleter
	{
		deleter(freelist_t& freelist)
			: freelist(freelist)
		{
		}

		void operator()(channel<T>* chan)
		{
			freelist.destruct(chan);
		}

		freelist_t& freelist;
	};

private:
	struct recv_node
	{
		recv_node(boost::system::error_code& ec, T& t, coroutine::ptr_t c)
			: ec(ec)
			, t(t)
			, coroutine(c)
		{
		}

		boost::system::error_code& ec;
		T& t;
		coroutine::ptr_t coroutine;
	};

	struct send_node
	{
		send_node(boost::system::error_code& ec, T const& t, coroutine::ptr_t c)
			: ec(ec)
			, t(t)
			, coroutine(c)
		{
		}

		boost::system::error_code& ec;
		T const& t;
		coroutine::ptr_t coroutine;
	};

private:
	handler_allocator<1024> handler_allocator_;
	boost::asio::io_service& io_service_;
	boost::asio::io_service::strand strand_;
	std::queue<recv_node, typename deque<recv_node>::type> recv_queue_;
	std::queue<send_node, typename deque<send_node>::type> send_queue_;
};
///////////////////////////////////////////////////////////////////////
/// @brief      channel ; 管道
/// @note void specialization ; void 特化
/// @date       2011/09/26
///////////////////////////////////////////////////////////////////////
template <>
class channel<void>
{
public:
	typedef boost::shared_ptr<channel<void> > ptr_t;

	static ptr_t create();
	static ptr_t create(boost::asio::io_service& io_service);

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	static ptr_t create();

	template <
		typename tag, 
		std::size_t thread_num, 
		std::size_t cache_num, 
		std::size_t page_size, 
		std::size_t page_grow_size
		>
	static ptr_t create(boost::asio::io_service& io_service);

	template <typename tag>
	static ptr_t create();

	template <typename tag>
	static ptr_t create(boost::asio::io_service& io_service);

	template <
		typename tag,
		std::size_t n,
		std::size_t grow_size
		>
	static ptr_t create();

	template <
		typename tag,
		std::size_t n,
		std::size_t grow_size
		>
	static ptr_t create(boost::asio::io_service& io_service);

	~channel();

public:
	/////////////////////////////////////////////////
	/// @brief     recv data ; 接受数据
	/// @param coroutine current coroutine ; 当前协程
	/// @note      block current coroutine until other coroutine call send; 阻塞当前协程，直到其它协程调用send
	/////////////////////////////////////////////////
	void recv(coroutine::ptr_t coroutine);

	/////////////////////////////////////////////////
	/// @brief     send data ; 发送数据
	/// @param coroutine current coroutine ; 当前协程
	/// @note      block current coroutine until other coroutine call recv; 阻塞当前协程，直到其它协程调用recv
	/////////////////////////////////////////////////
	void send(coroutine::ptr_t coroutine);

private:
	channel();
	explicit channel(boost::asio::io_service& io_service);

	template <class T, class A>
	friend  boost::shared_ptr<T> boost::allocate_shared(A const&);

	template <class T, class A, class A1>
	friend  boost::shared_ptr<T> boost::allocate_shared(A const&, A1 const&);

private:
	void handle_recv_request(boost::system::error_code& ec, coroutine::ptr_t coroutine)
	{
		if (send_queue_.empty())
		{
			node n(ec, coroutine);
			recv_queue_.push(n);
		}
		else
		{
			node n = send_queue_.front();
			send_queue_.pop();
			ec = n.ec;
			coroutine->schedule();
			n.coroutine->schedule();
		}
	}

	void handle_send_request(boost::system::error_code& ec, coroutine::ptr_t coroutine)
	{
		if (recv_queue_.empty())
		{
			node n(ec, coroutine);
			send_queue_.push(n);
		}
		else
		{
			node n = recv_queue_.front();
			recv_queue_.pop();
			n.ec = ec;
			coroutine->schedule();
			n.coroutine->schedule();
		}
	}

private:
	struct node
	{
		node(boost::system::error_code& ec, coroutine::ptr_t c)
			: ec(ec)
			, coroutine(c)
		{
		}

		boost::system::error_code& ec;
		coroutine::ptr_t coroutine;
	};

	template <typename freelist_t>
	struct deleter
	{
		deleter(freelist_t& freelist)
			: freelist(freelist)
		{
		}

		void operator()(channel<void>* chan)
		{
			freelist.destruct(chan);
		}

		freelist_t& freelist;
	};

private:
	handler_allocator<1024> handler_allocator_;
	boost::asio::io_service& io_service_;
	boost::asio::io_service::strand strand_;
	std::queue<node, typename deque<node>::type> recv_queue_;
	std::queue<node, typename deque<node>::type> send_queue_;
};
//-------------------------------------------------------------------------------------------------
// channel impl
//-------------------------------------------------------------------------------------------------
template <typename T>
channel<T>::channel()
	: io_service_(geek::concurrent::get_singleton().get_io_service())
	, strand_(io_service_)
{
}
//-------------------------------------------------------------------------------------------------
template <typename T>
channel<T>::channel(boost::asio::io_service& io_service)
	: io_service_(io_service)
	, strand_(io_service_)
{
}
//-------------------------------------------------------------------------------------------------
template <typename T>
typename channel<T>::ptr_t channel<T>::create()
{
	return channel<T>::ptr_t(new channel<T>());
}
//-------------------------------------------------------------------------------------------------
template <typename T>
typename channel<T>::ptr_t channel<T>::create(boost::asio::io_service& io_service)
{
	return channel<T>::ptr_t(new channel<T>(io_service));
}
//-------------------------------------------------------------------------------------------------
template <typename T>
template <
	typename tag, 
	std::size_t thread_num, 
	std::size_t cache_num, 
	std::size_t page_size, 
	std::size_t page_grow_size
	>
typename channel<T>::ptr_t channel<T>::create()
{
	typedef freelist_allocator_mt<channel<T>, thread_num, cache_num, page_size, page_grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<T>, chan_alloc_t>(chan_alloc_t());
}
//-------------------------------------------------------------------------------------------------
template <typename T>
template <
	typename tag, 
	std::size_t thread_num, 
	std::size_t cache_num, 
	std::size_t page_size, 
	std::size_t page_grow_size
	>
typename channel<T>::ptr_t channel<T>::create(boost::asio::io_service& io_service)
{
	typedef freelist_allocator_mt<channel<T>, thread_num, cache_num, page_size, page_grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<T>, chan_alloc_t>(chan_alloc_t(), boost::ref(io_service));
}
//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename tag>
typename channel<T>::ptr_t channel<T>::create()
{
	typedef freelist_allocator_mt<channel<T>, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32, tag> chan_alloc_t;
	return boost::allocate_shared<channel<T>, chan_alloc_t>(chan_alloc_t());
}
//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename tag>
typename channel<T>::ptr_t channel<T>::create(boost::asio::io_service& io_service)
{
	typedef freelist_allocator_mt<channel<T>, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32, tag> chan_alloc_t;
	return boost::allocate_shared<channel<T>, chan_alloc_t>(chan_alloc_t(), boost::ref(io_service));
}
//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename tag, std::size_t n, std::size_t grow_size>
typename channel<T>::ptr_t channel<T>::create()
{
	typedef freelist_allocator_st<channel<T>, n, grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<T>, chan_alloc_t>(chan_alloc_t());
}
//-------------------------------------------------------------------------------------------------
template <typename T>
template <typename tag, std::size_t n, std::size_t grow_size>
typename channel<T>::ptr_t channel<T>::create(boost::asio::io_service& io_service)
{
	typedef freelist_allocator_st<channel<T>, n, grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<T>, chan_alloc_t>(chan_alloc_t(), boost::ref(io_service));
}
//-------------------------------------------------------------------------------------------------
template <typename T>
channel<T>::~channel()
{
}
//-------------------------------------------------------------------------------------------------
template <typename T>
void channel<T>::recv(coroutine::ptr_t coroutine, T& t)
{
	boost::system::error_code ec;
	strand_.post(
		make_asio_alloc_handler(
			handler_allocator_,
			boost::bind(
				&channel<T>::handle_recv_request, 
				this,
				boost::ref(ec),
				boost::ref(t), 
				coroutine
				)
			)
		);

	coroutine->yield();

	if (ec)
	{
		boost::system::system_error e(ec);
		boost::throw_exception(e);
	}
}
//-------------------------------------------------------------------------------------------------
template <typename T>
void channel<T>::send(coroutine::ptr_t coroutine, T const& t)
{
	boost::system::error_code ec;
	strand_.post(
		make_asio_alloc_handler(
			handler_allocator_,
			boost::bind(
				&channel<T>::handle_send_request, 
				this,
				boost::ref(ec),
				boost::cref(t), 
				coroutine
				)
			)
		);

	coroutine->yield();

	if (ec)
	{
		boost::system::system_error e(ec);
		boost::throw_exception(e);
	}
}
//-------------------------------------------------------------------------------------------------
// channel (void specialization) impl
//-------------------------------------------------------------------------------------------------
channel<void>::channel()
	: io_service_(geek::concurrent::get_singleton().get_io_service())
	, strand_(io_service_)
{
}
//-------------------------------------------------------------------------------------------------
channel<void>::channel(boost::asio::io_service& io_service)
	: io_service_(io_service)
	, strand_(io_service_)
{
}
//-------------------------------------------------------------------------------------------------
channel<void>::ptr_t channel<void>::create()
{
	return channel<void>::ptr_t(new channel<void>());
}
//-------------------------------------------------------------------------------------------------
channel<void>::ptr_t channel<void>::create(boost::asio::io_service& io_service)
{
	return channel<void>::ptr_t(new channel<void>(io_service));
}
//-------------------------------------------------------------------------------------------------
template <
	typename tag, 
	std::size_t thread_num, 
	std::size_t cache_num, 
	std::size_t page_size, 
	std::size_t page_grow_size
	>
typename channel<void>::ptr_t channel<void>::create()
{
	typedef freelist_allocator_mt<channel<void>, thread_num, cache_num, page_size, page_grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<void>, chan_alloc_t>(chan_alloc_t());
}
//-------------------------------------------------------------------------------------------------
template <
	typename tag, 
	std::size_t thread_num, 
	std::size_t cache_num, 
	std::size_t page_size, 
	std::size_t page_grow_size
	>
typename channel<void>::ptr_t channel<void>::create(boost::asio::io_service& io_service)
{
	typedef freelist_allocator_mt<channel<void>, thread_num, cache_num, page_size, page_grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<void>, chan_alloc_t>(chan_alloc_t(), boost::ref(io_service));
}
//-------------------------------------------------------------------------------------------------
template <typename tag>
typename channel<void>::ptr_t channel<void>::create()
{
	typedef freelist_allocator_mt<channel<void>, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32, tag> chan_alloc_t;
	return boost::allocate_shared<channel<void>, chan_alloc_t>(chan_alloc_t());
}
//-------------------------------------------------------------------------------------------------
template <typename tag>
typename channel<void>::ptr_t channel<void>::create(boost::asio::io_service& io_service)
{
	typedef freelist_allocator_mt<channel<void>, GEEK_PROCESSOR_NUM, GEEK_PROCESSOR_NUM, 32, 32, tag> chan_alloc_t;
	return boost::allocate_shared<channel<void>, chan_alloc_t>(chan_alloc_t(), boost::ref(io_service));
}
//-------------------------------------------------------------------------------------------------
template <typename tag, std::size_t n, std::size_t grow_size>
typename channel<void>::ptr_t channel<void>::create()
{
	typedef freelist_allocator_st<channel<void>, n, grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<void>, chan_alloc_t>(chan_alloc_t());
}
//-------------------------------------------------------------------------------------------------
template <typename tag, std::size_t n, std::size_t grow_size>
typename channel<void>::ptr_t channel<void>::create(boost::asio::io_service& io_service)
{
	typedef freelist_allocator_st<channel<void>, n, grow_size, tag> chan_alloc_t;
	return boost::allocate_shared<channel<void>, chan_alloc_t>(chan_alloc_t(), boost::ref(io_service));
}
//-------------------------------------------------------------------------------------------------
channel<void>::~channel()
{
}
//-------------------------------------------------------------------------------------------------
void channel<void>::recv(coroutine::ptr_t coroutine)
{
	boost::system::error_code ec;
	strand_.post(
		make_asio_alloc_handler(
			handler_allocator_,
			boost::bind(
				&channel<void>::handle_recv_request, 
				this,
				boost::ref(ec),
				coroutine
				)
			)
		);

	coroutine->yield();

	if (ec)
	{
		boost::system::system_error e(ec);
		boost::throw_exception(e);
	}
}
//-------------------------------------------------------------------------------------------------
void channel<void>::send(coroutine::ptr_t coroutine)
{
	boost::system::error_code ec;
	strand_.post(
		make_asio_alloc_handler(
			handler_allocator_,
			boost::bind(
				&channel<void>::handle_send_request, 
				this,
				boost::ref(ec),
				coroutine
				)
			)
		);

	coroutine->yield();

	if (ec)
	{
		boost::system::system_error e(ec);
		boost::throw_exception(e);
	}
}
//-------------------------------------------------------------------------------------------------
}

#endif
