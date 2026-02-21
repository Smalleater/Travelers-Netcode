#ifndef TRA_NETCODE_ENGINE_NETWORK_ID_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_NETWORK_ID_COMPONENT_HPP

#include "TRA/ecs/component.hpp"

#include "TRA/netcode/engine/networkId.hpp"

namespace tra::netcode::engine::internal::components
{
	TRA_ECS_REGISTER_COMPONENT(NetworkIdComponent,
		NetworkId m_networkId;
	)
}

#endif
