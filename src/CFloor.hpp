/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CFLOOR_H
#define _CFLOOR_H

#include "CTile.hpp"
#include "CBeast.hpp"
#include "CItem.hpp"
#include "CDoor.hpp"
#include "CPlayer.hpp"
#include "CPopup.hpp"

#include <string>
#include <vector>
#include <set>
#include <ncurses.h>

/**
 * Class CFloor is for taking care of the floor map
 */

class CFloor {
	public: 
		/**
		 * CFloor contructor, sets up a basic CFloor<br>
		 * @param boardwin -> ncurses WINDOW there the map is drawn
		 * @param player   -> CPlayer pointer
		 * @param popups   -> CPopups pointer
		 */
		CFloor(WINDOW * boardwin, CPlayer * player, CPopup * popups);
		/**
		 * CFloor constructor that takes in a const char *, searches for a map under that name and if possible loads it into m_Map<br>
		 * it also stores the given WINDOW pointer and a pointer to the player<br>
		 * calls init() at the end to procces the inputed map/game
		 * @param floorName -> name of the map save
		 * @param boardwin  -> ncurses WINDOW there the map is drawn
		 * @param player    -> CPlayer pointer
		 * @param popups    -> CPopups pointer
		 */
		CFloor(const char * floorName, WINDOW * boardwin, CPlayer * player, CPopup * popups);
		//! CFloor destructor that takes care of the object on the board
		~CFloor();
		/**
		 * Initializes the Floor using m_Map:<br>
		 * 1. checks if the map is the right size and uses only knows glyphs<br>
		 * 2. stores the beasts in m_Beasts<br>
		 * 3. stores the items in m_Items<br>
		 * 4. stores the doors in m_Doors<br>
		 * 5. updates the m_Player coordinates
		 */
		void init();
		/**
		 * Loads the data from std::ifstream<br>
		 * 1. checks if the map is the right size and uses only knows glyphs<br>
		 * 2. stores the beasts in m_Beasts<br>
		 * 3. stores the items in m_Items<br>
		 * 4. stores the doors in m_Doors<br>
		 * @param save -> std::ifstream
		 */
		 void load(std::ifstream & save);
		//! Updates and draws the raw map to the m_Boardwin
		void draw();
		/**
		 * @return Return a pointer to the item at coords
		 * @param coords
		 */
		CItem * findItem(TCoordinates coords);
		/**
		 * Removes an item from the floor
		 * @param item -> CItem pointer
		 */
		void removeItem(CItem * item);
		/**
		 * @return Return a pointer to the beast at coords
		 * @param coords
		 */
		CBeast * findBeast(TCoordinates coords);
		/**
		 * "Kills" a beast -> deletes it from memory
		 * @param beast -> CBeast pointer
		 */
		bool killBeast(CBeast * beast);
		/**
		 * @return Return a pointer to the door at coords
		 * @param coords
		 */
		CDoor * findDoor(TCoordinates coords);
		//! @return Return a pointer to the player
		CPlayer * player();
		/**
		 * Check if the glyph is known
		 * @param const char glyph
		 * @return success
		 */
		bool isEntityCheck(const char glyph) const;
		/**
		 * Check if the glyph is a beast
		 * @param const char glyph
		 * @return success
		 */
		bool isBeastGlyph(const char glyph) const;
		/**
		 * Check if the glyph is an item
		 * @param const char glyph
		 * @return success
		 */
		bool isItemGlyph(const char glyph) const;
		//! @return Returns m_Boardwin for read/write purpose
		WINDOW * getWindow();
		//! @return Returns a reference to m_Beasts
		std::vector<CBeast*> & beasts();
		//! @return Return a reference to m_Map
		std::string & map();
		//! @return Exports the current state of the floor as an std::string
		std::string save();
	protected: 
		//! Stores a pointer to the ncurses window it draws itself onto
		WINDOW * m_Boardwin;
		//! Stores a pointer to the player
		CPlayer * m_Player;
		/**
		 * before init(), m_Map stores the map loaded from a file<br>
		 * after init(), stores the raw map(walls, floors, hallways, gold, keys, walls)
		 */
		std::string m_Map;
		//! Stores the Beasts on the current map
		std::vector<CBeast*> m_Beasts;
		//! Stores the Items on the current map
		std::vector<CItem*> m_Items;
		//! Stores the door on the current map
		std::vector<CDoor*> m_Doors;
		//! constant map height
		const uint m_MapHeight = 24;
		//! constant map width
		const uint m_MapWidth = 94;
		//! Known tile glyphs
		std::set<char> m_KnownGlyphs = {'c', 'b', 'd', 'g', 'h', 'l', 'm', 'r', 's', 'w', 'F', 'H', 'L', ' ', '#', '.', '+', '-', '|', '>', '$', '!', '@'};
		//! Known beast glyphs
		std::set<char> m_KnownBeasts = {'b', 'd', 'g', 's'};
		//! Known item glyphs
		std::set<char> m_KnownItems = {'c','h', 'l', 'm', 'r', 'w', 'F', 'H'};
		//! Popups pointer
		CPopup * m_Popups;
		//! Loads items from @param save -> std::ifstream
		void loadItems(std::ifstream & save);
		//! Loads beasts from @param save -> std::ifstream
		void loadBeasts(std::ifstream & save);
		//! Loads doors from @param save -> std::ifstream
		void loadDoors(std::ifstream & save);
};

#endif //_CFLOOR_H