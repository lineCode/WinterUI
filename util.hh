#pragma once

#include <atomic>

struct Spinlock final
{
	
	inline void lock() { while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory"); }
	inline bool try_lock() { return !accessor.test_and_set(); }
	inline void unlock() { accessor.clear(); }

private:
	
	std::atomic_flag accessor {false};
};

struct RWSpinlock final
{
	
	RWSpinlock() = default;
	
	inline void read_access()
	{
		while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory");
		while (write_sem.test_and_set()) __asm volatile ("pause" ::: "memory");
		write_sem.clear();
		readers.fetch_add(1);
		accessor.clear();
	}
	
	inline bool read_access_try()
	{
		if (accessor.test_and_set()) return false;
		if (write_sem.test_and_set())
		{
			accessor.clear();
			return false;
		}
		write_sem.clear();
		readers.fetch_add(1);
		accessor.clear();
		return true;
	}
	
	inline void read_done()
	{
		readers.fetch_sub(1);
	}
	
	inline void write_lock()
	{
		while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory");
		while (write_sem.test_and_set()) __asm volatile ("pause" ::: "memory");
		while (readers.load()) __asm volatile ("pause" ::: "memory");
		accessor.clear();
	}
	
	inline bool write_lock_try()
	{
		if (accessor.test_and_set()) return false;
		if (write_sem.test_and_set())
		{
			accessor.clear();
			return false;
		}
		if (readers.load())
		{
			write_sem.clear();
			accessor.clear();
			return false;
		}
		accessor.clear();
		return true;
	}
	
	inline void write_unlock()
	{
		write_sem.clear();
	}
	
	inline void write_to_read()
	{
		readers.fetch_add(1);
		write_sem.clear();
	}

private:
	std::atomic_uint_fast8_t readers {0};
	std::atomic_flag accessor {false};
	std::atomic_flag write_sem {false};
};
