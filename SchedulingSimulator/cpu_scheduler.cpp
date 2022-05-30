#include "cpu_scheduler.h"


void CpuScheduler::StepForward()
{
	if (!isRunning) {

		if (!pQ)
			return;
		if (pQ->Empty() && wQ.Empty()) {

			pQ = nullptr;
			return;
		}
		wQ.Clear();
		ganttChart.clear();
		time = 0;
		isRunning = true;
	}


	// Dispatch process from waitingQueue to CPU
	if (!wQ.Empty()) {

		// Preemptive
		if (isPreemptive) {
			currentProcess = wQ.Top();
			wQ.Pop();
			double delta;
			double curBurstT = currentProcess.GetBurstTime();

			if (wQ.GetAlgorithm() == Scheduling::SJF) {
				while (!pQ->Empty()
					&& pQ->Top().GetBurstTime() >= currentProcess.GetBurstTime()
					&& pQ->Top().GetArrivalTime() - time <= curBurstT) {
					wQ.Push(pQ->Top());
					pQ->Pop();
				}
			}
			else { //Priority
				while (!pQ->Empty()
					&& pQ->Top().GetPriority() >= currentProcess.GetPriority()
					&& pQ->Top().GetArrivalTime() - time <= curBurstT) {
					wQ.Push(pQ->Top());
					pQ->Pop();
				}
			}

			if (pQ->Empty() || pQ->Top().GetArrivalTime() - time > currentProcess.GetBurstTime()) {
				delta = currentProcess.GetBurstTime();
			}
			else {
				delta = pQ->Top().GetArrivalTime() - time;
				wQ.Push(currentProcess - delta);
			}

			time += delta;
		}

        // Round-Robin
        else if (isRoundRobin) {
            currentProcess = wQ.Top();
            if (currentProcess.GetBurstTime() > timeQuantum)
            {
                currentProcess -= timeQuantum;
                time += timeQuantum;
                wQ.Pop();
                while (!pQ->Empty() && time >= pQ->Top().GetArrivalTime()) {
                    wQ.Push(pQ->Top());
                    pQ->Pop();
                }

                wQ.Push(currentProcess);
            }
            else
            {
                time += currentProcess.GetBurstTime();
                currentProcess.SetBurstTime(0);
                wQ.Pop();
            }
        }

		// Non-preemptive and no time-quantum
		else {

			currentProcess = wQ.Top();
			time += currentProcess.GetBurstTime();
			wQ.Pop();
		}
	}
	// If waitingQueue is empty, empty process
	else {

		currentProcess.SetPid("");
		time = pQ->Top().GetArrivalTime();
	}

	while (!pQ->Empty() && time >= pQ->Top().GetArrivalTime()) {
		wQ.Push(pQ->Top());
		pQ->Pop();
	}

	
	// push to Gantt chart
	if (ganttChart.empty())
		ganttChart.emplace_back(currentProcess.GetPid(), time);
	else {

		if (ganttChart.rbegin()->first == currentProcess.GetPid())
			ganttChart.rbegin()->second = time;
		else
			ganttChart.emplace_back(currentProcess.GetPid(), time);
	}
	
	ganttChart.emplace_back(currentProcess.GetPid(), time);
	if (pQ->Empty() && wQ.Empty()) {

		isRunning = false;
		pQ = nullptr;
	}
}

void CpuScheduler::Reset()
{
	isRunning = false;
	pQ = nullptr;
	wQ.Clear();
	ganttChart.clear();
}
