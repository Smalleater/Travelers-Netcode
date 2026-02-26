#ifndef TRA_NETCODE_ENGINE_INTERNAL_SNAPSHOT_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_SNAPSHOT_COMPONENT_HPP

#include <unordered_map>
#include <string>

#include "TRA/ecs/component.hpp"

#include "TRA/netcode/engine/networkId.hpp"
#include "TRA/netcode/engine/networkComponent.hpp"

namespace tra::netcode::engine::internal::components
{
	TRA_ECS_REGISTER_COMPONENT(SnapshotComponent,
		int m_lastSnapshotId = 0;
		std::unordered_map<NetworkId, std::unordered_map<std::string, std::shared_ptr<NetworkComponent>>> m_firstSnapshot;
		std::unordered_map<NetworkId, std::unordered_map<std::string, std::shared_ptr<NetworkComponent>>> m_secondSnapshot;
	)
}

#endif
