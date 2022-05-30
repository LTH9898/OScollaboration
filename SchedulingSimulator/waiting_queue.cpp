#include "waiting_queue.h"


void WaitingQueue::SetAlgorithm(Scheduling algorithm)
{
	this->algorithm = algorithm;

	if (algorithm == Scheduling::FCFS)
		fp = &WaitingQueue::FCFS;
	else if (algorithm == Scheduling::SJF)
		fp = &WaitingQueue::SJF;
	else if (algorithm == Scheduling::Priority)
		fp = &WaitingQueue::Priority;
	else
		fp = nullptr;
}

void WaitingQueue::Push(const Process& newProcess)
{
	for (auto iter = data.cbegin(); iter != data.cend(); iter++) {
		if ((this->*fp)(newProcess, *iter)) {

			data.insert(iter, newProcess);
			return;
		}
	}
	data.push_back(newProcess);
}