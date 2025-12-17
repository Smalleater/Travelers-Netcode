#include <thread>
#include <chrono>

#include "TRA/ecs/engine.hpp"

using namespace tra;

std::vector<ecs::Entity> entities(2000000);

struct CreateEntitySystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < 2000000; i++)
		{
			entities[i] = _engine->createEntity();
		}
	}
};

struct DeleteEntitySystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < 2000000; i++)
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