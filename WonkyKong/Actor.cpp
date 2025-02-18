#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
	int key;
	if (getWorld()->getKey(key))
	{
		keyPressed(key);
	}
}

void Player::keyPressed(int key)
{
	int newX, newY;
	switch (key)
	{
	case KEY_PRESS_LEFT:
		if (getDirection() != left)	setDirection(left);
		if (!(getWorld()->is_blocked(getX() - 1, getY())))
		{
			getPositionInThisDirection(left, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right)	setDirection(right);
		if (!(getWorld()->is_blocked(getX() + 1, getY())))
		{
			getPositionInThisDirection(right, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_DOWN:
		getPositionInThisDirection(down, 1, newX, newY);
		moveTo(newX, newY);
		break;
	case KEY_PRESS_UP:
		getPositionInThisDirection(up, 1, newX, newY);
		moveTo(newX, newY);
		break;
	default:
		break;
	}
}