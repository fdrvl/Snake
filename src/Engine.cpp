#include <Engine.h>
#include <SnakeGame.h>
#include <random>

namespace
{
	constexpr int kWidth = 30;
	constexpr int kHeight = 20;
}


namespace Snake
{
	Engine::Engine()
	{
		m_world = new Entity*[kWidth];
		for (int i = 0; i < kWidth; i++)
		{
			m_world[i] = new Entity[kHeight];
		}
	}

	Engine::~Engine()
	{
		for (int i = 0; i < kWidth; i++)
		{
			delete[] m_world[i];
		}
		delete[] m_world;
	}

	bool Engine::create()
	{
		m_snakeHeadPosX = 5;
		m_snakeHeadPosY = 5;
		m_applePosX = 8; //TODO to change to random value
		m_applePosY = 8;
		m_length = 4;
		m_direction = Direction::UP;

		if (!m_snake.create(m_length, m_snakeHeadPosX, m_snakeHeadPosY))
		{
			LogError("Cannot create snake list");
			return false;
		}

		if (!fillWorld())
		{
			LogError("Cannot fill world");
			return false;
		}

		setApplePosition();
		
		return true;
	}

	void Engine::setApplePosition()
	{
		const int pos = (rand() % (kWidth * kHeight)) - m_length + 1;
		int posX = 1;
		int posY = 1;

		for (int i = 0; i < pos; i++)
		{
			posX++;
			if (posX >= kWidth - 1)
			{
				posX = 0;
				posY++;
				if (posY >= kHeight - 1)
				{
					posY = 0;
				}
			}

			while (m_world[posX][posY] != Entity::None)
			{
				posX++;
				if (posX >= kWidth - 1)
				{
					posX = 0;
					posY++;
					if (posY >= kHeight - 1)
					{
						posY = 0;
					}
				}
			}
		}

		m_applePosX = posX;
		m_applePosY = posY;

		return;
	}

	bool Engine::fillWorld()
	{
		for (int x = 0; x < kWidth; x++)
		{
			for (int y = 0; y < kHeight; y++)
			{
				if (x == 0 || x == kWidth - 1 || y == 0 || y == kHeight - 1)
				{
					m_world[x][y] = Entity::Wall;
				}
				else
				{
					m_world[x][y] = Entity::None;
				}
			}
		}

		if (!m_snake.fillWorld(m_world))
		{
			LogError("Cannot fill world with snake");
			return false;
		}

		m_world[m_applePosX][m_applePosY] = Apple;

		return true;
	}

	Entity Engine::getEntity(const int x, const int y) const
	{
		if (x < 0 || x >= kWidth || y < 0 || y >= kHeight)
		{
			return Entity::None;
		}

		return m_world[x][y];
	}

	int Engine::getHeight() const
	{
		return kHeight;
	}

	int Engine::getWidth() const
	{
		return kWidth;
	}

	bool Engine::move()
	{
		switch (m_direction)
		{
		case Direction::UP:
			m_snakeHeadPosY--;
			break;
		case Direction::DOWN:
			m_snakeHeadPosY++;
			break;
		case Direction::LEFT:
			m_snakeHeadPosX--;
			break;
		case Direction::RIGHT:
			m_snakeHeadPosX++;
			break;
		default:
			LogError("Invalid direction for move");
			return false;
		}

		bool withApple = false;

		switch (m_world[m_snakeHeadPosX][m_snakeHeadPosY]) 
		{
		case SnakeBody:
			return false;
			break;
		case SnakeWithApple:
			return false;
			break;
		case SnakeEnd:
			return false;
			break;
		case Wall:
			return false;
			break;
		case Apple:
			m_length++;
			withApple = true;
			setApplePosition();
			break;
		case None:
			break;
		default:
			LogError("Invalid entity for move");
			return false;
		}

		m_world[m_snakeHeadPosX][m_snakeHeadPosY] = Entity::None;

		m_snake.move(m_snakeHeadPosX, m_snakeHeadPosY, withApple);

		return true;
	}

	bool Engine::changeDirection(const Event & event)
	{
		switch (event)
		{
		case Event::LEFTKEY:
			switch (m_direction)
			{
			case Snake::UP:
				m_direction = Direction::LEFT;
				break;
			case Snake::DOWN:
				m_direction = Direction::RIGHT;
				break;
			case Snake::LEFT:
				m_direction = Direction::DOWN;
				break;
			case Snake::RIGHT:
				m_direction = Direction::UP;
				break;
			default:
				LogError("Invalid direction");
				return false;
				break;
			}
			break;
		case Event::RIGHTKEY:
			switch (m_direction)
			{
			case Snake::UP:
				m_direction = Direction::RIGHT;
				break;
			case Snake::DOWN:
				m_direction = Direction::LEFT;
				break;
			case Snake::LEFT:
				m_direction = Direction::UP;
				break;
			case Snake::RIGHT:
				m_direction = Direction::DOWN;
				break;
			default:
				LogError("Invalid direction");
				return false;
				break;
			}
			break;
		}

		return true;
	}

	SnakeModule::SnakeModule(SnakeModule* next, const Entity& entity, const int posX, const int posY) :
		m_next(next),
		m_entity(entity),
		m_posX(posX),
		m_posY(posY)
	{
	}


	void SnakeModule::move()
	{
		if (m_next)
		{
			if (m_next->getEntity() == Entity::SnakeEnd)
			{
				if (m_entity == Entity::SnakeWithApple)
				{
					m_entity = Entity::SnakeBody;
				}
				else
				{
					delete m_next;
					m_next = nullptr;
					m_entity = Entity::SnakeEnd;
				}
			}
		}

		return;
	}

	void SnakeList::append(const Entity& entity, const int x, const int y)
	{
		m_head = new SnakeModule(m_head, entity, x, y);
	}

	bool SnakeList::create(const int length, const int x, const int y)
	{
		//TODO verify if length and coordinate are ok
		m_head = new SnakeModule(nullptr, Entity::SnakeEnd, x, y + length - 1);

		for (int i = 0; i < length - 2; i++)
		{
			this->append(Entity::SnakeBody, x, y + length - 2 - i);
		}

		this->append(Entity::SnakeHead, x, y);

		return true;
	}

	void SnakeList::deleteRecursive(SnakeModule* module)
	{
		if (!module->getNext())
		{
			delete module;
			return;
		}

		deleteRecursive(module->getNext());
		return;
	}

	SnakeList::~SnakeList()
	{
		deleteRecursive(m_head);
	}

	// not recursive because it's a propagation 
	void SnakeList::move(const int x, const int y, const bool withApple)
	{
		if (withApple)
		{
			m_head->setEntity(Entity::SnakeWithApple);
		}
		else
		{
			m_head->setEntity(Entity::SnakeBody);
		}

		SnakeModule* module = m_head;
		while (module)
		{
			module->move();
			module = module->getNext();
		}

		this->append(Entity::SnakeHead, x, y);
	}

	bool SnakeList::fillWorld(Entity** world)
	{
		return fillWorldRecursive(world, m_head);
	}

	bool SnakeList::fillWorldRecursive(Entity** world, const SnakeModule* module)
	{
		if (!module)
		{
			return true;
		}

		if (!fillWorldRecursive(world, module->getNext()))
		{
			LogError("Error in recursive world filling");
			return false;
		}

		if (world[module->getX()][module->getY()] != Entity::None)
		{
			LogError("Invalid entity while filling the world with snake");
			return false;
		}

		world[module->getX()][module->getY()] = module->getEntity();

		return true;
	}
}