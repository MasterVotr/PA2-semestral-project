/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CWEAR_H
#define _CWEAR_H

#include "CWearable.hpp"
#include "CItem.hpp"

#include <vector>

class CPlayer;

/**
 * CWear implementation
 * 
 * represents the players equipment
 */

class CWear 
{
	public:
		/**
		 * CWear constructor takes player pointer and stores it
		 * @param player -> CPlayer pointer
		 */
		CWear(CPlayer * player);
		//! CWear destructor takes care of the memory allocated by the equipment
		~CWear();
		/**
		 * Equips the wearable provided, if possible<br>
		 * 1. Checks what kind of item the input is<br>
		 * 2.-->if the slot is empty -> equips item<br>
		 * ---->if the slot if full -> exchanges the input item and the stored item and returns the old item to be stored back in the inventory<br>
		 * @param item -> CItem pointer to be equiped<br>
		 */
		bool equip(CWearable * wearable);
		/**
		 * Unequip the given ArmorType wearable
		 * @param type -> ArmorType to be equiped
		 * @return success
		 */
		bool unequip(ArmorType type);
		/**
		 * @param type -> ArmorType to be unequiped
		 * @return name of equip in "slot" type or nullptr if type not found
		 */
		const char * equipmentName(ArmorType type);
		//! @return Return the total armor bonus
		int getArmorBonus() const;
		//! @return Return the total health bonus
		int getHealthBonus() const;
		//! @return Return the total damager bonus
		int getDamageBonus()const;
		/**
		 * Saves the state of equipment (wear)
		 * @return save std::string
		 */
		std::string save() const;
		/**
		 * Loads the state of equipment from ifstream
		 * @param save std::ifstream reference
		 */
		void load(std::ifstream & save);
	protected:
		//! Stores the equipment
		std::vector<CWearable *> m_Equipment;
		//! Stores the player pointer
		CPlayer * m_Player;
};

#endif //_CWEAR_H