#include "Components/PlayerControl.hpp"
#include "World/Entity.hpp"
#include "World/World.hpp"
#include "Ship/ShipComponent.hpp"

PlayerControl::PlayerControl(double linear, double angular, SDL_GameController* controllerToUse)
{
	linearSpeed = linear;
	angularSpeed = angular;
	m_controller = controllerToUse;
	printf("Using Controller: %s \n", SDL_GameControllerName(m_controller));
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

	int pitchAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTY);
	if (pitchAxis > deadzone || pitchAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)pitchAxis) / 32767.0;

		rotation = glm::angleAxis(amount * angularSpeed * (M_PI * 2.0) * deltaTime, transform.getLeft()) * rotation;
	}

	int yawAxis = -SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTX);
	if (yawAxis > deadzone || yawAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)yawAxis) / 32767.0;

		rotation = glm::angleAxis(amount * angularSpeed * (M_PI * 2.0) * deltaTime, vector3D(0, 1, 0)) * rotation;
	}

	transform.setOrientation(rotation);

	double linear = this->linearSpeed;

	if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
	{
		linear *= 100000.0;
	}

	int forwardAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTY);

	if (forwardAxis > deadzone || forwardAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)forwardAxis) / 32767.0;
		double distance = amount * deltaTime * linear;
		transform.setPosition(transform.getPosition() + (transform.getForward() * -distance));
	}

	int strafeAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTX);

	if (strafeAxis > deadzone || strafeAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)strafeAxis) / 32767.0;
		double distance = amount * deltaTime * linear;
		transform.setPosition(transform.getPosition() + (transform.getRight() * distance));
	}

	parent->setTransform(transform);

	static int lastButton = 0;
	int button = SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_A);
	if (button && !lastButton)
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

				printVec((vector3D) cellPos);
				printEndLine();

				//Sets parent to ride the ship
				entity->riddenByEntity = parent;
				parent->ridingEntity = entity;
			}
		}
	}
	lastButton = button;
}