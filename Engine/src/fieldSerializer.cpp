#include "TRA/netcode/engine/fieldSerializer.hpp"

namespace tra::netcode::engine::field_serializer
{
    void serializeField(std::vector<uint8_t>& _data, const std::string& _field)
    {
        size_t size = _field.size();
        serializeField(_data, size);

        _data.insert(_data.end(), _field.begin(), _field.end());
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, std::string& _field)
    {
        uint32_t size = 0;
        deserializeField(_data, _offset, size);

        _field.assign(reinterpret_cast<const char*>(&_data[_offset]), size);
        _offset += size;
    }

    void serializeField(std::vector<uint8_t>& _data, const Spawn& _field)
    {
        serializeField(_data, _field.m_networkId);
        serializeField(_data, _field.m_ownerId);
        serializeField(_data, _field.m_tag);
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, Spawn& _field)
    {
        deserializeField(_data, _offset, _field.m_networkId);
        deserializeField(_data, _offset, _field.m_ownerId);
        deserializeField(_data, _offset, _field.m_tag);
    }

    void serializeField(std::vector<uint8_t>& _data, const Despawn& _field)
    {
        serializeField(_data, _field.m_networkId);
    }

    void deserializeField(const std::vector<uint8_t>& _data, size_t& _offset, Despawn& _field)
    {
        deserializeField(_data, _offset, _field.m_networkId);
    }
}
