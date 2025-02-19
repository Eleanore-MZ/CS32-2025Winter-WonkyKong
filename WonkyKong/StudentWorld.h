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

  bool is_an_item(int x, int y, Level::MazeEntry item)
  {
	  if (m_maze[y][x] == item)
		  return true;
	  return false;
  }
  

private:
	std::vector <Actor*> m_actorList;
	Player* m_player;
	int m_score = 0;
	int m_lives = 3;
	int m_level = 0;
	Level::MazeEntry m_maze[VIEW_HEIGHT][VIEW_WIDTH];
};

#endif // STUDENTWORLD_H_
