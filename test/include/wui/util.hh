#pragma once

#include <string>
#include <atomic>
#include <fstream>
#include <vector>

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

inline std::vector<std::string> splitStr(std::string const &input, char const &splitOn, bool const &removeMatches)
{
	std::vector<std::string> out;
	if(input.begin() == input.end()) return out;
	std::string::const_iterator iEnd = input.begin(), iBegin = iEnd;
	while(iEnd < input.end())
	{
		if(*iEnd == splitOn)
		{
			if(iBegin != iEnd) out.push_back(std::string{iBegin, removeMatches ? iEnd : iEnd + 1});
			iBegin = ++iEnd;
			continue;
		}
		iEnd++;
	}
	if(*iEnd == splitOn && iEnd != iBegin) out.push_back(std::string{iBegin, removeMatches ? iEnd : iEnd + 1});
	else if(iBegin != iEnd) out.push_back(std::string{iBegin, iEnd});
	return out;
}
