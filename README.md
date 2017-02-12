# js2xbox

Simple user-space utility to map an input joypad to a virtual Xbox 360 controller; the main purpose is to ensure that old games (or games supporting only that joypad) can actually be played with some other input device. As of now only PS3 (USB and Bluetooth) are supported as input devices: a simpler version of *xboxdrv* in a nutshell.

## Building

Just download this repository and invoke `make` (`make release` for optimized build). This project does not depend on any library, only a recent kernel and compiler (c++11) are needed.

## Running

### All commands

```
js2xbox 0.2

Usage: ./js2xbox [options]

Create a new virtual Xbox 360 pad (or other joypad models)
from an existing joypad device (now only PS3)

Options can be:

-h,--help                 Print this screen and quit

-l,--list-pads            Lists all the Joypads supported, both
                          in input and output virtual target

--print-info e            Prints the technical capabilities/info of
                          the selected input Joypad and quit; the e
                          parameter should be the absolute path to the
                          event device (i.e. /dev/input/event22)

--silent                  Do not output any log

-t,--target-pad p         Specify which target pad (p) to use; see names
                          using -l/--list-pads option
```

### Sample usage

From PS3 Bluetooth to PS3 USB

```
./js2xbox -t ps3_usb
```

From PS3 (any) to Xbox 360

```
./js2xbox
```

## F.A.Q.

### Why did you put together this simple user driver?

I wanted to use my PS3 Bluetooth pad but unfortunately many Linux games only support PS3 USB or even only the Xbox 360 pad

### I've looked at the source code and it seems quite straightforward. Why isn't as complex as *xboxdrv*?

This software is supposed to be simple to understand and modify. True, given that there are no abstraction layers to mad buttons and sticks (ABS events) to *abstract* entities, it makes harder to support multiple pads and whatnot.
At the same time, I wanted to achieve mainly two objectives: quick development time, and easy to understand/share implementation.
Plus, not having many abstractions also optimizes the generated code and speed.

### My joypad *xyz* is not supported. Can you add it?

See answer above: the code is pretty simple and using the command *--print-info* (together with utilities such as *jstest-gtk*) will facilitate adding a specific pad. You can do it, and then send me a pull request!

### Why are you using *epoll* instead of just reading form the *input device file* straight away?

I wanted to have a timeout in place... I know it's lame, but it works, plus the overhead should be in terms of *nanosecond*s, so not an issue. Have to test this [performance claim](http://stackoverflow.com/questions/41920301/is-it-efficient-to-use-epoll-with-devices-dev-event) though.

### What if in future we want to add more pads and we need to support more output types (not only Xbox 360 and PS3)?

Then we should have an abstraction layer. Hopefully not as complex and with so many indirections as *xboxdrv* one...

## License

```
	js2xbox (C) 2017 E. Oriani, ema <AT> fastwebnet <DOT> it

	This file is part of js2xbox.

	js2xbox is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	js2xbox is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with js2xbox.  If not, see <http://www.gnu.org/licenses/>.
```

## Credits

Thanks to Linux for being open source and to:
- [xboxdrv](https://github.com/xboxdrv/xboxdrv) For not working with PS3 Bluetooth, being complex and thus inspiring me to write a simpler applet
- [Gregory Thiemonge](http://thiemonge.org/getting-started-with-uinput) For the great *uinput* getting started
- [Feral Interactive](https://www.feralinteractive.com/en/) and [Aspyr Media](http://www.aspyr.com/) For porting great games to Linux
