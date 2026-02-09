#include "internal/pendingDisconnectSystem.hpp"

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/engine/tags.hpp"

#include "internal/tags.hpp"
#include "internal/socketComponent.hpp"

namespace tra::netcode::engine::internal
{
	void PendingDisconnectSystem::update(ecs::World* _world)
	{
		for (auto& [entity, socketPtr] : _world->queryEntities(
			ecs::WithComponent<components::TcpConnectSocketComponent>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<tags::PendingDisconnectTag, engine::tags::ConnectedTag>{}))
		{
			_world->removeTag<tags::PendingDisconnectTag>(entity);
			_world->removeTag<engine::tags::ConnectedTag>(entity);

			_world->addTag<engine::tags::DisconnectedTag>(entity);

			TRA_INFO_LOG("NetworkEngine: Entity ID: %I32u disconnected", entity.id());
		}
	}
}
