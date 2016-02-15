#ifndef CHARACTERCONTROLLER_HPP
#define CHARACTERCONTROLLER_HPP

#include "EntityxInclude.hpp"
#include "glmInclude.hpp"
#include "Components.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/PhysicsWorld.hpp"

class CharacterController
{
public:
	bool m_onGround = false;
	bool m_isJumping = false;
	double m_walkSpeed, m_sprintSpeed, m_jumpSpeed;

	CharacterController(double walkSpeed, double sprintSpeed, double jumpSpeed)
	{
		m_walkSpeed = walkSpeed;
		m_sprintSpeed = sprintSpeed;
		m_jumpSpeed = jumpSpeed;
	};

	~CharacterController()
	{

	};
};

class CharacterControllerSystem : public System < CharacterControllerSystem >
{
	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		ComponentHandle<Transform> search1;
		ComponentHandle<Velocity> search2;
		ComponentHandle<RigidBody> search3;
		ComponentHandle<CharacterController> search4;

		for (Entity entity : es.entities_with_components(search1, search2, search3, search4))
		{
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
			ComponentHandle<Velocity> componentVelocity = entity.component<Velocity>();
			ComponentHandle<RigidBody> componentRigidBody = entity.component<RigidBody>();
			ComponentHandle<CharacterController> componentCharacterController = entity.component<CharacterController>();

			vector3D startPos = componentTransform->getPos();
			vector3D endPos = componentTransform->getPos() + (componentTransform->getUp() * -2.0);

			SingleRayTestResults result = componentRigidBody->getPhysicsWorld()->singleRayTest(startPos, endPos);

			if (result.hasHit)
			{
				vector3D diffrance = glm::abs(componentTransform->getPos() - result.hitPosition);
				double distance = glm::length(diffrance);

				//If the player is too close to the ground, adjust it up.
				if (distance < 1.0)
				{
					double adjustAmount = 1.0 - distance;
					componentTransform->setPos(componentTransform->getPos() + (componentTransform->getUp() * adjustAmount));

					if (componentVelocity->linearVelocity.y <= 0.0f)
					{
						componentVelocity->linearVelocity.y = 0.0;
					}
					componentCharacterController->m_onGround = true;

				}
				else
				{
					componentCharacterController->m_onGround = false;
				}

			}


		}
	}
};

#endif //CHARACTERCONTROLLER_HPP