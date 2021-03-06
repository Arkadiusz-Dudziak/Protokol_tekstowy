#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#define _USE_MATH_DEFINES
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <WS2tcpip.h>
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include<string>
#include <random>
#include <sstream> 
#include<fstream>
#include <math.h>
#include <regex>
#include <list>//
#include <algorithm>//
#pragma comment (lib, "ws2_32.lib")
using namespace std;
string p_data;
string operacja;
string status;
string id;
string liczba1;
string liczba2;
string znacznik_czasu;
string op_wiele_liczb;//+operator
vector<double> liczby_vec;
/*
 statusy: zly_arg -> co najmniej jeden z podanych argumentow (do liczenia) jest nieprawdidlowy
*/

//wartosci po przecinku
double stringtonum(string s)
{
	double x = 0;
	if (s[0] != '-')
	{
		stringstream geek(s);
		geek >> x;
		return x;//liczba dodatnia
	}
	else
	{
		string pom;
		for (int i = 1; i < s.size(); i++)
			pom = pom + s[i];
		stringstream geek(pom);
		geek >> x;
		return (-1) * x;//liczba ujemna
	}
}
void extractIntegerWords(string str)//zapisuje liczby ze stringa do vectora
{
	liczby_vec.clear();
	stringstream ss;

	/* Storing the whole string into string stream */
	ss << str;

	/* Running loop till the end of the stream */
	string temp;
	double found;
	while (!ss.eof())
	{

		/* extracting word by word from stream */
		ss >> temp;
		/* Checking the given word is double or not */
		if (stringstream(temp) >> found)
		{
			//cout << "\nliczby_vec.push_back(stringtonum(" << temp << "))";
			liczby_vec.push_back(stringtonum(temp));
		}

		/* To save from space at the end of string */
		temp = "";
	}
}
double extractIntegerWord(string str)
{
	stringstream ss;
	double numb=0;
	/* Storing the whole string into string stream */
	ss << str;

	/* Running loop till the end of the stream */
	string temp;
	double found;
	while (!ss.eof())
	{

		/* extracting word by word from stream */
		ss >> temp;
		/* Checking the given word is double or not */
		if (stringstream(temp) >> found)
		{
			//cout << "\nliczby_vec.push_back(stringtonum(" << temp << "))";
			numb=(stringtonum(temp));
		}

		/* To save from space at the end of string */
		temp = "";
	}
	return numb;
}
int stringtoint(string s)
{
	int x = 0;
	if (s[0] != '-')
	{
		stringstream geek(s);
		geek >> x;
		return x;//liczba dodatnia
	}
	else
	{
		string pom;
		for (int i = 1; i < s.size(); i++)
			pom = pom + s[i];
		stringstream geek(pom);
		geek >> x;
		return (-1) * x;//liczba ujemna
	}
}

string inttoString(int liczba)
{
	string s = to_string(liczba);
	return s;
}
//wartosci po przecinku
string numtoString(double liczba)
{
	string s = to_string(liczba);
	string ss;
	//usuwanie niepotrzebnych zer np. 3.1000000 = 3.1
	int i = s.size()-1;
	while (s[i] == '0')
		i--;
	if (s[i] == '.')
		i--;
	for (int j = 0; j <= i; j++)
	{
		ss = ss + s[j];
	}	
	return ss;
}
void writeId()//losuje id
{
	int max = 99999, min = 11111;
	int x = (rand() % (max - min) + min);
	id=inttoString(x);
	status = "przydzielono_Id";
}
void uzupelnij_czas()//pobiera date i wstawia do znacznika czasu
{
	znacznik_czasu.clear();
	SYSTEMTIME time;
	GetLocalTime(&time);
	GetLocalTime(&time);
	int days = time.wDay;
	int months = time.wMonth;
	int years = time.wYear;
	int hours = time.wHour;
	int minutes = time.wMinute;
	int seconds = time.wSecond;
	int miliseconds = time.wMilliseconds;
	znacznik_czasu = inttoString(days)+ "-"+inttoString(months)+"-"+ inttoString(years)+"|" +inttoString(hours) + ":" + inttoString(minutes) + ":" + inttoString(seconds)+":"+ inttoString(miliseconds);
}
void insert()//umieszcza dane w stringu do wyslania
{
	p_data.clear();
	uzupelnij_czas();
	if (status != "zly_arg"&&status != "przydzielono_Id"&&status!="wynik_poza_zakresem")//wysylamy wynik
	{
		p_data += "Operacja: " + operacja + ";Status: " + status + ";Identyfikator: " + id + ";czas: " + znacznik_czasu + ";wynik: " + liczba1 + ";";

	}
	else// nie mozna bylo policzyc, bez wyniku
		p_data += "Operacja: " + operacja + ";Status: " + status + ";Identyfikator: " + id + ";czas: " + znacznik_czasu + ";";

}
void wyluskaj(const string &p_data2)
{
	
	const string s = p_data2;
	//cout << "\ns: " << s;
	//operacja
	regex rgx("Operacja: (\\w+);");
	smatch match;
	if (regex_search(s.begin(), s.end(), match, rgx))
	operacja = match[1];
	//status
	regex rgx2("Status: (\\w+);");
	smatch match2;
	if (regex_search(s.begin(), s.end(), match2, rgx2))
	status = match2[1];
	//id
	regex rgx3("Identyfikator: (\\w+);");
	smatch match3;
	if (regex_search(s.begin(), s.end(), match3, rgx3))
	id = match3[1];
	//liczba1 (-?\d+[\.\d+]?)+   ||   (-?\\w+)
	regex rgx4("Liczba1: ([-?\\d+\\.]+);");
	smatch match4;
	if (regex_search(s.begin(), s.end(), match4, rgx4))
	{
		liczba1 = match4[1];
		//cout << "\nliczba1 r: " << liczba1;
	}
	
	//liczba2
	regex rgx5("Liczba2: ([-?\\d+\\.]+);");
	smatch match5;
	if (regex_search(s.begin(), s.end(), match5, rgx5))
	{
		liczba2 = match5[1];
		//cout << "\nliczba2: " << liczba2;
	}
	
	//operator + wiele liczb
	regex rgx6("operator_i_liczby: ([+\\-*\\/][ -?\\d+]+);");
	smatch match6;
	if (regex_search(s.begin(), s.end(), match6, rgx6))
		op_wiele_liczb = match6[1];
}

void oblicz()
{
	int blad = 0;
	/*int wynik=0, l1,l2;
	l1 = stringtoint(liczba1);
	l2 = stringtoint(liczba2);*/
	double l1, l2, wynik = 0;
	l1 = extractIntegerWord(liczba1);
	l2 = extractIntegerWord(liczba2);
	if (operacja == "dodaj")
	{
		//cout << "\nl1: " << l1;
		status = "zwracamWynik";
		wynik = l1 + l2;
	}
		
	if (operacja == "odejmij")
	{
		status = "zwracamWynik";
		wynik = l1 - l2;
	}
		
		
	if (operacja == "mnozenie")
	{
		status = "zwracamWynik";
		wynik = l1 * l2;
	}
		
	if (operacja == "dzielenie")
	{
		if (l2 != 0)
		{
			wynik = l1 / l2;
			status = "zwracamWynik";
		}
			
		else
		{
			status = "zly_arg";
		}
	}
	if (operacja == "modulo")
	{
		if (l2 != 0)
		{
			status = "zwracamWynik";
			wynik = (int)l1 % (int)l2;
		}
		else
		{
			status = "zly_arg";
		}
	}
	if (operacja == "potegowanie")
	{
		status = "zwracamWynik";
		wynik = pow(l1,l2);
		if (isinf(wynik))
		{
			status = "wynik_poza_zakresem";
			blad = 1;
			wynik = 0;
		}
			
	}
	if (operacja == "zespolona")//z=sqrt(l1^2+l2^2)
	{
		status = "zwracamWynik";
		wynik = sqrt(l1*l1 + l2 * l2);
	}
	if (operacja == "pwalca")
	{
		if (l1 > 0 && l2 > 0)//l1 promien, l2=h
		{
			status = "zwracamWynik";
			wynik = 2 * M_PI*l1*l1 + 2 * M_PI*l1*l2;
		}
		else
		{
			status = "zly_arg";
		}
	}
	if (operacja == "wiele_liczb")
	{
		int i;
		extractIntegerWords(op_wiele_liczb);
		char znak = op_wiele_liczb[0];
		//cout<<"\nOdczytane liczby: ";
		for (i = 0; i < liczby_vec.size(); i++)
			cout << liczby_vec[i] << " ";
		//cout << "\nznak: " << znak;
		switch (znak)
		{
			case '+':
			{
				for (i = 0; i < liczby_vec.size(); i++)
					wynik = wynik + liczby_vec[i];
				break;
			}
			case '-':
			{

				for (i = 0; i < liczby_vec.size(); i++)
				{
					if (i == 0)
						wynik = liczby_vec[i];
					else
						wynik = wynik - liczby_vec[i];
				}
				break;
			}
			case '*':
			{
				wynik = 1;
				for (i = 0; i < liczby_vec.size(); i++)
					wynik = wynik * liczby_vec[i];
				break;
			}
			case '/':
			{
				for (i = 0; i < liczby_vec.size(); i++)
				{
					if (i == 0)
						wynik = liczby_vec[i];
					else
						wynik = wynik / liczby_vec[i];

					if (liczby_vec[i] == 0)
					{
						status = "zly_arg";
						blad = 1;
					}
						
				}
				break;
			}
		}
		if(blad!=1)
		status = "zwracamWynik";
		
	}
	if (operacja != "zakoncz")
	{
		liczba1 = numtoString(wynik);
		//cout << "wynik: " << wynik;
		liczba2 = "0";
	}
	//cout << "\nwynik: " << wynik;
}
int main()
{
	using namespace std;
	srand(time(NULL));
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
		cout << "WSAStartup error" << endl;

	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mainSocket == INVALID_SOCKET)
	{
		cout << "Error creating socket: " << WSAGetLastError();
		WSACleanup();
		return 1;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");//127.0.0.1//192.168.0.2
	service.sin_port = htons(27016);

	if (bind(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "bind() failed" << endl;
		closesocket(mainSocket);
		return 1;
	}
	if (listen(mainSocket, 1) == SOCKET_ERROR)
		cout << "Error listening on socket" << endl;

	SOCKET acceptSocket = SOCKET_ERROR;
	cout << "Waiting for a client to connect..." << endl;

	while (acceptSocket == SOCKET_ERROR)
	{
		acceptSocket = accept(mainSocket, NULL, NULL);
	}

	cout << "Client connected" << endl;
	mainSocket = acceptSocket;
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	//char sendbuf[32] = "";
	char recvbuf[120000] = "";
	/////
	char host[NI_MAXHOST];
	char service2[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service2, NI_MAXSERV);
	if (getnameinfo((sockaddr*)&service, sizeof(service), host, NI_MAXHOST, service2, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service2 << endl;
	}
	////
	while(operacja!="zakoncz")
	{
		ZeroMemory(recvbuf, sizeof(recvbuf));//zeby nic nie zostalo
		bytesRecv = recv(mainSocket, recvbuf, 120000, 0);
		//cout << "Bytes received: " << bytesRecv << endl;
		if (bytesRecv == SOCKET_ERROR)
			break;
		cout << "\nReceived text: " << recvbuf;
		wyluskaj(recvbuf);
		if (operacja == "przydzielId")
			writeId();
		oblicz();
		insert();
		int i = 0, send_res = 1;
		char sendbuf[120];
		//strcpy_s(sendbuf, p_data.c_str());
		if (operacja != "zakoncz")
		{
			bytesSent = send(mainSocket, p_data.c_str(), p_data.length(), 0);
			cout << "\nSend text:     " << p_data;
			//cout << "Bytes sent: " << bytesSent << endl;
		}
	}
	
	if (shutdown(mainSocket, SD_RECEIVE) == SOCKET_ERROR)
	{
		cout << "shutdown failed with error: " << WSAGetLastError();
		closesocket(mainSocket);
		WSACleanup();
		return 1;
	}
	else
		cout << "Connection closed" << endl;
	getchar();
	getchar();
}







