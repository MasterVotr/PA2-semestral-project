/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CBeast.hpp"
#include "CFloor.hpp"
#include "CCombat.hpp"

#include <sstream>
#include <vector>
#include <algorithm>

//! PathfindingType operator << overload
std::ostream & operator << (std::ostream & o, PathfindingType type)
{
	switch(type)
	{
		case PathfindingType::RANDOM:  o << "random";  break;
		case PathfindingType::SEEKING: o << "seeking"; break;
	}
	return o;
}
//! PathfindingType operator >> overload
std::istream & operator >> (std::istream & i, PathfindingType & type)
{
	std::string pathfindingName;
	i >> pathfindingName;
	if     (pathfindingName == "random" ) type = PathfindingType::RANDOM;
	else if(pathfindingName == "seeking") type = PathfindingType::SEEKING;
	return i;
}

/**
 * CBeast implementation
 */

CBeast::CBeast(std::ifstream & save, char glyph, CPopup * popups)
	: CMovable(save, glyph, popups)
{
	std::string tmp;
	getline(save, tmp, ';');
	std::stringstream ss(tmp);
	ss >> m_Damage;

	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_Pathfinding;
	
	getline(save, tmp, ';');
	ss.clear();
	ss.str(tmp);
	ss >> m_Vision;

	if(save.fail() || !save.is_open())
		throw "Save corrupted (data corrupted (CBeast))";
}

CBeast::CBeast(TCoordinates coords, char glyph, std::string name, bool alive, int maxHP, int currHP, int dex,
			   int damage, PathfindingType pathfinding, int vision, CPopup * popups)
	: CMovable(coords, glyph, name, alive, maxHP, currHP, dex, popups)
	, m_Damage(damage)
	, m_Pathfinding(pathfinding)
	, m_Vision(vision)
{}

int CBeast::damage() const
{ return m_Damage; }

TCoordinates CBeast::nextMove(CFloor * floor)
{
	switch (m_Pathfinding)
	{
		case PathfindingType::RANDOM:
		{
			// The player is next to the beast
			if		((mvwinch(floor -> getWindow(),m_Coordinates.t_PosY - 1, m_Coordinates.t_PosX) & A_CHARTEXT) == '@')
				return TCoordinates(m_Coordinates.t_PosY - 1, m_Coordinates.t_PosX);
			else if	((mvwinch(floor -> getWindow(),m_Coordinates.t_PosY + 1, m_Coordinates.t_PosX) & A_CHARTEXT) == '@')
				return TCoordinates(m_Coordinates.t_PosY + 1, m_Coordinates.t_PosX);
			else if	((mvwinch(floor -> getWindow(),m_Coordinates.t_PosY, m_Coordinates.t_PosX - 1) & A_CHARTEXT) == '@')
				return TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX - 1);
			else if	((mvwinch(floor -> getWindow(),m_Coordinates.t_PosY, m_Coordinates.t_PosX + 1) & A_CHARTEXT) == '@')
				return TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX + 1);
			else
				// The player is not next to the beast, so it makes a random move
				return randMove(floor);
		}
		case PathfindingType::SEEKING:
		{
			TCoordinates playerCoords = floor -> player() -> getCoords();
			double dist = m_Coordinates.distanceTo(playerCoords);
			if(dist > (double)m_Vision)
				return randMove(floor);
			return pathMove(floor, floor -> player());
		}
		default:
			throw "Pathfinding type not found (CBeast::nextMove())";
	}
}

TCoordinates CBeast::pathMove(CFloor * floor, CMovable * target)
{
	TCoordinates targetCoords = target -> getCoords();
	double currDistanceToMe = m_Coordinates.distanceTo(targetCoords);
	std::vector<std::pair<double, char>> distIDLR = // up, down, left, right distances to target
	{
		{TCoordinates(m_Coordinates.t_PosY - 1, m_Coordinates.t_PosX).distanceTo(targetCoords), 'u'},
		{TCoordinates(m_Coordinates.t_PosY + 1, m_Coordinates.t_PosX).distanceTo(targetCoords), 'd'},
		{TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX - 1).distanceTo(targetCoords), 'l'},
		{TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX + 1).distanceTo(targetCoords), 'r'}
	};
	sort(distIDLR.begin(), distIDLR.end(), 
	[] (std::pair<double, char> & a, std::pair<double, char> & b) -> bool 
	{ return a.first > b.first; });
	TCoordinates nextPos;
	for(auto minDist : distIDLR)
	{
		if 		(minDist.second == 'u')
			nextPos = TCoordinates(m_Coordinates.t_PosY - 1, m_Coordinates.t_PosX);
		else if (minDist.second == 'd')
			nextPos = TCoordinates(m_Coordinates.t_PosY + 1, m_Coordinates.t_PosX);
		else if (minDist.second == 'l')
			nextPos = TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX - 1);
		else if (minDist.second == 'r')
			nextPos = TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX + 1);
		else
			throw "Unknown error (CBeast:pathMove())";
		
		int nextToMe = mvwinch(floor -> getWindow(), nextPos.t_PosY, nextPos.t_PosX) & A_CHARTEXT;
		if(floor -> findBeast(nextPos) == nullptr && nextToMe != '|' && nextToMe != '-' && 
			nextToMe != ' ' && minDist.first <= currDistanceToMe )
			{
				if (nextToMe == '+' && floor -> findDoor(nextPos) -> isLocked())
					return randMove(floor);
				return nextPos;
			}
	}
	return m_Coordinates;
}

TCoordinates CBeast::randMove(CFloor * floor)
{
	int nextToMe;
	TCoordinates nextPos;
	switch(rand()%4)
	{
		// fall through
		case 0:
			nextPos = TCoordinates(m_Coordinates.t_PosY - 1,m_Coordinates.t_PosX);
			nextToMe = mvwinch(floor -> getWindow(),nextPos.t_PosY,nextPos.t_PosX) & A_CHARTEXT;
			if((floor -> findBeast(nextPos) == nullptr && nextToMe != '|' && nextToMe != '-' && nextToMe != ' ')
		      && (nextToMe != '+' || (nextToMe == '+' && !floor -> findDoor(nextPos) -> isLocked())))
				return nextPos;
		// fall through
		case 1:
			nextPos = TCoordinates(m_Coordinates.t_PosY + 1,m_Coordinates.t_PosX);
			nextToMe = mvwinch(floor -> getWindow(),nextPos.t_PosY,nextPos.t_PosX) & A_CHARTEXT;
			if((floor -> findBeast(nextPos) == nullptr && nextToMe != '|' && nextToMe != '-' && nextToMe != ' ')
		      && (nextToMe != '+' || (nextToMe == '+' && !floor -> findDoor(nextPos) -> isLocked())))
				return nextPos;
		// fall through
		case 2:
			nextPos = TCoordinates(m_Coordinates.t_PosY,m_Coordinates.t_PosX - 1);
			nextToMe = mvwinch(floor -> getWindow(),nextPos.t_PosY,nextPos.t_PosX) & A_CHARTEXT;
			if((floor -> findBeast(nextPos) == nullptr && nextToMe != '|' && nextToMe != '-' && nextToMe != ' ')
		      && (nextToMe != '+' || (nextToMe == '+' && !floor -> findDoor(nextPos) -> isLocked())))
				return nextPos;
		// fall through
		case 3:
			nextPos = TCoordinates(m_Coordinates.t_PosY,m_Coordinates.t_PosX + 1);
			nextToMe = mvwinch(floor -> getWindow(),nextPos.t_PosY,nextPos.t_PosX) & A_CHARTEXT;
			if((floor -> findBeast(nextPos) == nullptr && nextToMe != '|' && nextToMe != '-' && nextToMe != ' ')
		      && (nextToMe != '+' || (nextToMe == '+' && !floor -> findDoor(nextPos) -> isLocked())))
				return nextPos;
		// fall through
		default:
			return m_Coordinates;
	}
}

int CBeast::move(CFloor * floor)
{
	TCoordinates nextPos = nextMove(floor);
	if (nextPos == floor -> player() -> getCoords())
	{	
		std::stringstream msg;
		CCombat combat(this, floor -> player());
		msg << combat.fight();
		m_Popups -> addMsg(msg.str().c_str());
	}
	else
		m_Coordinates = nextPos;
	if(!m_Alive)
	{
		floor -> killBeast(this);
		return m_MaxHP * 6;
	}
	return 0;
}

void CBeast::draw(WINDOW * boardwin) const
{ 
	if(!m_Alive)
		return;
	init_pair(2, COLOR_RED, COLOR_BLACK);
	wattrset(boardwin, COLOR_PAIR(2));
	mvwaddch(boardwin, m_Coordinates.t_PosY, m_Coordinates.t_PosX, m_Glyph);
	wattron(boardwin, COLOR_PAIR(2));
}

std::string CBeast::save() const
{
	std::stringstream save;
	save << CMovable::save() << m_Damage << ';' << m_Pathfinding << ';' << m_Vision << ';';
	return save.str();
}
