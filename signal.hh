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
		SharedData(Signal *parent) : parent(parent) {}
		
		Signal *parent;
		RWSpinlock accessor;
	};
	
	struct Connection
	{
		~Connection()
		{
			this->data->accessor.read_access();
			if(this->data->parent)
			{
				this->data->parent->disconnect(this->id);
			}
			this->data->accessor.read_done();
		}
		
		ID id;
		SP<SharedData> data;
	};
	
	Signal() = default;
	~Signal()
	{
		this->data->accessor.write_lock();
		this->data->parent = nullptr;
		this->data->accessor.write_unlock();
	}
	
	Connection connect(Func f)
	{
		this->sl.lock();
		ID id = this->idIncrementor++;
		this->cbs.emplace_back(id, std::move(f));
		Connection out{.id = id, .data = data};
		this->sl.unlock();
		return out;
	}
	
	void disconnect(ID id)
	{
		this->sl.lock();
		this->cbs.erase(std::remove_if(this->cbs.begin(), this->cbs.end(), [id](auto const &v){return v.first == id;}));
		this->sl.unlock();
	}
	
	void fire(Args ... args)
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


