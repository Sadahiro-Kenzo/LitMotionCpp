#pragma once
#include <functional>
#include "IJobParallelFor.h"

/**
* @brief execute IJobParallelFor
*
* Default implement work on main thread.
*/
class ParallelJobScheduler
{
public:
	/**
	* @brief execute IJobParallelFor
	*
	* @param job 
	* @param size
	* @param innerLoopBatchCount
	*/
	static void execute(IJobParallelFor& job, size_t size, size_t innerLoopBatchCount);
	static void setExecuter(std::function<decltype(execute)> executer);
};

