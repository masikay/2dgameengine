#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Logger/Logger.h"
#include <tuple>

//------------------------------------------
// C++ functions to bind with Lua functions
//------------------------------------------
std::tuple<double, double> GetEntityPosition(Entity entity)
{
	if (entity.HasComponent<TransformComponent>())
	{
		const auto transform = entity.GetComponent<TransformComponent>();
		return std::make_tuple(transform.position.x, transform.position.y);
	}
	else
	{
		Logger::Err("Trying to get the position of an entity that has no transform component.");
		return std::make_tuple(0.0, 0.0);
	}
}

std::tuple<double, double> GetEntityVelocity(Entity entity)
{
	if (entity.HasComponent<RigidBodyComponent>())
	{
		const auto rigidBody = entity.GetComponent<RigidBodyComponent>();
		return std::make_tuple(rigidBody.velocity.x, rigidBody.velocity.y);
	}
	else
	{
		Logger::Err("Trying to get the velocity of an entity that has no rigid-body component.");
		return std::make_tuple(0.0, 0.0);
	}
}

void SetEntityPosition(Entity entity, double x, double y)
{
	if (entity.HasComponent<TransformComponent>())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.position.x = x;
		transform.position.y = y;
	}
	else
	{
		Logger::Err("Trying to set the position of an entity that has no transform component.");
	}
}

void SetEntityVelocity(Entity entity, double x, double y)
{
	if (entity.HasComponent<RigidBodyComponent>())
	{
		auto& rigidBody = entity.GetComponent<RigidBodyComponent>();
		rigidBody.velocity.x = x;
		rigidBody.velocity.y = y;
	}
	else
	{
		Logger::Err("Trying to set the velocity of an entity that has no rigid-body component.");
	}
}

void SetEntityRotation(Entity entity, double angle)
{
	if (entity.HasComponent<TransformComponent>())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		transform.rotation = angle;
	}
	else
	{
		Logger::Err("Trying to set the rotation of an entity that has no transform component.");
	}
}

void SetEntityAnimationFrame(Entity entity, int frame)
{
	if (entity.HasComponent<AnimationComponent>())
	{
		auto& animation = entity.GetComponent<AnimationComponent>();
		animation.currentFrame = frame;
	}
	else
	{
		Logger::Err("Trying to set the animation frame of an entity that has no animation component.");
	}
}

void SetProjectileVelocity(Entity entity, double x, double y)
{
	if (entity.HasComponent<ProjectileEmitterComponent>())
	{
		auto& projectileEmitter = entity.GetComponent<ProjectileEmitterComponent>();
		projectileEmitter.projectileVelocity.x = x;
		projectileEmitter.projectileVelocity.y = y;
	}
	else
	{
		Logger::Err("Trying to set the projectile velocity of an entity that has no projectile emitter component.");
	}
}

class ScriptSystem : public System
{
public:
	ScriptSystem()
	{
		RequireComponent<ScriptComponent>();
	}

	void CreateLuaBinding(sol::state& lua)
	{
		// Create the "entity" user type for Lua
		lua.new_usertype<Entity>(
			"entity",
			"get_id", &Entity::GetId,
			"destroy", &Entity::Kill,
			"has_tag", &Entity::HasTag,
			"belongs_to_group", &Entity::BelongsToGroup
		);

		// Create the bindings between C++ and Lua
		lua.set_function("get_position", GetEntityPosition);
		lua.set_function("get_velocity", GetEntityVelocity);
		lua.set_function("set_position", SetEntityPosition);
		lua.set_function("set_velocity", SetEntityVelocity);
		lua.set_function("set_rotation", SetEntityRotation);
		lua.set_function("set_animation_frame", SetEntityAnimationFrame);
		lua.set_function("set_projectile_velocity", SetProjectileVelocity);
	}

	void Update(double deltaTime, int elapsedTime)
	{
		// Loop all the netities that have a script component and invoke their Lua function
		for (auto entity: GetSystemEntities())
		{ 
			const auto script = entity.GetComponent <ScriptComponent>();
			script.func(entity, deltaTime, elapsedTime);
		}
	}
};

#endif
