#pragma once

#include "api.hh"

#include <memory>
#include <mutex>
#include <algorithm>
#include <vector>
#include "def.hh"

/// Specialized pools for managing memory allocation and keeping similar objects in one place

namespace Beast
{
	namespace Util
	{
		/// A pointer pool using shared_ptr with automatic size management
		/// Objects are not stored in contiguous memory
		template <typename T> struct Pool
		{
			inline Pool() {this->pool.resize(this->upperThreshold);}
			inline ~Pool() {this->pool.clear();}
			
			/// Cannot be copied or moved
			BEAST_API Pool(Pool const &other) = delete;
			BEAST_API Pool & operator =(Pool const &other) = delete;
			BEAST_API Pool(Pool &&other) = delete;
			BEAST_API Pool & operator =(Pool &&other) = delete;
			
			/// Clean up unused objects and shrink the pool if needed
			inline void maintenance()
			{
				std::lock_guard<std::mutex> lock(this->mtx);
				this->pool.erase(std::remove_if(this->pool.begin(), this->pool.end(), [](SP<T> obj){return obj.use_count() == 1;}), this->pool.end());
				if(this->pool.size() <= this->upperThreshold / 2)
				{
					this->pool.shrink_to_fit();
					this->upperThreshold = static_cast<uint32_t>(this->pool.size() * this->allocFactor);
					this->pool.resize(this->upperThreshold);
				}
			}
			
			/// Create a new object
			template <typename... Args> inline SP<T> create(Args&&... args)
			{
				if(this->pool.size() + 1 > this->upperThreshold)
				{
					this->upperThreshold *= this->allocFactor;
					this->pool.resize(this->upperThreshold);
				}
				this->pool.emplace_back(MS<T>(std::forward<Args&&>(args)...));
				return this->pool.back();
			}
			
			/// Multiplier for how much to expand the pool by when upperThreshold is reached
			float allocFactor = 1.5f;
			std::vector<SP<T>> pool;
			
			private:
			std::mutex mtx;
			uint32_t upperThreshold = 20;
		};
	}
}
