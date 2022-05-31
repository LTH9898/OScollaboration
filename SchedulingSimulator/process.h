#ifndef __PROCESS_H__
#define __PROCESS_H__
#include <string>
#include <queue>
#include <list>
#include <memory>

class Process
{
public:
	Process()
		: pid(""), arrivalTime(0.0), burstTime(0.0), priority(0) {}
	Process(std::string pid, double arrivalTime, double burstTime, unsigned priority)
		: pid(pid), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority) {}
	Process(const Process& rhs)
		: pid(rhs.pid), arrivalTime(rhs.arrivalTime), burstTime(rhs.burstTime), priority(rhs.priority) {}

	void SetPid(const std::string& pid) { this->pid = pid; }
	void SetArrivalTime(const double arrivalTime) { this->arrivalTime = arrivalTime; }
	void SetBurstTime(const double burstTime) { this->burstTime = burstTime; }
	void SetPriority(const unsigned priority) { this->priority = priority; }

	std::string GetPid() const { return pid; }
	double GetArrivalTime() const { return arrivalTime; }
	double GetBurstTime() const { return burstTime; }
	unsigned GetPriority() const { return priority; }

	Process& operator=(const Process& rhs);
	Process& operator-=(const double time);
	Process& operator+=(const double time);
	Process operator-(const double time) const;
	Process operator+(const double time) const;

private:

	std::string pid;
	double arrivalTime;
	double burstTime;
	unsigned priority;
};

using GanttChart = std::list<std::pair<std::string, double>>;

#endif
