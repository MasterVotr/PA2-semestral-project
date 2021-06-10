/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CWEAPON_H
#define _CWEAPON_H

#include "CItem.hpp"

#include <string>

/**
 * CWeapon implementation
 * 
 * represents weapon tiles
 */

class CWeapon: public CItem 
{
	public:
		/**
		 * CWeapon constructor, used for loading CWeapon object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor
		 * @param save      -> std::ifstream from witch the data is loaded
		 * @param glyph     -> that is saved as m_Glyph
		 */
		CWeapon(std::ifstream & save, char glyph);
		/**
		 * CWeapon constructor, used for creating a new CWeapon from the given parameters<br>
		 * passes coords, glyph, name and onBoard to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m_Name of the object
		 * @param onBoard     -> defines wether item is still on the board or not, true by default
		 * @param damage	  -> defines how much damage the weapon does, stored in m_Damage
		 */
		CWeapon(TCoordinates coords, char glyph, std::string name , bool onBoard, int damage);
		virtual ~CWeapon(){}
		//! Returns how much damage the weapon does
		int damage() const;
		/**
		 * polymorphic implementation of the inherited function for item usage:<br>
		 * attept to equip the weapon to the player
		 * @param player pointer, to be used by the function
		 * @return Returns if the method was successfull
		 */
		virtual bool useItem(CPlayer * player) override;
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, polymophic override of draw() function from CTile
		 * @param boardwin WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const override;
		//! @return Returns information about the item
		virtual std::string itemInfo() const override;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override;
	protected: 
		//! Stores the amount of damage this weapon does
		int m_Damage;
};

#endif //_CWEAPON_H