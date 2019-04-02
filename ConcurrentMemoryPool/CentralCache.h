#pragma once
#include"Common.h"

class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_inst;
	}

	//从Page Cache获取一个Span
	Span* GetOneSpan(SpanList& spanlist, size_t byte_sizes);
	
	//从中心缓存获取一定数量的对象给Thread Cache
	size_t FetchRangeObj(void*&start, void*&end, size_t n, size_t byte_sizes);

	//将一定数量的对象释放到span跨度
	void ReleaseListToSpan(void* start, size_t size);
private:
	SpanList _spanlist[NLISTS];
private:
	CentralCache()
	{}
	CentralCache(CentralCache&) = delete;
	static CentralCache _inst;
};