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
/// @file       concurrent.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/09/25
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_CONCURRENT_CONCURRENT_HPP
#define GEEK_TOOLKITS_CONCURRENT_CONCURRENT_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <geek/toolkits/concurrent/config.hpp>
#include <boost/asio/io_service.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>
#include <geek/toolkits/log_system.hpp>
#include <geek/toolkits/singleton.hpp>
#include <boost/thread/thread.hpp>

namespace geek 
{
///////////////////////////////////////////////////////////////////////
/// @brief      concurrent ; 并发系统
///
/// @details    thread-safe ; 线程安全
/// @date       2011/09/25
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_CONCURRENT_DECL concurrent : public singleton<concurrent>
{
public:
	/////////////////////////////////////////////////
	/// @brief     concurrent constructor1 ; 构造函数1 
	/// @param queue count ; 队列数量
	/// @param thread count default value is cpu*core num - 1; 线程数量，默认值为系统cpu*核数 - 1
	/// @param is_force_shutdown whether or not force shutdown ; 是否强制关闭
	/// @note      
	/////////////////////////////////////////////////
	explicit concurrent(
		std::size_t queue_count, 
		std::size_t thread_count = boost::thread::hardware_concurrency() - 1,
		bool is_force_shutdown = false
		);

	/////////////////////////////////////////////////
	/// @brief     concurrent constructor2 ; 构造函数2 
	/// @param io_service ; 
	/// @param queue count ; 队列数量
	/// @param thread count default value is cpu*core num - 1; 线程数量，默认值为系统cpu*核数 - 1
	/// @param is_force_shutdown whether or not force shutdown ; 是否强制关闭
	/// @note      
	/////////////////////////////////////////////////
	concurrent(
		boost::asio::io_service& io_service, 
		std::size_t queue_count, 
		std::size_t thread_count = boost::thread::hardware_concurrency() - 1,
		bool is_force_shutdown = false
		);

	~concurrent(void);

public:
	/////////////////////////////////////////////////
	/// @brief     get io_service ; 返回io_service 
	/// @return io_service ; 返回io_service
	/// @note      no throw
	/////////////////////////////////////////////////
	boost::asio::io_service& get_io_service();

	/////////////////////////////////////////////////
	/// @brief     post handler to thread pool to run; 将handler放入线程池执行
	/// @param the function will be run ; 将要执行的函数
	/// @note      thread-safe ; 线程安全
	/////////////////////////////////////////////////
	template <typename handler_t>
	void post(handler_t const& handler)
	{
		io_service_->post(handler);
	}

	/////////////////////////////////////////////////
	/// @brief     post handler to thread pool with given queue index; 将handler放入线程池中指定的队列执行
	/// @note      thread-safe, may throw std::out_of_range if ; 线程安全, 可能会抛出std::out_of_range异常
	/////////////////////////////////////////////////
	template <typename handler_t>
	void post(std::size_t queue_index, handler_t const& handler)
	{
		queue_list_[queue_index]->post(handler); // no need check queue_index we know std::vector will throw out of range exception if any
	}

	/////////////////////////////////////////////////
	/// @brief     using given strand to wrap given handler to post to thread pool to run;
	/// @param strand ; 
	/// @param the function will be run ;
	/// @note      thread-safe ; 
	/////////////////////////////////////////////////
	template <typename handler_t>
	void post(boost::asio::io_service::strand& strand, handler_t const& handler)
	{
		BOOST_ASSERT(&strand.get_io_service() == io_service_.get());

		strand.post(handler);
	}

private:
	typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
	typedef boost::scoped_ptr<boost::asio::io_service::work> work_ptr;
	typedef boost::shared_ptr<boost::asio::io_service::strand> queue_ptr;

private:
	void initialize();
	void run();

	class io_service_deleter
	{
		io_service_deleter();

	public:
		explicit io_service_deleter(bool is_delete);
		void operator()(boost::asio::io_service* io_serivce);

	private:
		bool is_delete_;
	};

	bool is_internal_;
	bool is_force_shutdown_;
	io_service_ptr io_service_;
	work_ptr work_;
	std::size_t const queue_count_;
	std::size_t const thread_count_;
	vector<queue_ptr>::type queue_list_;

	boost::thread_group thread_pool_;
};
}
#endif // GEEK_TOOLKITS_CONCURRENT_CONCURRENT_HPP



