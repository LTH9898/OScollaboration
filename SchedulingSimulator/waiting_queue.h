#pragma once
#include "process.h"
#include <vector>



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
	//void SetAlgorithm(Scheduling algorithm);

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



/// <summary>
/// //////////////////////////non inline issue/////////////////////////////////
/// </summary>

	void SetAlgorithm(Scheduling algorithm)
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


	///////////////////////////////////////////////////////////////////////




private:
	std::vector<Process> data;
	Scheduling algorithm;
	void (waitingQueue::*fp)(const Process& newProcess);
	
	void FCFS(const Process& newProcess)
		{ data.push_back(newProcess); }
	//void SJF(const Process& newProcess);
	//void Priority(const Process& newProcess);

	void SJF(const Process& newProcess)
	{

	}

	void Priority(const Process& newProcess)
	{
		for (auto iter = data.cbegin(); iter != data.cend(); iter++) {

			if (iter->GetPriority() > newProcess.GetPriority()) {

				data.insert(iter, newProcess);
				return;
			}
		}
		data.push_back(newProcess);
	}
};




/*
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


*/