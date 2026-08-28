/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dashboard.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nico <nico@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 13:56:23 by nico              #+#    #+#             */
/*   Updated: 2026/08/27 18:09:12 by nico             ###   ########.fr       */
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
#define COL_WIDTH 25 // width of each half in two-column rows

struct t_column {
	std::string	title;
	std::vector<std::pair<std::string, std::string> > infoList;
	std::vector<std::pair<std::string, std::string> > elemInfoList;
};

struct t_section {
	bool		secondColumn;
	t_column	leftColumn;
	t_column	rightColumn;
	std::string	title;
	std::pair<std::string, std::string>	mainStatInfo;
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
		int		log(const std::string &level, const std::string &message);

		
		/*===========================*\
		|							  |
		|       RENDER MANAGEMENT     |
		|							  |
		\*===========================*/
		
		// === METHODS ===
		/* > Display the dashboard with given information */
		void render( void ) const;

		/* > Print the title of the dashboard */
		void printDashTitle( void ) const;

		/* > Print the header of section */
		void printSectionHeader(size_t index) const;

		/* > The bottom border of dashboard */
		void printDashBottom( void ) const;

		/* > Print a line of '-' between two section */
		void printSeparator( void ) const;

		/* > Print the information column */
		void printColumn(size_t index) const;

		/* > Print the information of two columns */
		void printTwoColumn(size_t index) const;
		
		// === GETTERS ===
		/* > Return the number of lines with every sections */
		int	getNbLines( void ) const;

		
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
		/* > Check information on section, return True if everything is correct, otherwise False */
		bool isSectionValid(t_section &section);
		
};

/* > Convert an Int into a string to use it in message or in dashboard */
std::string toStr(int num);

/* > Return current time as a string in Hours:Minutes:Seconds format */
std::string timestamp();

/* > Return a text centered */
std::string centerText(std::string &text, size_t width);
