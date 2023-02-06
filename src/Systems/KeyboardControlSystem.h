#ifndef KEYBOARD_CONTROL_SYSTEM_H
#define KEYBOARD_CONTROL_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Logger/Logger.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class KeyboardControlSystem : public System
{
public:
	KeyboardControlSystem()
	{
		RequireComponent<KeyboardControlledComponent>(); 
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubsrcibeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (auto entity : GetSystemEntities())
		{
			const auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

			switch (event.symbol)
			{
			case SDLK_UP:
				rigidBody.velocity = keyboardControl.upVelocity;
				sprite.srcRect.y = sprite.height * 0;
				break;
			
			case SDLK_RIGHT:
				rigidBody.velocity = keyboardControl.rightVelocity;
				sprite.srcRect.y = sprite.height * 1;
				break;

			case SDLK_DOWN:
				rigidBody.velocity = keyboardControl.downVelocity;
				sprite.srcRect.y = sprite.height * 2;
				break;

			case SDLK_LEFT:
				rigidBody.velocity = keyboardControl.leftVelocity;
				sprite.srcRect.y = sprite.height * 3;
				break;
			}
		}

	}

	void Update()
	{

	}
};

#endif
