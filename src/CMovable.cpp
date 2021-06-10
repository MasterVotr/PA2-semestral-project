/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */

#include "CMovable.hpp"
#include "CFloor.hpp"

/**
 * CMovable implementation
 * 
 * CMovable is for objects that can move
 */

CMovable::CMovable(std::ifstream & save, char glyph, CPopup * popups)
	: CTile(save, glyph)
	, m_Popups(popups)
{
	getline(save, m_Name, ';');
	
	std::string tmp;
	getline(save, tmp, ';');
	std::stringstream ss(tmp);
	ss >>m_Alive;

	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_MaxHP;

	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_CurrHP;

	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_Dexterity;

	if(save.fail() || !save.is_open())
		throw "Save corrupted (data corrupted (CMovable))";
}

CMovable::CMovable(TCoordinates coords, char glyph, std::string name, bool alive, int maxHP, int currHP, int dex, CPopup * popups)
	: CTile(coords, glyph)
	, m_Name(name)
	, m_Alive(alive)
	, m_MaxHP(maxHP)
	, m_CurrHP(currHP)
	, m_Dexterity(dex)
	, m_Popups(popups)
{}

int CMovable::injure(int damage)
{ 
	m_CurrHP -= damage;
	if(m_CurrHP <= 0)
	{
		m_CurrHP = 0;
		m_Alive = false;
	}
	return damage;
}

const std::string & CMovable::name() const
{ return m_Name; }

bool CMovable::isAlive() const
{ return m_Alive; }

void CMovable::changeHP(int change)
{ m_MaxHP += change; }

int CMovable::maxHP() const
{ return m_MaxHP; }

int CMovable::currHP() const
{ return m_CurrHP; }

void CMovable::setCoordinates(TCoordinates coords)
{ m_Coordinates = coords; }

int CMovable::dodgeChance() const
{ return m_Dexterity * 6; }

std::string CMovable::save() const
{
	std::stringstream save;
	save << CTile::save() << m_Name << ';' << m_Alive << ';' << m_MaxHP << ';' << m_CurrHP << ';' << m_Dexterity << ';' ;
	return save.str();
}