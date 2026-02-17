#ifndef TRA_NETCODE_ENGINE_NETWORK_COMPONENT_HPP
#define TRA_NETCODE_ENGINE_NETWORK_COMPONENT_HPP

#include "TRA/export.hpp"

#include <memory>

#include "TRA/ecs/componentRegister.hpp"

#include "TRA/netcode/engine/serializable.hpp"

namespace tra::netcode::engine
{
	struct TRA_API NetworkComponent : Serializable
	{
		virtual std::string getType() const = 0;

		static uint32_t hashTypeName(const char* _str);
		static void registerNetworkComponentType(const uint32_t _id, 
            std::shared_ptr<NetworkComponent>(*_creator)(const std::vector<uint8_t>&));
	};
}

#define TRA_NETCODE_DECLARE_NETWORK_COMPONENT_BEGIN(NAME) \
namespace tra::networkComponent { \
    struct NAME : NetworkComponent \
    { \
    public: \
        static constexpr const char* COMPONENT_NAME = #NAME; \
        inline static uint32_t COMPONENT_TYPE_ID = NetworkComponent::hashTypeName(COMPONENT_NAME); \
        using COMPONENT_TYPE = NAME;

#define TRA_NETCODE_DECLARE__NETWORK_COMPONENT_FIELD(TYPE, NAME) \
    public: \
        TYPE NAME; \
    private: \
        struct NAME##_Registrar \
        { \
            NAME##_Registrar() \
            { \
                const auto offset = reinterpret_cast<size_t>(&(static_cast<COMPONENT_TYPE*>(nullptr)->NAME)); \
                Serializable::registerSerializer<TYPE>(COMPONENT_TYPE_ID, #NAME, offset); \
                Serializable::registerDeserializer<TYPE>(COMPONENT_TYPE_ID, #NAME, offset); \
            } \
        }; \
        inline static NAME##_Registrar NAME##_reg; \

#define TRA_NETCODE_DECLARE_NETWORK_COMPONENT_END() \
    public: \
        std::string getType() const override { return COMPONENT_NAME; } \
        std::vector<uint8_t> serialize() const override \
        { \
            std::vector<uint8_t> data; \
            uint32_t typeId = COMPONENT_TYPE_ID; \
            data.insert(data.end(), \
                reinterpret_cast<const uint8_t*>(&typeId), \
                reinterpret_cast<const uint8_t*>(&typeId) + sizeof(typeId)); \
            auto& serializers = getSerializers(); \
            auto it = serializers.find(COMPONENT_TYPE_ID); \
            if (it != serializers.end()) \
            { \
                for (const auto& [fieldName, fieldData] : it->second) \
                { \
                    fieldData.second(this, data); \
                } \
            } \
            return data; \
        } \
        static std::shared_ptr<NetworkComponent> createFromBytes(const std::vector<uint8_t>& _payload) \
        { \
            std::shared_ptr<COMPONENT_TYPE> component = std::make_shared<COMPONENT_TYPE>(); \
            size_t offset = sizeof(uint32_t); \
            auto& deserializers = getDeserializers(); \
            auto it = deserializers.find(COMPONENT_TYPE_ID); \
            if (it != deserializers.end()) \
            { \
                for (const auto& [fieldName, fieldData] : it->second) \
                { \
                    fieldData.second(component.get(), _payload, offset); \
                } \
            } \
            return component; \
        } \
    private: \
        struct Register \
        { \
            Register() { NetworkComponent::registerNetworkComponentType(COMPONENT_TYPE_ID, COMPONENT_TYPE::createFromBytes); } \
        }; \
        inline static Register _register{}; \
        static inline tra::ecs::ComponentRegister<COMPONENT_TYPE> m_registrar{}; \
    }; \
}

#endif
