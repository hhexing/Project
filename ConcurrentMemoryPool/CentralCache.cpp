#include"CentralCache.h"
#include"PageCache.h"


CentralCache CentralCache::_inst;
Span* CentralCache::GetOneSpan(SpanList& spanlist, size_t byte_size)
{
	Span* span = spanlist.begin();
	while (span != spanlist.end())
	{
		if (span->_list != nullptr)
			return span;
		else
			span = span->_next;
	}

	Span* newspan = PageCache::GetInstance()->NewSpan(SizeClass::NumMovePage(byte_size));
	char* cur = (char*)(newspan->_pageid << PAGE_SHIFT);
	char* end = cur + (newspan->_npage << PAGE_SHIFT);

	newspan->_list = cur;
	newspan->_objsize = byte_size;

	while (cur + byte_size < end)
	{
		char* next = cur + byte_size;
		NEXT_OBJ(cur) = next;
		cur = next;
	}
	NEXT_OBJ(cur) = nullptr;

	spanlist.PushFront(newspan);
	return newspan;
}


size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size)
{
	size_t index = SizeClass::Index(byte_size);
	SpanList& spanlist = _spanlist[index];
	Span* span = GetOneSpan(spanlist, byte_size);

	size_t batchsize = 0;
	void* prev = nullptr;
	void* cur = span->_list;
	for (size_t i = 0; i < n; ++i)
	{
		prev = cur;
		cur = NEXT_OBJ(cur);
		++batchsize;
		if (cur == nullptr)
			break;
	}
	start = span->_list;
	end = prev;
	span->_list = cur;
	span->_usecount += batchsize;

	if (span->_list == nullptr)
	{
		spanlist.Erase(span);
		spanlist.PushBack(span);
	}
	return batchsize;
}

void CentralCache::ReleaseListToSpan(void* start, size_t size)
{
	size_t index = SizeClass::Index(size);
	SpanList& spanlist = _spanlist[index];
	while (start)
	{
		void* next = NEXT_OBJ(start);
		Span* span = PageCache::GetInstance()->MapObjectToSpan(start);
		NEXT_OBJ(start) = span->_list;
		span->_list = start;
		if (--span->_usecount == 0)
		{
			PageCache::GetInstance()->ReleaseSpanToPageCache(span);
		}
		start = next;
	}
}