/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CITEM_H
#define _CITEM_H

#include "CStatic.hpp"

#include <string>
#include <ncurses.h>

class CPlayer;

/**
 * CItem implementation
 * 
 * represents item tiles
 */

class CItem: public CStatic 
{
	public:
		/**
		 * CItem constructor, used for loading CItem object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor
		 * @param save      -> std::ifstream from witch the data is loaded
		 * @param glyph     -> that is saved as m_Glyph
		 * @param singleUse -> defines if the object is single use (erased after use), false by default
		 */
		CItem(std::ifstream & save, char glyph, bool singleUse = false)
			: CStatic(save, glyph)
			, m_SingleUse(singleUse)
		{
			getline(save, m_Name, ';');
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> m_OnBoard;
			if(save.fail() || !save.is_open())
				throw "Save corrupted (data corrupted (CItem))";
		}
		/**
		 * CItem constructor, used for creating a new CItem from the given parameters<br>
		 * passes coords and glyph to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m_Name of the object
		 * @param onBoard     -> defines wether item is still on the board or not, true by default
		 * @param singleUse   -> defines if the object is single use (erased after use), false by default
		 */
		CItem(TCoordinates coords, char glyph, std::string name, bool onBoard = true, bool singleUse = false)
			: CStatic(coords, glyph)
			, m_Name(name)
			, m_OnBoard(onBoard)
			, m_SingleUse(singleUse)
		{}
		virtual ~CItem(){}
		/**
		 * base function for item usage, each derived nonbase class implements by itself
		 * @param player pointer, to be used by the function
		 * @return Returns if the method was successfull
		 */
		virtual bool useItem(CPlayer * player) = 0;
		//! @return Returns information about the item
		virtual std::string itemInfo() const = 0;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override
		{
			std::stringstream save;
			save << CTile::save() << m_Name << ';' << m_OnBoard << ';';
			return save.str();
		}
		//! @return Returns the name of the item
		const std::string & name() const
		{ return m_Name; }
		//! @return Returns a reference to the m_OnBoard 
		bool & onBoard()
		{ return m_OnBoard; }
		//! @return Returns wether the item is single use or not
		bool singleUse() const
		{ return m_SingleUse; }
	protected:
		//! Hold the name of the item
		std::string m_Name;
		//! Holds if the item is on the board or not
		bool m_OnBoard;
		//! Holds if the item is single use or not
		bool m_SingleUse = false;
};

#endif //_CITEM_H