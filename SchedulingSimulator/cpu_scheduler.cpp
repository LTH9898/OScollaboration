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
		currentProcess.SetPid("");
		currentProcess.SetBurstTime(0);
		time = 0;
		isRunning = true;
	}



	// Dispatch process from waitingQueue to CPU
	if (!wQ.Empty()) {

		// Preemptive
		if (isPreemptive) {

			/*currentProcess = wQ.Top();
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

			time += delta;*/

			// Set currentProcess
			if (currentProcess.GetBurstTime() <= 0) {

				currentProcess = wQ.Top();
				wQ.Pop();
			}
			else if (wQ.HasMorePriority(wQ.Top(), currentProcess)) {

				Process prevProcess = currentProcess;
				currentProcess = wQ.Top();
				wQ.Pop();
				wQ.Push(prevProcess);
			}

			// Increase time
			if (!pQ->Empty() && currentProcess.GetBurstTime() > pQ->Top().GetArrivalTime() - time) {

				double oldTime = time;
				time = pQ->Top().GetArrivalTime();
				currentProcess -= time - oldTime;
			}
			else {

				time += currentProcess.GetBurstTime();
				currentProcess.SetBurstTime(0);
			}
		}

        // Round-Robin
        else if (isRoundRobin) {
			
			currentProcess = wQ.Top();
			wQ.Pop();

            if (currentProcess.GetBurstTime() > timeQuantum) {

                currentProcess -= timeQuantum;
                time += timeQuantum;
				wQ.Push(currentProcess);
            }
			else
                time += currentProcess.GetBurstTime();
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
		currentProcess.SetBurstTime(0);
		time = pQ->Top().GetArrivalTime();
	}




	// Process queue to waiting queue
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
