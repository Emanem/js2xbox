/*
*	js2xbox (C) 2017 E. Oriani, ema <AT> fastwebnet <DOT> it
*
*	This file is part of js2xbox.
*
*	js2xbox is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	js2xbox is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with js2xbox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "options.h"
#include <iostream>

bool	options::silent = false;

int	options::maj_ver = 0,
	options::min_ver = 2;

void options::usage(const char* p) {
	std::cout << "js2xbox " << maj_ver << "." << min_ver << "\n\n";
	std::cout << "Usage: " << p << " [options]\n" << std::endl;
	std::cout << "Create a new virtual Xbox 360 pad (or other joypad models)\n"
		  << "from an existing joypad device (now only PS3)\n" << std::endl;
	std::cout << "Options can be:\n\n"
		  << "-h,--help                 Print this screen and quit\n\n"
		  << "-l,--list-pads            Lists all the Joypads supported, both\n"
		  << "                          in input and output virtual target\n\n"
		  << "--print-info e            Prints the technical capabilities/info of\n"
		  << "                          the selected input Joypad and quit; the e\n"
		  << "                          parameter should be the absolute path to the\n"
		  << "                          event device (i.e. /dev/input/event22)\n\n"
		  << "--silent                  Do not output any log\n\n"
		  << "-t,--target-pad p         Specify which target pad (p) to use; see names\n"
		  << "                          using -l/--list-pads option\n\n"
		  << "";
	std::cout << std::flush;
	std::exit(0);
}

