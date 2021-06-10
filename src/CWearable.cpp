/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */

#include "CWearable.hpp"
#include "CPlayer.hpp"

#include <sstream>

//! ArmorType operator >> overload
std::istream & operator >> (std::istream & is, ArmorType & armorType)
{
	std::string armorName;
	is >> armorName;
	if     (armorName == "helmet"    ) armorType = ArmorType::HELMET;
	else if(armorName == "chestplate") armorType = ArmorType::CHESTPLATE;
	else if(armorName == "leggins"   ) armorType = ArmorType::LEGGINS;
	else if(armorName == "ring"      ) armorType = ArmorType::RING;
	else if(armorName == "amulet"    ) armorType = ArmorType::AMULET;
	else throw "Wrong ArmorType";
	return is;
}
//! ArmorType operator << overload
std::ostream & operator << (std::ostream & o, ArmorType armorType)
{
	switch(armorType)
	{
		case ArmorType::HELMET:     o << "helmet"; break;
		case ArmorType::CHESTPLATE: o << "chestplate"; break;
		case ArmorType::LEGGINS:    o << "leggins"; break;
		case ArmorType::RING:       o << "ring"; break;
		case ArmorType::AMULET:     o << "amulet"; break;
		default: throw "Wrong ArmorType";
	}
	return o;
}
//! BonusType operator >> overload
std::istream & operator >> (std::istream & is, BonusType & bonusType)
{
	std::string bonusName;
	is >> bonusName;
	if     (bonusName == "armor" ) bonusType = BonusType::ARMOR;
	else if(bonusName == "health") bonusType = BonusType::HEALTH;
	else if(bonusName == "damage") bonusType = BonusType::DAMAGE;
	else throw "Wrong ArmorType";
	return is;
}
//! BonusType operator << overload
std::ostream & operator << (std::ostream & o, BonusType bonusType)
{
	switch(bonusType)
	{
		case BonusType::ARMOR:  o << "armor"; break;
		case BonusType::HEALTH: o << "health"; break;
		case BonusType::DAMAGE: o << "damage"; break;
		default: throw "Wrong BonusType";
	}
	return o;
}

/**
 * CWearable implementation
 */

CWearable::CWearable(std::ifstream & save, char glyph)
	: CItem(save, glyph)
{
	std::string tmp;
	getline(save, tmp, ';');
	std::stringstream ss(tmp);
	ss >> m_ArmorType;

	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_BonusType;
	
	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_Bonus;

	if(save.fail() || !save.is_open())
		throw "Save corrupted (data corrupted (CWearable))";
}

CWearable::CWearable(TCoordinates coords, char glyph, std::string name, bool onBoard, ArmorType armorType, BonusType bonusType, int bonus)
	: CItem(coords, glyph, name, onBoard)
	, m_ArmorType(armorType)
	, m_BonusType(bonusType)
	, m_Bonus(bonus)
{}

bool CWearable::useItem(CPlayer * player)
{ return player->equipWearable(this); }

ArmorType CWearable::armorType() const
{ return m_ArmorType; }

BonusType CWearable::bonusType() const
{ return m_BonusType; }

int CWearable::bonus() const
{ return m_Bonus; }

void CWearable::draw(WINDOW * boardwin) const
{ 
	if(!m_OnBoard)
		return;
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	wattrset(boardwin, COLOR_PAIR(1));
	mvwaddch(boardwin, m_Coordinates.t_PosY, m_Coordinates.t_PosX, m_Glyph);
	wattron(boardwin, COLOR_PAIR(1));
}

std::string CWearable::itemInfo() const
{
	std::stringstream info;
	info << name() << ":\n Should prove useful\n bonus type: " << m_BonusType << ", bonus value: " << m_Bonus;
	return info.str();
}

std::string CWearable::save() const
{
	std::stringstream save;
	save << CItem::save() << m_ArmorType << ';' << m_BonusType << ';' << m_Bonus << ';';
	return save.str();
}