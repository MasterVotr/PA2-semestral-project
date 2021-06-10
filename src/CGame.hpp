/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CGAME_H
#define _CGAME_H

#include "CPlayer.hpp"
#include "CFloor.hpp"
#include "CPopup.hpp"

#include <ncurses.h>
#include <vector>

/**
 * CGame is the main class, where all the magic happens.
 */

class CGame
{
	public:
		//! CGame constructor, initializes and sets ncurses. <br>Allocates a CPopup 
		CGame();
		//! CGame destructor, takes care of m_Player, m_Floor and m_Popups
		~CGame();
		//! CGame copy constructor is not needed
		CGame(const CGame & src) = delete;
		//! CGame operator = is not needed
		CGame & operator = (const CGame src) = delete;
		/**
		 * Menu(), based on the result: <br>
		 * -> if Load is selected -> Load() is called and a saved game is loaded <br>
		 * -> if Exit is selected -> the game shuts down <br>
		 * -> if New game is selected <br>
		 * ----> new CFloor is created <br>
		 * ----> floor.init() -> initializes the floor(beasts, doors, the player, items) <br>
		 * ----> PlayerCreation() is called and the user will create a configure a new player <br>
		 * first render of popups, floor and player <br>
		 * while(42) -> Game loop <br>
		 * { <br>
		 * ----> User input is handled <br>
		 * ----> if save is selected -> the game saves it self <br>
		 * ----> if quit is selected -> quit() is called -> if return is true -> game exits <br>
		 * ----> otherwise m_Player -> move() handles the input <br>
		 * ----> All beasts move <br>
		 * ----> lose condition (m_Player -> isAlive()) is checked -> if false -> credits(0) <br>
		 * ---->render of popups, floor and player <br>
		 * } <br>
		 *  <br>
		 * if the game was won -> end credits -> congratulations -> program shuts down <br>
		 * if the game was lost(player died) -> end credits -> better luck next tme -> program shuts down <br>
		 */
		void start();
	protected:
		//! ncurses WINDOWs for each window
		WINDOW * m_Board,
			   * m_Popup;
		//! current floor is stored here
		CFloor * m_Floor;
		//! current player is stored here
		CPlayer * m_Player;
		//! Stores popups pointer
		CPopup * m_Popups;
		//! Screen parameters
		const int m_ScrHeight = 30,
				  m_ScrWidth = 120;
		/**
		 * Menu, returns decisions <br>
		 * 0 -> new game <br>
		 * 1 -> load <br>
		 * 2 -> exit
		 * @return decision
		 */
		int  menu();
		//! Game saves
		void saveGame();
		//! Game loads @return success
		bool load(const char * savefile);
		//! Load menu
		bool loadMenu();
		//! Player creation menu
		void playerCreation();
		//! Draw UI borders
		void drawBorders();
		//! Quit menu
		bool quit();
		/**
		 * End credits
		 * @param win
		 */
		void credits(bool win);
		/**
		 * clears the given ncurses WINDOW
		 * @param win -> WINDOW pointer
		 * @param height
		 * @param width
		 */
		void clearWin(WINDOW * win, int height, int width);
};

#endif //_CGAME_H