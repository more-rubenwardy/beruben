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
	Thread(int thread_id);
	
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
};

class Interpreter
{
public:
	Interpreter(bool enable_sleep = false);
	bool run();
	bool loadfile(const char *filename);	
	bool runfile(const char *filename);
private:
	Canvas canvas;
	std::vector<Thread*> threads;
	int th_count;
	bool sleeping;
};

}; // END NAMESPACE BERUBEN

