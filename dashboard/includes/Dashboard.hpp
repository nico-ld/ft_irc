/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dashboard.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 13:56:23 by nico              #+#    #+#             */
/*   Updated: 2026/08/28 16:32:14 by nico             ###   ########.fr       */
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
		std::string				_title;
		std::vector<t_section>	_sectionList;

		// Log info
		std::string _logPath;
		std::ofstream _logFile;

	public:
		// Constructor & destructor
		Dashboard(std::string title, std::string logPath);
		~Dashboard();


		// === GLOBAL METHODS ===

		/* > Write log into log file, return -1 on error */
		int		log(const std::string level, const std::string message);

		/* > Erase the full screen to draw the new dashboard */
		void	eraseScreen( void ) const;

		
		/*===========================*\
		|							  |
		|       RENDER MANAGEMENT     |
		|							  |
		\*===========================*/
		
		// === METHODS ===
		/* > Display the dashboard with given information */
		void render( void ) const;

		/* > Print the header of section */
		void printSectionHeader(size_t index) const;

		/* > Print the information column */
		void printColumn(size_t index) const;

		/* > Print the information of two columns */
		void printTwoColumn(size_t index) const;

		/* > Print elemList content */
		void printElemList(size_t index) const;

		/* > Print elemList left and right */
		void printTwoElemList(size_t index) const;
		
		// === GETTERS ===
		/* > Return the number of lines with every sections */
		int	getNbLines( void ) const;

		/* > Return the number of line of the longest element list in the section */
		int getElemLines(t_section section) const;

		
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


		// === BOOLEAN ===
		/* > Check information on section, return True if everything is correct, otherwise False. 
		Also it trigger every flags in function of each element detected */
		bool isSectionValid(t_section &section);
		
		/* > Check section title and main information */
		bool sectionInfoValid(t_section &section);

		/* > Check column informations */
		bool columnInfoValid(t_column &column, std::string sectionTitle);
};

/* > Convert an Int into a string to use it in message or in dashboard */
std::string toStr(int num);

/* > Return current time as a string in Hours:Minutes:Seconds format */
std::string timestamp();

/* > Return a text centered */
std::string centerText(std::string text, size_t width);

/* > Create a string in right format for a double column situation */
std::string createInfo(std::string key, std::string value);
