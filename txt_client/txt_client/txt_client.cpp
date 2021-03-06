#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include<iostream>
#include<string>
#include<ctime>
#include<fstream>
#include <regex>
#pragma comment (lib, "ws2_32.lib")
using namespace std;
// propozycje statusu: blad wykroczenia poza zakres, przydziel id, wysylam argumenty, przesylam wynik, zakoncz
// [operacja | status | identyfikator | liczba1 | liczba2 | znacznik_czasu]
//operacja | status | identyfikator | znacznik_czasu | operator_i_liczby
string p_data;
string operacja;
// dodane statusy: przydzielId, NULL, odslWynik, zakoncz
//operacja: zakoncz (?), daj_id (?), op_wiele_liczb
string status;
string id;
string liczba1;
string liczba2;
string znacznik_czasu;
string op_wiele_liczb;//+operator
string inttoString(int liczba)
{
	string s = to_string(liczba);
	return s;
}
string numtoString(double liczba)
{
	string s = to_string(liczba);
	string ss;
	//usuwanie niepotrzebnych zer np. 3.1000000 = 3.1
	int i = s.size() - 1;
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
void getNumber()
{
	double n1, n2;
	string l1, l2;
	cout << "\nPodaj liczbe1: ";
	cin >> n1;
	cout << "\nPodaj liczbe2: ";
	cin >> n2;
	l1 = numtoString(n1);
	l2 = numtoString(n2);
	liczba1 = l1;
	liczba2 = l2;
}
void uzupelnij_czas()////pobiera date i wstawia do znacznika czasu
{
	znacznik_czasu.clear();
	SYSTEMTIME time;
	GetLocalTime(&time);
	int days = time.wDay;
	int months = time.wMonth;
	int years = time.wYear;
	int hours = time.wHour;
	int minutes = time.wMinute;
	int seconds = time.wSecond;
	int miliseconds = time.wMilliseconds;
	znacznik_czasu = inttoString(days) + "-" + inttoString(months) + "-" + inttoString(years) + "|" + inttoString(hours) + ":" + inttoString(minutes) + ":" + inttoString(seconds) + ":" + inttoString(miliseconds);
}
void insert()//umieszcza dane w stringu do wyslania
{
	p_data.clear();
	uzupelnij_czas();
	if (operacja != "przydzielId"&&operacja!="zakoncz"&&operacja!="wiele_liczb")
		p_data += "Operacja: " + operacja + ";Status: " + status + ";Identyfikator: " + id + ";czas: " + znacznik_czasu + ";Liczba1: " + liczba1 + ";Liczba2: " + liczba2 + ";";
	else
	{
		if(operacja=="wiele_liczb")
			p_data += "Operacja: " + operacja + ";Status: " + status + ";Identyfikator: " + id + ";czas: " + znacznik_czasu + ";operator_i_liczby: "+op_wiele_liczb+";";
		else
			p_data += "Operacja: " + operacja + ";Status: " + status + ";Identyfikator: " + id + ";czas: " + znacznik_czasu + ";";
	}
		
}
void wyluskaj(const string &p_data2)
{
	const string s = p_data2;
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
	//liczba1
	regex rgx4("Liczba1: (\\w+);");
	smatch match4;
	if (regex_search(s.begin(), s.end(), match4, rgx4))
	liczba1 = match4[1];
	//liczba2
	regex rgx5("Liczba2: (\\w+);");
	smatch match5;
	if (regex_search(s.begin(), s.end(), match5, rgx5))
	liczba2 = match5[1];

}

void interfaceC()
{

	int x;
	cout << "\nWybierz akcje ktora chcesz wykonac:" << endl;
	cout << "0. Operacje na wielu argumentach" << endl;
	cout << "1. Dodawanie" << endl;
	cout << "2. Odejmowanie" << endl;
	cout << "3. Mnozenie" << endl;
	cout << "4. Dzielenie" << endl;
	cout << "5. Modulo" << endl;
	cout << "6. Potegowanie" << endl;
	cout << "7. Zespolona" << endl;
	cout << "8. Pole walca" << endl;
	cout << "9. Zakonczenie polaczenia" << endl;
	cin >> x;

	// zabezpieczenie przed podaniem nr zadania, ktorego nie ma
	while ((x > 9) || (x < 0))
	{
		cout << "Nie ma takiego zadania. Podaj nr z przedzialu 1-6" << endl;
		cin >> x;
	}

	switch (x)
	{
		case 0:
		{
			op_wiele_liczb.clear();
			operacja = "wiele_liczb";
			status = "NULL";
			char znak;
			string liczba;
			cout << "\nPodaj znak operacji (+-*/): ";
			cin >> znak;
			if (znak != '+'&&znak != '-'&&znak != '*'&&znak != '/')
				cout << "\nPodano nieprawidlowe dane!";
			else
			{
				op_wiele_liczb = op_wiele_liczb + znak+" ";
				cout << "\nAby zakonczyc podawanie liczb wcisnij N";
				while (liczba !="N")
				{
					cout << "\nPodaj liczbe: ";
					cin >> liczba;
					op_wiele_liczb = op_wiele_liczb+liczba+" ";
				}
				cout << "\nZakonczono podawanie liczb";
				op_wiele_liczb.resize(op_wiele_liczb.length() -3);//usuwanie znaku N
			}
			insert();
			break;
		}
		case 1: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "dodaj";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 2: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "odejmij";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 3: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "mnozenie";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 4: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "dzielenie";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 5: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "modulo";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 6: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "potegowanie";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 7: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "zespolona";
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 8: // wczytywanie dwoch liczb od uzytkownika do dodania
		{
			operacja = "pwalca"; //pole walca = 2*pi*r^2 + 2*pi*r*H
			status = "NULL";
			getNumber();
			insert();
			break;
		}
		case 9:
		{
			operacja = "zakoncz";
			status = "NULL";
			insert();
			break;
		}
	}

}
int main()
{
	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
		cout << "Initialization error" << endl;

	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mainSocket == INVALID_SOCKET)
	{
		cout << "Error creating socket: " << WSAGetLastError();
		WSACleanup();
		return 1;
	}
	
	
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");//192.168.0.1
	service.sin_port = htons(27016);

	if (connect(mainSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "Failed to connect" << endl;
		WSACleanup();
		return 1;
	}
	cout << "Server connected" << endl;
	//zeby wylosowal ID: 
	int bytesSent, i=0;
	id = "null";
	operacja = "przydzielId";
	status = "null";
	insert();
	char recvbuf[1200] = "";
	int bytesRecv = !SOCKET_ERROR;
	//
	do {
		if (i == 0)
		{
			bytesSent = send(mainSocket, p_data.c_str(), p_data.length(), 0);
			cout << "\nsent: " << p_data;
			bytesRecv = recv(mainSocket, recvbuf, sizeof(recvbuf), 0);
			if (bytesRecv == SOCKET_ERROR)
				break;
			cout << "\nRecv: " << recvbuf;
			wyluskaj(recvbuf);
			insert();
		}
		interfaceC();
		bytesSent = send(mainSocket, p_data.c_str(), p_data.length(), 0);
		cout << "\nsent: " << p_data;
		if (operacja == "zakoncz")
			break;
		ZeroMemory(recvbuf, sizeof(recvbuf));//zeby nic nie zostalo
		bytesRecv = recv(mainSocket, recvbuf, sizeof(recvbuf), 0);
		if (bytesRecv == SOCKET_ERROR)
			break;
		cout << "\nRecv: " << recvbuf;
		i++;
	} while (operacja!="zakoncz");
	cout << "\nConnection closed";
	getchar();
	getchar();
	closesocket(mainSocket);

	WSACleanup();
}




