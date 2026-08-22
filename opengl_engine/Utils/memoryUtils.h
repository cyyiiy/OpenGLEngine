#pragma once
#include <string>
#include <unordered_map>


namespace MemoryUtils
{
	// Note: This is an approximation and changes depending of the compilator (GCC ~= 16, Clang ~= 24, MSVC ~= 32)
	constexpr size_t mapNodeOverhead = 32;

	// Threshold of the Small String Optimization -> a string won't have a heap cost if it has less than 15 chars
	constexpr size_t ssoThreshold = 15;


	/** Get the memory used on the heap by a string. */
	inline uint64_t GetStringHeapMemory(const std::string& str)
	{
		return (str.capacity() > ssoThreshold) ? str.capacity() + 1 : 0;
	}

	/** Get an estimation of the memory used on the heap by an unordered map. */
	template <typename K, typename V>
	uint64_t EstimateUnorderedMapHeapMemory(const std::unordered_map<K, V>& map)
	{
		uint64_t total = 0;

		// 1. Compute the size of the bucket list of the map
		total += map.bucket_count() * sizeof(void*);

		// 2. Compute the size of each node of the map
		total += map.size() * sizeof(std::string) + sizeof(V) + mapNodeOverhead;

		return total;
	}

	/** Get an estimation of the memory used on the heap by an unordered map. Specialisation for string keys. */
	template <typename V>
	uint64_t EstimateUnorderedMapHeapMemory(const std::unordered_map<std::string, V>& map)
	{
		uint64_t total = 0;

		// 1. Compute the size of the bucket list of the map
		total += map.bucket_count() * sizeof(void*);

		// 2. Compute the size of each node of the map
		total += map.size() * sizeof(std::string) + sizeof(V) + mapNodeOverhead;

		// 3. Add the heap memory used by the string keys
		for (const auto& [key, value] : map)
		{
			total += GetStringHeapMemory(key);
		}

		return total;
	}
};

