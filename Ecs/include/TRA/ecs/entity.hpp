#ifndef TRA_ECS_ENTITY_HPP
#define TRA_ECS_ENTITY_HPP

#include <cstdint>
#include <functional>

namespace tra::ecs
{
	using EntityId = uint32_t;
	using EntityVersion = uint32_t;

	struct Entity
	{
		static const Entity Null;

		EntityId m_id : 24;
		EntityVersion m_version : 8;

		bool operator==(const Entity& _other) const noexcept
		{
			return m_id == _other.m_id && m_version == _other.m_version;
		}
	};
}

namespace std 
{
	template<>
	struct hash<tra::ecs::Entity>
	{
		size_t operator()(const tra::ecs::Entity& _e) const noexcept
		{
			size_t h1 = std::hash<tra::ecs::EntityId>{}(_e.m_id);
			size_t h2 = std::hash<tra::ecs::EntityVersion>{}(_e.m_version);
			return h1 ^ (h2 << 1);
		}
	};
}

#endif
