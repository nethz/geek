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
/// @file       std_header.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////

#ifndef GEEK_STD_HEADERS_HPP
#define GEEK_STD_HEADERS_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <iostream>   
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <errno.h>

// STL algorithms& functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>



#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:4396)
#pragma warning(disable:4675)
#pragma warning(disable:4267)
#pragma warning(disable:4996)
#pragma warning(disable:4099)
#pragma warning(disable:4819)

// define pointer int type
#include <boost/integer.hpp>
#if BOOST_VERSION <= 104100
#	include <boost/cstdint.hpp>
#endif

#include <boost/bind.hpp>
// #include <boost/checked_delete.hpp>
// #include <boost/static_assert.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/foreach.hpp>
//#include <boost/enable_shared_from_this.hpp> 
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/utility.hpp>
#include <boost/functional.hpp>
#include <boost/function.hpp>
#include <boost/regex.hpp>

// 
// #include <boost/cast.hpp>
// #include <boost/tuple/tuple.hpp>
// #include <boost/pool/object_pool.hpp>
// #include <boost/variant.hpp>
// #include <boost/any.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>
// #include <boost/current_function.hpp>
// #include <boost/type_traits.hpp>
// #include <boost/timer.hpp>
// #include <boost/circular_buffer.hpp>
// #include <boost/type_traits.hpp>
// #include <boost/format.hpp>
// #include <boost/regex.hpp>
// 
// 
// #include <boost/algorithm/string/classification.hpp>
// #include <boost/algorithm/string/split.hpp>
// 
// #include <boost/mpl/empty.hpp>
// #include <boost/mpl/int.hpp>
// #include <boost/mpl/list.hpp>
// #include <boost/mpl/find.hpp>
// #include <boost/mpl/inherit_linearly.hpp>
// #include <boost/mpl/find.hpp>
// #include <boost/mpl/vector.hpp>
// #include <boost/mpl/distance.hpp>
// #include <boost/mpl/size.hpp>
// 
// #include <boost/lambda/lambda.hpp>
// #include <boost/lambda/bind.hpp>
// 
// #include <boost/date_time/posix_time/posix_time.hpp>
// 
// #include <boost/statechart/event.hpp>
// #include <boost/statechart/state_machine.hpp>
// #include <boost/statechart/state.hpp>
// #include <boost/statechart/simple_state.hpp>
// #include <boost/statechart/transition.hpp>
// #include <boost/statechart/custom_reaction.hpp>
// #include <boost/statechart/shallow_history.hpp>
// #include <boost/statechart/deferral.hpp>
// 
// #include <boost/exception/info.hpp>
// #include <boost/exception/all.hpp>
// #include <boost/exception/exception.hpp>

// 
// #include <boost/functional/factory.hpp>
// #include <boost/signals2/signal.hpp>
// #include <boost/thread/mutex.hpp>
// #include <boost/thread/thread.hpp>



// #include <rapidxml/rapidxml.hpp>
// #include <utf8/utf8.h>


//#include <boost/exception/info.hpp>

//#include <boost/exception/exception.hpp>




#pragma warning(pop)


// 
// 
// 
// 
// extern "C" {
// 
// #   include <sys/types.h>
// #   include <sys/stat.h>
// 
// }

#endif
