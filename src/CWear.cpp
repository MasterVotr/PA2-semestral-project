/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CWear.hpp"
#include "EArmorType.hpp"
#include "EBonusType.hpp"
#include "CPlayer.hpp"

/**
 * CWear implementation
 */

CWear::CWear(CPlayer * player)
	: m_Player(player)
{}

CWear::~CWear()
{
	for(auto wearable : m_Equipment)
		if(wearable != nullptr)
			delete wearable;
}

bool CWear::equip(CWearable * wearable)
{
	if(m_Player -> inventoryFull())
		return false;
	unequip(wearable -> armorType());
	m_Equipment.push_back(wearable);
	return true;
}

bool CWear::unequip(ArmorType type)
{
	for(CWearable * wearable : m_Equipment)
		if(wearable -> armorType() == type && m_Player -> pickup(wearable))
		{
			wearable = m_Equipment[m_Equipment.size() - 1];
			m_Equipment.pop_back();
			return true;
		}
	return false;
}

int CWear::getArmorBonus() const
{
	int armor = 0;
	if(m_Equipment.empty()) 
		return armor;
	for(const auto wearable : m_Equipment)
		if(wearable->bonusType() == BonusType::ARMOR)
			armor += wearable->bonus();
	return armor;
}

const char * CWear::equipmentName(ArmorType type)
{
	for(auto wearable : m_Equipment)
		if(wearable -> armorType() == type)
			return wearable -> name().c_str();
	return "nothing";
}

int CWear::getHealthBonus() const
{
	int health = 0;
	if(m_Equipment.empty()) 
		return health;
	for(const auto wearable : m_Equipment)
		if(wearable->bonusType() == BonusType::HEALTH)
			health += wearable->bonus();
	return health;
}

int CWear::getDamageBonus() const
{
	int damage = 0;
	if(m_Equipment.empty()) 
		return damage;
	for(const auto wearable : m_Equipment)
		if(wearable->bonusType() == BonusType::DAMAGE)
			damage += wearable->bonus();
	return damage;
}

std::string CWear::save() const
{
	std::stringstream save;
	save << m_Equipment.size() << '\n';
	for(auto item : m_Equipment)
	{
		if(item == nullptr)
			continue;
		save << item -> save() << '\n';
	}
	return save.str();
}

void CWear::load(std::ifstream & save)
{
	int equipmentCnt;
	save >> equipmentCnt;
	if(!equipmentCnt)
		return;
	if(equipmentCnt > 5 || equipmentCnt < 0)
		throw "Save corrupted (wrong equipment count (CWear))";
	for(int i = 0; i < equipmentCnt; i++)
	{
		char glyph;
		std::string tmp;
		getline(save, tmp, ';');
		std::stringstream ss(tmp);
		ss >> glyph;
		if(!equip(new CWearable(save, glyph)))
			throw "Save corrupted (wrong equipment count (CWear))";
		if(save.fail())
			throw "Save corrupted (data corrupted (CWear))";
	}
}
