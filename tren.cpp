#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <ctime>
#include <regex>

using namespace std;

class Operator
{
public:
    string nume;
    string password;

    Operator(const string &nume, const string &password)
        : nume(nume), password(password) {}

    void display() const
    {
        cout << "Operator: " << nume << ", Parola: " << password << endl;
    }
};

class Utilizator
{
public:
    string nume;
    string prenume;
    string email;
    string password;
    string keyword;

    Utilizator(const string &nume, const string &prenume, const string &email, const string &password, const string &keyword)
        : nume(nume), prenume(prenume), email(email), password(password), keyword(keyword) {}
};

class tren
{
public:
    string oras_pelcare;
    string oras_destinatie;
    int zi_plecare;
    int luna_plecare;
    int ora_plecare;
    int minut_plecare;
    int locuri_clasa1;
    int locuri_clasa2;

    tren(const string &oras_pelcare, const string &oras_destinatie, int zi_plecare, int luna_plecare, int ora_plecare, int minut_plecare, int locuri_clasa1, int locuri_clasa2)
        : oras_pelcare(oras_pelcare), oras_destinatie(oras_destinatie), zi_plecare(zi_plecare), luna_plecare(luna_plecare), ora_plecare(ora_plecare), minut_plecare(minut_plecare), locuri_clasa1(locuri_clasa1), locuri_clasa2(locuri_clasa2) {}

    string toCSV() const
    {
        return oras_pelcare + ";" + oras_destinatie + ";" +
               to_string(zi_plecare) + ";" + to_string(luna_plecare) + ";" +
               to_string(ora_plecare) + ";" + to_string(minut_plecare) + ";" +
               to_string(locuri_clasa1) + ";" + to_string(locuri_clasa2);
    }
};

class Importer
{
public:
    static Operator importOperator(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            throw ios_base::failure("Nu s-a putut deschide fișierul pentru import: " + filename);
        }

        string line;
        if (getline(file, line))
        {
            istringstream stream(line);
            string nume, password;
            getline(stream, nume, ';');
            getline(stream, password, ';');
            return Operator(nume, password);
        }
        else
        {
            throw runtime_error("Fișierul este gol sau formatul este invalid.");
        }
    }

    static vector<Utilizator> importUtilizatori(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            throw ios_base::failure("Nu s-a putut deschide fișierul pentru import: " + filename);
        }

        vector<Utilizator> utilizatori;
        string line;
        while (getline(file, line))
        {
            istringstream stream(line);
            string nume, prenume, email, password, keyword;
            getline(stream, nume, ';');
            getline(stream, prenume, ';');
            getline(stream, email, ';');
            getline(stream, password, ';');
            getline(stream, keyword, ';');
            utilizatori.emplace_back(nume, prenume, email, password, keyword);
        }

        return utilizatori;
    }

    static vector<tren> importTrenuri(const string &filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            throw ios_base::failure("Nu s-a putut deschide fișierul pentru import: " + filename);
        }
        vector<tren> trenuri;
        string line;

        while (getline(file, line))
        {
            istringstream stream(line);
            string oras_pelcare, oras_destinatie;
            int zi_plecare, luna_plecare, ora_plecare, minut_plecare, locuri_clasa1, locuri_clasa2;
            getline(stream, oras_pelcare, ';');
            getline(stream, oras_destinatie, ';');
            string temp;
            getline(stream, temp, ';');
            zi_plecare = stoi(temp);
            getline(stream, temp, ';');
            luna_plecare = stoi(temp);
            getline(stream, temp, ';');
            ora_plecare = stoi(temp);
            getline(stream, temp, ';');
            minut_plecare = stoi(temp);
            getline(stream, temp, ';');
            locuri_clasa1 = stoi(temp);
            getline(stream, temp, ';');
            locuri_clasa2 = stoi(temp);
            tren tren(oras_pelcare, oras_destinatie, zi_plecare, luna_plecare, ora_plecare, minut_plecare, locuri_clasa1, locuri_clasa2);
            trenuri.push_back(tren);
        }
        return trenuri;
    }
};

class Exporter
{
public:
    static void exportUtilizator(const string &filename, const Utilizator &utilizator)
    {
        ofstream file(filename, ios::app); 
        if (!file.is_open())
        {
            throw ios_base::failure("Nu s-a putut deschide fișierul pentru export: " + filename);
        }

        
        file << utilizator.nume << ";"
             << utilizator.prenume << ";"
             << utilizator.email << ";"
             << utilizator.password << ";"
             << utilizator.keyword << "\n";

        
        if (file.fail())
        {
            throw ios_base::failure("Eroare la scrierea în fișier: " + filename);
        }
    }
};

class exportertren
{
public:
    static void exportTren(const string &filename, const tren &tren)
    {
        ofstream file(filename, ios::app);
        if (!file.is_open())
        {
            throw ios_base::failure("Nu s-a putut deschide fișierul pentru export: " + filename);
        }

        file << tren.oras_pelcare << ";"
             << tren.oras_destinatie << ";"
             << tren.zi_plecare << ";"
             << tren.luna_plecare << ";"
             << tren.ora_plecare << ";"
             << tren.minut_plecare << ";"
             << tren.locuri_clasa1 << ";"
             << tren.locuri_clasa2 << "\n";
    }
};

void rescrieFisierCSV(const string &numeFisier, const vector<tren> &trenuri)
{
    ofstream fisier(numeFisier);
    if (!fisier.is_open())
    {
        cerr << "Nu s-a putut deschide fisierul pentru rescriere!" << endl;
        return;
    }

    for (const auto &t : trenuri)
    {
        fisier << t.toCSV() << "\n";
    }

    fisier.close();
}

int strongpasswordchecker(string password)
{
    int n = password.length();
    if (n < 6)
        return 0;

    int missing = 4;
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;

    for (int i = 0; i < n; i++)
    {
        if (islower(password[i]))
            hasLower = true;
        if (isupper(password[i]))
            hasUpper = true;
        if (isdigit(password[i]))
            hasDigit = true;
        if (!isalnum(password[i]))
            hasSpecial = true;
    }

    if (hasLower)
        missing--;
    if (hasUpper)
        missing--;
    if (hasDigit)
        missing--;
    if (hasSpecial)
        missing--;

    if (n >= 6 && n <= 8 && missing == 0)
        return 1;

    if (n > 8 && missing == 0)
        return 2;

    if (n >= 20)
        return 3;
}

string encryptPassword(const string &password, const string &keyword)
{
    string encryptedPassword;
    int keywordLength = keyword.length();

    for (size_t i = 0; i < password.length(); ++i)
    {
        char passwordChar = password[i];
        char keyChar = keyword[i % keywordLength]; 

        if (isalpha(passwordChar))
        { 
            char base = isupper(passwordChar) ? 'A' : 'a';
            char keyBase = isupper(keyChar) ? 'A' : 'a';

           
            char encryptedChar = ((passwordChar - base) + (keyChar - keyBase)) % 26 + base;
            encryptedPassword += encryptedChar;
        }
        else
        {
           
            encryptedPassword += passwordChar;
        }
    }

    return encryptedPassword;
}
string generateKeyword()
{
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string keyword;
    for (int i = 0; i < 8; ++i) 
    {
        keyword += chars[rand() % chars.length()];
    }
    return keyword;
}

bool isValidEmail(const string &email)
{
    const regex emailPattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, emailPattern);
}

int ok, intorcere_dupa_inregistrare, varianta, luna_curenta, zi_curenta, ora_curenta, minut_curent;
bool emailValid;

int main()
{

    time_t now = time(0);
    tm *ltm = localtime(&now);
    luna_curenta = 1 + ltm->tm_mon;
    zi_curenta = ltm->tm_mday;
    ora_curenta = ltm->tm_hour;
    minut_curent = ltm->tm_min;
    while (true)
    {
        try
        {
            
            string filename1 = "operator.csv";

            Operator op = Importer::importOperator(filename1);

            
            vector<Utilizator> utilizatori = Importer::importUtilizatori("utilizatori.csv");
            cout<<"Nu poti sa dechizi fisieriul trenuri.csv"<<endl;
            
            vector<tren> trenuri = Importer::importTrenuri("trenuri.csv");
            cout<<"Buna ziua!"<<endl;

            cout << "Cum vrei sa te loghezi?" << endl;
            cout << "Admin: apasa tasta 'a'" << endl;
            cout << "Utilizator: apasa tasta 'u'" << endl;
            cout << "Pentru inregistrare apasa tasta 'r'" << endl;

            char c;
            cin >> c;

            string nume, parola, nume1, parola1, nume2, prenume2, email2, parola2;

            switch (c)
            {
            case 'a':

                cout << "Introduceti numele de utilizator: ";
                cin >> nume;
                cout << "Introduceti parola: ";
                cin >> parola;
                {
                    bool found = false;
                    if (nume == op.nume && parola == op.password)
                    {
                        cout << "Logare cu succes!" << endl;
                        found = true;
                    }
                    if (!found)
                    {
                        cout << "Nume de administraor sau parola gresita!" << endl;
                    }
                    if (found)
                    {
                        int adaugare;
                        cout << "Daca doriti sa adaugati un tren apasati tasta 1, daca doriti sa stergeti o clatorie apasati tasta 2, altfel apasati tasta 0: ";
                        cin >> adaugare;

                        while (adaugare == 1)
                        {
                        start3:
                            cout << "Introduceti orasul de plecare: ";
                            string oras_pelcare;
                            cin >> oras_pelcare;
                            int nr = oras_pelcare.length();
                            for (int i = 0; i < nr; i++)
                            {
                                if (isdigit(oras_pelcare[i]) || isalnum(oras_pelcare[i]) == 0)
                                {
                                    cout << "Oras inexistent!" << endl;
                                    goto start3;
                                }
                            }
                            
                        start4:
                            cout << "Introduceti orasul de destinatie: ";
                            string oras_destinatie;
                            cin >> oras_destinatie;
                            int nr1 = oras_destinatie.length();
                            for (int i = 0; i < nr1; i++)
                            {
                                if (isdigit(oras_destinatie[i]) || isalnum(oras_destinatie[i]) == 0)
                                {
                                    cout << "Oras inexistent!" << endl;
                                    goto start4;
                                }
                            }
                        start5:
                            cout << "Introduceti ziua plecarii: ";
                            int zi_plecare;
                            cin >> zi_plecare;
                            cout << "Introduceti luna plecarii: ";
                            int luna_plecare;
                            cin >> luna_plecare;
                            cout << "Introduceti ora plecarii: ";
                            int ora_plecare;
                            cin >> ora_plecare;
                            cout << "Introduceti minutul plecarii: ";
                            int minut_plecare;
                            cin >> minut_plecare;
                            if (luna_plecare < luna_curenta || (luna_plecare == luna_curenta && zi_plecare < zi_curenta) || (luna_plecare == luna_curenta && zi_plecare == zi_curenta && ora_plecare < ora_curenta) || (luna_plecare == luna_curenta && zi_plecare == zi_curenta && ora_plecare == ora_curenta && minut_plecare < minut_curent))
                            {
                                cout << "Data introdusa este in trecut!" << endl;
                                goto start5;
                            }
                            cout << "Introduceti numarul de locuri clasa 1: ";
                            int locuri_clasa1;
                            cin >> locuri_clasa1;
                            cout << "Introduceti numarul de locuri clasa 2: ";
                            int locuri_clasa2;
                            cin >> locuri_clasa2;
                            tren tren(oras_pelcare, oras_destinatie, zi_plecare, luna_plecare, ora_plecare, minut_plecare, locuri_clasa1, locuri_clasa2);
                            try
                            {
                                exportertren::exportTren("trenuri.csv", tren);
                                cout << "Trenul a fost inregistrat cu succes!" << endl;
                            }
                            catch (const exception &e)
                            {
                                cerr << "Eroare la export: " << e.what() << endl;
                            }
                            cout << "Daca doriti sa adaugati un alt tren apasati tasta 1, daca doriti sa stergeti un tren apasati tasta 2, altfel apasati tasta 0: ";
                            cin >> adaugare;
                        }

                        while (adaugare == 2)
                        {
                            cout << "Introduceti orasul de plecare: ";
                            string oras_pelcare;
                            cin >> oras_pelcare;

                            cout << "Introduceti orasul de destinatie: ";
                            string oras_destinatie;
                            cin >> oras_destinatie;

                            cout << "Introduceti ziua plecarii: ";
                            int zi_plecare;
                            cin >> zi_plecare;

                            cout << "Introduceti luna plecarii: ";
                            int luna_plecare;
                            cin >> luna_plecare;

                            cout << "Introduceti ora plecarii: ";
                            int ora_plecare;
                            cin >> ora_plecare;

                            cout << "Introduceti minutul plecarii: ";
                            int minut_plecare;
                            cin >> minut_plecare;

                            tren trenDeSters(oras_pelcare, oras_destinatie, zi_plecare, luna_plecare, ora_plecare, minut_plecare, 0, 0);

                            bool trenGasit = false;

                            for (auto it = trenuri.begin(); it != trenuri.end();)
                            {
                                if (trenDeSters.oras_pelcare == it->oras_pelcare &&
                                    trenDeSters.oras_destinatie == it->oras_destinatie &&
                                    trenDeSters.zi_plecare == it->zi_plecare &&
                                    trenDeSters.luna_plecare == it->luna_plecare &&
                                    trenDeSters.ora_plecare == it->ora_plecare &&
                                    trenDeSters.minut_plecare == it->minut_plecare)
                                {
                                    it = trenuri.erase(it); 
                                    trenGasit = true;
                                    cout << "Calatoria a fost stearsa cu succes!" << endl;
                                }
                                else
                                {
                                    ++it;
                                }
                            }

                            if (!trenGasit)
                            {
                                cout << "Trenul specificat nu a fost găsit!" << endl;
                            }

                            
                            rescrieFisierCSV("trenuri.csv", trenuri);

                            cout << "Daca doriti sa adaugati un alt tren apasati tasta 1, daca doriti sa stergeti un tren apasati tasta 2, altfel apasati tasta 0: ";
                            cin >> adaugare;
                        }
                    }
                    break;
                }

            case 'u':
            start2:
                cout << "Introduceti numele de utilizator: ";
                cin >> nume1;
                cout << "Introduceti parola: ";
                cin >> parola1;

                for (const auto &utilizator : utilizatori)
                {
                    if (nume1 == utilizator.nume)
                    {

                        string encryptedInput = encryptPassword(parola1, utilizator.keyword);

                        if (encryptedInput == utilizator.password)
                        {
                            cout << "Logare cu succes!" << endl;
                            cout << "Bine ai venit, " << utilizator.nume << " " << utilizator.prenume << "!" << endl;
                            cout << "Daca doriti sa cautati un tren apasati tasta 1, altfel apasati tasta 0: ";
                            cin >> varianta;
                        }
                        else
                        {
                            cout << "Nume de utilizator sau parola gresita!" << endl;
                            goto start2;
                        }
                    }
                }
                while (varianta)
                {
                    cout << "Introduceti orasul de plecare: ";
                    string oras_plecare;
                    cin >> oras_plecare;
                    cout << "Introduceti orasul de destinatie: ";
                    string oras_destinatie;
                    cin >> oras_destinatie;
                start6:
                    cout << "Introduceti ziua plecarii: ";
                    int zi_plecare;
                    cin >> zi_plecare;
                    cout << "Introduceti luna plecarii: ";
                    int luna_plecare;
                    cin >> luna_plecare;
                    cout << "Introduceti ora plecarii: ";
                    int ora_plecare;
                    cin >> ora_plecare;
                    cout << "Introduceti minutul plecarii: ";
                    int minut_plecare;
                    cin >> minut_plecare;

                    if (luna_plecare < luna_curenta || (luna_plecare == luna_curenta && zi_plecare < zi_curenta) || (luna_plecare == luna_curenta && zi_plecare == zi_curenta && ora_plecare < ora_curenta) || (luna_plecare == luna_curenta && zi_plecare == zi_curenta && ora_plecare == ora_curenta && minut_plecare < minut_curent))
                    {
                        cout << "Data introdusa este in trecut!" << endl;
                        goto start6;
                    }
                    for (auto &tren : trenuri)
                    {

                        if (oras_plecare == tren.oras_pelcare && oras_destinatie == tren.oras_destinatie && zi_plecare == tren.zi_plecare && luna_plecare == tren.luna_plecare && ora_plecare == tren.ora_plecare && minut_plecare == tren.minut_plecare)
                        {
                            cout << "Trenul a fost gasit!" << endl;
                            cout << "Numarul de locuri clasa 1 disponibile: " << tren.locuri_clasa1 << endl;
                            cout << "Numarul de locuri clasa 2 disponibile: " << tren.locuri_clasa2 << endl;
                            cout << "La ce clasa doriti sa va rezervati locul? Clasa 1 - apasati tasta 1, Clasa 2 - apasati tasta 2: ";
                            int clasa;
                            cin >> clasa;
                            if (clasa == 1)
                            {
                                if (tren.locuri_clasa1 > 0)
                                {
                                    cout << "Cate locuri doriti sa rezervati? ";
                                    int nr_locuri;
                                    cin >> nr_locuri;
                                    if (nr_locuri > tren.locuri_clasa1)
                                    {
                                        cout << "Nu sunt suficiente locuri disponibile!" << endl;
                                    }
                                    else
                                    {
                                        tren.locuri_clasa1 -= nr_locuri;
                                        cout << "Rezervare efectuata cu succes!" << endl;
                                    }
                                }
                                else
                                {
                                    cout << "Nu mai sunt locuri disponibile la clasa 1!" << endl;
                                }
                            }
                            else if (clasa == 2)
                            {
                                if (tren.locuri_clasa2 > 0)
                                {
                                    cout << "Cate locuri doriti sa rezervati? ";
                                    int nr_locuri;
                                    cin >> nr_locuri;
                                    if (nr_locuri > tren.locuri_clasa2)
                                    {
                                        cout << "Nu sunt suficiente locuri disponibile!" << endl;
                                    }
                                    else
                                    {
                                        tren.locuri_clasa2 -= nr_locuri;
                                        cout << "Rezervare efectuata cu succes!" << endl;
                                    }
                                }
                                else
                                {
                                    cout << "Nu mai sunt locuri disponibile la clasa 2!" << endl;
                                }
                            }
                        }
                        else
                        {
                            cout << "Trenul nu a fost gasit!" << endl;
                        }
                        cout << "am parcurs forul" << endl;
                    }
                    cout << "Daca doriti sa cautati un alt tren apasati tasta 1, altfel apasati tasta 0: ";
                    cin >> varianta;
                }
                break;

            case 'r':
                cout << "Introduceti numele: ";
                cin >> nume2;
                cout << "Introduceti prenumele: ";
                cin >> prenume2;
                cout << "Introduceti emailul: ";
                cin >> email2;
                while (true)
                {
                    if (isValidEmail(email2))
                    {
                        break;
                    }
                    cout << "Email invalid!" << endl;
                    cout << "Introduceti un alt email: ";
                    cin >> email2;
                }

                while (!emailValid)
                {
                    emailValid = 1;

                    for (const auto &utilizator : utilizatori)
                    {
                        if (email2 == utilizator.email)
                        {
                            cout << "Emailul este deja folosit!" << endl;
                            cout << "Introduceti un alt email: ";
                            cin >> email2;

                            emailValid = 0;
                            break;
                        }
                    }
                }
                cout << "Introduceti parola: ";
                cin >> parola2;
                ok = 0;
            start1:
                if (strongpasswordchecker(parola2) == 0)
                {
                    cout << "Parola trebuie sa aiba cel putin 6 caractere si sa contina cel putin o litera mica, o litera mare, o cifra si un caracter special!" << endl;
                    cout << "Introduceti o noua parola: ";
                    cin >> parola2;
                    goto start1;
                }
                else if (strongpasswordchecker(parola2) == 1)
                {
                    cout << "Parola este ok!" << endl;
                    ok = 1;
                }
                else if (strongpasswordchecker(parola2) == 2)
                {
                    cout << "Parola este puternica!" << endl;
                    ok = 1;
                }
                else if (strongpasswordchecker(parola2) == 3)
                {
                    cout << "Parola este prea lunga!" << endl;
                    cout << "Introduceti o noua parola: ";
                    cin >> parola2;
                    goto start1;
                }
                if (ok == 1)
                {
                    string keyword2 = generateKeyword();
                    Utilizator utilizator1(nume2, prenume2, email2, encryptPassword(parola2, keyword2), keyword2);
                    try
                    {
                        Exporter::exportUtilizator("utilizatori.csv", utilizator1);
                        cout << "Utilizatorul a fost inregistrat cu succes!" << endl;
                    }
                    catch (const exception &e)
                    {
                        cerr << "Eroare la export: " << e.what() << endl;
                    }
                }

                break;

            default:
                cout << "Optiune invalida!" << endl;
                break;
            }
        }

        catch (const exception &e)
        {
            cerr << "Eroare: " << e.what() << endl;
        }
        cout << "Daca doriti sa va intoarceti la meniu apasati tasta 1, altfel apasati tasta 0: ";
        cin >> intorcere_dupa_inregistrare;
        if (intorcere_dupa_inregistrare == 0)
        {
            break;
        }
    }

    return 0;
}
