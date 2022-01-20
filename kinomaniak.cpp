#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include "include/CkMailMan.h"
#include "include/CkEmail.h"

#define LOGIN 0
#define HASLO 1
#define IMIE 2
#define NAZWISKO 3
#define MAIL 4
#define PRZESUNIECIE 4

using namespace std;

vector<vector<string>> WczytajDane(string nazwa_pliku)
{
	vector<vector<string>> content;
	vector<string> row;
	string line, word;

	fstream fileodczyt(nazwa_pliku, ios::in);
	content.clear();
	row.clear();

	if (fileodczyt.is_open())
	{
		while (getline(fileodczyt, line))
		{
			row.clear();

			stringstream str(line);

			while (getline(str, word, ','))
			{
				row.push_back(word);
			}

			content.push_back(row);
		}
	}

	fileodczyt.close();

	return content;
}

void wyslijMaila(string odbiorca, string tresc)
{
	CkMailMan mailman;

	mailman.put_SmtpHost("smtp.gmail.com");

	mailman.put_SmtpUsername("projektkinomaniak@gmail.com");
	mailman.put_SmtpPassword("kinomaniakio1");

	CkEmail email;

	email.put_Subject("Bilety");
	email.put_Body(tresc.c_str());
	email.put_From("projektkinomaniak");
	bool success = email.AddTo(odbiorca.c_str(), odbiorca.c_str());

	success = mailman.SendEmail(email);
	if (success != true) {
		cout << "Taki mail nie istnieje." << "\n";
		return;
	}

	success = mailman.CloseSmtpConnection();
	if (success != true) {
		cout << "Taki mail nie istnieje." << "\n";
	}

	cout << "Mail zostal wyslany!" << "\n";
}

class Logowanie {
private:
	string imie;
	string nazwisko;
	string login;
	string haslo;
	string mail;
public:
	Logowanie( string login_, string haslo_, string imie_, string nazwisko_, string mail_):imie(imie_), nazwisko(nazwisko_), login(login_), haslo(haslo_), mail(mail_){};
	Logowanie() {};

	void setImie(string imie) {
		this->imie = imie;
	}
	void setNazwisko(string nazwisko) {
		this->nazwisko = nazwisko;
	}
	void setLogin(string login) {
		this->login = login;
	}
	void setMail(string mail) {
		this->mail = mail;
	}
	void setHaslo(string haslo) {
		this->haslo = haslo;
	}

	string getImie() {
		return imie;
	}

	string getNazwisko() {
		return nazwisko;
	}

	string getLogin() {
		return login;
	}

	string getMail() {
		return mail;
	}
	string getHaslo() {
		return haslo;
	}


	void PrzypomnienieHasla() {

		vector<vector<string>> content = WczytajDane("dane.csv");
		string imie, nazwisko, login, mail, haslo;
		bool czy_wystepuje = true;
		system("cls");

		fstream fileodczyt("dane.csv", ios::in);

		
		cout << "Wprowadz login: ";
		cin >> login;
		for (int i = 0; i < content.size(); ++i)
		{
			if (content[i][LOGIN] == login)
			{
				cout << "Podaj mail: ";
				cin >> mail;
				if (content[i][MAIL] == mail)
				{
					cout << "Podaj swoje imie: ";
					cin >> imie;
					if (content[i][IMIE] == imie)
					{
						cout << "Podaj swoje nazwisko: ";
						cin >> nazwisko;
						if (content[i][NAZWISKO] == nazwisko)
						{
							haslo = content[i][HASLO];
							for (int j = 0; j <= haslo.length(); j++) {
								if (int(haslo[j]) - PRZESUNIECIE < 48)
								{
									haslo[j] = int(haslo[j]) + 75 - PRZESUNIECIE;
								}
								else
								{
									haslo[j] = int(haslo[j]) - PRZESUNIECIE;
								}

							}
							cout << "\nTwoje haslo to " << haslo << endl << endl;
							system("pause");
							system("cls");
							return;
						}
					}
				}
			}
		}
		system("cls");
		cout << "Wprowadzone dane nie naleza do zadnego uzytkownika\n";
	}

	void UtworzenieKonta(string login, string haslo, string imie, string nazwisko, string mail) 
	{
		

		vector<vector<string>> content = WczytajDane("dane.csv");
		vector<string> row;
		string line, word;



		for (int i = 0; i <= haslo.length(); i++) {
			if (int(haslo[i]) + PRZESUNIECIE > 122)
			{
				haslo[i] = int(haslo[i]) - 75 + PRZESUNIECIE;
			}
			else
			{
				haslo[i] = int(haslo[i]) + PRZESUNIECIE;
			}
		}

		row.clear();
		row.push_back(login);
		row.push_back(haslo);
		row.push_back(imie);
		row.push_back(nazwisko);
		row.push_back(mail);
		content.push_back(row);

		fstream filezapis("dane.csv", ios::out);
		for (int i = 0; i < content.size(); ++i)
		{
			filezapis << content[i][LOGIN] << "," << content[i][HASLO] << "," << content[i][IMIE] << "," << content[i][NAZWISKO] << "," << content[i][MAIL] << "\n";
		}
		filezapis.close();

		cout << "\nRejestracja zakonczona\n\n";

	}

	void EdycjaKonta() 
	{

		vector<vector<string>> content = WczytajDane("dane.csv");
		vector<string> row;
		string line, word;
		int i = 0;

		system("cls");
		while (i < content.size())
		{
			if (content[i][LOGIN] == getLogin())
			{
				break;
			}
			i++;
		}


		string n = "a";
		int x = 1;
		string temp;
		string pewnosc;
		while (n != "0") 
		{
			cout << "Co chcesz zmienic w swoim koncie?\n";
			cout << "[1] Login\n";
			cout << "[2] Maila\n";
			cout << "[3] Haslo\n";
			cout << "[0] Anuluj\n";
			cout << "Twoj wybor: ";
			x = 1;
			cin >> n;

			if (n == "1")//login
			{
				system("cls");
				cout << "Twoj obecny login: " << getLogin();

				while (x == 1)
				{
					x = 0;
					cout << "\nWprowadz nowy login: ";
					cin >> temp;
					for (int j = 0; j < content.size(); ++j)
					{
						if (content[j][LOGIN] == temp)
						{
							cout << "Taki login juz istnieje" << endl;
							x = 1;
							break;
						}
					}
				}
				x = 1;
				while (x == 1) 
				{
					cout << "\nCzy jestes pewien zmiany loginu? t/n" << endl;
					cin >> pewnosc;
					if (pewnosc == "t") 
					{
						setLogin(temp);
						content[i][LOGIN] = temp;
						cout << "\nNowy Login to: " << getLogin() << endl;
						system("pause");
						x = 0;
						system("cls");
					}
					else if (pewnosc == "n") 
					{
						cout << "Anulowano zmiane loginu" << endl;
						system("pause");
						x = 0;
					}
					else 
					{
						cout << "Podano bledna odpowiedz, wprowadz t (tak) lub n (nie)" << endl;
						system("pause");
					}
				}
			}
			else if (n == "2")
			{
				system("cls");
				cout << "Twoj obecny mail: " << getMail();

				while (x == 1)
				{
					x = 0;
					cout << "\nWprowadz nowy mail: ";
					cin >> temp;
					for (int j = 0; j < content.size(); ++j)
					{
						if (content[j][MAIL] == temp)
						{
							cout << "Taki mail juz istnieje" << endl;
							x = 1;
							break;
						}
					}
				}
				x = 1;
				while (x == 1) {
					cout << "\nCzy jestes pewien zmiany maila? t/n" << endl;
					cin >> pewnosc;
					if (pewnosc == "t") {
						setMail(temp);
						content[i][MAIL] = temp;
						cout << "\nNowy Mail to: " << getMail() << endl;
						system("pause");
						system("cls");
						x = 0;
					}
					else if (pewnosc == "n") {
						cout << "Anulowano zmiane maila" << endl;
						system("pause");
						x = 0;
					}
					else {
						cout << "Podano bledna odpowiedz, wprowadz t (tak) lub n (nie)" << endl;
						system("pause");
					}
				}
			}
			else if (n == "3")
			{
				system("cls");

				while (x == 1)
				{
					x = 0;
					cout << "Wprowadz jeszcze raz poprawne haslo: ";
					cin >> temp;
					if (haslo != temp)
					{
						cout << "Bledne haslo" << endl << endl;
						x = 1;
					}


				}
				x = 1;
				system("cls");
				while (x == 1)
				{
					x = 0;
					cout << "Wprowadz nowe haslo: ";
					cin >> temp;
					if (temp.length() < 8)
					{
						cout << "Zbyt ktotkie haslo!" << endl;
						x = 1;
					}
				}
				x = 1;
				while (x == 1) {
					cout << "\nCzy jestes pewien zmiany hasla? t/n" << endl;
					cin >> pewnosc;
					if (pewnosc == "t") {
						for (int j = 0; j <= temp.length(); j++) {
							if (int(temp[j]) + PRZESUNIECIE > 122)
							{
								temp[j] = int(temp[j]) - 75 + PRZESUNIECIE;
							}
							else
							{
								temp[j] = int(temp[j]) + PRZESUNIECIE;
							}
						}
						setHaslo(temp);
						content[i][HASLO] = temp;
						cout << "\nNowe haslo zostalo ustawione" << endl;
						system("pause");
						system("cls");
						x = 0;
					}
					else if (pewnosc == "n") {
						cout << "Anulowano zmiane maila" << endl;
						system("pause");
						x = 0;
					}
					else {
						cout << "Podano bledna odpowiedz, wprowadz t (tak) lub n (nie)" << endl;
						system("pause");
					}
				}
			}
			else if(n == "0")
			{
				system("cls");
			}
			else
			{
				cout << "Wprowadzono znak spoza zakresu" << endl;
			}

			fstream filezapis("dane.csv", ios::out);
			for (int i = 0; i < content.size(); ++i)
			{
				filezapis << content[i][LOGIN] << "," << content[i][HASLO] << "," << content[i][IMIE] << "," << content[i][NAZWISKO] << "," << content[i][MAIL] << "\n";
			}
			filezapis.close();
		}
	}
};

class ZakupBiletu {
private:
	int ilosc;
	float kwota;
	string nazwaFilmu;
	int numerSali;
	int numerRzedu;
	int numerMiejsca;

public:
	void WyswietlRepertuar(string login) {
		vector<vector<string>> content = WczytajDane("repertuar.csv");
		vector<vector<string>> filmy = WczytajDane("baza_filmow.csv");
		string nazwa = "koszyk_" + login + ".csv";
		vector<vector<string>> koszyk = WczytajDane(nazwa);
		vector<string> row;
		string line, word, wybor;
		int licznik = 1;
		float oceny = 0, ilosc_ocen = 0;
		int wolne = 0, zajete = 0;

		
		for (int i = 0; i < content.size(); i = i + 6)
		{
			system("cls");
			wolne = 0;
			zajete = 0;
			oceny = 0;
			ilosc_ocen = 0;
			nazwaFilmu = content[i][0];
			numerSali = stoi(content[i + 3][0]);
			kwota = stof(content[i + 4][0]);
			cout << licznik << "." << nazwaFilmu << endl;
			cout << "Data: " << content[i + 1][0] << endl;
			cout << "Godzina seansu: " << content[i + 2][0] << endl;
			cout << "Sala: " << numerSali << endl ;
			cout << "Cena: " << kwota << " za miejsce" << endl;
			for (int j = 0; j < filmy.size(); j = j + 3)
			{
				if (content[i][0] == filmy[j][0])
				{
					for (int k = 2; k < filmy[j + 1].size(); k = k + 2)
					{
						oceny = oceny + stoi(filmy[j + 1][k]);
						ilosc_ocen++;
					}
					break;
				}
			}
			if (ilosc_ocen == 0)
			{
				cout << "Srednia ocen uzytkownikow: Nikt jeszcze nie ocenil tego filmu" << endl << endl;
			}
			else
			{
				oceny = oceny / ilosc_ocen;
				cout << "Srednia ocen uzytkownikow: " << oceny << "/10" << endl << endl;
			}
			cout << "Miejsca (0 - wolne, 1 - zajete):" << endl;
			for (numerRzedu = 0; numerRzedu < numerSali+5; numerRzedu++)
			{
				for (int y = 0; y < numerSali+5; y++)
				{
					numerMiejsca = y + ((numerSali + 5) * numerRzedu);
					cout << content[i + 5][numerMiejsca];
					if (content[i + 5][numerMiejsca] == "1")
					{
						zajete++;
					}
					else
					{
						wolne++;
					}
				}
				cout << endl;
			}
			cout << "\nLiczba miejsc zajetych: " << zajete << endl << "Liczba miejsc wolnych: " << wolne << endl << endl;
			cout << "Komentarze: " << endl;
			int k;
			for (int j = 0; j < filmy.size(); j = j + 3)
			{
				if (content[i][0] == filmy[j][0])
				{
					for (k = 2; k < filmy[j + 2].size(); k = k + 2)
					{
						cout << filmy[j + 2][k - 1] << ":" << endl; // autor wpisu
						cout << filmy[j + 2][k] << endl << endl; //komentarz
					}
					if (k == 2)
					{
						cout << "Nie zamieszczono jeszcze zadnych komentarzy do tego filmu" << endl << endl;
					}
					break;
				}
			}
			licznik++;
			system("pause");
		}
		system("cls");
		cout << "Wybierz numer od 1 do " << licznik - 1 << " aby wybrac film lub 0 jesli chcesz anulowac" << endl << endl;
		cout << "Twoj wybor:";
		getline(cin, wybor);
		getline(cin, wybor);

		if (wybor == "0" || wybor == "")
		{
			system("cls");
			cout << "Powrocono do glownego menu" << endl;
			return;
		}

		bool czy_wprzedziale = false;
		for (int i = 0; i < licznik; i++)
		{
			if (wybor == to_string(i))
			{
				czy_wprzedziale = true;
			}
		}

		if (czy_wprzedziale)
		{
			for (int i = 1; i < licznik; i++)
			{
				if (stoi(wybor) == i)
				{
					zajete = 0;
					wolne = 0;
					for (numerRzedu = 0; numerRzedu < stoi(content[3 + ((i - 1) * 6)][0]) + 5; numerRzedu++)
					{
						for (numerMiejsca = 0; numerMiejsca < stoi(content[3 + ((i - 1) * 6)][0]) + 5; numerMiejsca++)
						{
							if (content[5 + (i - 1) * 6][(stoi(content[3 + ((i - 1) * 6)][0]) + 5) * numerRzedu + numerMiejsca] == "1")
							{
								zajete++;
							}
							else
							{
								wolne++;
							}
						}
					}
					system("cls");
					cout << "Liczba miejsc zajetych dla wybranego filmu: " << zajete << endl << "Liczba miejsc wolnych: " << wolne << endl;
					if (wolne == 0)
					{
						cout << "Nie ma wolnych miejsc na seans" << endl;
					}
					else
					{
						cout << "\nIle biletow chcesz kupic?\nMiejsca zostana od razu zarezerwowane jednak musisz dokonczyc transakcje w koszyku" << endl;
						cout << "Twoj wybor: ";
						cin >> ilosc;
						if (ilosc > 0)
						{
							if (ilosc > wolne)
							{
								cout << "Wybrano zbyt duzo biletow na zbyt mala ilosc wolnych miejsc" << endl;
							}
							else
							{
								for (int j = 0; j < ilosc; j++)
								{
									bool x = false;
									while (x == false)
									{
										cout << "\nBilet nr " << j + 1 << endl;
										cout << "Podaj numer rzedu (od 0 do " << stoi(content[3 + ((i - 1) * 6)][0]) + 4 << " wlacznie): ";
										cin >> numerRzedu;
										cout << "Podaj numer miejsca (od 0 do " << stoi(content[3 + ((i - 1) * 6)][0]) + 4 << " wlacznie): ";
										cin >> numerMiejsca;

										if (content[5 + (i - 1) * 6][(stoi(content[3 + ((i - 1) * 6)][0]) + 5) * numerRzedu + numerMiejsca] == "0")
										{
											x = true;
											content[5 + (i - 1) * 6][(stoi(content[3 + ((i - 1) * 6)][0]) + 5) * numerRzedu + numerMiejsca] = "1";
										}
										else
										{
											cout << "To miejsce jest zajete, sprobuj inne" << endl;
										}

									}
								}

								fstream filezapisrepertuar("repertuar.csv", ios::out);
								for (int p = 0; p < content.size(); p = p + 6)
								{
									filezapisrepertuar << content[p][0] << "\n";
									filezapisrepertuar << content[p + 1][0] << "\n";
									filezapisrepertuar << content[p + 2][0] << "\n";
									filezapisrepertuar << content[p + 3][0] << "\n";
									filezapisrepertuar << content[p + 4][0] << "\n";
									for (int o = 0; o < content[p + 5].size(); ++o)
									{
										filezapisrepertuar << content[p + 5][o] << ",";
									}
									filezapisrepertuar << "\n";
								}
								filezapisrepertuar.close();

								string nazwawkoszyku = content[(i - 1) * 6][0] + "-" + content[1 + (i - 1) * 6][0] + "-" + content[2 + (i - 1) * 6][0];

								for (int t = 0; t < koszyk.size(); t++)
								{
									if (nazwawkoszyku == koszyk[t][0])
									{
										koszyk[t][1] = to_string(stoi(koszyk[t][1]) + ilosc);
										kwota = stof(content[4 + (i - 1) * 6][0]) * ilosc + stof(koszyk[t][2]);
										kwota = round(kwota * 100) / 100.0;

										fstream filezapis(nazwa, ios::out);
										for (int y = 0; y < koszyk.size(); y++)
										{
											if (t == y)
											{
												filezapis << koszyk[y][0] << "," << koszyk[y][1] << "," << kwota << "\n";
											}
											else
											{
												filezapis << koszyk[y][0] << "," << koszyk[y][1] << "," << koszyk[y][2] << "\n";
											}
										}
										filezapis.close();
										system("cls");
										cout << "Pomyslnie zarezerwowano bilety" << endl;
										system("cls");
										
										return;
									}
								}

								fstream filezapis(nazwa, ios::out);
								for (int q = 0; q < koszyk.size(); ++q)
								{
									filezapis << koszyk[q][0] << "," << koszyk[q][1] << "," << koszyk[q][2] << "\n";
								}
								filezapis << nazwawkoszyku << "," << ilosc << "," << stof(content[4 + (i - 1) * 6][0]) * ilosc << "\n";
								filezapis.close();


								cout << "\nPomyslnie zarezerwowano bilety" << endl;
							}

						}
					}

				}
			}
		}
		

	}
};

class PlatnoscOnline {
	float kwota;
	string sposobZaplaty;
public:
	void WyswietlenieParagonu(string login)
	{
		string plik = "koszyk_" + login + ".csv";
		vector<vector<string>> koszyk = WczytajDane(plik);
		vector<vector<string>> dane = WczytajDane("dane.csv");
		string email,tresc="";
		for (int i = 0; i < dane.size(); i++)
		{
			if (dane[i][0] == login)
			{
				email = dane[i][MAIL];
				break;
			}
		}

		string sposobInternetowy;
		
		kwota = 0;

		system("cls");

		for (int i = 0; i < koszyk.size(); i++)
		{
			kwota = kwota + stof(koszyk[i][2]);
		}


		cout << "1.Karta kredytowa\n2.PayPal" << endl;
		cin >> sposobInternetowy;

		if (sposobInternetowy == "1")
		{
			srand(time(NULL));
			string numerKarty;
			cout << "Podaj numer karty (oczywiscie nieprawdziwy):" << endl;
			getline(cin, numerKarty);
			getline(cin, numerKarty);
			bool x = false;
			while (x == false)
			{
				string tak;
				cout << kwota << "zl zostanie pobrane z twojego konta (t/n)" << endl;
				cin >> tak;
				if (tak == "t")
				{
					int numerZamowienia = rand() % 1000000000 + 1000000000;
					cout << "Transakcja zakonczona pomyslne, twoj kod do odebrania zakupionych rzeczy \nw kasie zostal wyslany na twojego maila: " << email << endl;
					tresc = "Witaj, tajny numer twojego zamowienia to " + to_string(numerZamowienia) + ".\nUzyj go przy kasie zeby odebrac zamowienie oraz zeby wejsc na seans.\nDo zobaczenia w kinie kinomaniaku!";
					wyslijMaila(email, tresc);
					remove(plik.c_str());
					x = true;
				}
				else if (tak == "n")
				{
					cout << "Anulowano transakcje" << endl;
					x = true;
				}
				else
				{
					cout << "Wpisano zly znak!" << endl;
				}
				
			}
		}
		else if (sposobInternetowy == "2")
		{
			srand(time(NULL));
			string paypal;
			cout << "Podaj email z uslugi PayPal (oczywiscie nieprawdziwy):" << endl;
			cin >> paypal;
			bool x = false;
			while (x == false)
			{
				string tak;
				cout << kwota << "zl zostanie pobrane z twojego konta (t/n)" << endl;
				cin >> tak;
				if (tak == "t")
				{
					int numerZamowienia = rand() % 1000000000 + 1000000000;
					cout << "Transakcja zakonczona pomyslne, twoj kod do odebrania zakupionych rzeczy \nw kasie zostal wyslany na twojego maila: " << email << endl;
					tresc = "Witaj, tajny numer twojego zamowienia to " + to_string(numerZamowienia) + ".\nUzyj go przy kasie zeby odebrac zamowienie oraz zeby wejsc na seans.\nDo zobaczenia w kinie kinomaniaku!";
					wyslijMaila(email, tresc);
					remove(plik.c_str());
					x = true;
				}
				else if (tak == "n")
				{
					cout << "Anulowano transakcje" << endl;
					x = true;
				}
				else
				{
					cout << "Wpisano zly znak!" << endl;
				}

			}

			remove(plik.c_str());
		}
		else
		{
			cout << "Wprowadzono zly znak!" << endl;
		}

		system("pause");
		system("cls");

	}
};

class PlatnoscNaMiejscu {
	float kwota;
	string sposobZaplaty;
public:
	void WyswietlenieParagonu(string login)
	{
		string plik = "koszyk_" + login + ".csv";
		string tresc = "";
		string email;
		vector<vector<string>> koszyk = WczytajDane(plik);
		vector<vector<string>> dane = WczytajDane("dane.csv");
		int numerZamowienia;
		srand(time(NULL));
		kwota = 0;

		system("cls");

		for (int i = 0; i < dane.size(); i++)
		{
			if (dane[i][0] == login)
			{
				email = dane[i][MAIL];
				break;
			}
		}

		numerZamowienia = rand() % 1000000000 + 1000000000;
		cout << "Twoj numer zamowienia to '" << numerZamowienia << "'.\nPodejdz z nim do kasy aby oplacic swoje zamowienie" << endl;
		tresc = "Witaj, tajny numer twojego zamowienia to " + to_string(numerZamowienia) + ".\nUzyj go przy kasie zeby odebrac zamowienie oraz zeby wejsc na seans.\nDo zobaczenia w kinie kinomaniaku!";
		wyslijMaila(email, tresc);
		remove(plik.c_str());
		system("pause");
		system("cls");
	}
};

class Platnosc {
private:
	string kwota;
	string sposobZaplaty;
	//int numerKlienta;
	//string hasloBank;

public:
	void RealizacjaPlatnosci(string login) 
	{
		string plik = "koszyk_" + login + ".csv",doUsuniecia;
		int licznik = 1;
		fstream fileodczyt(plik, ios::in);
		system("cls");
		if (!fileodczyt.is_open())
		{
			cout << "Nie masz nic w koszyku!" << endl;
			return;
		}
		fileodczyt.close();
		
		vector<vector<string>> koszyk = WczytajDane(plik);

		float suma = 0;
		cout << "Twoj koszyk: " << endl;
		for (int i = 0; i < koszyk.size(); i++)
		{
			cout << i + 1 << "." << koszyk[i][0] << "\n" << "Ilosc: " << koszyk[i][1] << "\n" << "Cena: " << koszyk[i][2] << "zl" << endl << endl;
			suma = suma + stof(koszyk[i][2]);
		}
		cout << "Laczna cena: " << suma << "zl" << endl << endl;
		
		cout << "Mozliwe operacje:" << endl << "1.Zaplata przy kasie\n2.Zaplata online\n3.Usun cos z koszyka\n0.Anuluj" << endl << endl;
		cout << "Twoj wybor:";
		cin >> sposobZaplaty;

		if (sposobZaplaty == "1")
		{
			PlatnoscNaMiejscu na_miejscu;
			na_miejscu.WyswietlenieParagonu(login);
		}
		else if(sposobZaplaty == "2")
		{
			PlatnoscOnline online;
			online.WyswietlenieParagonu(login);
		}
		else if (sposobZaplaty == "3")
		{
			system("cls");
			licznik = 1;
			for (int i = 0; i < koszyk.size(); i++)
			{
				cout << i + 1 << "." << koszyk[i][0] << "\n" << "Ilosc: " << koszyk[i][1] << "\n" << "Cena: " << koszyk[i][2] << "zl" << endl << endl;
				suma = suma + stof(koszyk[i][2]);
				licznik++;
			}
			cout << "Ktory przedmiot chesz usunac? (od 1 do " << licznik - 1 << "):" << endl;
			cout << "Twoj wybor: ";
			cin >> doUsuniecia;
			for (int i = 0; i < licznik; i++)
			{
				if (to_string(i) == doUsuniecia)
				{
					koszyk.erase(koszyk.begin() + i - 1);

					if (koszyk.size() == 0)
					{
						remove(plik.c_str());
					}
					else
					{
						fstream filezapis(plik, ios::out);
						for (int j = 0; j < koszyk.size(); ++j)
						{
							filezapis << koszyk[j][0] << "," << koszyk[j][1] << "," << koszyk[j][2] << "\n";
						}
						filezapis.close();
					}

					
					system("cls");
					cout << "Pomyslnie usunieto przedmiot z twojego koszyka" << endl << endl;
					return;
				}
			}
			system("cls");
			cout << "Wpisano liczbe spoza zakresu!" << endl << endl;
		}
		else if (sposobZaplaty == "0")
		{
			system("cls");
			return;
		}
		else
		{
			cout << "Wprowadzono zly znak!" << endl;
		}
	} 
};

class ZamawianieJedzenia {
private:
	string nazwaProduktu;
	int iloscProduktu;
	float kwota;
public:
	void WyswietlMenu() {
		vector<vector<string>> jedzenie = WczytajDane("jedzenie.csv");
		int licznik;

		system("cls");
		for (int i = 0, licznik= 1; i < jedzenie.size(); ++i, licznik++)
		{
			if (jedzenie[i][0].length() > 13)
			{
				cout << licznik << "." << jedzenie[i][0] << "\t" << "Cena: " << jedzenie[i][1] << endl;
			}
			else
			{
				cout << licznik << "." << jedzenie[i][0] << "\t\t" << "Cena: " << jedzenie[i][1] << endl;
			}
			
		}
		cout << "0.Anuluj" << endl << endl;
	}

	void WyborZamowienia(string numerProduktu, int iloscProduktu, string login) 
	{
		vector<vector<string>> jedzenie = WczytajDane("jedzenie.csv");
		string plik = "koszyk_" + login + ".csv";
		int numer = stoi(numerProduktu) - 1;
		string wybor;
		nazwaProduktu = jedzenie[numer][0];
		kwota = stof(jedzenie[numer][1]) * iloscProduktu;


		cout << nazwaProduktu << ", czy potwierdzasz zakup za " << kwota << " zl? (t/n)" << endl;
		cin >> wybor;
		if (wybor == "t")
		{
			vector<vector<string>> content = WczytajDane(plik);

			for (int i = 0; i < content.size(); ++i)
			{
				if (nazwaProduktu == content[i][0])
				{
					system("cls");
					content[i][1] = to_string(stoi(content[i][1]) + iloscProduktu);
					kwota = kwota + stof(content[i][2]);
					kwota = round(kwota * 100) / 100.0;
					fstream filezapis(plik, ios::out);
					for (int j = 0; j < content.size(); ++j)
					{
						if (i == j)
						{
							filezapis << content[j][0] << "," << content[j][1] << "," << kwota << "\n";
						}
						else
						{
							filezapis << content[j][0] << "," << content[j][1] << "," << content[j][2] << "\n";
						}
					}
					system("cls");
					filezapis.close();
					cout << "Pomyslnie zakupiono " << nazwaProduktu << endl;
					return;
				}
			}

			fstream filezapis(plik, ios::out);
			for (int i = 0; i < content.size(); ++i)
			{
				filezapis << content[i][0] << "," << content[i][1] << "," << content[i][2] << "\n";
			}
			filezapis << nazwaProduktu << "," << iloscProduktu << "," << kwota << "\n";

			filezapis.close();
		}
		else if (wybor == "n")
		{
			system("cls");
			cout << "Anulowano zamowienie" << endl;
			return;
		}
		else
		{
			system("cls");
			cout << "Wprowadzono zly znak, sprobuj od nowa" << endl;
		}
		
	}
};



class Repertuar {
	string baza;//string? czy podpinamy jakąś małą bazę choćby z pliku? w najgorszym przypadku chyba plik csv
public:
	void PrzegladanieFilmow() 
	{
		baza = "repertuar.csv";
		vector<vector<string>> content = WczytajDane(baza);
		vector<vector<string>> filmy = WczytajDane("baza_filmow.csv");
		float oceny = 0, ilosc_ocen = 0;
		int numerSali, numerMiejsca, licznik = 1, zajete, wolne;

		for (int i = 0; i < content.size(); i = i + 6)
		{
			system("cls");
			zajete = 0;
			wolne = 0;
			oceny = 0;
			ilosc_ocen = 0;
			numerSali = stoi(content[i + 3][0]);
			cout << licznik << "." << content[i][0] << endl;
			cout << "Data: " << content[i + 1][0] << endl;
			cout << "Godzina seansu: " << content[i + 2][0] << endl;
			cout << "Sala: " << content[i + 3][0] << endl;
			cout << "Cena: " << content[i + 4][0] << " za miejsce" << endl;
			for (int j = 0; j < filmy.size(); j = j + 3)
			{
				if (content[i][0] == filmy[j][0])
				{
					for (int k = 2; k < filmy[j + 1].size(); k = k + 2)
					{
						oceny = oceny + stoi(filmy[j + 1][k]);
						ilosc_ocen++;
					}
					break;
				}
			}
			if (ilosc_ocen == 0)
			{
				cout << "Srednia ocen uzytkownikow: Nikt jeszcze nie ocenil tego filmu" << endl << endl;
			}
			else
			{
				oceny = oceny / ilosc_ocen;
				cout << "Srednia ocen uzytkownikow: " << oceny << "/10" << endl << endl;
			}
			cout << "Miejsca (0 - wolne, 1 - zajete):" << endl;
			for (int numerRzedu = 0; numerRzedu < numerSali + 5; numerRzedu++)
			{
				for (int y = 0; y < numerSali + 5; y++)
				{
					numerMiejsca = y + ((numerSali + 5) * numerRzedu);
					cout << content[i + 5][numerMiejsca];
					if (content[i + 5][numerMiejsca] == "1")
					{
						zajete++;
					}
					else
					{
						wolne++;
					}
				}
				cout << endl;
			}
			cout << "\nLiczba miejsc zajetych: " << zajete << endl << "Liczba miejsc wolnych: " << wolne << endl << endl;
			cout << "Komentarze: " << endl;
			int k;
			for (int j = 0; j < filmy.size(); j = j + 3)
			{
				if (content[i][0] == filmy[j][0])
				{
					for (k = 2; k < filmy[j + 2].size(); k = k + 2)
					{
						cout << filmy[j + 2][k - 1] <<":" << endl; // autor wpisu
						cout << filmy[j + 2][k] << endl << endl; //komentarz
					}
					if (k == 2)
					{
						cout << "Nie zamieszczono jeszcze zadnych komentarzy do tego filmu" << endl << endl;
					}
					break;
				}
			}
			licznik++;
			system("pause");
		}
		system("cls");
	}

	void OcenaFilmow(string login) 
	{
		baza = "baza_filmow.csv";
		vector<vector<string>> content = WczytajDane(baza);
		int licznik = 1;
		string komentarz,ocena,wybor,film;
		bool x = false;
		bool pierwszy_raz = true;
		
		system("cls");
		cout << "Co chcesz zamiescic?" << endl << "1.Ocene" << endl << "2.Komentarz"<< endl << "3.Edycja oceny" << endl << "4.Edycja komentarza" << endl << endl;
		cout << "Twoj wybor: ";
		cin >> wybor;

		system("cls");

		if (wybor == "1")
		{
			licznik = 1;
			while (x == false)
			{
				
				licznik = 1;
				cout << "Ktory film chcesz ocenic?" << endl;
				
				for (int i = 0; i < content.size(); i = i + 3)
				{
					cout << licznik << "." << content[i][0] << endl;
					licznik++;
				}
				cout << "\nTwoj wybor: ";
				cin >> film;

				bool czy_wprzedziale = false;
				for (int i = 0; i < licznik; i++)
				{
					if (film == to_string(i))
					{
						czy_wprzedziale = true;
					}
				}

				if (czy_wprzedziale)
				{
					if (stoi(film) > 0 && stoi(film) < licznik)
					{
						while (x == false)
						{
							for (int i = 1; i < content[((stoi(film) - 1) * 3) + 1].size(); i = i + 2)
							{
								if (content[((stoi(film) - 1) * 3) + 1][i] == login)
								{
									system("cls");
									cout << "Ten film zostal juz przez ciebie oceniony" << endl;
									return;
								}
							}
							cout << "Jaka ocene (od 0 do 10) dajesz temu filmowi?" << endl;
							cout << "Twoj wybor: ";
							cin >> ocena;

							czy_wprzedziale = false;
							for (int i = 0; i <= 10; i++)
							{
								if (ocena == to_string(i))
								{
									czy_wprzedziale = true;
								}
							}
							if (czy_wprzedziale)
							{
								if (stoi(ocena) >= 0 && stoi(ocena) <= 10)
								{
									content[((stoi(film) - 1) * 3) + 1].push_back(login);
									content[((stoi(film) - 1) * 3) + 1].push_back(ocena);
									system("cls");
									cout << "Nadano temu filmowi ocene " << ocena << endl;
									x = true;
								}
								
							}
							else
							{
								system("cls");
								cout << "Zla ocena! Podaj jeszcze raz" << endl;
							}
							
						}

					}
				}
				else
				{
					system("cls");
					cout << "Wybrany film jest spoza zakresu!" << endl;
				}
				
				
			}
		}
		else if (wybor == "2")
		{
			pierwszy_raz = true;
			while (x == false)
			{
				licznik = 1;
				cout << "Ktory film chcesz skomentowac?" << endl;
				
				for (int i = 0; i < content.size(); i = i + 3)
				{
					cout << licznik << "." << content[i][0] << endl;
					licznik++;
				}
				cout << "\nTwoj wybor: ";
				cin >> film;

				bool czy_wprzedziale = false;
				for (int i = 0; i < licznik; i++)
				{
					if (film == to_string(i))
					{
						czy_wprzedziale = true;
					}
				}

				if (czy_wprzedziale)
				{
					for (int i = 1; i < content[((stoi(film) - 1) * 3) + 2].size(); i = i + 2)
					{
						if (content[((stoi(film) - 1) * 3) + 2][i] == login)
						{
							system("cls");
							cout << "Ten film zostal juz przez ciebie skomentowany, jezeli \nchcesz edytowac komentarz, wybierz 'Edycja komentarza'" << endl;
							return;
						}
					}

					if (stoi(film) > 0 && stoi(film) < licznik)
					{
						x = false;
						while (x == false)
						{
							x = true;
							cout << "Wstaw komentarz dla tego filmu: " << endl;
							if (pierwszy_raz)
							{
								getline(cin, komentarz);
							}
							getline(cin, komentarz);

							for (int i = 0; i < komentarz.length(); i++)
							{
								if (komentarz[i] == ',')
								{
									cout << "Zakazany znak (','). Prosze napisac komentarz jeszcze raz" << endl << endl;
									pierwszy_raz = false;
									x = false;
									break;
								}
							}

						}
						content[((stoi(film) - 1) * 3) + 2].push_back(login);
						content[((stoi(film) - 1) * 3) + 2].push_back(komentarz);

					}
				}
				else
				{
					system("cls");
					cout << "Wybrany film jest spoza zakresu!" << endl;
				}
			}
		}
		else if (wybor == "3")
		{
			vector<string> ocenione;

			while (x == false)
			{
				system("cls");
				ocenione.clear();
				licznik = 1;
				
				for (int i = 0; i < content.size(); i = i + 3)
				{
					for (int j = 1; j < content[i + 1].size(); j = j + 2)
					{
						if (content[i + 1][j] == login)
						{
							cout << licznik << "." << content[i][0] << "\nPoprzednio ocenione na " << content[i + 1][j + 1] << endl << endl;
							ocenione.push_back(content[i][0]);
							licznik++;
						}
					}
				}
				if (licznik == 1)
				{
					cout << "Nie oceniles jeszcze zadnego filmu" << endl;
					return;
				}
				else
				{
					cout << "Dla ktorego filmu chcesz zmienic ocene?" << endl;
					cout << "Twoj wybor: ";
				}

				cin >> film;
				bool czy_wprzedziale = false;
				for (int i = 0; i <= 10; i++)
				{
					if (film == to_string(i))
					{
						czy_wprzedziale = true;
					}
				}

				if (czy_wprzedziale)
				{
					if (stoi(film) > 0 && stoi(film) < licznik)
					{
						cout << "\nPodaj nowa ocene dla tego filmu" << endl;
						while (x == false)
						{
							cout << "Twoj wybor: ";
							cin >> ocena;
							czy_wprzedziale = false;
							for (int i = 0; i <= 10; i++)
							{
								if (ocena == to_string(i))
								{
									czy_wprzedziale = true;
								}
							}

							if (czy_wprzedziale)
							{
								for (int i = 0; i < content.size(); i = i + 3)
								{
									if (content[i][0] == ocenione[stoi(film) - 1])
									{
										for (int j = 1; j < content[i + 1].size(); j = j + 2)
										{
											if (content[i + 1][j] == login)
											{
												system("cls");
												cout << "Twoja poprzednia ocena " << content[i + 1][j + 1] << " zostala zmieniona na " << ocena << endl;
												content[i + 1][j + 1] = ocena;
												x = true;
											}
										}
									}
								}
							}
							else
							{
								cout << "\nZla ocena! Podaj jeszcze raz (od 0 do 10): " << endl;
							}

						}
					}
				}
				else
				{
					cout << "Wybrany film jest spoza zakresu!" << endl;
				}
				
			}

		}
		else if (wybor == "4")
		{
			pierwszy_raz = true;
			vector<string> ocenione;
			
			while (x == false)
			{
				
				ocenione.clear();
				licznik = 1;
				
				for (int i = 0; i < content.size(); i = i + 3)
				{
					for (int j = 1; j < content[i + 2].size(); j = j + 2)
					{
						if (content[i + 2][j] == login)
						{
							cout << licznik << "." << content[i][0] << "\nPoprzedni komentarz to: '" << content[i + 2][j + 1] << "'" << endl << endl;
							ocenione.push_back(content[i][0]);
							licznik++;
						}
					}
				}

				if (licznik == 1)
				{
					cout << "Nie skomentowales jeszcze zadnego filmu" << endl;
					return;
				}
				else
				{
					cout << "Dla ktorego filmu chcesz zmienic komentarz?" << endl;
					cout << "Twoj wybor: ";
				}

				cin >> film;
				bool czy_wprzedziale = false;
				for (int i = 0; i < licznik; i++)
				{
					if (film == to_string(i))
					{
						czy_wprzedziale = true;
					}
				}
				
				if (czy_wprzedziale)
				{
					if (stoi(film) > 0 && stoi(film) < licznik)
					{
						while (x == false)
						{
							while (x == false)
							{
								x = true;
								cout << "\nWstaw komentarz dla tego filmu: " << endl;
								if (pierwszy_raz)
								{
									getline(cin, komentarz);
								}
								getline(cin, komentarz);
								for (int i = 0; i < komentarz.length(); i++)
								{
									if (komentarz[i] == ',')
									{
										cout << "Zakazany znak (','). Prosze napisac komentarz jeszcze raz" << endl << endl;
										pierwszy_raz = false;
										x = false;
										break;
									}
								}
							}


							for (int i = 0; i < content.size(); i = i + 3)
							{
								if (content[i][0] == ocenione[stoi(film) - 1])
								{
									for (int j = 1; j < content[i + 2].size(); j = j + 2)
									{
										if (content[i + 2][j] == login)
										{
											system("cls");
											cout << "Twoj komentarz zostal zmieniony" << endl;
											content[i + 2][j + 1] = komentarz;
											x = true;
										}
									}
								}
							}
						}
					}
				}
				else
				{
					system("cls");
					cout << "Wybrany film jest spoza zakresu!" << endl;
				}
				
			}
		}
		else
		{
			cout << "Liczba spoza zakresu!" << endl;
		}

		fstream filezapis(baza, ios::out);
		for (int i = 0; i < content.size(); i=i+3)
		{
			filezapis << content[i][0] << "\n";
			for (int j = 0; j < content[i + 1].size(); j++)
			{
				filezapis << content[i + 1][j] << ",";
			}
			filezapis << "\n";
			for (int j = 0; j < content[i + 2].size(); j++)
			{
				filezapis << content[i + 2][j] << ",";
			}
			filezapis << "\n";

		}
		filezapis.close();

	}
};


int main()
{
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
	string login, haslo, zaszyfrowanehaslo;
	bool poprawne = false;
	string wybor;

	while (true)
	{
		poczatek:
		content = WczytajDane("dane.csv");
		bool x = false, y = false;

		Logowanie uzytkownik;
		ZakupBiletu bilet;
		ZamawianieJedzenia jedzenie;
		PlatnoscOnline online;
		Repertuar kino;
		Platnosc placenie;

		cout << "Witaj w kinomaniaku! Co dzis obejrzymy?" << endl;
		cout << "1.Logowanie" << endl << "2.Przypomnienie hasla" << endl << "3.Utworzenie konta" << endl << endl;
		cout << "Twoj wybor: ";
		cin >> wybor;

		if (wybor == "1")
		{
			//logowanie
			
			while (x == false)
			{
				x = true;
				system("cls");
				cout << "Login: ";
				getline(cin, login);
				if (y == false)
				{
					getline(cin, login);
				}


				for (int i = 0; i < login.length(); i++)
				{
					if (login[i] == ' ')
					{
						system("cls");
						cout << "Bledny login!" << endl;
						x = false;
						y = true;
						break;
					}
				}
			}


			for (int i = 0; i < content.size(); ++i)
			{
				if (content[i][LOGIN] == login)
				{
					x = false;
					y = false;
					poprawne = true;
					system("cls");

					while (x == false)
					{
						x = true;
						cout << "Podaj haslo: ";
						getline(cin, haslo);

						for (int i = 0; i < haslo.length(); i++)
						{
							if (haslo[i] == ' ')
							{
								system("cls");
								cout << "Bledne haslo!" << endl << endl;
								x = false;
								y = true;
								break;
							}
						}
					}

					zaszyfrowanehaslo = haslo;

					//mechanizm szyfrowania hasła
					for (int j = 0; j <= zaszyfrowanehaslo.length(); j++) {
						if (int(zaszyfrowanehaslo[j]) + PRZESUNIECIE > 122)
						{
							zaszyfrowanehaslo[j] = int(zaszyfrowanehaslo[j]) - 75 + PRZESUNIECIE;
						}
						else
						{
							zaszyfrowanehaslo[j] = int(zaszyfrowanehaslo[j]) + PRZESUNIECIE;
						}
					}

					if (content[i][HASLO] == zaszyfrowanehaslo)
					{
						Logowanie uzytkownik(login, haslo, content[i][IMIE], content[i][NAZWISKO], content[i][MAIL]);
						system("cls");
						while (1)
						{
							//i tutaj sie cos bedzie zamawiac itd itp
							string znak;

							cout << endl << "Co chcesz zrobic:" << endl;
							cout << "1.Edycja konta" << endl << "2.Wyswietl repertuar oraz kup bilety" << endl << "3.Pokaz repertuar bez kupienia biletu" << endl << "4.Wybor i zakup przekasek" << endl << "5.Pokaz koszyk" << endl << "6.Ocen lub skomentuj film" << endl << "7.Wyloguj sie" << endl << "8.Wyjdz z kinomaniaka" << endl << endl;
							cout << "Twoj wybor: ";
							cin >> znak;

							if (znak == "1")
							{
								uzytkownik.EdycjaKonta();
							}
							else if (znak == "2")
							{
								bilet.WyswietlRepertuar(login);
							}
							else if (znak == "3")
							{
								kino.PrzegladanieFilmow();
							}
							else if (znak == "4")
							{
								int ilosc;
								string numerProduktu;
								vector<vector<string>> spisJedzenia = WczytajDane("jedzenie.csv");

								jedzenie.WyswietlMenu();

								cout << "Podaj numer produktu: ";
								cin >> numerProduktu;

								if (stoi(numerProduktu) <= spisJedzenia.size() && stoi(numerProduktu) > 0)
								{
									cout << "Podaj ilosc: ";
									cin >> ilosc;
									jedzenie.WyborZamowienia(numerProduktu, ilosc, content[i][LOGIN]);
									system("cls");
								}
								else if (numerProduktu == "0")
								{
									system("cls");
								}
								else
								{
									system("cls");
									cout << "Wybrano produkt spoza zasiegu" << endl;
								}


							}
							else if (znak == "5") //koszyk
							{
								placenie.RealizacjaPlatnosci(login);
							}
							else if (znak == "6")
							{
								kino.OcenaFilmow(login);
							}
							else if (znak == "7")
							{
								system("cls");
								break;
							}
							else if (znak == "8")
							{
								exit(0);
							}
							else
							{
								system("cls");
								cout << "Znak spoza zakresu!";
							}
						}
					}
					else
					{
						system("cls");
						cout << "Bledne haslo" << endl << endl;
						break;
					}
				}
			}
			if (poprawne == false)
			{
				system("cls");
				cout << "Nie znaleziono loginu" << endl << endl;
			}
		}
		else if (wybor == "2")
		{
			//przypominanie hasla
			uzytkownik.PrzypomnienieHasla();
		}
		else if (wybor == "3")
		{
			int n = 0;
			string mail,imie,nazwisko;
			bool nieZawieraMalpy = true, nieZawieraKropki= true;
			
			system("cls");

			while (n == 0)
			{
				n = 1;
				cout << "Podaj unikalny login (zawsze mozesz wpisac 0 aby anulowac): ";
				cin >> login;
				if (login == "0")
				{
					system("cls");
					goto poczatek;
				}
				for (int i = 0; i < content.size(); ++i)
				{
					if (content[i][LOGIN] == login)
					{
						cout << "Taki login juz istnieje" << endl;
						n = 0;
					}
				}
			}
			n = 0;
			while (n == 0)
			{
				n = 1;
				cout << "Podaj haslo: ";
				cin >> haslo;
				if (haslo == "0")
				{
					system("cls");
					goto poczatek;
				}
				if (haslo.length() < 8)
				{
					cout << "Zbyt ktotkie haslo!" << endl;
					n = 0;
				}
			}
			n = 0;
			while (n == 0)
			{
				n = 1;
				cout << "Podaj mail: ";
				cin >> mail;

				if (mail == "0")
				{
					system("cls");
					goto poczatek;
				}

				for (int i = 0; i < mail.length(); i++)
				{
					if (mail[i] == '@')
					{
						nieZawieraMalpy = false;
					}
					else if (mail[i] == '.')
					{
						nieZawieraKropki = false;
					}
				}

				if (nieZawieraKropki == true && nieZawieraMalpy == true)
				{
					cout << "Podany mail nie zawiera malpy oraz kropki" << endl;
					n = 0;
				}
				else if (nieZawieraKropki == true)
				{
					cout << "Podany mail nie zawiera kropki" << endl;
					n = 0;
				}
				else if (nieZawieraMalpy == true)
				{
					cout << "Podany mail nie zawiera malpy" << endl;
					n = 0;
				}

				for (int i = 0; i < content.size(); ++i)
				{
					if (content[i][MAIL] == mail)
					{
						cout << "Taki mail juz istnieje" << endl;
						n = 0;
					}
				}
			}
			cout << "Podaj imie:";
			cin >> imie;
			if (imie == "0")
			{
				system("cls");
				goto poczatek;
			}
			
			cout << "Podaj nazwisko:";
			cin >> nazwisko;
			if (nazwisko == "0")
			{
				system("cls");
				goto poczatek;
			}
			
			uzytkownik.UtworzenieKonta(login,haslo,imie,nazwisko,mail);
		}
		else
		{
			system("cls");
			cout << "Liczba poza zakresem" << endl << endl;
		}
	}
}