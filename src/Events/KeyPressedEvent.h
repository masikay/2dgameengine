#ifndef KEY_PRESSED_EVENT_H
#define KEY_PRESSED_EVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class KeyPressedEvent : public Event
{
public:
	SDL_Keycode symbol;

	KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};

#endif