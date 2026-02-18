#ifndef TRA_NETCODE_ENGINE_INTERNAL_NETWORK_COMPONENT_FACTORY_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_NETWORK_COMPONENT_FACTORY_HPP

#include "TRA/netcode/engine/networkComponent.hpp"

namespace tra::netcode::engine::internal
{
	class NetworkComponentFactory
	{
	public:
		using Creator = std::function<std::shared_ptr<NetworkComponent>(const std::vector<uint8_t>&)>;

		static void registerNetworkComponent(const uint32_t _id, Creator _creator);
		static std::vector<uint8_t> serialize(const NetworkComponent& _message);
		static std::shared_ptr<NetworkComponent> deserialize(const std::vector<uint8_t>& _payload);

	private:
		static std::unordered_map<uint32_t, Creator> m_registry;
	};
}

#endif
