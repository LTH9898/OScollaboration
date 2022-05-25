#include "waiting_queue.h"


void waitingQueue::SetAlgorithm(Scheduling algorithm)
{
	this->algorithm = algorithm;

	if (algorithm == Scheduling::FCFS)
		fp = &waitingQueue::FCFS;
	else if (algorithm == Scheduling::SJF)
		fp = &waitingQueue::SJF;
	else if (algorithm == Scheduling::Priority)
		fp = &waitingQueue::Priority;
	else
		fp = nullptr;
}



void waitingQueue::SJF(const Process& newProcess)
{

}

void waitingQueue::Priority(const Process& newProcess)
{
	for (auto iter = data.cbegin(); iter != data.cend(); iter++) {

		if (iter->GetPriority() > newProcess.GetPriority()) {

			data.insert(iter, newProcess);
			return;
		}
	}
	data.push_back(newProcess);
}


