#ifndef TRA_NETCODE_ENGINE_I_SERIALIZABLE_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_I_SERIALIZABLE_COMPONENT_HPP

#include "TRA/netcode/engine/INetworkComponent.hpp"

#include <vector>
#include <cstdint>

namespace tra::netcode::engine
{
	struct ISerializableComponent : public INetworkComponent
	{
		virtual std::vector<uint8_t> serialize() const = 0;
		virtual void deserialize(std::vector<uint8_t> _buffer) = 0;
	};
}

#endif
