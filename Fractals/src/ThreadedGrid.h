#ifndef _THREADED_GRID_
#define _THREADED_GRID_

#include "NumberGrid.h"
#include <vector>
#include <mutex>

class Task {
	public:
		Task();
		Task(int row, int column);

		int getRow();
		int getColumn();

	protected:
		int mRow;
		int mColumn;
};

class ThreadedGrid: public NumberGrid {
	public:
		//constructors
		ThreadedGrid();
		ThreadedGrid(const int& height, const int& width);	
		virtual ~ThreadedGrid();

		//methods
		virtual void calculateAllNumbers();
		virtual void worker();

	protected:	
		//data members
		std::vector<Task> mTasks;
		std::mutex mMutex;
};

#endif
