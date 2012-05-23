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
/// @file       prerequisites.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/24
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_PREREQUISITES_HPP
#define GEEK_PREREQUISITES_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/chrono.hpp>


namespace geek
{
#if GEEK_ARCH_TYPE == GEEK_ARCHITECTURE_32
	typedef boost::uint32_t	 ptr_integer_t;
#elif GEEK_ARCH_TYPE == GEEK_ARCHITECTURE_64
	typedef boost::uint64_t	 ptr_integer_t;
#else
#	error "Unsupport arch for pointer int type"
#endif

#ifdef GEEK_PROCESSOR_COUNT
#	define GEEK_PROCESSOR_NUM GEEK_PROCESSOR_COUNT
#else
#	define GEEK_PROCESSOR_NUM 1
#endif

	typedef boost::chrono::system_clock geek_clock;
	typedef geek_clock::time_point time_point_type;
	typedef geek_clock::duration duration_type;
	inline float duration_to_float_seconds(duration_type const& d){return float(boost::chrono::duration_cast<boost::chrono::microseconds >(d).count())/ 1000000.f;}
	//inline duration_type float_seconds_to_duration(float seconds){}
}

#endif // GEEK_PREREQUISITES_HPP
