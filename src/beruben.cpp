#include "beruben.hpp"
#define ST_NEW_THREAD 1
#define ST_CRASH 2
using namespace beruben;

#ifdef DEBUG
#define debug(X) std::cerr << X
#else
#define debug(X)
#endif

void Canvas::set(int x, int y, char value)
{
	while (map.size() < y + 1) {
		map.push_back(std::vector<char>());
	}
	while (map[y].size() < x + 1) {
		map[y].push_back(' ');
	}
	map[y][x] = value;
}

const char Canvas::get(int x, int y)
{
	if (y >= map.size() || x >= map[y].size())
		return ' ';

	return map[y][x];
}

bool Canvas::mutex(int x, int y, int id)
{
	for (std::vector<Lock>::const_iterator it = locks.begin();
			it != locks.end(); ++it) {
		if (x == (*it).x && y == (*it).y) {
			return (id == (*it).owner);
		}
	}
	Lock lock;
	lock.x = x;
	lock.y = y;
	lock.owner = id;
	locks.push_back(lock);
	return true;
}

bool Canvas::mutex_release(int x, int y, int id)
{
	for (std::vector<Lock>::iterator it = locks.begin();
			it != locks.end(); ++it) {
		if (x == (*it).x && y == (*it).y) {
			if (id == (*it).owner) {
				locks.erase(it);
				return true;
			} else
				return false;
		}
	}
	return false;
}

Thread::Thread(int thread_id):
	c_x(0),
	c_y(0),
	direction(EDIR_RIGHT),
	is_alive(true),
	id(thread_id),
	string_mode(false)
{}

int Thread::step(Canvas *canvas)
{
	if (!is_alive)
		return 0;
	
	const char symbol = canvas->get(c_x, c_y);
	
	if (string_mode) {
		if (symbol == '"') {			
			string_mode = false;
			move();
			return 0;
		} else {
			push(symbol);
			move();
			return 0;
		}
	}

	debug("T" << id << " (" << c_x << ", " << c_y << ")\t");

	switch (symbol) {
	/*
		Program Flow
	*/
	case ' ':
		break;
	case '^':
		debug("Up");
		direction = EDIR_UP;
		break;
	case '>':
		debug("Right");
		direction = EDIR_RIGHT;
		break;
	case 'v':
		debug("Down");
		direction = EDIR_DOWN;
		break;
	case '<':
		debug("Left");
		direction = EDIR_LEFT;
		break;
	case '#':
		debug("Jump");
		move();
		break;
	case '_': {
		debug("If");
		int a = pop_int();
		if (a == 0)
			direction = EDIR_RIGHT;
		else
			direction = EDIR_LEFT;
		break;
	}
	case '@':
		debug("Exit");
		is_alive = false;
		break;

	/*
		Threading
	*/
	case 't':
		debug("Thread\t");
		printStack();
		direction = EDIR_LEFT;
		return ST_NEW_THREAD;
	case 'm':{
		int a = pop_int();
		int b = pop_int();
		if (!canvas->mutex(b, a, id)) {
			debug("MUTLOCK\t");
			printStack();
			push(b);
			push(a);
			return 0;
		}
		break;
	}
	case 'n':{
		int a = pop_int();
		int b = pop_int();
		if (!canvas->mutex_release(b, a, id)) {			
			debug("Mutex release failure\t");
			printStack();
			is_alive = false;
			return ST_CRASH;
		}
		break;
	}
	/*
		Logical Operators
	*/
	case '=': {
		debug("Equals");

		int a = pop_int();
		int b = pop_int();
		push((b == a)?0:1);
		break;
	}
	case '!': {
		debug("Not");

		push((pop_int() == 0)?1:0);
		break;
	}
	
	/*
		Numerical Operators
	*/
	case '+': {
		debug("Add");
		int a = pop_int();
		int b = pop_int();
		push(b + a);
		break;
	}
	case '-': {
		debug("Subtract");

		int a = pop_int();
		int b = pop_int();
		push(b - a);
		break;}
	case '*': {
		debug("Multiply");

		int a = pop_int();
		int b = pop_int();
		push(b * a);
		break;
	}
	case '/': {
		debug("Divide");

		int a = pop_int();
		int b = pop_int();
		push(b - a);
		break;
	}
	case '%': {
		debug("Modulo");

		int a = pop_int();
		int b = pop_int();
		push(b % a);
		break;
	}

	/*
		Data Operators
	*/
	case ':': {
		debug("Dup");

		const char a = pop();
		push(a);
		push(a);
		break;
	}

	/*
		Input and Output
	*/
	case ',': {
		std::cout << pop();
		break;
	}
	case '.': {
		std::cout << pop_int();
		break;
	}
	case '"':
		debug("Str");
		string_mode = true;
		break;

	/*
		Other
	*/
	default: {
		if (isdigit(symbol)) {
			int num = (int)symbol - 48;
			debug("Num " << num);
			stack.push_back(num);
		} else {			
			debug("?" << symbol << "["
				  << (int)symbol << "]");
		}
	}}
	
	debug("\t");
	printStack();

	move();	
	return 0;	
}

void Thread::move()
{
	if (!is_alive)
		return;

	switch(direction)
	{
	case EDIR_UP:
		c_y--;
		break;
	case EDIR_RIGHT:
		c_x++;
		break;
	case EDIR_DOWN:
		c_y++;
		break;
	case EDIR_LEFT:
		c_x--;
		break;
	};
}

void Thread::setPosition(int px, int py, EDIR dir)
{
	if (!is_alive)
		return;

	c_x = px;
	c_y = py;
	direction = dir;
}

const char Thread::pop()
{
	if (stack.size() == 0)
		return 0;
	const char val = stack.back();
	stack.pop_back();
	return val;
}

void Thread::printStack()
{
	debug("[");
	int count = 0;
	for (std::vector<char>::const_iterator it = stack.begin();
			it != stack.end(); ++it) {
		if (count != 0)
			debug(", ");
		debug((int)(*it));
		count++;
	}
	debug("]" << std::endl);
}

bool IsDead(const Thread *thread)
{
	return !thread->is_alive;
}

Interpreter::Interpreter(bool enable_sleep):
	th_count(0),
	sleeping(enable_sleep)
{}

bool Interpreter::run()
{
	threads.clear();
	threads.push_back(new Thread(++th_count));
	
	while (threads.size() > 0) {
		std::vector<Thread*> toadd;
		for (std::vector<Thread*>::const_iterator it = threads.begin();
				it != threads.end(); ++it) {
			int res = (*it)->step(&canvas);
			if (res == ST_NEW_THREAD) {
				Thread *thread = new Thread(++th_count);
				thread->setStack((*it)->getStack());
				thread->c_x = (*it)->c_x + 1;
				thread->c_y = (*it)->c_y;
				thread->direction = EDIR_RIGHT;
				(*it)->c_x -= 1;
				toadd.push_back(thread);
			} else if (res == ST_CRASH) {
				std::cerr << std::endl << "Program Crashed!" << std::endl;
				return false;
			}
		}
		for (std::vector<Thread*>::const_iterator it = toadd.begin();
				it != toadd.end(); ++it) {
			threads.push_back((*it));
		}
		threads.erase(std::remove_if(threads.begin(), threads.end(),
			IsDead), threads.end());
		std::cout.flush();
		if (sleeping)
			sleep_ms(20);
	}
	std::cout << std::endl;
	return true;
}

bool Interpreter::loadfile(const char *filename)
{
	canvas = Canvas();

	// Read file into canvas
	std::string line;
	std::ifstream file(filename);
	if (!file)		
		return false;
	int y = 0;		
	while (std::getline(file, line)) {
		int x = 0;
		for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
		    char c = *it;
			if (c == '\t')
				x += CHARS_IN_TAB;
			else {
				canvas.set(x, y, c);
				x++;
			}
		}
		y++;
	}
	file.close();
	return true;
}

bool Interpreter::runfile(const char *filename)
{
	debug("Running " << filename << std::endl);
	return (loadfile(filename) && run());
}
