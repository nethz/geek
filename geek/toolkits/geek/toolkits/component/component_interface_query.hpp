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
/// @file       component_interface_query.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_COMPONENT_INTERFACE_QUERY_HPP
#define GEEK_TOOLKITS_COMPONENT_INTERFACE_QUERY_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/component/config.hpp>
#include <geek/toolkits/pool/stl_containter.hpp>



namespace geek{
class component;
class component_interface;

///////////////////////////////////////////////////////////////////////
/// @brief     interface query ; 接口查询类
///
/// @details    
/// @date       2011/09/27
///////////////////////////////////////////////////////////////////////
class GEEK_TOOLKITS_COMPONENT_DECL component_interface_query
{
public:
	component_interface_query(void);
	~component_interface_query(void);


	/////////////////////////////////////////////////
	/// @brief     add a sub component ; 添加一个子组件;
	/// @details   
	/// @param component sub component ptr ; 组件指针
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	void add(component* component);


	/////////////////////////////////////////////////
	/// @brief     add a interface ; 添加一个组件接口
	/// @details   
	/// @param c_interface component interface ;组件接口
	/// @return    
	/// @note      
	/////////////////////////////////////////////////
	void add(component_interface* c_interface);
	component_interface* query_interface(std::type_info const& info);
private:

	typedef std::vector<component*> component_list;
	typedef std::vector<component_interface*> interface_list;
	component_list components_;
	interface_list interfaces_;
	
};


}
#endif // GEEK_TOOLKITS_COMPONENT_INTERFACE_QUERY_HPP



