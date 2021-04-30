/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CInventory.hpp"
#include "CPlayer.hpp"
#include "CPotion.hpp"
#include "CFood.hpp"

#include <sstream>
#include <string>

/**
 * CInventory implementation
 * 
 * Players Inventory
 * holds CItems and their count
 */

CInventory::CInventory(CPlayer * player)
	: m_Player(player)
{
	m_Inventwin = newwin(m_InventHeight, m_InventWidth, 1, 1);
	keypad(m_Inventwin, true);
}

CInventory::~CInventory()
{
	for(auto item : m_Items)
		if(item != nullptr)
			delete item;
}

void CInventory::show()
{
	refresh();
	m_Player -> info(true);

	int inventposun;
	int highlight = 0;

//	throw "CInventory.show() - before while";
	while(42)
	{
		for (int h = 0; h < m_InventHeight; h++)
			for(int w = 0; w < m_InventWidth; w++)
				mvwaddch(m_Inventwin, h, w, ' ');
		m_Player -> info(true);
		if(LINES < m_ScrHeight || COLS < m_ScrWidth)
			throw "Please resize your terminal to at least 120x30 and relaunch the game";
//		throw "CInventory.show() - before header";
		// Write header
		mvwprintw(m_Inventwin, 1, 23, "INVENTORY");
		mvwprintw(m_Inventwin, 1, 76, "EQUIPMENT");
//		throw "CInventory.show() - before choices";
		// Write out all choices
		for(int i = 0; i < 8; i++)
		{
			if(i == highlight && m_Items.size() != 0)
				wattron(m_Inventwin, A_REVERSE);
			mvwaddch(m_Inventwin, (i < 4 ? i: i - 4) + 6, 1 + (i < 4 ? 0 : 27), '>');
			wattroff(m_Inventwin, A_REVERSE);
		}
		for(int i = 0; i < (int)m_Items.size(); i++)
		{
			if(i == highlight)
				wattron(m_Inventwin, A_REVERSE);
			mvwprintw(m_Inventwin, (i < 4 ? i: i - 4) + 6, 1 + (i < 4 ? 1 : 28), m_Items[i] -> name().c_str());
			wattroff(m_Inventwin, A_REVERSE);
		}
//		throw "CInventory.show() - before info";
		// Write out info about selected item
		if(m_Items.size() != 0)
			mvwprintw(m_Inventwin, 11, 1, m_Items[highlight] -> itemInfo().c_str());
//		throw "CInventory.show() - before equip";
		// Write out equipment
		mvwprintw(m_Inventwin,  6, 73, "HEAD:   %s", m_Player -> equipmentName(ArmorType::HELMET));
		mvwprintw(m_Inventwin,  7, 73, "CHEST:  %s", m_Player -> equipmentName(ArmorType::CHESTPLATE));
		mvwprintw(m_Inventwin,  8, 73, "LEGS:   %s", m_Player -> equipmentName(ArmorType::LEGGINS));
		mvwprintw(m_Inventwin,  9, 73, "NECK:   %s", m_Player -> equipmentName(ArmorType::AMULET));
		mvwprintw(m_Inventwin, 10, 73, "HAND:   %s", m_Player -> equipmentName(ArmorType::RING));
		mvwprintw(m_Inventwin, 11, 73, "WEAPON: %s", m_Player -> weaponName());
//		throw "CInventory.show() - before controls";
		// Write out controls
		mvwprintw(m_Inventwin, 15, (m_InventWidth - 28)/2,      "COTROLS");
		mvwprintw(m_Inventwin, 16, (m_InventWidth - 28)/2,      "use/equip:           E");
		mvwprintw(m_Inventwin, 17, (m_InventWidth - 28)/2,      "movement:            Arrow Keys");
		mvwprintw(m_Inventwin, 19, (m_InventWidth - 28)/2,      "Exit inventory:      I");
		mvwprintw(m_Inventwin, 20, (m_InventWidth - 57)/2,      "Take off helmet:     H");
		mvwprintw(m_Inventwin, 21, (m_InventWidth - 57)/2,      "Take off chestplate: C");
		mvwprintw(m_Inventwin, 22, (m_InventWidth - 57)/2,      "Take off leggins:    L");
		mvwprintw(m_Inventwin, 20, (m_InventWidth - 57)/2 + 29, "Take off amulet:     A");
		mvwprintw(m_Inventwin, 21, (m_InventWidth - 57)/2 + 29, "Take off ring:       R");
		mvwprintw(m_Inventwin, 22, (m_InventWidth - 57)/2 + 29, "Take off weapon:     W");
//		throw "CInventory.show() - before input";
		// Get input and process it
		switch (inventposun = wgetch(m_Inventwin))
		{
			case KEY_UP:
				if(--highlight == -1 && m_Items.size() != 0)
					highlight = (int)m_Items.size()-1;
				break;
			case KEY_DOWN:
				if(++highlight == (int)m_Items.size() && m_Items.size() != 0)
					highlight = 0;
				break;
			case KEY_LEFT:
				highlight %= 4;
				break;
			case KEY_RIGHT:
				if(highlight < 4)
					if(highlight += 4 > m_Items.size())
						highlight = m_Items.size() - 1;
				break;
			case 'e':
			// fall through
			case 'E':
				if(m_Items.size() != 0 && useItem(highlight))
				{
					if(--highlight == -1)
						highlight = 0;
				}
				break;
			case 'd':
			// fall through
			case 'D':
				if(m_Items.size() != 0)
				{
					discard(highlight);
					if(--highlight == -1)
						highlight = 0;
				}
				break;
			case 'h':
			// fall through
			case 'H':
				m_Player -> unequipWearable(ArmorType::HELMET);
				break;
			case 'c':
			// fall through
			case 'C':
				m_Player -> unequipWearable(ArmorType::CHESTPLATE);
				break;
			case 'l':
			// fall through
			case 'L':
				m_Player -> unequipWearable(ArmorType::LEGGINS);
				break;
			case 'a':
			// fall through
			case 'A':
				m_Player -> unequipWearable(ArmorType::AMULET);
				break;
			case 'r':
			// fall through
			case 'R':
				m_Player -> unequipWearable(ArmorType::RING);
				break;
			case 'w':
			// fall through
			case 'W':
				m_Player -> unequipWeapon();
				break;
			case 'i':
			// fall through
			case 'I':
				return;
			default:
				break;
		}
		napms(10);
	}
}

void CInventory::discard(int position)
{
	if(m_Items[position] != nullptr)
	{
		delete m_Items[position];
		m_Items.erase(m_Items.begin() + position);
	}
}

bool CInventory::addItem(CItem * item)
{
	if(isFull())
		return false;
	m_Items.push_back(item);
	return true;
}

bool CInventory::useItem(int position)
{
	if(!m_Items[position] -> useItem(m_Player))
		return false;
	if(m_Items[position] -> singleUse())
		delete m_Items[position];
	m_Items.erase(m_Items.begin() + position);
	m_Player -> updateStats(1);
	return true;
}

bool CInventory::isFull() const
{ return m_Items.size() == m_InventorySize; }

std::string CInventory::save() const
{
	std::stringstream save;
	save << m_Items.size() << '\n';
	for(auto item : m_Items)
	{
		if(item == nullptr)
			continue;
		save << item -> save() << '\n';
	}
	return save.str();
}

void CInventory::load(std::ifstream & save)
{
	int itemCnt;
	save >> itemCnt;
	if(!itemCnt)
		return;
	if(itemCnt > 8 || itemCnt < 0)
		throw "Save corrupted (wrong item count (CInventory))";
	for(int i = 0; i < itemCnt; i++)
	{
		char glyph, semicolon = 0;
		save >> glyph >> semicolon; if(semicolon != ';') throw "Save corrupted(; missing) (CInventory)";
		if(!save.good())
			throw "Save corrupted (data corrupted (CInventory))";
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
				throw "Save corrupted (Uknown glyph (CInventory))";
		}
		if(save.fail())
			throw "Save corrupted (data corrupted (CInventory))";
	}
}
