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
/// @file       freelist.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     NX
/// @date       2011/09/30
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_FREELIST_HPP
#define GEEK_TOOLKITS_FREELIST_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <geek/toolkits/pool/config.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/call_traits.hpp>
#include <boost/static_assert.hpp>

namespace geek
{
///////////////////////////////////////////////////////////////////////
/// @brief      freelist ; 自由列表
/// @date       2011/09/30
///////////////////////////////////////////////////////////////////////
template <typename obj_t, typename policy_t>
class freelist
{
public:
	explicit freelist(policy_t& policy);
	~freelist();

public:
	obj_t* construct();
	obj_t* construct(std::size_t const n);

	template <typename arg_t>
	obj_t* construct(typename boost::call_traits<arg_t>::param_type arg);

	template <typename arg_t>
	obj_t* construct(typename boost::call_traits<arg_t>::param_type arg, std::size_t const n);

	void destruct(obj_t* o);
	void destruct(obj_t* o, std::size_t const n);

	obj_t* allocate();
	obj_t* allocate(std::size_t const n);

	void deallocate(obj_t* o);
	void deallocate(obj_t* o, std::size_t const n);

	bool is_from(obj_t* o);
	bool is_from(obj_t* o, std::size_t const n);

private:
	policy_t* policy_;
};
//-------------------------------------------------------------------------------------------------
// freelist impl
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline freelist<obj_t, policy_t>::freelist(policy_t& policy)
	: policy_(&policy)
{
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline freelist<obj_t, policy_t>::~freelist()
{
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline obj_t* freelist<obj_t, policy_t>::construct(void)
{
	obj_t* o = policy_->allocate();
	new (o) obj_t();
	return o;
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline obj_t* freelist<obj_t, policy_t>::construct(std::size_t const n)
{
	if (n == 1)
	{
		return construct();
	}

	obj_t* o = policy_->allocate(n);
	for (std::size_t i=0; i<n; ++i)
	{
		new ((void*)(o + i)) obj_t();
	}
	return o;
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
template <typename arg_t>
inline obj_t* freelist<obj_t, policy_t>::construct(typename boost::call_traits<arg_t>::param_type arg)
{
	obj_t* o = policy_->allocate();
	new (o) obj_t(arg);
	return o;
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
template <typename arg_t>
inline obj_t* freelist<obj_t, policy_t>::construct(typename boost::call_traits<arg_t>::param_type arg, std::size_t const n)
{
	if (n == 1)
	{
		return construct(arg);
	}

	obj_t* o = policy_->allocate(n);
	for (std::size_t i=0; i<n; ++i)
	{
		new ((void*)(o + i)) obj_t(arg);
	}
	return o;
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline void freelist<obj_t, policy_t>::destruct(obj_t* o)
{
	o->~obj_t();
	policy_->deallocate(o);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline void freelist<obj_t, policy_t>::destruct(obj_t* o, std::size_t const n)
{
	if (n == 1)
	{
		destruct(o);
		return;
	}

	for (std::size_t i=0; i<n; ++i) 
	{
		(o)[i].~obj_t();
	}

	policy_->deallocate(o, n);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline obj_t* freelist<obj_t, policy_t>::allocate()
{
	return policy_->allocate();
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline obj_t* freelist<obj_t, policy_t>::allocate(std::size_t const n)
{
	return policy_->allocate(n);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline void freelist<obj_t, policy_t>::deallocate(obj_t* o)
{
	policy_->deallocate(o);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline void freelist<obj_t, policy_t>::deallocate(obj_t* o, std::size_t const n)
{
	policy_->deallocate(o, n);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline bool freelist<obj_t, policy_t>::is_from(obj_t* o)
{
	return policy_->is_from(o);
}
//-------------------------------------------------------------------------------------------------
template <typename obj_t, typename policy_t>
inline bool freelist<obj_t, policy_t>::is_from(obj_t* o, std::size_t const n)
{
	return policy_->is_from(o, n);
}
//-------------------------------------------------------------------------------------------------
}
#endif // GEEK_TOOLKITS_SINGLETON_CONFIG_HPP



