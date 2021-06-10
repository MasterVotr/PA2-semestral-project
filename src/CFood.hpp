/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CFOOD_H
#define _CFOOD_H

#include "CConsumable.hpp"

#include <sstream>

/**
 * CFood implementation
 * 
 * represents CFood tiles
 */

class CFood: public CConsumable 
{
	public:
		/**
		 * CFood constructor, used for loading CFood object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor
		 * @param save      -> std::ifstream from witch the data is loaded
		 * @param glyph     -> that is saved as m_Glyph
		 */
		CFood(std::ifstream & save, char glyph)
			: CConsumable(save, glyph)
		{
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> m_FoodPoints;
			if(save.fail() || !save.is_open())
				throw "Save corrupted (data corrupted (CFood))";
		}
		/**
		 * CFood constructor, used for creating a new CFood from the given parameters<br>
		 * passes coords, glyph, name and onBoard to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m_Name of the object
		 * @param onBoard     -> defines wether item is still on the board or not, true by default
		 * @param foodPoints  -> to store the m_FoodPoints, representing how much food units this object adds
		 */
		CFood(TCoordinates coords, char glyph , std::string name, bool onBoard , int foodPoints)
			: CConsumable(coords, glyph, name, onBoard)
			, m_FoodPoints(foodPoints)
		{}
		virtual ~CFood(){}
		/**
		 * polymorphic implementation of the inherited function for item usage:<br>
		 * attept to feed the player
		 * @param player pointer, to be used by the function
		 * @return Returns if the method was successfull
		 */
		virtual bool useItem(CPlayer * player) override
		{ return player -> eat(m_FoodPoints); }
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override
		{
			std::stringstream save;
			save << CItem::save() << m_FoodPoints << ';';
			return save.str();
		}
		//! @return Returns information about the item
		virtual std::string itemInfo() const override
		{
			std::stringstream info;
			info << name() << ":\n I found that lying over there, should I eat it?\n food: " << m_FoodPoints;
			return info.str();
		}
	protected: 
		//! The number of food units this food adds
		int m_FoodPoints;
};

#endif //_CFOOD_H