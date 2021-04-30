/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CPLAYER_H
#define _CPLAYER_H

#include "CMovable.hpp"
#include "CInventory.hpp"
#include "CWeapon.hpp"
#include "CWear.hpp"
#include "CPopup.hpp"

#include <string>

/**
 * CPlayer implementation
 * 
 * represents the Player tile
 */

class CPlayer: public CMovable 
{
	public:
		/**
		 * CPlayer constructor, used for loading CPlayer object from an std::ifstream<br>
		 * passes save, glyph and popups to his base class constructor<br>
		 * calls the inventory a wear loading methods
		 * @param save   -> std::ifstream from witch the data is loaded
		 * @param popups -> to store CPopup pointer, so the movable is able to use popup messages
		 */
		CPlayer(std::ifstream & save, CPopup * popups);
		/**
		 * CPlayer constructor, used for creating a new CPlayer from the given parameters<br>
		 * passes save, glyph, name, alive, maxHP, currHP, dex and popups to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m-Name of the object
		 * @param alive		  -> to store wether the movable is alive or not
		 * @param maxHP		  -> to store the m_MaxHP
		 * @param currHP	  -> to store the m_CurrHP
		 * @param dex		  -> to store the amount of m_Dexterity
		 * @param stre		  -> to store the amount of m_Strength
		 * @param endu		  -> to store the amount of m_Endurance
		 * @param popups	  -> to store CPopup pointer, so the movable is able to use popup messages
		 */
		CPlayer(TCoordinates coords, char glyph, std::string name, bool alive, int maxHP, int currHP, int dex, int stre, int endu, CPopup * popups);
		virtual ~CPlayer();
		/**
		 * "Eat" food -> increase the m_CurrFood by amount, caps of at m_MaxFood
		 * @param amount
		 * @return success
		 */
		bool eat(int amount);
		/**
		 * "Heals" HP -> increase the m_CurrHP, caps of at m_MaxHP
		 * @param amount
		 * @return success
		 */
		bool heal(int amount);
		/**
		 * Tries to equip a Weapon, if the "slot" if full it switches the two weapons
		 * @param weapon -> CWeapon pointer
		 * @return success
		 */
		bool equipWeapon(CWeapon * weapon);
		/**
		 * Takes off m_Weapon
		 * @return success
		 */
		bool unequipWeapon();
		/**
		 * Tries to equip an item, if the "slot" if full it switches the two items
		 * @param item -> CItem pointer
		 * @return success
		 */
		bool equipWearable(CWearable * wearable);
		/**
		 * Takes off m_Weapon
		 * @param type -> ArmorType
		 * @return success
		 */
		bool unequipWearable(ArmorType type);
		/**
		 * @param type -> ArmorType
		 * @return name of equip in "slot" type or nullptr if not item in slot
		 */
		const char * equipmentName(ArmorType type);
		//! @return if m_inventory is full
		bool inventoryFull() const;
		/**
		 * Use a key, return false if there are no keys left to use
		 * @return success
		 */
		bool useKey();
		//! Adds a key
		void addKey();
		/**
		 * Transfer an Item to the players inventory, but only if it fits.
		 * @param item -> Item pointer
		 * @return success
		 */
		bool pickup(CItem * item);
		//! @return int damage amount extracted from m_Weapon and enhanced with m_Strength and equipment
		virtual int damage() const override;
		/**
		 * CPlayer "takes damage", calculated using inputed damage and teh armor of the player
		 * @param damage
		 * @return damage received
		 */
		virtual int injure(int damage)override;
		//! @return int armor amount
		int armor() const;
		//! @returns players gold
		int gold() const;
		//! @return weapon name
		const char * weaponName() const;
		/**
		 * Adds gold to m_Gold
		 * @param g -> amount of gold
		 */
		void pickupGold(int g);
		/**
		 * Polymophic implementation of the base method move()<br>
		 * 1. Gets input from the player<br>
		 * 2.--> If the input is 'I' or 'i', opens the inventory screen<br>
		 * ----> If the input is 'Ctrl-Q', the game stops<br>
		 * ----> If the input is 'Ctrl-S', the game is saved under the player name<br>
		 * ----> If the input is one of the arrow keys the method calls movePos(), witch sorts out the player movement<br>
		 * 3. updates the players stats accordingly<br>
		 * Return the decision the player made<br>
		 * @param floor CFloor pointer
		 * @return decision
		 */
		virtual int move(CFloor * floor) override;
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, polymophic override of draw() function from CTile
		 * @param boardwin -> WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const override;
		/**
		 * Writes out the current stats of the player
		 * @param menu:
		 * 0 -> standard<br>
		 * 1 -> inventory<br>
		 */
		void info(int menu) const;
		/**
		 * Saves the state of inventory
		 * @return save std::string
		 */
		std::string save() const;
		//! Updates the stats of the player
		void updateStats(int menu);
	protected: 
		//! Inventory of the player
		CInventory m_Inventory;
		//! current weapon of the player
		CWeapon * m_Weapon;
		//! What the player wears
		CWear m_Wear;
		//! Stores the players strength
		int m_Strength;
		//! Stores the players endurance
		int m_Endurance;
		//! Stores the players amount of keys
		int m_KeyCnt;
		//! Constant maximum amount of food
		const int m_MaxFood = 200;
		//! Stores the players current food amount
		int m_CurrFood;
		//! Stores the players gold
		int m_Gold;
		//! Stores the ncurses WINDOW for the information screen
		WINDOW * m_Info;
		/**
		 * Handles the players movement:<br>
		 * ----> If the player steps on an item, he attempts to pick it up<br>
		 * ----> If the tile where the player wants to move is a beast, combat starts<br>
		 * ----> If the tile where the player wants to move is a wall, nothing happens<br>
		 * ----> otherwise player moves to the free tile<br>
		 * A popup message if written if something happens<br>
		 */
		int movePos(CFloor * floor, TCoordinates newcoords);
};

#endif //_CPLAYER_H