
#ifndef ACTORS_INCLUDED
#define ACTORS_INCLUDED

#include <list>
#include <vector>
#include "objects.h"
#include <queue>

using namespace std;

class dungeon;
class actor
{
public:
    //constructor & destructor
    actor(dungeon* dunge, int r, int c);
    actor(dungeon* dunge, int HP, int maxHP, int arm, int str, int dex, int sleep, string name);
    virtual ~actor();
    
    //accessor
    int getRow() const;
    int getCol() const;
    int getHP() const;
    int getArm() const;
    int getStr() const;
    int getDex() const;
    bool getDeath() const;
    int getSleep() const;
    int getMaxHp() const;
    string getName() const;
    weapon* getWeapon() const;
    dungeon* getDungeon() const;
    
    //mutators
    bool setWeapon(weapon* weapon);
    bool setPos(int r, int c);
    void attack(actor* attaker, actor* defender);
    void move(char dir);
    void increaseStr(int a);
    void increaseHP(int a);
    void increaseDex(int a);
    void increaseArm(int a);
    void increaseSleep(int a);
    void decreaseSleep(int a);
    bool sleepCheck();
    void increaseMaxHP(int a);
    void setGrid(int r, int c, actor* a);
    void monsterMove(char dir, actor* a);

    
private:
    dungeon* m_dungeon;
    int m_r;
    int m_c;
    int m_HP;
    int m_max_HP;
    weapon* m_weapon;
    int m_arm;
    int m_str;
    int m_dex;
    int m_sleep;
    bool m_death;
    string m_name;
};

class player: public actor
{
public:
    //constructor and destructor
    player(dungeon* dunge);
    virtual ~player();
    
    //accessors
    list<object*> getInventory() const;
    bool win() const;
    
    //mutators
    void showInventory();
    void pickObject();
    void nextLevel();
    void cheat();
    void playerMove(char dir);
    void useInventoryObjs(char a);
    void wieldWeapon(char a);
    void readScroll(char a);
    void wearWeapon(object* a);
    bool checkObject();
    void ranndomRecovery();

private:
    bool m_win;
    list<object*> inventory;
};

class monster: public actor
{
public:
    //constructor and destructor
    monster(dungeon* m_dunge, int m_HP, int m_maxHP, int m_arm, int m_str, int m_dex, int m_sleep, string m_name);
    virtual ~monster(){};
    
    //mutators
    virtual void doSomething(dungeon* dunge)=0;
    bool smell(dungeon* dunge, int r);
    void monsterDrop(dungeon* dunge, monster* a);
    char chooseDirection(dungeon* dunge, int r, int c);
};

class bogeyman: public monster
{
public:
    //constructor and destructor
    bogeyman(dungeon* dunge);
    virtual ~bogeyman(){};
    
    //mutators
    virtual void doSomething(dungeon* dunge);
    void drop(dungeon* dunge);
};

class snakewoman:public monster
{
public:
    //constructor and destructor
    snakewoman(dungeon* dunge);
    virtual ~snakewoman(){};
    
    //mutators
    virtual void doSomething(dungeon* dunge);
    void drop(dungeon* dunge);
};

class dragon:public monster
{
public:
    //constructor and destructor
    dragon(dungeon* dunge);
    virtual ~dragon(){};
    
    //mutators
    virtual void doSomething(dungeon* dunge);
    void recovery();
};

class goblin:public monster
{
public:
    //constructor and destructor
    goblin(dungeon* dunge);
    virtual ~goblin(){};
    
    //accessor
    int getDistance() const;
    void setDistance();
    
    //mutators
    virtual void doSomething(dungeon* dunge);
    bool goblinSmell(int r, int c, int d);
    char getGoblinDirection();
    bool goblinMovable(int r, int c);
private:
    int goblinsmelldistance;
    char goblinArray[18][70];
    struct point{
        int r;
        int c;
        point(){r = 0; c = 0;}
        point(int r, int c){this->r = r; this->c = c;}
    };
    point pos;
    bool findPath;
    queue<char> direction;
};
#endif // defined ACTORS_INCLUDED

