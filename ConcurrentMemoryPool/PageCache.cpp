#include"PageCache.h"
PageCache PageCache::_inst;

Span* PageCache::NewSpan(size_t n)
{
	assert(n < NPAGES);
	if (!_spanlist[n].empty())
	{
		return _spanlist[n].PopFront();
	}
	for (size_t i = n + 1; i < NPAGES; ++i)
	{
		if (!_spanlist[i].empty())
		{
			Span* span = _spanlist[i].PopFront();
			Span* split = new Span;

			split->_pageid = span->_pageid+n;
			split->_npage = span->_npage-n;
			span->_npage = n;
			for (size_t i = 0; i < n; ++i)
			{
				_idspanmap[split->_pageid + i] = split;
			}
			_spanlist[split->_npage].PushFront(split);
			return span;
		}
	}
	Span* span = new Span;
	void* ptr = VirtualAlloc(0, (NPAGES - 1)*(1 << PAGE_SHIFT),
		MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	span->_pageid = (PageID)ptr >> PAGE_SHIFT;
	span->_npage = NPAGES - 1;

	for (size_t i = 0; i < span->_npage; ++i)
	{
		_idspanmap[span->_pageid + 1] = span;
	}
	_spanlist[span->_npage].PushFront(span);
	return NewSpan(n);
}

Span* PageCache::MapObjectToSpan(void* obj)
{
	PageID id = (PageID)obj >> PAGE_SHIFT;
	auto it = _idspanmap.find(id);
	if (it != _idspanmap.end())
	{
		return it->second;
	}
	else
	{
		assert(false);
		return nullptr;
	}
}

void PageCache::ReleaseSpanToPageCache(Span* cur)
{
	while (1)
	{
		PageID curid = cur->_pageid;
		PageID previd = curid - 1;
		auto it = _idspanmap.find(previd);
		if (it == _idspanmap.end())
		{
			break;
		}
		if (it->second->_usecount != 0)
		{
			break;
		}
		Span* prev = it->second;
		_spanlist[prev->_npage].Erase(prev);

		prev->_npage += cur->_npage;
		delete cur;
		cur = prev;
	}

	while (1)
	{
		PageID curid = cur->_pageid;
		PageID nextid = cur->_pageid + 1;
		auto it = _idspanmap.find(nextid);
		if (it == _idspanmap.end())
		{
			break;
		}
		if (it->second->_usecount != 0)
		{
			break;
		}
		Span* next = it->second;
		_spanlist[next->_npage].Erase(next);
		cur->_npage += next->_npage;
		_spanlist[cur->_npage].PushFront(cur);
	}
}