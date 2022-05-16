#pragma once
#include "process.h"
#include <vector>

#define NO_TIME_QUANTUM  -1;
#define NON_PREEMPTIVE false
#define PREEMPTIVE true

enum
{
	INDEX_FCFS = 1,
	INDEX_SJF,
	INDEX_Priority
};


class waitingQueue
{
public:
	waitingQueue() : fp(nullptr) {}
	void SetAlgorithm(int select);

	const std::vector<Process>& GetWaitingQueue() const { return data; }
	bool Empty() { return data.empty(); }

	const Process& Top() const { return data.front(); }
	void Push(Process newProcess);
	void Pop();

private:
	std::vector<Process> data;
	void (waitingQueue::*fp)(Process newProcess);
	
	void FCFS(Process newProcess);
	void SJF(Process newProcess);
	void Priority(Process newProcess);
};


void waitingQueue::SetAlgorithm(int select)
{
	switch (select) {

	case INDEX_FCFS:
		fp = &waitingQueue::FCFS;
		break;

	case INDEX_SJF:
		fp = &waitingQueue::SJF;
		break;

	case INDEX_Priority:
		fp = &waitingQueue::Priority;
		break;

	default:
		fp = nullptr;
	}
}


void waitingQueue::Push(Process newProcess)
{
	(this->*fp)(newProcess);
}

void waitingQueue::Pop()
{
	data.erase(data.begin());
}



void waitingQueue::FCFS(Process newProcess)
{
	data.push_back(newProcess);
}

void waitingQueue::SJF(Process newProcess)
{

}

void waitingQueue::Priority(Process newProcess)
{

}











GanttChart test(waitingQueue pQ, const bool preemptive, const double timeQuantum)
{
	double time = 0.0;
	GanttChart ganttChart;

	while (!pQ.Empty()) {

		if (time >= pQ.Top().GetArrivalTime()) {

			const Process& currentProcess = pQ.Top();
			time += currentProcess.GetBurstTime();
			ganttChart.emplace(currentProcess.GetPid(), time);
			pQ.Pop();
		}

		else {

			time = pQ.Top().GetArrivalTime();
			ganttChart.emplace(std::string(), time);
		}
	}

	return ganttChart;
}