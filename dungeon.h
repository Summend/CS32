#ifndef DUNGEON_INCLUDED
#define DUNGEON_INCLUDED

#include "actors.h"
#include <list>
#include <vector>

struct rectangle
{
    int x_region;
    int y_region;
    int height;
    int width;
};

class dungeon
{
public:
    //constructor and destructor
    dungeon(int a);
    ~dungeon();
    
    //accessors
    char getGrid(int r, int c) const;
    int getLevel() const;
    player* getPlayer() const;
    object* getStair() const;
    object* getGoldenIdol() const;
    monster* getMonster(int r, int c, monster* a);
    list<object*>& getObjectList();
    list<monster*>& getMonsterList();
    vector<string>& getLogStrings();
    int getGoblinSmell() const;
    
    //mutators
    void setGrid(int r, int c, char symbol);
    void setLevel(int a);
    void addPlayer(player* p);
    void setPlayer();
    void addObject();
    void setObject();
    void addMonster();
    void setMonster();
    void deleteDungeon(dungeon* dunge);
    void setDropItems(dungeon* dunge, object* a);
    bool movable(int r, int c);
    bool isMonster(int r, int c) const;
    bool isPlayer(int r, int c) const;
    bool isStair(int r, int c) const;
    void cleanCorpse();
    void display();
    
private:
    void createMap(int n_regions);
    int level;
    int n_regions;
    int n_objs;
    char grid[18][70];
    player* m_player;
    list<object*> object_list;
    list<monster*> monster_list;
    vector<string> logStrings;
    object* goldenIdol;
    object* stair;
    int n_Goblin_smell_distance;
};

bool recCmp(rectangle r1, rectangle r2);

#endif // defined DUNGEON_INCLUDED 
