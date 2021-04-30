/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CPOTION_H
#define _CPOTION_H

#include "CConsumable.hpp"

#include <sstream>

/**
 * CPotion implementation
 * 
 * represents CPotion tiles
 */

class CPotion: public CConsumable {
	public:
		/**
		 * CPotion constructor, used for loading CPotion object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor
		 * @param save      -> std::ifstream from witch the data is loaded
		 * @param glyph     -> that is saved as m_Glyph
		 */
		CPotion(std::ifstream & save, char glyph)
			: CConsumable(save, glyph)
		{
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> m_HealthPoints;
			if(save.fail() || !save.is_open())
				throw "Save corrupted (data corrupted (CPotion))";
		}
		/**
		 * CPotion constructor, used for creating a new CPotion from the given parameters<br>
		 * passes coords, glyph, name and onBoard to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m_Name of the object
		 * @param onBoard     -> defines wether item is still on the board or not, true by default
		 * @param healthPoints  -> to store the m_HealthPoints, representing how much health points this object adds
		 */
		CPotion(TCoordinates coords, char glyph, std::string name, bool onBoard, int healthPoints)
			: CConsumable(coords, glyph, name, onBoard)
			, m_HealthPoints(healthPoints)
		{}
		virtual ~CPotion(){}
		/**
		 * polymorphic implementation of the inherited function for item usage:<br>
		 * attept to heal the player
		 * @param player pointer, to be used by the function
		 * @return Returns if the method was successfull
		 */
		virtual bool useItem(CPlayer * player) override
		{ return player -> heal(m_HealthPoints); }
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override
		{
			std::stringstream save;
			save << CItem::save() << m_HealthPoints << ';';
			return save.str();
		}
		//! @return Returns information about the item
		virtual std::string itemInfo() const override
		{
			std::stringstream info;
			info << name() << ":\n A potion! For health!\n heal: " << m_HealthPoints;
			return info.str();
		}
	protected: 
		//! The number of health points this potion adds
		int m_HealthPoints;
};

#endif //_CPOTION_H