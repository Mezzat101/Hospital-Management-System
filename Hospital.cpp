
using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ios>    //used to get stream size
#include <limits> //used to get numeric limits
#include "Hospital.hpp"

/*************************Person Decleration************************/
person::person()
{
    id = -1;
}
void person::addPerson(int16_t minAge, int16_t maxAge)
{
    // getting basic details of the person from the user side;
    cout << "\nEnter name: \nFirst name:\n";
    getline(cin >> ws, firstName);
    cout << "\nLast name:\n";
    getline(cin, lastName);

    cout << "\nEnter age: \n";
    cin >> age;
    while (age <= 0)
        cout << "Was that supposed to make any kind of sense?\nEnter again!\n", cin >> age;
    if (category != 2)
    {
        if (age < minAge)
            return void(cout << "Sorry, person should be at least " << minAge << " years old to be registered as a " << cat << ".\n");
        else if (age > maxAge)
            return void(cout << "Sorry, we can't register a person older than " << maxAge << " years as a " << cat << ".\n");
    }

    cout << "\nGender (M = Male || F = Female): \n";
    cin >> gender;
    while (gender != 'M' && gender != 'F')
        cout << "M or F?\n", cin >> gender;
    cout << "\nEnter mobile number (with country code): \n";
    getline(cin >> ws, mobNumber);
    add.takeInput();
    return;
}
void person::printDetails()
{
    if (id == -1)
        return;
    cout << "\nDetails:\n";
    cout << "ID              : " << id << "\n";
    cout << "Full Name       : " << firstName << " " << lastName << "\n";
    cout << "Gender          : " << gender << "\n";
    cout << "Age             : " << age << "\n";
    cout << "Mobile          : " << mobNumber << "\n";
    cout << "Address         : ";
    add.print();
    return;
}
void person::printDetailsFromHistory()
{
    if (id == -1)
        return;
    cout << "\nHistory Details :\n";
    cout << "Full Name       : " << firstName << " " << lastName << "\n";
    cout << "Gender          : " << gender << "\n";
    cout << "Age             : " << age << "\n";
    cout << "Mobile          : " << mobNumber << "\n";
    cout << "Address         : ";
    add.print();
    return;
}

/*************************************Patient Decleration**********************************************/

patient::patient()
{
    id = -1;
    hospitalized = 0;
    alive = 1;
    cat = "patient";
    category = 2;
}
void patient::fillMap()
{
    fstream f;
    f.open("./data/patients.csv", ios::in);
    string temp;
    // skipping the first row containing column headers;
    getline(f >> ws, temp);
    // analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        patient p;
        // creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s4, s5, s7, s8, s9, s10, s11;
        // reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, p.firstName, ',');
        getline(s, p.lastName, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        getline(s, p.mobNumber, ',');
        getline(s, s7, ',');
        getline(s, s8, ',');
        getline(s, s9, ',');
        getline(s, s10, ',');
        getline(s, s11, ',');
        p.id = strToNum(s1);
        p.gender = s4[0];
        p.age = strToNum(s5);
        p.add.strToAdd(s7);
        p.height = strToNum(s8);
        p.weight = strToNum(s9);
        p.hospitalized = (s10 == "Y");
        p.alive = (s11 == "Y");
        hospital::patientsList[p.id] = p;
    }
    f.close();
    return;
}
void patient::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "patientId,firstName,lastName,gender,age,mobNumber,address,height,weight,wasHospitalized?,stillAlive(ifHospitalized)?\n";
    for (auto i : hospital::patientsList)
        f << i.second.id << "," << i.second.firstName << "," << i.second.lastName << "," << i.second.gender
          << "," << i.second.age << "," << i.second.mobNumber << "," << i.second.add.addToStr()
          << "," << i.second.height << "," << i.second.weight << ","
          << (i.second.hospitalized ? 'Y' : 'N') << "," << (i.second.alive ? 'Y' : 'N') << endl;
    f.close();
    remove("./data/patients.csv");
    rename("./data/temp.csv", "./data/patients.csv");
    return;
}
void patient::addPerson()
{
    // getting the basic details of patient from the user side;
    person::addPerson();
    // getting patient specific details;
    cout << "\nEnter the height of the patient (in cms):\n";
    cin >> height;
    cout << "\nEnter the weight of the patient (in pounds):\n";
    cin >> weight;
    char tt;
    cout << "\nIs the patient being hospitalized? (Y = Yes || N = No)\n";
    cin >> tt;
    while (tt != 'Y' && tt != 'N')
        cout << "Y or N?\n", cin >> tt;
    hospitalized = (tt == 'Y');
    if (hospital::patientsList.rbegin() != hospital::patientsList.rend())
        id = ((hospital::patientsList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::patientsList[id] = *this;

    // creating a fstream object to read/write from/to files;
    fstream f;
    // creating a record in patientsHistory.csv;
    f.open("./data/patientsHistory.csv", ios::app);
    f << firstName << "," << lastName << "," << gender << "," << age
      << "," << mobNumber << "," << add.addToStr() << "," << height << ","
      << weight << "," << ((hospitalized) ? "Y" : "N")
      << ","
      << "Y"
      << ","
      << "N"
      << "\n";
    f.close();

    cout << "\n"
         << firstName << " " << lastName << " registered successfully!\n";
    cout << "Their ID is: " << id << "\n";

    return;
}
void patient::printDetails()
{
    if (id == -1)
        return;
    person::printDetails();
    cout << "Height (cms)    : " << height << "\n";
    cout << "Weight (pounds) : " << weight << "\n";
    cout << "Was Hospitalized? " << ((hospitalized) ? "Y" : "N") << "\n";
    cout << "Alive?          : " << ((alive) ? "Y" : "N") << "\n";
    return;
}
void patient::printDetailsFromHistory(string extraDetails)
{
    if (id == -1)
        return;
    person::printDetailsFromHistory();
    stringstream k(extraDetails);
    string s1;
    getline(k, s1, ',');
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/patientsHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            patient p;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s3, s4, s6, s7, s8, s9, s10;
            // reading from the string stream object 's';
            getline(s, p.firstName, ',');
            getline(s, p.lastName, ',');
            getline(s, s3, ',');
            getline(s, s4, ',');
            getline(s, p.mobNumber, ',');

            if (p.firstName == firstName && p.lastName == lastName && p.mobNumber == mobNumber)
            {

                getline(s, s6, ',');
                getline(s, s7, ',');
                getline(s, s8, ',');
                getline(s, s9, ',');
                getline(s, s10, ',');
                getline(s, s1, ',');
            }
        }
        f.close();
    }
    cout << "Height (cms)    : " << height << "\n";
    cout << "Weight (pounds) : " << weight << "\n";
    cout << "Was Hospitalized? " << ((hospitalized) ? "Y" : "N") << "\n";
    cout << "Alive?          : " << ((alive) ? "Y" : "N") << "\n";
    cout << "Discharged?     : " << s1 << "\n";
    return;
}
void patient::getDetails(int rec)
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by Name\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;
    // 1: Filter by ID;
    if (opt == 1)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::patientsList.find(reqId) != hospital::patientsList.end())
            *this = hospital::patientsList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    // 2: Filter by name;
    else if (opt == 2)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<patient> matchingRecords;
        for (auto i : hospital::patientsList)
        {
            if (i.second.firstName == reqFName && i.second.lastName == reqLName)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > rec)
        {
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required patient: ";
                cin >> reqId;
                if (hospital::patientsList.find(reqId) != hospital::patientsList.end())
                    *this = hospital::patientsList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
        }
    }
    return;
}
void patient::getDetailsFromHistory()
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by Name\n[2]: Filter by Mobile Number\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;
    // 1: Filter by name;
    if (opt == 1)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<patient> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/patientsHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            patient p;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s3, s4, s6, s7, s8, s9, s10, s12;
            // reading from the string stream object 's';
            getline(s, p.firstName, ',');
            getline(s, p.lastName, ',');

            if (p.firstName == reqFName && p.lastName == reqLName)
            {
                getline(s, s3, ',');
                getline(s, s4, ',');
                getline(s, p.mobNumber, ',');
                getline(s, s6, ',');
                getline(s, s7, ',');
                getline(s, s8, ',');
                getline(s, s9, ',');
                getline(s, s10, ',');
                getline(s, s10, ',');
                getline(s, s12);
                p.id = 0;
                p.gender = s3[0];
                p.age = strToNum(s4);
                add.strToAdd(s6);
                p.height = strToNum(s7);
                p.weight = strToNum(s8);
                p.hospitalized = (s9 == "Y");
                p.alive = (s10 == "Y");
                matchingRecords.push_back(p);
                extraDetails.push_back(s12);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    // 2: Filter by mobNumber;
    else if (opt == 2)
    {
        string reqMobNum;
        cout << "Enter mobile number:\n";
        cin >> reqMobNum;
        vector<patient> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/patientsHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            patient p;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s3, s4, s6, s7, s8, s9, s10, s12;
            // reading from the string stream object 's';
            getline(s, p.firstName, ',');
            getline(s, p.lastName, ',');
            getline(s, s3, ',');
            getline(s, s4, ',');
            getline(s, p.mobNumber, ',');

            if (reqMobNum == p.mobNumber)
            {
                getline(s, s6, ',');
                getline(s, s7, ',');
                getline(s, s8, ',');
                getline(s, s9, ',');
                getline(s, s10, ',');
                getline(s, s12);
                p.id = 0;
                p.gender = s3[0];
                p.age = strToNum(s4);
                add.strToAdd(s6);
                p.height = strToNum(s7);
                p.weight = strToNum(s8);
                p.hospitalized = (s9 == "Y");
                p.alive = (s10 == "Y");
                matchingRecords.push_back(p);
                extraDetails.push_back(s12);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    return;
}
void patient::hospitalize()
{
    cout << "\nSearch for the patient.\n";
    getDetails();
    if (id == -1)
        return;
    hospital::patientsList[id].hospitalized = 1;
    string s, temp, corrected;
    stringstream str;
    fstream f, fout;
    str << firstName << "," << lastName
        << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr()
        << "," << height << "," << weight << "," << ((hospitalized) ? "Y" : "N")
        << ","
        << ((alive) ? "Y" : "N")
        << ",N"
        << "\n";
    getline(str >> ws, s);
    str << firstName << "," << lastName
        << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr()
        << "," << height << "," << weight << ","
        << "Y,"
        << ((alive) ? "Y,N\n" : "N,N\n");
    getline(str >> ws, corrected);
    f.open("./data/patientsHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
            fout << corrected << "\n";
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/patientsHistory.csv");
    rename("./data/temp.csv", "./data/patientsHistory.csv");
    cout << firstName << " " << lastName << " hospitalized successfully!\n";
    return;
}
void patient::reportADeath()
{
    cout << "\nSearch for the patient.\n";
    getDetails();
    if (id == -1)
        return;
    hospital::patientsList[id].alive = 0;
    if (!hospital::patientsList[id].hospitalized)
        hospital::patientsList.erase(id);
    string s, temp, corrected;
    stringstream str;
    fstream f, fout;
    str << firstName << "," << lastName
        << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr()
        << "," << height << "," << weight << "," << ((hospitalized) ? "Y" : "N")
        << ","
        << ((alive) ? "Y" : "N")
        << ",N"
        << "\n";
    getline(str, s);
    f.open("./data/patientsHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << firstName << "," << lastName
                 << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr()
                 << "," << height << "," << weight << "," << ((hospitalized) ? "Y,N,N\n" : "N,N,N\n");
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/patientsHistory.csv");
    rename("./data/temp.csv", "./data/patientsHistory.csv");
    cout << firstName << " " << lastName << " reported dead, we hope their soul rests in peace, amen!\n";
    return;
}
void patient::removePerson()
{
    cout << "\nSearch for the patient you want to discharge.\n";
    getDetails();
    if (id == -1)
        return;
    if (!hospitalized)
    {
        cout << "\nPatient wasn't hospitalized, can't be discharged!\n\n";
        return;
    }
    hospital::patientsList.erase(id);
    string s, temp;
    stringstream str;
    fstream f, fout;
    str << firstName << "," << lastName
        << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr()
        << "," << height << "," << weight << "," << ((hospitalized) ? "Y" : "N")
        << ","
        << ((alive) ? "Y" : "N")
        << ",N"
        << "\n";
    getline(str, s);
    f.open("./data/patientsHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << firstName << "," << lastName
                 << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr()
                 << "," << height << "," << weight << "," << ((hospitalized) ? "Y" : "N")
                 << ","
                 << ((alive) ? "Y" : "N")
                 << ",Y"
                 << "\n";
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/patientsHistory.csv");
    rename("./data/temp.csv", "./data/patientsHistory.csv");
    cout << firstName << " " << lastName << " discharged!\n";
    return;
}

// Removing a patient: it's rather "DISCHARGING a HOSPITALIZED patient";
// If a patient wasn't hospitalized, just came to the hospital for an appointment with;
// a doctor then that appointment object will be cleared on the next day automatically;
// and there's no need of removing the patient's record from patients.csv in that case;

// Maybe consider this as a kind of limitation of this system;

/**********************************************nurse Decleration*******************************************/

nurse::nurse()
{
    id = -1;
    type = "";
    cat = "nurse";
    category = 3;
}
void nurse::fillMap()
{
    fstream f;
    f.open("./data/nurses.csv", ios::in);
    string temp;
    // skipping the first row containing column headers;
    getline(f >> ws, temp);
    // analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        nurse n;
        // creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s4, s5, s7;
        // reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, n.firstName, ',');
        getline(s, n.lastName, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        getline(s, n.mobNumber, ',');
        getline(s, s7, ',');
        getline(s, n.type, ',');
        n.id = strToNum(s1);
        n.gender = s4[0];
        n.age = strToNum(s5);
        n.add.strToAdd(s7);
        hospital::nursesList[n.id] = n;
    }
    f.close();
    return;
}
void nurse::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "nurseId,firstName,lastName,gender,age,mobNumber,address,type\n";
    for (auto i : hospital::nursesList)
        f << i.second.id << "," << i.second.firstName << "," << i.second.lastName << "," << i.second.gender
          << "," << i.second.age << "," << i.second.mobNumber << "," << i.second.add.addToStr()
          << "," << i.second.type << endl;
    f.close();
    remove("./data/nurses.csv");
    rename("./data/temp.csv", "./data/nurses.csv");
    return;
}
void nurse::addPerson()
{
    if (hospital::nursesList.size() == hospital::nursesLimit)
    {
        cout << "\n\nNurses limit reached, can't add more!\n\n";
        return;
    }
    // 18 and 65 are the age limits for registration of a new nurse;
    person::addPerson(18, 65);
    if ((age < 18) || (age > 65))
        return;
    cout << "\nEnter the type of the nurse: \n";
    getline(cin >> ws, type);
    if (hospital::nursesList.rbegin() != hospital::nursesList.rend())
        id = ((hospital::nursesList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::nursesList[id] = *this;

    // creating a fstream object to read/write from/to files;
    fstream f;
    // creating a record in nursesHistory.csv;
    f.open("./data/nursesHistory.csv", ios::app);
    f << firstName << "," << lastName << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr() << "," << type << ",N,NA" << endl;
    f.close();

    cout << "\n"
         << firstName << " " << lastName << " registered successfully!\n";
    cout << "Their ID is: " << id << "\n";

    return;
}
void nurse::printDetails()
{
    if (id == -1)
        return;
    person::printDetails();
    cout << "Type            : " << type << "\n";
    return;
}
void nurse::printDetailsFromHistory(string extraDetails)
{
    if (id == -1)
        return;
    person::printDetailsFromHistory();
    stringstream k(extraDetails);
    string s1, s2;
    getline(k, s1, ',');
    getline(k, s2, ',');
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/nursesHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            nurse n;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7;
            // reading from the string stream object 's';
            getline(s, n.firstName, ',');
            getline(s, n.lastName, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            getline(s, n.mobNumber, ',');

            if (n.firstName == firstName && n.lastName == lastName && n.mobNumber == mobNumber)
            {

                getline(s, s7, ',');
                getline(s, n.type, ',');
                getline(s, s1, ',');
                getline(s, s2, ',');
            }
        }
        f.close();
    }
    cout << "Type            : " << type << "\n";
    cout << "Left Work?      : " << s1 << "\n";
    if (s1 == "Y")
        cout << "Reason          : " << s2 << "\n";
    return;
}
void nurse::getDetails(int rec)
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by Name\n[3]: Filter by Type\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2 && opt != 3)
        cout << "option 1, 2 or 3?\n", cin >> opt;
    // 1: Filter by ID;
    if (opt == 1)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::nursesList.find(reqId) != hospital::nursesList.end())
            *this = hospital::nursesList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    // 2: Filter by name;
    else if (opt == 2)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<nurse> matchingRecords;
        for (auto i : hospital::nursesList)
        {
            if (i.second.firstName == reqFName && i.second.lastName == reqLName)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > rec)
        {
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required nurse: ";
                cin >> reqId;
                if (hospital::nursesList.find(reqId) != hospital::nursesList.end())
                    *this = hospital::nursesList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
        }
    }
    // 3: Filter by type;
    else if (opt == 3)
    {
        string reqType;
        cout << "Enter the type of nurse required:\n";
        getline(cin >> ws, reqType);
        vector<nurse> matchingRecords;
        for (auto i : hospital::nursesList)
        {
            if (i.second.type == reqType)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > 0)
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required nurse: ";
                cin >> reqId;
                if (hospital::nursesList.find(reqId) != hospital::nursesList.end())
                    *this = hospital::nursesList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
    }
    return;
}
void nurse::getDetailsFromHistory()
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by Name\n[2]: Filter by Type\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;

    // 1: Filter by name;
    if (opt == 1)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<nurse> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/nursesHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            nurse n;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7, s9;
            // reading from the string stream object 's';
            getline(s, n.firstName, ',');
            getline(s, n.lastName, ',');

            if (n.firstName == reqFName && n.lastName == reqLName)
            {
                getline(s, s4, ',');
                getline(s, s5, ',');
                getline(s, n.mobNumber, ',');
                getline(s, s7, ',');
                getline(s, n.type, ',');
                getline(s, s9);
                n.id = 0;
                n.gender = s4[0];
                n.age = strToNum(s5);
                n.add.strToAdd(s7);
                matchingRecords.push_back(n);
                extraDetails.push_back(s9);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    // 2: Filter by type;
    else if (opt == 2)
    {
        string reqType;
        cout << "Enter the type of nurse required:\n";
        getline(cin >> ws, reqType);
        vector<nurse> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/nursesHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            nurse n;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7, s9;
            // reading from the string stream object 's';
            getline(s, n.firstName, ',');
            getline(s, n.lastName, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            getline(s, n.mobNumber, ',');
            getline(s, s7, ',');
            getline(s, n.type, ',');
            if (n.type == reqType)
            {
                getline(s, s9);
                n.id = 0;
                n.gender = s4[0];
                n.age = strToNum(s5);
                n.add.strToAdd(s7);
                matchingRecords.push_back(n);
                extraDetails.push_back(s9);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    return;
}
void nurse::removePerson()
{
    cout << "\nSearch for the nurse you want to remove.\n";
    getDetails();
    if (id == -1)
        return;
    hospital::nursesList.erase(id);

    string s, temp;
    stringstream str;
    fstream f, fout;
    string reason;
    cout << "\nReason?\n";
    getline(cin >> ws, reason);
    str << firstName << "," << lastName << "," << gender << "," << age
        << "," << mobNumber << "," << add.addToStr() << "," << type << ",N,NA\n";
    getline(str, s);
    f.open("./data/nursesHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << firstName << "," << lastName << "," << gender << "," << age
                 << "," << mobNumber << "," << add.addToStr() << "," << type << ",Y," << reason << "\n";
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/nursesHistory.csv");
    rename("./data/temp.csv", "./data/nursesHistory.csv");
    cout << firstName << " " << lastName << " removed successfully!\n";
    return;
}

/********************************************************************************/

// defining already declared static members in the global scope;

map<int, doctor> hospital::doctorsList;
map<int, patient> hospital::patientsList;
map<int, nurse> hospital::nursesList;
map<int, driver> hospital::driversList;
map<int, ambulance> hospital::ambulancesList;
map<int, appointment> hospital::appointmentsList;

const int hospital::doctorsLimit = 30;
const int hospital::nursesLimit = 50;
const int hospital::driversLimit = 30;
const int hospital::ambulancesLimit = 30;
const int hospital::appointmentsLimit = 240; // per day;
// ummm, patients limit, ummm, no!
// appointments limit is kind of a patients limit;
// rest hospitalized patients limit must be equal to;
// number of available rooms, maybe I'll add this;
// feature some other day in the future;
// not feeling like doing it right now!

// defining already declared methods;

void hospital::printDoctors()
{
    for (auto i : doctorsList)
        i.second.printDetails(), cout << "\n";
    return;
}
void hospital::printPatients()
{
    for (auto i : patientsList)
        i.second.printDetails(), cout << "\n";
    return;
}
void hospital::printNurses()
{
    for (auto i : nursesList)
        i.second.printDetails(), cout << "\n";
    return;
}
void hospital::printDrivers()
{
    for (auto i : driversList)
        i.second.printDetails(), cout << "\n";
    return;
}
void hospital::printAmbulances()
{
    for (auto i : ambulancesList)
        i.second.printDetails(), cout << "\n";
    return;
}
void hospital::printAppointments()
{
    for (auto i : appointmentsList)
        i.second.printDetails(), cout << "\n";
    return;
}
/*******************************************/
int yyyymmdd;
int power(int n, int exp)
{
    int res = 1;
    while (exp)
    {
        if (exp & 1)
            res *= n, exp--;
        else
            n *= n, exp >>= 1;
    }
    return res;
}
int strToNum(string s)
{
    int res = 0;
    for (int i = 0; i < s.size(); i++)
        res += ((s[s.size() - 1 - i] - '0') * power(10, i));
    return res;
}

/***************************driver Decleration***************************/

driver::driver()
{
    id = -1;
    licenseNumber = "";
    idle = 1;
    cat = "driver";
    category = 4;
}
void driver::fillMap()
{
    fstream f;
    f.open("./data/drivers.csv", ios::in);
    string temp;
    // skipping the first row containing column headers;
    getline(f >> ws, temp);
    // analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        driver d;
        // creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s4, s5, s7, s9;
        // reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, d.firstName, ',');
        getline(s, d.lastName, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        getline(s, d.mobNumber, ',');
        getline(s, s7, ',');
        getline(s, d.licenseNumber, ',');
        getline(s, s9, ',');
        d.id = strToNum(s1);
        d.gender = s4[0];
        d.age = strToNum(s5);
        d.add.strToAdd(s7);
        d.idle = (s9 == "Y");
        hospital::driversList[d.id] = d;
    }
    f.close();
    return;
}
void driver::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "driverId,firstName,lastName,gender,age,mobNumber,address,licenseNumber,idle?\n";
    for (auto i : hospital::driversList)
        f << i.second.id << "," << i.second.firstName << "," << i.second.lastName << "," << i.second.gender
          << "," << i.second.age << "," << i.second.mobNumber << "," << i.second.add.addToStr()
          << "," << i.second.licenseNumber << "," << (i.second.idle ? 'Y' : 'N') << endl;
    f.close();
    remove("./data/drivers.csv");
    rename("./data/temp.csv", "./data/drivers.csv");
    return;
}
void driver::addPerson()
{
    if (hospital::driversList.size() == hospital::driversLimit)
    {
        cout << "\n\nDrivers limit reached, can't add more!\n\n";
        return;
    }
    // 18 and 65 are the age limits for registration of a new driver;
    person::addPerson(18, 65);
    if ((age < 18) || (age > 65))
        return;
    cout << "\nEnter the license number of the driver: \n";
    getline(cin >> ws, licenseNumber);
    if (hospital::driversList.rbegin() != hospital::driversList.rend())
        id = ((hospital::driversList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::driversList[id] = *this;

    // creating a fstream object to read/write from/to files;
    fstream f;
    // creating a record in driversHistory.csv;
    f.open("./data/driversHistory.csv", ios::app);
    f << firstName << "," << lastName << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr() << "," << licenseNumber << ",N,NA" << endl;
    f.close();

    cout << "\n"
         << firstName << " " << lastName << " registered successfully!\n";
    cout << "Their ID is: " << id << "\n";

    return;
}
void driver::printDetails()
{
    if (id == -1)
        return;
    person::printDetails();
    cout << "License Number  : " << licenseNumber << "\n";
    cout << "Idle?           : " << (idle ? "Y\n" : "N\n");
    return;
}
void driver::printDetailsFromHistory(string extraDetails)
{
    if (id == -1)
        return;
    person::printDetailsFromHistory();
    stringstream k(extraDetails);
    string s1, s2;
    getline(k, s1, ',');
    getline(k, s2, ',');
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/driversHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            driver d;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7;
            // reading from the string stream object 's';
            getline(s, d.firstName, ',');
            getline(s, d.lastName, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            getline(s, d.mobNumber, ',');

            if (d.firstName == firstName && d.lastName == lastName && d.mobNumber == mobNumber)
            {
                getline(s, s7, ',');
                getline(s, d.licenseNumber, ',');
                getline(s, s1, ',');
                getline(s, s2, ',');
            }
        }
        f.close();
    }
    cout << "License Number  : " << licenseNumber << "\n";
    cout << "Left Work?      : " << s1 << "\n";
    if (s1 == "Y")
        cout << "Reason          : " << s2 << "\n";
    return;
}
void driver::getDetails(int rec)
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by Name\n[3]: Filter by License Number\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2 && opt != 3)
        cout << "option 1, 2 or 3?\n", cin >> opt;
    // 1: Filter by ID;
    if (opt == 1)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::driversList.find(reqId) != hospital::driversList.end())
            *this = hospital::driversList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    // 2: Filter by name;
    else if (opt == 2)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<driver> matchingRecords;
        for (auto i : hospital::driversList)
        {
            if (i.second.firstName == reqFName && i.second.lastName == reqLName)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > rec)
        {
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required driver: ";
                cin >> reqId;
                if (hospital::driversList.find(reqId) != hospital::driversList.end())
                    *this = hospital::driversList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
        }
    }
    // 3: Filter by licenseNumber;
    else if (opt == 3)
    {
        string reqlicenseNumber;
        cout << "Enter the license number of driver required:\n";
        getline(cin >> ws, reqlicenseNumber);
        for (auto i : hospital::driversList)
        {
            if (i.second.licenseNumber == reqlicenseNumber)
            {
                *this = i.second;
                return;
            }
        }
        // if a record is found, it's details will be stored in the driver class object that called this function,
        // and the control is returned;
        // else:
        cout << "\nNo matching record found!\n";
    }
    return;
}
void driver::getDetailsFromHistory()
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by Name\n[2]: Filter by License Number\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;

    // 1: Filter by name;
    if (opt == 1)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<driver> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/driversHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            driver d;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7, s9;
            // reading from the string stream object 's';
            getline(s, d.firstName, ',');
            getline(s, d.lastName, ',');

            if (d.firstName == reqFName && d.lastName == reqLName)
            {
                getline(s, s4, ',');
                getline(s, s5, ',');
                getline(s, d.mobNumber, ',');
                getline(s, s7, ',');
                getline(s, d.licenseNumber, ',');
                getline(s, s9);
                d.id = 0;
                d.gender = s4[0];
                d.age = strToNum(s5);
                d.add.strToAdd(s7);
                matchingRecords.push_back(d);
                extraDetails.push_back(s9);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    // 2: Filter by licenseNumber;
    else if (opt == 2)
    {
        string reqlicenseNumber;
        cout << "Enter the licenseNumber of driver required:\n";
        getline(cin >> ws, reqlicenseNumber);
        vector<driver> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/driversHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7, s9;
            // reading from the string stream object 's';
            getline(s, firstName, ',');
            getline(s, lastName, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            getline(s, mobNumber, ',');
            getline(s, s7, ',');
            getline(s, licenseNumber, ',');
            if (licenseNumber == reqlicenseNumber)
            {
                getline(s, s9);
                id = 0;
                gender = s4[0];
                age = strToNum(s5);
                add.strToAdd(s7);
                printDetailsFromHistory(s9);
                return;
            }
        }
        f.close();
        // if a record is found, it's details will be stored in the driver class object that called this function,
        // and the control is returned;
        // else:
        cout << "\nNo matching record found!\n";
    }
    return;
}
void driver::removePerson()
{
    cout << "\nSearch for the driver you want to remove.\n";
    getDetails();
    if (id == -1)
        return;
    if (!idle)
    {
        cout << "\nSorry, the driver you selected to remove is NOT currently idle.\nOnly idlers can be removed.\n\n";
        return;
    }
    hospital::driversList.erase(id);

    string s, temp;
    stringstream str;
    fstream f, fout;
    string reason;
    cout << "\nReason?\n";
    getline(cin >> ws, reason);
    str << firstName << "," << lastName << "," << gender << "," << age
        << "," << mobNumber << "," << add.addToStr() << "," << licenseNumber << ",N,NA\n";
    getline(str, s);
    f.open("./data/driversHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << firstName << "," << lastName << "," << gender << "," << age
                 << "," << mobNumber << "," << add.addToStr() << "," << licenseNumber << ",Y," << reason << "\n";
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/driversHistory.csv");
    rename("./data/temp.csv", "./data/driversHistory.csv");
    cout << firstName << " " << lastName << " removed successfully!\n";
    return;
}

/*****************************************doctor  Decleration******************************************/

doctor::doctor()
{
    id = -1;
    type = "";
    appointmentsBooked = 0;
    cat = "doctor";
    category = 1;
}
void doctor::fillMap()
{
    fstream f;
    f.open("./data/doctors.csv", ios::in);
    string temp;
    // skipping the first row containing column headers;
    getline(f >> ws, temp);
    // analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        doctor d;
        // creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s4, s5, s7, s9;
        // reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, d.firstName, ',');
        getline(s, d.lastName, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        getline(s, d.mobNumber, ',');
        getline(s, s7, ',');
        getline(s, d.type, ',');
        getline(s, s9, ',');
        d.id = strToNum(s1);
        d.gender = s4[0];
        d.age = strToNum(s5);
        d.add.strToAdd(s7);
        d.appointmentsBooked = strToNum(s9);
        hospital::doctorsList[d.id] = d;
    }
    f.close();
    return;
}
void doctor::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "doctorId,firstName,lastName,gender,age,mobNumber,address,type,appointmentsBooked\n";
    for (auto i : hospital::doctorsList)
        f << i.second.id << "," << i.second.firstName << "," << i.second.lastName << "," << i.second.gender
          << "," << i.second.age << "," << i.second.mobNumber << "," << i.second.add.addToStr()
          << "," << i.second.type << "," << i.second.appointmentsBooked << endl;
    f.close();
    remove("./data/doctors.csv");
    rename("./data/temp.csv", "./data/doctors.csv");
    return;
}
void doctor::addPerson()
{
    if (hospital::doctorsList.size() == hospital::doctorsLimit)
    {
        cout << "\n\nDoctors limit reached, can't add more!\n\n";
        return;
    }
    // 18 and 65 are the age limits for registration of a new doctor;
    person::addPerson(18, 65);
    if ((age < 18) || (age > 65))
        return;
    cout << "\nEnter the type of the doctor: \n";
    getline(cin >> ws, type);
    if (hospital::doctorsList.rbegin() != hospital::doctorsList.rend())
        id = ((hospital::doctorsList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::doctorsList[id] = *this;

    // creating a fstream object to read/write from/to files;
    fstream f;
    // creating a record in doctorsHistory.csv;
    f.open("./data/doctorsHistory.csv", ios::app);
    f << firstName << "," << lastName << "," << gender << "," << age << "," << mobNumber << "," << add.addToStr() << "," << type << ",N,NA" << endl;
    f.close();

    cout << "\n"
         << firstName << " " << lastName << " registered successfully!\n";
    cout << "Their ID is: " << id << "\n";

    return;
}
void doctor::printDetails()
{
    if (id == -1)
        return;
    person::printDetails();
    cout << "Type            : " << type << "\n";
    cout << "Appointments    : " << appointmentsBooked << "/8 (appointments booked today)\n";
    return;
}
void doctor::printDetailsFromHistory(string extraDetails)
{
    if (id == -1)
        return;
    person::printDetailsFromHistory();
    stringstream k(extraDetails);
    string s1, s2;
    getline(k, s1, ',');
    getline(k, s2, ',');
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/doctorsHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            doctor d;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7;
            // reading from the string stream object 's';
            getline(s, d.firstName, ',');
            getline(s, d.lastName, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            getline(s, d.mobNumber, ',');

            if (d.firstName == firstName && d.lastName == lastName && d.mobNumber == mobNumber)
            {

                getline(s, s7, ',');
                getline(s, d.type, ',');
                getline(s, s1, ',');
                getline(s, s2, ',');
            }
        }
        f.close();
    }
    cout << "Type            : " << type << "\n";
    cout << "Left Work?      : " << s1 << "\n";
    if (s1 == "Y")
        cout << "Reason          : " << s2 << "\n";
    return;
}
void doctor::getDetails(int rec)
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by Name\n[3]: Filter by Type\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2 && opt != 3)
        cout << "option 1, 2 or 3?\n", cin >> opt;
    // 1: Filter by ID;
    if (opt == 1)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::doctorsList.find(reqId) != hospital::doctorsList.end())
            *this = hospital::doctorsList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    // 2: Filter by name;
    else if (opt == 2)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<doctor> matchingRecords;
        for (auto i : hospital::doctorsList)
        {
            if (i.second.firstName == reqFName && i.second.lastName == reqLName)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > rec)
        {
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required doctor: ";
                cin >> reqId;
                if (hospital::doctorsList.find(reqId) != hospital::doctorsList.end())
                    *this = hospital::doctorsList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
        }
    }
    // 3: Filter by type;
    else if (opt == 3)
    {
        string reqType;
        cout << "Enter the type of doctor required:\n";
        getline(cin >> ws, reqType);
        vector<doctor> matchingRecords;
        for (auto i : hospital::doctorsList)
        {
            if (i.second.type == reqType)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > 0)
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required doctor: ";
                cin >> reqId;
                if (hospital::doctorsList.find(reqId) != hospital::doctorsList.end())
                    *this = hospital::doctorsList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
    }
    return;
}
void doctor::getDetailsFromHistory()
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by Name\n[2]: Filter by Type\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;

    // 1: Filter by name;
    if (opt == 1)
    {
        string reqFName, reqLName;
        cout << "First Name:\n";
        getline(cin >> ws, reqFName);
        cout << "\nLast Name:\n";
        getline(cin, reqLName);
        vector<doctor> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/doctorsHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            doctor d;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7, s9;
            // reading from the string stream object 's';
            getline(s, d.firstName, ',');
            getline(s, d.lastName, ',');

            if (d.firstName == reqFName && d.lastName == reqLName)
            {
                getline(s, s4, ',');
                getline(s, s5, ',');
                getline(s, d.mobNumber, ',');
                getline(s, s7, ',');
                getline(s, d.type, ',');
                getline(s, s9);
                d.id = 0;
                d.gender = s4[0];
                d.age = strToNum(s5);
                d.add.strToAdd(s7);
                matchingRecords.push_back(d);
                extraDetails.push_back(s9);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    // 2: Filter by type;
    else if (opt == 2)
    {
        string reqType;
        cout << "Enter the type of doctor required:\n";
        getline(cin >> ws, reqType);
        vector<doctor> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/doctorsHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            doctor d;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4, s5, s7, s9;
            // reading from the string stream object 's';
            getline(s, d.firstName, ',');
            getline(s, d.lastName, ',');
            getline(s, s4, ',');
            getline(s, s5, ',');
            getline(s, d.mobNumber, ',');
            getline(s, s7, ',');
            getline(s, d.type, ',');
            if (d.type == reqType)
            {
                getline(s, s9);
                d.id = 0;
                d.gender = s4[0];
                d.age = strToNum(s5);
                d.add.strToAdd(s7);
                matchingRecords.push_back(d);
                extraDetails.push_back(s9);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    return;
}
void doctor::removePerson()
{
    cout << "\nSearch for the doctor you want to remove.\n";
    getDetails();
    if (id == -1)
        return;
    if (appointmentsBooked > 0)
    {
        cout << "\nSelected doctor has appointments booked for today, can't be removed.\n\n";
        return;
    }
    hospital::doctorsList.erase(id);

    string s, temp;
    stringstream str;
    fstream f, fout;
    string reason;
    cout << "\nReason?\n";
    getline(cin >> ws, reason);
    str << firstName << "," << lastName << "," << gender << "," << age
        << "," << mobNumber << "," << add.addToStr() << "," << type << ",N,NA\n";
    getline(str, s);
    f.open("./data/doctorsHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << firstName << "," << lastName << "," << gender << "," << age
                 << "," << mobNumber << "," << add.addToStr() << "," << type << ",Y," << reason << "\n";
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/doctorsHistory.csv");
    rename("./data/temp.csv", "./data/doctorsHistory.csv");
    cout << firstName << " " << lastName << " removed successfully!\n";
    return;
}
/**********************************appointment Declreation***********************************/

appointment::appointment()
{
    id = -1;
    D.id = -1;
    P.id = -1;
}
appointment::~appointment()
{
    id = -1;
    D.id = -1;
    P.id = -1;
    return;
}
void appointment::fillMap()
{
    fstream f;
    f.open("./data/appointments.csv", ios::in);
    string temp;
    // skipping the first row containing column headers;
    getline(f >> ws, temp);
    // analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        appointment a;
        // creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s2, s3, s4, s5;
        // reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, s2, ','); // date is of no use here;
        getline(s, s3, ',');
        getline(s, s4, ',');
        getline(s, s5, ',');
        a.id = strToNum(s1);
        a.D = hospital::doctorsList[strToNum(s3)];
        a.P = hospital::patientsList[strToNum(s4)];
        a.hh = strToNum(s5);
        hospital::appointmentsList[a.id] = a;
    }
    f.close();
    return;
}
void appointment::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "appointmentId,date(YYYYMMDD),doctorId,patientId,startTime(in 24-hr format)\n";
    for (auto i : hospital::appointmentsList)
        f << i.second.id << "," << yyyymmdd << "," << i.second.D.id << "," << i.second.P.id
          << "," << i.second.hh << endl;
    f.close();
    remove("./data/appointments.csv");
    rename("./data/temp.csv", "./data/appointments.csv");
    return;
}
void appointment::printDetails()
{
    if (id == -1)
        return;
    cout << "\n\n\nAppointment Details:\nID                 : " << id << "\n"
         << "Patient's Name     : " + P.firstName + " " + P.lastName + "(ID = " << P.id << ")\n"
         << "Doctor's Name      : " + D.firstName + " " + D.lastName + "(ID = " << D.id << ")\n"
         << "Time (24 Hr format): " << hh << ":00 Hrs to " << hh + 1 << ":00 Hrs\n\n";
    return;
}
void appointment::book()
{
    if (hospital::appointmentsList.size() >= 8 * hospital::doctorsList.size())
    {
        cout << "\n\nSorry, no doctor is available for appointment today!\n\n";
        return;
    }
    cout << "\n\nIs the patient already registered (Y : Yes || N : No)?\n";
    char ans;
    cin >> ans;
    while (ans != 'Y' && ans != 'N')
    {
        cout << "Y or N?\n";
        cin >> ans;
    }
    if (ans == 'N')
    {
        cout << "Register the patient:\n";
        P.addPerson();
    }
    else
    {
        cout << "Search for the required patient:\n\n";
        ans = 'Y';
        while (ans == 'Y')
        {
            P.getDetails();
            ans = 'K';
            if (P.id == -1)
            {
                cout << "Try again (Y : Yes || N : No)?\n";
                cin >> ans;
                while (ans != 'Y' && ans != 'N')
                {
                    cout << "Y or N?\n";
                    cin >> ans;
                }
            }
        }
        if (ans == 'N')
        {
            return;
        }
    }
    cout << "\n\nNow, search for the required doctor:\n";
    ans = 'Y';
    while (ans == 'Y')
    {
        D.getDetails();
        ans = 'K';
        if (D.id == -1)
        {
            cout << "Try again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != 'Y' && ans != 'N')
            {
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
        else if (D.appointmentsBooked >= 8)
        {
            cout << "Sorry, selected doctor has no free slot left for the day!\n";
            cout << "Search again (Y : Yes || N : No)?\n";
            cin >> ans;
            while (ans != 'Y' && ans != 'N')
            {
                cout << "Y or N?\n";
                cin >> ans;
            }
        }
    }
    if (ans == 'N')
    {
        return;
    }
    if (hospital::appointmentsList.rbegin() != hospital::appointmentsList.rend())
        id = ((hospital::appointmentsList.rbegin())->first) + 1;
    else
        id = 1;
    hh = 9 + D.appointmentsBooked;
    hospital::appointmentsList[id] = *this;

    hospital::doctorsList[D.id].appointmentsBooked++;
    cout << "\nAppointment of patient " + P.firstName + " " + P.lastName + " with doctor "
         << D.firstName << " " << D.lastName << " booked successfully!\n";
    printDetails();
    return;
}
void appointment::getDetails()
{
    cout << "\nEnter appointment ID:\n";
    cin >> id;
    if (hospital::appointmentsList.find(id) == hospital::appointmentsList.end())
    {
        cout << "\nInvalid appointment ID!\n";
        id = -1;
        return;
    }
    *this = hospital::appointmentsList[id];
    return;
}

/************************************************ambulance Decleration**************************************************/

ambulance::ambulance()
{
    id = -1;
    add.strToAdd("`````");
    D.id = -1;
}
void ambulance::fillMap()
{
    fstream f;
    f.open("./data/ambulances.csv", ios::in);
    string temp;
    // skipping the first row containing column headers;
    getline(f >> ws, temp);
    // analyzing each entry afterwards;
    while (getline(f >> ws, temp))
    {
        ambulance a;
        // creating a string stream object to read from string 'temp';
        stringstream s(temp);
        string s1, s5, s6, s7;
        // reading from the string stream object 's';
        getline(s, s1, ',');
        getline(s, a.model, ',');
        getline(s, a.manufacturer, ',');
        getline(s, a.vrn, ',');
        getline(s, s5, ',');
        getline(s, s6, ',');
        getline(s, s7, ',');
        a.id = strToNum(s1);
        a.idle = (s5 == "Y");
        if (!a.idle)
        {
            a.add.strToAdd(s6);
            a.D = hospital::driversList[strToNum(s7)];
        }
        hospital::ambulancesList[a.id] = a;
    }
    f.close();
    return;
}
void ambulance::saveMap()
{
    fstream f;
    f.open("./data/temp.csv", ios::out);
    // `le first line conataining column headers:
    f << "ambulanceId,model,manufacturer,vrn,idle?,headedTowards(ifNotIdle),driverID(ifNotIdle)\n";
    for (auto i : hospital::ambulancesList)
    {
        f << i.second.id << "," << i.second.model << "," << i.second.manufacturer << "," << i.second.vrn
          << "," << (i.second.idle ? ("Y,NA,NA\n") : ("N," + i.second.add.addToStr() + ","));
        if (!(i.second.idle))
        {
            f << i.second.D.id << endl;
        }
    }
    f.close();
    remove("./data/ambulances.csv");
    rename("./data/temp.csv", "./data/ambulances.csv");
    return;
}
void ambulance::addAmbulance()
{
    if (hospital::ambulancesList.size() == hospital::ambulancesLimit)
    {
        cout << "\n\nAmbulances limit reached, can't add more!\n\n";
        return;
    }
    // getting the basic details of the ambulance from the user side;
    cout << "\nEnter Model of the ambulance:\n";
    getline(cin >> ws, model);
    cout << "\nEnter Manufacturer Name of the ambulance:\n";
    getline(cin >> ws, manufacturer);
    cout << "\nEnter Vehicle Registration Number of the ambulance:\n";
    getline(cin >> ws, vrn);
    if (hospital::ambulancesList.rbegin() != hospital::ambulancesList.rend())
        id = ((hospital::ambulancesList.rbegin())->first) + 1;
    else
        id = 1;
    hospital::ambulancesList[id] = *this;

    // creating a fstream object to read/write from/to files;
    fstream f;
    // creating a record in ambulancesHistory.csv;
    f.open("./data/ambulancesHistory.csv", ios::app);
    f << model << "," << manufacturer << "," << vrn << ",Y" << endl;
    f.close();

    cout << "\n"
         << model << " by " << manufacturer << " added successfully!\n";
    cout << "Its ID is: " << id << "\n";
}
void ambulance::printDetails()
{
    if (id == -1)
        return;
    cout << "Details:\n";
    cout << "ID              : " << id << "\n";
    cout << "Manufacturer    : " << manufacturer << "\n";
    cout << "Model           : " << model << "\n";
    cout << "Reg Number      : " << vrn << "\n";
    cout << "Idle?           : " << ((idle) ? "Y" : "N") << "\n";
    if (!idle)
    {
        cout << "Going to Address: ";
        add.print();
        cout << "Driver ID       : " << D.id << "\n";
    }
    return;
}
void ambulance::printDetailsFromHistory(string extraDetails)
{
    if (id == -1)
        return;
    if (extraDetails == "")
    {
        fstream f;
        f.open("./data/driversHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            ambulance a;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4;
            // reading from the string stream object 's';
            getline(s, a.model, ',');
            getline(s, a.manufacturer, ',');
            getline(s, s4, ',');

            if (vrn == s4)
            {
                getline(s, extraDetails, ',');
            }
        }
        f.close();
    }
    cout << "Model           : " << model << "\n";
    cout << "Manufacturer    : " << manufacturer << "\n";
    cout << "Reg. Number     : " << vrn << "\n";
    cout << "Still owned?    : " << extraDetails << "\n";
    return;
}
void ambulance::getDetails(int rec)
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by ID\n[2]: Filter by Model\n[3]: Filter by Vehicle Reg. Number\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2 && opt != 3)
        cout << "option 1, 2 or 3?\n", cin >> opt;
    // 1: Filter by ID;
    if (opt == 1)
    {
        int reqId;
        cout << "\nEnter ID:\n";
        cin >> reqId;
        if (hospital::ambulancesList.find(reqId) != hospital::ambulancesList.end())
            *this = hospital::ambulancesList[reqId];
        else
            cout << "\nNo matching record found!\n";
    }
    // 2: Filter by model;
    else if (opt == 2)
    {
        string reqModel;
        cout << "Model:\n";
        getline(cin >> ws, reqModel);
        vector<ambulance> matchingRecords;
        for (auto i : hospital::ambulancesList)
        {
            if (i.second.model == reqModel)
                matchingRecords.push_back(i.second);
        }
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (auto i : matchingRecords)
            i.printDetails();
        char tt = 'N';
        if (matchingRecords.size() > rec)
        {
            do
            {
                int reqId;
                cout << "\nEnter the ID of the required ambulance: ";
                cin >> reqId;
                if (hospital::ambulancesList.find(reqId) != hospital::ambulancesList.end())
                    *this = hospital::ambulancesList[reqId];
                else
                {
                    cout << "\nInvalid ID!\nTry again? (Y = Yes || N = No)\n";
                    cin >> tt;
                    while (tt != 'Y' || tt != 'N')
                        cout << "Y or N?\n", cin >> tt;
                }
            } while (tt == 'Y');
        }
    }
    // 3: Filter by vehicle reg. number;
    else if (opt == 3)
    {
        string reqVRN;
        cout << "Enter the vehicle reg. number of ambulance required:\n";
        getline(cin >> ws, reqVRN);
        for (auto i : hospital::ambulancesList)
        {
            if (i.second.vrn == reqVRN)
            {
                *this = i.second;
                return;
            }
        }
        // if a record is found, it's details will be stored in the driver class object that called this function,
        // and the control is returned;
        // else:
        cout << "\nNo matching record found!\n";
    }
    return;
}
void ambulance::getDetailsFromHistory()
{
    int opt = 0;
    cout << "\nOPTIONS:\n[1]: Filter by model\n[2]: Filter by vehicle reg. number\n\n";
    cin >> opt;
    while (opt != 1 && opt != 2)
        cout << "option 1 or 2?\n", cin >> opt;

    // 1: Filter by name;
    if (opt == 1)
    {
        string reqModel;
        cout << "Model:\n";
        getline(cin >> ws, reqModel);
        vector<ambulance> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/ambulancesHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            ambulance a;
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4;
            bool owned;
            // reading from the string stream object 's';
            getline(s, a.model, ',');

            if (a.model == reqModel)
            {

                getline(s, a.manufacturer, ',');
                getline(s, a.vrn, ',');
                getline(s, s4, ',');
                a.id = 0;
                matchingRecords.push_back(a);
                extraDetails.push_back(s4);
            }
        }
        f.close();
        cout << "\n";
        cout << matchingRecords.size() << " matching record(s) found!\n";
        for (int i = 0; i < matchingRecords.size(); i++)
            matchingRecords[i].printDetailsFromHistory(extraDetails[i]);
    }
    // 2: Filter by vrn;
    else if (opt == 2)
    {
        string reqVRN;
        cout << "Enter the vehicle reg. number of ambulance required:\n";
        getline(cin >> ws, reqVRN);
        vector<ambulance> matchingRecords;
        vector<string> extraDetails;
        fstream f;
        f.open("./data/ambulancesHistory.csv", ios::in);
        string temp;
        // skipping the first row containing column headers;
        getline(f >> ws, temp);
        // analyzing each entry afterwards;
        while (getline(f >> ws, temp))
        {
            // creating a string stream object to read from string 'temp';
            stringstream s(temp);
            string s4;
            // reading from the string stream object 's';
            getline(s, model, ',');
            getline(s, manufacturer, ',');
            getline(s, vrn, ',');
            if (vrn == reqVRN)
            {
                getline(s, s4, ',');
                id = 0;
                printDetailsFromHistory(s4);
                return;
            }
        }
        f.close();
        // if a record is found, it's details will be stored in the ambulance class object that called this function,
        // and the control is returned;
        // else:
        cout << "\nNo matching record found!\n";
    }
    return;
}
void ambulance::send()
{

    //*************picking an idle ambulance*************;

    bool gotOne = 0;
    for (auto i : hospital::ambulancesList)
    {
        if (i.second.idle)
        {
            *this = i.second;
            gotOne = 1;
            break;
        }
    }
    if (!gotOne)
    {
        cout << "No, idle ambulance found!"
             << "\n";
        return;
    }
    //*************  picking a free driver  *************;

    gotOne = 0;
    for (auto i : hospital::driversList)
    {
        if (i.second.idle)
        {
            D = i.second;
            gotOne = 1;
            break;
        }
    }
    if (!gotOne)
    {
        cout << "No, idle driver found!"
             << "\n";
        return;
    }

    idle = 0;

    cout << "Enter destination address:\n";
    add.takeInput();

    // updating status of ambulance;
    hospital::ambulancesList[id] = *this;

    // updating status of driver;
    hospital::driversList[D.id].idle = 0;

    cout << model << " by " << manufacturer << " sent with driver " << D.firstName << " " << D.lastName << " (ID = " << D.id << ") successfully!\n";
    return;
}
void ambulance::reportArrival()
{
    getDetails();

    // updating status of driver;
    // note that if we first update the status of ambulance we will lose the identity of it's driver;
    // and then there will be no way to update the status of the driver;
    hospital::driversList[D.id].idle = 1;

    // updating status of ambulance;
    hospital::ambulancesList[id].idle = 1;
    hospital::ambulancesList[id].add.strToAdd("`````");
    driver d;
    hospital::ambulancesList[id].D = d;

    cout << "\nStatus updated successfully!\n\n";
    return;
}
void ambulance::removeAmbulance()
{
    cout << "\nSearch for the ambulance you want to remove.\n";
    getDetails();
    if (id == -1)
        return;
    if (!idle)
    {
        cout << "\nSorry, the ambulance you selected to remove is NOT currently idle.\nOnly idlers can be removed.\n\n";
        return;
    }
    hospital::doctorsList.erase(id);

    string s, temp;
    stringstream str;
    fstream f, fout;
    str << model << "," << manufacturer << "," << vrn << ",Y\n";
    getline(str, s);
    f.open("./data/ambulancesHistory.csv", ios::in);
    fout.open("./data/temp.csv", ios::out);
    while (getline(f, temp))
    {
        if (temp == s)
        {
            fout << model << "," << manufacturer << "," << vrn << ",N"
                 << "\n";
        }
        else
            fout << temp << "\n";
    }
    f.close();
    fout.close();
    s.erase();
    temp.erase();
    remove("./data/ambulancesHistory.csv");
    rename("./data/temp.csv", "./data/ambulancesHistory.csv");
    cout << model << " by " << manufacturer << " (VRN = " << vrn << ") removed successfully!\n";
    return;
}

/**********************************address decleration******************************/

address::address()
{
    line1 = "";
    line2 = "";
    city = "";
    state = "";
    pinCode = "";
    country = "";
}
void address::takeInput()
{
    cout << "\nEnter address:\n";
    cout << "\nLine 1:\n";
    getline(cin >> ws, line1);
    cout << "\nLine 2 (optional):\n";
    getline(cin, line2);
    cout << "\nCity:\n";
    getline(cin >> ws, city);
    cout << "\nState:\n";
    getline(cin >> ws, state);
    cout << "\nPin Code:\n";
    getline(cin >> ws, pinCode);
    cout << "\nCountry:\n";
    getline(cin >> ws, country);
    return;
}
void address::print()
{
    cout << "Line 1: " << line1 << "\n";
    if (line2 != "")
        cout << "                  "
             << "Line 2: " << line2 << "\n";
    cout << "                  ";
    cout << "City: " << city << "\n";
    cout << "                  ";
    cout << "State: " << state << "\n";
    cout << "                  ";
    cout << "Pin Code: " << pinCode << "\n";
    cout << "                  ";
    cout << "Country: " << country << "\n";
    return;
}
string address::addToStr()
{
    stringstream s;
    s << line1 << "`" << line2 << "`" << city << "`" << state << "`" << pinCode << "`" << country;
    string add;
    getline(s, add);
    for (auto &i : add)
        if (i == ',')
            i = '^';
    return add;
}
void address::strToAdd(string str)
{
    stringstream s(str);

    getline(s, line1, '`');
    for (auto &i : line1)
        if (i == '^')
            i = ',';

    getline(s, line2, '`');
    for (auto &i : line2)
        if (i == '^')
            i = ',';

    getline(s, city, '`');
    getline(s, state, '`');
    getline(s, pinCode, '`');
    getline(s, country, '`');
    return;
}
/*============================================================================*/
/*****************************Functions Decleration****************************/
/*============================================================================*/
void appointmentsMenu()
{
    bool exit = false;
    while (!exit)
    {
        int purpose = 0;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\nSelect an option:\n\n";

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "./HOME/APPOINTMENTS\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "[01] : Book an appointment\n";
        cout << "[02] : Get appointment details\n";
        cout << "[03] : Show all appointments\n\n";
        cout << "[-1] : Back\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
        cout << "Enter your choice: ";
        cin >> purpose;

        if (purpose == 1)
        {
            appointment a;
            a.book();
        }
        else if (purpose == 2)
        {
            appointment a;
            a.getDetails();
            a.printDetails();
        }
        else if (purpose == 3)
        {
            hospital::printAppointments();
        }
        else if (purpose == -1)
        {
            exit = true;
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

        cout << endl;
        cout << "\nPress ENTER to continue...\n";
        cout << endl;

        getchar();
    }
    return;
}

void patientsMenu()
{
    bool exit = false;
    while (!exit)
    {
        int purpose = 0;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\nSelect an option:\n\n";

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "./HOME/PATIENTS\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "[01] : Register a new patient\n";
        cout << "[02] : Get patient details\n";
        cout << "[03] : Hospitalize a registered patient\n";
        cout << "[04] : Report a patient's death\n";
        cout << "[05] : Discharge a patient or their body\n";
        cout << "[06] : Fetch patient details from history\n";
        cout << "[07] : Get details of all registered patients\n\n";
        cout << "[-1] : Back\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
        cout << "Enter your choice: ";
        cin >> purpose;

        if (purpose == 1)
        {
            patient p;
            p.addPerson();
        }
        else if (purpose == 2)
        {
            patient p;
            p.getDetails(1);
            p.printDetails();
        }
        else if (purpose == 3)
        {
            patient p;
            p.hospitalize();
        }
        else if (purpose == 4)
        {
            patient p;
            p.reportADeath();
        }
        else if (purpose == 5)
        {
            patient p;
            p.removePerson();
        }
        else if (purpose == 6)
        {
            patient p;
            p.getDetailsFromHistory();
        }
        else if (purpose == 7)
        {
            hospital::printPatients();
        }
        else if (purpose == -1)
        {
            exit = true;
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

        cout << endl;
        cout << "\nPress ENTER to continue...\n";
        cout << endl;

        getchar();
    }
    return;
}

void doctorsMenu()
{
    bool exit = false;
    while (!exit)
    {
        int purpose = 0;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\nSelect an option:\n\n";

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "./HOME/DOCTORS\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "[01] : Register a new doctor\n";
        cout << "[02] : Get doctor details\n";
        cout << "[03] : Remove a doctor\n";
        cout << "[04] : Fetch doctor details from history\n";
        cout << "[05] : Get details of all registered doctors\n\n";
        cout << "[-1] : Back\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
        cout << "Enter your choice: ";
        cin >> purpose;

        if (purpose == 1)
        {
            doctor d;
            d.addPerson();
        }
        else if (purpose == 2)
        {
            doctor d;
            d.getDetails(1);
            d.printDetails();
        }
        else if (purpose == 3)
        {
            doctor d;
            d.removePerson();
        }
        else if (purpose == 4)
        {
            doctor d;
            d.getDetailsFromHistory();
        }
        else if (purpose == 5)
        {
            hospital::printDoctors();
        }
        else if (purpose == -1)
        {
            exit = true;
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

        cout << endl;
        cout << "\nPress ENTER to continue...\n";
        cout << endl;

        getchar();
    }
    return;
}

void nursesMenu()
{
    bool exit = false;
    while (!exit)
    {
        int purpose = 0;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\nSelect an option:\n\n";

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "./HOME/NURSES\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "[01] : Register a new nurse\n";
        cout << "[02] : Get nurse details\n";
        cout << "[03] : Remove a nurse\n";
        cout << "[04] : Fetch nurse details from history\n";
        cout << "[05] : Get details of all registered nurses\n\n";
        cout << "[-1] : Back\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
        cout << "Enter your choice: ";
        cin >> purpose;

        if (purpose == 1)
        {
            nurse n;
            n.addPerson();
        }
        else if (purpose == 2)
        {
            nurse n;
            n.getDetails(1);
            n.printDetails();
        }
        else if (purpose == 3)
        {
            nurse n;
            n.removePerson();
        }
        else if (purpose == 4)
        {
            nurse n;
            n.getDetailsFromHistory();
        }
        else if (purpose == 5)
        {
            hospital::printNurses();
        }
        else if (purpose == -1)
        {
            exit = true;
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

        cout << endl;
        cout << "\nPress ENTER to continue...\n";
        cout << endl;

        getchar();
    }
    return;
}

void driversMenu()
{
    bool exit = false;
    while (!exit)
    {
        int purpose = 0;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\nSelect an option:\n\n";

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "./HOME/DRIVERS\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "[01] : Register a new driver\n";
        cout << "[02] : Get driver details\n";
        cout << "[03] : Remove a driver\n";
        cout << "[04] : Fetch driver details from history\n";
        cout << "[05] : Get details of all registered drivers\n\n";
        cout << "[-1] : Back\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
        cout << "Enter your choice: ";
        cin >> purpose;

        if (purpose == 1)
        {
            driver d;
            d.addPerson();
        }
        else if (purpose == 2)
        {
            driver d;
            d.getDetails(1);
            d.printDetails();
        }
        else if (purpose == 3)
        {
            driver d;
            d.removePerson();
        }
        else if (purpose == 4)
        {
            driver d;
            d.getDetailsFromHistory();
        }
        else if (purpose == 5)
        {
            hospital::printDrivers();
        }
        else if (purpose == -1)
        {
            exit = true;
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

        cout << endl;
        cout << "\nPress ENTER to continue...\n";
        cout << endl;

        getchar();
    }
    return;
}

void ambulancesMenu()
{
    bool exit = false;
    while (!exit)
    {
        int purpose = 0;
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "\nSelect an option:\n\n";

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "./HOME/AMBULANCES\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "[01] : Add an ambulance\n";
        cout << "[02] : Send an ambulance\n";
        cout << "[03] : Get ambulance details\n";
        cout << "[04] : Report ambulance arrival\n";
        cout << "[05] : Remove an ambulance\n";
        cout << "[06] : Fetch ambulance details from history\n";
        cout << "[07] : Get details of all registered ambulances\n\n";
        cout << "[-1] : Back\n";
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
        cout << "Enter your choice: ";
        cin >> purpose;

        if (purpose == 1)
        {
            ambulance a;
            a.addAmbulance();
        }
        else if (purpose == 2)
        {
            ambulance a;
            a.send();
        }
        else if (purpose == 3)
        {
            ambulance a;
            a.getDetails(1);
            a.printDetails();
        }
        else if (purpose == 4)
        {
            ambulance a;
            a.reportArrival();
        }
        else if (purpose == 5)
        {
            ambulance a;
            a.removeAmbulance();
        }
        else if (purpose == 6)
        {
            ambulance a;
            a.getDetailsFromHistory();
        }
        else if (purpose == 7)
        {
            hospital::printAmbulances();
        }
        else if (purpose == -1)
        {
            exit = true;
            break;
        }
        else
        {
            cout << "Invalid choice!\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

        cout << endl;
        cout << "\nPress ENTER to continue...\n";
        cout << endl;

        getchar();
    }
    return;
}