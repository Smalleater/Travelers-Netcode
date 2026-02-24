#ifndef TRA_NETCODE_ENGINE_SPAWN_DESPAWN_HPP
#define TRA_NETCODE_ENGINE_SPAWN_DESPAWN_HPP

#include <cstdint>

namespace tra::netcode::engine
{
	struct Spawn
	{
		uint32_t m_networkId;
		uint32_t m_ownerId;
		uint16_t m_tag;
	};

	struct Despawn
	{
		uint32_t m_networkId;
	};
}

#endif
