#ifndef TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_COMPONENT_HPP

#include <vector>
#include <cstdint>
#include <unordered_map>

#include "TRA/ecs/component.hpp"

namespace tra::netcode::engine
{
	struct Message;
}

namespace tra::netcode::engine::internal::components
{
	TRA_ECS_REGISTER_COMPONENT(SendTcpMessageComponent,
		std::vector<std::shared_ptr<Message>> m_messagesToSend;
		std::vector<std::vector<uint8_t>> m_serializedToSend;
		int m_lastMessageByteSent = 0;
	)

	TRA_ECS_REGISTER_COMPONENT(ReceiveTcpMessageComponent,
		std::unordered_map<std::string, std::vector<std::shared_ptr<Message>>> m_receivedMessages;
		std::vector<uint8_t> m_receivedBuffer;
	)
}

#endif
