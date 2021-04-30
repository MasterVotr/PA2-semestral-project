/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */

#ifndef _CCombat_H
#define _CCombat_H

#include "CMovable.hpp"

#include <string>

/**
 * CCombat implementation
 * 
 * represents combat between two movables
 */

class CCombat
{
	public:
		/**
		 * Constructor for CCombat
		 * @param initiator -> CMovable pointer of initiator
		 * @param victim    -> CMovable pointer of victim
		 */
		CCombat(CMovable * initiator, CMovable * victim);
		/**
		 * 1. Calculates if the initiator has hit the target<br>
		 * 2. If yes, damage dealt is calculated<br>
		 * 3. HP loss using injure()<br>
		 * @return Returns a popup message of the result
		 */
		std::string fight() const;
	protected:
		//! Stores the initiator of the combat
		CMovable * m_Initiator;
		//! Stores the victim of the combat
		CMovable * m_Victim;
};

#endif //_CCombat_H