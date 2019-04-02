#pragma once
#include<iostream>
#include<vector>
#include<thread>
#include<map>
#include<assert.h>
#include<algorithm>
using std::cout;
using std::endl;

#include<Windows.h>

const size_t MAX_BYTES = 64 * 1024;
const size_t NLISTS = MAX_BYTES / 8;
const size_t NPAGES = 129;
const size_t PAGE_SHIFT = 12;

inline static void*& NEXT_OBJ(void* obj)
{
	return (*(void**)obj);
}
class FreeList
{
public:
	void Push(void* obj)
	{
		NEXT_OBJ(obj) = _list;
		obj = _list;
		++_size;
	}
	void PushRange(void* start, void * end, size_t n)
	{
		NEXT_OBJ(end) = _list;
		_list = start;
		_size += n;
	}
	void* Pop()
	{
		void* obj = _list;
		_list = NEXT_OBJ(obj);
		--_size;
		return obj;
	}
	bool empty()
	{
		return _list == nullptr;
	}

private:
	void* _list = nullptr;
	size_t _size = 0;
};

class SizeClass
{
	inline static size_t _Index(size_t size, size_t align)
	{
		size_t alignnum = 1 << align;
		return ((size + alignnum - 1 )>> align) - 1;
	}
	inline static size_t _Roundup(size_t size,size_t align)
	{
		size_t alignnum = 1 << align;
		return (size + alignnum - 1)&~(alignnum - 1);
	}
public:
	inline static size_t Index(size_t size)
	{
		assert(size <= MAX_BYTES);
		static int group_array[4] = { 16, 56, 56, 56 };
		if (size <= 128)
		{
			return _Index(size, 3);
		}
		else if (size <= 1024)
		{
			return _Index(size - 128, 4) + group_array[0];
		}
		else if (size<=8192)
		{
			return _Index(size - 1024, 7) + group_array[0] + group_array[1];
		}
		else
		{
			return _Index(size - 8 * 1024, 10) + group_array[0] + group_array[1] + group_array[2];
		}
	}
	inline static size_t Roundup(size_t bytes)
	{
		assert(bytes <= MAX_BYTES);
		if (bytes <= 128)
		{
			return _Roundup(bytes, 3);
		}
		else if (bytes <= 1024)
		{
			return _Roundup(bytes, 4);
		}
		else if (bytes <= 8192)
		{
			return _Roundup(bytes, 7);
		}
		else
		{
			return _Roundup(bytes, 10);
		}
	}

	static size_t NumMoveSize(size_t size)
	{
		if (size == 0)
			return 0;
		int num = (int)(MAX_BYTES / size);
		if (num < 2)
			num = 2;
		if (num>512)
			num = 512;
		return num;
	}

	//计算一次向系统获取几个页
	static size_t NumMovePage(size_t size)
	{
		size_t num = NumMoveSize(size);
		size_t npage = num*size;
		npage >>= PAGE_SHIFT;
		if (npage == 0)
			npage = 1;
		return npage;
	}
};

#ifdef _WIN32
typedef size_t PageID;
#else
typedef long long PageID;
#endif
    
   
struct Span
{
	PageID _pageid = 0;
	size_t _npage = 0;

	Span* _prev = nullptr;
	Span* _next = nullptr;
	void* _list = nullptr;

	size_t _usecount = 0;
	size_t _objsize = 0;

};

//双向带头循环链表
class SpanList
{

public:
	SpanList()
	{
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	}
	Span* begin()
	{
		return _head->_next;
	}
	Span* end()
	{
		return _head;
	}
	SpanList(const SpanList&) = delete;
	SpanList& operator=(const SpanList&) = delete;

	~SpanList()
	{
		Span* cur = _head->_next;
		while (cur != _head)
		{
			Span* next = cur->_next;
			delete cur;
			cur = next;
		}
		delete _head;
		_head = nullptr;
	}
	void PushBack(Span* newspan)
	{
		Insert(begin(),newspan);
	}
	void PushFront(Span* newspan)
	{
		Insert(end(), newspan);
	}
	Span* PopBack()
	{
		Span* span = _head->_prev;
		Erase(span);
		return span;
	}
	Span* PopFront()
	{
		Span* span = _head->_next;
		Erase(span);
		return span;

	}
	bool empty()
	{
		return (_head->_next == _head);
	}
	void Insert(Span* cur, Span* newspan)
	{
		Span* prev = cur->_prev;
		newspan->_next = cur;
		prev->_next = newspan;
		newspan->_prev = prev;
		cur->_prev = newspan;
	}
	void Erase(Span* cur)
	{
		Span* prev = cur->_prev;
		Span* next = cur->_next;
		prev->_next = next;
		next->_prev = prev;
	}

private:
	Span* _head;
};