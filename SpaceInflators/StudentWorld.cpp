#include "StudentWorld.h"
#include "GameConstants.h"
#include <vector>
#include<string>
#include <iostream> 
#include <sstream> 
#include <iomanip> 
using namespace std;

GameWorld* createStudentWorld()
{
    return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, actor.h and actor.cpp
StudentWorld::StudentWorld()
{
	m_playerShip = NULL;
	m_round = 1;
	m_numAlienProj = 0;
	m_killed=0;
}

StudentWorld::~StudentWorld()
{
    delete m_playerShip;
	vector<Actor*>::iterator it;
	it = m_objects.begin();
	while(it != m_objects.end())
	{
		delete (*it);
		it++;
	}
}

void StudentWorld::init()
{
	m_playerShip = new Player(this);
	m_killed =0;
	m_numAlienProj=0;

}

int StudentWorld::move()
{
	if(testParamsProvided())
	{
		if(!m_objects.empty())
		{;}
		else
		{
			int ch = getTestParam(TEST_PARAM_ACTOR_INDEX);
			switch(ch)
			{
			   case TEST_PARAM_NACHLING:
				   addActor(new nachling(this, m_round));
				   break;
			   case TEST_PARAM_WEALTHY_NACHLING:
				   addActor(new wealthyNachling(this, m_round));
				   break;
			   case TEST_PARAM_SMALLBOT:
				   addActor(new smallBot(this, m_round));
				   break;
			   case TEST_PARAM_GOODIE_ENERGY:
				   addActor(new EnergyGoodie(this,15,39));
				   break;
			   case TEST_PARAM_GOODIE_TORPEDO:
				   addActor(new TorpedoGoodie(this,15,39));
				   break;
			   case TEST_PARAM_GOODIE_FREE_SHIP:
				   addActor(new FreeShipGoodie(this,15,39));
				   break;
			   default:
				   break;
			}

		}
	}
	else
	{
	   //determine what to add
	   //stars
	    if (rand()%3 == 0)
	    {   
			addActor(new Star(this));
		}
		if(numAliens() < getMaxAllowedAliens() && numAliens()<((4*m_round)-m_killed))
		{
			if(rand()%100 <= 69)
			{
				if(rand()%100 <= 19)
				{
					addActor(new wealthyNachling(this,m_round));
				}
				else
				{
					addActor(new nachling(this,m_round));
				}
			}
			else
				addActor(new smallBot(this,m_round));
		}
	}

	//display text
	setDisplayText();


	//Dosomething
	for (size_t k=0; k < m_objects.size();k++)
	{
		 if(!(m_objects[k]->isDead()))
	     {
			 m_objects[k]->DoSomething();
		 }

    }
	if(!(m_playerShip->isDead()))
         m_playerShip->DoSomething();

	//delete dead
	for (size_t k=0; k < m_objects.size();k++)
	{
		 if(m_objects[k]->isDead())
	     {
			 delete m_objects[k];
			 m_objects.erase(m_objects.begin() +k);
		 }

    }

	//check if game over
	//return GWSTATUS_CONTINUE_GAME;
	
		if(m_playerShip->isDead())
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		else
		{
			if(roundComplete())
			{
				m_round++;
				m_killed =0;
			}
	        return GWSTATUS_CONTINUE_GAME;
		}


	
}

void StudentWorld::cleanUp()
{
	    delete m_playerShip;
	    while(!m_objects.empty())
		{
			delete m_objects[0];
			m_objects.erase(m_objects.begin());
		}
		m_killed =0;
	    m_numAlienProj=0;
	
}

Player* StudentWorld::PS()
{
	return m_playerShip;
}

int StudentWorld::getRound() const
{
	return m_round;
}

int StudentWorld::getNumAlienProj() const
{
	return m_numAlienProj;
}


vector<Alien*> StudentWorld::getCollidingAliens(Actor* a)
{
	vector<Alien*> temp;
	for (size_t k=0; k < m_objects.size();k++)
	{
		if(a->getX()==m_objects[k]->getX() && a->getY()==m_objects[k]->getY())
		{
			Actor* aa=m_objects[k];
		    Alien* bb = dynamic_cast<Alien*>(aa);
		    if(bb != NULL)
		    {
			    temp.push_back(bb);
		    }
		}
	}
	return temp;
}

Player* StudentWorld::getCollidingPlayer(Actor* a)
{
	if(a->getX()==m_playerShip->getX() && a->getY()==m_playerShip->getY())
	{
		return m_playerShip;
	}
	else
	    return NULL;
}

void StudentWorld::addActor(Actor* a)
{
	m_objects.push_back(a);
}

void StudentWorld::addAlienProj()
{
	m_numAlienProj++;
}

void StudentWorld::subtractProj()
{
	m_numAlienProj--;
}

int StudentWorld::getMaxAllowedAliens()
{
	return int(2+.5*m_round);
}

int StudentWorld::numAliens()
{
	int t=0;
	for (size_t k=0; k < m_objects.size();k++)
	{
			Actor* aa=m_objects[k];
		    Alien* bb = dynamic_cast<Alien*>(aa);
		    if(bb != NULL)
		    {
			    t++;
		    }
	}
	return t;
	
}

double StudentWorld::getPercentageOfEnergyThePlayerHasLeft()
{
	double a=m_playerShip->getEnergy();
	return a*2;
}

void StudentWorld::setDisplayText()
{
int score = getScore();
int round = m_round;
double energyPercent = getPercentageOfEnergyThePlayerHasLeft();
int torpedoes = m_playerShip->getNumTorpedoes();
int shipsLeft = getLives();
// Next, create a string from your statistics, of the form: 
// ÅgScore: 0000123 Round: 01 Energy: 98% Torpedoes: 003 Ships: 03Åh
// where all numeric values except the Energy value must have leading 
// zeros (e.g., 003 vs. 3), and each
// statistic should be separated from the last statistic by two 
// spaces. E.g., between the Åg0000123Åh of the
// score and the ÅgRÅh in ÅgRoundÅh there must be two spaces. Each field 
// must be exactly as wide as shown,
// e.g., the score must be exactly 7 digits long, the torpedo field 
// must be 3 digits long, except for the 
// Energy field, which could be between 1 and 3 digits (e.g., 5%, 89% 
// or 100%)
string s = getDisplay(score,round, energyPercent, torpedoes, shipsLeft);
// Finally, update the display text at the top of the screen with your 
// newly created stats
setGameStatText(s); // calls GameWorld::setGameStatText
}

string StudentWorld::getDisplay(int score,int round, double energyPercent,int torpedoes,int shipsLeft)
{
	ostringstream k;
	k.fill('0');
	k<<"Score: "<<setw(7)<<score;
	k.fill('0');
	k<<"  Round: "<<setw(2)<<round;
	k.fill(' ');
	k<<"  Energy: "<<setw(2)<<energyPercent;
	k.fill('0');
    k<<"%  Torpedoes: "<<setw(3)<<torpedoes;
	k.fill('0');
	k<<"  Ships: "<<setw(2)<<shipsLeft;
    string temp = k.str();
	return temp;
}

void StudentWorld::killedAlien()
{
	m_killed++;
}

bool StudentWorld::roundComplete()
{
	if (m_killed >= (4*m_round))
		return true;
	else return false;
}