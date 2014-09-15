/*
	This file is part of Beruben.

	Beruben is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as
	published by the Free Software Foundation, either version 3 of
	the License, or	(at your option) any later version.

	Beruben is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lessor General Public
	License	along with Beruben.
	If not, see <http://www.gnu.org/licenses/lgpl.txt>.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctype.h>
#define CHARS_IN_TAB 4
#ifdef _MSC_VER
#include <windows.h>
#define sleep_ms(X) Sleep(X)
#else
#include <unistd.h>
#define sleep_ms(X) usleep(X*1000)
#endif

namespace beruben {

class Canvas
{
public:
	void set(int x, int y, char value);	
	const char get(int x, int y);
	bool mutex(int x, int y, int id);
	bool mutex_release(int x, int y, int id);
private:
	struct Lock
	{
		int x;
		int y;
		int owner;
	};
	std::vector< std::vector<char> > map;
	std::vector<Lock> locks;
};

enum EDIR
{
	EDIR_UP = 0,
	EDIR_RIGHT,
	EDIR_DOWN,
	EDIR_LEFT,
};

class Thread
{
public:
	Thread(int thread_id, bool strict_mode);
	
	// Executing
	int step(Canvas *canvas);
	bool is_alive;

	// Location	
	void move();	
	void setPosition(int px, int py, EDIR dir);

	// Stack
	const char pop();
	inline int pop_int(){return (int)pop();}
	inline void push(char inp){stack.push_back(inp);}
	std::vector<char> getStack(){ return stack; }
	void setStack(std::vector<char> s){ stack = s; }
	void printStack();
	
	int c_x;
	int c_y;
	EDIR direction;
private:
	std::vector<char> stack;
	int id;
	bool string_mode;
	bool strict;
};

class Interpreter
{
public:
	Interpreter(bool enable_sleep = false, bool strict_mode = true);
	bool run();
	bool loadfile(const char *filename);	
	bool runfile(const char *filename);
private:
	Canvas canvas;
	std::vector<Thread*> threads;
	int th_count;
	bool sleeping;
	bool strict;
};

}; // END NAMESPACE BERUBEN

