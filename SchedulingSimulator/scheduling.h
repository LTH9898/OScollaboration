#pragma once
#include "process.h"
#include <vector>

constexpr int NO_TIME_QUANTUM = -1;


GanttChart FCFS(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM);
/*GanttChart SJF(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM);
GanttChart SRTF(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM);
GanttChart RR(ProcessQueue pQ, const double timeQuantum);
GanttChart Priority(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM);
GanttChart PreemptivePriority(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM);
GanttChart PriorityRR(ProcessQueue pQ, const double timeQuantum);*/



/////////////////////////////////////////////////////////////////////
GanttChart FCFS(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM)
{
	double time = 0.0;
	GanttChart ganttChart;
	
	while (!pQ.empty()) {

		if (time >= pQ.top().GetArrivalTime()) {

			const Process& currentProcess = pQ.top();
			time += currentProcess.GetBurstTime();
			ganttChart.emplace(currentProcess.GetPid(), time);
			pQ.pop();
		}
		else {

			time = pQ.top().GetArrivalTime();
			ganttChart.emplace(std::string(), time);
		}
	}

	return ganttChart;
}
/*

GanttChart SJF(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM)
{
}


GanttChart SRTF(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM)
{
}


GanttChart RR(ProcessQueue pQ, const double timeQuantum)
{
}


GanttChart Priority(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM)
{
}


GanttChart PreemptivePriority(ProcessQueue pQ, const double timeQuantum = NO_TIME_QUANTUM)
{
}


GanttChart PriorityRR(ProcessQueue pQ, const double timeQuantum)
{
}*/