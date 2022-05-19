#include "process.h"
#include <vector>
#ifndef _WAITING_QUEUE_H
#define _WAITING_QUEUE_H

//test

enum class Scheduling
{
	FCFS,
	SJF,
	Priority,
	None
};


class waitingQueue
{
public:
	waitingQueue() : fp(nullptr), algorithm(Scheduling::None) {}
	void SetAlgorithm(Scheduling algorithm);

	Scheduling GetAlgorithm() const
		{ return algorithm; }
	const std::vector<Process>& GetWaitingQueue() const
		{ return data; }

	const Process& Top() const
		{ return data.front(); }
	bool Empty()
		{ return data.empty(); }
	void Clear()
		{ data.clear(); }
	void Push(const Process& newProcess)
		{ (this->*fp)(newProcess); }
	void Pop()
		{ data.erase(data.begin()); }

private:
	std::vector<Process> data;
	Scheduling algorithm;
	void (waitingQueue::*fp)(const Process& newProcess);
	
	void FCFS(const Process& newProcess)
		{ data.push_back(newProcess); }
	void SJF(const Process& newProcess);
	void Priority(const Process& newProcess);
};

#endif
