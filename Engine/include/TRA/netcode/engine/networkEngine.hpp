#ifndef TRA_NETCODE_ENGINE_NETWORK_ENGINE_HPP
#define TRA_NETCODE_ENGINE_NETWORK_ENGINE_HPP

#include "TRA/export.hpp"
#include "TRA/errorCode.hpp"

#include <cstdint>

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"
#include "TRA/netcode/core/udpSocket.hpp"

namespace tra::netcode::engine
{
	struct Message;

	class NetworkEngine
	{
	public:
		TRA_API NetworkEngine();
		TRA_API ~NetworkEngine();

		TRA_API ErrorCode startTcpListenOnPort(uint16_t _port, bool _blocking);
		TRA_API ErrorCode startTcpConnectToAddress(const std::string& _address, uint16_t _port, bool _blocking);
		//TRA_API ErrorCode startUdpOnPort(uint16_t _port, bool _blocking);

		TRA_API ErrorCode stopTcpListen();
		TRA_API ErrorCode stopTcpConnect();
		TRA_API ErrorCode stopUdp();

		TRA_API void beginUpdate();
		TRA_API void endUpdate();

		TRA_API ErrorCode sendTcpMessage(ecs::Entity _entityId, std::shared_ptr<Message> _message);
		TRA_API std::vector<std::shared_ptr<Message>> getTcpMessages(ecs::Entity _entity, const std::string& _messageType);

		TRA_API EntityId getSelfEntityId();

		template<typename ComponentType>
		ErrorCode addComponentToEntity(EntityId _entityId, std::shared_ptr<ComponentType> _component)
		{
			return m_networkEcs->addComponentToEntity(_entityId, _component);
		}

		template<typename ComponentType>
		bool entityHasComponent(EntityId _entityId)
		{
			return m_networkEcs->hasComponent<ComponentType>(_entityId);
		}

		template<typename ...ComponentType>
		std::vector<EntityId> queryEntityIds()
		{
			return m_networkEcs->queryIds<ComponentType...>();
		}

		template<typename ...ComponentType>
		std::vector<std::tuple<EntityId, std::shared_ptr<ComponentType>...>> queryEntity()
		{
			return m_networkEcs->query<ComponentType...>();
		}

	private:
		//core::UdpSocket* m_udpSocket;

		std::unique_ptr<ecs::World> m_ecsWorld;

		ecs::Entity m_selfEntity;
	};
}

#endif
