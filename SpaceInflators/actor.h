#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Player;

class Actor:public GraphObject
{
   public:
	  Actor(StudentWorld* sw, int imageID, int startX, int startY);
	  virtual ~Actor();
	  virtual void DoSomething()=0;
	  bool isDead() const; //check if dead
	  void setDead();   //set to dead
	  StudentWorld* getWorld(); //returns studentworld pointer
   private:
	   StudentWorld* m_sw;
	   bool m_dead;
};

class Star:public Actor   //STARS
{
   public:
	   Star(StudentWorld* sw);
	   virtual ~Star();
	   virtual void DoSomething();
   private:
};

class Projectile : public Actor
{
    public:
        Projectile(StudentWorld* sw, int imageID, int startX, int startY, bool playerFired, int damagePoints);
		virtual ~Projectile();
        virtual void DoSomething(); 
        bool playerFired() const; //returns true if fired by player
        virtual int getPlayerFiredSound() = 0; //returns the sound to be played when a certain projectile is fired
    private:
	    bool m_playerFired;
		int m_damagePoints;
};

class Bullet:public Projectile //SEPTIC BULLET
{
    public:
        Bullet(StudentWorld* sw, int startX, int startY, bool playerFired);
    	virtual ~Bullet();
        virtual int getPlayerFiredSound();
    private:
};

class Torpedo:public Projectile  //TORPEDO
{
    public:
        Torpedo(StudentWorld* sw, int startX, int startY, bool playerFired);
	    virtual ~Torpedo();
        virtual int getPlayerFiredSound();
    private:
};

class Goodie:public Actor
{
    public:
        Goodie(StudentWorld* sw, int imageID, int startX, int startY);
		virtual ~Goodie();
        virtual void DoSomething();
        virtual void doSpecialAction(Player* p) = 0; //perform special action when a player is on the same spot
     private:
	     double goodieTickLifetime;
		 double ticksLeft;
		 int m_move; //move is 0, otherwise increment to 1 or 2

};

class FreeShipGoodie:public Goodie //FREESHIPGOODIE
{
    public:
        FreeShipGoodie(StudentWorld* sw, int startX, int startY);
		virtual ~FreeShipGoodie();
        virtual void doSpecialAction(Player* p);
};

class EnergyGoodie:public Goodie //ENERGYGOODIE
{
    public:
        EnergyGoodie(StudentWorld* sw, int startX, int startY);
		virtual ~EnergyGoodie();
        virtual void doSpecialAction(Player* p);
};

class TorpedoGoodie:public Goodie //TORPEDOGOODIE
{
    public:
        TorpedoGoodie(StudentWorld* sw, int startX, int startY);
        virtual ~TorpedoGoodie();
		virtual void doSpecialAction(Player* p);
};

class HpActor:public Actor
{
    public:
		HpActor(StudentWorld* sw, int imageID, int startX, int startY, int startEn);
		virtual ~HpActor();
		virtual void DoSomething()=0;
		int getEnergy() const; 
		void decreaseEn(int hit); //decrease health by certain amount
		void decreaseAllEn(); //decrease all health
		void restoreEn(); //restore health to origional
    private:
		int m_energy;
		int m_startEn;
};

class Player:public HpActor //PLAYERSHIP
{
   public:
	  Player(StudentWorld* sw);
	  virtual ~Player();
	  virtual void DoSomething();
	  void damage(int points, bool hitByProjectile); 
	  int getNumTorpedoes() const; //get number of torpedoes player has
	  void addTorpedoes(int n); //add n number of torpedoes
   private:
	   int m_numTorpedoes;
	   bool firedLastTick;
};

class Alien:public HpActor
{
    public:
		Alien(StudentWorld* sw, int imageID, int energy, int worth);
		virtual ~Alien();
		virtual void DoSomething()=0;
		virtual void damage(int points, bool hitByProjectile);
		bool shouldIMove() const; //check to see if alien should move 
		void changeMove(); //called every tick so alien moves every other tick
		virtual void maybeDropGoodie();
	private:
		bool m_shouldIMove;
		int m_worth;
};

class nachlingBase:public Alien
{
    public:
	    nachlingBase(StudentWorld* sw, int ImageID, int energy, int worth);
        virtual ~nachlingBase();
		int getState() const; //gets the state of the nachling
		void changeState(int state); //changes the nachling state
		virtual void DoSomething();
    private:
		int m_state;
		int HMR;
		int HMD;
		int HMdir; //0=left, 1=right
};

class nachling:public nachlingBase
{
    public:
	    nachling(StudentWorld* sw, int round);
	    virtual ~nachling();
	private:
};

class wealthyNachling:public nachlingBase
{
    public:
	    wealthyNachling(StudentWorld* sw,int round);
        virtual ~wealthyNachling();
		virtual void DoSomething();
		virtual void maybeDropGoodie();
	private:
		bool isMal;
		int numMal;
};

class smallBot:public Alien
{
	public:
		smallBot(StudentWorld* sw, int round);
		virtual ~smallBot();
		virtual void DoSomething();
        virtual void maybeDropGoodie();
		void setHit(); //sets the smallBot to just hit if it was hit by a bullet
    private:
		bool justHit;
};

#endif // _ACTOR_H_