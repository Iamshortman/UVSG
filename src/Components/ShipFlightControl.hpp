#ifndef SHIPFLIGHTCONTROL_HPP
#define SHIPFLIGHTCONTROL_HPP

#include "Components/Component.hpp"
#include "GLM_Include.hpp"

class ShipFlightControl : public Component
{
public:
	ShipFlightControl();
	virtual ~ShipFlightControl();

	virtual void update(double deltaTime);

private:
	//Flight Data
	vector3D m_throttle;

	vector3D m_turnSpeedCurrent;

	//In Rotations Per Second
	vector3D m_turnSpeedMax;
	//In Rotations Per Second/Per Second
	vector3D m_turnSpeedAcceleration;
	vector3D m_turnSpeedDeceleration;

	double m_SpeedCurrent;
	double m_SpeedAcceleration;
};

#endif //SHIPFLIGHTCONTROL_HPP