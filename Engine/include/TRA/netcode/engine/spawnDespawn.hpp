#ifndef TRA_NETCODE_ENGINE_SPAWN_DESPAWN_HPP
#define TRA_NETCODE_ENGINE_SPAWN_DESPAWN_HPP

#include <cstdint>

namespace tra::netcode::engine
{
	struct Spawn
	{
		uint32_t _networkId;
		uint32_t _ownerId;
		uint16_t _tag;
	};

	struct Despawn
	{
		uint32_t _networkId;
	};
}

#endif
