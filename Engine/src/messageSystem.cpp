#include "internal/messageSystem.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"

#include "TRA/netcode/engine/tags.hpp"

#include "internal/socketComponent.hpp"
#include "internal/messageComponent.hpp"
#include "internal/tags.hpp"
#include "internal/messageSerializer.hpp"

#define TRA_MAX_TCP_MESSAGES_TO_RECEIVE_PAR_TICK 32

namespace tra::netcode::engine::internal
{
	void SendTcpMessageSystem::update(ecs::World* _world)
	{
		for (auto& [entity, socketPtr, sendMessagePtr] : _world->queryEntities(
			ecs::WithComponent<components::TcpConnectSocketComponent, components::SendTcpMessageComponent>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<engine::tags::ConnectedTag>{},
			ecs::WithoutTag<tags::PendingDisconnectTag>{}))
		{
			std::vector<uint8_t> serializedMessage;
			for (auto& message : sendMessagePtr->m_messagesToSend)
			{
				serializedMessage = MessageSerializer::serializePayload(*message.get());
				serializedMessage = MessageSerializer::serializeForNetwork(serializedMessage);

				sendMessagePtr->m_serializedToSend.push_back(serializedMessage);

				serializedMessage.clear();
			}

			sendMessagePtr->m_messagesToSend.clear();

			for (auto messageIt = sendMessagePtr->m_serializedToSend.begin(); messageIt != sendMessagePtr->m_serializedToSend.end();)
			{
				int byteSent = 0;
				auto sendDataResult = socketPtr->m_tcpSocket->sendData(messageIt->data() + sendMessagePtr->m_lastMessageByteSent,
					messageIt->size() - sendMessagePtr->m_lastMessageByteSent, byteSent);

				if (sendDataResult.first != ErrorCode::Success)
				{
					if (sendDataResult.first == ErrorCode::SocketSendPartial)
					{
						sendMessagePtr->m_lastMessageByteSent = byteSent;
						TRA_DEBUG_LOG("SendTcpMessageSystem::update: Partial data sent for entity %llu, BytesSent: %d/%llu",
							static_cast<unsigned long long>(entity.id()), byteSent, static_cast<unsigned long long>(messageIt->size()));
					}
					else 
					{
						if (sendDataResult.first != ErrorCode::SocketConnectionClosed)
						{
							TRA_ERROR_LOG("SendTcpMessageSystem::update: Failed to send data for entity %llu, ErrorCode: %d, Last socket error: %d",
								static_cast<unsigned long long>(entity.id()), static_cast<int>(sendDataResult.first), static_cast<int>(sendDataResult.second));
						}
						
						_world->addTag<tags::PendingDisconnectTag>(entity);
					}

					break;
				}

				messageIt = sendMessagePtr->m_serializedToSend.erase(messageIt);
			}
		}
	}

	void ReceiveTcpMessageSystem::update(ecs::World* _world)
	{
		for (auto& [entity, socketPtr, receiveMessagePtr] : _world->queryEntities(
			ecs::WithComponent<components::TcpConnectSocketComponent, components::ReceiveTcpMessageComponent>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<engine::tags::ConnectedTag>{},
			ecs::WithoutTag<tags::PendingDisconnectTag>{}))
		{
			receiveMessagePtr->m_receivedMessages.clear();

			std::vector<uint8_t> newReceivedBuffer;

			auto receiveDataResult = socketPtr->m_tcpSocket->receiveData(newReceivedBuffer);
			if (receiveDataResult.first != ErrorCode::Success && receiveDataResult.first != ErrorCode::SocketWouldBlock)
			{
				if (receiveDataResult.first != ErrorCode::SocketConnectionClosed)
				{
					TRA_ERROR_LOG("ReceiveTcpMessageSystem::update: Failed to receive data for entity %llu, ErrorCode: %d, Last socket error: %d",
						static_cast<unsigned long long>(entity.id()), static_cast<int>(receiveDataResult.first), static_cast<int>(receiveDataResult.second));
				}

				_world->addTag<tags::PendingDisconnectTag>(entity);

				continue;
			}

			receiveMessagePtr->m_receivedBuffer.insert(receiveMessagePtr->m_receivedBuffer.end(), newReceivedBuffer.begin(), newReceivedBuffer.end());
			
			std::vector<uint8_t> payload;
			size_t consumedBytes = 0;

			while (true)
			{
				if (!MessageSerializer::getPayloadFromNetworkBuffer(receiveMessagePtr->m_receivedBuffer, payload, consumedBytes))
				{
					break;
				}

				receiveMessagePtr->m_receivedBuffer.erase(receiveMessagePtr->m_receivedBuffer.begin(),
					receiveMessagePtr->m_receivedBuffer.begin() + static_cast<std::vector<uint8_t>::difference_type>(consumedBytes));

				std::shared_ptr<Message> newMessage = MessageSerializer::deserializePayload(payload);
				if (!newMessage)
				{
					TRA_ERROR_LOG("ReceiveTcpMessageSystem::update: Failed to deserialize message for entity %llu",
						static_cast<unsigned long long>(entity.id()));
					continue;
				}

				receiveMessagePtr->m_receivedMessages[newMessage->getType()].push_back(newMessage);
			}
		}
	}
}