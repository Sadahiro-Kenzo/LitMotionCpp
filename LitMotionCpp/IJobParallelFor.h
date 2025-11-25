#pragma once
struct IJobParallelFor
{
	virtual void execute(int index) = 0;
};