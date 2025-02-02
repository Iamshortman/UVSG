#include "Components/ShipFlightControl.hpp"

#include "World/Entity.hpp"
#include "Util.hpp"
#include "Ship/Directions.hpp"
#include "Ship/ShipComponent.hpp"
#include "Input/InputManager.hpp"

#include <stdio.h>
#include <stdlib.h> 

#define M_PI 3.14159265358979323846

ShipFlightControl::ShipFlightControl()
{	
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
		printf("Error: No parent!!!\n");
		return;
	}

	bool flightAssist;
	bool throttleUp;
	bool throttleDown;
	bool leaveSeat;
	bool hyperSpeed;

	double throttle;
	double pitch;
	double yaw;
	double roll;

	if (tempSeat != nullptr && tempSeat->isOccupied())
	{
		//Get Inputs
		flightAssist = InputManager::Instance->getButtonDown("ship_flight_assist");
		throttleUp = InputManager::Instance->getButtonDown("ship_throttle_up");
		throttleDown = InputManager::Instance->getButtonDown("ship_throttle_down");
		leaveSeat = InputManager::Instance->getButtonPressed("ship_eject");
		hyperSpeed = InputManager::Instance->getButtonDown("ship_warp");

		throttle = InputManager::Instance->getAxis("ship_throttle");
		pitch = InputManager::Instance->getAxis("ship_pitch");
		yaw = InputManager::Instance->getAxis("ship_yaw");
		roll = InputManager::Instance->getAxis("ship_roll");
	}
	else
	{
		//Default Inputs
		flightAssist = false;
		throttleUp = false;
		throttleDown = false;
		leaveSeat = false;
		hyperSpeed = false;

		throttle = -1.0;
		pitch = 0.0;
		yaw = 0.0;
		roll = 0.0;
	}


	//Max Speed in meters per second
	double maxSpeed[] = { 15.0, 15.0, 250.0, 50.0, 15.0, 15.0 };

	//Acceleration in meters per second per second
	double accelerations[] = { 25.0, 25.0, 25.0, 25.0, 25.0, 25.0 };

	bool FlightAssistEnabled = true;

	Transform transform = parent->getTransform();
	quaternionD rotation = transform.getOrientation();
	vector3D linearVelocity = parent->getLinearVelocity();

	FlightAssistEnabled = true;

	if (flightAssist)
	{
		FlightAssistEnabled = false;
	}

	if (true)
	{
		double turnSpeedDesired = m_turnSpeedMax.x * pitch;
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
		double turnSpeedDesired = m_turnSpeedMax.y * yaw;
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
		double turnSpeedDesired = m_turnSpeedMax.z * roll;
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

	quaternionD rotation1 = glm::angleAxis(m_turnSpeedCurrent.x * (M_PI * 2.0) * deltaTime, transform.getLeft());
	rotation1 = glm::angleAxis(m_turnSpeedCurrent.y * (M_PI * 2.0) * deltaTime, transform.getUp()) * rotation1;
	rotation1 = glm::angleAxis(m_turnSpeedCurrent.z * (M_PI * 2.0) * deltaTime, transform.getForward()) * rotation1;

	transform.setOrientation(rotation1 * rotation);
	parent->setTransform(transform);

	parent->setAngularVelocity(Lerp(parent->getAngularVelocity(), vector3D(0.0), deltaTime * 3.0));

	//Forward Backward Movment
	if (true)
	{
		if (throttle)
		{
			m_throttle.z = throttle;
			m_throttle.z += 1.0;
			m_throttle.z /= 2.0;
		}

		if (throttleUp)
		{
			m_throttle.z += 0.5 * deltaTime;
			if (m_throttle.z > 1.0)
			{
				m_throttle.z = 1.0;
			}
		}
		else if (throttleDown)
		{
			m_throttle.z -= 0.5 * deltaTime;
			if (m_throttle.z < 0.0)
			{
				m_throttle.z = 0.0;
			}
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

				linearVelocity += transform.getUp() * diffrence;
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

				linearVelocity += -transform.getUp() * diffrence;
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

				linearVelocity += transform.getLeft() * diffrence;
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

				linearVelocity += -transform.getLeft() * diffrence;
			}
		}
	}

	//Update Velocity
	parent->setLinearVelocity(linearVelocity);

	if (leaveSeat)
	{
		if (parent->hasComponent("shipComponent"))
		{
			ShipComponent* component = (ShipComponent*)parent->getComponent("shipComponent");
			component->EjectOccupancy();
		}
		else if (tempSeat != nullptr)
		{
			if (tempSeat->isOccupied())
			{
				tempSeat->clearOccupancy();
			}
		}
	}
}