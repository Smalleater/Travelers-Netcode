#include "internal/acceptConnectionSystem.hpp"

#include "TRA/debugUtils.hpp"

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"

#include "TRA/netcode/engine/tags.hpp"

#include "internal/socketComponent.hpp"
#include "internal/messageComponent.hpp"
#include "internal/networkComponentIdBuffer.hpp"

#define MAX_ACCEPTED_CONNECTIONS_PAR_TICK 32

namespace tra::netcode::engine::internal
{
	void AcceptConnectionSystem::update(ecs::World* _world)
	{
		uint8_t acceptedConnections = 0;

		for (auto& [entity] : _world->queryEntities(
			ecs::WithComponent<>{},
			ecs::WithoutComponent<>{},
			ecs::WithTag<tags::NewConnectionTag>{}))
		{
			_world->removeTag<tags::NewConnectionTag>(entity);
		}

		std::vector<core::TcpSocket*> newSockets;

		core::TcpSocket* newSocket = nullptr;
		for (auto& [entity, socketPtr] : _world->queryEntities(
			ecs::WithComponent<components::TcpListenSocketComponent>{}))
		{
			acceptedConnections = 0;
			while (acceptedConnections < MAX_ACCEPTED_CONNECTIONS_PAR_TICK)
			{
				newSocket = nullptr;

				auto acceptResult = socketPtr->m_tcpSocket->acceptSocket(&newSocket);
				if (acceptResult.first == ErrorCode::SocketWouldBlock)
				{
					break;
				}
				else if (acceptResult.first != ErrorCode::Success)
				{
					TRA_ERROR_LOG("AcceptConnectionSystem::update: Failed to accept new connection on entity %I32u, ErrorCode: %d, Last socket error: %d",
						entity.id(), static_cast<int>(acceptResult.first), static_cast<int>(acceptResult.second));
					break;
				}

				auto setBlockingResult = newSocket->setBlocking(false);
				if (setBlockingResult.first != ErrorCode::Success)
				{
					TRA_ERROR_LOG("AcceptConnectionSystem::update: Failed to set non-blocking mode on accepted socket. ErrorCode: %d, Last socket error: %d",
						static_cast<int>(setBlockingResult.first), static_cast<int>(setBlockingResult.second));
					newSocket->closeSocket();
					delete newSocket;
					continue;
				}

				newSockets.push_back(newSocket);

				++acceptedConnections;
			}
		}

		for (auto socket : newSockets)
		{
			ecs::Entity entity = _world->createEntity();

			_world->addTag<tags::NewConnectionTag>(entity);
			_world->addTag<tags::ConnectedTag>(entity);

			_world->addComponent(entity, components::TcpConnectSocketComponent{ socket });
			_world->addComponent(entity, components::ReceiveTcpMessageComponent{});
			_world->addComponent(entity, components::SendTcpMessageComponent{});
			_world->addComponent(entity, components::NetworkComponentIdBuffer{});

			TRA_INFO_LOG("NetworkEngine: Accepted new TCP connection. Entity ID: %I32u", entity.id());
		}
	}
}