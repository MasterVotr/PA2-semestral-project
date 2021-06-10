/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CWEARABLE_H
#define _CWEARABLE_H

#include "CItem.hpp"
#include "EArmorType.hpp"
#include "EBonusType.hpp"

/**
 * CWearable implementation
 */

class CWearable: public CItem 
{
	public:
		/**
		 * CWearable constructor, used for loading CWearable object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor
		 * @param save      -> std::ifstream from witch the data is loaded
		 * @param glyph     -> that is saved as m_Glyph
		 */
		CWearable(std::ifstream & save, char glyph);
		/**
		 * CWearable constructor, used for creating a new CWearable from the given parameters<br>
		 * passes coords, glyph, name and onBoard to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m_Name of the object
		 * @param onBoard     -> defines wether item is still on the board or not, true by default
		 * @param armorType	  -> defines what ArmorType this wearable is
		 * @param bonusType	  -> defines the BonusType this wearable gives
		 * @param bonus		  -> to store the m_Bonus, amount of bonus this wearable gives
		 */
		CWearable(TCoordinates coords, char glyph, std::string name, bool onBoard, ArmorType armorType, BonusType bonusType, int bonus);
		virtual ~CWearable(){}
		/**
		 * polymorphic implementation of the inherited function for item usage:<br>
		 * attept to equip the wearable to the player
		 * @param player pointer, to be used by the function
		 * @return Returns if the method was successfull
		 */
		virtual bool useItem(CPlayer * player) override;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override;
		//! @return Returns the ArmorType of the wearable
		ArmorType armorType() const;
		//! @return Returns the BonusType the wearable gives
		BonusType bonusType() const;
		//! @return Returns the amount of bonus the wearable gives
		int bonus() const;
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, polymophic override of draw() function from CTile
		 * @param boardwin WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const override;
		//! @return Returns information about the item
		virtual std::string itemInfo() const override;
	protected:
		//! he type of armor(head, body, leggins, ring, amulet,...) this Wearable is
		ArmorType m_ArmorType;
		//! The type of bonus(armor, health, damage,...) this Wearable adds
		BonusType m_BonusType;
		//! The amount of bonus this Wearable gives
		int m_Bonus;
};

#endif //_CWEARABLE_H