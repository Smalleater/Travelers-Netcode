#ifndef TRA_NETCODE_ENGINE_MESSAGE_HPP
#define TRA_NETCODE_ENGINE_MESSAGE_HPP

#include "TRA/export.hpp"
#include "TRA/debugUtils.hpp"

#include <string>
#include <map>
#include <vector>
#include <variant>
#include <memory>
#include <cstdint>
#include <functional>
#include <algorithm>

#include "TRA/netcode/engine/serializable.hpp"

namespace tra::netcode::engine
{
	struct TRA_API Message : Serializable
	{
	public:
        virtual std::string getType() const = 0;

        static uint32_t hashTypeName(const char* _str);
        static void registerMessageType(const uint32_t _id, std::shared_ptr<Message>(*_creator)(const std::vector<uint8_t>&));
	};
}

#define DECLARE_MESSAGE_BEGIN(MessageType) \
namespace tra::message { \
    struct MessageType : public Message \
    { \
    public: \
        static constexpr const char* MESSAGE_TYPE_NAME = #MessageType; \
        inline static uint32_t MESSAGE_TYPE_ID = Message::hashTypeName(MESSAGE_TYPE_NAME); \
        using CurrentMessageType = MessageType;

#define FIELD(type, name) \
    type name; \
    private: \
        struct name##_Registrar \
        { \
            name##_Registrar() \
            { \
                const auto offset = reinterpret_cast<size_t>(&(static_cast<CurrentMessageType*>(nullptr)->name)); \
                Serializable::registerSerializer<type>(MESSAGE_TYPE_ID, #name, offset); \
                Serializable::registerDeserializer<type>(MESSAGE_TYPE_ID, #name, offset); \
            } \
        }; \
        inline static name##_Registrar name##_reg; \
    public: \

#define DECLARE_MESSAGE_END() \
        std::string getType() const override { return MESSAGE_TYPE_NAME; } \
        std::vector<uint8_t> serialize() const override \
        { \
            std::vector<uint8_t> data; \
            uint32_t typeId = MESSAGE_TYPE_ID; \
            data.insert(data.end(), \
                reinterpret_cast<const uint8_t*>(&typeId), \
                reinterpret_cast<const uint8_t*>(&typeId) + sizeof(typeId)); \
            auto& serializers = getSerializers(); \
            auto it = serializers.find(MESSAGE_TYPE_ID); \
            if (it != serializers.end()) \
            { \
                for (const auto& [fieldName, fieldData] : it->second) \
                { \
                    fieldData.second(this, data); \
                } \
            } \
            return data; \
        } \
        static std::shared_ptr<Message> createFromBytes(const std::vector<uint8_t>& _payload) \
        { \
            std::shared_ptr<CurrentMessageType> message = std::make_shared<CurrentMessageType>(); \
            size_t offset = sizeof(uint32_t); \
            auto& deserializers = getDeserializers(); \
            auto it = deserializers.find(MESSAGE_TYPE_ID); \
            if (it != deserializers.end()) \
            { \
                for (const auto& [fieldName, fieldData] : it->second) \
                { \
                    fieldData.second(message.get(), _payload, offset); \
                } \
            } \
            return message; \
        } \
    private: \
        struct Register \
        { \
            Register() { Message::registerMessageType(MESSAGE_TYPE_ID, CurrentMessageType::createFromBytes); } \
        }; \
        inline static Register _register{}; \
    }; \
}

#endif