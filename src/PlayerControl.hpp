#ifndef PLAYERCONTROL_HPP
#define PLAYERCONTROL_HPP

#include <entityx/entityx.h>
#include <entityx\System.h>
#include <entityxInclude.hpp>
#include <VoxelComponent.hpp>

#include "Util.hpp"

struct PlayerControlComponent
{
	PlayerControlComponent(float linear, float angular)
	{
		linearSpeed = linear;
		angularSpeed = angular;
	};

	float linearSpeed;

	//rad per second
	float angularSpeed;
};

class PlayerControlSystem : public System < PlayerControlSystem >
{

	void push3(vector<unsigned int>* vector, unsigned int a, unsigned int b, unsigned int c)
	{
		vector->push_back(a - 1);
		vector->push_back(b - 1);
		vector->push_back(c - 1);
	}

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override
	{
		float timestep = ((float)dt);
		SDL_GameController* controller = UVSG::getInstance()->controller;
		SDL_Joystick* joystick = UVSG::getInstance()->joystick;

		//For all entities with the Player Control and Tranform components.
		ComponentHandle<PlayerControlComponent> playerControlComponentSearch;
		ComponentHandle<Transform> transformComponentSearch;
		for (Entity entity : es.entities_with_components(playerControlComponentSearch, transformComponentSearch))
		{
			//Get the component
			ComponentHandle<PlayerControlComponent> playerControlComponent = entity.component<PlayerControlComponent>();
			ComponentHandle<Transform> componentTransform = entity.component<Transform>();
		
			int deadzone = 8000;

			int pitchAxis = -SDL_JoystickGetAxis(joystick, 1);

			if (pitchAxis > deadzone || pitchAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)pitchAxis) / 32767.0f;
				float angle = amount * timestep * playerControlComponent->angularSpeed;

				//Negitive angle because the stick is backwards
				quaternion pitchQuat = glm::normalize(glm::angleAxis( -angle, componentTransform->getRight() ));

				componentTransform->orientation = pitchQuat * componentTransform->orientation;
			}

			int yawAxis = SDL_JoystickGetAxis(joystick, 2);

			if (yawAxis > deadzone || yawAxis < -deadzone)
			{
				//Get between -1 and 1
				float amount = ((float)yawAxis) / 32767.0f;
				float angle = amount * timestep * playerControlComponent->angularSpeed;

				quaternion yawQuat = glm::normalize(glm::angleAxis( -angle, vector3(0.0f, 1.0f, 0.0f) ));

				componentTransform->orientation = yawQuat * componentTransform->orientation;
			}


			int hatSwitch= SDL_JoystickGetHat(joystick, 0);

			if (hatSwitch == SDL_HAT_UP || hatSwitch == SDL_HAT_DOWN)
			{
				//Get between -1 and 1
				float amount = (hatSwitch == SDL_HAT_UP) ? 1.0f : -1.0f;
				float distance = amount * timestep * playerControlComponent->linearSpeed;

				componentTransform->position += componentTransform->getForward() * distance;
			}

			if (hatSwitch == SDL_HAT_RIGHT || hatSwitch == SDL_HAT_LEFT)
			{
				//Get between -1 and 1
				float amount = (hatSwitch == SDL_HAT_LEFT) ? 1.0f : -1.0f;
				float distance = amount * timestep * playerControlComponent->linearSpeed;

				componentTransform->position += componentTransform->getRight() * distance;
			}

			int Button = SDL_JoystickGetButton(joystick, 0);
			static int lastButton = 0;

			if (Button)
			{
				vector3 start = componentTransform->getPos() + componentTransform->getForward();
				vector3 direction = componentTransform->getForward();
				float distance = 1000.0f;

				PhysxWorld* physxWorld = UVSG::getInstance()->physxWorld;
				raycastSingleResult result = physxWorld->raycastSingle(start, direction, distance);

				//If it hits an object and the object has an assositated entity.
				if (result.hasHit && result.hasHitEntity)
				{
					if (result.hitEntity.has_component<VoxelComponent>() && result.hitEntity.has_component<Transform>())
					{
						ComponentHandle<VoxelComponent> componentVoxel = result.hitEntity.component<VoxelComponent>();
						ComponentHandle<Transform> componentVoxelTransform = result.hitEntity.component<Transform>();

						matrix4 inverseModelMatrix = glm::inverse(componentVoxelTransform->getModleMatrix());

						std::printf("Vec3F: {%f, %f, %f}\n", result.worldHitPos.x, result.worldHitPos.y, result.worldHitPos.z);

						//Get the hit position in local space
						vector4 worldHitPoint = vector4(result.worldHitPos + (componentTransform->getForward() * 0.2f), 1.0f);
						vector3 localHitPoint = vector3(inverseModelMatrix * worldHitPoint);
						localHitPoint /= componentVoxel->getCubeSize();

						std::printf("Vec3F_: {%f, %f, %f}\n", localHitPoint.x, localHitPoint.y, localHitPoint.z);

						int x = (int)(localHitPoint.x + 0.5f);
						int y = (int)(localHitPoint.y + 0.5f);
						int z = (int)(localHitPoint.z + 0.5f);
						std::printf("Vec3: {%i, %i, %i}\n", x, y, z);

						if (componentVoxel->getBlock(x, y, z) != 0)
						{
							componentVoxel->setBlock(x, y, z, 0);
						}
					}
				}

				
			}
			lastButton = Button;

			static int lastShot = 0;
			int shoot = SDL_JoystickGetButton(joystick, 1);

			if (shoot && !lastShot)
			{
				Entity entity1 = es.create();
				entity1.assign<MeshComponent>();
				entity1.assign<Transform>();
				entity1.assign<Velocity>();

				PhysxWorld* physxWorld = UVSG::getInstance()->physxWorld;
				RenderingManager* renderingManager = UVSG::getInstance()->renderingManager;

				entity1.component<Transform>()->position = renderingManager->camera.getForward() + renderingManager->camera.getPos();
				entity1.component<Velocity>()->linearVelocity = renderingManager->camera.getForward() * 50.0f;

				//2-Creating dynamic cube
				PxMaterial* material = physxWorld->gPhysicsSDK->createMaterial(0.5, 0.5, 0.0);
				physx::PxBoxGeometry* boxGeometry = new physx::PxBoxGeometry(PxVec3(1, 1, 1)); //Defining geometry for box actor
				entity1.assign<RigidBodyPx>(physxWorld, entity, boxGeometry, material, 0.1f);

				entity1.assign<TimeToLiveComponent>(10.0f);

				vector<vector3> vertices = vector<vector3>();
				vertices.push_back(vector3(1.0f, -1.0f, -1.0f));
				vertices.push_back(vector3(1.0f, -1.0f, 1.0f));
				vertices.push_back(vector3(-1.0f, -1.0f, 1.0f));
				vertices.push_back(vector3(-1.0f, -1.0f, -1.0f));
				vertices.push_back(vector3(1.0f, 1.0f, -1.0f));
				vertices.push_back(vector3(1.0f, 1.0f, 1.0f));
				vertices.push_back(vector3(-1.0f, 1.0f, 1.0f));
				vertices.push_back(vector3(-1.0f, 1.0f, -1.0f));


				vector<vector3> colors = vector<vector3>();
				colors.push_back(vector3(1.0F, 1.0F, 0.0F));
				colors.push_back(vector3(0.0F, 1.0F, 1.0F));
				colors.push_back(vector3(1.0F, 0.0F, 1.0F));
				colors.push_back(vector3(0.0F, 1.0F, 1.0F));
				colors.push_back(vector3(1.0F, 1.0F, 0.0F));
				colors.push_back(vector3(0.0F, 1.0F, 1.0F));
				colors.push_back(vector3(1.0F, 0.0F, 1.0F));
				colors.push_back(vector3(0.0F, 1.0F, 1.0F));

				vector<vector3> normals = vector<vector3>();

				vector<unsigned int> indices = vector<unsigned int>();

				push3(&indices, 5, 8, 6);//TOP
				push3(&indices, 8, 7, 6);
				normals.push_back(vector3(0.0F, 1.0F, 0.0F));
				normals.push_back(vector3(0.0F, 1.0F, 0.0F));
				normals.push_back(vector3(0.0F, 1.0F, 0.0F));

				push3(&indices, 1, 2, 4);//Bottom
				push3(&indices, 2, 3, 4);
				normals.push_back(vector3(0.0F, -1.0F, 0.0F));
				normals.push_back(vector3(0.0F, -1.0F, 0.0F));
				normals.push_back(vector3(0.0F, -1.0F, 0.0F));

				push3(&indices, 2, 6, 3);//Back
				push3(&indices, 6, 7, 3);
				normals.push_back(vector3(0.0F, 0.0F, -1.0F));
				normals.push_back(vector3(0.0F, 0.0F, -1.0F));
				normals.push_back(vector3(0.0F, 0.0F, -1.0F));

				push3(&indices, 1, 4, 8);//Front
				push3(&indices, 5, 1, 8);
				normals.push_back(vector3(0.0F, 0.0F, 1.0F));
				normals.push_back(vector3(0.0F, 0.0F, 1.0F));
				normals.push_back(vector3(0.0F, 0.0F, 1.0F));

				push3(&indices, 1, 5, 2);//Right
				push3(&indices, 5, 6, 2);
				normals.push_back(vector3(1.0F, 0.0F, 0.0F));
				normals.push_back(vector3(1.0F, 0.0F, 0.0F));
				normals.push_back(vector3(1.0F, 0.0F, 0.0F));

				push3(&indices, 3, 7, 4);//Left
				push3(&indices, 7, 8, 4);
				normals.push_back(vector3(-1.0F, 0.0F, 0.0F));
				normals.push_back(vector3(-1.0F, 0.0F, 0.0F));
				normals.push_back(vector3(-1.0F, 0.0F, 0.0F));

				ComponentHandle<MeshComponent> componentMesh1 = entity1.component<MeshComponent>();
				componentMesh1->mesh.addVertices(vertices, colors, indices);
			}
			lastShot = shoot;
		}
	};
};

#endif //PLAYERCONTROL_HPP