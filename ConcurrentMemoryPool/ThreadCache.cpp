#include"ThreadCache.h"
#include"CentralCache.h"

//从中心缓存获取对象
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	size_t nutomove = SizeClass::NumMoveSize(size);
	void* start = nullptr;
	void* end = nullptr;
	size_t batchsize = CentralCache::GetInstance()->FetchRangeObj(start,end,nutomove,size);
	if (batchsize > 1)
	{
		FreeList* freelist = &_freelist[index];
		freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);
	}
	return start;
}

void* ThreadCache::Allocate(size_t size)
{
	size_t index =SizeClass::Index(size);
	FreeList* freelist = & _freelist[index];
	if (!freelist->empty())
	{
		return freelist->Pop();
	}
	else
	{
		return FetchFromCentralCache(index, SizeClass::Roundup(size));
	}
}
void ThreadCache::Deallocate(void* ptr,size_t size)
{
	size_t index = SizeClass::Index(size);
	FreeList* freelist = &_freelist[index];
	freelist->Push(ptr);
}