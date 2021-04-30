/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CCONSUMABLE_H
#define _CCONSUMABLE_H

#include "CItem.hpp"
#include "CPlayer.hpp"

/**
 * CConsumable implementation
 * 
 * represents CConsumable tiles
 */

class CConsumable: public CItem 
{
	public:
		/**
		 * CConsumable constructor, used for loading CConsumable object from an std::ifstream<br>
		 * passes save, glyph and singleUse (true) to his base class constructor
		 * @param save      -> std::ifstream from witch the data is loaded
		 * @param glyph     -> that is saved as m_Glyph
		 */
		CConsumable(std::ifstream & save, char glyph)
			: CItem(save, glyph, true)
		{}
		/**
		 * CConsumable constructor, used for creating a new CConsumable from the given parameters<br>
		 * passes coords, glyph, name, onBoard and singleUse (true) to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m_Name of the object
		 * @param onBoard     -> defines wether item is still on the board or not, true by default
		 */
		CConsumable(TCoordinates coords, int glyph, std::string name, bool onBoard = true)
			: CItem(coords, glyph, name, onBoard, true)
		{}
		virtual ~CConsumable(){}
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, polymophic override of draw() function from CTile
		 * @param boardwin WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const override
		{ 
			if(!m_OnBoard)
				return;
			init_pair(3, COLOR_GREEN, COLOR_BLACK);
			wattrset(boardwin, COLOR_PAIR(3));
			mvwaddch(boardwin, m_Coordinates.t_PosY, m_Coordinates.t_PosX, m_Glyph);
			wattron(boardwin, COLOR_PAIR(3));
		}
};

#endif //_CCONSUMABLE_H