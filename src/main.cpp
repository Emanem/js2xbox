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

#include <signal.h>
#include <getopt.h>
#include "log.h"
#include "events.h"
#include "uinput.h"
#include "joypads.h"
#include <memory>
#include <string.h>

namespace {
	volatile bool	run = true;

	void sigint(int signo) {
		LOG_INFO("Exiting...\n");
		run = false;
		signal(signo, SIG_DFL);
	}

	void list_pads(void) {
		std::cout << "\tInput Joypads (in order of priority)\n";
		for(const events::js_desc **p = joypads::in; *p; ++p)
			std::cout << (*p)->i_name << "\n";
		std::cout << "\n\tOutput target Joypads\n";
		for(const events::js_desc **p = joypads::out; *p; ++p)
			std::cout << (*p)->i_name << "\n";
		std::cout << std::flush;
	}
}

int main(int argc, char *argv[]) {
	try {
		// parse options
		events::js_desc	const	*j_target = 0;
		const char*		print_info_only_path = 0;
		{
			const static struct option	long_options[] = {
				{"help",	no_argument, 		0,  'h' },
				{"list-pads",	no_argument,		0,  'l' },
				{"print-info",	required_argument,	0,  0 },
				{"silent",	no_argument,		0,  0 },
				{"target-pad",	required_argument,	0,  't' },
				{0,         	0,                 	0,  0 }
			};
			int	opt = -1,
				opt_idx = 0;
			while ((opt = getopt_long(argc, argv, "hlt:", long_options, &opt_idx)) != -1) {
				switch (opt) {
				case 0: {
					if(!strcmp("silent", long_options[opt_idx].name)) options::silent = true;
					else if (!strcmp("print-info", long_options[opt_idx].name))  {
						print_info_only_path = optarg;
					}
				} break;
				case 'h': options::usage(argv[0]); break;
				case 'l': list_pads(); std::exit(0); break;
				case 't': {
					for(const events::js_desc **p = joypads::out; *p; ++p) {
						if(!strcasecmp((*p)->i_name, optarg)) {
							j_target = *p;
							break;
						}
					}
					if(!j_target)
						throw std::runtime_error(std::string("Target Joypad '") + optarg + "' is not valid (run with -l/--list-pads to list valid names)");
				} break;
				case '?': std::exit(-1);
				default:  throw std::runtime_error("Unknown option");
				}
			}
		}
		LOG_INFO("Starting js2xbox ...");
		// if we're in print info mode...
		if(print_info_only_path) {
			LOG_INFO("Reporting info for '" << print_info_only_path << "'...");
			events::print_info_js(print_info_only_path);
			LOG_INFO("Info reported, exiting");
			return 0;
		}
		// in case we don't have a target, default to Xbox 360
		if(!j_target) {
			j_target = &joypads::j_xbox_360;
			LOG_INFO("Target virtual Joypad not specified, defaulting to'" << j_target->i_name << "' ...");
		} else {
			LOG_INFO("Using target virtual Joypad as '" << j_target->i_name << "' ...");
		} 
		// register the Ctrl-C callback
		if(signal(SIGINT, sigint) == SIG_ERR)
			throw std::runtime_error("Can't override SIGINT");
		if(signal(SIGTERM, sigint) == SIG_ERR)
			throw std::runtime_error("Can't override SIGTERM");

		// default devices, for now PS3 only
		const events::js_desc	**js_list = joypads::in;
		events::js_desc	const	*j_found = 0;
		// get the device name
		const std::string		dev_name = events::find_event_js(js_list, &j_found);
		// open the handle
		uinput::evt_reader		reader(dev_name.c_str());
		std::unique_ptr<uinput::pad>	pad(uinput::get_pad(j_found, j_target));
		char				dev_path[128];
		input_event			ie = {0};
		LOG_INFO("Virtual device created at '" << pad->get_device_name(dev_path, 128) << "'");
		while(run) {
			if(reader.read(ie) && pad->translate_event(ie))
				pad->send_event(ie);
		}
	} catch(const std::exception& e) {
		LOG_ERR("Exception: " << e.what());
		return -1;
	} catch(...) {
		LOG_ERR("Unknown exception");
		return -1;
	}
	LOG_INFO("Virtual device destroyed");
}

