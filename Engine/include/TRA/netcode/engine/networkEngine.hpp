#ifndef TRA_NETCODE_ENGINE_NETWORK_ENGINE_HPP
#define TRA_NETCODE_ENGINE_NETWORK_ENGINE_HPP

#include "TRA/export.hpp"
#include "TRA/errorCode.hpp"

#include <cstdint>
#include <chrono>
#include <type_traits>

#include "TRA/ecs/world.hpp"

#include "TRA/netcode/core/tcpSocket.hpp"
#include "TRA/netcode/core/udpSocket.hpp"

#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/networkComponent.hpp"
#include "TRA/netcode/engine/networkIdManager.hpp"

namespace tra::netcode::engine
{
	class NetworkEngine
	{
	public:
		TRA_API NetworkEngine();
		TRA_API ~NetworkEngine();

		TRA_API ErrorCode startTcpListenOnPort(uint16_t _port, bool _blocking);
		TRA_API ErrorCode startTcpConnectToAddress(const std::string& _address, uint16_t _port, bool _blocking);
		//TRA_API ErrorCode startUdpOnPort(uint16_t _port, bool _blocking);

		TRA_API void stopTcpListen();
		TRA_API void stopTcpConnect();
		//TRA_API ErrorCode stopUdp();

		TRA_API void setTickRate(uint8_t _tickRate);

		TRA_API uint8_t getTickRate();
		TRA_API uint32_t getCurrentTick();
		TRA_API float getFixedDeltaTime();

		TRA_API void resetElapsedTime();

		TRA_API bool canUpdateNetcode();

		TRA_API void updateElapsedTime();
		
		TRA_API void beginUpdate();
		TRA_API void endUpdate();

		TRA_API ErrorCode sendTcpMessage(ecs::Entity _entity, std::shared_ptr<Message> _message);
		TRA_API std::vector<std::shared_ptr<Message>> getTcpMessages(ecs::Entity _entity, const std::string& _messageType);

		TRA_API std::shared_ptr<NetworkComponent> getNetworkComponentFromSnapshot(
			const NetworkId _networkId, const std::string& _componentType, bool _lastSnapshot);

		TRA_API ecs::World* getEcsWorld();
		TRA_API ecs::Entity getSelfEntity();

		TRA_API NetworkId createNetworkEntity();
		TRA_API bool destroyNetworkEntity(NetworkId _networkId);

		template<typename T>
		void addNetworkComponent(NetworkId _networkId, T&& _component)
		{
			if constexpr (!std::is_base_of<NetworkComponent, T>::value)
			{
				TRA_WARNING_LOG("NetworkEngine: Attempted to add a component that is not a NetworkComponent.");
				return;
			}

			ecs::Entity entity = m_networkIdManager.getEntity(_networkId);
			if (entity.isNull())
			{
				TRA_ERROR_LOG("NetworkEngine: Attempted to add a component to an invalid NetworkId: %I32u.", _networkId);
				return;
			}

			if (!entityHasNetworkComponentIdBuffer(entity))
			{
				TRA_ERROR_LOG("NetworkEngine: Failed to get NetworkComponentIdBuffer for entity %I32u.", entity.id());
				return;
			}

			if (m_ecsWorld->hasComponent<T>(entity))
			{
				TRA_WARNING_LOG("NetworkEngine: Entity %I32u already has a component of type %s.", entity.id(), typeid(T).name());
				return;
			}

			const size_t componentId = ecs::ComponentLibrary::getComponent<T>().m_id;
			addIdToNetworkComponentIdBuffer(entity, componentId);

			m_ecsWorld->addComponent(entity, std::forward<T>(_component));
		}

		template<typename T>
		void removeNetworkComponent(NetworkId _networkId)
		{
			if constexpr (!std::is_base_of<NetworkComponent, T>::value)
			{
				TRA_WARNING_LOG("NetworkEngine: Attempted to remove a component that is not a NetworkComponent.");
				return;
			}

			ecs::Entity entity = m_networkIdManager.getEntity(_networkId);
			if (entity.isNull())
			{
				TRA_ERROR_LOG("NetworkEngine: Attempted to remove a component to an invalid NetworkId: %I32u.", _networkId);
				return;
			}

			if (!entityHasNetworkComponentIdBuffer(entity))
			{
				TRA_ERROR_LOG("NetworkEngine: Failed to get NetworkComponentIdBuffer for entity %I32u.", entity.id());
				return;
			}

			if (!m_ecsWorld->hasComponent<T>(entity))
			{
				TRA_WARNING_LOG("NetworkEngine: Entity %I32u does not have a component of type %s.", entity.id(), typeid(T).name());
				return;
			}

			const size_t componentId = ecs::ComponentLibrary::getComponent<T>().m_id;
			removeIdToNetworkComponentIdBuffer(entity, componentId);

			m_ecsWorld->removeComponent<T>(entity);
		}

	private:
		//core::UdpSocket* m_udpSocket;

		std::unique_ptr<ecs::World> m_ecsWorld;

		NetworkIdManager m_networkIdManager;

		ecs::Entity m_selfEntity;

		uint32_t m_currentTick;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_lastElapsedTimeUpdate;

		float m_fixedDeltaTime;
		float m_elapsedTime;

		uint8_t m_tickRate;
		std::chrono::high_resolution_clock m_clock;

		TRA_API bool entityHasNetworkComponentIdBuffer(ecs::Entity _entity);
		bool networkComponentIdBufferHasId(ecs::Entity _entity, size_t _componentId);

		TRA_API void addIdToNetworkComponentIdBuffer(ecs::Entity _entity, size_t _componentId);
		TRA_API void removeIdToNetworkComponentIdBuffer(ecs::Entity _entity, size_t _componentId);
	};
}

#endif
