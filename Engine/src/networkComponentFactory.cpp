#include "internal/networkComponentFactory.hpp"

namespace tra::netcode::engine::internal
{
	std::unordered_map<uint32_t, NetworkComponentFactory::Creator> NetworkComponentFactory::m_registry;

    void NetworkComponentFactory::registerNetworkComponent(const uint32_t _id, Creator _creator)
    {
        m_registry[_id] = std::move(_creator);
    }

    std::vector<uint8_t> NetworkComponentFactory::serialize(const NetworkComponent& _component)
    {
        return _component.serialize();
    }

    std::shared_ptr<NetworkComponent> NetworkComponentFactory::deserialize(const std::vector<uint8_t>& _buffer)
    {
        if (_buffer.size() < sizeof(uint32_t))
        {
            throw std::runtime_error("Buffer too small");
        }

        uint32_t typeId;
        std::memcpy(&typeId, _buffer.data(), sizeof(uint32_t));

        std::unordered_map<uint32_t, Creator>::iterator it = m_registry.find(typeId);
        if (it == m_registry.end())
        {
            throw std::runtime_error("Unknown message type: " + std::to_string(typeId));
        }

        return it->second(_buffer);
    }
}