#include "process.h"

Process& Process::operator=(const Process& rhs)
{
	this->pid = rhs.pid;
	this->arrivalTime = rhs.arrivalTime;
	this->burstTime = rhs.burstTime;
	this->priority = rhs.priority;

	return *this;
}

Process& Process::operator-=(const double time)
{
	this->burstTime -= time;
	return *this;
}

Process& Process::operator+=(const double time)
{
	this->burstTime += time;
	return *this;
}

const Process Process::operator-(const double time)
{
	Process temp(*this);
	temp.burstTime -= time;
	return temp;
}

const Process Process::operator+(const double time)
{
	Process temp(*this);
	temp.burstTime += time;
	return temp;
}
