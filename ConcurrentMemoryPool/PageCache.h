#include"Common.h"

class PageCache
{
public:
	static PageCache* GetInstance()
	{
		return &_inst;
	}
	Span* NewSpan(size_t size);

	Span* MapObjectToSpan(void* obj);

	void ReleaseSpanToPageCache(Span* span);

private:
	SpanList _spanlist[NPAGES];
	std::map<PageID, Span*> _idspanmap;
private:
	PageCache(){}
	PageCache(const PageCache&) = delete;
	static PageCache _inst;

};