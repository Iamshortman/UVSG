#ifndef CHARACTERCONTROLLER_HPP
#define CHARACTERCONTROLLER_HPP

#include "EntityxInclude.hpp"
#include "GLM_Include.hpp"
#include "Components.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/PhysicsWorld.hpp"

class CharacterController
{
public:
	bool m_onGround = false;
	double timeInAir = 0.0f;
	double m_walkSpeed, m_sprintSpeed, m_jumpSpeed, m_airVelocityChange;
	vector3D m_fallVelocity;

	double tempCameraPitch = 0.0;

	CharacterController(double walkSpeed, double sprintSpeed, double jumpSpeed, double airSpeed)
	{
		m_walkSpeed = walkSpeed;
		m_sprintSpeed = sprintSpeed;
		m_jumpSpeed = jumpSpeed;
		m_airVelocityChange = airSpeed;
	};

	~CharacterController()
	{

	};
};

/*class CharacterControllerSystem : public System < CharacterControllerSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		SDL_GameController* controller = UVSG::getInstance()->controller;

		ComponentHandle<Transform> search1;
		ComponentHandle<Velocity> search2;
		ComponentHandle<RigidBody> search3;
		ComponentHandle<CharacterController> search4;

		for (Entity entity : es.entities_with_components(search1, search2, search3, search4))
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();
			ComponentHandle<CharacterController> componentCharacter = entity.component<CharacterController>();

			vector3D startPos = componentTransform->getPosition();
			vector3D endPos = componentTransform->getPosition() + (componentTransform->getUp() * -2.0);

			SingleRayTestResult result = componentRigidBody->getPhysicsWorld()->singleRayTestNotMe(startPos, endPos, componentRigidBody->rigidBody);

			if (result.hitBody == componentRigidBody->rigidBody)
			{
				printf("Crap!!!!");
				exit(1);
			}

			componentCharacter->m_onGround = false;
			if (result.hasHit && result.hitBody != componentRigidBody->rigidBody)
			{
				vector3D diffrance = glm::abs(componentTransform->getPosition() - result.hitPosition);
				double distance = glm::length(diffrance);

				//If the player is too close to the ground, adjust it up.
				if (distance <= 1.5)
				{
					double adjustAmount = 1.5 - distance;

					componentTransform->setPosition(componentTransform->getPosition() + (componentTransform->getUp() * adjustAmount));
					componentVelocity->linearVelocity.y = 0.0;
					componentCharacter->m_onGround = true;
					componentCharacter->timeInAir = 0.0;
				}
			}

			if (!componentCharacter->m_onGround)
			{
				componentCharacter->timeInAir += dt;

				//Update velocity due to gravity, terminal velocity is 55 m/s
				componentVelocity->linearVelocity.y += -9.8 * dt;
				if (componentVelocity->linearVelocity.y < -55.0)
				{
					componentVelocity->linearVelocity.y = -55.0;
				}
			}

			if (componentCharacter->m_onGround)
			{
				Uint8 shouldJump = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
				if (shouldJump == 1)
				{
					componentVelocity->linearVelocity.y += componentCharacter->m_jumpSpeed;
				}
			}

			int deadzone = 8000;
			int forwardAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
			int strafeAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			double forwardVelocity = 0.0;
			double strafeVelocity = 0.0;

			if (forwardAxis > deadzone || forwardAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = -((double)forwardAxis) / 32767.0f;
				forwardVelocity = amount;
			}

			if (strafeAxis > deadzone || strafeAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)strafeAxis) / 32767.0f;
				strafeVelocity = amount;
			}

			vector3D planeFoward = componentTransform->getForward();
			planeFoward.y = 0.0;//Clear vertical movement;
			planeFoward = glm::normalize(planeFoward); //Make unit vector

			vector3D planeRight = componentTransform->getRight();
			planeRight.y = 0.0;//Clear vertical movement;
			planeRight = glm::normalize(planeRight); //Make unit vector

			vector3D forwardMovement = planeFoward * forwardVelocity;
			vector3D strafeMovement = planeRight * strafeVelocity;
			vector3D combinedMovement = forwardMovement + strafeMovement;
			
			if(componentCharacter->m_onGround)
			{
				combinedMovement = combinedMovement * componentCharacter->m_walkSpeed;

				//Applys the velocity in the correct direction on the horizontal plane
				componentVelocity->linearVelocity.x = combinedMovement.x;
				componentVelocity->linearVelocity.z = combinedMovement.z;
			}
			else
			{
				//In the air we only apply a small about of velocity change per second.
				combinedMovement = combinedMovement * componentCharacter->m_airVelocityChange;

				//Scale the velocity change the time that has passed.
				combinedMovement *= dt;
				componentVelocity->linearVelocity.x += combinedMovement.x;
				componentVelocity->linearVelocity.z += combinedMovement.z;
			}

			//Rotation
			int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

			if (yawAxis > deadzone || yawAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)yawAxis) / 32767.0f;
				double angle = amount * dt * 3.0f;

				quaternionD yawQuat = glm::normalize(glm::angleAxis(-angle, vector3D(0.0, 1.0, 0.0)));

				componentTransform->m_orientation = yawQuat * componentTransform->m_orientation;
			}

			int ptichAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

			if (ptichAxis > deadzone || ptichAxis < -deadzone)
			{
				//Get between -1 and 1
				double amount = ((double)ptichAxis) / 32767.0f;
				componentCharacter->tempCameraPitch += amount * dt * -3.0f;
			}

		}
	}
};*/

#endif //CHARACTERCONTROLLER_HPP