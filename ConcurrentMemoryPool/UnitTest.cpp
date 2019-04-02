#include "Common.h"
#include "ConcurrentAlloc.h"
#include"PageCache.h"

void TestSize()
{
	cout << SizeClass::Index(10) << endl;
	cout << SizeClass::Index(16) << endl;

	cout << SizeClass::Roundup(10) << endl;
	cout << SizeClass::Roundup(12) << endl;
	cout << SizeClass::Roundup(16) << endl;
	cout << SizeClass::Roundup(17) << endl;
}
void Alloc(size_t n)
{
	std::vector<void*> v;
	for (size_t i = 0; i < n; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
	}

}
void TestThreadCache()
{
	std::thread t1(Alloc, 5);
	t1.join();
}
void TestCentralCache()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 8; ++i)
	{
		v.push_back(ConcurrentAlloc(10));
	}
	for (size_t i = 0; i < 8; ++i)
	{
		cout << v[i] << endl;
	}
}
void TestPageCache()
{
	PageCache::GetInstance()->NewSpan(2);
}
int main()
{
	//TestSize();
	//TestThreadCache();
	//TestCentralCache();
	TestPageCache();
	return 0;
}