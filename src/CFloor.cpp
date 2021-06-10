/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CFloor.hpp"
#include "EPathfindingType.hpp"
#include "EArmorType.hpp"
#include "EBonusType.hpp"
#include "CWearable.hpp"
#include "CFood.hpp"
#include "CPotion.hpp"

#include <string>
#include <fstream>
#include <exception>
#include <algorithm>

/**
 * CFloor implementation
 */

CFloor::CFloor(WINDOW * boardwin, CPlayer * player, CPopup * popups)
	: m_Boardwin(boardwin)
	, m_Player(player)
	, m_Popups(popups)
{}

CFloor::CFloor(const char * floorName, WINDOW * boardwin, CPlayer * player, CPopup * popups)
	: m_Boardwin(boardwin)
	, m_Player(player)
	, m_Popups(popups)
{
	std::ifstream input;
	std::string path("./maps/");
	path.append(floorName);
	input.open(path);
	if(!input.is_open())
	{
		mvprintw(1, 0, path.c_str());
		throw "Map not found";
	}
	std::string tmpInput;
	while(getline(input, tmpInput))
		m_Map.append(tmpInput);
	if (input.bad())
		throw "IO error";
	else if(!input.eof())
		throw "Format error (!eof)";
	input.close();
}

CFloor::~CFloor()
{
	for(auto beast : m_Beasts)
		if(beast != nullptr)
			delete beast;
	for(auto item : m_Items)
		if(item != nullptr)
			delete item;
	for(auto door : m_Doors)
		if(door != nullptr)
			delete door;
}

void CFloor::init()
{
	int playerPresent = 0;
	uint i = 0;
	if(m_Map.size() != m_MapHeight * m_MapWidth)
	{	
		mvprintw(1, 0, "Loaded map size (%d) is not equal the standard map size (%d)", m_Map.size(), m_MapHeight * m_MapWidth);
		throw "Map file corrupted (Invalid size)";
	}
	for(i = 0; i < m_MapHeight * m_MapWidth; i++)
	{
		if(!isEntityCheck(m_Map[i]))
		{
			mvprintw(1, 0, "Glyph: i: %d %c %d unknown", i, m_Map[i], m_Map[i]);
			throw "Map file corrupted (Unknown glyph)";
		}
		switch (m_Map[i])
		{
		case 'b': // bat
			m_Beasts.push_back(new CBeast(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'b', "bat", true, 5, 5, 8, 3, PathfindingType::RANDOM, 5, m_Popups));
			m_Map[i] = '.';
			break;
		case 'c': // chestplate
			m_Items.push_back(new CWearable(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'c', "chestplate", true, ArmorType::CHESTPLATE, BonusType::ARMOR, 3));
			m_Map[i] = '.';
			break;
		case 'd': // dragon
			m_Beasts.push_back(new CBeast(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'd', "dragon", true, 40, 40, 4, 10, PathfindingType::SEEKING, 12, m_Popups));
			m_Map[i] = '.';
			break;
		case 'g': // goblin
			m_Beasts.push_back(new CBeast(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'g', "goblin", true, 9, 9, 5, 5, PathfindingType::SEEKING, 6, m_Popups));
			m_Map[i] = '.';
			break;
		case 'h': // helmet
			m_Items.push_back(new CWearable(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'h', "helmet", true, ArmorType::HELMET, BonusType::ARMOR, 2));
			m_Map[i] = '.';
			break;
		case 'l': // leggins
			m_Items.push_back(new CWearable(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'l', "leggins", true, ArmorType::LEGGINS, BonusType::ARMOR, 2));
			m_Map[i] = '.';
			break;
		case 'm': // amulet
			m_Items.push_back(new CWearable(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'm', "amulet", true, ArmorType::AMULET, BonusType::HEALTH, 25));
			m_Map[i] = '.';
			break;
		case 'r': // ring
			m_Items.push_back(new CWearable(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'r', "ring", true, ArmorType::RING, BonusType::DAMAGE, 25));
			m_Map[i] = '.';
			break;
		case 's': // giant spider
			m_Beasts.push_back(new CBeast(TCoordinates(i/m_MapWidth, i%m_MapWidth), 's', "spider", true, 7, 7, 2, 6, PathfindingType::RANDOM, 5, m_Popups));
			m_Map[i] = '.';
			break;
		case 'w': // weapon
			m_Items.push_back(new CWeapon(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'w', "spear", true, 6));
			m_Map[i] = '.';
			break;
		case 'F': // food
			m_Items.push_back(new CFood(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'F', "suspicious looking bread", true, 150));
			m_Map[i] = '.';
			break;
		case 'H': // health potion
			m_Items.push_back(new CPotion(TCoordinates(i/m_MapWidth, i%m_MapWidth), 'H', "health potion", true, 18));
			m_Map[i] = '.';
			break;
		case 'L': // locked doors
			m_Doors.push_back(new CDoor(TCoordinates(i/m_MapWidth, i%m_MapWidth), '+', true));
			break;
		case '+': // door
			m_Doors.push_back(new CDoor(TCoordinates(i/m_MapWidth, i%m_MapWidth), '+', false));
			break;
		case '-': // horizontal wall
		case '|': // vertical wall
		case '.': // floor
		case '#': // hallway
		case ' ': // void tile
		case '>': // next level
		case '$': // gold
		case '!': // key
			break;
		case '@':
			++playerPresent;
			m_Player -> setCoordinates(TCoordinates(i/m_MapWidth, i%m_MapWidth));
			m_Map[i] = '.';
			break;
		default:
			mvprintw(1, 0, "Glyph (default): %c %d unknown; at location: [y, x] [%d, %d]", m_Map[i], m_Map[i], i/m_MapWidth, i%m_MapWidth);
			throw "Map file corrupted (Unknown glyph)";
			break;
		}
	}
	if(playerPresent != 1)
	{
		mvprintw(3, 0, "Player count: %d", playerPresent);
		throw "Map file corrupted (Player not present)";
	}
}

void CFloor::load(std::ifstream & save)
{
	try
	{
		getline(save, m_Map);
		if(m_Map.size() != m_MapHeight * m_MapWidth)
			throw "Save corrupted (invalid map size)";
		for(uint i = 0; i < m_MapHeight * m_MapWidth; i++)
			if(!isEntityCheck(m_Map[i]))
				throw "Save corrupted (Unknown glyph) entitycheck 1";
		loadItems(save);
		loadBeasts(save);
		loadDoors(save);
	} catch(const char * e)
	{
		throw e;
	}
}

void CFloor::loadItems(std::ifstream & save)
{
	int itemCnt = 0;
	save >> itemCnt;
	if(itemCnt)
	{
		for(int i = 0; i < itemCnt; i++)
		{
			if(!save.good())
				throw "Save corrupted (data corrupted)";
			char glyph;
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> glyph;
			if(!isItemGlyph(glyph))
				throw "Save corrupted (Unknown item glyph)";
			if(!save.good())
				throw "Save corrupted (data corrupted)";
			switch (glyph)
			{
				case 'c':
				// fall through
				case 'h':
				// fall through
				case 'l':
				// fall through
				case 'm':
				// fall through
				case 'r':
					m_Items.push_back(new CWearable(save, glyph));
					break;
				case 'w':
					m_Items.push_back(new CWeapon(save, glyph));
					break;
				case 'H':
					m_Items.push_back(new CPotion(save, glyph));
					break;
				case 'F':
					m_Items.push_back(new CFood(save, glyph));
					break;
				default:
					throw "Save corrupted (Uknown glyph) switch";
			}
			if(save.fail())
				throw "Save corrupted (data corrupted)";
		}
	}
}

void CFloor::loadBeasts(std::ifstream & save)
{
	int beastCnt = 0;
	save >> beastCnt;
	if(beastCnt)
	{
		for(int i = 0; i < beastCnt; i++)
		{
			if(!save.good())
				throw "Save corrupted (data corrupted)";
			char glyph;
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> glyph;
			if(!isBeastGlyph(glyph))
				throw "Save corrupted (Unknown beast glyph)";
			if(!save.good())
				throw "Save corrupted (data corrupted)";
			m_Beasts.push_back(new CBeast(save, glyph, m_Popups));
			if(save.fail())
				throw "Save corrupted (data corrupted)";
		}
	}
}

void CFloor::loadDoors(std::ifstream & save)
{
	int doorCnt = 0;
	save >> doorCnt;
	if(doorCnt)
	{
		for(int i = 0; i < doorCnt; i++)
		{
			if(!save.good())
				throw "Save corrupted (data corrupted)";
			char glyph;
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> glyph;
			if(glyph != '+')
				throw glyph;
			if(!save.good())
				throw "Save corrupted (data corrupted)";
			m_Doors.push_back(new CDoor(save, glyph));
			if(save.fail())
				throw "Save corrupted (data corrupted)";
		}
	}
}

std::string CFloor::save()
{
	std::stringstream save;
	//SAVING RAW MAP
	save << m_Map << '\n';
	//SAVING ITEMS ON BOARD
	save << m_Items.size() << '\n';
	for(auto item : m_Items)
	{
		if(item == nullptr || !item -> onBoard())
			continue;
		save << item -> save() << '\n';
	}
	// SAVING BEASTS
	save << m_Beasts.size() << '\n';
	for(auto beast : m_Beasts)
	{
		if(beast == nullptr || !beast -> isAlive())
			continue;
		save << beast -> save() << '\n';
	}
	// SAVING DOORS
	save << m_Doors.size() << '\n';
	for(auto door : m_Doors)
	{
		if(door == nullptr)
			continue;
		save << door -> save() << '\n';
	}
	return save.str();
}

void CFloor::draw()
{
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	for (uint i = 0; i < m_MapHeight * m_MapWidth; i++)
	{
		if(m_Map[i] == '$')
			wattrset(m_Boardwin, COLOR_PAIR(5));
		mvwaddch(m_Boardwin, i/m_MapWidth, i%m_MapWidth, m_Map[i]);
		wattroff(m_Boardwin, COLOR_PAIR(5));
	}
	for(auto item : m_Items)
		item -> draw(m_Boardwin);
	for(auto door : m_Doors)
		door -> draw(m_Boardwin);
	for(auto beast : m_Beasts)
		beast -> draw(m_Boardwin);
	m_Player -> draw(m_Boardwin);
	wrefresh(m_Boardwin);
}

CItem * CFloor::findItem(TCoordinates coords)
{
	for (auto item : m_Items)
		if (item -> getCoords() == coords)
			return item;
	return nullptr;
}

void CFloor::removeItem(CItem * item)
{
	uint i = 0;
	for ( ; i < m_Items.size(); i++)
		if (m_Items[i] -> getCoords() == item -> getCoords())
			break;
	m_Items.erase(m_Items.begin() + i);
}

CBeast * CFloor::findBeast(TCoordinates coords)
{
	for (auto beast : m_Beasts)
		if (beast -> getCoords() == coords)
			return beast;
	return nullptr;
}

bool CFloor::killBeast(CBeast * beast)
{
	for(auto b = m_Beasts.begin(); b != m_Beasts.end(); b++)
		if(*b == beast)
		{
			delete *b;
			m_Beasts.erase(b);
			return true;
		}
	throw "Beast not found";
	return false;
}

CDoor * CFloor::findDoor(TCoordinates coords)
{
	for (auto door : m_Doors)
		if (door -> getCoords() == coords)
			return door;
	return nullptr;
}

CPlayer * CFloor::player()
{ return m_Player; }

bool CFloor::isEntityCheck(const char glyph) const
{ return m_KnownGlyphs.find(glyph) != m_KnownGlyphs.end(); }

bool CFloor::isBeastGlyph(const char glyph) const
{ return m_KnownBeasts.find(glyph) != m_KnownBeasts.end(); }

bool CFloor::isItemGlyph(const char glyph) const
{ return m_KnownItems.find(glyph) != m_KnownItems.end(); }

WINDOW * CFloor::getWindow()
{ return m_Boardwin; }

std::vector<CBeast*> & CFloor::beasts()
{ return m_Beasts; }

std::string & CFloor::map()
{ return m_Map; }