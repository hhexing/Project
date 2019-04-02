#pragma once
#include"Common.h"
#include"ThreadCache.h"

void* ConcurrentAlloc(size_t size)
{
	if (size > MAX_BYTES)
	{
		return malloc(size);
	}
	else
	{
		if (tlslist == nullptr)
		{
			tlslist = new ThreadCache;
		}
		return tlslist->Allocate(size);
	}
}
void ConcurrentFree(void* ptr, size_t size)
{
	if (size > MAX_BYTES)
	{
		free(ptr);
	}
	else
	{
		return tlslist->Deallocate(ptr, size);
	}
}