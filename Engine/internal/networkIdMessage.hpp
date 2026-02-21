#ifndef TRA_NETCODE_ENGINE_INTERNAL_NETWORK_ID_MESSAGE_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_NETWORK_ID_MESSAGE_HPP

#include "TRA/netcode/engine/message.hpp"
#include "TRA/netcode/engine/networkId.hpp"

namespace tra::netcode::engine
{
	DECLARE_MESSAGE_BEGIN(UpdateNetworkIdMessage)
	FIELD(std::vector<NetworkId>, m_newNetworkIds)
	FIELD(std::vector<NetworkId>, m_removedNetworkIds)
	DECLARE_MESSAGE_END()
}

#endif
