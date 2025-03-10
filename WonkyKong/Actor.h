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
	virtual void doSomething() = 0;
	bool is_alive() const { return m_alive; }
	virtual void set_dead() { m_alive = false; }
	virtual bool canAttack(Actor* attacked) const { return false; }
	virtual bool isEnemy() const { return false; }
	virtual bool isBarrel() const { return false; }
	virtual bool canGiveBonus() const { return false; }
	StudentWorld* getWorld() const { return m_world; }
	virtual void giveBonus() const {}
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
	int getPoint() const { return m_bonusPoint; }
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
	void giveBonus() const;
protected:
	void set_dead();
};

class GarlicGoodie :public Goodie
{
public:
	GarlicGoodie(StudentWorld* world, int startX, int startY) 
		:Goodie(world, IID_GARLIC_GOODIE, startX, startY, 25) {}
	void doSomething();
protected:
	void set_dead();
	void giveBonus() const;
	int getBurp() const { return m_bonusBurp; }
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
	bool canAttack(Actor* attacked) const { return attacked->isEnemy(); }
	void attack();
	bool canGiveBonus() const { return true; }
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
	bool canAttack(Actor* attacked) const { return attacked->isBarrel(); }
};

//************MOVABLE ACTOR**************//

class MovableActor :public Actor
{
public:
	MovableActor(StudentWorld* world, int imageID, int startX, int startY, int dir) 
		: Actor(world, imageID, startX, startY, dir, true) {}
};

class Player :public MovableActor
{
public:
	Player(StudentWorld* world, int startX, int startY)
		: MovableActor(world, IID_PLAYER, startX, startY, right) { };
	virtual void doSomething();
	void keyPressed(int key);

	int getBurps() const { return m_burps; }
	void PlayerReceiveBurp(int x) { m_burps += x; }

	// freeze
	bool is_frozen() const { return m_frozen; }
	void set_frozen() { m_frozen = true; m_freeze_timer = 50; }

	// jump
	bool is_jumping() const { return m_jumping; }
	void jumpSequence(int tick);
	void terminateJump() { m_jump_tick = -1; m_jumping = false; }
	void set_dead();
private:
	int m_burps = 0;
	bool m_frozen = false;
	int m_freeze_timer = 0;
	bool m_jumping = false;
	int m_jump_tick = -1;
};

//************ENEMY ACTOR**************//

class Enemy :public MovableActor
{
public:
	Enemy(StudentWorld* world, int imageID, int startX, int startY, int dir)
		:MovableActor(world, imageID, startX, startY, dir) {}
	virtual bool isEnemy() const { return true; }
	void incTickTimer() { m_tick_timer++; }
	int getTickTimer() const { return m_tick_timer; }
	void setTickTimerToZero() { m_tick_timer = 0; }
private:
	int m_tick_timer = 0;
};

class Fireball :public Enemy
{
public:
	Fireball(StudentWorld* world, int startX, int startY)
		:Enemy(world, IID_FIREBALL, startX, startY, randInt(0, 1) * 180) {}
	void doSomething();
	void set_dead();
	int getPoint() const { return m_bonusPoint; }
	void giveBonus() const;
private:
	int m_bonusPoint = 100;
	int m_climbing = 0; //0: non climbing; 1: climbing down; 2: climbing up
};

class Koopa :public Enemy
{
public:
	Koopa(StudentWorld* world, int startX, int startY)
		:Enemy(world, IID_KOOPA, startX, startY, randInt(0, 1) * 180) {}
	void doSomething();
	void set_dead();
	int getPoint() const { return m_bonusPoint; }
	void giveBonus() const;
private:
	int m_freeze_cooldown_timer = 0;
	int m_bonusPoint = 100;
};

class Barrel :public Enemy
{
public:
	Barrel(StudentWorld* world, int startX, int startY, int dir)
		:Enemy(world, IID_BARREL, startX, startY, dir) {}
	bool isBarrel() const { return true; }
	void doSomething();
	void set_dead();
	int getPoint() const { return m_bonusPoint; }
	void giveBonus() const;
private:
	int m_bonusPoint = 100;
	bool m_falling = false;
};

class Kong :public Enemy
{
public:
	Kong(StudentWorld* world, int startX, int startY, int dir)
		:Enemy(world, IID_KONG, startX, startY, dir) {}
	void doSomething();
	int getPoint() const { return m_bonusPoint; }
	virtual bool isEnemy() const { return false; }
private:
	bool m_fleeing = false;
	int m_barrel_timer = 0;
	int m_bonusPoint = 1000;
};

#endif // ACTOR_H_