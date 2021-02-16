#pragma once
#include <cstring>
#include <stdlib.h>
#undef UNICODE
#define _WINSOCK_DEPRECATED_NO_WARNINGS //for no inet_ntoa() warning
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501 //so getaddrinfo() works
//#include "definitions.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <string> //to convert bytes into string
#include <msclr\marshal_cppstd.h> //for String^ to std::string
#include <cliext/map>

namespace ChatApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	
	struct IPinfo
	{
		const char* ip;
		int numCon = 1;
	};
	/// <summary>
	/// Summary for HostServer
	/// </summary>
	public ref class HostServer : public System::Windows::Forms::Form
	{
	public:
		HostServer(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		String^ getIP(SOCKET& clientSocket)
		{
			SOCKADDR_IN clientInfo = { 0 };
			int addrsize = sizeof(clientInfo);
			getpeername(clientSocket, (struct sockaddr*)&clientInfo, &addrsize);
			return gcnew String(inet_ntoa(clientInfo.sin_addr));
		}
		void SendWelcome(SOCKET clientSoc)
		{
			std::string welcomeMessage = "\n\nWelcome to the Server!\r\n\r(type \"./leave\" to leave the server)\r\n\r";
			const char* welcomeBuf = welcomeMessage.c_str(); //22
			int resultSend = send(clientSoc, welcomeBuf, (int)strlen(welcomeBuf), 0);
			if (resultSend == SOCKET_ERROR)
			{
				MessageBox::Show("Sending Welcome failed with error: " + Convert::ToString(WSAGetLastError()));
				closesocket(clientSoc);
				WSACleanup();
			}
		}
		void SendFULL(SOCKET clientSoc)
		{
			std::string fullMessage = "\n\nToo many connections!\nPlease wait until traffic from your network subsides\nTry again later";
			const char* fullBuf = fullMessage.c_str();
			int resultSend = send(clientSoc, fullBuf, (int)strlen(fullBuf), 0);
			printf("Trying to send: %s of length: %d\n", fullBuf, (int)strlen(fullBuf));
			if (resultSend == SOCKET_ERROR)
			{
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(clientSoc);
				WSACleanup();
			}
		}
		void ShowConn()
		{
			textBox1->Text += "------------------------------------------\r\n\r";
			for each (IPmap::value_type elem in IPObj)
				textBox1->Text += elem->second + " connection(s) from IPv4 " + elem->first + "\r\n\r";
			textBox1->Text += "Total number of connection(s): " + numClients + "\r\n\r";
			textBox1->Text += "------------------------------------------\r\n\r";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HostServer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::ComponentModel::BackgroundWorker^ backgroundWorker1;

		   SOCKET ListenSocket = INVALID_SOCKET;
		   typedef cliext::map<String^, int> IPmap;
		   IPmap IPObj; //for inserting pairs (and consequently other functions)
		   int numClients = 0;
		   String^ ServerIP;

	#define DEFAULT_PORT "27015"
	#define DEFAULT_BUFLEN 512
		  
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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(HostServer::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::Transparent;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16));
			this->label1->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->label1->Location = System::Drawing::Point(311, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(131, 26);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Your IPv4 is";
			this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->label1->Click += gcnew System::EventHandler(this, &HostServer::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::Transparent;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->label2->Location = System::Drawing::Point(188, 91);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(386, 20);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Clients will need to know this to join your server";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::Transparent;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14));
			this->label3->ForeColor = System::Drawing::SystemColors::ButtonFace;
			this->label3->Location = System::Drawing::Point(312, 58);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(60, 24);
			this->label3->TabIndex = 3;
			this->label3->Text = L"label3";
			this->label3->Click += gcnew System::EventHandler(this, &HostServer::label3_Click);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Tahoma", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(12, 141);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(760, 359);
			this->textBox1->TabIndex = 4;
			this->textBox1->TabStop = false;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &HostServer::textBox1_TextChanged);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &HostServer::backgroundWorker1_DoWork);
			// 
			// HostServer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(784, 512);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"HostServer";
			this->Text = L"HostServer";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &HostServer::HostServer_FormClosing);
			this->Load += gcnew System::EventHandler(this, &HostServer::HostServer_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void HostServer_Load(System::Object^ sender, System::EventArgs^ e) 
	{
		textBox1->Text += "Booting Server...\r\n\r";
		WSADATA wsaData;

		int resultStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (resultStart != 0)
		{
		MessageBox::Show("WSAStartup failed");
			this->Close();
			Application::Exit();
		}

		struct addrinfo* result = NULL, hints;
		//void that Fills a block of memory with zeros.
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET; //address family. rhs = IPv4 addrs family
		hints.ai_socktype = SOCK_STREAM; //socket type.rhs=2way,connection based byte stream(uses TCP)
		hints.ai_protocol = IPPROTO_TCP; //uses TCP << ^^
		hints.ai_flags = AI_PASSIVE;//indicates will used returned addr to call bind()

		int flagCheck = resultStart = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (resultStart != 0) {
			MessageBox::Show("getaddrinfo failed with error: " + Convert::ToString(resultStart));
			WSACleanup();
			Application::Exit();
		}

		//listen socket to listen for client connections
		
		ListenSocket = socket(result->ai_family, //The address family specification. 
			result->ai_socktype, //The type specification for the new socket
			result->ai_protocol); //The protocol to be used. 
		if (ListenSocket == INVALID_SOCKET) //make sure socket is valid
		{
			MessageBox::Show("Error at socket(): " + Convert::ToString(WSAGetLastError()));
			freeaddrinfo(result);
			WSACleanup();
			Application::Exit();
		}
		//Binding a socket:
		//"For a server to accept client connections, it must be bound to a network address within the system."
		//binding tcp listenning socket
		resultStart = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (resultStart == SOCKET_ERROR) //making sure bind returns valid val
		{
			MessageBox::Show("Bind failed with error: " + Convert::ToString(WSAGetLastError()));
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			Application::Exit();
		}

		SOCKET tempSock = ListenSocket; //needed this to use temp for getIP (doesn't work if directly input ListenSocket)
		char tb[24];
		gethostname(tb, 24);
		struct hostent* phe = gethostbyname(tb);
		if (phe == 0) 
		{
			MessageBox::Show("Error retreiving Host IP");
			Application::Exit();
		}

		for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
		{
			struct in_addr addr;
			memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
			ServerIP = gcnew String(inet_ntoa(addr));
		}
		label3->Text = ServerIP;

		freeaddrinfo(result);// frees the memory allocated by the getaddrinfo function 

		//Listening on a Socket:
		//second arg is mag length of queue (pending connections)
		//SOMAXCONN: max reasonable value
		if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			MessageBox::Show("Listen failed with error: " + Convert::ToString(WSAGetLastError()));
			closesocket(ListenSocket);
			WSACleanup();
			Application::Exit();
		}

		backgroundWorker1->RunWorkerAsync(1);
		textBox1->Text += "Server now running\r\n\r";
	}

	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void backgroundWorker1_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e) 
	{
		fd_set master;
		FD_ZERO(&master); //sets master->fd_count to zero?
		FD_SET(ListenSocket, &master);
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		msclr::interop::marshal_context context;
		std::string tempStr;
		while (true)
		{
			fd_set masterCopy = master; //creating copy so master doesnt get overwritten
			//determines the status of one or more sockets, (synchronous)
			int numSockets = select(0, &masterCopy, NULL, NULL, NULL);
			if (numSockets == -1)
			{
				closesocket(ListenSocket);
				WSACleanup();
				Sleep(2000);
				exit(0);
			}

			for (int i = 0; i < numSockets; i++)
			{
				SOCKET tempSock = masterCopy.fd_array[i]; //initializing tempsock to socketArr in master
				if (tempSock == ListenSocket)
				{
					//accept new connection
					SOCKET ClientSocket = INVALID_SOCKET;
					ClientSocket = accept(ListenSocket, NULL, NULL);       
					if (ClientSocket == INVALID_SOCKET)
					{
						closesocket(ListenSocket);
						WSACleanup();
					}
					textBox1->Text += "Client from " + gcnew String(getIP(ClientSocket)) + " has connected\r\n\r";
					cliext::map<String^, int>::const_iterator it;
					it = IPObj.find((getIP(ClientSocket)));
					//checking if unique IP
					if (it != IPObj.end())
					{
						if ((*it)->second < 4) //max num of clients from single network
						{
							(*it)->second++;
							numClients++;
							textBox1->Text += (*it)->second + " /4 for " + (*it)->first + "\r\n\r";
							ShowConn();
						}
						else
						{
							textBox1->Text += "*Max num clients for " + (*it)->first + ". Sending FULL message\r\n\r";
							SendFULL(ClientSocket);
							goto done;
						}
					}
					else
					{
						IPinfo clientIP;
						tempStr = context.marshal_as<std::string>(getIP(ClientSocket));
						clientIP.ip = tempStr.c_str();
						textBox1->Text += gcnew String(clientIP.ip) + " is unique; Adding IP to map\r\n\r";
						IPObj.insert(IPmap::make_value(getIP(ClientSocket), clientIP.numCon));
						numClients++;
						ShowConn();
					}
					it = IPObj.find(gcnew String(getIP(ClientSocket)));
					//adding socket to connected socket array
					FD_SET(ClientSocket, &master);
					SendWelcome(ClientSocket);

				done:;
				}
				else
				{
					//accept and print to server
					memset(recvbuf, '\0', DEFAULT_BUFLEN);
					int recvInt = recv(tempSock, recvbuf, recvbuflen, 0);
					if (recvbuf > 0)
						textBox1->Text += gcnew String(recvbuf) + "\r\n\r";
					if (recvInt <= 0)
					{
						//end connection and drop if recvInt flagged
						textBox1->Text += "Lost client connection from " + gcnew String(getIP(tempSock)) + "\r\n\r";
						//determining if closing connection is unique or not to update map
						cliext::map<String^, int>::iterator it = IPObj.find(Convert::ToString(getIP(tempSock)));
						if ((*it)->second > 1)
						{
							textBox1->Text += "There are multiple connections from " + gcnew String(getIP(tempSock)) + "\r\n\r" + "Subtracting 1 from " + (*it)->first + " in map\r\n\r";
							(*it)->second--;
							closesocket(tempSock);
							FD_CLR(tempSock, &master);
							numClients--;
							ShowConn();
						}
						else
						{
							textBox1->Text += "There was a single connection from " + gcnew String(getIP(tempSock)) + ". Erasing from map\r\n\r";
							IPObj.erase((*it)->first);
							closesocket(tempSock);
							FD_CLR(tempSock, &master);
							numClients--;
							ShowConn();
						}
					}
					else
					{
						//send message to clients
						for (size_t i = 0; i < master.fd_count; i++)
						{
							SOCKET outputSock = master.fd_array[i];
							if (outputSock != ListenSocket) //if socket is not the Server socket, send message.
							{
								send(outputSock, recvbuf, recvInt, 0);
							}

						}
					}
				}

			}
			if (textBox1->Visible)
			{
				textBox1->SelectionStart = textBox1->TextLength;
				textBox1->ScrollToCaret();
			}

		}
	}
	private: System::Void HostServer_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) 
	{
		Application::Exit();
	}
};
}
