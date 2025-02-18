#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor:public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int dir = 0, bool alive = true) 
		:GraphObject(imageID, startX, startY, dir), m_alive(alive), m_world(world) {}
	virtual void doSomething() { return; } // change to pure virtual!!!
	bool is_alive() { return m_alive; }
	void set_dead() { m_alive = false; }
	StudentWorld* getWorld() { return m_world; }
private:
	bool m_alive;
	StudentWorld* m_world;
};

//************IMMOVABLE ACTOR**************//

class ImmovableActor :public Actor
{
public:
	ImmovableActor(StudentWorld* world, int imageID, int startX, int startY, int dir = none) 
		:Actor(world,imageID, startX, startY, dir, true) {}
	virtual void doSomething() {}
private:
};


class Floor :public ImmovableActor
{
public:
	Floor(StudentWorld* world, int startX, int startY) 
		: ImmovableActor(world, IID_FLOOR, startX, startY) {}
	virtual void doSomething() {}
};

class Ladder :public ImmovableActor
{
public:
	Ladder(StudentWorld* world, int startX, int startY)
		: ImmovableActor(world, IID_LADDER, startX, startY) {}
	virtual void doSomething() {}
};

class Goodie :public ImmovableActor
{
public:
	Goodie(StudentWorld* world, int imageID, int startX, int startY, int bonusPoint) 
		:ImmovableActor(world, imageID, startX, startY) {}
	virtual void doSomething() {}
	virtual void giveBonus() {}
private:
	int bonusPoint;
};

class ExtraLifeGoodie :public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
		:Goodie(world, IID_EXTRA_LIFE_GOODIE, startX, startY, 50) {}
	void doSoemthing() {}
	void giveBonus() {}
private:
	int bonusLife = 1;
};

class GarlicGoodie :public Goodie
{
public:
	GarlicGoodie(StudentWorld* world, int startX, int startY) 
		:Goodie(world, IID_GARLIC_GOODIE, startX, startY, 25) {}
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
	MovableActor(StudentWorld* world, int imageID, int startX, int startY, int dir) 
		: Actor(world, imageID, startX, startY, dir, true) {}
	virtual void is_attacked() {}
};

class Player :public MovableActor
{
public:
	Player(StudentWorld* world, int startX, int startY)
		: MovableActor(world, IID_PLAYER, startX, startY, right) { };
	virtual void doSomething();
	int burps() { return m_burps; }
	void keyPressed(int key);

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