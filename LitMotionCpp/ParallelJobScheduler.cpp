#include "pch.h"
#include "IJobParallelFor.h"
#include "ParallelJobScheduler.h"

void defaultExecuter(IJobParallelFor& job, size_t size, size_t /*innerLoopBatchCount*/)
{
	for (int index = 0; index < size; index++)
	{
		job.execute(index);
	}
}

std::function<decltype(ParallelJobScheduler::execute)> g_executer = defaultExecuter;


void ParallelJobScheduler::execute(IJobParallelFor& job, size_t size, size_t innerLoopBatchCount)
{
	g_executer(job, size, innerLoopBatchCount);
}

void ParallelJobScheduler::setExecuter(std::function<decltype(execute)> executer)
{
	if (executer)
	{
		g_executer = executer;
	}
	else
	{
		g_executer = defaultExecuter;
	}

}
