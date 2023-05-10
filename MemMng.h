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

//class for logical address
class logicaladdress {

public:
	int pid;
	int pno;
	int offset;
	logicaladdress()
	{
		this->pid = 0;
		this->pno = 0;
		this->offset = 0;
	}
	logicaladdress(int pid, int pno, int offset)
	{
		this->pid = pid;
		this->pno = pno;
		this->offset = offset;
	}

	inline int pid_generator() {
		int pid = rand() % 10;
		return pid;
	}

	inline int pno_generator() {
		int pno = rand() % 9;
		return pno;
	}

	inline int offset_generator() {
		int offset = rand() % 8;
		return offset;
	}

	inline void getlogicaladdress(int& pid, int& pno, int& offset) {
		this->pid = pid_generator();
		pid = this->pid;
		this->pno = pno_generator();
		pno = this->pid;
		this->offset = offset_generator();
		offset = this->offset;
	}


	inline void print()
	{
		cout << "pid: " << pid << " pno: " << pno << " offset: " << offset << endl;
	}
};

/************************************* hash page table ********************************/

class HNode
{
public:
	int pno;
	int fno;
	int offset;
	HNode* next;

	//default constructor
	HNode()
	{
		this->pno = 0;
		this->fno = 0;
		this->offset = 0;
		this->next = NULL;
	}
	//parameterize constructor
	HNode(int pno, int fno, int offset)
	{
		this->pno = pno;
		this->fno = fno;
		this->offset = offset;
		this->next = NULL;
	}
};
// HashMap Class Declaration
class Hashing
{
private:
	HNode** htable;
public:
	int TS = 10;
	Hashing()
	{
		htable = new HNode * [TS];
		for (int i = 0; i < TS; i++)
			htable[i] = NULL;
	}
	~Hashing()
	{
		for (int i = 0; i < TS; ++i)
		{
			HNode* current = htable[i];
			while (current != NULL)
			{
				HNode* prev = current;
				current = current->next;
				delete prev;
			}
		}
		delete[] htable;
	}

	//Hash Function 
inline int HashFunc(int pno)
	{
		return pno % TS;
	}

	//function to check weather the random no generated before or not
inline	bool checkframeno(int frameno) {
		for (int i = 0; i < TS; i++) {
			HNode* temp = htable[i];
			while (temp != NULL) {
				if (temp->fno == frameno) {
					return true;
				}
				temp = temp->next;
			}
		}
		return false;
	}

	//generating random frame no corresponding to page no
inline	int getframeno() {
		int frameno = (1 + rand() % 10) + (1 + rand() % 10);
		//	cout << "Frame no is: " << frameno << endl;

			//check random no generated before or not
		if (checkframeno(frameno))
			frameno = (rand() % 10 + rand() % 10);
		else
			return frameno;

		return frameno;
	}

inline	int getoffset(int pno) {
		int count = -1;
		HNode* temp = htable[pno];
		for (int i = 0; temp->next != NULL; i++) {
			count++;
			temp = temp->next;
		}

		return count;
	}

inline	void Insert(int pno)
	{
		HNode* temp = NULL;
		int hash_val = HashFunc(pno);
		HNode* prev = NULL;
		HNode* current = htable[hash_val];
		while (current != NULL)
		{
			prev = current;
			current = current->next;
		}
		if (current == NULL)
		{
			int frameno = getframeno();
			current = new HNode(pno, frameno, 0);

			if (prev == NULL)
			{
				htable[hash_val] = current;
			}
			else
			{
				//setting the offset of every node
				temp = prev;
				prev->next = current;
				current->offset = temp->offset + 1;

			}
		}
		else
		{
			int frameno = getframeno();
			current->pno = pno;
			current->fno = frameno;
		}
	}

inline	void display() {
		cout << "\nhtable[pageno] ->||pg#|frame#|offset||\n" << endl;
		for (int i = 0; i < TS; i++) {
			HNode* current = htable[i];
			cout << "htable[" << i << "] ";
			while (current != NULL) {
				cout << " ->  " << current->pno << " " << current->fno << " " << current->offset;
				current = current->next;
			}
			cout << endl;
		}
	}

	//function claculating the logical address using hashpage-table
inline	int getlogicaladdress(int pno, int offset, int& pageno, int& frameno) {
		int logicaladdress = 0;

		int hash_val = HashFunc(pno);
		HNode* current = htable[hash_val];
		while (current != NULL) {
			if (((current->pno) % 10 == hash_val)  && current->offset == offset) {
				//offset = current->offset;
				pageno = current->pno;
				frameno = current->fno;
				logicaladdress = current->pno + offset;
				return logicaladdress;
			}
			current = current->next;
		}
		return -1;
	}

};
inline int random(int size) {
	int pno = rand() % size;
	return pno;
}

/************************************* inverted page table ***************************/

const int TABLE_SIZE = 5;

// Node Class Declaration
class Node
{
public:
	//process id
	int pid;
	//page no
	int pno;
	//pointer to next node in table
	Node* next;

	//default constructor
	Node()
	{
		this->pno = 0;
		this->pid = 0;
		this->next = NULL;
	}
	//parameterize constructor
	Node(int pno, int pid)
	{
		this->pno = pno;
		this->pid = pid;
		this->next = NULL;
	}
};

// inverted page table class declaration
class invertedpageTable
{
private:
	Node* htable;
public:
	invertedpageTable()
	{
		//table size = fame no in main memory
		htable = new Node[TABLE_SIZE];
		htable = NULL;
	}
	~invertedpageTable()
	{

		Node* current = htable;
		while (current != NULL)
		{
			Node* prev = current;
			current = current->next;
			delete prev;
		}
		delete htable;
	}
	//function to generate the process id
	inline int generateProcessId() {
		int pid = rand() % 10;
		return pid;
	}

	//inserting process id and page no in inverted page table
	inline void Insert(int p_no, int p_id)
	{
		Node* prev = NULL;
		Node* current = htable;
		while (current != NULL)
		{
			prev = current;
			current = current->next;
		}
		if (current == NULL) {
			current = new Node(p_id, p_no);
			if (prev == NULL)
				htable = current;
			else
				prev->next = current;

		}
		else {
			current->pid = p_id;
			current->pno = p_no;
		}
	}

	//search the process id and page no in inverted page table to get corresponding frame no
	inline  int searchToGetFrameNO(int p_id, int pno) {
		int count_fno = 0;
		Node* current = htable;
		while (current != NULL) {
			if (current->pid == p_id && current->pno == pno)
				return count_fno;

			count_fno++;
			current = current->next;
		}
		if (current == NULL) {
			cout << "\n BHAI KIYA KR RAHA HAI! PAGE NO OR PID NHI HAI PAGE TABLE MA\n\n";
			return -1;
		}
	}

	inline void display() {
		cout << "\nptable [] \n -> pid# pno# \n" << endl;
		Node* current = htable;
		while (current != NULL) {
			cout << " -> " << current->pid << " " << current->pno << " " << endl;
			current = current->next;
		}
		cout << endl;
	}


};

namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MemMng
	/// </summary>
	public ref class MemMng : public System::Windows::Forms::Form
	{
	public:
		MemMng(void)
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
		~MemMng()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button1;
	protected:

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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MemMng::typeid));
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
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
			this->label4->Location = System::Drawing::Point(173, 26);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(214, 37);
			this->label4->TabIndex = 29;
			this->label4->Text = L"OS STIMULATOR";
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::Color::DarkViolet;
			this->button5->FlatAppearance->BorderSize = 0;
			this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button5->Location = System::Drawing::Point(193, 178);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(163, 23);
			this->button5->TabIndex = 33;
			this->button5->Text = L"INVERTED PAGE TBALE";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &MemMng::button5_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::DarkViolet;
			this->button3->FlatAppearance->BorderSize = 0;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Location = System::Drawing::Point(193, 111);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(163, 23);
			this->button3->TabIndex = 32;
			this->button3->Text = L"HASH PAGE TABLE";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &MemMng::button3_Click);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
				static_cast<System::Int32>(static_cast<System::Byte>(24)));
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->ForeColor = System::Drawing::Color::Indigo;
			this->button2->Location = System::Drawing::Point(459, 264);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(78, 32);
			this->button2->TabIndex = 35;
			this->button2->Text = L"BACK";
			this->button2->UseVisualStyleBackColor = false;
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Indigo;
			this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->ForeColor = System::Drawing::Color::SeaShell;
			this->button1->Location = System::Drawing::Point(34, 264);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(84, 32);
			this->button1->TabIndex = 34;
			this->button1->Text = L"EXIT";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MemMng::button1_Click);
			// 
			// MemMng
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(578, 464);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label4);
			this->Name = L"MemMng";
			this->Text = L"MemMng";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	
		   

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		Application::Exit();
	}

	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		Hashing hash;
		int pageno = 0;
		int frameno = 0;
		long int process = 0;
		int size = sizeof(process) * 4;

		//now the size of 1 page = 4 bytes
		int pages = size / 4;

		//generating random page no between 0-3
		/*
		hash.Insert(random(pages));
		hash.Insert(random(pages));
		hash.Insert(random(pages));
		hash.Insert(random(pages));
		*/
		hash.Insert(0);
		hash.Insert(1);
		hash.Insert(2);
		hash.Insert(3);
		hash.Insert(5);
		hash.Insert(4);
		hash.Insert(6);
		hash.Insert(7);
		hash.Insert(8);
		hash.Insert(9);
		hash.Insert(10);
		hash.Insert(13);
		hash.Insert(17);
		hash.Insert(16);

		hash.display();

		cout << endl;
		cout << "\nEnter the page no : and offset to get the logical address" << endl;
		int pno, offset;
		cin >> pno;
		cin >> offset;
		int logicaladdress = hash.getlogicaladdress(pno, offset, pageno, frameno);
		cout << "Logical address is: " << logicaladdress << endl;
		cout << "Page no of node is: " << pageno << endl;
		cout << "Frame no of node is: " << frameno << endl;
		//cout << "Offset of node is : " << offset;
	}
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {

		invertedpageTable* ipt = new invertedpageTable();

		ipt->Insert(ipt->generateProcessId(), 2);//pid , pno
		ipt->Insert(ipt->generateProcessId(), 3);
		ipt->Insert(ipt->generateProcessId(), 4);
		ipt->Insert(ipt->generateProcessId(), 5);
		ipt->Insert(ipt->generateProcessId(), 6);
		ipt->display();



		/*******************************/

		int p_id = 0;
		int p_no = 0;

		cout << "\n TO GET CORRESPONDING FRAME NO FROM PAGE TABLE ENTER THESE DETAILS \n";
		cout << "\n Enter Page no: ";
		cin >> p_no;
		cout << "\n Enter Process id: ";
		cin >> p_id;

		int frameno = ipt->searchToGetFrameNO(p_id, p_no);//pid,pno
		cout << "\nFrame no is : " << frameno;

		/*******************************/
		cout << "\n\n TO GET CORRESPONDING FRAME NO FROM PAGE TABLE ENTER THESE DETAILS CASE CHECKER\n";
		cout << "\n Enter Page no: ";
		cin >> p_no;
		cout << "\n Enter Process id: ";
		cin >> p_id;

		int frameno1 = ipt->searchToGetFrameNO(7, 8);


		/*******************************************/
	   /*
		//assuming this logical address is generated by CPU
		int pid = 0;
		int pNO = 0;
		int offset = 0;
		logicaladdress* la = new logicaladdress();
		la->getlogicaladdress(pid, pNO, offset);

		//putting this logical address in page table

		ipt->Insert(pid, pNO);

		for (int i = 0; i < 5; i++) {
			la->getlogicaladdress(pid, pNO, offset);
			ipt->Insert(pid, pNO);
		}

		ipt->display();
		*/
	}
};
}

