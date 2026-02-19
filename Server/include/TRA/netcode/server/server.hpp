#ifndef TRA_NETCODE_SERVER_SERVER_HPP
#define TRA_NETCODE_SERVER_SERVER_HPP

#include "TRA/export.hpp"

#include "TRA/errorCode.hpp"

#include <cstdint>
#include <unordered_map>
#include <chrono>

#include "TRA/netcode/engine/networkEngine.hpp"

#include "TRA/netcode/server/clientEntityRegistry.hpp"

namespace tra::netcode::server
{
	using EntityId = uint32_t;

	class Server
	{
	public:
		Server(Server& other) = delete;
		void operator=(const Server&) = delete;

		TRA_API static Server* Get();

		TRA_API ErrorCode start(uint16_t _port, uint8_t _tickRate = 20);
		TRA_API ErrorCode stop();

		TRA_API bool isRunning() const;

		TRA_API uint32_t getCurrentTick();
		TRA_API float getFixedDeltaTime();
		TRA_API std::vector<ClientId> getNewClientIds();
		TRA_API std::vector<ClientId> getDisconnectedClientIds();
		
		TRA_API bool canUpdateNetcode();

		TRA_API void updateElapsedTime();

		TRA_API void beginUpdate();
		TRA_API void endUpdate();

		TRA_API ecs::World* getEcsWorld();

		TRA_API bool sendTcpMessage(const ClientId _clientId, std::shared_ptr<engine::Message> _message);
		TRA_API std::vector<std::shared_ptr<engine::Message>> getTcpMessages(const ClientId _clientId, const std::string& _messageType);

		template<typename T>
		void addNetworkComponent(ecs::Entity _entity, T&& _component)
		{
			m_networkEngine->addNetworkComponent(_entity, std::forward<T>(_component));
		}

		template<typename T>
		void removeNetworkComponent(ecs::Entity _entity)
		{
			m_networkEngine->removeNetworkComponent<T>(_entity);
		}

	private:
		static Server* m_singleton;

		std::unique_ptr<engine::NetworkEngine> m_networkEngine;

		ClientId m_nextClientId;
		ClientEntityRegistry m_clientEntityRegistry;
		std::vector<ClientId> m_newClientIds;
		std::vector<ClientId> m_disconnectedClientIds;

		Server() = default;
		~Server();

		void disconnectedClient();
		void initializeNewClient();
		void setClientReady();
	};
}

#endif