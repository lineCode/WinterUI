#pragma once

#include "api.hh"

#include <atomic>

struct Spinlock final
{
	
	inline void lock() { while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory"); }
	inline bool tryLock() { return !accessor.test_and_set(); }
	inline void unlock() { accessor.clear(); }

private:
	
	std::atomic_flag accessor {false};
};

struct RWSpinlock final
{
	
	RWSpinlock() = default;
	
	inline void readAccess()
	{
		while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory");
		while (writeSem.test_and_set()) __asm volatile ("pause" ::: "memory");
		writeSem.clear();
		readers.fetch_add(1);
		accessor.clear();
	}
	
	inline bool readAccessTry()
	{
		if (accessor.test_and_set()) return false;
		if (writeSem.test_and_set())
		{
			accessor.clear();
			return false;
		}
		writeSem.clear();
		readers.fetch_add(1);
		accessor.clear();
		return true;
	}
	
	inline void readDone()
	{
		readers.fetch_sub(1);
	}
	
	inline void writeLock()
	{
		while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory");
		while (writeSem.test_and_set()) __asm volatile ("pause" ::: "memory");
		while (readers.load()) __asm volatile ("pause" ::: "memory");
		accessor.clear();
	}
	
	inline bool writeLockTry()
	{
		if (accessor.test_and_set()) return false;
		if (writeSem.test_and_set())
		{
			accessor.clear();
			return false;
		}
		if (readers.load())
		{
			writeSem.clear();
			accessor.clear();
			return false;
		}
		accessor.clear();
		return true;
	}
	
	inline void writeUnlock()
	{
		writeSem.clear();
	}
	
	inline void writeToRead()
	{
		readers.fetch_add(1);
		writeSem.clear();
	}

private:
	std::atomic_uint_fast8_t readers {0};
	std::atomic_flag accessor {false};
	std::atomic_flag writeSem {false};
};
