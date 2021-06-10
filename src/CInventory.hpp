/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CInventory_H
#define _CInventory_H

#include "CItem.hpp"

#include <vector>

class CPlayer;

/**
 * CInventory implementation
 * 
 * Players Inventory
 * holds CItems and their count
 */

class CInventory 
{
	public:
		/**
		 * CInventory constructor takes player pointer and stores it
		 * @param player -> CPlayer pointer
		 */
		CInventory(CPlayer * player);
		//! CInventory destructor takes care of the memory allocated by the items
		~CInventory();
		//! player inventory management
		void show();
		/**
		 * Adds an item to inventory, if not full
		 * @param item -> CItem pointer
		 */
		bool addItem(CItem * item);
		//! @return Return wether the inventory is full
		bool isFull() const;
		/**
		 * Saves the state of inventory
		 * @return save std::string
		 */
		std::string save() const;
		/**
		 * Loads the state of inventory from ifstream
		 * @param save -> std::ifstream reference
		 */
		void load(std::ifstream & save);
	protected: 
		//! Holds the content of Inventory CItem pointers
		std::vector<CItem*> m_Items;
		//! Constant inventory site
		const uint m_InventorySize = 8;
		//! Stores the player pointer
		CPlayer * m_Player;
		//! Stores the screen paramenters
		const int m_ScrHeight = 30,
				  m_ScrWidth = 120;
		//! Stores the ncurses WINDOW pointer
		WINDOW * m_Inventwin;
		//! Stores the inventory menu paramenters
		const int m_InventHeight = 24,
				  m_InventWidth  = 94;
		/**
		 * Use an item at postion
		 * @param position
		 */
		bool useItem(int position);
		/**
		 * Discards an item from inventory at position
		 * @param position
		 */
		void discard(int position);
};

#endif //_CInventory_H