/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */

#include "CDoor.hpp"

/**
 * CDoor implementation
 * 
 * Represents door tiles
 */

CDoor::CDoor(std::ifstream & save, char glyph)
	:CStatic(save, glyph)
{
	std::string tmp;
	getline(save, tmp, ';');
	std::stringstream ss(tmp);
	ss >> m_Locked;
	
	if(save.fail() || !save.is_open())
		throw "Save corrupted (data corrupted (CDoor))";
}

CDoor::CDoor(TCoordinates coords, char glyph, bool locked)
	: CStatic(coords, glyph)
	, m_Locked(locked)
{}

bool CDoor::isLocked() const
{ return m_Locked; }

void CDoor::Unlock()
{ m_Locked = false; }

void CDoor::draw(WINDOW * boardwin) const
{ 
	if(m_Locked)
	{
		init_pair(2, COLOR_RED, COLOR_BLACK);
		wattrset(boardwin, COLOR_PAIR(2));
	}
	else
	{
		init_pair(0, COLOR_WHITE, COLOR_BLACK);
		wattrset(boardwin, COLOR_PAIR(0));
	}
	
	mvwaddch(boardwin, m_Coordinates.t_PosY, m_Coordinates.t_PosX, m_Glyph);
	wattroff(boardwin, COLOR_PAIR(2));
	wattrset(boardwin, COLOR_PAIR(0));
}

std::string CDoor::save() const
{
	std::stringstream save;
	save << CTile::save() << m_Locked << ';';
	return save.str();
}