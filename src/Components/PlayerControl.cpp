#include "Components/PlayerControl.hpp"
#include "World/Entity.hpp"
#include "World/World.hpp"

void PlayerControl::update(double deltaTime)
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("No parent!!!\n");
		exit(1);
	}

	int deadzone = 8000;

	int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY); //SDL_JoystickGetAxis(joystick, 5);

	if (pitchAxis > deadzone || pitchAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)pitchAxis) / 32767.0f;
		double angle = amount * deltaTime * this->angularSpeed;

		//Negitive angle because the joystick layout is backwards
		quaternionD pitchQuat = glm::normalize(glm::angleAxis(-angle, parent->getTransform().getRight()));

		parent->setOrientation(pitchQuat * parent->getTransform().getOrientation());
	}

	int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

	if (yawAxis > deadzone || yawAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)yawAxis) / 32767.0f;
		double angle = amount * deltaTime * this->angularSpeed;

		quaternionD yawQuat = glm::normalize(glm::angleAxis(-angle, vector3D(0.0f, 1.0f, 0.0f)));

		parent->setOrientation(yawQuat * parent->getTransform().getOrientation());
	}


	int forwardAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY); //SDL_JoystickGetAxis(joystick, 1);

	if (forwardAxis > deadzone || forwardAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)forwardAxis) / 32767.0f;
		double distance = amount * deltaTime * this->linearSpeed;
		parent->setPosition(parent->getPosition() + (parent->getTransform().getForward() * -distance));
	}

	int strafeAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

	if (strafeAxis > deadzone || strafeAxis < -deadzone)
	{
		//Get between -1 and 1
		double amount = ((double)strafeAxis) / 32767.0f;
		double distance = amount * deltaTime * this->linearSpeed;
		parent->setPosition(parent->getPosition() + (parent->getTransform().getRight() * distance));
	}

	static int lastButton = 0;

	int button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
	if (button && !lastButton)
	{
		double rayDistance = 1000.0f;
		vector3D startPos = parent->getTransform().getPos();
		vector3D endPos = parent->getTransform().getPos() + (parent->getTransform().getForward() * rayDistance);
		SingleRayTestResult result = parent->getWorld()->m_physicsWorld->singleRayTest(startPos, endPos);

		if (result.hasHit)
		{
			printf("Hit\n");
		}

	}
	lastButton = button;

}