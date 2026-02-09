#include "internal/disconnectSystem.hpp"

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"
#include "TRA/netcode/engine/tags.hpp"

namespace tra::netcode::engine::internal
{
	void DisconnectSystem::update(ecs::World* _world)
	{
		for (auto& [entity] : _world->queryEntities(
			ecs::WithComponent<>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<tags::DisconnectedTag>{},
			ecs::WithoutTag<tags::SelfTag>{}))
		{
			_world->destroyEntity(entity);
		}
	}
}