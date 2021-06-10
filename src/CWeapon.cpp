/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CWeapon.hpp"
#include "CPlayer.hpp"

#include <sstream>

/**
 * CWeapon implementation
 */

CWeapon::CWeapon(std::ifstream & save, char glyph)
	: CItem(save, glyph)
{
	std::string tmp;
	getline(save, tmp, ';');
	std::stringstream ss(tmp);
	ss >> m_Damage;
	if(save.fail() || !save.is_open())
		throw "Save corrupted (data corrupted (CWeapon))";
}

CWeapon::CWeapon(TCoordinates coords, char glyph, std::string name , bool onBoard, int damage)
	: CItem(coords, glyph, name, onBoard)
	, m_Damage(damage)
{}

std::string CWeapon::itemInfo() const
{
	std::stringstream info;
	info << name() << ":\n Deadly in the right hands.\n damage: " << damage();
	return info.str();
}

std::string CWeapon::save() const
{
	std::stringstream save;
	save << CItem::save() << m_Damage << ';';
	return save.str();
}

int CWeapon::damage() const
{ return m_Damage; }

bool CWeapon::useItem(CPlayer * player)
{ return player->equipWeapon(this); }

void CWeapon::draw(WINDOW * boardwin) const
{ 
	if(!m_OnBoard)
		return;
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	wattrset(boardwin, COLOR_PAIR(1));
	mvwaddch(boardwin, m_Coordinates.t_PosY, m_Coordinates.t_PosX, m_Glyph);
	wattron(boardwin, COLOR_PAIR(1));
}