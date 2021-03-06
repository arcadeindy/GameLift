#pragma once
#include <stdlib.h>
#define OUT

class CircularBuffer
{
public:

	CircularBuffer(size_t capacity) : mBRegionPointer(nullptr), mARegionSize(0), mBRegionSize(0)
	{
		mBuffer = new char[capacity];
		mBufferEnd = mBuffer + capacity;
		mARegionPointer = mBuffer;
	}

	~CircularBuffer()
	{
		delete [] mBuffer;
	}

	bool Peek(OUT char* destbuf, size_t bytes) const;
	bool Read(OUT char* destbuf, size_t bytes);
	bool Write(const char* data, size_t bytes);


	void Remove(size_t len);

	size_t GetFreeSpaceSize()
	{
		if ( mBRegionPointer != nullptr )
			return GetBFreeSpace();
		else
		{
			if ( GetAFreeSpace() < GetSpaceBeforeA() )
			{
				AllocateB();
				return GetBFreeSpace();
			}
			else
				return GetAFreeSpace();
		}
	}

	size_t GetStoredSize() const
	{
		return mARegionSize + mBRegionSize;
	}

	size_t GetContiguiousBytes() const 
	{
		if ( mARegionSize > 0 )
			return mARegionSize;
		else
			return mBRegionSize;
	}

	void* GetBuffer() const
	{
		if( mBRegionPointer != nullptr )
			return mBRegionPointer + mBRegionSize;
		else
			return mARegionPointer + mARegionSize;
	}


	void Commit(size_t len)
	{
		if ( mBRegionPointer != nullptr )
			mBRegionSize += len;
		else
			mARegionSize += len;
	}

	void* GetBufferStart() const
	{
		if ( mARegionSize > 0 )
			return mARegionPointer;
		else
			return mBRegionPointer;
	}


private:

	void AllocateB()
	{
		mBRegionPointer = mBuffer;
	}

	size_t GetAFreeSpace() const
	{ 
		return (mBufferEnd - mARegionPointer - mARegionSize); 
	}

	size_t GetSpaceBeforeA() const
	{ 
		return (mARegionPointer - mBuffer); 
	}


	size_t GetBFreeSpace() const
	{ 
		if (mBRegionPointer == nullptr)
			return 0; 

		return (mARegionPointer - mBRegionPointer - mBRegionSize); 
	}

private:

	char*	mBuffer;
	char*	mBufferEnd;

	char*	mARegionPointer;
	size_t	mARegionSize;

	char*	mBRegionPointer;
	size_t	mBRegionSize;

};
