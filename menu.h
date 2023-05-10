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
#include <cstdlib>
#include <string>
#include <cstdio>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <iomanip>

using namespace std;

#include "MemMng.h"
#include "cpu.h"



// ----------------------------------------------BANKER'S ALGORITHM----------------------------------------------

inline int* SUM(int** array, int rows, int cols)
{
	int* sum = new int[cols];

	for (int i = 0; i < cols; i++)
	{
		sum[i] = 0;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			sum[j] += array[i][j];
		}
	}

	//cout << "sum array is: " << endl;
	//for (int i = 0; i < cols; i++)
	//{
	//	cout << sum[i] << " ";
	//}
	//cout << endl;

	return sum;
}

inline bool Compare(int* array1, int* array2, int s1, int s2)
{
	if (s1 != s2)
	{
		return false;
	}

	for (int i = 0; i < s1; i++)
	{
		if (array1[i] > array2[i])
		{
			return false;
		}
	}
	return true;
}

inline void Bankers_Algorithem()
{
	int total_resources;
	int total_processes;
	int* total_resource_array = nullptr;
	int** allocated_resources = nullptr;
	int** max_resources = nullptr;
	int* total_Allocated = nullptr;
	int* available_resources = nullptr;
	int** need = nullptr;
	int* safe_state = nullptr;
	int safe_state_index = 0;
	bool is_terminated;
	bool compare = true;

	cout << "enter total number of resources: ";
	cin >> total_resources;

	total_resource_array = new int[total_resources];

	cout << "enter the total for each resource: " << endl;
	for (int i = 0; i < total_resources; i++)
	{
		cout << "enter the total for resource number " << i + 1 << ": ";
		cin >> total_resource_array[i];
	}
	
	cout << endl;

	cout << "enter the total number of processes: ";
	cin >> total_processes;

	cout << "enter allocated resources for each process: " << endl;

	allocated_resources = new int* [total_processes];

	for (int i = 0; i < total_processes; i++)
	{
		allocated_resources[i] = new int[total_resources];
	}

	cout << "enter the data now: " << endl;
	for (int i = 0; i < total_processes; i++)
	{
		for (int j = 0; j < total_resources; j++)
		{
			cout << "enter the amount of resources " << j + 1 << " allocated to process " << i + 1 << ": ";
			cin >> allocated_resources[i][j];
		}
	}

	max_resources = new int* [total_processes];
	for (int i = 0; i < total_processes; i++)
	{
		max_resources[i] = new int[total_resources];
	}

	cout << "enter the maximum resource for each process: " << endl;
	for (int i = 0; i < total_processes; i++)
	{
		for (int j = 0; j < total_resources; j++)
		{
			cout << "enter the maximum amount of resources " << j + 1 << " needed by the process " << i + 1 << ": ";
			cin >> max_resources[i][j];
		}
	}

	// available resources


	total_Allocated = new int[total_resources];

	total_Allocated = SUM(allocated_resources, total_processes, total_resources);

	available_resources = new int[total_resources];

	for (int i = 0; i < total_resources; i++)
	{
		available_resources[i] = total_resource_array[i] - total_Allocated[i];
	}

	need = new int* [total_processes];
	for (int i = 0; i < total_processes; i++)
	{
		need[i] = new int[total_resources];
	}

	// need for each process

	for (int i = 0; i < total_processes; i++)
	{
		for (int j = 0; j < total_resources; j++)
		{
			need[i][j] = max_resources[i][j] - allocated_resources[i][j];
		}
	}

	safe_state = new int[total_processes];

	while (compare)
	{
		compare = false;

		for (int i = 0; i < total_processes; i++)
		{
			is_terminated = false;
			for (int j = 0; j < safe_state_index; j++)
			{
				if (safe_state[j] == i)
				{
					is_terminated = true;
				}
			}

			if (!is_terminated)
			{
				if (Compare(need[i], available_resources, total_resources, total_resources))
				{
					safe_state[safe_state_index] = i;
					safe_state_index++;

					for (int j = 0; j < total_resources; j++)
					{
						available_resources[j] += allocated_resources[i][j];
					}

					compare = true;
				}
			}
		}
	}

	cout << "safe state is: < ";
	for (int i = 0; i < safe_state_index; i++)
	{
		cout << "process" << safe_state[i] + 1 << " ";
	}
	cout << " >  "<<endl;

	if (total_processes == safe_state_index)
	{
		cout << "system not in deadlock: " << endl;
	}

	else
	{
		cout << "system in deadlock: " << endl;
	}

}

// ------------------------------------------SECOND CHANCE ALGORITHM-----------------------------------------------

inline bool checkMemory(vector <pair<int, bool>> mem)
{
	int occupied = 0;
	for (int i = 0; i < mem.size(); i++)
	{
		if (mem[i].first != -1)
		{
			occupied++;
		}
		else
		{
			continue;
		}
	}

	if (occupied < mem.size())
	{
		return true;
	}

	else
	{
		return false;
	}
}

inline bool checkAvailability(vector <pair<int, bool>> mem, int target)
{
	for (int i = 0; i < mem.size(); i++)
	{
		if (mem[i].first == target)
		{
			return true;
		}
	}

	return false;
}

inline vector <pair<int, bool>> updateQueue(vector <pair<int, bool>> queue, int index, int target)
{
	for (; index < (queue.size() - 1); index++)
	{
		queue[index] = queue[index + 1];
	}

	queue[index].first = target;
	queue[index].second = false;

	return queue;
}

inline vector <int> strToArr(string temp_refStr)
{
	vector <int> ref_Str;

	stringstream text_stream(temp_refStr);
	string item;
	while (std::getline(text_stream, item, ','))
	{
		ref_Str.push_back(stoi(item));
	}

	return ref_Str;
}

inline int getIndex(vector<pair<int, bool>> mem, int target)
{
	for (int i = 0; i < mem.size(); i++)
	{
		if (mem[i].first == target)
		{
			return i;
		}
	}

	return -1;
}

inline void printQ(vector <pair<int, bool>> queue)
{
	cout << endl << endl;
	for (int i = 0; i < queue.size(); i++)
	{
		cout << queue[i].first << " " << queue[i].second << endl;
	}
	cout << endl << endl;
}

inline void SCA_Calculation(int pf, int tot)
{
	float pageFault = (float)pf;
	float total = (float)tot;

	float PageFault_Probability = pageFault / total;
	int pageFault_Percentage = PageFault_Probability * 100;

	cout << "Total Page Slots are: " << total << endl;
	cout << "Total Page Faults are: " << pageFault << endl;
	cout << "Page Fault Probabilty is: " << PageFault_Probability << endl;
	cout << "Page Fault Percentage is: " << pageFault_Percentage << "%" << endl;
}

inline void Second_Chance_Algorithm()
{
	int page_slots;
	int page_fault = 0;
	string temp_refStr;
	vector <int> ref_String;
	vector <pair<int, bool>> memorySpace;
	vector<pair<int, bool>> queue;

	cout << "enter total page slots: ";
	cin >> page_slots;

	cout << "enter reference string: ";
	cin >> temp_refStr;

	ref_String = strToArr(temp_refStr);

	for (int i = 0; i < page_slots; i++)
	{
		memorySpace.push_back(make_pair(-1, false));
	}

	for (int count = 0; count < ref_String.size(); count++)
	{

		// if memory is not full
		if (checkMemory(memorySpace))
		{
			for (int i = 0; i < memorySpace.size(); i++)
			{
				if (memorySpace[i].first == ref_String[count])
				{
					memorySpace[i].second = true;
					queue[i].second = true;
					//printQ(queue);
					break;
				}

				else if (memorySpace[i].first == -1)
				{
					memorySpace[i].first = ref_String[count];
					page_fault++;
					queue.push_back(make_pair(ref_String[count], false));
					//printQ(queue);
					break;
				}
			}
		}

		// if memory is full
		else
		{
			if (checkAvailability(memorySpace, ref_String[count]))
			{
				bool forQueue = false;
				for (int i = 0; i < memorySpace.size() && forQueue == false; i++)
				{
					if (memorySpace[i].first == ref_String[count])
					{
						memorySpace[i].second = true;
						forQueue = true;
					}

					if (forQueue)
					{

						for (int i = 0; i < queue.size(); i++)
						{
							if (queue[i].first == ref_String[count])
							{
								queue[i].second = true;
								//printQ(queue);
								break;
							}
						}
					}
				}
			}

			// for replacing
			else
			{
				for (int i = 0; i < memorySpace.size(); i++)
				{
					if (queue[i].second == false)
					{
						int index = getIndex(memorySpace, queue[i].first);
						memorySpace[index].first = ref_String[count];
						memorySpace[index].second = false;
						//cout << queue[i].first << " " << queue[i].second << endl;
						queue = updateQueue(queue, i, ref_String[count]);
						//printQ(queue);
						page_fault++;
						break;
					}
					else
					{
						queue[i].second = false;
						continue;
					}
				}
			}
		}
	}

	SCA_Calculation(page_fault, ref_String.size());
}

// -------------------------------------------------SEMAPHORES--------------------------------------------------------

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for menu
	/// </summary>
	public ref class menu : public System::Windows::Forms::Form
	{
	public:
		menu(void)
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
		~menu()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button7;
	protected:
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button1;

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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(menu::typeid));
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::Color::DarkViolet;
			this->button7->FlatAppearance->BorderSize = 0;
			this->button7->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button7->Location = System::Drawing::Point(200, 313);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(163, 23);
			this->button7->TabIndex = 33;
			this->button7->Text = L"PAGE REPLACEMENT";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &menu::button7_Click);
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::Color::DarkViolet;
			this->button6->FlatAppearance->BorderSize = 0;
			this->button6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button6->Location = System::Drawing::Point(200, 267);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(163, 23);
			this->button6->TabIndex = 32;
			this->button6->Text = L"MEMORY MANAGEMENT";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &menu::button6_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::Color::DarkViolet;
			this->button5->FlatAppearance->BorderSize = 0;
			this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button5->Location = System::Drawing::Point(200, 179);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(163, 23);
			this->button5->TabIndex = 31;
			this->button5->Text = L"SEMAPHORE";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &menu::button5_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::DarkViolet;
			this->button4->FlatAppearance->BorderSize = 0;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Location = System::Drawing::Point(200, 223);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(163, 23);
			this->button4->TabIndex = 30;
			this->button4->Text = L"DEADLOCK";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &menu::button4_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::DarkViolet;
			this->button3->FlatAppearance->BorderSize = 0;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Location = System::Drawing::Point(200, 133);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(163, 23);
			this->button3->TabIndex = 29;
			this->button3->Text = L"CPU SCHEDULING";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &menu::button3_Click);
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
			this->label4->Location = System::Drawing::Point(173, 52);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(214, 37);
			this->label4->TabIndex = 28;
			this->label4->Text = L"OS STIMULATOR";
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(24)));
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->ForeColor = System::Drawing::Color::Indigo;
			this->button2->Location = System::Drawing::Point(465, 379);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(78, 32);
			this->button2->TabIndex = 27;
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
			this->button1->Location = System::Drawing::Point(40, 379);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(84, 32);
			this->button1->TabIndex = 26;
			this->button1->Text = L"EXIT";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &menu::button1_Click);
			// 
			// menu
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(583, 463);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"menu";
			this->Text = L"menu";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Hide();
		MemMng^ mem = gcnew MemMng();
		mem->ShowDialog();
	}
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		this->Hide();
		cpu^ cp = gcnew cpu();
		cp->ShowDialog();
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
		Bankers_Algorithem();
	}
	private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {
		Second_Chance_Algorithm();
	}
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
		//semaphore
	}
};
}
