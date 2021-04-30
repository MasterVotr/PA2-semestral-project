/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CMOVABLE_H
#define _CMOVABLE_H

#include "CTile.hpp"
#include "CPopup.hpp"

#include <string>

class CFloor;

/**
 * CMovable implementation
 * 
 * CMovable is for objects that can move
 */

class CMovable: public CTile 
{
	public: 
		/**
		 * CMovable constructor, used for loading CMovable object from an std::ifstream<br>
		 * passes save, glyph and popups to his base class constructor
		 * @param save   -> std::ifstream from witch the data is loaded
		 * @param glyph  -> that is saved as m_Glyph
		 * @param popups -> to store CPopup pointer, so the movable is able to use popup messages
		 */
		CMovable(std::ifstream & save, char glyph, CPopup * popups);
		/**
		 * CMovable constructor, used for creating a new CMovable from the given parameters<br>
		 * passes save and glyph to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m-Name of the object
		 * @param alive		  -> to store wether the movable is alive or not
		 * @param maxHP		  -> to store the m_MaxHP
		 * @param currHP	  -> to store the m_CurrHP
		 * @param dex		  -> to store the amount of m_Dexterity
		 * @param popups	  -> to store CPopup pointer, so the movable is able to use popup messages
		 */
		CMovable(TCoordinates coords, char glyph, std::string name, bool alive, int maxHP, int currHP, int dex, CPopup * popups);
		virtual ~CMovable(){}
		/**
		 * Base virtual method for derived classes to implement their movement
		 * @param floor -> CFloor pointer, used to get information about the current floor
		 */
		virtual int move(CFloor * floor) = 0;
		//! Base virtual method for derived classes to implement how much damage they deal
		virtual int damage() const = 0;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override;
		/**
		 * CMovable "takes damage"
		 * @param damage taken
		 * @return Returns damage received
		 */
		virtual int injure(int damage);
		//! @return Returns the name of the movable
		const std::string & name() const;
		//! Return wether the movable is alive
		bool isAlive() const;
		//! @param change Applies the change to m_MaxHP
		void changeHP(int change);
		//! @return Returns how much the movable has m_MaxHP
		int maxHP() const;
		//! @return Returns how much the movable has m_CurrHP
		int currHP() const;
		//! @param coords Sets the m_Coordinates of the movable
		void setCoordinates(TCoordinates coords);
		//! @return Returns the chance to dodge an attack
		int dodgeChance() const;
	protected: 
		//! Stores the name  of the CMovable object
		std::string m_Name;
		//! Stores wether the CMovable is alive or dead
		bool m_Alive;
		//! Stores the maximum amount of health points the movable has
		int m_MaxHP;
		//! Stores the current amount of health points the movable has
		int m_CurrHP;
		//! Stores how much dexterity the movable has
		int m_Dexterity;
		//! Stores CPopup pointer, so the movable is able to use popup messages
		CPopup * m_Popups;
};

#endif //_CMOVABLE_H