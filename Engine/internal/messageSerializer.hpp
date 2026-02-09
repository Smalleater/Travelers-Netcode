#ifndef TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_SERIALIZER_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_SERIALIZER_HPP

#include "internal/messageFactory.hpp"

namespace tra::netcode::engine
{
    class MessageSerializer
    {
    public:
        static std::vector<uint8_t> serializePayload(const Message& _message);
        static std::unique_ptr<Message> deserializePayload(const std::vector<uint8_t>& _payload);
        static std::vector<uint8_t> serializeForNetwork(const std::vector<uint8_t>& _payload, bool _internal = false);
		static bool getPayloadFromNetworkBuffer(const std::vector<uint8_t>& _buffer, std::vector<uint8_t>& _outPayload, size_t& _outConsumedBytes);
    };    
}

#endif