#ifndef TRA_NETCODE_ENGINE_INTERNAL_I_NETWORK_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_I_NETWORK_SYSTEM_HPP

#include <cstdint>

namespace tra::netcode::engine
{
	using EntityId = uint32_t;

	class NetworkEcs;
	struct INetworkSystem
	{
		virtual ~INetworkSystem() = default;
		virtual void update(NetworkEcs* _ecs) = 0;
	};
}

#endif
