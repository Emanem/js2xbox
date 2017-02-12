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

#include "events.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <linux/input.h>
#include "log.h"

namespace {
	struct fd_holder {
		int	fd;

		fd_holder(const char* fname) : fd(open(fname, O_RDONLY)) {
			if(fd < 0)
				throw std::runtime_error(std::string("Can't open '") + fname + "'");
		}

		~fd_holder() {
			close(fd);
		}
	};
}

int events::is_event_device(const struct dirent *dir) {
	return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

std::string events::find_event_js(const js_desc** js, js_desc const **out) {
	if(!js)
		throw std::runtime_error("You need to pass at least one js_desc descriptor");

	struct dirent	**namelist = 0;
	std::string 	ret;

	const int 	ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, alphasort);
	if (ndev <= 0)
		throw std::runtime_error("Can't open DEV_INPUT_EVENT");

	for(int i = 0; i < ndev; i++) {
		char 		name[256] = "???";
		struct input_id id = {0};

		const std::string	fname = std::string(DEV_INPUT_EVENT) + "/" + namelist[i]->d_name;
		const int		fd = open(fname.c_str(), O_RDONLY);

		free(namelist[i]);
		if(fd < 0) continue;
		// get a bunch of info
		const int	nm_len = ioctl(fd, EVIOCGNAME(sizeof(name)), name),
				id_ctl = ioctl(fd, EVIOCGID, &id);
		close(fd);
		// if we can't get returned values from ioctl, skip it
		if(nm_len < 0 || id_ctl)
			continue;

		// linear search for the given vendor/product
		for(const js_desc** p = js; *p; ++p) {
			if(id.bustype == (*p)->bus && id.vendor == (*p)->vendor && id.product == (*p)->product) {
				ret = fname;
				if(out) *out = *p;
				char	buf[64];
				snprintf(buf, 64, "id: 0x%04x,0x%04x,0x%04x,0x%04x", id.bustype, id.vendor, id.product, id.version);
				LOG_INFO("Found a device: [" << name << "] (" << buf << ") on '" << fname << "'");
				break;
			}
		}
		if(!ret.empty()) {
			for(int j = i+1; j < ndev; ++j)
				free(namelist[j]);
			break;
		}
	}
	free(namelist);

	if(ret.empty())
		throw std::runtime_error("Couldn't find any joypad in the specified list");

	return ret;
}

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

void events::print_info_js(const std::string& fname) {
	fd_holder	f(fname.c_str());

	int 		version = 0;
	unsigned long	bit[EV_MAX][NBITS(KEY_MAX)] = {0};
	char 		name[256] = "???";
 
	if(0 > ioctl(f.fd, EVIOCGNAME(sizeof(name)), name)) {
		sprintf(name, "Unknown name");
	}

	if(ioctl(f.fd, EVIOCGVERSION, &version))
		throw std::runtime_error("Can't get EVIOCGVERSION");
 	
	char	buf[128];
	snprintf(buf, 128, "%d.%d.%d", version >> 16, (version >> 8) & 0xff, version & 0xff);
	std::cout << "Device [" << name << "] on '" << fname << "' version: " << buf << std::endl; 

	struct input_id id = {0};
	if(ioctl(f.fd, EVIOCGID, &id))
		throw std::runtime_error("Can't get EVIOCGID");
	snprintf(buf, 128, "Bus 0x%04x, Vendor 0x%04x, Product 0x%04x, Version 0x%04x", id.bustype, id.vendor, id.product, id.version);
	std::cout << "Device id info: " << buf << std::endl;

	memset(bit, 0, sizeof(bit));
	ioctl(f.fd, EVIOCGBIT(0, EV_MAX), bit[0]);
	std::cout << "Supported events:" << std::endl;
	for (int i = 0; i < EV_MAX; i++) {
		if (test_bit(i, bit[0])) {
			std::cout << "\tEvent type " << i << std::endl;
			if (!i) continue;
			ioctl(f.fd, EVIOCGBIT(i, KEY_MAX), bit[i]);
			for (int j = 0; j < KEY_MAX; j++) {
				if (test_bit(j, bit[i])) {
					char buf[16];
					snprintf(buf, 16, "0x%04x", j);
					std::cout << buf;
					// if it's absolue, then print out these details too
					if(i == EV_ABS) {
						struct input_absinfo	abs = {0};
						ioctl(f.fd, EVIOCGABS(j), &abs);
						std::cout << '(' << abs.value << ',' << abs.minimum << ',' << abs.maximum 
					        	  << ',' << abs.fuzz << ',' << abs.flat << ',' << abs.resolution << ')';
					}
					std::cout << ' ';
				}
			}
			std::cout << std::endl;
		}
	}
}

