/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */


#include "CGame.hpp"

#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <fstream>

#define CTRL(k) ((k)  & 31)	// Used to simulate Ctrl + KEY for ncurses

/**
 * CGame implementation
 */


CGame::CGame()
	: m_Floor(nullptr)
	, m_Player(nullptr)
{
	/* NCURSES INIT */
	initscr();
	raw();
	noecho();
	curs_set(0);
	m_Board = newwin(24, 94, 1, 1);
	m_Popup = newwin(3, 118, 26, 1);
	keypad(m_Board, true);
	keypad(m_Popup, true);
	start_color();
	m_Popups = new CPopup(m_Popup);
}

CGame::~CGame()
{
	if(m_Floor != nullptr)
		delete m_Floor;
	if(m_Player != nullptr)
		delete m_Player;
	delete m_Popups;

	/* NCURSES END*/
	delwin(m_Board);
	delwin(m_Popup);
	endwin();
}

void CGame::start()
{
	try{
		bool inMenus = true;
		while(inMenus)
			switch (menu())
			{
				case 0: // New Game
					playerCreation();
					m_Floor = new CFloor("level.map", m_Board, m_Player, m_Popups);
					m_Floor -> init();
					inMenus = false;
					break;
				case 1: // Load
					if(loadMenu())
						inMenus = false;
					break;
				default: // Exit
					return;
			}
		drawBorders();
		m_Player -> info(0);
		m_Floor -> draw();
		m_Popups -> addMsg("Where am I?");
		m_Popups -> addMsg("I should have a look around, maybe I will find something useful.");
		m_Popups -> draw();
		while(42)
		{
			if(LINES < m_ScrHeight || COLS < m_ScrWidth)
				throw "Please resize your terminal to at least 120x30 and relaunch the game";
			else
				curs_set(0);
			switch(m_Player -> move(m_Floor))
			{
			// QUIT
			case CTRL('q'):
				if(quit())
					return;
				else
					break;
			// SAVE
			case CTRL('s'):
				saveGame();
				drawBorders();
				break;
			// WIN
			case 1 :
				credits(true);
				return;
			}
			for(auto beast : m_Floor -> beasts())
			{
				if(int g = beast -> move(m_Floor))
				{
					m_Player -> pickupGold(g);
					std::stringstream ss;
					ss << "Picked up " << g << " gold";
					m_Popups -> addMsg(ss.str().c_str());
				}
				wrefresh(m_Board);
			}
			if(!m_Player -> isAlive())
			{
				credits(false);
				return;
			}
			m_Player -> info(0);
			m_Floor -> draw();
			m_Popups -> draw();
			napms(10); 
		}
	} catch (const char * e)
	{
		clearWin(stdscr, m_ScrHeight, m_ScrWidth);
		mvprintw(0,0,e);
		refresh();
		getch();
		endwin();
		std::cout << "Error: " << e << std::endl;
	}
}

bool CGame::loadMenu()
{
	const int loadHeight = 8;
	const int loadWidth  = 50;
	WINDOW * loadwin = newwin(loadHeight, loadWidth, (m_ScrHeight-loadHeight)/2, (m_ScrWidth-loadWidth)/2);
	box(loadwin, 0, 0);
	refresh();
	wrefresh(loadwin);
	keypad(loadwin, true);

	std::string name;
	int loadOption;
	bool gameLoaded = false;

	while(42)
	{
		if(LINES < m_ScrHeight || COLS < m_ScrWidth)
			throw "Please resize your terminal to at least 120x30 and relaunch the game";
		clearWin(loadwin, loadHeight, loadWidth);
		box(loadwin, 0, 0);
		mvwprintw(loadwin, 2, (loadWidth - 21)/2, "save name: %s", name.c_str());
		mvwprintw(loadwin, 6, (loadWidth - 29)/2, "load: Enter    back: <Ctrl-Q>", name.c_str());
		try
		{	
			switch (loadOption = wgetch(loadwin))
			{
				case KEY_BACKSPACE:
					if(name.length() == 0)
						break;
					name.pop_back();
					break;
				case 10:
					if(name.length() < 1)
						break;
					if(load(name.c_str()))
					{
						gameLoaded = true;
						clearWin(loadwin, loadHeight, loadWidth);
						delwin(loadwin);
						return gameLoaded;
					}
					clearWin(loadwin, loadHeight, loadWidth);
					box(loadwin, 0, 0);
					break;
				case CTRL('q'):
					clearWin(loadwin, loadHeight, loadWidth);
					delwin(loadwin);
					return gameLoaded;
				default:
					if(!isalpha(loadOption) || name.length() >= 10)
						break;
					name.push_back(loadOption);
					break;
			}
		} catch(const char * e)
		{
			mvwprintw(loadwin, 4, 1, "                                            ");
			mvwprintw(loadwin, 4, (loadWidth - 7 - strlen(e))/2, "Error: %s",e);
			wgetch(loadwin);
		} catch(char e)
		{
			mvwprintw(loadwin, 4, 1, "                                            ");
			mvwprintw(loadwin, 4, (loadWidth - 25)/2, "Error: Uknown glyph '%c'",e);
			wgetch(loadwin);
		} catch(int e)
		{
			mvwprintw(loadwin, 4, 1, "                                            ");
			mvwprintw(loadwin, 4, (loadWidth - 26)/2, "Error: Uknown number '%d'",e);
			wgetch(loadwin);
		}
		napms(10);
	}
}

bool CGame::load(const char * savefile)
{
	// LOADING PLAYER
	std::ifstream playerSave;
	try
	{
		playerSave.open(std::string("./saves/").append(savefile).append(".psave").c_str());
		if(!playerSave.good() || !playerSave.is_open())
			throw "Save corrupted (file not found)";
		char glyph = 0, semicolon = 0;
		playerSave >> glyph >> semicolon; if(semicolon != ';') throw "Save corrupted(; missing)";
		if(glyph != '@')
			throw "Save corrupted (player missing)";
		m_Player = new CPlayer(playerSave, m_Popups);
		playerSave.close();
	} catch(const char * e)
	{
		playerSave.close();
		throw e;
	} catch(char e)
	{
		playerSave.close();
		throw e;
	} catch(int e)
	{
		playerSave.close();
		throw e;
	}
	// LOADING FLOOR
	std::ifstream floorSave;
	try
	{
		floorSave.open(std::string("./saves/").append(savefile).append(".msave").c_str());
		if(!floorSave.good() || !floorSave.is_open())
			throw "Save corrupted (file not found)";
		m_Floor = new CFloor(m_Board, m_Player, m_Popups);
		m_Floor -> load(floorSave);
		floorSave.close();
	} catch(const char * e)
	{
		floorSave.close();
		throw e;
	} catch(char e)
	{
		floorSave.close();
		throw e;
	} catch(int e)
	{
		floorSave.close();
		throw e;
	}
	// LOADING DONE
	return true;
}

void CGame::saveGame()
{
	const int savemenuHeight = 6;
	const int savemenuWidth  = 37 + m_Player -> name().size();
	WINDOW * savemenu = newwin(savemenuHeight, savemenuWidth, (m_ScrHeight-savemenuHeight)/2, (m_ScrWidth-savemenuWidth)/2);
	box(savemenu, 0, 0);
	refresh();
	wrefresh(savemenu);
	keypad(savemenu, true);

	mvwprintw(savemenu, 2, (savemenuWidth - (15 + m_Player -> name().size()))/2, "Saving under %s", m_Player -> name().c_str());
	wrefresh(savemenu);

	// SAVING MAP
	system("mkdir saves 2>/dev/null 1>&2");
	std::string filename = "./saves/";
	filename.append(m_Player -> name());
	filename.append(".msave");
	std::ofstream output(filename.c_str(), std::ofstream::trunc);
	if(output.fail() || !output.is_open())
		throw "Saving error!";
	output << m_Floor -> save();
	if(output.fail() || !output.is_open())
		throw "Saving error!";
	output.close();
	// SAVING PLAYER
	filename = "./saves/";
	filename.append(m_Player -> name());
	filename.append(".psave");
	output.open(filename.c_str(), std::ofstream::trunc);
	if(output.fail() || !output.is_open())
		throw "Saving error!";
	output << m_Player -> save();
	if(output.fail() || !output.is_open())
		throw "Saving error!";
	output.close();
	// SAVING DONE

	mvwprintw(savemenu, 4, (savemenuWidth-27)/2, "<Press any key to continue>");
	wgetch(savemenu);
	clearWin(savemenu, savemenuHeight, savemenuWidth);
	delwin(savemenu);
}

void CGame::playerCreation()
{
	const int menuHeight = 12;
	const int menuWidth  = 24;
	WINDOW * menuwin = newwin(menuHeight, menuWidth, (m_ScrHeight-menuHeight)/2, (m_ScrWidth-menuWidth)/2);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	keypad(menuwin, true);

	std::string name;
	int attributeCnt = 3;
	std::vector<std::string> attributes = {"Strength", "Endurance", "Dexterity"};	// 14
	std::vector<int> attributeVal = {3, 3, 3};
	int menuOption;
	int highlight = 0;
	int unspentPoints = 3;

	while(42)
	{
		if(LINES < m_ScrHeight || COLS < m_ScrWidth)
			throw "Please resize your terminal to at least 120x30 and relaunch the game";
		clearWin(menuwin, menuHeight, menuWidth);
		box(menuwin, 0, 0);

		mvwprintw(menuwin, 2, (menuWidth - 16)/2, "name: %s", name.c_str());
		mvwprintw(menuwin, 4, (menuWidth - 16)/2, "Unspent points: %d", unspentPoints);
		for(int i = 0; i < attributeCnt; i++)
		{
			mvwprintw(menuwin, i + menuHeight - attributes.size() - 2, (menuWidth - 14)/2, attributes[i].c_str());
			if(i == highlight)
				wattron(menuwin, A_REVERSE);
			mvwprintw(menuwin, i + menuHeight - attributes.size() - 2, (menuWidth - 14)/2 + 10, "< %d >", attributeVal[i]);
			wattroff(menuwin, A_REVERSE);
		}
		switch (menuOption = wgetch(menuwin))
		{
			case KEY_UP:
				if(--highlight == -1)
					highlight = attributeCnt-1;
				break;
			case KEY_DOWN:
				if(++highlight == attributeCnt)
					highlight = 0;
				break;
			case KEY_LEFT:
				if(attributeVal[highlight] <= 1)
					break;
				--attributeVal[highlight];
				++unspentPoints;
				break;
			case KEY_RIGHT:
				if(unspentPoints == 0)
					break;
				++attributeVal[highlight];
				--unspentPoints;
				break;
			case KEY_BACKSPACE:
				if(name.length() == 0)
					break;
				name.pop_back();
				break;
			case 10:
				if(name.length() < 1)
					break;
				m_Player = new CPlayer(TCoordinates(0,0), '@', name, true, 10 + attributeVal[1]*5, 10 + attributeVal[1]*5, attributeVal[2], attributeVal[0], attributeVal[1], m_Popups);
				clearWin(menuwin, menuHeight, menuWidth);
				delwin(menuwin);
				return;
			default:
				if(!isalpha(menuOption) || name.length() >= 10)
					break;
				name.push_back(menuOption);
				break;
		}
		napms(10);
	}
}

int CGame::menu()
{
	const int menuHeight = 14;
	const int menuWidth  = 30;
	WINDOW * menuwin = newwin(menuHeight, menuWidth, (m_ScrHeight-menuHeight)/2, (m_ScrWidth-menuWidth)/2);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	keypad(menuwin, true);

	int choiceCnt = 3;
	std::vector<std::string> choices = {"New Game", "Load", "Exit"};
	int menuOption;
	int highlight = 0;

	if(!has_colors())
	{
		mvwprintw(menuwin, 1, 1, "Your terminal does not support colors");
		refresh();
	}

	while(42)
	{
		if(LINES < m_ScrHeight || COLS < m_ScrWidth)
			throw "Please resize your terminal to at least 120x30 and relaunch the game";
		// Write header
		mvwprintw(menuwin, 2, (menuWidth - 24)/2, "#### #  #  ###  ##  ### ");
		mvwprintw(menuwin, 3, (menuWidth - 24)/2, "#    #  # #    #  # #  #");
		mvwprintw(menuwin, 4, (menuWidth - 24)/2, "###  #  # # ## #  # ### ");
		mvwprintw(menuwin, 5, (menuWidth - 24)/2, "#    #  # #  # #  # # # ");
		mvwprintw(menuwin, 6, (menuWidth - 24)/2, "####  ##   ###  ##  #  #");

		// Write out all choices
		for(int i = 0; i < choiceCnt; i++)
		{
			if(i == highlight)
				wattron(menuwin, A_REVERSE);
			mvwprintw(menuwin, i + menuHeight - choices.size() - 2, (menuWidth - choices[i].length())/2, choices[i].c_str());
			wattroff(menuwin, A_REVERSE);
		}
		// Get input and process it
		switch (menuOption = wgetch(menuwin))
		{
			case KEY_UP:
				if(--highlight == -1)
					highlight = choiceCnt-1;
				break;
			case KEY_DOWN:
				if(++highlight == choiceCnt)
					highlight = 0;
				break;
			case 10:
				clearWin(menuwin, menuHeight, menuWidth);
				delwin(menuwin);
				return highlight;
			default:
				break;
		}
		napms(10);
	}
}

void CGame::drawBorders()
{
	mvprintw( 0, 0, "########################################################################################################################");
	mvprintw( 1, 0, "#                                                                                              #                       #");
	mvprintw( 2, 0, "#                                                                                              #                       #");
	mvprintw( 3, 0, "#                                                                                              #                       #");
	mvprintw( 4, 0, "#                                                                                              #                       #");
	mvprintw( 5, 0, "#                                                                                              #                       #");
	mvprintw( 6, 0, "#                                                                                              #                       #");
	mvprintw( 7, 0, "#                                                                                              #                       #");
	mvprintw( 8, 0, "#                                                                                              #                       #");
	mvprintw( 9, 0, "#                                                                                              #                       #");
	mvprintw(10, 0, "#                                                                                              #                       #");
	mvprintw(11, 0, "#                                                                                              #                       #");
	mvprintw(12, 0, "#                                                                                              #                       #");
	mvprintw(13, 0, "#                                                                                              #                       #");
	mvprintw(14, 0, "#                                                                                              #                       #");
	mvprintw(15, 0, "#                                                                                              #                       #");
	mvprintw(16, 0, "#                                                                                              #                       #");
	mvprintw(17, 0, "#                                                                                              #                       #");
	mvprintw(18, 0, "#                                                                                              #                       #");
	mvprintw(19, 0, "#                                                                                              #                       #");
	mvprintw(20, 0, "#                                                                                              #                       #");
	mvprintw(21, 0, "#                                                                                              #                       #");
	mvprintw(22, 0, "#                                                                                              #                       #");
	mvprintw(23, 0, "#                                                                                              #                       #");
	mvprintw(24, 0, "#                                                                                              #                       #");
	mvprintw(25, 0, "########################################################################################################################");
	mvprintw(26, 0, "#                                                                                                                      #");
	mvprintw(27, 0, "#                                                                                                                      #");
	mvprintw(28, 0, "#                                                                                                                      #");
	mvprintw(29, 0, "########################################################################################################################");
	refresh();
}

bool CGame::quit()
{
	const int menuHeight = 10;
	const int menuWidth  = 36;
	WINDOW * menuwin = newwin(menuHeight, menuWidth, (m_ScrHeight-menuHeight)/2, (m_ScrWidth-menuWidth)/2);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	keypad(menuwin, true);

	int choiceCnt = 2;
	std::vector<std::string> choices = { "No, I shall fight some more!", "Yes, I surrender" };
	int menuOption;
	int highlight = 0;

	while(42)
	{
		// Write out quit question
		mvwprintw(menuwin, 2, (menuWidth - 30)/2, "Are you sure you want to quit?");
		mvwprintw(menuwin, 3, (menuWidth - 32)/2, "(You will lose unsaved progress)");
		// Write out all choices
		for(int i = 0; i < choiceCnt; i++)
		{
			if(i == highlight)
				wattron(menuwin, A_REVERSE);
			mvwprintw(menuwin, i + menuHeight - choices.size() - 2, (menuWidth - choices[i].length())/2, choices[i].c_str());
			wattroff(menuwin, A_REVERSE);
		}
		// Get input and process it
		switch (menuOption = wgetch(menuwin))
		{
			case KEY_UP:
				if(--highlight == -1)
					highlight = choiceCnt-1;
				break;
			case KEY_DOWN:
				if(++highlight == choiceCnt)
					highlight = 0;
				break;
			case 10:
				clearWin(menuwin, menuHeight, menuWidth);
				delwin(menuwin);
				return highlight;
			default:
				break;
		}
		napms(10);
	}
}

void CGame::credits(bool win)
{
	beep();
	clearWin(stdscr, m_ScrHeight, m_ScrWidth);
	if(win)
	{   
		mvprintw( 2, (m_ScrWidth - 36)/2,    "#  #  ##  #  #   # # #  ##  #  # ###");
		mvprintw( 3, (m_ScrWidth - 36)/2,    "#  # #  # #  #   # # # #  # ## # ###");
		mvprintw( 4, (m_ScrWidth - 36)/2,    " ### #  # #  #   # # # #  # # ##  # ");
		mvprintw( 5, (m_ScrWidth - 36)/2,    "   # #  # #  #   # # # #  # #  #    ");
		mvprintw( 6, (m_ScrWidth - 36)/2,    " ##   ##   ##     # #   ##  #  #  # ");
	}
	else
	{
		mvprintw( 2, (m_ScrWidth - 37)/2,    "#  #  ##  #  #   #     ##   ### #####");
		mvprintw( 3, (m_ScrWidth - 37)/2,    "#  # #  # #  #   #    #  # #      #  ");
		mvprintw( 4, (m_ScrWidth - 37)/2,    " ### #  # #  #   #    #  #  ##    #  ");
		mvprintw( 5, (m_ScrWidth - 37)/2,    "   # #  # #  #   #    #  #    #   #  ");
		mvprintw( 6, (m_ScrWidth - 37)/2,    " ##   ##   ##    ####  ##  ###    #  ");
	}
	if(win)
		mvprintw( 8, (m_ScrWidth - 16)/2,              "Congratulations!");
	else
		mvprintw( 8, (m_ScrWidth - 22)/2,           "Better luck next time!");	
	mvprintw( 9, (m_ScrWidth - 10)/2,                    "SCORE: %d", m_Player -> gold());
	mvprintw( 13, (m_ScrWidth - 30)/2,          "Thank you for playing my game!");
	mvprintw( 14, (m_ScrWidth - 21)/2,              "I hope you enjoyed it.");
	mvprintw( 16, (m_ScrWidth - 23)/2,             "Creator: Jakub Votrubec");
	mvprintw( 17, (m_ScrWidth - 18)/2,                "student of CTU FIT");
	mvprintw( 19, (m_ScrWidth - 33)/2,         "created within the subject BI-PA2");
	mvprintw( 21, (m_ScrWidth - 49)/2, "created: june 2020, during the COVID-19 quarantine");
	
	mvprintw( m_ScrHeight - 1, (m_ScrWidth - 23)/2, "<Press any key to exit>");

	refresh();
	getch();
}

void CGame::clearWin(WINDOW * win, int height, int width)
{
	for (int h = 0; h < height; h++)
		for(int w = 0; w < width; w++)
			mvwaddch(win, h, w, ' ');
	wrefresh(win);
}