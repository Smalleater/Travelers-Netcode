#include "internal/snapshotSystem.hpp"

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/engine/fieldSerializer.hpp"

#include "internal/snapshotMessage.hpp"
#include "internal/networkIdComponent.hpp"
#include "internal/networkComponentIdBuffer.hpp"
#include "internal/networkComponentFactory.hpp"
#include "internal/messageComponent.hpp"
#include "internal/socketComponent.hpp"
#include "internal/snapshotComponent.hpp"
#include "internal/messageSerializer.hpp"

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
			engine::field_serializer::serializeField(snapshotMessage->m_data, networkComponentIdBuffer->m_componentsId.size());

			for (auto componentId : networkComponentIdBuffer->m_componentsId)
			{
				auto networkComponent = reinterpret_cast<NetworkComponent*>(_world->getComponentPtrById(entity, componentId));
				std::vector<uint8_t> data = NetworkComponentFactory::serialize(*networkComponent);
				data = MessageSerializer::serializeForNetwork(data);

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

	void ReceiveSnapshotSystem::update(ecs::World* _world)
	{
		if (!_world->queryEntities(ecs::WithComponent<components::TcpListenSocketComponent>{}).empty())
		{
			return;
		}

		for (auto& [entity, tcpConnectSocket, receiveTcpMessage, snapshot] : _world->queryEntities(
			ecs::WithComponent<components::TcpConnectSocketComponent, internal::components::ReceiveTcpMessageComponent, 
				components::SnapshotComponent>{}))
		{
			auto it = receiveTcpMessage->m_receivedMessages.find("SnapshotMessage");
			if (it == receiveTcpMessage->m_receivedMessages.end())
			{
				continue;
			}

			auto snapshotMessages = it->second;
			if (snapshotMessages.empty())
			{
				continue;
			}

			auto snapshotBuffer = static_cast<message::SnapshotMessage*>(snapshotMessages.at(0).get())->m_data;

			std::vector<uint8_t> payload;
			size_t consumedBytes = 0;

			while (true)
			{
				if (snapshotBuffer.empty())
				{
					break;
				}
				
				NetworkId networkId = 0;
				
				consumedBytes = 0;
				engine::field_serializer::deserializeField(snapshotBuffer, consumedBytes, networkId);
				snapshotBuffer.erase(snapshotBuffer.begin(), snapshotBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

				size_t componentCount = 0;

				consumedBytes = 0;
				engine::field_serializer::deserializeField(snapshotBuffer, consumedBytes, componentCount);
				snapshotBuffer.erase(snapshotBuffer.begin(), snapshotBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

				for (size_t i = 0; i < componentCount; i++)
				{
					if (!MessageSerializer::getPayloadFromNetworkBuffer(snapshotBuffer, payload, consumedBytes))
					{
						break;
					}

					snapshotBuffer.erase(snapshotBuffer.begin(), snapshotBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

					std::shared_ptr<NetworkComponent> newMessage = NetworkComponentFactory::deserialize(payload);
					if (!newMessage)
					{
						TRA_ERROR_LOG("ReceiveSnapshotSystem::update: Failed to deserialize snapshot message for entity %llu",
							static_cast<unsigned long long>(entity.id()));
						continue;
					}

					if (snapshot->m_lastSnapshotId != 0)
					{
						snapshot->m_firstSnapshot[networkId][newMessage->getType()] = newMessage;
					}
					else
					{
						snapshot->m_secondSnapshot[networkId][newMessage->getType()] = newMessage;
					}
				}
			}

			snapshot->m_lastSnapshotId = snapshot->m_lastSnapshotId == 0 ? 1 : 0;

			if (snapshotMessages.size() > 1)
			{
				auto snapshotBuffer = static_cast<message::SnapshotMessage*>(snapshotMessages.at(1).get())->m_data;

				std::vector<uint8_t> payload;
				size_t consumedBytes = 0;

				while (true)
				{
					if (snapshotBuffer.empty())
					{
						break;
					}

					NetworkId networkId = 0;

					consumedBytes = 0;
					engine::field_serializer::deserializeField(snapshotBuffer, consumedBytes, networkId);
					snapshotBuffer.erase(snapshotBuffer.begin(), snapshotBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

					size_t componentCount = 0;

					consumedBytes = 0;
					engine::field_serializer::deserializeField(snapshotBuffer, consumedBytes, componentCount);
					snapshotBuffer.erase(snapshotBuffer.begin(), snapshotBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

					for (size_t i = 0; i < componentCount; i++)
					{
						if (!MessageSerializer::getPayloadFromNetworkBuffer(snapshotBuffer, payload, consumedBytes))
						{
							break;
						}

						snapshotBuffer.erase(snapshotBuffer.begin(), snapshotBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

						std::shared_ptr<NetworkComponent> newMessage = NetworkComponentFactory::deserialize(payload);
						if (!newMessage)
						{
							TRA_ERROR_LOG("ReceiveSnapshotSystem::update: Failed to deserialize snapshot message for entity %llu",
								static_cast<unsigned long long>(entity.id()));
							continue;
						}

						if (snapshot->m_lastSnapshotId != 0)
						{
							snapshot->m_firstSnapshot[networkId][newMessage->getType()] = newMessage;
						}
						else
						{
							snapshot->m_secondSnapshot[networkId][newMessage->getType()] = newMessage;
						}
					}
				}
			}
		}
	}
}