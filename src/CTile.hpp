/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CTILE_H
#define _CTILE_H

#include "TCoordinates.hpp"

#include <string>
#include <sstream>
#include <fstream>
#include <ncurses.h>

/**
 * CTile implementation
 * 
 * Basic CTile, used to represent objects that can be drawn on the board
 */

class CTile {
	public:
		/**
		 * CTile constructor, used for loading CTile object from an std::ifstream
		 * @param save  -> std::ifstream from witch the data is loaded
		 * @param glyph -> that is saved as m_Glyph
		 */
		CTile(std::ifstream & save, char glyph)
			: m_Glyph(glyph)
		{
			std::string tmp;
			getline(save, tmp, ';');
			std::stringstream ss(tmp);
			ss >> m_Coordinates.t_PosY;
			
			getline(save, tmp, ';');
			ss.clear();
			ss.str(tmp);
			ss >> m_Coordinates.t_PosX;

			if(save.fail() 				  || !save.is_open()		   || 
			   m_Coordinates.t_PosY <= 0  || m_Coordinates.t_PosX <= 0 ||
			   m_Coordinates.t_PosY >= 94 || m_Coordinates.t_PosX >= 94)
				throw "Save corrupted (data corrupted (CTile))";
		}
		/**
		 * CTile constructor, used for creating a new CTile from the given parameters
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 */
		CTile(TCoordinates coordinates, char glyph)
			: m_Coordinates(coordinates)
			, m_Glyph(glyph)
		{}
		virtual ~CTile(){}
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, base function to be implemented by derived classes
		 * @param boardwin WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const = 0;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const
		{ 
			std::stringstream save;
			save << m_Glyph << ";" << m_Coordinates.t_PosY << ";" << m_Coordinates.t_PosX << ';';
			return save.str();
		}
		//! @return the m_Glyph of the object
		int glyph() const
		{ return m_Glyph; }
		//! @return the m_Coordinates of the object 
		TCoordinates getCoords() const
		{ return m_Coordinates; }
	protected: 
		//! Stores the coordinates
		TCoordinates m_Coordinates;
		//! Stores the glyph (visual representation)
		char m_Glyph;
};

#endif //_CTILE_H