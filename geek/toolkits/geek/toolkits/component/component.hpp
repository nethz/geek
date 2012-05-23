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
/// @file       component.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_COMPONENT_HPP
#define GEEK_TOOLKITS_COMPONENT_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/component/config.hpp>
#include <geek/toolkits/component/component_interface_query.hpp>
#include <geek/toolkits/component/component_interface.hpp>


namespace geek{

///////////////////////////////////////////////////////////////////////
/// @brief      component ; 组件
///
/// @details    
/// @date       2011/09/27
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_COMPONENT_DECL component
{
public:
	virtual ~component(void);
	component(void);

	/////////////////////////////////////////////////
	/// @brief     query interface ; 查询接口
	/// @details  reflex mechanism ; 简单的反射机制
	/// @param 
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	template<class InterfaceType>
	inline InterfaceType* query_interface(void)
	{
		return dynamic_cast<InterfaceType *>(_query_interface(typeid(InterfaceType)));
	}

	/////////////////////////////////////////////////
	/// @brief    query interface ;  查询接口2
	/// @details   reflex mechanism ; 简单的反射机制
	/// @param info interface type info ; 接口类型信息
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	virtual component_interface * _query_interface(std::type_info const& info);
protected:
	
	component_interface_query query_;
};
typedef boost::shared_ptr<component> component_ptr;

}
#endif // GEEK_TOOLKITS_SINGLETON_CONFIG_HPP



