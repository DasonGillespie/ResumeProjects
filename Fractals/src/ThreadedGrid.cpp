#include "ThreadedGrid.h"
#include <thread>
#include "NumberGrid.h"

//-----ThreadedGrid-----
ThreadedGrid::ThreadedGrid()
	: NumberGrid() {

}

ThreadedGrid::ThreadedGrid(const int& height, const int& width)
	: NumberGrid(height, width) {

}

ThreadedGrid::~ThreadedGrid() {

}

void ThreadedGrid::calculateAllNumbers() {
	//create tasks
        for(int row = 0; row < getHeight(); row++) {
                for(int column = 0; column < getWidth(); column++) {
			Task task(row, column);
			mTasks.push_back(task);
                }
        }

	//spawn threads
	std::vector<std::thread> threads;
	unsigned int max_threads = std::thread::hardware_concurrency();
	for(unsigned int i = 0; i < max_threads; i++) {
		threads.push_back(std::thread(&ThreadedGrid::worker, this));
	}

	//join threads
	for(long unsigned int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}

void ThreadedGrid::worker() {
	while(true) {

		mMutex.lock();
		if(mTasks.size() <= 0) {
			mMutex.unlock();
			break;
		}
		//get task
		Task task = mTasks.back(); 
		//remove task
		mTasks.pop_back();
		mMutex.unlock();

		int row = task.getRow();
		int column = task.getColumn();
		int number = calculateNumber(row, column);

		mMutex.lock();
		setNumber(row, column, number);
		mMutex.unlock();
	}
}

//-----Task-----
Task::Task() {

}

Task::Task(int row, int column)
	: mRow(row), mColumn(column) {
	
}

int Task::getRow() {
	return mRow;
}

int Task::getColumn() {
	return mColumn;
}
