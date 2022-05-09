#pragma once
#include "process.h"
#include <vector>

GanttChart FCFS(ProcessQueue q);
/*GanttChart SJF(ProcessQueue q);
GanttChart SRTF(ProcessQueue q);
GanttChart RR(ProcessQueue q, const double timeQuantum);
GanttChart priority(ProcessQueue q);
GanttChart preemptivePriority(ProcessQueue q);
GanttChart priorityRR(ProcessQueue q, const double timeQuantum);*/




GanttChart FCFS(ProcessQueue pQ)
{
	double time = 0.0;
	GanttChart ganttChart;
	
	while (!pQ.empty()) {

		if (time >= pQ.top().getArrivalTime()) {

			const Process& currentProcess = pQ.top();
			time += currentProcess.getBurstTime();
			ganttChart.emplace(currentProcess.getPid(), time);
			pQ.pop();
		}
		else {

			time = pQ.top().getArrivalTime();
			ganttChart.emplace(std::string(), time);
		}
	}

	return ganttChart;
}
/*

GanttChart SJF(ProcessQueue q)
{
}


GanttChart SRTF(ProcessQueue q)
{
}


GanttChart RR(ProcessQueue q, const double timeQuantum)
{
}


GanttChart priority(ProcessQueue q)
{
}


GanttChart preemptivePriority(ProcessQueue q)
{
}


GanttChart priorityRR(ProcessQueue q, const double timeQuantum)
{
}*/