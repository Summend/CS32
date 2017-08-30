
#ifndef __PROJECT3_QAQQAQ__objects__
#define __PROJECT3_QAQQAQ__objects__

#include <string>
using namespace std;

class dungeon;
class player;

class object
{
public:
    //constructor & destructor
    object();
    object(string name,string action);
    object(string name,string action,int r,int c);
    object(string name, int r, int c);
    virtual ~object(){};
    
    //accessor
    string getName() const;
    string getAction() const;
    int getRow() const;
    int getCol() const;
    
    //mutator
    void setPos(int r, int c);
private:
    string m_name;
    string m_action;
    int m_r;
    int m_c;
};

class weapon:public object
{
public:
    //constructor & destructor
    weapon(string name ,string action, int dex, int str);
    weapon(string name ,string action, int dex, int str, int r, int c);
    virtual ~weapon(){};
    
    //accessor
    int getDexBonus() const;
    int getStrAmt() const;
private:
    int dexBonus;
    int StrAmt;
    
};

class shortSword : public weapon
{
public:
    //constructor & destructor
    shortSword();
    virtual ~shortSword(){};
};

class mace : public weapon
{
public:
    //constructor & destructor
    mace();
    virtual ~mace(){};
};

class longSword:public weapon
{
public:
    //constructor & destructor
    longSword();
    virtual ~longSword(){};
    
};

class magicAxe:public weapon
{
public:
    //constructor & destructor
    magicAxe();
    magicAxe(int r, int c);
    virtual ~magicAxe(){};
};

class magicFang:public weapon
{
public:
    //constructor & destructor
    magicFang();
    magicFang(int r, int c);
    virtual ~magicFang(){};
};


class scroll:public object
{
public:
    //constructor & destructor
    scroll(string name, string effect);
    scroll(string name, string effect, int r, int c);
    virtual ~scroll(){};
};

class strengthScroll : public scroll
{
public:
    //constructor & destructor
    strengthScroll();
    strengthScroll(int r,int c);
    virtual ~strengthScroll(){};
    
};

class armorScroll : public scroll
{
public:
    //constructor & destructor
    armorScroll();
    armorScroll(int r,int c);
    virtual ~armorScroll(){};
};

class healthScroll :public scroll
{
public:
    //constructor & destructor
    healthScroll();
    healthScroll(int r,int c);
    virtual ~healthScroll(){};
};

class dexterityScroll :public scroll
{
public:
    //constructor & destructor
    dexterityScroll();
    dexterityScroll(int r,int c);
    virtual ~dexterityScroll(){};
};

class teleScroll: public scroll
{
public:
    //constructor & destructor
    teleScroll();
    teleScroll(int r, int c);
    virtual ~teleScroll(){};
};
#endif /* defined(__PROJECT3_QAQQAQ__objects__) */
