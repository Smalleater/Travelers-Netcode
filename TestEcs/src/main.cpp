#include <thread>
#include <chrono>

#include "TRA/ecs/engine.hpp"

constexpr size_t ENTITY_COUNT = 100000;

using namespace tra;

std::vector<ecs::Entity> entities(ENTITY_COUNT);

struct TestComponent : ecs::IComponent
{
	uint16_t test = 2006;
	std::string string = "Hello";
};

struct CreateEntitySystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			entities[i] = _engine->createEntity();
		}
	}
};

struct AddTestComponentSystem : public ecs::ISystem
{
	TestComponent testComponent;

	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			_engine->addComponentToEntity<TestComponent>(entities[i], testComponent);
		}
	}
};

struct RemoveTestComponentSystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			_engine->removeComponentFromEntity<TestComponent>(entities[i]);
		}
	}
};

struct DeleteEntitySystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			_engine->deleteEntity(entities[i]);
		}
	}
};

int main() 
{
	std::cout << "Create ECS\n";
	ecs::Engine ecsEngine;

	ecsEngine.addBeginUpdateSystem<CreateEntitySystem>();

	ecsEngine.addBeginUpdateSystem<AddTestComponentSystem>();
	ecsEngine.addEndUpdateSystem<RemoveTestComponentSystem>();

	ecsEngine.addEndUpdateSystem<DeleteEntitySystem>();
	
	while (true)
	{
		auto start = std::chrono::high_resolution_clock::now();

		ecsEngine.beginUpdate();

		ecsEngine.endUpdate();

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		std::cout << "Update duration: " << duration << " microseconds\n";
	}

	return 0;
}