#include "TRA/netcode/engine/serializable.hpp"

namespace tra::netcode::engine
{
    SerializerMap& Serializable::getSerializers()
    {
        static SerializerMap serializers;
        return serializers;
    }

    DeserializerMap& Serializable::getDeserializers()
    {
        static DeserializerMap deserializers;
        return deserializers;
    }
}