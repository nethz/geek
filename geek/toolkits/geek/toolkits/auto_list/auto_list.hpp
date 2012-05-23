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
/// @file       auto_list.hpp
/// @brief      
/// @details    
/// @version    0.99a
/// @author     RD
/// @date       2011/09/23
///////////////////////////////////////////////////////////////////////



#ifndef GEEK_TOOLKITS_AUTO_LIST_HPP
#define GEEK_TOOLKITS_AUTO_LIST_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif



#include <geek/toolkits/auto_list/config.hpp>



namespace geek{
	
template<typename T> class 
auto_list
{

public:
	///列表类型
	typedef typename std::vector<T *> list_type;

	///迭代器类型
	typedef typename list_type::iterator iterator;

	///迭代器常量类型
	typedef typename list_type::const_iterator const_iterator;

public:
	
	auto_list(void):inlist_(false){}
	virtual ~auto_list(void){}

	///连接入列表
	void link(void)
	{
		if(!inlist_)
		{
			list_.push_back(static_cast<T *>(this));
			inlist_ = true;
		}
	}
	///从列表中移除
	void unlink(void)
	{
		if(inlist_)
		{

			/*
			list_type::iterator end_itr(list_.end());
			list_type::iterator itr(
				std::find(
				list_.begin(),
				end_itr, this
				)
				);

			if (itr != end_itr)
			{
				T*& curr = *itr;
				// swap and pop back vector to effective erase element
				T*& back = list_.back(); // can't be empty, so this always success
				
				if (back != this)
				{
					using std::swap;
					swap(back, curr); // swap always success in c++ standard
				}

				list_.pop_back(); // pop back to erase and destroy plugin that swapped
			}

*/
			list_.erase(std::remove(list_.begin(), list_.end(), this), list_.end());
			inlist_ = false;
		}
	}

	///检查是否在列表中
	bool is_link(void)
	{
		return inlist_;
	}

	///移除所有列表中实例，并清空列表
	static void unlink_all(void)
	{
		for(iterator it = begin(); it != end(); ++it)
		{
			(*it)->inlist_ = false;
		}
		list_.clear();
	}

	///得到起始位置迭代器
	static iterator begin(void)
	{
		return(list_.begin());
	}
	///得到最终位置迭代器
	static iterator end(void)
	{
		return (list_.end());
	}

	///得到起始位置常量迭代器
	static const_iterator const_begin(void)
	{
		const list_type & clist = list_;
		return(clist.begin());
	}

	///得到最终位置常量迭代器
	static const_iterator const_end(void)
	{
		const list_type & clist = list_;
		return(clist.end());
	}
protected:
	static list_type list_;
private:
	bool inlist_;
};


}
#endif // GEEK_TOOLKITS_SINGLETON_CONFIG_HPP



