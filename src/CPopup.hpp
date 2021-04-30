/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#ifndef _CPOPUP_H
#define _CPOPUP_H

#include <string>
#include <ncurses.h>

/**
 * CPopup implementation
 * 
 * This class handles the Popup window
 */

class CPopup {
	public: 
		/**
		 * CPopup contructor, stores the given WINDOW pointer
		 * @param popupwin -> ncurses WINDOW for popups
		 */
		CPopup(WINDOW * popupwin);
		/**
		 * Adds a new message to m_MsgBot
		 * @param const char * msg
		 */
		void addMsg(const char * msg);
		//! draws(writes out) the messages to m_Popupwin
		void draw();
	private:
		void clearPopups();
	protected: 
		//! Popupwindown parameters
		const int m_PopupwinHeight = 3,
				  m_PopupwinWidth = 118;
		//! Stores the top message
		std::string m_MsgTop;
		//! Stores the middle message
		std::string m_MsgMid;
		//! Stores the bottom message
		std::string m_MsgBot;
		//! Stores the WINDOW * to write popup messages into
		WINDOW * m_Popupwin;
};

#endif //_CPOPUP_H