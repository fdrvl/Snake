#include <GraphicEngine.h>
#include <SnakeGame.h>

namespace
{
	constexpr int kHeight = 400;
	constexpr int kWidth = 600;
}

namespace Snake
{
	GameWindow::GameWindow()
	{
		m_isInit = false;
	}

	bool GameWindow::create()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
		{
			LogError("Cannot initialize video for SDL");
			return false;
		}

		m_window = SDL_CreateWindow("Snake",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			kWidth,
			kHeight,
			SDL_WINDOW_OPENGL);

		if (!m_window)
		{
			LogError("Cannot create window");
			return false;
		}
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

		if (!m_renderer)
		{
			LogError("Cannot create renderer");
			return false;
		}

		return true;
	}

	bool GameWindow::close()
	{
		SDL_DestroyWindow(m_window);

		SDL_Quit();

		return true;
	}

	Event GameWindow::getEvent()
	{
		if (SDL_PollEvent(&m_event) == 1)
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
				return Event::QUIT;
			case SDL_KEYDOWN:
				switch (m_event.key.keysym.sym)
				{
				case SDLK_LEFT:
					return Event::LEFTKEY;
				case SDLK_RIGHT:
					return Event::RIGHTKEY;
				case SDLK_q:
					return Event::QUIT;
				}
			}
		}

		return Event::NONE;
	}

	bool GameWindow::display(const Engine& engine)
	{
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
		SDL_RenderClear(m_renderer);

		constexpr int kXOffset = 0;
		constexpr int kYOffset = 20;

		constexpr int worldWidth = kWidth - kXOffset;
		constexpr int worldHeight = kHeight - kYOffset;	

		if (!drawWorld(engine, kXOffset, kYOffset, worldWidth, worldHeight))
		{
			LogError("Cannot draw world");
			return false;
		}

		SDL_RenderPresent(m_renderer);

		return true;
	}

	bool GameWindow::drawWorld(const Engine& engine, const int xOffset, const int yOffset, const int width, const int height)
	{
		const int entitySizeX =width / (engine.getWidth());
		const int entitySizeY = height / (engine.getHeight());

		int entitySize = entitySizeX;
		if (entitySizeY < entitySizeX)
			entitySize = entitySizeY;

		SDL_Rect rect;
		rect.w = entitySize;
		rect.h = entitySize;

		for (int x = 0; x < engine.getWidth(); x++)
		{
			for (int y = 0; y < engine.getHeight(); y++)
			{
				rect.x = x * entitySize;
				rect.y = y * entitySize;
				switch (engine.getEntity(x, y))
				{
				case Entity::Wall:
					SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
					SDL_RenderFillRect(m_renderer, &rect);
					break;
				case Entity::SnakeBody:
					SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
					SDL_RenderFillRect(m_renderer, &rect);
					break;
				case Entity::SnakeHead:
					SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
					SDL_RenderFillRect(m_renderer, &rect);
					break;
				case Entity::SnakeWithApple:
					SDL_SetRenderDrawColor(m_renderer, 255, 0, 255, 255);
					SDL_RenderFillRect(m_renderer, &rect);
					break;
				case Entity::SnakeEnd:
					SDL_SetRenderDrawColor(m_renderer, 0, 255, 255, 255);
					SDL_RenderFillRect(m_renderer, &rect);
					break;
				case Entity::Apple:
					SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
					SDL_RenderFillRect(m_renderer, &rect);
					break;
				}
			}
		}

		return true;
	}
}