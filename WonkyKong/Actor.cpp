#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void Player::doSomething()
{
	if (!is_alive()) return;
	if (is_jumping())
	{
		// jump sequence
		jumpSequence(m_jump_tick);
		return;
	}
	if (is_frozen())
	{
		// Decrement the freeze timer by one tick
		return;
	}
	if (!(getWorld()->is_an_item(getX(), getY() - 1, Level::floor))
		&& !(getWorld()->is_an_item(getX(), getY() - 1, Level::ladder)))
	{
		// Attempt to move the Player one square downward(simulate falling).
		moveTo(getX(), getY() - 1);
		return;
	}
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
		if (!(getWorld()->is_an_item(getX() - 1, getY(), Level::floor)))
		{
			getPositionInThisDirection(left, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right)	setDirection(right);
		if (!(getWorld()->is_an_item(getX() + 1, getY(), Level::floor)))
		{
			getPositionInThisDirection(right, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_DOWN:
		if ((getWorld()->is_an_item(getX(), getY(), Level::ladder)
			|| getWorld()->is_an_item(getX(), getY() - 1, Level::ladder))
			&& !(getWorld()->is_an_item(getX(), getY() - 1, Level::floor)))
		{
			getPositionInThisDirection(down, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_UP:
		if ((getWorld()->is_an_item(getX(), getY(), Level::ladder)
			|| getWorld()->is_an_item(getX(), getY() + 1, Level::ladder))
			&& !(getWorld()->is_an_item(getX(), getY() + 1, Level::floor)))
		{
			getPositionInThisDirection(up, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_SPACE:
		if (!is_jumping() 
			&& (getWorld()->is_an_item(getX(), getY() - 1, Level::floor) 
				|| getWorld()->is_an_item(getX(), getY() - 1, Level::ladder) 
				|| getWorld()->is_an_item(getX(), getY(), Level::ladder)))
		{
			// Perform the first step of a new jump sequence (see below). 
			m_jump_tick = 0;
			m_jumping = true;
			jumpSequence(m_jump_tick);
			// Play the jump sound effect
			getWorld()->playSound(SOUND_JUMP);
		}
		break;
	case KEY_PRESS_TAB:
		if (getBurps() != 0)
		{
			// Play the Burp sound effect (SOUND_BURP)
			// Create a new Burp object in the square immediately adjacent to
			// the Player in the direction the Player is currently facing and add it to the game.
			m_burps--;
		}
		break;
	default:
		break;
	}
}

void Player::jumpSequence(int tick)
{
	int newX = getX();
	int newY = getY();
	switch (tick)
	{
	case 0: 
		if (getWorld()->is_an_item(getX(), getY() + 1, Level::floor) || getY() + 1 >= VIEW_HEIGHT) terminateJump();
		else
		{
			moveTo(getX(), getY() + 1); 
			if (getWorld()->is_an_item(getX(), getY(), Level::ladder)) terminateJump();
			else m_jump_tick++;
		}
		break;
	case 1:
	case 2:
	case 3:
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (getWorld()->is_an_item(newX, newY, Level::floor) || newX < 0 || newX >= VIEW_WIDTH || newY<0 || newY>VIEW_HEIGHT) terminateJump();
		else moveTo(newX, newY);
		if (getWorld()->is_an_item(getX(), getY(), Level::ladder)) terminateJump();
		else m_jump_tick++;
		break;
	case 4: 
		if (getWorld()->is_an_item(getX(), getY() - 1, Level::floor) || getY() - 1 < 0) terminateJump();
		else
		{
			moveTo(getX(), getY() - 1);
			terminateJump();
		}
		break;
	default: break;
	}
}