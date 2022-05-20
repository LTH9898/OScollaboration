#ifndef __CPU_SCHEDULER_H__
#define __CPU_SCHEDULER_H__
#include "waiting_queue.h"


class CpuScheduler
{
public:
	CpuScheduler() : time(0), isRunning(false), isPreemptive(false), isRoundRobin(false), timeQuantum(-1) {}
	void SetProcessQueue(std::unique_ptr<ProcessQueue> pQ)
		{ this->pQ = std::move(pQ); }
	void SetAlgorithm(Scheduling wQalgorithm, bool isPreemptive, bool isRoundRobin)
		{ wQ.SetAlgorithm(wQalgorithm); this->isPreemptive = isPreemptive; this->isRoundRobin = isRoundRobin; }
	void SetTimeQuantum(const double tq)
		{ timeQuantum = tq;}

	const waitingQueue& GetWQhandler() const
		{ return wQ; }
	const GanttChart& GetGantthandler() const
		{ return ganttChart; }
	bool Empty() const
		{ return ganttChart.empty(); }
	bool IsRunning() const
		{ return isRunning; }

	void StepForward();
	void Reset();

private:
	std::unique_ptr<ProcessQueue> pQ;
	waitingQueue wQ;
	GanttChart ganttChart;
	Process currentProcess;
	double time;
	double timeQuantum;

	bool isPreemptive;
	bool isRoundRobin;
	bool isRunning;
};

#endif
