#ifndef TRA_ECS_ENTITY_HPP
#define TRA_ECS_ENTITY_HPP

#include <cstdint>

namespace tra::ecs
{
	using EntityId = uint32_t;
	using EntityVersion= uint32_t;

	struct Entity
	{
		EntityId m_id;
		EntityVersion m_version;

		bool operator==(const Entity& _other) const noexcept
		{
			return m_id == _other.m_id && m_version == _other.m_version;
		}
	};
}

#endif
