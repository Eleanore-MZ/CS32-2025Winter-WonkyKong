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
	virtual void doSomething() = 0; // change to pure virtual!!!
	bool is_alive() { return m_alive; }
	void set_dead() { m_alive = false; }
	virtual bool canAttack(Actor* attacked) { return false; }
	virtual bool isEnemy() { return false; }
	virtual bool isBarreal() { return false; }
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
	virtual void doSomething() = 0;
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
		:ImmovableActor(world, imageID, startX, startY),m_bonusPoint(bonusPoint) {}
	virtual void doSomething() {}
	
protected:
	int getPoint() { return m_bonusPoint; }
	virtual void giveBonus() {}
private:
	int m_bonusPoint;
};

class ExtraLifeGoodie :public Goodie
{
public:
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
		:Goodie(world, IID_EXTRA_LIFE_GOODIE, startX, startY, 50) {}
	void doSomething();
protected:
	void giveBonus();
	int getLife() { return m_bonusLife; }
private:
	int m_bonusLife = 1;
};

class GarlicGoodie :public Goodie
{
public:
	GarlicGoodie(StudentWorld* world, int startX, int startY) 
		:Goodie(world, IID_GARLIC_GOODIE, startX, startY, 25) {}
	void doSomething();
protected:
	void giveBonus();
	int getBurp() { return m_bonusBurp; }
private:
	int m_bonusBurp = 5;
};

class Attack :public ImmovableActor
{
public:
	Attack(StudentWorld* world, int imageID, int startX, int startY, int dir) 
		:ImmovableActor(world, imageID, startX, startY, dir) {}
	virtual void attack() {}
};

class Burp :public Attack
{
public:
	Burp(StudentWorld* world, int startX, int startY, int dir)
		:Attack(world, IID_BURP, startX, startY, dir) {}
	void doSomething();
	bool canAttack(Actor* attacked) { return attacked->isEnemy(); }
	void attack();

private:
	int remaining_tick = 5;
};

class Bonfire :public Attack
{
public:
	Bonfire(StudentWorld* world, int startX, int startY)
		:Attack(world, IID_BONFIRE, startX,startY,-1) {}
	void doSomething();
	void attack();
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
	void keyPressed(int key);

	int getBurps() { return m_burps; }
	void PlayerReceiveBurp(int x) { m_burps += x; }

	// freeze
	bool is_frozen() { return m_frozen; }
	void set_frozen() { m_frozen = true; }

	// jump
	bool is_jumping() { return m_jumping; }
	void jumpSequence(int tick);
	void terminateJump() { m_jump_tick = -1; m_jumping = false; }
	

private:
	int m_burps = 0;
	bool m_frozen = false;
	bool m_jumping = false;
	int m_jump_tick = -1;
};

//************ENEMY ACTOR**************//

class Enemy :public MovableActor
{
public:
	Enemy(StudentWorld* world, int imageID, int startX, int startY, int dir)
		:MovableActor(world, imageID, startX, startY, dir) {}
	bool isEnemy() { return true; }
};

class Fireball :public Enemy
{
public:
	Fireball(StudentWorld* world, int startX, int startY)
		:Enemy(world, IID_FIREBALL, startX, startY, randInt(0, 1) * 180) {}
	void doSomething();
};

class Koopa :public Enemy
{
public:
	Koopa(StudentWorld* world, int startX, int startY)
		:Enemy(world, IID_KOOPA, startX, startY, randInt(0, 1) * 180) {}
	void doSomething();
private:
	int m_freeze_cooldown_timer = 0;
	int m_tick_timer = 0;
};

class Barrel :public Enemy
{
public:
	Barrel(StudentWorld* world, int startX, int startY, int dir)
		:Enemy(world, IID_BARREL, startX, startY, dir) {}
	bool isBarrel() { return true; }
	void doSomething();
};

class Kong :public Enemy
{
public:
	Kong(StudentWorld* world, int startX, int startY, int dir)
		:Enemy(world, IID_KONG, startX, startY, dir) {}
	void doSomething();
private:
	bool flee = false;
};

#endif // ACTOR_H_