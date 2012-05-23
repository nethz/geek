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
/// @file       log_stream.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_TOOLKITS_LOG_STREAM_HPP
#define GEEK_TOOLKITS_LOG_STREAM_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <boost/iostreams/stream.hpp>
#include <geek/toolkits/log_system/config.hpp>
#include <boost/iostreams/stream_buffer.hpp>
namespace geek
{
class log_sink 
{
public:
	typedef std::string::value_type char_type;
	typedef boost::iostreams::sink_tag category;

	log_sink(std::string& container) : container_(container) {}

	std::streamsize write(const char_type* s, std::streamsize n)
	{
		container_.insert(container_.end(), s, s + n);
		return n;
	}

	std::string& container() { return container_; }

private:
	std::string& container_;
};

class log_stream
{
public:
	log_stream(void) 
		: result_()
		, buf_(result_)
		, out_(&buf_)
	{
	}

	std::string& result(void)
	{
		return result_;
	}
	std::ostream& stream(void)
	{
		return out_;
	}
private:

	std::string result_;
	boost::iostreams::stream_buffer<log_sink> buf_;//(result);
	std::ostream out_;//(&buf);
};
}

#endif 
