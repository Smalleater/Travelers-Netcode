#ifndef TRA_NETCODE_ENGINE_DISCONNECT_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_DISCONNECT_SYSTEM_HPP

#include "internal/iNetworkSystem.hpp"

namespace tra::netcode::engine
{
	struct DisconnectSystem : INetworkSystem
	{
		void update(NetworkEcs* _ecs) override;
	};
}

#endif
