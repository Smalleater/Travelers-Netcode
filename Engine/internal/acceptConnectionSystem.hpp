#ifndef TRA_NETCODE_ENGINE_ACCEPT_CONNECTION_SYSTEM_HPP
#define TRA_NETCODE_ENGINE_ACCEPT_CONNECTION_SYSTEN_HPP

#include "iNetworkSystem.hpp"

namespace tra::netcode::engine
{
	struct AcceptConnectionSystem : INetworkSystem
	{
		void update(NetworkEcs* _ecs) override;
	};
}

#endif
