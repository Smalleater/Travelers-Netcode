#ifndef TRA_NETCODE_ENGINE_NETWORK_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_NETWORK_COMPONENT_HPP

#include "TRA/export.hpp"

#include "TRA/netcode/engine/serializable.hpp"

namespace tra::netcode::engine
{
	struct TRA_API NetworkComponent : Serializable
	{
		virtual std::string getType() const = 0;
	};

	namespace internal
	{
		TRA_API uint32_t hashTypeName(const char* _str);

		TRA_API void registerMessageType(const uint32_t _id,
			std::shared_ptr<NetworkComponent>(*_creator)(const std::vector<uint8_t>&));
	}
}

#endif
