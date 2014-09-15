/*
	This file is part of Beruben.

	Beruben is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as
	published by the Free Software Foundation, either version 3 of
	the License, or	(at your option) any later version.

	Beruben is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lessor General Public License for more details.

	You should have received a copy of the GNU Lessor General Public
	License	along with Beruben.
	If not, see <http://www.gnu.org/licenses/lgpl.txt>.
*/

#include "beruben.hpp"
#include <iostream>

void usage()
{
	std::cerr << "Beruben Interpreter by rubenwardy" << std::endl;
	std::cerr << "Usage  : path/to/file [flags]" << std::endl;
	std::cerr << "Example: examples/hello_world.br -s" << std::endl;
	std::cerr << "Flags:" << std::endl;
	std::cerr << "  -s     Use sleep to slow down command execution" << std::endl;
	std::cerr << " --lax   Disable strict mode" << std::endl;
}

int main(int arg, char* args[])
{
	if (arg <= 1) {
		usage();
		return EXIT_FAILURE;
	}

	bool sleep = false;
	bool strict = true;
	for (int i = 1; i < arg; i++) {
		std::string a(args[i]);
		if (a == "-s") {
			sleep = true;
		} else if (a == "--lax") {
			strict = false;
		} else if (i > 1 && a != "") {
			usage();
			return EXIT_FAILURE;
		}
	}
	beruben::Interpreter *intp = new beruben::Interpreter(sleep, strict);
	if (!intp->runfile(args[1])) {
		std::cerr << "Failed to run file " << args[1] << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

