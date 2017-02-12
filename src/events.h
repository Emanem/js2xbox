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

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <string>
#include <dirent.h>

namespace events {
	const char	DEV_INPUT_EVENT[] = "/dev/input",
			EVENT_DEV_NAME[] = "event";

	extern int is_event_device(const struct dirent *dir);

	struct js_desc {
		const char*	i_name; // internal js2xbox name
		uint16_t	bus,
				vendor,
				product,
				version;
	};

	// find the first event interface
	// which matches the descriptions 
	// in the order specified
	// js has to be NULL terminated array
	// of pointers
	std::string find_event_js(const js_desc** js, js_desc const **out);

	// prints info descriptions
	void print_info_js(const std::string& fname);
}

#endif //_EVENTS_H_

