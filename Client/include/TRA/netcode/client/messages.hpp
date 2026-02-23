#ifndef TRA_NETCODE_CLIENT_MESSAGES_HPP
#define TRA_NETCODE_CLIENT_MESSAGES_HPP

#include "TRA/netcode/engine/message.hpp"

using namespace tra::netcode::engine;

DECLARE_MESSAGE_BEGIN(InitializeClientMessage)
FIELD(int, m_tickRate)
FIELD(uint32_t, m_clientId)
DECLARE_MESSAGE_END()

DECLARE_MESSAGE_BEGIN(ClientIsReadyMessage)
DECLARE_MESSAGE_END()

DECLARE_MESSAGE_BEGIN(SpawnDespawnMessage)
FIELD(std::vector<Spawn>, m_spawns)
FIELD(std::vector<Despawn>, m_despawns)
DECLARE_MESSAGE_END()

#endif
