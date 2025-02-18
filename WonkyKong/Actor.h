#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor:public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, int dir = 0, bool alive = true) :GraphObject(imageID, startX, startY, dir) {}
	virtual void doSomething() { return; } // change to pure virtual!!!
	bool is_alive() { return m_alive; }
	void set_dead() { m_alive = false; }
private:
	bool m_alive;
	
};

//************IMMOVABLE ACTOR**************//

class ImmovableActor :public Actor
{
public:
	ImmovableActor(int imageID, int startX, int startY, int dir = none) :Actor(imageID, startX, startY, dir, true) {}
	virtual void doSomething() {}
private:
};


class Floor :public ImmovableActor
{
public:
	Floor(int startX, int startY) : ImmovableActor(IID_FLOOR, startX, startY) {}
	virtual void doSomething() {}
};

class Ladder :public ImmovableActor
{
public:
	Ladder(int startX, int startY) : ImmovableActor(IID_LADDER, startX, startY) {}
	virtual void doSomething() {}
};

class Goodie :public ImmovableActor
{
public:
	Goodie(int imageID, int startX, int startY, int bonusPoint) :ImmovableActor(imageID, startX, startY) {}
	virtual void doSomething() {}
	virtual void giveBonus() {}
private:
	int bonusPoint;
};

class ExtraLifeGoodie :public Goodie
{
public:
	ExtraLifeGoodie(int startX, int startY) :Goodie(IID_EXTRA_LIFE_GOODIE, startX, startY, 50) {}
	void doSoemthing() {}
	void giveBonus() {}
private:
	int bonusLife = 1;
};

class GarlicGoodie :public Goodie
{
public:
	GarlicGoodie(int startX, int startY) :Goodie(IID_GARLIC_GOODIE, startX, startY, 25) {}
	void doSoemthing() {}
	void giveBonus() {}
private:
	int bonusBurp = 5;
};

class Attack :public ImmovableActor
{

};

class Burp :public Attack
{

};

class Bonfire :public Attack
{

};

//************MOVABLE ACTOR**************//

class MovableActor :public Actor
{
public:
	MovableActor(int imageID, int startX, int startY, int dir) : Actor(imageID, startX, startY, dir, true) {}
	virtual void is_attacked() {}
};

class Player :public MovableActor
{
public:
	Player(int startX, int startY) : MovableActor(IID_PLAYER, startX, startY, right) { };
	void doSomething() {}
	int burps() { return m_burps; }
	void keyPressed(int key)
	{
		switch (key)
		{
		case KEY_PRESS_LEFT:
			break;
		case KEY_PRESS_RIGHT:
			break;
		case KEY_PRESS_UP:
			break;
		case KEY_PRESS_DOWN:
			break;
		case KEY_PRESS_TAB:
			break;
		case KEY_PRESS_SPACE:
			break;
		default:
			break;
		}
	}

private:
	int m_burps = 0;
};

//************ENEMY ACTOR**************//

class Enemy :public MovableActor
{
	
};

class Fireball :public Enemy
{

};

class Koopa :public Enemy
{

};

class Barrel :public Enemy
{

};

class Kong :public Enemy
{

};

#endif // ACTOR_H_