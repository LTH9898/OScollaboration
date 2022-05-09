#pragma once
#include <string>
#include <queue>


class Process
{
public:
	Process()
		: pid(), arrivalTime(0.0), burstTime(0.0), priority(0) {}
	Process(std::string pid, double arrivalTime, double burstTime, unsigned priority)
		: pid(pid), arrivalTime(arrivalTime), burstTime(burstTime), priority(priority) {}
	Process(const Process& rhs)
		: pid(rhs.pid), arrivalTime(rhs.arrivalTime), burstTime(rhs.burstTime), priority(rhs.priority) {}

	void setPid(const std::string& pid) { this->pid = pid; }
	void setArrivalTime(const double arrivalTime) { this->arrivalTime = arrivalTime; }
	void setBurstTime(const double burstTime) { this->burstTime = burstTime; }
	void setPriority(const unsigned priority) { this->priority = priority; }

	std::string getPid() const { return pid; }
	double getArrivalTime() const { return arrivalTime; }
	double getBurstTime() const { return burstTime; }
	unsigned getPriority() const { return priority; }

	Process& operator=(const Process& rhs);

	// 프로세스 객체와 'double'의 연산자로 서비스시간을 감소
	// 예) Process P1 ( burstTime = 10 )
	//     P1 -= 3
	// 결과) P1 ( burstTime = 7 )
	Process& operator-=(const double time);
	Process& operator+=(const double time);
	const Process operator-(const double time);
	const Process operator+(const double time);

private:
	std::string pid;
	double arrivalTime;
	double burstTime;
	unsigned priority;
};


auto CMP_PROCESS = [](const Process& lhs, const Process& rhs) { return lhs.getArrivalTime() > rhs.getArrivalTime(); };

// 도착 시간이 가장 작은 프로세스가 가장 높은 우선순위를 갖는 프로세스 큐
using ProcessQueue = std::priority_queue<Process, std::vector<Process>, decltype(CMP_PROCESS)>;
// 프로세스 큐 생성 함수
ProcessQueue createProcessQueue();
// 간트 차트에 프로세스 ID와 그 프로세스의 종료 시점을 기록
using GanttChart = std::queue<std::pair<std::string, double>>;




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
	*this -= -time;
	return *this;
}

const Process Process::operator-(const double time)
{
	Process temp(*this);
	temp -= time;
	return temp;
}

const Process Process::operator+(const double time)
{
	Process temp(*this);
	temp += time;
	return temp;
}


ProcessQueue createProcessQueue()
{
	return ProcessQueue(CMP_PROCESS);
}