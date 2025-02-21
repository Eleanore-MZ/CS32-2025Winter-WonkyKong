#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


/*********PLAYER IMPLEMENTATION*********/

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
	if(getWorld()->getItem(getX(),getY()-1)!=Level::floor 
		&& getWorld()->getItem(getX(), getY() - 1) != Level::ladder)
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
		if (getWorld()->getItem(getX() - 1, getY()) != Level::floor)
		{
			getPositionInThisDirection(left, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right) setDirection(right);
		if (getWorld()->getItem(getX() + 1, getY()) != Level::floor)
		{
			getPositionInThisDirection(right, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_DOWN:
		if ((getWorld()->getItem(getX(), getY()) == Level::ladder
			|| getWorld()->getItem(getX(), getY() - 1) == Level::ladder)
			&& getWorld()->getItem(getX(), getY() - 1) != Level::floor)
		{
			getPositionInThisDirection(down, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_UP:
		if ((getWorld()->getItem(getX(), getY()) == Level::ladder
			|| getWorld()->getItem(getX(), getY() + 1) == Level::ladder)
			&& getWorld()->getItem(getX(), getY() + 1) != Level::floor)
		{
			getPositionInThisDirection(up, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_SPACE:
		if (!is_jumping()
			&& (getWorld()->getItem(getX(), getY() - 1) == Level::floor
				|| getWorld()->getItem(getX(), getY() - 1) == Level::ladder
				|| getWorld()->getItem(getX(), getY()) == Level::ladder))
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
			getWorld()->playSound(SOUND_BURP);
			int newX = getX(), newY = getY();
			getPositionInThisDirection(getDirection(), 1, newX, newY);
			getWorld()->addActor(new Burp(getWorld(), newX, newY, getDirection()));

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
		if (getWorld()->getItem(getX(), getY() + 1) == Level::floor || getY() + 1 >= VIEW_HEIGHT) terminateJump();
		else
		{
			moveTo(getX(), getY() + 1); 
			if(getWorld()->getItem(getX(), getY()) == Level::ladder) terminateJump();
			else m_jump_tick++;
		}
		break;
	case 1:
	case 2:
	case 3:
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (newX < 0 || newX >= VIEW_WIDTH || newY < 0 || newY >= VIEW_HEIGHT 
			|| getWorld()->getItem(newX, newY) == Level::floor) terminateJump();
		else moveTo(newX, newY);
		if (getWorld()->getItem(getX(), getY()) == Level::ladder) terminateJump();
		else m_jump_tick++;
		break;
	case 4: 
		if (getWorld()->getItem(getX(), getY() - 1) == Level::floor || getY() - 1 < 0) terminateJump();
		else
		{
			moveTo(getX(), getY() - 1);
			terminateJump();
		}
		break;
	default: break;
	}
}


/*********EXTRALIFEGOODIE IMPLEMENTATION*********/

void ExtraLifeGoodie::doSomething()
{
	if (!is_alive()) return;
	if (getX() == getWorld()->getPlayer()->getX()
		&& getY() == getWorld()->getPlayer()->getY())
	{
		// Inform the StudentWorld object that the user is to receive 50 more points.
		// Give the Player one extra life
		giveBonus();
		// Set its own state to dead(so that it will be removed from the game by the
		// StudentWorld object at the end of the current tick).
		set_dead();
		// Play a sound effect to indicate that the Player picked up the goodie :SOUND_GOT_GOODIE.
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void ExtraLifeGoodie::giveBonus()
{
	getWorld()->increaseScore(getPoint());
	getWorld()->incLives();
}


/*********GARLICGOODIE IMPLEMENTATION*********/

void GarlicGoodie::doSomething()
{
	if (!is_alive()) return;
	if (getX() == getWorld()->getPlayer()->getX()
		&& getY() == getWorld()->getPlayer()->getY())
	{
		giveBonus();
		set_dead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

void GarlicGoodie::giveBonus()
{
	getWorld()->increaseScore(getPoint());
	getWorld()->receiveBurp(getBurp());
}


/*********BURP IMPLEMENTATION*********/

void Burp::doSomething()
{
	if (!is_alive()) return;
	remaining_tick--;
	if (remaining_tick == 0) set_dead();
	// attack any actor on the square
	attack();
}

void Burp::attack()
{
	getWorld()->atSameGrid(this);
}

/*********BONFIRE IMPLEMENTATION*********/

void Bonfire::doSomething()
{
	increaseAnimationNumber();
	attack();
}

void Bonfire::attack()
{
	//If the Player is on the same square as a Bonfire, 
	// then the Bonfire must attack the Player causing them to lose a life.
	if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())
	{
		getWorld()->decLives();
		getWorld()->getPlayer()->set_dead();
	}

	// If a Barrel is on the same square as the Bonfire, 
	// the Barrel must be attacked and destroyed
	// (the Bonfire remains alive).
	getWorld()->atSameGrid(this);
	
}


/*********FIREBALL IMPLEMENTATION*********/

void Fireball::doSomething()
{
	if (!is_alive()) return;

}

/*********BARREL IMPLEMENTATION*********/

void Barrel::doSomething()
{
	if (!is_alive()) return;
}


/*********KOOPA IMPLEMENTATION*********/

void Koopa::doSomething()
{
	if (!is_alive()) return;
	if (getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY()
		&& m_freeze_cooldown_timer == 0)
	{
		getWorld()->getPlayer()->set_frozen();
		m_freeze_cooldown_timer = 50;
		return;
	}
	if (m_freeze_cooldown_timer > 0) m_freeze_cooldown_timer--;
	if (m_tick_timer < 10) m_tick_timer++;
	else
	{
		m_tick_timer = 0;
		int newX = getX(), newY = getY();
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (getWorld()->getItem(newX, newY) == Level::floor
			|| (getWorld()->getItem(newX, newY - 1) != Level::floor
				&& getWorld()->getItem(newX, newY - 1) != Level::ladder))
		{
			if (getDirection() == left) setDirection(right);
			else setDirection(left);
		}
		else
		{
			moveTo(newX, newY);
		}
	}
}

/*********KONG IMPLEMENTATION*********/

void Kong::doSomething()
{
	if (!is_alive()) return;
	increaseAnimationNumber();
}