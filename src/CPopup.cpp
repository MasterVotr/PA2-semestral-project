/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CPopup.hpp"

/**
 * CPopup implementation
 */

CPopup::CPopup(WINDOW * popupwin)
	: m_Popupwin(popupwin)
{}

void CPopup::addMsg(const char * newMsg)
{
	m_MsgTop = m_MsgMid;
	m_MsgMid = m_MsgBot;
	m_MsgBot = newMsg;
}

void CPopup::draw()
{
	clearPopups();
	mvwprintw(m_Popupwin, 0, 1, m_MsgTop.c_str());
	mvwprintw(m_Popupwin, 1, 1, m_MsgMid.c_str());
	mvwprintw(m_Popupwin, 2, 1, m_MsgBot.c_str());
	wrefresh(m_Popupwin);
}

void CPopup::clearPopups()
{
	for (int h = 0; h < m_PopupwinHeight; h++)
		for(int w = 0; w < m_PopupwinWidth; w++)
			mvwaddch(m_Popupwin, h, w, ' ');
}