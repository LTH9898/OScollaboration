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

	// 프로세스 객체와 'double'의 연산자로 서비스시간을 감소
	// 예) Process P1 ( burstTime = 10 )
	//     P1 -= 3
	// 결과) P1 ( burstTime = 7 )
	Process& operator-=(const double time);
	Process& operator+=(const double time);
	const Process operator-(const double time);
	const Process operator+(const double time);

private:
	std::string pid;		// 프로세스 ID
	double arrivalTime;		// 도착시간
	double burstTime;		// 서비스시간
	unsigned priority;		// 우선순위
};


auto CMP_PROCESS = [](const Process& lhs, const Process& rhs) { return lhs.GetArrivalTime() > rhs.GetArrivalTime(); };

// 도착 시간이 가장 작은 프로세스가 가장 높은 우선순위를 갖는 프로세스 큐
using ProcessQueue = std::priority_queue<Process, std::vector<Process>, decltype(CMP_PROCESS)>;
// 프로세스 큐 생성 함수
std::unique_ptr<ProcessQueue> CreateProcessQueue()
	{ return std::make_unique<ProcessQueue>(CMP_PROCESS); }

// 간트 차트에 프로세스 ID와 그 프로세스의 종료 시점을 기록
using GanttChart = std::list<std::pair<std::string, double>>;

#endif
