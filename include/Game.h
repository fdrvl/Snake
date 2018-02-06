#ifndef GAME_H
#define GAME_H

#include "GraphicEngine.h"

namespace Snake
{
	class Game
	{
	public:
		Game();
		~Game();
		bool start();
		bool mainLoop();
	private:
		bool m_play;
		GameWindow m_gameWindow;
		Engine m_engine;
	};
}

#endif
