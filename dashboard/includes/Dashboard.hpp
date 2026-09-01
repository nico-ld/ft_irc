/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dashboard.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 13:56:23 by nico              #+#    #+#             */
/*   Updated: 2026/09/01 09:42:29 by nico             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/ansi.hpp"

#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <iostream>
#include <iterator>
#include <iomanip>

#define INNER_WIDTH 53 // Between two vertical border
#define COL_WIDTH 25 // Width of each half in two-column rows
#define SEP_WIDTH 3 // Width of separator between two column

#define INFO_LIST std::vector<std::pair<std::string, std::string> > // List of information from dashboard

enum e_side {
	LEFT,
	RIGHT
};

struct t_column {
	bool		hasTitle;
	bool		hasSimpleInfo;
	bool		hasElemList;
	bool		hasContent;
	std::string	title;
	std::string elemListTitle;
	std::vector<std::pair<std::string, std::string> > infoList;
	std::vector<std::vector<std::pair<std::string, std::string> > > elemList;
};

struct t_section {
	bool		hasLeftColumn;
	bool		hasRightColumn;
	bool		hasMainInfo;
	t_column	leftColumn;
	t_column	rightColumn;
	std::string	title;
	std::pair<std::string, std::string>	mainInfo;
};

class Dashboard
{
	private:
		// Dashboard content
		size_t					_lineWritten;
		std::string				_title;
		std::vector<t_section>	_sectionList;

		// Log info
		std::string _logPath;
		std::ofstream _logFile;

		// === RENDER ===
		/* > Print the header of section */
		void printSectionHeader(t_section &section);

		/* > Print columns titles if necessary */
		void printColumnsTitles(t_section &section);

		/* > Print content line by line */
		void printSectionContent(t_section &section);

		/* > Print only one column */
		void oneColumnCase(t_column &column);

		/* > Erase the full screen to draw the new dashboard */
		void	eraseScreen( void ) const;

		
		// === BOOLEAN ===
		/* > Check information on section, return True if everything is correct, otherwise False. 
		Also it trigger every flags in function of each element detected */
		bool isSectionValid(t_section &section);
		
		/* > Check section title and main information */
		bool sectionInfoValid(t_section &section);

		/* > Check column informations */
		bool columnInfoValid(t_column &column, std::string sectionTitle);
		
	public:
		// Constructor & destructor
		Dashboard(std::string title, std::string logPath);
		~Dashboard();


		// === GLOBAL METHODS ===

		/* > Write log into log file, return -1 on error */
		int		log(const std::string level, const std::string message);

		/* > Display the dashboard with given information */
		void 	render( void );

		// === Simple info ===
		
		/* > Return a pointer on the list of simple imformation */
		INFO_LIST	*getSimpleInfo(t_section *section, e_side colSide);
		
		/* > Update an information in a specific section (with index) */
		void	updateInfo(t_section *section, e_side colSide, size_t index, std::string newValue);

		/* > Increase a numeric info by one */
		void	increaseInfo(t_section *section, e_side colSide, size_t index);

		/* > Decrease a numeric info by one */
		void	decreaseInfo(t_section *section, e_side colSide, size_t index);

		/* > Add a new simple information */
		void	addSimpleInfo(t_section *section, e_side colSide, std::string key, std::string value);

		/* > Remove a simple information */
		void	removeSimpleInfo(t_section *section, e_side colSide, size_t index);

		// === List of elements === 
		
		/* > Return the list of element */
		std::vector<INFO_LIST> *getElemList(t_section *section, e_side colSide);

		/* > Return a point on an element from the element list */
		INFO_LIST *getElem(t_section *section, e_side colSide, size_t index);
		
		/* > Add an element to the element list */
		void	addElem(t_section *section, e_side colSide, INFO_LIST newElem);
						
		/* > Remove an element from the element list */
		void 	removeElem(t_section *section, e_side colSide, size_t index);

		/* > Set an element with new values */
		void setElem(t_section *section, e_side colSide, size_t index, INFO_LIST newElem);
				

		/*===========================*\
		|							  |
		|      SECTION MANAGEMENT     |
		|							  |
		\*===========================*/
		
		/* > Add a new section on dashboard */
		void	addSection(t_section &newSection);

		/* > Remove a section on dashboard (with title) */
		void	removeSection(std::string &title);

		/* > Remove a section on dashboard (with index) */
		void	removeSection(size_t index);
		
		// === GETTERS ===		
		/* > Get the full list of sections */
		std::vector<t_section> getSectionList( void ) const;

		/* > Get a specific section of dashboard with the title */
		t_section *getSectionByTitle(std::string &title);

		/* > Get a specific section of dashboard with the index */
		t_section *getSectionByIndex(size_t index);
		
		
		// === SETTERS ===
		/* > Set information for a section (with title) */
		void setSection(std::string &title, t_section &newSection);

		/* > Set information for a section (with index) */
		void setSection(size_t index, t_section &newSection);

		/* > Update the main information of a section */
		void	updateMainInfo(t_section *section, std::string newValue);


};

/* > Convert an Int into a string to use it in message or in dashboard */
std::string toStr(int num);

/* > Convert a string into a Int */
int 		toInt(std::string str);

/* > Return current time as a string in Hours:Minutes:Seconds format */
std::string timestamp();

/* > Return a text centered */
std::string centerText(std::string text, size_t width);

/* > Create a string in right format for a double column situation */
std::string createInfo(std::string key, std::string value);

/* > Helper for user methods, return true if the section is NULL */
bool sectionIsNull(t_section *section, Dashboard *dash);

/* > Helper for user methods, return true if the index is out of range */
bool indexOutOfRange(Dashboard *dash, t_section *section, size_t index, size_t size);
