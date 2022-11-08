#include <string>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <thread> 
#include <chrono>   
#include <mutex>
using namespace std;

const int SIZE = 10;

class Board
{
private:
	std::vector<std::vector<char>> grid;
	std::mutex m_mutex;
public:
	Board()
	{
		for (int i = 0; i < SIZE; ++i)
		{
			grid.emplace_back(std::vector<char>(SIZE,' '));
		}
	}

	void Lock()
	{
		m_mutex.lock();
	}

	void Unlock()
	{
		m_mutex.unlock();
	}

	bool IsPosEmpty(int row, int col)
	{
		return grid[row][col] == ' ';
	}

	void PutRobotInPos(char robot, int row, int col)
	{
		grid[row][col] = robot;
	}

	void DisplayBoard()
	{
		system("cls");
		for (auto row : grid)
		{
			for (auto j : row)
			{
				std::cout << j << "\t";
			}
			std::cout << endl;
		}
	}
};

class RobotBase
{
protected:
	int row, col;
	char chName;
	
public:
	Board* pBoard;
	RobotBase(Board* p,int r,int c,char name) :pBoard(p) 
	{
		srand(time(0));
		row = r;
		col = c;
		chName = name;

		pBoard->PutRobotInPos(chName,  r,c);
	}
	virtual void move() = 0;

	int GetNextRandom()
	{
		return rand() % 10 + 1;
	}

	void UpdateRobotPos(int newr, int newc)
	{
		row = newr;
		col = newc;
		pBoard->PutRobotInPos(chName, newr, newc);
	}

	void ClearRobotPos(int r, int c)
	{
		pBoard->PutRobotInPos(' ', r, c);
	}
};

class HorizontalRobot : public RobotBase
{
public:
	HorizontalRobot(Board* p,int r,int c, char name) : RobotBase(p,r,c,name)
	{
	}
	virtual void move()
	{
		/*cout << "horozontal robot move";*/
		int randNum = GetNextRandom();
		int newrow = row, newcol = col;
		if (randNum < 3)
		{
			newcol = newcol - 1;
			if (newcol < 0)
				newcol = SIZE - 1;
		}
		else if (randNum < 7)
		{
			//stay still
		}
		else
		{
			newcol = newcol + 1;
			if (newcol >= SIZE)
				newcol = 0;
		}
		if (newrow == row && newcol == col)
			return;

		pBoard->Lock();
		if (pBoard->IsPosEmpty(newrow, newcol))
		{
			ClearRobotPos(row, col);
			UpdateRobotPos(newrow, newcol);
		}
		pBoard->Unlock();
	}
	
};

class VerticalRobot : public RobotBase
{
public:
	VerticalRobot(Board* p, int r, int c, char name) : RobotBase(p, r, c, name)
	{
	}
	virtual void move()
	{
		/*cout << "horozontal robot move";*/
		int randNum = GetNextRandom();
		int newrow = row, newcol = col;
		if (randNum < 3)
		{
			newrow = newrow - 1;
			if (newrow < 0)
				newrow = SIZE - 1;
		}
		else if (randNum < 7)
		{
			//stay still
		}
		else
		{
			newrow = newrow + 1;
			if (newrow >= SIZE)
				newrow = 0;
		}
		pBoard->Lock();
		if (pBoard->IsPosEmpty(newrow, newcol))
		{
			ClearRobotPos(row, col);
			UpdateRobotPos(newrow, newcol);
		}
		pBoard->Unlock();
	}
};

class SquareRobot : public RobotBase
{
public:
	SquareRobot(Board* p, int r, int c, char name) : RobotBase(p, r, c, name)
	{
	}
	
	virtual void move()
	{
		int randNum = GetNextRandom();
		int newrow = row, newcol = col;
		if (randNum < 2)
		{
			newcol = newcol - 1;
			if (newcol < 0)
				newcol = SIZE - 1;
		}
		else if (randNum < 4)
		{
			newrow = newrow - 1;
			if (newrow < 0)
				newrow = SIZE - 1;
		}
		else if (randNum < 6)
		{
			newcol = newcol + 1;
			if (newcol >= SIZE)
				newcol = 0;
		}
		else if (randNum < 8)
		{
			newrow = newrow + 1;
			if (newrow >= SIZE)
				newrow = 0;
		}
		else
		{
			//stay still
		}
		pBoard->Lock();
		if (pBoard->IsPosEmpty(newrow, newcol))
		{
			ClearRobotPos(row, col);
			UpdateRobotPos(newrow, newcol);
			pBoard->DisplayBoard();
		}
		pBoard->Unlock();
	}
};

void RobotThread(RobotBase* robot,Board* pBoard)
{
	for (int i = 0; i < 100; ++i)
	{
		robot->move();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main()
{	
	Board b;

	HorizontalRobot robot1(&b, 0, 0, 'H');
	VerticalRobot robot2(&b, 5, 4, 'V');
	SquareRobot robot3(&b, 6, 8, 'S');

	std::thread t1{ RobotThread,&robot1,&b };
	std::thread t2{ RobotThread,&robot2,&b };
	std::thread t3{ RobotThread,&robot3,&b };

	t1.join();
	t2.join();
	t3.join();
}
