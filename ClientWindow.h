#pragma once
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
//so getaddrinfo() works
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <string> //to convert bytes into string
#include <stdlib.h>
#include <msclr\marshal_cppstd.h> //for String^ to std::string
#include "Header.h"

namespace ChatApplication {
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	

	/// <summary>
	/// Summary for ClientWindow
	/// </summary>
	public ref class ClientWindow : public System::Windows::Forms::Form
	{
	public:
		ClientWindow(void)
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
		~ClientWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ textBox1;
	protected:
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::Button^ button1;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

		//ADDED VARS
		String^ tempFull = "\n\nToo many connections!\nPlease wait until traffic from your network subsides\nTry again later";
		String^ tempLeaveMessage = "./leave";
		String^ message;

		SOCKET ConnectSocket = INVALID_SOCKET;
	private: System::ComponentModel::BackgroundWorker^ backgroundWorker1;
		   array<wchar_t>^ recvBuffer = gcnew array<wchar_t>(512);

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(ClientWindow::typeid));
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Tahoma", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(12, 12);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(760, 440);
			this->textBox1->TabIndex = 1;
			this->textBox1->TabStop = false;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &ClientWindow::textBox1_TextChanged);
			// 
			// textBox2
			// 
			this->textBox2->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->textBox2->Location = System::Drawing::Point(133, 475);
			this->textBox2->MaxLength = 512;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(423, 20);
			this->textBox2->TabIndex = 2;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &ClientWindow::textBox2_TextChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(562, 475);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Send";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ClientWindow::button1_Click);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &ClientWindow::backgroundWorker1_DoWork);
			// 
			// ClientWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(784, 512);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"ClientWindow";
			this->Text = L"ClientWindow";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ClientWindow::ClientWindow_FormClosing);
			this->Load += gcnew System::EventHandler(this, &ClientWindow::ClientWindow_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void ClientWindow_Load(System::Object^ sender, System::EventArgs^ e)
	{
		
		WSADATA wsaData;
		int resultStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (resultStart != 0)
		{
			MessageBox::Show("WSAStartup error");
			this->Close();
			Application::Exit();
		}

		struct addrinfo* result = NULL, * ptr = NULL, hints;

		//void that Fills a block of memory with zeros.
		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_UNSPEC; //address family. rhs = unspecified addrs
		hints.ai_socktype = SOCK_STREAM; //socket type. rhs = 2way,connection based byte stream
		hints.ai_protocol = IPPROTO_TCP; //uses TCP << ^^

		#define DEFAULT_PORT "27015"
		
		msclr::interop::marshal_context context;
		std::string temp = context.marshal_as<std::string>(ManagedGlobals::IPstr);
		const char* ip= temp.c_str();
		resultStart = getaddrinfo(
			ip, //pointer to ANSI string that contains host(node) name or numeric host address string
			DEFAULT_PORT,//port number represented as string
			&hints, //
			&result); //response info about the host
		if (resultStart != 0)
		{
			MessageBox::Show("getaddrinfo failed");
			WSACleanup();
			this->Close();
			Application::Exit();
		}
		
		//pointer set to first address returned by getaddrinfo()
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next)//attempting connections
		{												  //ptr-> to next structure in linked list
			//Creating a socket when stable connection
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET)
			{
				MessageBox::Show("Error at socket()");
				freeaddrinfo(result);
				WSACleanup();
				this->Close();
				Application::Exit();
			}
			//Connecting to a Socket:
			resultStart = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (resultStart == SOCKET_ERROR)
			{
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue; //go to next loop iteration (next available connection)
			}
			break;
		}

		freeaddrinfo(result);
		if (ConnectSocket == INVALID_SOCKET) //ultimately faulty connection
		{
			MessageBox::Show("Unable to connect to server!\n");
			WSACleanup();
			this->Close();
			Application::Exit();
		}
		#define DEFAULT_BUFLEN 512
		int recBufLen = DEFAULT_BUFLEN;

		char recBuf[DEFAULT_BUFLEN];
		memset(recBuf, '\0', DEFAULT_BUFLEN);
		this->Show();
		resultStart = recv(ConnectSocket, recBuf, recBufLen, 0); //welcome or FULL message
		if (std::string(recBuf) == "\n\nToo many connections!\nPlease wait until traffic from your network subsides\nTry again later" && resultStart > 0)
		{
			MessageBox::Show(tempFull);
			closesocket(ConnectSocket);
			WSACleanup();
			Application::Exit();
		}
		if (resultStart > 0) 
		{
			//MessageBox::Show("RECEIVED welcome");
			textBox1->Text = gcnew String(recBuf) + "\r\n\r";
		}
		else if (resultStart == 0)
			MessageBox::Show("Connection closed");
		else
			MessageBox::Show("recv failed");
		backgroundWorker1->RunWorkerAsync(1);
		}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
	private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		message = textBox2->Text;
		if (message == gcnew String("./leave"))
		{
			msclr::interop::marshal_context context;
			const char* messageCharP;
			String^ leaveMess = ManagedGlobals::CName + " has left the chat room\r\n\r";
			std::string temp = context.marshal_as<std::string>(leaveMess);
			messageCharP = temp.c_str();
			int byteCheck = send(ConnectSocket, messageCharP, (int)strlen(messageCharP) + 1, 0);
			Sleep(30);
			closesocket(ConnectSocket);
			WSACleanup();
			Application::Exit();
		}
		else
		{ 
			//converting message to suitable type for sending
			msclr::interop::marshal_context context;
			const char* messageCharP;
			message = ManagedGlobals::CName + ": " + message;
			std::string temp = context.marshal_as<std::string>(message);
			messageCharP = temp.c_str();

			int byteCheck = send(ConnectSocket, messageCharP, (int)strlen(messageCharP) + 1, 0);
			if (byteCheck == SOCKET_ERROR) 
			{
				MessageBox::Show("Send failed");
				closesocket(ConnectSocket);
				WSACleanup();
			}
			textBox2->Text = "";
		}
	}
	private: System::Void backgroundWorker1_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
		{
			char recBuf[DEFAULT_BUFLEN];
			memset(recBuf, '\0', DEFAULT_BUFLEN);
			while (true)
			{
				if (backgroundWorker1->CancellationPending) //if it was cancelled
				{
					e->Cancel = true;
					break;
				}
				int byteCheck = recv(ConnectSocket, recBuf, 512, 0);
				if (byteCheck > 0)
				{
					textBox1->Text += gcnew String(recBuf) + "\r\n\r";
				}
				if (textBox1->Visible)
				{
					textBox1->SelectionStart = textBox1->TextLength;
					textBox1->ScrollToCaret();
				}
				Thread::Sleep(300);   //wait for 1/3 second
			}
		}
	private: System::Void ClientWindow_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) 
	{
		msclr::interop::marshal_context context;
		const char* messageCharP;
		String^ leaveMess = "~ " + ManagedGlobals::CName + " has left the chat room ~\r\n\r";
		std::string temp = context.marshal_as<std::string>(leaveMess);
		messageCharP = temp.c_str();
		int byteCheck = send(ConnectSocket, messageCharP, (int)strlen(messageCharP) + 1, 0);
		Application::Exit();
	}
};

}
