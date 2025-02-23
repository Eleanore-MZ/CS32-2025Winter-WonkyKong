#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  bool isFloor(int x, int y) { return m_maze[y][x] == Level::floor; }
  bool isLadder(int x, int y) { return m_maze[y][x] == Level::ladder; }

  void addActor(Actor* newActor) { m_actorList.push_back(newActor); }
  Player* getPlayer() { return m_player; }
  bool playerAtSamePosition(int x, int y) { return m_player->getX() == x && m_player->getY() == y; }
  void receiveBurp(int x) { m_player->PlayerReceiveBurp(x); }
  void atSameGrid(Actor* attacker);
  void finishLevel() { m_finishedLevel = true; }


private:
	std::vector <Actor*> m_actorList;
	Player* m_player;
	Level::MazeEntry m_maze[VIEW_HEIGHT][VIEW_WIDTH];
	bool m_finishedLevel = false;
	int m_level = 0;
};

#endif // STUDENTWORLD_H_
