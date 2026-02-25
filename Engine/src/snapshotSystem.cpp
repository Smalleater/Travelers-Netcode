#include "internal/snapshotSystem.hpp"

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/engine/fieldSerializer.hpp"

#include "internal/snapshotMessage.hpp"
#include "internal/networkIdComponent.hpp"
#include "internal/networkComponentIdBuffer.hpp"
#include "internal/networkComponentFactory.hpp"
#include "internal/messageComponent.hpp"
#include "internal/socketComponent.hpp"

namespace tra::netcode::engine::internal
{
	void SendSnapshotSystem::update(ecs::World* _world)
	{
		if (_world->queryEntities(ecs::WithComponent<components::TcpListenSocketComponent>{}).empty())
		{
			return;
		}

		auto snapshotMessage = std::make_shared<tra::message::SnapshotMessage>();

		for (auto& [entity, networkId, networkComponentIdBuffer] : _world->queryEntities(
			ecs::WithComponent<components::NetworkIdComponent, components::NetworkComponentIdBuffer>{}))
		{
			if (networkComponentIdBuffer->m_componentsId.empty())
			{
				continue;
			}

			engine::field_serializer::serializeField(snapshotMessage->m_data, networkId->m_networkId);

			for (auto componentId : networkComponentIdBuffer->m_componentsId)
			{
				auto networkComponent = reinterpret_cast<NetworkComponent*>(_world->getComponentPtrById(entity, componentId));
				std::vector<uint8_t> data = NetworkComponentFactory::serialize(*networkComponent);

				if (!data.empty())
				{
					snapshotMessage->m_data.insert(snapshotMessage->m_data.end(), data.begin(), data.end());
				}
			}
		}

		for (auto& [entity, sendTcpMessageComponent] : _world->queryEntities(
			ecs::WithComponent<components::SendTcpMessageComponent>{}))
		{
			sendTcpMessageComponent->m_messagesToSend.push_back(snapshotMessage);
		}
	}
}