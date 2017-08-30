#include "objects.h"

////////////////////////////////
//Object Class Implementations//
///////////////////////////////

//object class constructors

object::object(string name, string action)
{
    m_name = name;
    m_action = action;
}

object::object(string name, string action, int r, int c)
{
    m_name = name;
    m_action = action;
    m_r = r;
    m_c = c;
}

object::object(string name, int r, int c)
{
    m_name = name;
    m_r = r;
    m_c = c;
}

//object class accessors

int object::getCol() const {return m_c;}

int object::getRow() const {return m_r;}

string object::getAction() const {return m_action;}

string object::getName() const {return m_name;}

int weapon::getDexBonus() const {return dexBonus;}

int weapon::getStrAmt() const {return StrAmt;}

//object setPos function
void object::setPos(int r, int c)
{
    m_r = r;
    m_c = c;
}

////////////////////////////////
//Weapon Class Implementations//
///////////////////////////////

//weapon class constructors
weapon::weapon(string name, string action, int dex, int str):object(name,action)
{
    dexBonus = dex;
    StrAmt = str;
}

weapon::weapon(string name ,string action, int dex, int str, int r, int c):object(name,action,r,c)
{
    dexBonus = dex;
    StrAmt = str;
}

shortSword::shortSword():weapon("short sword", "slashes", 0, 2){}

mace::mace():weapon("mace", "swings", 0, 2){}

longSword::longSword():weapon("long sword", "swings", 2, 4){}

magicAxe::magicAxe():weapon("magic axe", "chops", 5, 5){};

magicFang::magicFang():weapon("magic fangs of sleep", "strikes", 3, 2){};

magicFang::magicFang(int r, int c):weapon("magic fangs of sleep", "strikes", 3, 2, r, c){};
magicAxe::magicAxe(int r, int c):weapon("magic axe", "chops", 5, 5,r,c){};

////////////////////////////////
//Scroll Class Implementations//
///////////////////////////////

scroll::scroll(string name, string effect, int r, int c):object(name,effect,r,c){}
scroll::scroll(string name, string effect):object(name,effect)
{}
strengthScroll::strengthScroll():scroll("strength scroll", "Your muscles bulge."){};

armorScroll::armorScroll():scroll("scroll of enhance armor", "Your armor glows blue."){};

healthScroll::healthScroll():scroll("scroll of enhance health", "You feel your heart beating stronger."){};

dexterityScroll::dexterityScroll():scroll("scroll of enhance dexterity", "You feel like less of a klutz."){};

teleScroll::teleScroll():scroll("scroll of teleportation","You feel your body wrenched in space and time."){};

strengthScroll::strengthScroll(int r, int c):scroll("strength scroll", "Your muscles bulge.",r,c){};

armorScroll::armorScroll(int r, int c):scroll("scroll of enhance armor", "Your armor glows blue.",r,c){};

healthScroll::healthScroll(int r, int c):scroll("scroll of enhance health", "You feel your heart beating stronger.",r,c){};

dexterityScroll::dexterityScroll(int r, int c):scroll("scroll of enhance dexterity", "You feel like less of a klutz.",r,c){};

teleScroll::teleScroll(int r, int c):scroll("scroll of teleportation","You feel your body wrenched in space and time.",r,c){};

