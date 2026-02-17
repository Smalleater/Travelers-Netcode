#include "TRA/netcode/engine/networkComponent.hpp"

#include "internal/networkComponentFactory.hpp"

namespace tra::netcode::engine
{
	uint32_t NetworkComponent::hashTypeName(const char* _str)
	{
		uint32_t hash = 2166136261u;
		while (*_str)
		{
			hash ^= static_cast<uint32_t>(*_str++);
			hash *= 16777619u;
		}

		return hash;
	}

	void NetworkComponent::registerNetworkComponentType(const uint32_t _id,
		std::shared_ptr<NetworkComponent>(*_creator)(const std::vector<uint8_t>&))
	{
		NetworkComponentFactory::registerNetworkComponent(_id, _creator);
	}
}