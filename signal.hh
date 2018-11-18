#pragma once

#include "util.hh"

#include <functional>
#include <beast/def.hh>

template <typename... Args> struct Signal
{
	using Func = std::function<void(Args ...)>;
	using ID = uint64_t;
	using Callback = std::pair<ID, Func>;
	
	struct SharedData
	{
		inline explicit SharedData(Signal *parent) : parent(parent) {}
		
		Signal *parent;
		RWSpinlock accessor;
	};
	
	struct Connection
	{
		inline ~Connection()
		{
			this->data->accessor.readAccess();
			if(this->data->parent)
			{
				this->data->parent->disconnect(this->id);
			}
			this->data->accessor.readDone();
		}
		
		ID id;
		SP<SharedData> data;
	};
	
	Signal() = default;
	
	inline ~Signal()
	{
		this->data->accessor.writeLock();
		this->data->parent = nullptr;
		this->data->accessor.writeUnlock();
	}
	
	inline Connection connect(Func f)
	{
		this->sl.lock();
		ID id = this->idIncrementor++;
		this->cbs.emplace_back(id, std::move(f));
		Connection out{.id = id, .data = data};
		this->sl.unlock();
		return out;
	}
	
	inline void disconnect(ID id)
	{
		this->sl.lock();
		this->cbs.erase(std::remove_if(this->cbs.begin(), this->cbs.end(), [id](auto const &v){return v.first == id;}));
		this->sl.unlock();
	}
	
	inline void fire(Args ... args)
	{
		this->sl.lock();
		for(Callback &callback : this->cbs)
		{
			callback.second(args...);
		}
		this->sl.unlock();
	}

private:
	std::vector<Callback> cbs;
	ID idIncrementor = 0;
	Spinlock sl;
	SP<SharedData> data = MS<SharedData>(this);
};


