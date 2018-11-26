#pragma once

#include "util.hh"
#include "definitions.hh"
#include "observer.hh"

#include <functional>

template <typename... Args> struct Signal
{
	using Func = std::function<void(Args ...)>;
	using ID = uint64_t;
	using Callback = std::pair<ID, Func>;
	
	struct SharedData
	{
		inline SharedData(Signal *parent) : parent(parent) {}
		
		Signal *parent;
		RWSpinlock accessor;
	};
	
	struct Connection : public ConnectionBase
	{
		inline Connection(ID id, SP<SharedData> data) : id(id), data(data) {}
		
		inline ~Connection()
		{
			this->data->accessor.readAccess();
			if(this->data->parent) this->data->parent->disconnect(this->id);
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
	
	inline void connect(Observer &connectionObserver, Func f)
	{
		this->sl.lock();
		ID id = this->idIncrementor++;
		this->cbs.emplace_back(id, std::move(f));
		this->sl.unlock();
		connectionObserver.connections.emplace_back(MU<Connection>(id, this->data));
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
