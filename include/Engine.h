#ifndef ENGINE_H
#define ENGINE_H

#include <Event.h>

namespace Snake
{
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	enum Entity
	{
		None,
		SnakeBody,
		SnakeHead,
		SnakeWithApple,
		SnakeEnd,
		Apple,
		Wall
	};

	class SnakeModule
	{
	public:
		SnakeModule(SnakeModule* next, const Entity& entity, const int posX, const int posY);
		~SnakeModule() {};
		void move();
		Entity getEntity() const { return m_entity; }
		int getX() const { return m_posX; }
		int getY() const { return m_posY; }
		SnakeModule* getNext() const { return m_next; }
		void setEntity(const Entity& entity) { m_entity = entity; }

	private:
		SnakeModule* m_next;
		Entity m_entity;
		int m_posX;
		int m_posY;
	};

	class SnakeList
	{
	public:
		SnakeList() {};
		~SnakeList();
		bool create(const int length, const int x, const int y);
		void move(const int x, const int y, const bool withApple);
        void append(const Entity& entity, const int x, const int y);
		bool fillWorld(Entity** world);
	private:
		void deleteRecursive(SnakeModule* module);
		bool fillWorldRecursive(Entity** world, const SnakeModule* module);

		SnakeModule* m_head;
		

	};


	class Engine
	{
	public:
		Engine();
		~Engine();
		bool create();
		Entity getEntity (const int x, const int y) const;
		int getHeight() const;
		int getWidth() const;
		bool move();
		bool fillWorld();
		bool changeDirection(const Event& event);

	private:
		void setApplePosition();

		Entity** m_world;
		int m_snakeHeadPosX;
		int m_snakeHeadPosY;
		int m_applePosX;
		int m_applePosY;
		Direction m_direction;
		int m_length;
		SnakeList m_snake;
		
	};
}

#endif