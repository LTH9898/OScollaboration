#ifndef __PROCESS_QUEUE_H__
#define __PROCESS_QUEUE_H__
#include "process.h"
#include <list>


class ProcessQueue
{
public:
	Process& Top()
		{ return data.front(); }
	bool Empty() const
		{ return data.empty(); }

	void Push(const Process& process)
	{
		auto iter = data.cbegin();
		for (; iter != data.cend(); iter++)
			if (process.GetArrivalTime() < iter->GetArrivalTime())
				break;
		data.insert(iter, process);
	}
	void Emplace(std::string pid, double arrival, double burst, unsigned priority)
	{
		auto iter = data.cbegin();
		for (; iter != data.cend(); iter++)
			if (arrival < iter->GetArrivalTime())
				break;
		data.emplace(iter, pid, arrival, burst, priority);
	}
	void Pop()
		{ data.pop_front(); }

private:
	std::list<Process> data;
};


#endif
