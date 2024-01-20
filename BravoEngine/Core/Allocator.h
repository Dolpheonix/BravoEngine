#pragma once
#include <cstdint>
#include <malloc.h>

using namespace std;

class Allocator
{
public:
    Allocator(size_t size) : _base(nullptr), _size(size) {}
	virtual ~Allocator() {};
    virtual void* Allocate(size_t allocSize) = 0;
protected:
    void* _base;
    size_t _size;
};

class StackAllocator : public Allocator
{
	typedef size_t Marker;

public:
	StackAllocator(size_t size) : Allocator(size)
	{
		_base = malloc(size);
		_marker = 0;
	}
	~StackAllocator()
	{
		free(_base);
	}

public:
	virtual void* Allocate(size_t allocSize) override
	{
		void* subbase = (char*)_base + _marker;
		_marker += allocSize;
		return subbase;
	}
	void RollBack(Marker ret)
	{
		_marker = ret;
	}
	Marker getMarker() { return _marker; }

	void clear()
	{
		_marker = 0;
	}

private:
	// stack's top
	Marker _marker;
};