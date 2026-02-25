#ifndef TRA_NETCODE_ENGINE_INTERNAL_SNAPSHOT_MESSAGE_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_SNAPSHOT_MESSAGE_HPP

#include "TRA/netcode/engine/message.hpp"

using namespace tra::netcode::engine;

DECLARE_MESSAGE_BEGIN(SnapshotMessage)
FIELD(std::vector<uint8_t>, m_data)
DECLARE_MESSAGE_END()

#endif
