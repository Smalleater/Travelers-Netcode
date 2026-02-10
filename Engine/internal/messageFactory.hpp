#ifndef TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_FACTORY_HPP
#define TRA_NETCODE_ENGINE_INTERNAL_MESSAGE_FACTORY_HPP

#include <functional>
#include <memory>
#include <cstdint>

#include "TRA/netcode/engine/message.hpp"

#include "internal/messageHeader.hpp"

namespace tra::netcode::engine
{
    class MessageFactory
    {
    public:
        using Creator = std::function<std::shared_ptr<Message>(const std::vector<uint8_t>&)>;

        static void registerMessage(const uint32_t _id, Creator _creator);
        static std::vector<uint8_t> serialize(const Message& _message);
        static std::shared_ptr<Message> deserialize(const std::vector<uint8_t>& _payload);

    private:
        static std::unordered_map<uint32_t, Creator> m_registry;
    };
}

#endif