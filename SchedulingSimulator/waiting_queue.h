#ifndef __WAITING_QUEUE_H__
#define __WAITING_QUEUE_H__
#include "process.h"
#include <vector>


enum class Scheduling
{
	FCFS,
	SJF,
	Priority,
	None
};


class WaitingQueue
{
public:
	WaitingQueue() : fp(nullptr), algorithm(Scheduling::None) {}
	void SetAlgorithm(Scheduling algorithm);

	Scheduling GetAlgorithm() const
		{ return algorithm; }
	const std::vector<Process>& GetWaitingQueue() const
		{ return data; }

	const Process& Top() const
		{ return data.front(); }
	size_t Size() const
		{ return data.size(); }
	bool Empty() const
		{ return data.empty(); }
	void Clear()
		{ data.clear(); }
	void Push(const Process& newProcess);
	void Pop()
		{ data.erase(data.begin()); }

	bool HasMorePriority(const Process& lhs, const Process& rhs) const
		{ return (this->*fp)(lhs, rhs); }


private:
	std::vector<Process> data;
	Scheduling algorithm;
	bool (WaitingQueue::*fp)(const Process&, const Process&) const;
	
	bool FCFS(const Process& lhs, const Process& rhs) const
		{ return false; }
	bool SJF(const Process& lhs, const Process& rhs) const
		{ return lhs.GetBurstTime() < rhs.GetBurstTime();	}
	bool Priority(const Process& lhs, const Process& rhs) const
		{ return lhs.GetPriority() < rhs.GetPriority(); }
};

#endif
