#include "actors.h"
#include "dungeon.h"
#include "utilities.h"
#include <iostream>

using namespace std;

///////////////////////////////
//Actor Class Implementations//
//////////////////////////////

//actor constructor and destructor
actor::actor(dungeon* dunge, int r, int c)
{
    m_dungeon = dunge;
    m_r = r;
    m_c = c;
}

actor::actor(dungeon* dunge, int HP, int maxHP, int arm, int str, int dex, int sleep, string name)
{
    m_dungeon = dunge;
    m_HP = HP;
    m_max_HP = maxHP;
    m_arm = arm;
    m_str = str;
    m_dex = dex;
    m_sleep = sleep;
    m_death = false;
    m_name = name;
}

actor::~actor() {delete m_weapon;}

//actor accessors
bool actor::getDeath() const {return m_death;}
int actor::getCol() const {return m_c;}
int actor::getRow() const {return m_r;}
int actor::getHP() const {return m_HP;}
int actor::getArm() const {return m_arm;}
int actor::getStr() const {return m_str;}
int actor::getDex() const {return m_dex;}
weapon* actor::getWeapon() const {return m_weapon;}
string actor::getName() const {return m_name;}
int actor::getSleep() const {return m_sleep;}
dungeon* actor::getDungeon() const {return m_dungeon;}
int actor::getMaxHp() const {return m_max_HP;}

//change member value functions

void actor::decreaseSleep(int a) {m_sleep = m_sleep - a;}
void actor::increaseStr(int a) {m_str = m_str + a;}
void actor::increaseHP(int a)
{
    if (m_HP <= m_max_HP - a)
        m_HP = m_HP + a;
    if (m_HP > m_max_HP - a && m_HP < m_max_HP)
        m_HP = m_max_HP;
    if (m_HP > m_max_HP)
        return;
}
void actor::increaseDex(int a) {m_dex = m_dex + a;}
void actor::increaseArm(int a) {m_arm = m_arm + a;}
void actor::increaseSleep(int a) {m_sleep = m_sleep + a;}
void actor::increaseMaxHP(int a) {m_max_HP = m_max_HP + a;}
bool actor::setWeapon(weapon* weapon)
{
    m_weapon = weapon;
    return true;
}

bool actor::setPos(int r, int c)
{
    m_r = r;
    m_c = c;
    return true;
}



//attack function
void actor::attack(actor* attacker, actor* defender)
{
    int attackerPoint = attacker->m_dex + attacker->m_weapon->getDexBonus();
    int defenderPoint = defender->m_dex + defender->m_arm;
    int a = randInt(attackerPoint);
    int b = randInt(defenderPoint);
    if (a >= b)
        //whether it can attack or not
    {
        int damage = randInt(attacker->m_str + attacker->m_weapon->getStrAmt());
        defender->m_HP = defender->m_HP - damage;   //generate the damage amount
        if (defender->m_HP > 0 && attacker->getWeapon()->getName() != "magic fangs of sleep")
            //if the defender is still alive && attacker's weapon is not magic fang
        {
            getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + " and hits.");
            //store the string value
        }
        else
        {
            if (defender->m_HP <= 0)    //if the defender is dead
            {
                defender->m_death = true;
                monster* check = dynamic_cast<monster*>(defender);
                if(check != nullptr)
                {
                    check->monsterDrop(defender->getDungeon(), check);
                    getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + " dealing a final blow.");    //store the string value
                }
                else
                {
                    player* check = dynamic_cast<player*>(defender);
                    if (check != nullptr)
                    {
                        getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + " dealing a final blow.");    //the player is dead
                        getDungeon()->getLogStrings().push_back("Press q to exit game.");   //cout string
                    }
                }
            }
            if (defender->m_HP > 0 && attacker->getWeapon()->getName() == "magic fangs of sleep")
                //is the defender is still alive and the attacker's weapon is magic fang
            {
                bool sleep = trueWithProbability(1.0/5);
                if(sleep && defender->getSleep() == 0)
                {
                    defender->increaseSleep(randInt(5)+2);
                    getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and hits, putting " + defender->getName() +" to sleep.");
                    //store the value
                }
                else if (sleep && defender->getSleep() > 0) //when the defender is already asleep
                {
                    int sleepTime = max(defender->getSleep(), randInt(5)+2);    //generate the sleeptime
                    defender->increaseSleep(sleepTime - defender->getSleep());
                    getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and hits, putting " + defender->getName() +" to sleep.");
                    //store the string value
                }
                else if (!sleep)
                {
                    getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and hits.");
                    //store the string value
                }
            }
        }
    }
    else if(a < b && attacker->getWeapon()->getName() != "magic fangs of sleep")
    {
        getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " " + attacker->getWeapon()->getName() + " at " + defender->getName() + " and misses.");
    }
    else if(a < b && attacker->getWeapon()->getName() == "magic fangs of sleep")
    {
        getDungeon()->getLogStrings().push_back("the " + attacker->getName() + " " + attacker->getWeapon()->getAction() + " magic fangs at " + defender->getName() + " and misses.");
    }
}
//setGrid after move
void setGrid(int r, int c, actor* a)
{
    if(a->getName() == "Player")
        a->getDungeon()->setGrid(r, c, '@');
    else if (a->getName() == "Snakewoman")
        a->getDungeon()->setGrid(r, c, 'S');
    else if (a->getName() == "Bogeyman")
        a->getDungeon()->setGrid(r, c, 'B');
    else if(a->getName() == "Snakewoman")
        a->getDungeon()->setGrid(r, c, 'S');
    else if (a->getName() == "Dragon")
        a->getDungeon()->setGrid(r, c, 'D');
}
//move function

void actor::move(char dir)
{
    switch (dir) {
        case ARROW_LEFT:
            if(m_dungeon->movable(m_r, m_c - 1))
            {
                m_dungeon->setGrid(m_r, m_c, ' ');
                m_c--;
                //setGrid(m_r, m_c-1, this);
            }
            break;
        case ARROW_RIGHT:
            if(m_dungeon->movable(m_r, m_c+1))
            {
                m_dungeon->setGrid(m_r, m_c, ' ');
                m_c++;
                //setGrid(m_r, m_c+1, this);
            }
            break;
        case ARROW_DOWN:
            if (m_dungeon->movable(m_r + 1, m_c))
            {
                m_dungeon->setGrid(m_r, m_c, ' ');
                m_r++;
                //setGrid(m_r+1, m_c, this);
            }
            break;
        case ARROW_UP:
            if (m_dungeon->movable(m_r - 1, m_c))
            {
                m_dungeon->setGrid(m_r, m_c, ' ');
                m_r--;
                //setGrid(m_r-1, m_c, this);
            }
            break;
        default:
            break;
    }
    this->getDungeon()->display();
}

////////////////////////////////
//Player Class Implementations//
///////////////////////////////

//player constructor and destructor

player::player(dungeon* dunge):actor(dunge,20,20,2,2,2,0,"Player")
{
    weapon* initialWeapon = new shortSword();
    actor::setWeapon(initialWeapon);
    inventory.push_back(initialWeapon);
    m_win = false;
}

player::~player()
{
    for (list<object*>::iterator i = inventory.begin(); i!=inventory.end();i++)
        delete *i;
}

//player accessors

bool player::win() const {return m_win;}

list<object*> player::getInventory() const {return inventory;}

//player move function
void player::playerMove(char dir)
{
    if (getDeath()) //check if the player is dead
        return;
    
    if (getSleep() > 0) //check if the player is asleep
    {
        decreaseSleep(1);
        return;
    }
    ranndomRecovery();      //random recovery for the player
    int p_r = getRow();
    int p_c = getCol();
    dungeon* now = getDungeon();
    switch (dir) {
        case ARROW_LEFT:
            if(now->isMonster(p_r, p_c - 1))
            {
                monster* a;
                a = now->getMonster(p_r, p_c - 1, a);
                attack(this, a);    //attack if the monster is attackable
            }
            else
                actor::move(dir);   //otherwise just move
            break;
        case ARROW_RIGHT:
            if(now->isMonster(p_r, p_c + 1))
            {
                monster* b;
                b = now->getMonster(p_r, p_c + 1, b);
                attack(this, b);
            }
            else
                actor::move(dir);
            break;
        case ARROW_UP:
            if(now->isMonster(p_r - 1, p_c))
            {
                monster* c;
                c = now->getMonster(p_r - 1, p_c, c);
                attack(this, c);
            }
            else
                actor::move(dir);
            break;
        case ARROW_DOWN:
            if(now->isMonster(p_r + 1, p_c))
            {
                monster* d;
                d = now->getMonster(p_r + 1, p_c, d);
                attack(this, d);
            }
            else
                actor::move(dir);
            break;
        default:
            break;
    }
}

//show inventory
void player::showInventory()
{
    if(getDeath())
        return;
    if (getSleep() > 0)
    {
        decreaseSleep(1);
        return;
    }
    clearScreen();
    cout << "Inventory:" << endl;
    char symbol = 'a';
    for (list<object*>::iterator i = inventory.begin(); i != inventory.end(); i++, symbol++)
        cout << " " << symbol << ". " << (*i)->getName() << endl;   //display all objects in the inventory
}

//check objects
bool player::checkObject()
{
    list<object*> check = getDungeon()->getObjectList();
    for (list<object*>::iterator i = check.begin(); i != check.end(); i++)
    {
        int r = (*i)->getRow();
        int c = (*i)->getCol();
        if (r == getRow() && c == getCol())
            return true;
    }
    return false;
}

//pickup objects
void player::pickObject()
{
    if (getDungeon()->getGoldenIdol() != nullptr && getRow() == getDungeon()->getGoldenIdol()->getRow() && getCol() == getDungeon()->getGoldenIdol()->getCol())
    {
        m_win = true;
        cout << "You pick up the golden idol" << endl
        << "Congratulations, you won!" << endl;
        return;
    }               //if the object going to be picked up is golden idol, then win
    
    if(!checkObject())
    {
        return;
    }               //if there is no object to be picked up
    else
    {
        if(inventory.size() > 25)   //if the bag is full
        {
            getDungeon()->getLogStrings().push_back("Your knapsack is full; you can't pick that up.");
            return;
        }
        for (list<object*>::iterator i = getDungeon()->getObjectList().begin(); i != getDungeon()->getObjectList().end(); i++)
        {
            if(getRow() == (*i)->getRow() && getCol() == (*i)->getCol())
            {
                inventory.push_back(*i);
                weapon* check = dynamic_cast<weapon*>(*i);
                if (check != nullptr)   //if the object is a weapon
                {
                    if((*i)->getName() == "short sword")
                        getDungeon()->getLogStrings().push_back("You pick up a short sword");
                    else if ((*i)->getName() == "long sword")
                        getDungeon()->getLogStrings().push_back("You pick up a long sword");
                    else if ((*i)->getName() == "mace")
                        getDungeon()->getLogStrings().push_back("You pick up a mace");
                    else if ((*i)->getName() == "magic fangs of sleep")
                        getDungeon()->getLogStrings().push_back("You pick up a magic fang of sleep");
                    else if ((*i)->getName() == "magic axe")
                        getDungeon()->getLogStrings().push_back("You pick up a magic axe");
                    getDungeon()->getObjectList().erase(i);
                    break;
                }
            
                scroll* check2 = dynamic_cast<scroll*>(*i);
                if (check2 != nullptr)  //if the object is a scroll
                {
                    if((*i)->getName() == "strength scroll")
                        getDungeon()->getLogStrings().push_back("You pick up a scroll called strength scroll");
                    else if((*i)->getName() == "scroll of enhance armor")
                        getDungeon()->getLogStrings().push_back("You pick up a scroll called scroll of enhance armor");
                    else if((*i)->getName() == "scroll of enhance health")
                        getDungeon()->getLogStrings().push_back("You pick up a scroll called scroll of enhance health");
                    else if ((*i)->getName() == "scroll of enhance dexterity")
                        getDungeon()->getLogStrings().push_back("You pick up a scroll called scroll of enhance dexterity");
                    else if ((*i)->getName() == "scroll of teleportation")
                        getDungeon()->getLogStrings().push_back("You pick up a scroll called scroll of teleportation");
                    getDungeon()->getObjectList().erase(i);
                    break;
                }
            }
        }
    }
}

//readScroll function
void player::readScroll(char a)
{
    int num_obj = a - 'a';
    int w = 0;
    for(list<object*>::iterator i = inventory.begin(); i != inventory.end(); i++)
    {
        if (w > num_obj)
            break;
        if (w == num_obj)
        {
            scroll* check = dynamic_cast<scroll*>(*i);//if the object is a scroll
            if (check != nullptr)
            {
                string s_name = (*i)->getName();
                if (s_name == "strength scroll")
                    actor::increaseStr(randInt(3)+1);
                else if (s_name == "scroll of enhance armor")
                    actor::increaseArm(randInt(3)+1);
                else if (s_name == "scroll of enhance health")
                    actor::increaseHP(randInt(6)+3);
                else if (s_name == "scroll of enhance dexterity")
                    actor::increaseDex(1);
                else if (s_name == "scroll of teleportation")
                {
                    dungeon* now = getDungeon();
                    int t_r, t_c;
                    do{
                        t_r = randInt(18);
                        t_c = randInt(70);  //teleportation of the player
                    }while(now->getGrid(t_r, t_c) == '@' || now->getGrid(t_r, t_c) == '#' || now->getGrid(t_r, t_c) == 'B' || now->getGrid(t_r, t_c) == 'S' || now->getGrid(t_r, t_c) == 'D' || now->getGrid(t_r, t_c) == 'G');
                    this->getDungeon()->setGrid(this->getRow(), this->getCol(), ' ');
                    this->setPos(t_r, t_c);
                }
                getDungeon()->getLogStrings().push_back("You read the scroll called " + (*i)->getName());
                getDungeon()->getLogStrings().push_back((*i)->getAction());
                inventory.erase(i); //after reading, delete the scroll
                break;
            }
            else    //if the object is not a scroll
            {
                getDungeon()->getLogStrings().push_back("You can't read a "+(*i)->getName());
                break;
            }
        }
        else
            w++;
    }
}

//wield weapon function
void player::wieldWeapon(char a)
{
    int num_obj = a - 'a';
    int w = 0;
    for(list<object*>::iterator i = inventory.begin(); i != inventory.end(); i++)
    {
        if (w > num_obj)
            break;
        if (w == num_obj)
        {
            weapon* check = dynamic_cast<weapon*>(*i);
            if (check != nullptr) //if the object is a weapon
            {
                string w_name = (*i)->getName();
                if (w_name == "short sword")
                    actor::setWeapon(new shortSword());
                else if (w_name == "long sword")
                    actor::setWeapon(new longSword());
                else if (w_name == "mace")
                    actor::setWeapon(new mace());
                else if (w_name == "magic axe")
                    actor::setWeapon(new magicAxe());
                else if (w_name == "magic fangs of sleep")
                    actor::setWeapon(new magicFang());
                getDungeon()->getLogStrings().push_back("You are wielding " + (*i)->getName());
                break;
            }
            else    //if the object is not a weapon
            {
                getDungeon()->getLogStrings().push_back("You can't wield " + (*i)->getName());
                break;
            }
        }
        else
            w++;
    }
}

//cheat function
void player::cheat()
{
    actor::increaseHP(50 - getHP());
    actor::increaseMaxHP(50 - getMaxHp());
    actor::increaseStr(9 - getStr());
}

//recovery function
void player::ranndomRecovery()
{
    bool p_recovery = trueWithProbability(1.0/10);
    if (p_recovery && getHP() < getMaxHp())
    {
        actor::increaseHP(1);
    }
}

/////////////////////////////////
//Monster Class Implementations//
////////////////////////////////

//monster class constructor
monster::monster(dungeon* m_dunge, int m_HP, int m_maxHP, int m_arm, int m_str, int m_dex, int m_sleep, string m_name):actor(m_dunge,m_HP,m_maxHP,m_arm,m_str,m_dex,m_sleep,m_name){}

//monster smell function
bool monster::smell(dungeon *dunge, int r)
{
    int s_r = getRow();
    int s_c = getCol();
    int p_r = dunge->getPlayer()->getRow();
    int p_c = dunge->getPlayer()->getCol();
    int c_r = max(p_r,s_r);
    int d_r = min(p_r,s_r);
    int c_c = max(p_c,s_c);
    int d_c = min(p_c,s_c);
    int n_moves_to_player = c_r - d_r + (c_c - d_c);
    if (n_moves_to_player <= r)
        return true;
    return false;
}

//monster drop function
void monster::monsterDrop(dungeon *dunge, monster *a)
{
    int r = a->getRow();
    int c = a->getCol();
    if(a->getDeath() == true && dunge->getGrid(r, c) != '(' && dunge->getGrid(r, c) != '?' && dunge->getGrid(r, c) != '>' && dunge->getGrid(r, c) != '@')
    {
        if (a->getName() == "Snakewoman")   //if it is a snakewoman
        {
            bool dropFang = trueWithProbability(1.0/3);
            if(dropFang)
            {
                dunge->getObjectList().push_back(new magicFang(r,c));
            }
        }
        else if (a->getName() == "Bogeyman") //if it is a bogeyman
        {
            bool dropMagicAxe = trueWithProbability(1.0/10);
            if(dropMagicAxe)
            {
                dunge->getObjectList().push_back(new magicAxe(r,c));
            }
        }
        else if(a->getName() == "Dragon")    //if it is a dragon
        {
            int p_scroll = randInt(5);  //randomly generate a scroll
            switch (p_scroll) {
                case 0:
                    dunge->getObjectList().push_back(new strengthScroll(r,c));
                    break;
                case 1:
                    dunge->getObjectList().push_back(new armorScroll(r,c));
                    break;
                case 2:
                    dunge->getObjectList().push_back(new healthScroll(r,c));
                    break;
                case 3:
                    dunge->getObjectList().push_back(new dexterityScroll(r,c));
                    break;
                case 4:
                    dunge->getObjectList().push_back(new teleScroll(r,c));
                    break;
                default:
                    break;
            }
        }
        else if (a->getName() == "Goblin")  //if it is a goblin
        {
            bool goblinDrop = trueWithProbability(1.0/3);
            if(goblinDrop)
            {
                bool dropAxe = trueWithProbability(0.5);
                if(dropAxe)
                    dunge->getObjectList().push_back(new magicAxe(r,c));
                else
                    dunge->getObjectList().push_back(new magicFang(r,c));
            }
        }
    }
}

char monster::chooseDirection(dungeon* dunge, int r, int c)
{
    int p_r = r;
    int p_c = c;
    int d_row = dunge->getPlayer()->getRow() - r;
    int d_col = dunge->getPlayer()->getCol() - c;
    char dir = '\0';
    if (d_row > 0 && d_col >= 0)
    {
        if (d_row >= d_col && d_col != 0)
        {
            if(!dunge->movable(p_r, p_c+1))
                dir = ARROW_DOWN;
            else
                dir = ARROW_RIGHT;
        }
        else
        {
            if(!dunge->movable(p_r+1, p_c))
                dir =ARROW_RIGHT;
            else
                dir = ARROW_DOWN;
        }
    }
    else if (d_row >= 0 && d_col < 0)
    {
        if(d_row >= (-d_col) || d_row == 0)
        {
            if (!dunge->movable(p_r, p_c-1))
                dir = ARROW_DOWN;
            else
                dir = ARROW_LEFT;
        }
        else
        {
            if(!dunge->movable(p_r+1, p_c))
                dir =ARROW_LEFT;
            else
                dir = ARROW_DOWN;
        }
    }
    else if (d_row < 0 && d_col <= 0 )
    {
        if((-d_row) >= (-d_col) && d_col != 0)
        {
            if (!dunge->movable(p_r, p_c-1))
                dir = ARROW_UP;
            else
                dir = ARROW_LEFT;
        }
        else
        {
            if(!dunge->movable(p_r-1, p_c))
                dir =ARROW_LEFT;
            else
                dir = ARROW_UP;
        }
    }
    else if (d_row <= 0 && d_col > 0)
    {
        if((-d_row) >= d_col  || d_row == 0)
        {
            if (!dunge->movable(p_r, p_c+1))
                dir = ARROW_UP;
            else
                dir = ARROW_RIGHT;
        }
        else
        {
            if(!dunge->movable(p_r-1, p_c))
                dir =ARROW_RIGHT;
            else
                dir = ARROW_UP;
        }
    }
    return dir;
}

///////////////////////////////////
//Bogeyman Class Implementations//
//////////////////////////////////

//bogeyman constructor
bogeyman::bogeyman(dungeon* dunge):monster(dunge,randInt(6)+5,randInt(6)+5,2,randInt(1)+2,randInt(1)+2,0,"Bogeyman")
{
    weapon* bogeyWeapon = new shortSword();
    actor::setWeapon(bogeyWeapon);
}

//bogeyman doSomething function
void bogeyman::doSomething(dungeon* dunge)
{
    if (getSleep() > 0) //if bogeyman is asleep
    {
        decreaseSleep(1);
        return;
    }
    
    if (getDeath() || dunge->getPlayer()->getDeath())   //if bogeyman is dead
    {
        return;
    }
    if(monster::smell(dunge,5)) //check if the monser can move or not
    {
        int p_r = getRow();
        int p_c = getCol();
        char dir = monster::chooseDirection(dunge, p_r, p_c);
                                                            //choose the direction to move
        switch (dir) {
            case ARROW_LEFT:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());   //attack
                }
                else
                {
                    actor::move(dir);   //move
                }
                break;
            case ARROW_RIGHT:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                    
                }
                break;
            case ARROW_UP:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_DOWN:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            default:
                break;
        }
    }
}

////////////////////////////////////
//Snakewoman Class Implementations//
///////////////////////////////////

//snakewoman constructor
snakewoman::snakewoman(dungeon* dunge):monster(dunge,randInt(4)+3,randInt(4)+3,3,2,3,0,"Snakewoman")
{
    weapon* snakeWeapon = new magicFang();
    actor::setWeapon(snakeWeapon);
}

//snakewoman doSomething function
void snakewoman::doSomething(dungeon* dunge)
{
    if (getSleep() > 0)
    {
        decreaseSleep(1);
        return;
    }
    
    if (getDeath() || dunge->getPlayer()->getDeath())
    {
        return;
    }
    if(monster::smell(dunge,3)) //the only difference between this and bogeyman doSomething function
    {
        int p_r = getRow();
        int p_c = getCol();
        char dir = monster::chooseDirection(dunge, p_r, p_c);
        switch (dir) {
            case ARROW_LEFT:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_RIGHT:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_UP:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_DOWN:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            default:
                break;
        }
    }
}

/////////////////////////////////
//Dragon Class Implementations//
///////////////////////////////

//dragon constructor
dragon::dragon(dungeon *dunge):monster(dunge,randInt(6)+20,randInt(6) + 20,4,4,4,0,"Dragon")
{
    weapon* dragonWeapon = new longSword();
    actor::setWeapon(dragonWeapon);
}

//dragon recovery
void dragon::recovery()
{
    bool p_recover = trueWithProbability(1.0/10);   //random possibility
    if(getHP() < getMaxHp() && p_recover == true)
    {
        increaseHP(1);  //increase HP by 1
    }
}

//dragon doSomething function
void dragon::doSomething(dungeon *dunge)
{
    if (getSleep() > 0) //if it is asleep
    {
        decreaseSleep(1);
        return;
    }
    
    if (getDeath() || dunge->getPlayer()->getDeath())   //if it is dead
    {
        return;
    }
    recovery();     //recovery
    int p_r = getRow();
    int p_c = getCol();
    if(dunge->isPlayer(p_r, p_c - 1))
        attack(this, dunge->getPlayer());
    else if(dunge->isPlayer(p_r, p_c + 1))
        attack(this, dunge->getPlayer());
    else if(dunge->isPlayer(p_r + 1, p_c))
        attack(this, dunge->getPlayer());
    else if(dunge->isPlayer(p_r - 1, p_c))
        attack(this, dunge->getPlayer());   //dragon can attack, but cannot move
}


/////////////////////////////////
//Goblin Class Implementations//
///////////////////////////////
goblin::goblin(dungeon* dunge):monster(dunge,randInt(6)+15,randInt(6)+15,1,3,1,0,"Goblin")
{
    weapon* goblinWeapon = new shortSword();
    actor::setWeapon(goblinWeapon);
    setDistance();
}

int goblin::getDistance() const {return goblinsmelldistance;}

//goblin smell distance set function
void goblin::setDistance()
{
    goblinsmelldistance = getDungeon()->getGoblinSmell();
}

bool goblin::goblinMovable(int r, int c)
{
    if (getDungeon()->getGrid(r, c)== ' ' || getDungeon()->getGrid(r, c) == ')' || getDungeon()->getGrid(r, c) == '>' || getDungeon()->getGrid(r, c) == '?' || getDungeon()->getGrid(r, c) == '&' || getDungeon()->getGrid(r, c) == '@')
        return true;
    else
        return false;
}

bool goblin::goblinSmell(int r, int c, int d)
{
    int p_row = getDungeon()->getPlayer()->getRow();
    int p_col = getDungeon()->getPlayer()->getCol();
    if (d < 1)
        return false;
    if (r == p_row && c == p_col)
        return true;
    if (r < p_row)
    {
        if (goblinSmell(r+1, c, d-1) && goblinMovable(r+1, c))
        {
            direction.push(ARROW_DOWN);
            return true;
        }
    }
    if (r > p_row)
    {
        if (goblinSmell(r-1, c, d-1) && goblinMovable(r-1, c))
        {
            direction.push(ARROW_UP);
            return true;
        }
    }
    if ( c > p_col)
    {
        if(goblinSmell(r, c-1, d-1) && goblinMovable(r, c-1))
        {
            direction.push(ARROW_LEFT);
            return true;
        }
    }
    if (c < p_col)
    {
        if(goblinSmell(r, c+1, d-1) && goblinMovable(r, c+1))
        {
            direction.push(ARROW_RIGHT);
            return true;
        }
    }
    return false;
}

void goblin::doSomething(dungeon *dunge)
{
    if (getSleep() > 0)
    {
        decreaseSleep(1);
        return;
    }
    
    if (getDeath() || dunge->getPlayer()->getDeath())
    {
        return;
    }

    if(goblinSmell(getRow(), getCol(),goblinsmelldistance))
    {
        int p_r = getRow();
        int p_c = getCol();
        char dir = direction.front();
        direction.pop();
        switch (dir) {
            case ARROW_LEFT:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_RIGHT:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_UP:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            case ARROW_DOWN:
                if(dunge->isPlayer(p_r, p_c - 1) || dunge->isPlayer(p_r+1, p_c) || dunge->isPlayer(p_r-1,p_c) || dunge->isPlayer(p_r,p_c+1))
                {
                    attack(this, dunge->getPlayer());
                }
                else
                {
                    actor::move(dir);
                }
                break;
            default:
                break;
        }
    }
    queue<char> empty;
    direction.swap(empty);
}
