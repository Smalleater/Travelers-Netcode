#include "TRA/netcode/engine/message.hpp"

#include "internal/messageFactory.hpp"

namespace tra::netcode::engine
{
    std::map<uint32_t, std::vector<std::pair<std::string, std::pair<size_t, SerializerFunc>>>>& Message::getSerializers()
    {
        static std::map<uint32_t, std::vector<std::pair<std::string, std::pair<size_t, SerializerFunc>>>> serializers;
        return serializers;
    }

    std::map<uint32_t, std::vector<std::pair<std::string, std::pair<size_t, DeserializerFunc>>>>& Message::getDeserializers()
    {
        static std::map<uint32_t, std::vector<std::pair<std::string, std::pair<size_t, DeserializerFunc>>>> deserializers;
        return deserializers;
    }

    namespace internal
    {
        uint32_t hashTypeName(const char* _str)
        {
            uint32_t hash = 2166136261u;
            while (* _str)
            {
                hash ^= static_cast<uint32_t>(*_str++);
                hash *= 16777619u;
            }
            
            return hash;
        }

        void registerMessageType(const uint32_t _id, std::shared_ptr<Message>(*_creator)(const std::vector<uint8_t>&))
        {
            MessageFactory::registerMessage(_id, _creator);
        }
    }
}
