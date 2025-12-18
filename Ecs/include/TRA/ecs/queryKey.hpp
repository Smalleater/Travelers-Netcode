#ifndef TRA_ECS_QUERY_KEY_HPP
#define TRA_ECS_QUERY_KEY_HPP

#include <vector>

namespace tra::ecs
{
	struct QueryKey
	{
		std::vector<size_t> m_typeHashes;

		bool operator==(const QueryKey& _other) const
		{
			return m_typeHashes == _other.m_typeHashes;
		}
	};

	struct QueryWithEntitiesKey
	{
		QueryKey m_queryKey;
		size_t m_entitiesHash;

		bool operator==(const QueryWithEntitiesKey& _other) const
		{
			return m_queryKey == _other.m_queryKey && m_entitiesHash == _other.m_entitiesHash;
		}
	};
}

namespace std
{
	template<>
	struct hash<tra::ecs::QueryKey>
	{
		size_t operator()(const tra::ecs::QueryKey& _key) const
		{
			size_t seed = 0;
			for (size_t h : _key.m_typeHashes)
			{
				seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}

			return seed;
		}
	};

	template<>
	struct hash<tra::ecs::QueryWithEntitiesKey>
	{
		size_t operator()(const tra::ecs::QueryWithEntitiesKey& _key) const
		{
			return std::hash<tra::ecs::QueryKey>()(_key.m_queryKey) ^ (_key.m_entitiesHash + 0x9e3779b9);
		}
	};
}

#endif
