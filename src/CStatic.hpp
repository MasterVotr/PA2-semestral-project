/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CSTATIC_H
#define _CSTATIC_H

#include "CTile.hpp"

/**
 * CStatic implementation
 * 
 * CStatic class is for objects, that dont move on their own
 */

class CStatic: public CTile {
	public:
		/**
		 * CStatic constructor, used for loading CStatic object from an std::ifstream<br>
		 * passes save and glyph to his base class constructor
		 * @param save  -> std::ifstream from witch the data is loaded
		 * @param glyph -> that is saved as m_Glyph
		 */
		CStatic(std::ifstream & save, char glyph)
			: CTile(save, glyph)
		{}
		/**
		 * CStatic constructor, used for creating a new CStatic from the given parameters<br>
		 * passes save and glyph to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 */
		CStatic(TCoordinates coords, int glyph)
			: CTile(coords, glyph)
		{}
		virtual ~CStatic(){}
};

#endif //_CSTATIC_H