/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CDOOR_H
#define _CDOOR_H

#include "CStatic.hpp"

/**
 * Represents door tiles
 */

class CDoor: public CStatic {
	public:
		/**
		 * CDoor constructor, used for loading CDoor object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor<br>
		 * @param save  -> std::ifstream from witch the data is loaded
		 * @param glyph -> that is saved as m_Glyph
		 */
		CDoor(std::ifstream & save, char glyph);
		/**
		 * CStatic constructor, used for creating a new CStatic from the given parameters<br>
		 * passes coords and glyph to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param locked      -> to store wether the door is locked or not
		 */
		CDoor(TCoordinates coords, char glyph, bool locked);
		virtual ~CDoor(){}
		//! @return Returns wether the door is locked or not
		bool isLocked() const;
		//! Unlocks the door
		void Unlock();
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, polymophic override of draw() function from CTile
		 * @param boardwin WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const override;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override;
	protected: 
		//! Holds the state of the door (locked/unlocked)
		bool m_Locked;
};

#endif //_CDOOR_H