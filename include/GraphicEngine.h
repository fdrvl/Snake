#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H

#include "SDL.h"
#include "Engine.h"

namespace Snake
{
	enum Event
	{
		NONE,
		LEFTKEY,
		RIGHTKEY,
		QUIT
	};

	class GameWindow
	{
	public:
		GameWindow();
		~GameWindow() {};
		bool isInit() const { return m_isInit; }
		bool create();
		bool close();
		Event getEvent();
		bool display(const Engine& engine);

	private:
		bool drawWorld(const Engine& engine, const int xOffset, const int yOffset, const int width, const int height);

		bool m_isInit;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Event m_event;
	};
}

#endif
