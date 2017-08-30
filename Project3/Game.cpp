// Game.cpp

#include "Game.h"
#include "utilities.h"
#include "dungeon.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
{
    n_goblinSmellDistance = goblinSmellDistance;
    map = new dungeon(goblinSmellDistance);        //create a new map for the game
    //map->setGoblinDistance(goblinSmellDistance); //set the goblin smell distance
    map->addObject();           //add objects to the map
    map->addMonster();          //add monsters to the map
    MoShen = new player(map);   //add player to the map
    map->addPlayer(MoShen);
}

Game::~Game()
{
    delete map;
}

void Game::play()
{
    player* p = map->getPlayer();
    char input;         //get input
    do{
        map->display();
        input = getCharacter();
        switch (input) {
            case 'q':       //quit the game
                return;
            case 'g':       //pick up the object or golden idol
                p->pickObject();
                break;
            case 'i':       //show inventory
            {
                p->showInventory();
                getCharacter(); //back to the dungeon interface
                break;
            }
            case 'w':      //show inventory & wield weapons
            {
                p->showInventory();
                char inputWeapon = getCharacter();
                p->wieldWeapon(inputWeapon);
                break;
            }
            case 'r':   //show inventory & read scrolls
            {
                p->showInventory();
                char readScroll = getCharacter();
                p->readScroll(readScroll);
                break;
            }
            case 'c':   //cheat
                p->cheat();
                break;
            case '>':   //go to the next level
            {
                if (map->isStair(map->getPlayer()->getRow(), map->getPlayer()->getCol()) && map->getLevel() != 4)
                {
                    map->deleteDungeon(map);
                    break;
                }
            }
            case ARROW_DOWN:    //4 moves
            case ARROW_LEFT:
            case ARROW_RIGHT:
            case ARROW_UP:
                p->playerMove(input);
            default:
                break;
        }
        for(list<monster*>::iterator i = map->getMonsterList().begin(); i!=map->getMonsterList().end(); i++)
            (*i)->doSomething(map); //monster moves
    }while(!(p->win()));
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
