#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "actor.h"
#include <vector>
#include<string>



// Students:  Add code to this file, StudentWorld.cpp, actor.h, and actor.cpp

class StudentWorld : public GameWorld
{
public:

	StudentWorld();
	virtual ~StudentWorld();
	virtual void init();
	virtual int move();
	virtual void cleanUp();
	Player* PS();
    int getRound() const;
	int getNumAlienProj() const;
	  // Get a list of aliens at the same location as the actor
    std::vector<Alien*> getCollidingAliens(Actor* a);

      // Get a pointer to the player if at the same location as the actor,
      // otherwise NULL
    Player* getCollidingPlayer(Actor* a);
	void addActor(Actor* a);
	void addAlienProj();
	void subtractProj();
	int getMaxAllowedAliens();
	int numAliens();
	void setDisplayText();
	double getPercentageOfEnergyThePlayerHasLeft();
	std::string getDisplay(int score,int round, double energyPercent, int torpedoes, int shipsLeft);
	void killedAlien();
	bool roundComplete();

private:
	std::vector<Actor*> m_objects;
	Player* m_playerShip;
	int m_round;
	int m_numAlienProj;
	int m_killed;


	
};

#endif // _GAMEWORLD_H_
