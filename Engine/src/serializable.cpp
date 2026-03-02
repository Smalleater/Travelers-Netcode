#include "TRA/netcode/engine/serializable.hpp"

namespace tra::netcode::engine
{
    Serializable::SerializerMap& Serializable::getSerializers()
    {
        static SerializerMap serializers;
        return serializers;
    }

    Serializable::DeserializerMap& Serializable::getDeserializers()
    {
        static DeserializerMap deserializers;
        return deserializers;
    }
}