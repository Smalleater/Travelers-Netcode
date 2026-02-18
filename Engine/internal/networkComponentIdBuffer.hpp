#ifndef TRA_NETCODE_ENGINE_INTERNAL_NETWORK_COMPONENT_ID_BUFFER_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_NETWORK_COMPONENT_ID_BUFFER_HPP

#include "TRA/ecs/componentLibrary.hpp"
#include "TRA/ecs/component.hpp"

namespace tra::netcode::engine::internal::components
{
	TRA_ECS_REGISTER_COMPONENT(NetworkComponentIdBuffer,
		std::vector<size_t> m_componentsId;
	)
}

#endif
