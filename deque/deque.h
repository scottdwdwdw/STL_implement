#ifndef DEQUE_H
#define DEQUE_H

#include"Allocator.h"
#include"Iterator.h"
#include"Algorithm.h"
#include<algorithm>




namespace MyCppSTL
{



	#define	_NODE_SIZE 8
	#define max(a,b)  (a>b)?(a):(b)
	inline std::size_t __deque_buf_size(std::size_t n) { return n < 512 ? (512 / n) : std::size_t(1); }

	/*  deque的const迭代器 */
	template<class T,class alloc=MyCppSTL::allocator<T>>
	class deque_const_iterator :public iterator<MyCppSTL::random_access_iterator_tag, T>
	{
	public:
		//内嵌型别
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque_const_iterator<T>    _MyIter;
		typedef pointer*				   map_pointer;
		//存储区大小
		static inline std::size_t deque_buf_size(std::size_t n){return __deque_buf_size(n);}
	public:
		//成员
		pointer _cur; //指示当前位置
		pointer _first;
		pointer _last;
		map_pointer _node;    //指示当前所在的map节点
	public:
		//默认构造函数
		deque_const_iterator():_cur(0), _first(0), _last(0),_node(0)
		{
		}
		//构造函数
		deque_const_iterator(pointer cur,map_pointer node):_cur(cur),_node(node)
		{
			set_node(node);
		}
	   
		//拷贝构造函数
		deque_const_iterator(_MyIter&rhs) :_cur(rhs._cur), _node(rhs._node), _first(rhs._first), _last(rhs._last)
		{
		}
		
		//迭代器操作
		const_reference operator*() const //取值
		{
			return *_cur;
		}
	
		//迭代器自增 ++
		_MyIter&operator++() //前置递增
		{
			++_cur;
			if (_cur == _last)   //到达该缓冲区的末尾
			{
				set_node(_node + 1);
				_cur = _first;
			}
			return *this;
		}

		_MyIter operator++(int) //后置递增
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}

		_MyIter&operator+=(difference_type n) //复合赋值
		{
			difference_type offset = n + (_cur - _first);
			if (offset >= 0 && offset < (difference_type)deque_buf_size(sizeof(size_type)))
			{
				_cur += n;
			}
			else
			{
				difference_type node_offset = offset > 0 ? (offset / (difference_type)deque_buf_size(sizeof(size_type))) :
					(offset / (difference_type)deque_buf_size(sizeof(size_type))) - 1;

				set_node(_node + node_offset);
				_cur = _first + (offset - node_offset*deque_buf_size(sizeof(size_type)));
			}
			return *this;
		}

		_MyIter operator+(difference_type n) const//
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}

		_MyIter&operator--() //前置递减
		{
			//--_cur;
			if (_cur == _first)   //到达该缓冲区的末尾
			{
				set_node(_node - 1);
			}
			_cur = --_last;
			return *this;
		}

		_MyIter operator--(int) //后置递减
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}
		_MyIter&operator-=(difference_type n) //复合赋值
		{
			*this += -n;
			return *this;
		}

		_MyIter operator-(difference_type n) //复合赋值
		{
			auto tmp = *this;
			tmp -= n;
			return tmp;
		}

		const_reference operator[](const size_type n)
		{
			return *(*this + n);
		}

		//逻辑操作
		bool operator==(_MyIter&rhs) const
		{
			return _cur == rhs._cur;
		}
		bool operator!=(_MyIter&rhs) const
		{
			return (!(*this == rhs));
		}
		bool operator<(_MyIter&rhs) const
		{
			return (_node == rhs._node ? _cur < rhs._cur : _node < rhs._node);
		}
		bool operator>(_MyIter&rhs) const
		{
			return (rhs < *this);
		}
		bool operator<=(_MyIter&rhs) const
		{
			return (!(*this>rhs));
		}

		bool operator>=(_MyIter&rhs) const
		{
			return (!(*this<rhs));
		}


	public:
		void set_node(map_pointer node)
		{
			_node = node;
			_first = *node;
			_last = _first + difference_type(deque_buf_size(sizeof(value_type)));
		}
		
	
	}; //end-of-const-iterator



template<class T, class alloc = MyCppSTL::allocator<T>>
class deque_iterator :public deque_const_iterator<T>
{
	public:
		//内嵌型别
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque_const_iterator<T>    _MyIter;
		typedef pointer*				   map_pointer;
		typedef deque_iterator<T>          _Myiter;
		typedef deque_const_iterator<T>    _MyBase;
	public:
		//构造函数
		//默认构造函数
		deque_iterator() :deque_const_iterator() {}
		//接受两个参数的构造函数
		deque_iterator(pointer cur, map_pointer node) :deque_const_iterator(cur, node) {}
		//拷贝构造函数
		deque_iterator(_Myiter&rhs) :deque_const_iterator(rhs) {}
		
		//迭代器的操作
		reference operator*()   //解引用
		{
			return (reference)**((_MyBase*)this);
		}
		_Myiter&operator++()  //前置递增
		{
			++*(_MyBase*)this;
			return *this;
		}
		_Myiter operator++(int) //后置递增
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		_Myiter&operator--()  //前置递减
		{
			--*(_MyBase*)this;
			return *this;
		}
		_Myiter operator--(int) //后置递减
		{
			auto tmp = *this;
			--*this;
			return tmp;
		}

		_Myiter &operator+=(difference_type n) //
		{
			*(_MyBase*)this += n;
				return *this;
		}

		_Myiter operator+(difference_type n)
		{
			auto tmp = *this;
			tmp += n;
			return tmp;
		}

		_Myiter operator-(difference_type n)
		{
			auto tmp = *this;
			tmp += -n;
			return tmp;
		}

		_Myiter &operator-=(difference_type n)
		{
			return *this += -n;
		}

		reference operator[](const size_type &n)
		{
			return *(*this + n);
		}


};  //end of iterator

/***************/
//deque模板
template<class T,class alloc=MyCppSTL::allocator<T>>
class deque {
	public:
		typedef  T							value_type;
		typedef  T&							reference;
		typedef const T&					const_reference;
		typedef  T*							pointer;
		typedef const T*					const_pointer;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef deque<T>					my_deque;

		typedef deque_const_iterator<T>		const_iterator;
		typedef deque_iterator<T>			iterator;
		typedef pointer*					map_pointer;
		
		typedef MyCppSTL::allocator<pointer>  map_alloc;
		enum {NODESIZE= _NODE_SIZE};
	public:
		//构造函数
	explicit deque()  //默认构造函数 
			{
				create_map(0);   //创建map
			}
	
	deque(size_type count, const T& value)  //接受一定数量的值初始化
	{
		create_map_and_node(count, value);
	}

	explicit deque(size_type count)
	{
		create_map_and_node(count, T());
	}

	template< class InputIt >
	deque(InputIt first, InputIt last)
	{
		
	}

	//迭代器
	
	iterator begin() { return iterator(_begin); }
	const_iterator begin() const 
	{ 
		return const_iterator(_begin._cur, _begin._node);
		
	}
	const_iterator cbegin() const 
	{
		return const_iterator(_begin._cur,_begin._node);
	}
	
	iterator end() { return _end; }
	const_iterator end() const 
	{ 
		return const_iterator(_end._cur,_end._node);
	}
	const_iterator cend() const
	{ 
		return const_iterator(_finish._cur, _finish._node); 
	}

	//元素操作
	void push_back(const value_type&value)
	{
		if (_end._cur != _end._last)
		{
			construct(_end._cur, value);
		}
		else
		{
		
			
		}
	}
	protected:
		//辅助函数，创建map
		std::size_t deque_buf_size(std::size_t n) { return __deque_buf_size(n); }
		void create_map(size_type elem_num);
		void create_map_and_node(size_type elem_num,value_type value);

		template< class InputIt >
		void deque_aux(InputIt first, InputIt last, std::false_type);  //是迭代器
		template< class InputIt >
		void deque_aux(InputIt first, InputIt last, std::true_type);   //不是迭代器
		
		void push_back_aux(const value_type&value); //当map空间不够的时候从新分配
		void push_front_aux(const value_type&value);

		
		void reserve_map_at_back(size_type nodes_to_add = 1);
		void reserve_map_at_front(size_type nodes_to_add = 1);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);

	private:
		map_pointer	_map; //指向map的指针
		size_type _map_size; //map大小
		iterator _begin; 
		iterator _end;   
	
	};  //end of deque


	//deque implement
	/***辅助函数****/

	//创建map

    template<class T, class alloc = MyCppSTL::allocator<T>>
	void deque<T, alloc>::create_map(size_type elem_num)
	{
		//计算需要多个map节点
		//deque_buf_size返回的是单个节点存储大小
		size_type num_nodes = elem_num / deque_buf_size(sizeof(value_type)) +1;  
		_map_size = max(NODESIZE, num_nodes+2);   
		_map =map_alloc::allocate(_map_size);    //分配节点
		map_pointer nstart = _map + (_map_size - num_nodes) / 2;//从中间开始
		map_pointer nend = nstart + num_nodes-1;    
		map_pointer cur = nstart;
		for (; cur <= nend; ++cur)
		{
			*cur=alloc::allocate(deque_buf_size(sizeof(value_type))); //为每个节点分配存储区
		}
		_begin.set_node(nstart);
		_end.set_node(nend);
		_begin._cur = _begin._first;
		_end._cur = _end._first + elem_num%deque_buf_size(sizeof(value_type));
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	void deque<T, alloc>::create_map_and_node(size_type count, value_type value)
	{
		create_map(count);    //构造map节点，并初始化成员
		map_pointer cur = _begin._node;
		for (; cur < _end._node - 1; ++cur)
		{
			uninitialized_fill(*cur, *cur + deque_buf_size(sizeof(value_type)), value);
		}
		uninitialized_fill(_end._first, _end._cur, value);
	}

	template<class T,class alloc=MyCppSTL::allocator<T>>
	template<class InputIt>
	void deque<T, alloc>::deque_aux(InputIt first, InputIt last, std::false_type)
	{
		create_map(0);   //构造出空deque
		for (; first < last; ++first)
		{
		
		}
	}


}  




#endif 
