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
/// @file       stl_containter.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/09/30
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_STL_CONTAINTER_HPP
#define GEEK_TOOLKITS_STL_CONTAINTER_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <geek/toolkits/pool/allocator.hpp>

namespace geek
{
//-------------------------------------------------------------------------------------------------
template <typename T, typename A = allocator<T> >
struct vector 
{
	typedef typename std::vector<T, A> type;
};
//-------------------------------------------------------------------------------------------------
template <typename T, typename A = allocator<T> >
struct deque 
{
	typedef typename std::deque<T, A> type;
};
//-------------------------------------------------------------------------------------------------
template <typename T, typename A = allocator<T> >
struct list
{
	typedef typename std::list<T, A> type;
};
//-------------------------------------------------------------------------------------------------
template <typename T, typename P = std::less<T>, typename A = allocator<T> >
struct set
{
	typedef typename std::set<T, P, A> type;
};
//-------------------------------------------------------------------------------------------------
template <typename T, typename P = std::less<T>, typename A = allocator<T> >
struct multiset
{
	typedef typename std::multiset<T, P, A> type;
};
//-------------------------------------------------------------------------------------------------
template <typename K, typename V, typename P = std::less<K>, typename A = allocator<std::pair<K const, V> > >
struct map
{
	typedef typename std::map<K, V, P, A> type;
}; 
//-------------------------------------------------------------------------------------------------
template <typename K, typename V, typename P = std::less<K>, typename A = allocator<std::pair<K const, V> > >
struct multimap
{
	typedef typename std::multimap<K, V, P, A> type;
};
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_STL_CONTAINTER_HPP



