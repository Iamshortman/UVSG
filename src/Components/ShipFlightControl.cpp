#include "Components/ShipFlightControl.hpp"
#include "World/Entity.hpp"
#include <stdio.h>
#include <stdlib.h> 
#include "Util.hpp"

#include "Ship/Directions.hpp"

ShipFlightControl::ShipFlightControl(SDL_GameController* controllerToUse)
{
	m_controller = controllerToUse;
	
	m_turnSpeedCurrent = vector3D(0.0);
	m_turnSpeedMax = vector3D(0.2, 0.15, 0.4);
	m_turnSpeedAcceleration = vector3D(0.4);

	double m_SpeedCurrent = 0;
	double m_SpeedAcceleration = 10.0;
}

ShipFlightControl::~ShipFlightControl()
{

}

vector3D Lerp(vector3D start, vector3D end, double percent)
{
	return (start + percent*(end - start));
}

void ShipFlightControl::update(double deltaTime)
{
	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("No parent!!!\n");
		exit(1);
	}

	//Max Speed in meters per second
	double maxSpeed[] = { 15.0, 15.0, 115.0, 15.0, 15.0, 15.0 };

	//Acceleration in meters per second per second
	double accelerations[] = { 15.0, 15.0, 15.0, 15.0, 15.0, 15.0 };

	bool FlightAssistEnabled = true;

	Transform transform = parent->getTransform();
	quaternionD rotation = parent->getOrientation();
	vector3D linearVelocity = parent->getLinearVelocity();

	FlightAssistEnabled = true;

	if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_A))
	{
		FlightAssistEnabled = false;
	}

	//Forward Backward Movment
	if (true)
	{
		if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
		{
			m_throttle.z = 1.0;
		}
		else if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
		{
			m_throttle.z = -1.0;
		}
		else
		{
			m_throttle.z = 0.0;
		}

		double currentForwardVelocity = glm::dot(transform.getForward(), linearVelocity);
		double desiredForwardVelocity = m_throttle.z * (m_throttle.z > 0 ? maxSpeed[FORWARD] : maxSpeed[BACKWARD]);
		double diffrence = abs(currentForwardVelocity - desiredForwardVelocity);

		if (currentForwardVelocity < desiredForwardVelocity)
		{
			if (FlightAssistEnabled || m_throttle.z > 0.0)
			{
				double velocitytoAdd = accelerations[FORWARD] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				linearVelocity += transform.getForward() * velocitytoAdd;
			}
		}
		else if (currentForwardVelocity > desiredForwardVelocity)
		{

			if (FlightAssistEnabled || m_throttle.z < 0.0)
			{
				double velocitytoAdd = accelerations[BACKWARD] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				linearVelocity += -transform.getForward() * velocitytoAdd;
			}
		}
	}

	//Up Down Movement
	if (true)
	{
		double currentUpVelocity = glm::dot(transform.getUp(), linearVelocity);
		double desiredUpVelocity = m_throttle.y * (m_throttle.y > 0 ? maxSpeed[UP] : maxSpeed[DOWN]);
		double diffrence = abs(currentUpVelocity - desiredUpVelocity);

		if (currentUpVelocity < desiredUpVelocity)
		{
			if (FlightAssistEnabled || m_throttle.y > 0.0)
			{
				double velocitytoAdd = accelerations[UP] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				linearVelocity += transform.getUp() * velocitytoAdd;
			}
		}
		else if (currentUpVelocity > desiredUpVelocity)
		{

			if (FlightAssistEnabled || m_throttle.y < 0.0)
			{
				double velocitytoAdd = accelerations[DOWN] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				linearVelocity += -transform.getUp() * velocitytoAdd;
			}
		}
	}

	//Left Right Movement
	if (true)
	{
		double currentLeftVelocity = glm::dot(transform.getLeft(), linearVelocity);
		double desiredLeftVelocity = m_throttle.x * (m_throttle.x > 0 ? maxSpeed[LEFT] : maxSpeed[RIGHT]);
		double diffrence = abs(currentLeftVelocity - desiredLeftVelocity);

		if (currentLeftVelocity < desiredLeftVelocity)
		{
			if (FlightAssistEnabled || m_throttle.x > 0.0)
			{
				double velocitytoAdd = accelerations[LEFT] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				linearVelocity += transform.getLeft() * velocitytoAdd;
			}
		}
		else if (currentLeftVelocity > desiredLeftVelocity)
		{

			if (FlightAssistEnabled || m_throttle.x < 0.0)
			{
				double velocitytoAdd = accelerations[RIGHT] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				linearVelocity += -transform.getLeft() * velocitytoAdd;
			}
		}
	}

	//Update Velocity
	parent->setLinearVelocity(linearVelocity);

	int deadzone = 8000;

	if (true)
	{
		double yawAmount = 0.0;
		int yawAxis = -SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			yawAmount = ((double)yawAxis) / 32767.0;
		}
		double turnSpeedDesired = m_turnSpeedMax.x * yawAmount;
		double turnSpeedCurrent = m_turnSpeedCurrent.x;
		if (turnSpeedCurrent < turnSpeedDesired)
		{
			turnSpeedCurrent += m_turnSpeedAcceleration.x * deltaTime;
			if (turnSpeedCurrent > turnSpeedDesired)
			{
				turnSpeedCurrent = turnSpeedDesired;
			}
		}
		else if (turnSpeedCurrent > turnSpeedDesired)
		{
			turnSpeedCurrent -= m_turnSpeedAcceleration.x * deltaTime;
			if (turnSpeedCurrent < turnSpeedDesired)
			{
				turnSpeedCurrent = turnSpeedDesired;
			}
		}
		m_turnSpeedCurrent.x = turnSpeedCurrent;
	}

	if (true)
	{
		double yawAmount = 0.0;
		int yawAxis = -SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			yawAmount = ((double)yawAxis) / 32767.0;
		}
		double turnSpeedDesired = m_turnSpeedMax.y * yawAmount;
		double turnSpeedCurrent = m_turnSpeedCurrent.y;
		if (turnSpeedCurrent < turnSpeedDesired)
		{
			turnSpeedCurrent += m_turnSpeedAcceleration.y * deltaTime;
			if (turnSpeedCurrent > turnSpeedDesired)
			{
				turnSpeedCurrent = turnSpeedDesired;
			}
		}
		else if (turnSpeedCurrent > turnSpeedDesired)
		{
			turnSpeedCurrent -= m_turnSpeedAcceleration.y * deltaTime;
			if (turnSpeedCurrent < turnSpeedDesired)
			{
				turnSpeedCurrent = turnSpeedDesired;
			}
		}
		m_turnSpeedCurrent.y = turnSpeedCurrent;
	}

	if (true)
	{
		double rollAmount = 0.0;
		int rollAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (rollAxis > deadzone || rollAxis < -deadzone)
		{
			//Get between -1 and 1
			rollAmount = ((double)rollAxis) / 32767.0;
		}
		double turnSpeedDesired = m_turnSpeedMax.z * rollAmount;
		double turnSpeedCurrent = m_turnSpeedCurrent.z;
		if (turnSpeedCurrent < turnSpeedDesired)
		{
			turnSpeedCurrent += m_turnSpeedAcceleration.z * deltaTime;
			if (turnSpeedCurrent > turnSpeedDesired)
			{
				turnSpeedCurrent = turnSpeedDesired;
			}
		}
		else if (turnSpeedCurrent > turnSpeedDesired)
		{
			turnSpeedCurrent -= m_turnSpeedAcceleration.z * deltaTime;
			if (turnSpeedCurrent < turnSpeedDesired)
			{
				turnSpeedCurrent = turnSpeedDesired;
			}
		}
		m_turnSpeedCurrent.z = turnSpeedCurrent;
	}

	rotation = glm::angleAxis(m_turnSpeedCurrent.x * (M_PI * 2.0) * deltaTime, transform.getLeft()) * rotation;
	rotation = glm::angleAxis(m_turnSpeedCurrent.y * (M_PI * 2.0) * deltaTime, transform.getUp()) * rotation;
	rotation = glm::angleAxis(m_turnSpeedCurrent.z * (M_PI * 2.0) * deltaTime, transform.getForward()) * rotation;

	parent->setOrientation(rotation);

	parent->setAngularVelocity(Lerp(parent->getAngularVelocity(), vector3D(0.0), deltaTime * 3.0));
}