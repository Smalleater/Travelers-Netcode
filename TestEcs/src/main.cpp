#include <thread>
#include <chrono>

#include "TRA/ecs/engine.hpp"

constexpr size_t ENTITY_COUNT = 1000000;

using namespace tra;

std::vector<ecs::Entity> entities(ENTITY_COUNT);

struct TestComponent0 : ecs::IComponent
{
	uint16_t test = 2006;
	std::string string = "Hello";
};

struct TestComponent1 : ecs::IComponent
{
	std::string string = "HelloWorld";
};

struct TestComponent2 : ecs::IComponent
{
	uint64_t test0 = 2006;
	uint64_t test1 = 2006;
	uint64_t test2 = 2006;
	uint64_t test3 = 2006;
	uint64_t test4 = 2006;
	uint64_t test5 = 2006;
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
	TestComponent0 testComponent;

	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			_engine->addComponentToEntity<TestComponent0>(entities[i], testComponent);
		}
	}
};

struct GetTestComponentSystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		TestComponent0* testComponent;
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			testComponent = nullptr;
			if (_engine->entityHasComponent<TestComponent0>(entities[i]))
			{
				testComponent = _engine->getEntityComponent<TestComponent0>(entities[i]);
				if (testComponent)
				{
					++testComponent->test;
					--testComponent->test;
				}
			}
		}
	}
};

struct QuerryWithTestSystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		std::vector<ecs::Entity> queryResult;
		queryResult = _engine->queryEntityWith<TestComponent0>(entities);
		queryResult = _engine->queryEntityWith<TestComponent1>(entities);
		queryResult = _engine->queryEntityWith<TestComponent0, TestComponent2>(entities);
	}
};

struct QuerryWithoutTestSystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		std::vector<ecs::Entity> queryResult;
		queryResult = _engine->queryEntityWithout<TestComponent0>(entities);
		queryResult = _engine->queryEntityWithout<TestComponent1>(entities);
		queryResult = _engine->queryEntityWithout<TestComponent0, TestComponent2>(entities);
	}
};

struct QuerryEntityBufferTestSystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		std::vector<ecs::Entity> queryResult;
		queryResult = _engine->queryEntity()
			->queryEntityWith<TestComponent0>()
			->getEntities();

		queryResult = _engine->queryEntity()
			->queryEntityWith<TestComponent0>()
			->queryEntityWithout<TestComponent1>()
			->getEntities();

		queryResult = _engine->queryEntity()
			->queryEntityWith<TestComponent0, TestComponent2>()
			->queryEntityWithout<TestComponent1>()
			->getEntities();
	}
};


struct RemoveTestComponentSystem : public ecs::ISystem
{
	void update(ecs::Engine* _engine) override
	{
		for (size_t i = 0; i < ENTITY_COUNT; i++)
		{
			_engine->removeComponentFromEntity<TestComponent0>(entities[i]);
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

	for (size_t i = 0; i < ENTITY_COUNT; i++)
	{
		entities[i] = ecsEngine.createEntity();
	}

	TestComponent0 testComponent0;
	TestComponent1 testComponent1;
	TestComponent2 testComponent2;
	for (size_t i = 0; i < ENTITY_COUNT; i++)
	{
		int result = i % 3;
		if (result == 0)
		{
			ecsEngine.addComponentToEntity<TestComponent0>(entities[i], testComponent0);
		}
		else if (result == 1)
		{
			ecsEngine.addComponentToEntity<TestComponent1>(entities[i], testComponent1);
		}
		else if (result == 2)
		{
			ecsEngine.addComponentToEntity<TestComponent0>(entities[i], testComponent0);
			ecsEngine.addComponentToEntity<TestComponent2>(entities[i], testComponent2);
		}
	}

	//ecsEngine.addBeginUpdateSystem<CreateEntitySystem>();
	//ecsEngine.addBeginUpdateSystem<AddTestComponentSystem>();
	//ecsEngine.addBeginUpdateSystem<GetTestComponentSystem>();
	//ecsEngine.addBeginUpdateSystem<QuerryWithTestSystem>();
	//ecsEngine.addBeginUpdateSystem<QuerryWithoutTestSystem>();
	ecsEngine.addBeginUpdateSystem<QuerryEntityBufferTestSystem>();

	//ecsEngine.addEndUpdateSystem<RemoveTestComponentSystem>();
	//ecsEngine.addEndUpdateSystem<DeleteEntitySystem>();

	std::cout << "End Init\n";

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