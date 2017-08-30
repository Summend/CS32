// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.

#include "dungeon.h"
#include "actors.h"

class Game
{
public:
	Game(int goblinSmellDistance);
    ~Game();
	void play();
private:
    dungeon* map;
    player* MoShen;
    int n_goblinSmellDistance;
    
};

#endif // GAME_INCLUDED
