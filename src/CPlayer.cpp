/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CPlayer.hpp"
#include "CFloor.hpp"
#include "CDoor.hpp"
#include "CItem.hpp"
#include "CBeast.hpp"
#include "CCombat.hpp"
#include "EArmorType.hpp"

#include <sstream>
#include <fstream>

#define CTRL(k) ((k)  & 31)	// Used to simulate Ctrl + KEY for ncurses

/**
 * CPlayer implementation
 */

CPlayer::CPlayer(std::ifstream & save, CPopup * popups)
	: CMovable(save, '@', popups)
	, m_Inventory(this)
	, m_Weapon(nullptr)
	, m_Wear(this)
{
	try
	{
		m_Info  = newwin(24, 23, 1, 96);
		// LOADING PLAYER STATS
		std::string line;
		getline(save, line, ';');
		std::stringstream ss(line);
		ss >> m_Strength;
		
		getline(save, line, ';');
		ss.clear();
		ss.str(line);
		ss >> m_Endurance;
		
		getline(save, line, ';');
		ss.clear();
		ss.str(line);
		ss >> m_KeyCnt;
		
		getline(save, line, ';');
		ss.clear();
		ss.str(line);
		ss >> m_CurrFood;

		getline(save, line, ';');
		ss.clear();
		ss.str(line);
		ss >> m_Gold;

		if(!save.good())
			throw "Save corrupted (data corrupted (CPlayer))";
		// LOADING PLAYERS WEAPON
		int hasWeapon;
		save >> hasWeapon;
 		if(hasWeapon)
		{
			char glyph;
			getline(save, line, ';');
			ss.clear();
			ss.str(line);
			ss >> glyph;
			equipWeapon(new CWeapon(save, glyph));
			if(!save.good())
				throw "Save corrupted (data corrupted (CPlayer::CWeapon))";
		}
		// LOADING INVENTORY
		m_Inventory.load(save);
		// LOADING EQUIPMENT
		m_Wear.load(save);
	} catch(const char * e)
	{
		throw e;
	}
}

CPlayer::CPlayer(TCoordinates coords, char glyph, std::string name, bool alive,
				int maxHP, int currHP, int dex, int stre, int endu, CPopup * popups)
	: CMovable(coords, glyph, name, alive, maxHP, currHP, dex, popups)
	, m_Inventory(this)
	, m_Weapon(nullptr)
	, m_Wear(this)
	, m_Strength(stre)
	, m_Endurance(endu)
	, m_KeyCnt(0)
	, m_CurrFood(150)
	, m_Gold(0)
{
	m_Info  = newwin(24, 23, 1, 96);
}

CPlayer::~CPlayer()
{
	delwin(m_Info);
	if(m_Weapon != nullptr && !m_Weapon -> onBoard())
		delete m_Weapon;
}

bool CPlayer::eat(int amount)
{
	m_CurrFood += amount;
	if(m_CurrFood > m_MaxFood)
		m_CurrFood = m_MaxFood;
	return m_Alive;
}

bool CPlayer::heal(int amount)
{
	m_CurrHP += amount;
	if(m_CurrHP > m_MaxHP)
		m_CurrHP = m_MaxHP;
	return m_Alive;
}

bool CPlayer::useKey()
{
	if(m_KeyCnt == 0)
		return false;
	--m_KeyCnt;
	return true;
}

void CPlayer::addKey()
{ ++m_KeyCnt; }

int CPlayer::move(CFloor * floor)
{
	while(42)
	{
		switch (int decision = wgetch(floor -> getWindow()))
		{
			case KEY_UP:
				return movePos(floor, TCoordinates(m_Coordinates.t_PosY - 1, m_Coordinates.t_PosX));
			case KEY_DOWN:
				return movePos(floor, TCoordinates(m_Coordinates.t_PosY + 1, m_Coordinates.t_PosX));
			case KEY_LEFT:
				return movePos(floor, TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX - 1));
			case KEY_RIGHT:
				return movePos(floor, TCoordinates(m_Coordinates.t_PosY, m_Coordinates.t_PosX + 1));
			case 'i':
			case 'I':
				m_Inventory.show();
				return 0;
			case CTRL('q'):
			case CTRL('s'):
				return decision;
			default:
				break;
		}
	}
	return 0;
}

int CPlayer::movePos(CFloor * floor, TCoordinates newcoords)
{
	std::stringstream msg;
	// foundGlyph is composed of: a character returned from m_Board (floor -> getWindow()) and
	//   & A_CHARTEXT to extrapolate the character and not character + color
	int foundGlyph = mvwinch(floor -> getWindow(), newcoords.t_PosY, newcoords.t_PosX) & A_CHARTEXT;
	switch (foundGlyph)
	{
		// NEXT LEVEL/WIN
		case '>':
			return 1;
		// VERTICAL WALL  
		case '|':
		// HORIZONTAL WALL  
		case '-':
		// VOID  
		case ' ':
		{
			msg << "*Ouch* My nose! I walked into a wall. Why did I do that?";
			break;
		}
		// HALLWAY and FLOOR  
		case '#':
		case '.':
		{
			m_Coordinates = newcoords;
			break;
		}
		// KEY  
		case '!':
		{
			msg << "I found a key! Might be useful in the future!";
			addKey();
			m_Coordinates = newcoords;
			floor -> map()[newcoords.t_PosY * 94 + newcoords.t_PosX] = '.';
			break;
		}
		// GOLD  
		case '$':
		{
			m_Coordinates = newcoords;
			int g = rand()%9 + 1;
			pickupGold(g);
			msg << "I picked up " << g << " gold";
			floor -> map()[newcoords.t_PosY * 94 + newcoords.t_PosX] = '.';
			break;
		}
		// DOOR  
		case '+':
		{
			CDoor * door = floor -> findDoor(newcoords);
			if(door -> isLocked())
				if(useKey())
				{
					m_Coordinates = newcoords;
					door -> Unlock();
					msg << "Door unlocked";
				}
				else
					msg << "It is locked. I should find a key somewhere.";
			else
				m_Coordinates = newcoords;
			break;
		}
		// FOOD  
		case 'F':
		// HP POTION  
		case 'H':
		// CHESTPLATE  
		case 'c':
		// HELMET  
		case 'h':
		// LEGGINS  
		case 'l':
		// AMULET  
		case 'm':
		// RING  
		case 'r':
		// WEAPON  
		case 'w':
		{
			CItem * item = floor -> findItem(newcoords);
			if(pickup(item))
			{
				item -> onBoard() = false;
				floor -> removeItem(item);
				msg << "I picked up a/an " << item -> name();
			}
			else
				msg << "I cannot pick that up";					
			m_Coordinates = newcoords;
			break;
		}
		// BAT  
		case 'b':
		// GOBLIN  
		case 'g':
		// SPIDER  
		case 's':
		// DRAGON  
		case 'd':
		{
			CCombat combat(this, floor -> findBeast(newcoords));
			msg << combat.fight();
			break;
		}
		default:
		{
			mvprintw(1, 0, "Uknown glyph: %d %c ", foundGlyph, foundGlyph);
			throw "Uknown glyph at new position (CPlayer::move())";
		}
	}
	// STARVING AT EACH MOVE
	if(--m_CurrFood <= 0)
	{
		m_CurrFood = 0;
		if(--m_CurrHP <= 0)
		{
			m_CurrHP = 0;
			m_Alive = false;
		}
	}
	m_Popups -> addMsg(msg.str().c_str());
	return 0;
}

void CPlayer::draw(WINDOW * boardwin) const
{ 
	if(!m_Alive)
		return;
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	wattrset(boardwin, COLOR_PAIR(4));
	mvwaddch(boardwin, m_Coordinates.t_PosY, m_Coordinates.t_PosX, m_Glyph);
	wattron(boardwin, COLOR_PAIR(4));
}

int CPlayer::damage() const
{
	if(m_Weapon == nullptr)
		return 1 * (1 + (m_Strength * 6.0 / 100)); 
	return m_Weapon -> damage() * (1 + ( (m_Strength * 10.0) / 100) + (m_Wear.getDamageBonus() / 100.0) );
}

int CPlayer::injure(int damage)
{ return CMovable::injure(damage * ((100 - armor() * 6)/100.0)); }

int CPlayer::armor() const
{ return m_Wear.getArmorBonus(); }

int CPlayer::gold() const
{ return m_Gold; }

const char * CPlayer::weaponName() const
{
	if(m_Weapon == nullptr)
		return "unarmed"; 
	return m_Weapon -> name().c_str();
}

void CPlayer::pickupGold(int g)
{ m_Gold += g; }

bool CPlayer::pickup(CItem * item)
{ return m_Inventory.addItem(item); }

bool CPlayer::equipWeapon(CWeapon * weapon)
{
	if (unequipWeapon())
	{
		m_Weapon = weapon;
		return true;
	}
	return false;
}

bool CPlayer::unequipWeapon()
{
	if(m_Inventory.isFull())
		return false;
	if(m_Weapon != nullptr)
	{
		m_Inventory.addItem(m_Weapon);
		m_Weapon = nullptr;
	}
	return true;
}

bool CPlayer::equipWearable(CWearable * wearable)
{ return m_Wear.equip(wearable); }

bool CPlayer::unequipWearable(ArmorType type)
{
	if(m_Inventory.isFull())
		return false;
	return m_Wear.unequip(type); }

const char * CPlayer::equipmentName(ArmorType type)
{ return m_Wear.equipmentName(type); }

bool CPlayer::inventoryFull() const
{ return m_Inventory.isFull(); }

void CPlayer::updateStats(int menu)
{
	m_MaxHP = 6 + m_Endurance * 6 * (1 + m_Wear.getHealthBonus()/100.0);
	info(menu);
}

void CPlayer::info(int menu) const
{
	mvwprintw(m_Info,  4, 0, " Name:   %s             ", name().c_str());
	mvwprintw(m_Info,  5, 0, " HP:     %d/%d          ", currHP(), maxHP());
	mvwprintw(m_Info,  6, 0, " Food:   %d/%d          ", m_CurrFood, m_MaxFood);
	mvwprintw(m_Info,  7, 0, " Keys:   %d             ", m_KeyCnt);
	mvwprintw(m_Info,  9, 0, " Gold:   %dg            ", gold());
	mvwprintw(m_Info, 11, 0, " Damage: %d             ", damage());
	mvwprintw(m_Info, 12, 0, " Armor:  %d             ", armor());
	mvwprintw(m_Info, 13, 0, " Dodge:  %d%%           ", dodgeChance());
	mvwprintw(m_Info, 15, 0, " Weapon: %s             ", weaponName());
	if(menu == 0)
	{
		mvwprintw(m_Info, 17, 0, " CONTROLS:              ");
		mvwprintw(m_Info, 18, 0, " Movement:  Arrow Keys  ");
		mvwprintw(m_Info, 19, 0, " Inventory: I           ");
		mvwprintw(m_Info, 20, 0, " Quit:      <Ctrl+Q>    ");
		mvwprintw(m_Info, 21, 0, " Save:      <Ctrl+S>    ");
	}
	else if(menu == 1)
	{
		mvwprintw(m_Info, 17, 0, "                        ");
		mvwprintw(m_Info, 18, 0, "                        ");
		mvwprintw(m_Info, 19, 0, "                        ");
		mvwprintw(m_Info, 20, 0, "                        ");
		mvwprintw(m_Info, 21, 0, "                        ");
	}
	wrefresh(m_Info);
}

std::string CPlayer::save() const
{
	std::stringstream save;
	// SAVING PLAYER STATS
	save << CMovable::save() << m_Strength << ';' << m_Endurance 
		 << ';' << m_KeyCnt << ';' << m_CurrFood << ';' << m_Gold << ';'
	     << '\n';
	// SAVING WEAPON
	if(m_Weapon == nullptr)
		save << false;
	else
	{	
		save << true << '\n';
		save << m_Weapon -> save();
	}
	save << '\n';
	// SAVING INVENTORY
	save << m_Inventory.save();
	// SAVING EQUIPMENT
	save << m_Wear.save();
	return save.str();
}