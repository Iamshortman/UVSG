#include "Components/PlayerControl.hpp"
#include "World/Entity.hpp"
#include "World/EntityWorld.hpp"
#include "World/World.hpp"
#include "Ship/ShipComponent.hpp"
#include "Input/InputManager.hpp"

PlayerControl::PlayerControl(double linear, double angular)
{
	linearSpeed = linear;
	angularSpeed = angular;
};

void PlayerControl::update(double deltaTime)
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("No parent!!!\n");
		exit(1);
	}

	if (parent->ridingEntity != nullptr)
	{ 
		//No movement if is riding entity
		return;
	}

	Transform transform = parent->getTransform();
	quaternionD rotation = transform.getOrientation();

	int deadzone = 8000;

	if (InputManager::Instance->hasAxis("player_pitch"))
	{
		rotation = glm::angleAxis(InputManager::Instance->getAxis("player_pitch") * angularSpeed * (M_PI * 2.0) * deltaTime, transform.getLeft()) * rotation;
	}

	if (InputManager::Instance->hasAxis("player_yaw"))
	{
		rotation = glm::angleAxis(InputManager::Instance->getAxis("player_yaw") * angularSpeed * (M_PI * 2.0) * deltaTime, transform.getUp()) * rotation;
	}

	transform.setOrientation(rotation);

	double linear = this->linearSpeed;

	if (InputManager::Instance->getButtonDown("player_fast"))
	{
		linear *= 100.0;
	}

	if (InputManager::Instance->hasAxis("player_forward"))
	{
		double distance = InputManager::Instance->getAxis("player_forward") * deltaTime * linear;
		transform.setPosition(transform.getPosition() + (transform.getForward() * -distance));
	}

	if (InputManager::Instance->hasAxis("player_strafe"))
	{
		double distance = InputManager::Instance->getAxis("player_strafe") * deltaTime * linear;
		transform.setPosition(transform.getPosition() + (transform.getLeft() * -distance));
	}

	parent->setTransform(transform);

	if (InputManager::Instance->getButtonPressed("player_click"))
	{
		double rayDistance = 1000.0f;
		vector3D startPos = transform.getPosition();
		vector3D endPos = transform.getPosition() + (transform.getForward() * rayDistance);
		SingleRayTestResult result = parent->getWorld()->m_physicsWorld->singleRayTest(startPos, endPos);

		if (result.hasHit)
		{
			Entity* entity = result.hitEntity;

			if (entity->hasComponent("shipComponent"))
			{
				ShipComponent* component = (ShipComponent*)entity->getComponent("shipComponent");
				vector3B cellPos = getUnpackedPos(result.userValue);
				ShipCellData data = component->getCell(cellPos);

				//If the cell has a cockpit, update the position of the occupying entities
				if (component->m_seatMap.find(cellPos) != component->m_seatMap.end())
				{
					printVec((vector3D)cellPos);
					printEndLine();

					//Sets parent to ride the ship
					parent->ridingEntity = entity;
					component->m_seatMap[cellPos][0].m_occupyingEntity = parent;
				}

			}
			else if (entity->hasComponent("BigShip"))
			{
				EntityWorld* entityWorld = (EntityWorld*)entity;

				parent->addToWorld(entityWorld);
				parent->setTransform(Transform());

				//entityWorld->setAngularVelocity(vector3D(0.0, 0.0, 1.0));
			}
		}
	}
}