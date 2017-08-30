#include "dungeon.h"
#include <iostream>
#include <algorithm>
#include <string>
#include "utilities.h"
#include "objects.h"
#include "actors.h"

using namespace std;


dungeon::dungeon(int a)  //dungeon constructor
{
    n_regions = randInt(3) + 3;     //set number of regions
    createMap(n_regions);           //create map
    n_objs = randInt(2) + 2;        //set number of objects
    level = 0;                      //set the initial level
    n_Goblin_smell_distance = a;
    
}

dungeon::~dungeon() //dungeon destructor
{
    if (level == 4)
        delete goldenIdol;
    else
        delete stair;
    for (list<object*>::iterator i = object_list.begin(); i!= object_list.end(); i++)
        delete *i;
    for (list<monster*>::iterator q = monster_list.begin(); q != monster_list.end(); q++)
        delete *q;  //delete monsters & objects
}

//dungeon accessors

int dungeon::getGoblinSmell() const {return n_Goblin_smell_distance;}

vector<string>& dungeon::getLogStrings() {return logStrings;}

player* dungeon::getPlayer() const {return m_player;}

object* dungeon::getStair() const {return stair;}

object* dungeon::getGoldenIdol() const {return goldenIdol;}

list<object*>& dungeon::getObjectList() {return object_list;}

list<monster*>& dungeon::getMonsterList() {return monster_list;}

char dungeon::getGrid(int r, int c) const {return grid[r][c];}

int dungeon::getLevel() const {return level;}

monster* dungeon::getMonster(int r, int c, monster* a)
{
    //tranverse the monster list
    for (list<monster*>::iterator i = monster_list.begin(); i != monster_list.end(); i++)
    {
        if((*(*i)).getRow() == r && (*(*i)).getCol() == c)
        {
            a = *i;
            return a;
        }
    }
    return a;
}

//dungeon mutators

// 4 checks for the map actors
bool dungeon::movable(int r, int c)
{
    if (grid[r][c] == ' ' || grid[r][c] == ')' || grid[r][c] == '>' || grid[r][c] == '?' || grid[r][c] == '&')
        return true;
    else
        return false;
}

bool dungeon::isMonster(int r, int c) const
{
    if (grid[r][c] == 'B' || grid[r][c] == 'S' || grid[r][c] == 'D' || grid[r][c] == 'G')
    {
        return true;
    }
    else
        return false;
}

bool dungeon::isPlayer(int r, int c) const
{
    if(grid[r][c] == '@' || (r == m_player->getRow() && c == m_player->getCol()))
        return true;
    else
        return false;
}

bool dungeon::isStair(int r, int c) const
{
    int row = getStair()->getRow();
    int col = getStair()->getCol();
    if (r == row && c == col)
        return true;
    else
        return false;
}

//set functions

void dungeon::setGrid(int r, int c, char symbol)
{
    grid[r][c] = symbol;
}

/*void dungeon::setGoblinDistance(int a)
{
    for (list<monster*>::iterator i = monster_list.begin(); i != monster_list.end(); i++)
    {
        if(((*i)->getName() == "Goblin"))
        {
            goblin* check = dynamic_cast<goblin*>(*i);
            if (check != nullptr)
                check->setSmellDistance(a);
        }
    }
}*/

void dungeon::setLevel(int a)
{
    level = a;
}

void dungeon::setPlayer()
{
    int n_r, n_c;
    do{
        n_r = randInt(18);
        n_c = randInt(70);
    } while (getGrid(n_r, n_c) == '#' || getGrid(n_r, n_c) == 'S' || getGrid(n_r, n_c) == 'G' || getGrid(n_r, n_c) == 'D' || getGrid(n_r, n_c) == 'B');
    m_player->setPos(n_r, n_c);
}

void dungeon::setDropItems(dungeon *dunge, object* a)
{
    dunge->getObjectList().push_back(a);
}

void dungeon::setObject()
{
    //tranverse the object list
    for (list<object*>::iterator i = object_list.begin(); i != object_list.end(); i++)
    {
        int m_r, m_c;
        do{
            m_r = randInt(18);
            m_c = randInt(70);      //initialize the position of the object
        } while (getGrid(m_r, m_c) == '#'||getGrid(m_r, m_c) == '>'||getGrid(m_r, m_c) == '?'||getGrid(m_r, m_c) == '&'||getGrid(m_r, m_c) == ')');
        (*(*i)).setPos(m_r,m_c);
        weapon* check = dynamic_cast<weapon*>(*i);  //check the object type
        if (check != nullptr)
            grid[m_r][m_c] = ')';
        else
            grid[m_r][m_c] = '?';   //set grids of weapon and scroll respectively
    }
    int n_r, n_c;
    do{
        n_r = randInt(18);
        n_c = randInt(70);
    } while (getGrid(n_r, n_c) == '#' || getGrid(n_r,n_c) == ')' || getGrid(n_r, n_c) == '?');
    
    if (level != 4)
    {
        stair = new object("stair", n_r, n_c);      ///set the stair
        grid[n_r][n_c] = '>';
    }
    else
    {
        goldenIdol = new object("golden idol", n_r, n_c);   //set the golden idol
        grid[n_r][n_c] = '&';
    }
}

void dungeon::setMonster()
{
    //tranverse the monster list
    for (list<monster*>::iterator i = monster_list.begin(); i != monster_list.end(); i++)
    {
        int m_r,m_c;
        do{
            m_r = randInt(18);
            m_c = randInt(70);
        }while (getGrid(m_r,m_c) == 'B' || getGrid(m_r,m_c) == 'D' || getGrid(m_r,m_c) == 'S' || getGrid(m_r,m_c) == 'G' || getGrid(m_r,m_c) == '@' || getGrid(m_r, m_c) == '#');
        (*(*i)).setPos(m_r, m_c);
        if((*(*i)).getName() == "Goblin")
            grid[m_r][m_c] = 'G';
        else if((*(*i)).getName() == "Snakewoman")
            grid[m_r][m_c] = 'S';
        else if ((*(*i)).getName() == "Bogeyman")
            grid[m_r][m_c] = 'B';
        else if ((*(*i)).getName() == "Dragon")     //set the four kinds of monsters respectively
            grid[m_r][m_c] = 'D';
    }
}

//add functions

void dungeon::addPlayer(player* p)
{
    m_player = p;
    setPlayer();
}
 
void dungeon::addObject()
{
    for (int i = 0; i < n_objs; i++)    //choose random numbers to generate random objects
    {
        
        int objectNum = randInt(8);
        switch (objectNum) {
            case 0:
                object_list.push_back(new strengthScroll());
                break;
            case 1:
                object_list.push_back(new armorScroll());
                break;
            case 2:
                object_list.push_back(new healthScroll());
                break;
            case 3:
                object_list.push_back(new dexterityScroll());
                break;
            case 4:
                object_list.push_back(new shortSword());
                break;
            case 5:
                object_list.push_back(new longSword());
                break;
            case 6:
                object_list.push_back(new mace());
                break;
            case 7:
                object_list.push_back(new teleScroll());
                break;
            default:
                break;
        }
    }
    setObject();
}

void dungeon::addMonster()
{
    int n_monsters = 2 + randInt(5*(level+1));
    for(int i = 0; i < n_monsters; i++)     //generate random numbers to generate random monsters
    {
        if (level == 0 || level == 1)       //generate snakewoman and goblin in level 0 and level 1
        {
            int monster_index = randInt(2);
            switch (monster_index) {
                case 0:
                    monster_list.push_back(new goblin(this));
                    break;
                case 1:
                    monster_list.push_back(new snakewoman(this));
                    break;
                default:
                    break;
            }
        }
        if (level == 2)     //generate snakewoman, goblin, bogeyman in level 2
        {
            int monster_index_2 = randInt(3);
            switch (monster_index_2) {
                case 0:
                    monster_list.push_back(new goblin(this));
                    break;
                case 1:
                    monster_list.push_back(new snakewoman(this));
                    break;
                case 2:
                    monster_list.push_back(new bogeyman(this));
                    break;
                default:
                    break;
            }
        }
        if (level == 3 || level == 4)   //generate all monsters in level 3 and 4
        {
            int monster_index_3 = randInt(4);
            switch (monster_index_3) {
                case 0:
                    monster_list.push_back(new goblin(this));
                    break;
                case 1:
                    monster_list.push_back(new snakewoman(this));
                    break;
                case 2:
                    monster_list.push_back(new bogeyman(this));
                    break;
                case 3:
                    monster_list.push_back(new dragon(this));
                    break;
                default:
                    break;
            }
        }
    }
    setMonster();
}

//clean monsters' corpse
void dungeon::cleanCorpse()
{
    for (list<monster*>::iterator i = monster_list.begin(); i != monster_list.end();)
    {
        if((*i)->getDeath() == true)
        {
            grid[(*i)->getRow()][(*i)->getCol()] = ' ';
            i = monster_list.erase(i);
        }
        else
            i++;
    }
}

//delete the old dungeon and set the new dungeon in the new level
void dungeon::deleteDungeon(dungeon* dunge)
{
    level++;
    n_regions = randInt(3) + 3;
    createMap(n_regions);
    addPlayer(dunge->m_player);
    n_objs = randInt(2) + 2;
    monster_list.clear();
    object_list.clear();
    addMonster();
    addObject();
}

//display function
void dungeon::display()
{
    clearScreen();
    cleanCorpse();
    
    if(level != 4)
        grid[stair->getRow()][stair->getCol()] = '>';
    else
        grid[goldenIdol->getRow()][goldenIdol->getCol()] = '&'; //display the golden idol or the stair
    
    for (list<object*>::iterator i = object_list.begin(); i != object_list.end(); i++)
    {
        int n_r = (*i)->getRow();
        int n_c = (*i)->getCol();
        weapon* check = dynamic_cast<weapon*>(*i);
        if (check != nullptr)
            grid[n_r][n_c] = ')';
        else
            grid[n_r][n_c] = '?';
    }                               //display the objects
    
    for (list<monster*>::iterator i = monster_list.begin(); i != monster_list.end(); i++)
    {
        int m_r = (*i)->getRow();
        int m_c = (*i)->getCol();
        if((*(*i)).getName() == "Goblin")
            grid[m_r][m_c] = 'G';
        else if((*(*i)).getName() == "Snakewoman")
            grid[m_r][m_c] = 'S';
        else if ((*(*i)).getName() == "Bogeyman")
            grid[m_r][m_c] = 'B';
        else if ((*(*i)).getName() == "Dragon")
            grid[m_r][m_c] = 'D';
    }                               //display the mosnters
    
    if (m_player != nullptr)
        grid[m_player->getRow()][m_player->getCol()] = '@'; //display the player
    
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 70; j++)
        {
            cout << grid[i][j];
        }
        cout << endl;   //display the map
    }
    
    cout << "Dungeon Level: " << level << ", Hit Points: ";
    cout << m_player->getHP() << ", Armor: " << m_player->getArm() << ", Strength: " << m_player->getStr() << ", Dexterity: "  << m_player->getDex() << endl;   //the player information
    cout << endl;
    for(unsigned long i = 0; i < logStrings.size(); i++)
    {
        cout << logStrings[i] << endl;  //display the strings
    }
    
    if (getPlayer()->getDeath())    //see if the player is dead
        return;
    logStrings.clear();
}

//create map functions
bool recCmp(rectangle r1, rectangle r2)
{
    return r1.x_region > r2.x_region;   //compare rectangles to sort the regions
}

void dungeon::createMap(int n_regions)
{
    for (int r = 0; r < 18; r++)
        for (int w = 0; w < 70; w++)
            grid[r][w] = '#';
    
    rectangle* regions = new rectangle[n_regions];  //create new rectangle list
    for (int k  = 0; k < n_regions; k++)
    {
        regions[k].width = randInt(9) + 5;
        regions[k].height = randInt(6) + 5;
        regions[k].x_region = randInt(64 - regions[k].width) + 3;
        regions[k].y_region = randInt(12 - regions[k].height) + 3;  //set the coords and width and length
        bool check = false;
        for (int w = 0; w < k; w++)
        {
            int x_max = regions[w].x_region + regions[w].width;
            int y_max = regions[w].y_region + regions[w].height;
            if (regions[k].x_region > x_max+1 || regions[k].x_region + regions[k].width < regions[w].x_region-1 || regions[k].y_region > y_max+1 || regions[k].y_region + regions[k].height < regions[w].y_region-1)
            {
                check = true;
                if (w == k-1)
                    break;
            }
            else                            //to check the regions are not overlapping
            {
                check = false;
            }
            if (check == false && k > 0)
                k--;
            else
                continue;
        }
    }
    
    for (int k  = 0; k < n_regions; k++)
    {
        for (int h = 0; h < regions[k].height; h++)
            for (int w = 0; w < regions[k].width; w++)
                grid[regions[k].y_region + h][regions[k].x_region + w] = ' ';   //set the regions as ' '
    }
    
    sort(regions, regions + n_regions, recCmp); //sort the regions in order
    
    for (int i  = 0; i < n_regions-1; i++)  //create corridors between regions
    {
        int x_1 = randInt(regions[i].width) + regions[i].x_region;
        int y_1 = randInt(regions[i].height) + regions[i].y_region;
        int x_2 = randInt(regions[i+1].width) + regions[i+1].x_region;
        int y_2 = randInt(regions[i+1].height) + regions[i+1].y_region;
        if (x_1 <= x_2 && y_1 <= y_2)
        {
            for(int i = x_1; i <= x_2; i++)
                grid[y_1][i] = ' ';
            for(int i = y_1; i <= y_2; i++)
                grid[i][x_2] = ' ';
        }
        if (x_1 > x_2 && y_1 > y_2)
        {
            for(int i = x_2; i <= x_1; i++)
                grid[y_2][i] = ' ';
            for(int i = y_2; i <= y_1; i++)
                grid[i][x_1] = ' ';
        }
        if (x_1 <= x_2 && y_1 > y_2)
        {
            for(int i = x_1; i <= x_2; i++)
                grid[y_1][i] = ' ';
            for(int i = y_2; i <= y_1; i++)
                grid[i][x_2] = ' ';
        }
        if (x_1 > x_2 && y_1 <= y_2)
        {
            for(int i = x_2; i <= x_1; i++)
                grid[y_2][i] = ' ';
            for(int i = y_1; i <= y_2; i++)
                grid[i][x_1] = ' ';
        }
    }
    delete[] regions;
}
