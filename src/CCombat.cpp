/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CCombat.hpp"

#include <random>
#include <sstream>

/**
 * CCombat implementation
 */
CCombat::CCombat(CMovable * initiator, CMovable * victim)
	: m_Initiator(initiator)
	, m_Victim(victim)
{}

std::string CCombat::fight() const
{
	if(!m_Initiator -> isAlive())
		return "Ewww.. this corpse smells";
	std::stringstream msg;
	int r = rand()%101;
	if(m_Victim -> dodgeChance() > r)
		msg << m_Initiator -> name() << " missed " << m_Victim -> name() << "!";
	else
	{
		msg << m_Initiator -> name() << " dealt " <<  m_Victim -> injure(m_Initiator -> damage()) << " damage to " << m_Victim -> name() << ".";
		if(!m_Victim -> isAlive())
			msg << " " << m_Victim -> name() << " died!";
	}
	return msg.str();
}