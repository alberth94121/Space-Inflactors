#include "actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
using namespace std;

// Students:  Add code to this file (if you wish), actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY):
GraphObject(imageID, startX, startY)
{
	setVisible(true);
	m_sw = sw;
	m_dead = false;
}

Actor::~Actor()
{
}

bool Actor::isDead() const
{
	return m_dead;
}

void Actor::setDead()
{
	m_dead = true;
}

StudentWorld* Actor::getWorld() 
{
	return m_sw;
}


//Star

Star::Star(StudentWorld* sw):
Actor(sw, IID_STAR, rand()%VIEW_WIDTH, VIEW_HEIGHT-1)
{}

Star::~Star()
{}

void Star::DoSomething()
{
	moveTo(getX(),(getY()-1));
	if(getY() < 0)
		setDead();
}

//Projectile
Projectile::Projectile(StudentWorld* sw, int imageID, int startX, int startY, bool playerFired, int damagePoints):
Actor(sw, imageID, startX, startY)
{
	m_playerFired = playerFired;
	m_damagePoints = damagePoints;
}

Projectile::~Projectile()
{}

void Projectile::DoSomething()
{
	if(playerFired()) //check if hit something
	{
		vector<Alien*> hitAliens;
		hitAliens = getWorld()->getCollidingAliens(this);
		if(hitAliens.empty())
		{
				moveTo(getX(),getY()+1);
		}
		else
		{
			for(size_t k=0;k < hitAliens.size();k++)
		    {
			    hitAliens[k]->damage(m_damagePoints, true);
				if(!(hitAliens[k]->isDead())) //if alien is not dead check to see if smallbot to set the justhit variable
				{
					Alien* xx=hitAliens[k];
					smallBot* ss = dynamic_cast<smallBot*>(xx);
					if(ss != NULL)
					{
						ss->setHit();
					}
				}
		    }
		    setDead();
			return;
		}
	}
	else //not player fired;
	{
		Player* hitPlayer;
		hitPlayer = getWorld()->getCollidingPlayer(this);
		if(hitPlayer == NULL)
		{
			moveTo(getX(),getY()-1);
		}
		else
		{
			hitPlayer->damage(m_damagePoints,true);
			setDead();
			getWorld()->subtractProj();
			return;
		}

	}
	//did not hit anything, so it moved up/down
	if(getY() >= VIEW_HEIGHT || getY() < 0) //check if dead after moving
	{  
		if (getY() <0)
		{
			getWorld()->subtractProj();
		}
		setDead();
		return;
	}
	else //check if hit something again
	{
		if(playerFired())
	    {
		    vector<Alien*> hitAliens2;
		    hitAliens2 = getWorld()->getCollidingAliens(this);
		    if(!hitAliens2.empty())
			{
			     for(size_t k=0;k < hitAliens2.size();k++)
		         {
			         hitAliens2[k]->damage(m_damagePoints, true);
				     if(!(hitAliens2[k]->isDead()))
				     {
					    Alien* yy=hitAliens2[k];
					    smallBot* y = dynamic_cast<smallBot*>(yy);
					    if(y != NULL)
					    {
						    y->setHit();
					    }
					
				     }
		         }
		         setDead();
			     return;
		    }
	    }
	    else //not player fired;
	    {
		     Player* hitPlayer2;
		     hitPlayer2 = getWorld()->getCollidingPlayer(this);
		     if(hitPlayer2 != NULL)
		     {
			     hitPlayer2->damage(m_damagePoints,true);
			     setDead();
				 getWorld()->subtractProj();
			     return;
		     }
	    }
	}
}

bool Projectile::playerFired() const
{
	return m_playerFired;
}

//Bullet
Bullet::Bullet(StudentWorld* sw, int startX, int startY, bool playerFired):
Projectile(sw, IID_BULLET, startX, startY, playerFired, 2)
{}

Bullet::~Bullet()
{}

int Bullet::getPlayerFiredSound()
{
	return SOUND_PLAYER_FIRE;
}

//Torpedo
Torpedo::Torpedo(StudentWorld* sw, int startX, int startY, bool playerFired):
Projectile(sw, IID_TORPEDO, startX, startY, playerFired, 8)
{}

Torpedo::~Torpedo()
{}

int Torpedo::getPlayerFiredSound()
{
	return SOUND_PLAYER_TORPEDO;
}

//Goodie
Goodie::Goodie(StudentWorld* sw, int imageID, int startX, int startY):
Actor(sw, imageID, startX, startY)
{
	goodieTickLifetime = ((int(100.0/(getWorld()->getRound())))+30);
	ticksLeft = ((int(100.0/(getWorld()->getRound())))+30);
	m_move = 0;
}

Goodie::~Goodie()
{}

void Goodie::DoSomething()
{
	Player* hitPlayer = getWorld()->getCollidingPlayer(this);
	if(hitPlayer == NULL)
	{
		double rty=((ticksLeft/goodieTickLifetime)+0.2);
		setBrightness(rty);
		if(ticksLeft==0)
		{
			setDead();
			return;
		}
		if(m_move == 1)
		{
				m_move =2;
				ticksLeft--;
				return;
		}
		else if(m_move==2)
		{
				m_move =0;
				ticksLeft--;
				return;
		}
		else if (m_move ==0)
		{
			m_move =1;
			moveTo(getX(),getY()-1);
			if(getY()<0)
			{
				setDead();
				return;
			}
			else 
			{
				Player* hitPlayer2 = getWorld()->getCollidingPlayer(this);
				if(hitPlayer2 != NULL)
				{
					doSpecialAction(hitPlayer2);
					setDead();
					return;
				}
				
			}
			ticksLeft--;
			return;
		}
	}
	else
	{
		doSpecialAction(hitPlayer);
		setDead();
		return;
	}
}

//FreeShip Goodie
FreeShipGoodie::FreeShipGoodie(StudentWorld* sw, int startX, int startY):
Goodie(sw,IID_FREE_SHIP_GOODIE, startX, startY)
{}

FreeShipGoodie::~FreeShipGoodie()
{}

void FreeShipGoodie::doSpecialAction(Player* p)
{
	getWorld()->increaseScore(5000);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	getWorld()->incLives();
}

//Energy Goodie
EnergyGoodie::EnergyGoodie(StudentWorld* sw, int startX, int startY):
Goodie(sw,IID_ENERGY_GOODIE, startX, startY)
{}

EnergyGoodie::~EnergyGoodie()
{}

void EnergyGoodie::doSpecialAction(Player* p)
{
	getWorld()->increaseScore(5000);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	p->restoreEn();
}

//Torpedo Goodie
TorpedoGoodie::TorpedoGoodie(StudentWorld* sw, int startX, int startY):
Goodie(sw,IID_TORPEDO_GOODIE, startX, startY)
{}

TorpedoGoodie::~TorpedoGoodie()
{}

void TorpedoGoodie::doSpecialAction(Player* p)
{	
	getWorld()->increaseScore(5000);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	p->addTorpedoes(5);
}


//HpActor
HpActor::HpActor(StudentWorld* sw, int imageID, int startX, int startY, int startEn):
Actor(sw, imageID, startX, startY)
{
	m_energy = startEn;
	m_startEn = startEn;
}

HpActor::~HpActor()
{
}

int HpActor::getEnergy() const
{
	return m_energy;
}

void HpActor::decreaseEn(int hit)
{
	m_energy -= hit;
}

void HpActor::decreaseAllEn()
{
	m_energy = 0;
}

void HpActor::restoreEn()
{
	m_energy = m_startEn;
}

//Player Ship
Player::Player(StudentWorld* sw):
HpActor(sw, IID_PLAYER_SHIP, VIEW_WIDTH/2, 1, 50)
{
	m_numTorpedoes = 0;
	firedLastTick = false;
}

Player::~Player()
{}

void Player::DoSomething()
{
	vector<Alien*> hitAliens = getWorld()->getCollidingAliens(this);
	if(!hitAliens.empty())
	{
		for(size_t k=0;k<hitAliens.size();k++)
		{
			damage(15,false);
			hitAliens[k]->damage(0,false);
			if(isDead())
		    {
		   	    return;
	     	}
	 	}
	}
	int ch;
	if (getWorld()->getKey(ch))
	{
		if (ch!=KEY_PRESS_SPACE && ch!=KEY_PRESS_TAB)
	    {
		     firedLastTick=false;
	    }
		switch(ch)
		{
		    case KEY_PRESS_LEFT:
				if (getX() > 0)
				{
					moveTo(getX()-1,getY());
				}
				break;
			case KEY_PRESS_RIGHT:
				if (getX() < VIEW_WIDTH-1)
				{
					moveTo(getX()+1,getY());
				}
				break;
			case KEY_PRESS_UP:
				if (getY() < VIEW_HEIGHT-1)
				{
					moveTo(getX(),getY()+1);
				}
				break;
			case KEY_PRESS_DOWN:
				if (getY() > 0)
				{
					moveTo(getX(),getY()-1);
				}
				break;
			case KEY_PRESS_SPACE:
				if(getY()==39)
				{
					firedLastTick=false;
					break;
				}
				if (firedLastTick)
				{
					firedLastTick=false;
					break;
				}
				else
				{
					firedLastTick=true;
					Bullet* tempBullet = new Bullet(getWorld(),getX(),getY()+1,true);
					getWorld()->addActor(tempBullet);
					getWorld()->playSound(tempBullet->getPlayerFiredSound());
				}
				break;
			case KEY_PRESS_TAB:
				if(getY()==39 || m_numTorpedoes ==0)
				{
					firedLastTick=false;
					break;
				}
				if(firedLastTick)
				{
					firedLastTick=false;
					break;
				}
				else
				{
					firedLastTick=true;
					Torpedo* tempTorp = new Torpedo(getWorld(),getX(),getY()+1,true);
					getWorld()->addActor(tempTorp);
					getWorld()->playSound(tempTorp->getPlayerFiredSound());
					m_numTorpedoes--;
				}
				break;
			default: 
				break;
		}
		
	}
	//moved
       if (ch==KEY_PRESS_LEFT || ch==KEY_PRESS_RIGHT || ch==KEY_PRESS_UP || ch==KEY_PRESS_DOWN)
	   {
		    vector<Alien*> hitAliens2 = getWorld()->getCollidingAliens(this);
	        if(!hitAliens2.empty())
    	    {
		        for(size_t k=0;k<hitAliens2.size();k++)
		        {
			        damage(15,false);
			        hitAliens2[k]->damage(0,false);
			        if(isDead())
		         {
		   	         return;
	     	     }
		     }
	        }
	   }
    
}


void Player::damage(int points, bool hitByProjectile)
{
	if(hitByProjectile)
	{
		decreaseEn(points);
		getWorld()->playSound(SOUND_PLAYER_HIT);
	}
	else
	{
		decreaseEn(points);
		getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
	}
	if(getEnergy() <= 0)
	{
		setDead();
		getWorld()->playSound(SOUND_PLAYER_DIE);
	}
}

int Player::getNumTorpedoes() const
{
	return m_numTorpedoes;
}

void Player::addTorpedoes(int n)
{
	m_numTorpedoes += n;
}

//Alien
Alien::Alien(StudentWorld* sw, int imageID, int energy,int worth):
HpActor(sw, imageID, rand()%VIEW_WIDTH, 39, energy)
{
	m_shouldIMove = true;
	m_worth = worth;
}

Alien::~Alien()
{}

void Alien::damage(int points, bool hitByProjectile)
{
	if (hitByProjectile)
		decreaseEn(points);
	else
		decreaseAllEn();
	if(getEnergy() <= 0)
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		setDead();
		if(hitByProjectile)
		{
			getWorld()->increaseScore(m_worth);
			getWorld()->killedAlien();
		    maybeDropGoodie();
		}
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);

}

bool Alien::shouldIMove() const
{
	return m_shouldIMove;
}


void Alien::changeMove()
{
	if (m_shouldIMove)
		m_shouldIMove = false;
	else
		m_shouldIMove = true;
}

void Alien::maybeDropGoodie()
{
	return;
}

//nachlingBase
nachlingBase::nachlingBase(StudentWorld* sw, int ImageID, int energy, int worth):
Alien(sw, ImageID, energy, worth)
{
	m_state=0;
	HMR = 0;
	HMdir =0;
	HMD =0;
}

nachlingBase::~nachlingBase()
{}

int nachlingBase::getState() const
{
	return m_state;
}

void nachlingBase::changeState(int state)
{
	m_state = state;
}

void nachlingBase::DoSomething()
{
	if (shouldIMove())
	{
		switch(getState())
		{	
		    case 0:
				if (getWorld()->PS()->getX() != getX())
				{
					if (rand()%3 == 0)
					{
						if( getWorld()->PS()->getX() > getX())
						{
						    moveTo(getX()+1,getY()-1);
						}
						else 
						{
						    moveTo(getX()-1,getY()-1);
						}
						
					}
					else
					{
						moveTo(getX(),getY()-1);
					}
				}
				else
				{
				   if (getX() != 29 && getX() != 1)
				   { 
					  changeState(1);
					  int MDB = 0;
					  int tempHMD = 0;
					  if ( getX() <= 29-getX())
					  	  MDB = getX();
					  else
						  MDB = 29-getX();
					  if (MDB > 3)
						  tempHMD = (rand()%3+1);
					  else 
						  tempHMD = MDB;
					  if (rand()%2 == 0)
						  HMdir =0;
					  else 
						  HMdir =1;
					  HMR = tempHMD;
					  HMD = tempHMD;
				   }
				   moveTo(getX(),getY()-1);
				}
				break;
			case 1:
				if( getWorld()->PS()->getY() > getY())
					changeState(2);
				else
				{
					if(HMR ==0)
					{
						if(HMdir == 0)
							HMdir = 1;
						else
							HMdir = 0;
						HMR = 2*HMD;
					}
					else 
						HMR--;
					if(HMdir == 0)
						moveTo(getX()-1,getY());
					else
						moveTo(getX()+1,getY());
					//fire bullet
					int chancesofFiring= ((int(10/getWorld()->getRound()))+1);
					if (rand()%chancesofFiring == 0)
					{
						if(getWorld()->getNumAlienProj() < (2*getWorld()->getRound())) //fire bullet
						{
							if(getY()>0)
							{
								getWorld()->addActor(new Bullet(getWorld(),getX(),getY()-1,false));
								getWorld()->addAlienProj();
								getWorld()->playSound(SOUND_ENEMY_FIRE);
							}
						}
					}
					if(rand()%20 == 0)
						changeState(2);
				}
				break;
			case 2:
				if (getY() == 39)
					changeState(0);
				else
			    {
				 	if (getX() ==0)
				    {
					    HMdir = 1;
				    }
				    else if (getX()==29)
				    {
					    HMdir =0;
				    }
				    else
				    {
					    HMdir = rand()%2;
				    }
				    if (HMdir == 1)
					    moveTo(getX()+1,getY()+1);
				    else
					    moveTo(getX()-1,getY()+1);
				}
				break;
			default:
				break;
		}
	}
	changeMove();
	if(getY() <0)
		setDead();
	
}
//nachling
nachling::nachling(StudentWorld* sw, int round):
nachlingBase(sw,IID_NACHLING, int(5*(0.9+(0.1*round))),1000)
{}

nachling::~nachling()
{}

//wealthyNachling
wealthyNachling::wealthyNachling(StudentWorld* sw, int round):
nachlingBase(sw, IID_WEALTHY_NACHLING, int(8*(0.9+(0.1*round))), 1200)
{
	isMal = false;
	numMal = 0;
}

wealthyNachling::~wealthyNachling()
{}

void wealthyNachling::DoSomething()
{
	if(isMal)
	{
		numMal--;
		if (numMal <= 0)
		{
			isMal = false;
		}
		return;
	}
	else
	{
		if (rand()%200 == 0)
		{
			numMal = 30;
			isMal = true;
			return;
		}
		else
			nachlingBase::DoSomething();
	}
}

void wealthyNachling::maybeDropGoodie()
{
	
	if(rand()%3 == 0)
	{
		if(rand()%2 == 0)
			getWorld()->addActor(new EnergyGoodie(getWorld(), getX(), getY()));
		else
		    getWorld()->addActor(new TorpedoGoodie(getWorld(), getX(), getY()));
	}
}


//SmallBot
smallBot::smallBot(StudentWorld* sw, int round):
Alien(sw, IID_SMALLBOT,int(12*(0.9+(0.1*round))), 1500)
{
	justHit = false;
}

smallBot::~smallBot()
{}

void smallBot::DoSomething()
{
	if (shouldIMove())
	{
		if(justHit)
		{
			justHit=false;
			int x=getX();
			if(rand()%3 == 0)
			{
			   if(x == 0)
				   x=1;
			   else if(x == 29)
				   x=28;
			   else if (rand()%2 == 0)
				   x++;
			   else 
				   x--;
			}
			moveTo(x,getY()-1);
		}
		else
			moveTo(getX(), getY()-1);
		//fire
		if(getX() == getWorld()->PS()->getX())
		{
			if(getY() > getWorld()->PS()->getY())
			{
				int q = ((int(100.0/(getWorld()->getRound())))+1);
				if(getWorld()->getNumAlienProj() < (2*(getWorld()->getRound())))
				{
					if(rand()%q == 0)
				    {
					    getWorld()->addActor(new Torpedo(getWorld(),getX(),getY()-1,false));
					    getWorld()->addAlienProj();
						getWorld()->playSound(SOUND_ENEMY_FIRE);
				    }
				    else
					{
					    getWorld()->addActor(new Bullet(getWorld(),getX(),getY()-1,false));
					    getWorld()->addAlienProj();
						getWorld()->playSound(SOUND_ENEMY_FIRE);
				    }
				}
			}
		}

	}
	changeMove();
	if(getY()<0)
		setDead();
}


void smallBot::maybeDropGoodie()
{
	if(rand()%3 == 0)
			getWorld()->addActor(new FreeShipGoodie(getWorld(), getX(), getY()));
}

void smallBot::setHit()
{
	justHit=true;
}