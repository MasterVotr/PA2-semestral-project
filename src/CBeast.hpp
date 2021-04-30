/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CBEAST_H
#define _CBEAST_H

#include "CMovable.hpp"
#include "EPathfindingType.hpp"

/**
 * CBeast implementation
 * 
 * reppresents beats tiles
 */

class CBeast: public CMovable {
	public:
		/**
		 * CBeast constructor, used for loading CBeast object from an std::ifstream<br>
		 * passes save, glyph and popups to his base class constructor
		 * @param save   -> std::ifstream from witch the data is loaded
		 * @param glyph  -> that is saved as m_Glyph
		 * @param popups -> to store CPopup pointer, so the movable is able to use popup messages
		 */
		CBeast(std::ifstream & save, char glyph, CPopup * popups);
		/**
		 * CBeast constructor, used for creating a new CBeast from the given parameters<br>
		 * passes save, glyph, name, alive, maxHP, currHP, dex and popups to his base class constructor
		 * @param coordinates -> to store the m_Coordinates of the object
		 * @param glyph		  -> to store the m_Glyph of the object
		 * @param name		  -> to store the m-Name of the object
		 * @param alive		  -> to store wether the movable is alive or not
		 * @param maxHP		  -> to store the m_MaxHP
		 * @param currHP	  -> to store the m_CurrHP
		 * @param dex		  -> to store the amount of m_Dexterity
		 * @param damage	  -> to store the m_Damage, amount of damage the beast deals
		 * @param pathfinding -> The type of pathfinding the beast uses
		 * @param vision	  -> how far the beast "sees"
		 * @param popups	  -> to store CPopup pointer, so the movable is able to use popup messages
		 */
		CBeast(TCoordinates coords, char glyph, std::string name, bool alive,  int maxHP, int currHP, int dex, int damage, PathfindingType pathfinding, int vision, CPopup * popups);
		virtual ~CBeast(){}
		//! overriden virtual method that return how to much damage the beast deals @return Retun the amount of damage the beast deals
		virtual int damage() const override;

		/**
		 * Polymophic implementation of the abstract move() method<br>
		 * 1. The beast "decides" how to move depending on the attribute m_Pathfinding move randomly or seek the player<br>
		 * ----> If m_Pathfinding is set to "seeking" and the beast sees the player(their distance is smaller or equal to m_Vision) it moves towards him, otherwise it also moves randomly<br>
		 * 2. Checks wether it can move there<br>
		 * 3. resolves combat if needed<br>
		 * 4. updates its state and position on the floor<br>
		 * \sa CCombat, nextMove(), randMove(), pathMove()
		 * @param floor -> CFloor pointer, used to get information about the current floor
		 */
		virtual int move(CFloor * floor) override;
		/**
		 * Draws out the tile on the board, using m_Coordinates and m_Glyph, polymophic override of draw() function from CTile
		 * @param boardwin -> WINDOW pointer to the ncurses WINDOW
		 */
		virtual void draw(WINDOW * boardwin) const override;
		/**
		 * Saves the state of this object
		 * @return save std::string
		 */
		virtual std::string save() const override;
	protected:
		//! Damage that the beast deals
		int m_Damage;
		//! Determines the way of movement for the monster (random, seeking,...)
		PathfindingType m_Pathfinding;
		//! How was does the beast "see"
		int m_Vision;
		//! The beast figures out where to move, based on m:Pathfinding
		TCoordinates nextMove(CFloor * floor);
		//! Returns a random position next to the beast to move to
		TCoordinates randMove(CFloor * floor);
		/**
		 * Returns a position to get closer to the target
		 * @param floor  -> CFloor pointer, used to get information about the current floor
		 * @param target ->CMovable pointer target 
		 */
		TCoordinates pathMove(CFloor * floor, CMovable * target);
		};

#endif //_CBEAST_H