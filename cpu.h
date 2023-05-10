#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include <process.h>
#include <windows.h>
#include <cstdlib>s
#include <string>
#include <cstdio>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

using namespace std;



// 1) MultiLevel Feedback Queue SCHEDULING

struct process1
{
	int B_Time;
	int Turnaround_Time;
	int  Waiting_Time;
	int Remaining_Time;
};

inline void RoundRobin(std::vector<process1>& v1, int quantum)
{

	for (int i = 0; i < v1.size(); i++)
	{

		if (v1.at(i).Remaining_Time >= quantum)
		{
			v1.at(i).Remaining_Time -= quantum;
			for (int j = 0; j < v1.size(); j++)
			{
				if (j != i)
				{
					v1.at(j).Waiting_Time = v1.at(j).Waiting_Time + quantum;
				}
			}
		}
		else
		{
			for (int j = 0; j < v1.size(); j++)
			{
				if (j != i)
				{
					v1.at(j).Waiting_Time = v1.at(j).Waiting_Time + v1.at(i).Remaining_Time;
				}
			}
			v1.at(i).Remaining_Time = 0;
		}


	}
}

inline void Calculate_Turnaround(std::vector<process1>& v1)
{
	for (int i = 0; i < v1.size(); i++)
	{
		v1.at(i).Turnaround_Time = v1.at(i).Waiting_Time + v1.at(i).B_Time;
	}
}

inline float avgWaiting(std::vector<process1> v1, int num)
{
	float avg = 0;
	for (int i = 0; i < v1.size(); i++)
	{
		avg = avg + v1.at(i).Waiting_Time;
	}
	float avgf = avg / num;
	return avgf;
}

inline float avgTAT(std::vector<process1> v1, int num)
{
	float avg = 0;
	for (int i = 0; i < v1.size(); i++)
	{
		avg = avg + v1.at(i).Turnaround_Time;
	}
	float avgf = avg / num;
	return avgf;
}

inline void print(std::vector<process1> v1)
{
	for (int i = 0; i < v1.size(); i++)
	{
		cout << v1.at(i).B_Time << v1.at(i).Remaining_Time << v1.at(i).Waiting_Time << v1.at(i).Turnaround_Time;
	}
	cout << endl;
}

inline void MultiLevelFeedbackQueue()
{
	int num;
	cout << "Enter the total number of process: " << endl;
	cin >> num;
	struct process1 p[1000];

	cout << "Enter the burst time of each process: " << endl;
	for (int i = 0; i < num; i++)
	{
		cout << "burst time of process " << i + 1 << ": ";
		cin >> p[i].B_Time;
		p[i].Remaining_Time = p[i].B_Time;
		p[i].Waiting_Time = 0;
	}


	std::vector<process1> v;

	for (int i = 0; i < num; i++)
	{
		v.push_back(p[i]);
	}


	RoundRobin(v, 8);
	RoundRobin(v, 16);

	Calculate_Turnaround(v);

	cout << endl;
	float Average_Turnaround = avgTAT(v, num);
	float Average_Waiting = avgWaiting(v, num);
	cout << "Average TAT is:  " << Average_Turnaround << endl;
	cout << "average Waiting Time is: " << Average_Waiting << endl;
}



// 2) MULTI-LEVEL QUEUE SCHEDULING

struct process2
{
	int pri;
	int b_time;
	int tt_time;
	int total_t = 0;
};

struct queues
{
	int priority_start;
	int priority_end;
	int total_t = 0;
	int length = 0;
	process2* p;
	bool executed = false;
};

inline bool notComplete(queues q[]) {
	bool a = false;
	int countInc = 0;
	for (int i = 0; i < 3; i++) {
		countInc = 0;
		for (int j = 0; j < q[i].length; j++) {
			if (q[i].p[j].b_time != 0) {
				a = true;
			}
			else {
				countInc += 1;
			}
		}
		if (countInc == q[i].length) {

			q[i].executed = true;
		}
	}
	return a;
}

inline void sort_ps(queues q) {
	//Queue q has to be sorted according to priority of processes
	for (int i = 1; i < q.length; i++) {
		for (int j = 0; j < q.length - 1; j++) {
			if (q.p[j].pri < q.p[j + 1].pri) {
				process2 temp = q.p[j + 1];
				q.p[j + 1] = q.p[j];
				q.p[j] = temp;
			}
		}
	}
}

inline void checkCompleteTimer(queues q[]) {
	bool a = notComplete(q);
	for (int i = 0; i < 3; i++) {
		if (q[i].executed == false) {
			for (int j = 0; j < q[i].length; j++) {
				if (q[i].p[j].b_time != 0) {
					q[i].p[j].total_t += 1;
				}
			}
			q[i].total_t += 1;
		}
	}
}

inline void MultilevelQueue()
{
	//Initializing 3 queues
	queues q[3];
	q[0].priority_start = 7;
	q[0].priority_end = 9;
	q[1].priority_start = 4;
	q[1].priority_end = 6;
	q[2].priority_start = 1;
	q[2].priority_end = 3;

	int totl, priority_of_process, burst_time;
	cout << "Enter the number of processes\n";
	cin >> totl;
	process2 p1[5];

	for (int i = 0; i < totl; i++) {
		cout << "Enter the priority of the process " << i + 1 << ": ";
		cin >> priority_of_process;
		cout << "Enter the burst time of the process " << i + 1 << ": ";
		cin >> burst_time;
		cout << endl << endl;
		p1[i].pri = priority_of_process;
		p1[i].b_time = burst_time;
		p1[i].tt_time = burst_time;
		for (int j = 0; j < 3; j++) {
			if (q[j].priority_start <= priority_of_process && priority_of_process <= q[j].priority_end) {
				q[j].length++;
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		int len = q[i].length;
		q[i].p = new process2[len];
	}


	int a = 0;
	int b = 0;
	int c = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < totl; j++) {
			if ((q[i].priority_start <= p1[j].pri) && (p1[j].pri <= q[i].priority_end)) {
				if (i == 0) {
					q[i].p[a++] = p1[j];

				}
				else if (i == 1) {
					q[i].p[b++] = p1[j];
				}
				else {
					q[i].p[c++] = p1[j];
				}
			}
		}
	}

	a--; b--; c--;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < q[i].length; j++) {
			cout << q[i].p[j].pri << "->";
		}
		cout << "NULL\n";
	}

	int timer = 0;
	int l = -1;
	int rr_timer = 4;
	int counter = 0;
	int counterps = 0;
	int counterfcfs = 0;
	while (notComplete(q)) {
		if (timer == 10) {
			timer = 0;
		}
		l += 1;
		if (l >= 3) {
			l = l % 3;
		}


		//Finally you now have a queue which is not completely executed
		//Process the incomplete processes over it

		if (l == 0) {
			if (rr_timer == 0) {
				rr_timer = 4;
			}

			for (int i = 0; i < q[l].length; i++) {
				if (q[l].p[i].b_time == 0) {
					counter++;
					continue;
				}
				if (counter == q[l].length) {
					break;
				}
				while (rr_timer > 0 && q[l].p[i].b_time != 0 && timer != 10) {
					q[l].p[i].b_time--;
					checkCompleteTimer(q);
					rr_timer--;
					timer++;

				}
				if (timer == 10) {
					break;
				}


			}
		}


		else {
			//FCFS
			for (int i = 0; i < q[l].length; i++) {
				if (q[l].p[i].b_time == 0) {
					counterfcfs++;
					continue;
				}
				if (counterfcfs == q[l].length) {
					break;
				}
				while (q[l].p[i].b_time != 0 && timer != 10) {
					q[l].p[i].b_time--;
					checkCompleteTimer(q);

					timer++;
				}
				if (timer == 10) {
					break;
				}
				if (q[l].p[i].b_time == 0) {
					continue;
				}

			}

		}
	}

	int total_tt = 0;
	int total_wt = 0;

	cout << "\n\nProcess     | Turn Around Time | Waiting Time\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < q[i].length; j++) {
			cout << "Process P" << j + 1 << "\t" << q[i].p[j].total_t << "\t\t    " << q[i].p[j].total_t - q[i].p[j].tt_time << "\n";
			total_tt += q[i].p[j].total_t;
			total_wt += q[i].p[j].total_t - q[i].p[j].tt_time;
		}
	}

	int TAT;
	TAT = total_tt / totl;

	int AWT;
	AWT = total_wt / totl;
	cout << endl << endl;
	cout << "Average TAT is: " << TAT << endl;
	cout << "Average waiting time is: " << AWT << endl;
}



// 3) LONGEST JOB FIRST

class LJF
{
public:
	int n;
	int j;
	int total;
	int index;
	int temp;
	int i;
	float avg_wt, avg_tat;
	int A[100][4];

inline	void func() {
		cout << "Enter number of process: ";
		cin >> n;
		cout << "Enter Burst Time:" << endl;
		for (i = 0; i < n; i++) {
			cout << "Process" << i + 1 << " ";
			cin >> A[i][1];
			A[i][0] = i + 1;
		}

		cout << "The burst times for the processes are: " << endl;
		for (i = 0; i < n; i++) {
			cout << A[i][1] << " ";
		}



		for (i = 0; i < n; i++) {
			index = i;
			for (j = i + 1; j < n; j++)
				if (A[j][1] > A[index][1])
					index = j;
			temp = A[i][1];
			A[i][1] = A[index][1];
			A[index][1] = temp;

			temp = A[i][0];
			A[i][0] = A[index][0];
			A[index][0] = temp;
		}
		cout << endl;

		cout << "The sorted processes according ot their burst times are: " << endl;
		for (i = 0; i < n; i++) {
			cout << A[i][1] << " ";
		}
		cout << endl << endl;

		A[0][2] = 0;
		// Calculation of Waiting Times
		for (i = 1; i < n; i++) {
			A[i][2] = 0;
			for (j = 0; j < i; j++)
				A[i][2] += A[j][1];
			total += A[i][2];
		}

		avg_wt = (float)total / n;
		total = 0;
		cout << "Process    Burst Time    Waiting Time   Turn Around Time" << endl;
		// Calculation of Turn Around Time and printing the
		// data.
		for (i = 0; i < n; i++) {
			A[i][3] = A[i][1] + A[i][2];
			total += A[i][3];
			cout << A[i][0] << "           " << A[i][1] << "             " << A[i][2] << "              " << A[i][3];
			cout << endl;
		}
		cout << endl << endl;
		avg_tat = (float)total / n;
		cout << endl;
		cout << "Average Waiting Time is: " << avg_wt << endl;
		cout << "Average TAT is: " << avg_tat;
		cout << endl << endl;
	}
};

inline void longestJobFirst()
{
	LJF l1;
	l1.func();
}


// 4) HIGHEST FEEDBACK RATIO NEXT SCHEDULING

class RatioFeedback {

private:
	int processID;
	int burstTime;
	int arrivalTime;
	int waitingTime;
	int turnaroundTime;
	float FeedbackRatio;

public:

	RatioFeedback(int processID, int burstTime, int arrivalTime, int WT)
	{
		this->processID = processID;
		this->burstTime = burstTime;
		this->arrivalTime = arrivalTime;
		this->waitingTime = WT;
		this->turnaroundTime = 0;
		this->FeedbackRatio = 0;
	}
	inline int getProcessID() {
		return processID;
	}
	inline int getBurstTime() {
		return burstTime;
	}
	inline int getArrivalTime() {
		return arrivalTime;
	}
	inline int getWaitingTime() {
		return waitingTime;
	}
	inline int getTurnaroundTime() {
		return turnaroundTime;
	}
	inline float getFeedbackRatio() {
		return FeedbackRatio;
	}
	inline void setWaitingTime(int waitingTime) {
		this->waitingTime = waitingTime;
	}
	inline void setTurnaroundTime(int turnaroundTime) {
		this->turnaroundTime = turnaroundTime;
	}
	inline void setFeedbackRatio(int FeedbackRatio) {
		this->FeedbackRatio = FeedbackRatio;
	}
	inline void INPUT()
	{
		cout << "Enter Process ID : ";
		cin >> processID;
		cout << "Enter Burst Time : ";
		cin >> burstTime;
		cout << "Enter Arrival Time : ";
		cin >> arrivalTime;
		cout << "Enter Waiting Time : ";
		cin >> waitingTime;
	}
};

inline void HFR(vector<RatioFeedback>* jobs)
{
	//Feedback Ratio is calculated by the given formula :
//1. Feedback Ratio = (W + S) / S
//Where : W is Waiting Time, S is Service Time or Burst Time

	for (int i = 0; i < jobs->size(); i++)
	{
		float FeedbackRatio = (jobs->at(i).getWaitingTime() + jobs->at(i).getBurstTime()) / jobs->at(i).getBurstTime();
		jobs->at(i).setFeedbackRatio(FeedbackRatio);
	}

	// first we need to sort the jobs according to their arrival time

	for (int i = 0; i < jobs->size(); i++)
	{
		for (int j = 0; j < jobs->size() - 1; j++)
		{
			if (jobs->at(j).getArrivalTime() > jobs->at(j + 1).getArrivalTime())
			{
				RatioFeedback temp = jobs->at(j);
				jobs->at(j) = jobs->at(j + 1);
				jobs->at(j + 1) = temp;
			}
		}
	}
	// now if two jobs have same arrival time then we need to sort them according to their feedbackratio 
	for (int i = 0; i < jobs->size(); i++)
	{
		for (int j = 0; j < jobs->size() - 1; j++)
		{
			if (jobs->at(j).getArrivalTime() == jobs->at(j + 1).getArrivalTime())
			{
				if (jobs->at(j).getFeedbackRatio() < jobs->at(j + 1).getFeedbackRatio())
				{
					RatioFeedback temp = jobs->at(j);
					jobs->at(j) = jobs->at(j + 1);
					jobs->at(j + 1) = temp;
				}
			}
		}
	}

	int currentTime = 0;
	int totalWaitingTime = 0;
	int totalTurnaroundTime = 0;
	int totalResponseTime = 0;
	int totalJobs = jobs->size();
	int totalBurstTime = 0;
	for (int i = 0; i < jobs->size(); i++)
	{
		totalBurstTime += jobs->at(i).getBurstTime();
	}
	int currenttime = 0;
	for (int i = 0; i < jobs->size(); i++)
	{
		if (jobs->at(i).getArrivalTime() > currenttime)
		{
			currenttime = jobs->at(i).getArrivalTime();
		}
		//jobs->at(i).setWaitingTime(currenttime - jobs->at(i).getArrivalTime());
		currenttime += jobs->at(i).getBurstTime();
		jobs->at(i).setTurnaroundTime(currenttime - jobs->at(i).getArrivalTime());
		totalWaitingTime += jobs->at(i).getWaitingTime();
		totalTurnaroundTime += jobs->at(i).getTurnaroundTime();
	}
	cout << "Process ID\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\tFeedback Ratio" << endl;
	for (int i = 0; i < jobs->size(); i++)
	{
		cout << jobs->at(i).getProcessID() << "\t\t" << jobs->at(i).getBurstTime() << "\t\t" << jobs->at(i).getArrivalTime() << "\t\t" << jobs->at(i).getWaitingTime() << "\t\t" << jobs->at(i).getTurnaroundTime() << "\t\t" << jobs->at(i).getFeedbackRatio() << endl;
	}
	cout << "Average Waiting Time : " << (float)totalWaitingTime / totalJobs << endl;
	cout << "Average Turnaround Time : " << (float)totalTurnaroundTime / totalJobs << endl;
	//cout << "Average Response Time : " << (float)totalResponseTime / totalJobs << endl;
}
inline void HFRmenu()
{
	vector<RatioFeedback> jobs;
	int n;
	cout << "Enter Number of Processes : ";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		RatioFeedback job(0, 0, 0, 0);
		job.INPUT();
		jobs.push_back(job);
	}
	HFR(&jobs);
}




namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for cpu
	/// </summary>
	public ref class cpu : public System::Windows::Forms::Form
	{
	public:
		cpu(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~cpu()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label1;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(cpu::typeid));
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::Color::DarkViolet;
			this->button6->FlatAppearance->BorderSize = 0;
			this->button6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button6->Location = System::Drawing::Point(132, 284);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(269, 23);
			this->button6->TabIndex = 40;
			this->button6->Text = L"HIGHEST FEEDBACK RATIO NEXT SCHEDULING";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &cpu::button6_Click);//HFRNS
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::Color::DarkViolet;
			this->button5->FlatAppearance->BorderSize = 0;
			this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button5->Location = System::Drawing::Point(132, 204);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(269, 23);
			this->button5->TabIndex = 39;
			this->button5->Text = L"MULTILEVEL FEEDBACK QUEUE SCHEDULING";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &cpu::button5_Click);//LJF
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::DarkViolet;
			this->button4->FlatAppearance->BorderSize = 0;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Location = System::Drawing::Point(132, 245);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(269, 23);
			this->button4->TabIndex = 38;
			this->button4->Text = L"LONGEST JOB FIRST";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &cpu::button4_Click);//MFQS
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::DarkViolet;
			this->button3->FlatAppearance->BorderSize = 0;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Location = System::Drawing::Point(132, 162);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(269, 23);
			this->button3->TabIndex = 37;
			this->button3->Text = L"MULTILEVEL QUEUE SCHEDULING";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &cpu::button3_Click);//MQS
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(24)));
			this->label4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->ForeColor = System::Drawing::Color::Indigo;
			this->label4->Location = System::Drawing::Point(155, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(214, 37);
			this->label4->TabIndex = 36;
			this->label4->Text = L"OS STIMULATOR";
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(24)));
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->ForeColor = System::Drawing::Color::Indigo;
			this->button2->Location = System::Drawing::Point(456, 355);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(78, 32);
			this->button2->TabIndex = 35;
			this->button2->Text = L"Sign In";
			this->button2->UseVisualStyleBackColor = false;
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Indigo;
			this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->ForeColor = System::Drawing::Color::SeaShell;
			this->button1->Location = System::Drawing::Point(31, 355);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(84, 32);
			this->button1->TabIndex = 34;
			this->button1->Text = L"EXIT";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &cpu::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(24)));
			this->label1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::Color::Indigo;
			this->label1->Location = System::Drawing::Point(142, 91);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(236, 37);
			this->label1->TabIndex = 41;
			this->label1->Text = L"CPU SCHEDULING";
			// 
			// cpu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(565, 414);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"cpu";
			this->Text = L"cpu";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		MultilevelQueue();
	}
		   
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
		MultiLevelFeedbackQueue();
	}
		   
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
		longestJobFirst();
	}
	
	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		HFRmenu();
	}
};
}
