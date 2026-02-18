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

		TRA_API ecs::World* getEcsWorld();
		TRA_API ecs::Entity getSelfEntity();

		template<typename T>
		void addNetworkComponent(ecs::Entity _entity, T&& _component)
		{
			if constexpr (!std::is_base_of<NetworkComponent, T>::value)
			{
				TRA_WARNING_LOG("NetworkEngine: Attempted to add a component that is not a NetworkComponent.");
				return;
			}

			if (!entityHasNetworkComponentIdBuffer(_entity))
			{
				TRA_ERROR_LOG("NetworkEngine: Failed to get NetworkComponentIdBuffer for entity %I32u.", _entity.id());
				return;
			}

			if (m_ecsWorld->hasComponent<T>(_entity))
			{
				TRA_WARNING_LOG("NetworkEngine: Entity %I32u already has a component of type %s.", _entity.id(), typeid(T).name());
				return;
			}

			const size_t componentId = ecs::ComponentLibrary::getComponent<T>().m_id;
			addIdToNetworkComponentIdBuffer(_entity, componentId);

			m_ecsWorld->addComponent(_entity, std::forward<T>(_component));
		}

		template<typename T>
		void removeNetworkComponent(ecs::Entity _entity)
		{
			if constexpr (!std::is_base_of<NetworkComponent, T>::value)
			{
				TRA_WARNING_LOG("NetworkEngine: Attempted to remove a component that is not a NetworkComponent.");
				return;
			}

			if (!entityHasNetworkComponentIdBuffer(_entity))
			{
				TRA_ERROR_LOG("NetworkEngine: Failed to get NetworkComponentIdBuffer for entity %I32u.", _entity.id());
				return;
			}

			if (!m_ecsWorld->hasComponent<T>(_entity))
			{
				TRA_WARNING_LOG("NetworkEngine: Entity %I32u does not have a component of type %s.", _entity.id(), typeid(T).name());
				return;
			}

			const size_t componentId = ecs::ComponentLibrary::getComponent<T>().m_id;
			removeIdToNetworkComponentIdBuffer(_entity, componentId);

			m_ecsWorld->removeComponent<T>(_entity);
		}

	private:
		//core::UdpSocket* m_udpSocket;

		std::unique_ptr<ecs::World> m_ecsWorld;

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
