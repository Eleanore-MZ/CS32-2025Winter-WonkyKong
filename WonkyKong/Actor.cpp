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

	if (m_freeze_timer == 0) m_frozen = false;
	if (m_frozen)
	{
		// Decrement the freeze timer by one tick
		m_freeze_timer--;
		return;
	}

	if (!(getWorld()->isLadder(getX(), getY())) && !(getWorld()->isFloor(getX(), getY() - 1)) && !(getWorld()->isLadder(getX(), getY() - 1)))
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
		else if (!(getWorld()->isFloor(getX() - 1, getY())))
		{
			getPositionInThisDirection(left, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_RIGHT:
		if (getDirection() != right) setDirection(right);
		else if (!(getWorld()->isFloor(getX() + 1, getY())))
		{
			getPositionInThisDirection(right, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_DOWN:
		if ((getWorld()->isLadder(getX(), getY()) 
			|| getWorld()->isLadder(getX(), getY() - 1)) 
			&& !(getWorld()->isFloor(getX(), getY() - 1)))
		{
			getPositionInThisDirection(down, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_UP:
		if (getWorld()->isLadder(getX(), getY()) && !(getWorld()->isFloor(getX(), getY() + 1)))
		{
			getPositionInThisDirection(up, 1, newX, newY);
			moveTo(newX, newY);
		}
		break;
	case KEY_PRESS_SPACE:
		if (!is_jumping() 
			&& getWorld()->isFloor(getX(), getY() - 1) 
			|| getWorld()->isLadder(getX(), getY() - 1) 
			|| getWorld()->isLadder(getX(), getY()))
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
		if (getWorld()->isFloor(getX(), getY() + 1) || getY() + 1 >= VIEW_HEIGHT)
			terminateJump();
		else
		{
			moveTo(getX(), getY() + 1); 
			if(getWorld()->isLadder(getX(), getY()))
				terminateJump();
			else m_jump_tick++;
		}
		break;
	case 1:
	case 2:
	case 3:
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (newX < 0 || newX >= VIEW_WIDTH || newY < 0 || newY >= VIEW_HEIGHT
			|| getWorld()->isFloor(newX, newY))
			terminateJump();
		else moveTo(newX, newY);
		if (getWorld()->isLadder(getX(), getY()))
			terminateJump();
		else m_jump_tick++;
		break;
	case 4: 
		if (getWorld()->isFloor(getX(), getY() - 1) || getY() - 1 < 0)
			terminateJump();
		else
		{
			moveTo(getX(), getY() - 1);
			terminateJump();
		}
		break;
	default: break;
	}
}

void Player::set_dead()
{
	MovableActor::set_dead();
	getWorld()->decLives();
	getWorld()->playSound(SOUND_PLAYER_DIE);
}

/*********EXTRALIFEGOODIE IMPLEMENTATION*********/

void ExtraLifeGoodie::doSomething()
{
	if (!is_alive()) return;
	if (getWorld()->playerAtSamePosition(getX(), getY()))
		// Inform the StudentWorld object that the user is to receive 50 more points.
		// Give the Player one extra life
		// Set its own state to dead(so that it will be removed from the game by the
		// StudentWorld object at the end of the current tick).
		set_dead();
		// Play a sound effect to indicate that the Player picked up the goodie :SOUND_GOT_GOODIE.
}

void ExtraLifeGoodie::set_dead()
{
	giveBonus();
	Goodie::set_dead();
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

void ExtraLifeGoodie::giveBonus() const
{
	getWorld()->increaseScore(getPoint());
	getWorld()->incLives();
}

/*********GARLICGOODIE IMPLEMENTATION*********/

void GarlicGoodie::doSomething()
{
	if (!is_alive()) return;
	if (getWorld()->playerAtSamePosition(getX(), getY()))
		set_dead();
}

void GarlicGoodie::set_dead()
{
	giveBonus();
	Goodie::set_dead();
	getWorld()->playSound(SOUND_GOT_GOODIE);
}

void GarlicGoodie::giveBonus() const
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
	if (getWorld()->playerAtSamePosition(getX(), getY()))
	{
		getWorld()->getPlayer()->set_dead();
	}

	// If a Barrel is on the same square as the Bonfire, 
	// the Barrel must be attacked and destroyed
	// (the Bonfire remains alive).
	attack();
}

void Bonfire::attack()
{
	getWorld()->atSameGrid(this);
}


/*********FIREBALL IMPLEMENTATION*********/

void Fireball::doSomething()
{
	if (!is_alive()) return;
	if (getWorld()->playerAtSamePosition(getX(), getY()))
	{
		// attack the player
		getWorld()->getPlayer()->set_dead();
		return;
	}
	if (getTickTimer() < 10) incTickTimer();
	else
	{
		setTickTimerToZero();
		//If the Fireball¡¯s current square is climbable, there is no solid object above the 
		// Fireball, and the Fireball is not in a climbing down state
		if ((getWorld()->isLadder(getX(), getY()))
			&& !(getWorld()->isFloor(getX(), getY() + 1))
			&& m_climbing != 1)
		{
			if (m_climbing == 2 || randInt(1, 3) == 3)
			{
				m_climbing = 2;
				moveTo(getX(), getY() + 1);
				// Once it moves, the Fireball should skip to step e
			}
		}
		//  if the square below the Fireball is climbable and the Fireball is not in a climbing up state, 
		// it must decide whether to climb down
		else if ((getWorld()->isLadder(getX(), getY() - 1))
			&& m_climbing != 2)
		{
			if (m_climbing == 1 || randInt(1, 3) == 3)
			{
				m_climbing = 1;
				moveTo(getX(), getY() - 1);
				// Once it moves, the Fireball should skip to step e
			}
		}
		else if (m_climbing != 0)
		{
			m_climbing = 0;
		}

		// The Fireball will attempt to move horizontally in the direction it is currently facing
		if (m_climbing == 0)
		{
			int newX = getX(), newY = getY();
			getPositionInThisDirection(getDirection(), 1, newX, newY);
			if (getWorld()->isFloor(newX, newY)
				|| (!(getWorld()->isFloor(newX, newY - 1))
					&& !(getWorld()->isLadder(newX, newY - 1))))
			{
				if (getDirection() == left) setDirection(right);
				else setDirection(left);
			}
			else
				moveTo(newX, newY);
		}
		
		if (getWorld()->playerAtSamePosition(getX(), getY()))
		{
			// attack the player
			getWorld()->getPlayer()->set_dead();
			return;
		}
	}
}

void Fireball::set_dead()
{
	Enemy::set_dead();
	getWorld()->playSound(SOUND_ENEMY_DIE);
}

void Fireball::giveBonus() const
{
	getWorld()->increaseScore(getPoint());
	if (randInt(1, 3) == 3)
		getWorld()->addActor(new GarlicGoodie(getWorld(), getX(), getY()));
}

/*********BARREL IMPLEMENTATION*********/

void Barrel::doSomething()
{
	if (!is_alive()) return;
	if (getWorld()->playerAtSamePosition(getX(), getY()))
	{
		getWorld()->getPlayer()->set_dead();
		return;
	}
	
	if (!(getWorld()->isFloor(getX(), getY() - 1)))
	{
		moveTo(getX(), getY() - 1);
		m_falling = true;
	}

	if (getWorld()->isFloor(getX(), getY() - 1) && m_falling)
	{
		m_falling = false;
		if (getDirection() == left) setDirection(right);
		else setDirection(left);
	}
	
	if (getTickTimer() < 10) incTickTimer();
	else
	{
		setTickTimerToZero();
		int newX = getX(), newY = getY();
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (getWorld()->isFloor(newX, newY))
		{
			if (getDirection() == left) setDirection(right);
			else setDirection(left);
		}
		else
			moveTo(newX, newY);
	}
	if (getWorld()->playerAtSamePosition(getX(),getY()))
	{
		getWorld()->getPlayer()->set_dead();
		return;
	}
}

void Barrel::set_dead()
{
	Enemy::set_dead();
}

void Barrel::giveBonus() const
{
	getWorld()->playSound(SOUND_ENEMY_DIE);
	getWorld()->increaseScore(getPoint());
}

/*********KOOPA IMPLEMENTATION*********/

void Koopa::doSomething()
{
	if (!is_alive()) return;
	if (getWorld()->playerAtSamePosition(getX(),getY())
		&& m_freeze_cooldown_timer == 0)
	{
		getWorld()->getPlayer()->set_frozen();
		m_freeze_cooldown_timer = 50;
		return;
	}
	if (m_freeze_cooldown_timer > 0) m_freeze_cooldown_timer--;
	if (getTickTimer() < 10) incTickTimer();
	else
	{
		setTickTimerToZero();
		int newX = getX(), newY = getY();
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (getWorld()->isFloor(newX, newY) 
			|| (!(getWorld()->isFloor(newX, newY - 1))
				&& !(getWorld()->isLadder(newX, newY - 1))))
		{
			if (getDirection() == left) setDirection(right);
			else setDirection(left);
		}
		else
			moveTo(newX, newY);
	}
	if (getWorld()->playerAtSamePosition(getX(), getY())
		&& m_freeze_cooldown_timer == 0)
	{
		getWorld()->getPlayer()->set_frozen();
		m_freeze_cooldown_timer = 50;
		return;
	}
}

void Koopa::set_dead()
{
	Enemy::set_dead();
	getWorld()->playSound(SOUND_ENEMY_DIE);
}

void Koopa::giveBonus() const
{
	getWorld()->increaseScore(getPoint());
	if (randInt(1, 3) == 3)
		getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
}

/*********KONG IMPLEMENTATION*********/

void Kong::doSomething()
{
	if (!is_alive()) return;
	increaseAnimationNumber();
	
	int kongX = getX();
	int kongY = getY();
	int playerX = getWorld()->getPlayer()->getX();
	int playerY = getWorld()->getPlayer()->getY();
	double distance = std::sqrt(std::pow(kongX - playerX, 2) + std::pow(kongY - playerY, 2));
	if (distance <= 2) m_fleeing = true;

	int N = std::max(200 - 50 * getWorld()->getLevel(), 50);

	if (!m_fleeing && m_barrel_timer == N)
	{
		int newX = getX(), newY = getY();
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		getWorld()->addActor(new Barrel(getWorld(), newX, newY, getDirection()));
		m_barrel_timer = 0;
	}
	else m_barrel_timer++;

	if (getTickTimer() == 5)
	{
		setTickTimerToZero();
		if (m_fleeing)
		{
			if (getY() + 1 < VIEW_HEIGHT)
				moveTo(getX(), getY() + 1);
			else
			{
				getWorld()->increaseScore(getPoint());
				getWorld()->playSound(SOUND_FINISHED_LEVEL);
				getWorld()->finishLevel();
			}
		}
	}
	else incTickTimer();
}