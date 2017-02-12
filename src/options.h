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

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

namespace options {
	extern bool	silent;
	extern int	maj_ver,
			min_ver;

	extern void usage(const char* p);
}

#endif //_OPTIONS_H_

