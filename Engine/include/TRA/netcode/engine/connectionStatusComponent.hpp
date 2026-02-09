#ifndef TRA_NETCODE_ENGINE_CONNECTION_STATUS_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_CONNECTION_STATUS_COMPONENT_HPP

#include "TRA/netcode/engine/iNetworkComponent.hpp"

namespace tra::netcode::engine
{
	struct ConnectedComponentTag : INetworkComponent
	{

	};

	struct ListeningComponentTag : INetworkComponent
	{

	};
}

#endif
