#include <SnakeGame.h>


namespace Snake
{
	Game::Game() : m_play(false)
	{

	}

	Game::~Game()
	{
		
	}

	bool Game::start()
	{
		if (m_play)
		{
			LogError("Game is already running");
			return false;
		}
		if (!m_gameWindow.create())
		{
			LogError("Cannot create game window");
			return false;
		}

		if (!m_engine.create())
		{
			LogError("Cannot create game engine");
			return false;
		}

		m_play = true;

		if (!mainLoop())
		{
			LogError("Error occured in the main loop");
			return false;
		}

		m_gameWindow.close();

		return true;
	}

	bool Game::mainLoop()
	{
		if (!m_play)
		{
			LogError("Cannot start main loop when game isn't running");
			return false;
		}

		
		while(m_play)
		{
			Event event = m_gameWindow.getEvent();
			if (event == Event::QUIT)
			{
				m_play = false;
				break;
			}

			m_engine.fillWorld();
			m_gameWindow.display(m_engine);
		}

		//for debug
		m_engine.move();
		m_engine.fillWorld();
		m_gameWindow.display(m_engine);
		system("PAUSE");

		return true;
	}
}




