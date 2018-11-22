#pragma once

#include <string>
#include <atomic>
#include <fstream>
#include <locale>
#include <codecvt>

#if defined(_WIN32)
#include <windows.h>
#include <libloaderapi.h>
#elif defined(__linux__)
#include <sys/stat.h>
#include <unistd.h>
#endif

std::string getCWD()
{
#if defined(_WIN32)
	wchar_t rawdir[2048];
	GetModuleFileNameW(nullptr, rawdir, 2048);
	std::string exe_dir = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(std::wstring(rawdir));
	return exe_dir.substr(0, exe_dir.find_last_of('\\'));
#elif defined(__linux__)
	char dir[2048];
			getcwd(dir, 2048);
			return {dir};
#endif
}

std::string readTextFile(std::string const &filePath)
{
	std::ifstream fileIn;
	fileIn.open(filePath);
	if(!fileIn) printf("Error opening file %s\n", filePath.data());
	return {std::istreambuf_iterator<char>(fileIn), std::istreambuf_iterator<char>()};
}

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
