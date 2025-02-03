#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include <tuple>
#include <sstream>

using namespace std;

struct Kontakt {
	string prezime, ime, broj;

	Kontakt* otac;
	Kontakt* levi, * desni;
};

Kontakt* formiranjeBST(string imeDat);
Kontakt* izmenaKontakta(Kontakt* koren, Kontakt* kont, string prezime, string ime, string broj);
Kontakt* pretragaImenikaPrez(Kontakt* koren, string prezime, string ime, bool IiP);
Kontakt* pretragaImenikaImePrez(Kontakt* koren, string prezime, string ime);
Kontakt* dodavanjeKontakt(Kontakt* koren, string prezime, string ime, string broj);
Kontakt* brisanjeKontakt(Kontakt*& koren, string prezime, string ime);
void ispisBST(Kontakt* koren);
void brisanjeBST(Kontakt*& koren);

int main()
{
	int izbor;
	string imeDat;
	Kontakt* koren = nullptr;

	while (true) {
		cout << "/nIzaberite jedna od sledecih opcija:\n";
		cout << "\t1) Formiranje stablo\n\t2) Pretraga po prezime\n\t3) Pretraga po ime i prezime\n\t4) Izmena kontakta\n";
		cout << "\t5) Dodavanje novog kontakta\n\t6) Uklanjanje kontakt\n\t7) Ispis stabla\n\t8) Brisanje stabla\n\t9) Izlaz iz programa\n";
		cin >> izbor;
		switch (izbor) {
		case 1:
		{
			cout << "Ime datoteke: ";
			cin >> imeDat;
			koren = formiranjeBST(imeDat);
			cout << endl;
			break;
		}
		case 2:
		{
			cout << "Unesi prezime za pretragu.\n";
			cout << "Prezime: ";
			string prezime;
			cin >> prezime;
			Kontakt* rez = pretragaImenikaPrez(koren, prezime, prezime, false);
			cout << endl;
			break;
		}
		case 3:
		{
			cout << "Unesi prezime i ime za pretragu.\n";
			string ime, prezime;
			cout << "Prezime: ";
			cin >> prezime;
			cout << "Ime: ";
			cin >> ime;
			cout << endl;
			Kontakt* rez = pretragaImenikaPrez(koren, prezime, ime, true); 
			cout << endl;
			break;
		}
		case 4:
		{
			int k;
			string prezime, ime, broj;
			cout << "Koji kontakt hocete da promenite.\n";
			cout << "Prezime: ";
			cin >> prezime;
			cout << "Ime: ";
			cin >> ime;
			Kontakt * kont = pretragaImenikaPrez(koren, prezime, ime, true);
			if (kont == nullptr)
				break;
			cout << "Koji su azurirane informacije?\n";
			cout << "Prezime: ";
			cin >> prezime;
			cout << "Ime: ";
			cin >> ime;
			cout << "Broj: ";
			cin >> broj;
			koren = izmenaKontakta(koren, kont, prezime, ime, broj);
			cout << endl;
			break;
		}
		case 5:
		{
			string prezime, ime, broj;
			cout << "Unesite informacije novog kontakta.\n";
			cout << "Prezime: ";
			cin >> prezime;
			cout << "Ime: ";
			cin >> ime;
			cout << "Broj: ";
			cin >> broj;
			koren = dodavanjeKontakt(koren, prezime, ime, broj);
			cout << endl;
			break;
		}
		case 6:
		{
			string prezime, ime;
			cout << "Koji cvor hocete da bude obrisan?\n";
			cout << "Prezime: ";
			cin >> prezime;
			cout << "Ime: ";
			cin >> ime;
			koren = brisanjeKontakt(koren, prezime, ime);
			cout << endl;
			break;
		}
		case 7:
		{
			ispisBST(koren);
			cout << endl;
			break;
		}
		case 8:
		{
			brisanjeBST(koren);
			cout << endl;
			break;
		}
		case 9:
		{
			if (koren != nullptr)
				brisanjeBST(koren);
			return 0;
		}
		}
	}

	return 0;
}

/*
* @brief Ucitava datoteka sa datom imenom imeDat i formira tuplovi koje cine delova stringa podeljeni zarezom
* Formira BST koristeci iterativan binaran prolazak pomocu steck-a
* 
* @param imeDat Ime datoteke koja je izvor kontakata
* 
* @return Pokazivac na korena formiranog stabla
*/
Kontakt* formiranjeBST(string imeDat)
{
	imeDat = imeDat + ".txt";
	ifstream dat(imeDat);

	if (!dat.is_open()) {
		cout << "Datoteka nije pronadjena.\n";
		return 0;
	}

	string linija;
	getline(dat, linija);

	string temp;

	vector<tuple<string, string, string>> linijeDat;
	while (getline(dat, linija)) {
		stringstream str(linija);
		string s1, s2, s3;
		if (getline(str, s1, ',') && getline(str, s2, ',') && getline(str, s3, ',')) {
			linijeDat.push_back(make_tuple(s1, s2, s3));
		}
	}
	if (linijeDat.empty())
		return nullptr;
	
	stack<tuple<Kontakt*, int, int>> s; //inicijalizacija stack-a koriscen za formiranje BST od sortirani niz	

	int low = 0, high = linijeDat.size() - 1, mid;
	mid = (low + high) / 2;

	Kontakt* koren = new Kontakt;

	koren->prezime = get<0>(linijeDat[mid]);
	koren->ime = get<1>(linijeDat[mid]);
	koren->broj = get<2>(linijeDat[mid]);

	koren->otac = nullptr;
	koren->levi = nullptr;
	koren->desni = nullptr;

	s.push(make_tuple(koren, mid + 1, high));
	s.push(make_tuple(koren, low, mid - 1));

	while (!s.empty()) {
		tuple<Kontakt*, int, int> tren = s.top();
		s.pop();

		Kontakt* otac = get<0>(tren);
		int low = get<1>(tren);
		int high = get<2>(tren);

		if (low > high)
			continue;

		mid = (low + high) / 2;

		Kontakt* novi = new Kontakt;

		novi->prezime = get<0>(linijeDat[mid]);
		novi->ime = get<1>(linijeDat[mid]);
		novi->broj = get<2>(linijeDat[mid]);

		novi->otac = otac;
		novi->levi = nullptr;
		novi->desni = nullptr;

		if (novi->prezime < otac->prezime)
			otac->levi = novi;
		else
			otac->desni = novi;

		s.push(make_tuple(novi, mid + 1, high)); 
		s.push(make_tuple(novi, low, mid - 1));
	}

	return koren;
}

/*
* @brief Pretrazuje stablo sa datim prezimenom pomocu binary search-a
* Proverava da li postoje vise kontakta sa tim prezimenom i ispisuje svi
* 
* @param koren Pokazivac na korena stabla
* @param prezime, ime Podaci o kontakta
* @param IiP koristi se da bi funkcija nastavila pretragu i po imenom ukoliko potrebno
* 
* @return rez Prva pronadjena instanca sa datim prezimenom
*/
Kontakt* pretragaImenikaPrez(Kontakt* koren, string prezime, string ime, bool IiP) {
	if (koren == nullptr) {
		cout << "Ne postoji formirano stablo.\n";
		return nullptr;
	}

	Kontakt* rez = koren;

	while (rez != nullptr && rez->prezime != prezime) {
		if (prezime > rez->prezime)
			rez = rez->desni;
		else
			rez = rez->levi;
	}

	if (rez == nullptr) {
		cout << "Kontakt sa tim prezimenom nije pronadjen.\n";
		return nullptr;
	}

	if (IiP) {
		rez = pretragaImenikaImePrez(rez, prezime, ime);
		return rez;
	}

	stack<Kontakt*> s;
	Kontakt* tren = rez;

	// Primena pseudokoda za iterativni inorder obilazak dat u "Algoritmi i strukture podataka", Milo Tomasevic, 2008, str 134
	while (tren != nullptr || !s.empty()) {
		while (tren != nullptr) {
			s.push(tren);
			tren = tren->levi;
		}
		tren = s.top();
		s.pop();

		if(tren->prezime == prezime)
			cout << tren->prezime << ", " << tren->ime << ", " << tren->broj << endl;

		tren = tren->desni;
	}

	return rez;
}

/*
* @brief Pretraga sa datim prezimenom i imenom
* F-ja je povikana od pretragaImenkaPrez ukoliko je IiP true
* 
* @param rez Prvi cvor sa nadjenim prezimenom od koga nastavlja pretraga
* @param prezime, ime Primarni i sekundarni kljuc u pretrazi
* 
* @return rez Prvi pronadjeni cvor sa trazenim kljucevima
*/
Kontakt* pretragaImenikaImePrez(Kontakt* rez, string prezime, string ime)
{
	if (rez == nullptr) {
		cout << "Doslo je do greske!\n";
		return nullptr;
	}

	/* Skloneto jer ne radi samo u slucaju inorder sledbenici, a zbog primarnog kljuca to nije uvek tako
	while (rez != nullptr && rez->ime != ime) {
		if (ime < rez->ime && ime < rez->ime)
			rez = rez->levi;
		else if (ime > rez->ime && ime > rez->ime)
			rez = rez->desni;
	}*/

	if (rez == nullptr) {
		cout << "Kontakt s tim prezimenom nije pronadjen.\n";
		return nullptr;
	}

	// Primena pseudokoda za iterativni inorder obilazak dat u "Algoritmi i strukture podataka", Milo Tomasevic, 2008, str 134
	stack<Kontakt*> s;
	Kontakt* tren = rez;

	while (tren != nullptr || !s.empty()) {
		while (tren != nullptr) {
			s.push(tren);
			tren = tren->levi;
		}
		tren = s.top();
		s.pop();

		if (tren->ime == ime)
			cout << tren->prezime << ", " << tren->ime << ", " << tren->broj << endl;

		tren = tren->desni;
	}

	return rez;
}

/*
* @brief Brise nevalidni i ponovo dodaje azurirani kontakt
* Poziva funkcije brisanjeKontakta() i  dodavanjeKontakta()
* 
* @param koren Pokazivac na korena stabla
* @param kont Pokazivac na pronadjeni kontakt sa datim imenom i prezimenom
* @param prezime, ime, broj Azurirani informacioni sadrzaj
* 
* @return Azurirani pokazivac na korena stabla
*/
Kontakt* izmenaKontakta(Kontakt* koren, Kontakt* kont, string prezime, string ime, string broj)
{
	if (koren == nullptr) {
		cout << "Ne postoji formirano stablo.\n";
		return nullptr;
	}

	brisanjeKontakt(koren, kont->prezime, kont->ime);

	dodavanjeKontakt(koren, prezime, ime, broj);

	return koren;
}

/*
* @brief Binarna pretraga sa datim informacionim sadrzajem, gde se i novi cvor ubacuje ukoliko je bila neuspesna
* 
* @param koren Pokazivac na korena stabla
* @param prezime, ime, broj Informacioni sadrzaj novog cvora
* 
* @return koren Azurirani pokazivac na korena stabla
*/
Kontakt* dodavanjeKontakt(Kontakt* koren, string prezime, string ime, string broj)
{
	if (koren == nullptr) {
		cout << "Ne postoji formirano stablo.\n";
		return nullptr;
	}

	// Adaptirano od pseudokoda od "Algoritmi i strukture podataka", Milo Tomasevic, 2008, str. 229
	Kontakt* p = koren, * q = nullptr;
	while (p != nullptr) {
		q = p;
		if ((p->prezime == prezime) && (p->ime == ime) && (p->broj == broj)) {
			cout << "Kontakt vec postoji.\n";
			return p;
		}
		if ((prezime < p->prezime) || (prezime == p->prezime && ime < p->ime))
			p = p->levi; 
		else
			p = p->desni;
	}

	if (q == nullptr) {
		koren->prezime = prezime;
		koren->ime = ime;
		koren->broj = broj;
		koren->otac = nullptr;
		koren->levi = nullptr;
		koren->desni = nullptr;
	}
	else {
		if ((prezime > q->prezime) || (prezime == q->prezime && ime > q->ime)) {
			q->desni = new Kontakt;
			q->desni->prezime = prezime;
			q->desni->ime = ime;
			q->desni->broj = broj;
			q->desni->otac = q;
			q->desni->levi = nullptr;
			q->desni->desni = nullptr;
		}
		else if ((prezime < q->prezime) || (prezime == q->prezime && ime < q->ime)) {
			q->levi = new Kontakt;
			q->levi->prezime = prezime;
			q->levi->ime = ime;
			q->levi->broj = broj;
			q->levi->otac = q;
			q->levi->levi = nullptr;
			q->levi->desni = nullptr;
		}
	}
	return koren;
}

/*
* @brief Binarna pretraga sa datim informacionim sadrzajem i brisanje datog cvora pri uspesna pretraga
* 
* @param koren Pokazivac na korena stabla
* @param prezime, ime Informacioni sadrzaj
* 
* @return koren Azuriran pokazivac na korena stabla
*/
Kontakt* brisanjeKontakt(Kontakt*& koren, string prezime, string ime)
{
	if (koren == nullptr) {
		cout << "Ne postoji formirano stablo.\n";
		return nullptr;
	}

	Kontakt* tren = koren, *q = nullptr;
	Kontakt* rtren = nullptr;

	// Adaptirano od pseudokoda za brisanje u BST dat u "Algoritmi i strukture podataka", Milo Tomasevic, 2008, str.233
	while (tren != nullptr && prezime != tren->prezime) {
		q = tren;
		if (prezime < tren->prezime)
			tren = tren->levi;
		else
			tren = tren->desni;
	}
	if (tren == nullptr) {
		cout << "Kontakt ne postoji.\n";
		return nullptr;
	}
	else{ 
		if (tren->desni == nullptr) 
			 rtren = tren->desni;
		else {
			Kontakt* o = tren;
			rtren = tren->desni;
			Kontakt* s = rtren->levi;

			while (s != nullptr) {
				o = rtren;
				rtren = s;
				s = rtren->levi;
			}
			if (o != tren) {
				o->levi = rtren->desni;
				rtren->desni = tren->desni;
			}
			rtren->levi = tren->levi;
		}
	}
	if (q == nullptr)
		koren = rtren;
	else if (tren == q->levi)
		q->levi = rtren;
	else
		q->desni = rtren;

	delete(tren);
	tren = nullptr;

	return koren;
}

/*
* @brief Ispisuje formatirano stablo po inorder obilasku da bi ispis bio sortiran
* 
* @param koren Pokazivac na koren stabla 
*/
void ispisBST(Kontakt* koren)
{
	if (koren == nullptr) {
		cout << "Ne postoji formirano stablo.\n";
		return;
	}

	stack<Kontakt*> s;
	Kontakt* tren = koren;

	// Primena pseudokoda za iterativni inorder obilazak dat u "Algoritmi i strukture podataka", Milo Tomasevic, 2008, str 134
	while (tren!= nullptr || !s.empty()) {
		while (tren != nullptr) {
			s.push(tren);
			tren = tren->levi;
		}
		tren = s.top();
		s.pop();

		cout << tren->prezime << ", " << tren->ime << ", " << tren->broj << endl;

		tren = tren->desni;
	}
}

/* @brief Brise prosledjeno stablo po inorderu
* 
* @param koren Koren stabla, prosledjen tako da bi se azurirao na nil kad stablo bude izbrisano
*/
void brisanjeBST(Kontakt*& koren)
{
	if (koren == nullptr) {
		cout << "Ne postoji formirano stablo.\n";
		return;
	}

	stack<Kontakt*> s;
	Kontakt* tren = koren, *priv;

	// Primena pseudokoda za iterativni inorder obilazak dat u "Algoritmi i strukture podataka", Milo Tomasevic, 2008, str 134
	while (tren != nullptr || !s.empty()) {
		while (tren != nullptr) {
			s.push(tren);
			tren = tren->levi;
		}
		tren = s.top();
		s.pop();

		priv = tren;
		tren = tren->desni;

		delete priv;
		priv = nullptr;
	}
	koren = nullptr;
}