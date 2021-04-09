//
//  main.cpp
//  simple_database
//
//  Created by MacBook on 21.07.2020.
//  Copyright © 2020 MacBook. All rights reserved.
//

#include <iostream>
#include <map>
#include <set>
#include <exception>
#include <iomanip>
#include <sstream>

using namespace std;

class Date {
public:
    Date() {
        year = 0;
        month = 0;
        year = 0;
    }
    Date(const int& new_year, const int& new_month, const int& new_day) {
        if (new_month > 12 || new_month  <= 0)
            throw runtime_error("Month value is invalid: " + to_string(new_month));
        if (new_day > 31 || new_day < 1)
            throw runtime_error("Day value is invalid: " + to_string(new_day));
        year = new_year;
        month = new_month;
        day = new_day;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
    
private:
    int year;
    int month;
    int day;
};


bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth())
        return lhs.GetDay() < rhs.GetDay();
    else if (lhs.GetYear() == rhs.GetYear())
        return lhs.GetMonth() < rhs.GetMonth();
    return lhs.GetYear() < rhs.GetYear();
}

istream& operator>> (istream& stream, Date& date) {
    int day, month, year;
    string s;
    if (stream) {
        stream >> s;
        stringstream sdate(s);
        sdate >> year;
        if (sdate.peek() != '-') {
            throw runtime_error("Wrong date format: " + s);
        }
        sdate.ignore(1);
        sdate >> month;
        if (sdate.peek() != '-') {
            throw runtime_error("Wrong date format: " + s);
        }
        sdate.ignore(1);
        if (!isdigit(sdate.peek()) && sdate.peek() != '+')
            throw runtime_error("Wrong date format: " + s);
        sdate >> day;
        if (sdate.peek() != EOF) {
            throw runtime_error("Wrong date format: " + s);
        }
        date = {year, month, day};
    }
    return stream;
}

ostream& operator<< (ostream& stream, Date date) {
    stream << setfill('0') << setw(4) << date.GetYear() << '-' << setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        database[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (database[date].count(event)) {
            database[date].erase(event);
            return true;
        }
        return false;
    }
    int  DeleteDate(const Date& date) {
        int num;
        try {
            num = database[date].size();
            database.erase(date);
        }catch (runtime_error& ex) {
            cout << ex.what();
        }
        return num;
    }

    void Find(const Date& date) const {
        if (database.count(date))
            for (const string& event: database.at(date))
                cout << event << endl;
    }
  
    void Print() const {
        for (auto item: database)
            for (const string& event: database.at(item.first))
                cout << item.first << " " << event << endl;
    }
private:
    map <Date, set<string>> database;
};

void CommandTry (const string& command, Database& db) {
    stringstream stream(command);
    string op;
    stream >> op;
    stream.ignore(1);
    if (op != "Add" && op != "Print" && op != "Del" && op != "Find" && op != "")
        throw runtime_error("Unknown command: " + command);
    else if (op == "Print")
        db.Print();
    else {
        Date date;
        try {
            stream >> date;
            if (op == "Add") {
                string event;
                stream.ignore(1);
                stream >> event;
                db.AddEvent(date, event);
            }
            else if (op == "Find")
                db.Find(date);
            else if (op == "Del") {
                if (stream.peek() == ' ') {
                    stream.ignore(1);
                    string event;
                    stream >> event;
                    if (db.DeleteEvent(date, event))
                        cout << "Deleted successfully" << endl;
                    else
                        cout << "Event not found" << endl;
                }
                else {
                    int n = db.DeleteDate(date);
                    cout << "Deleted " << n << " events" << endl;
                }
            }
        } catch (runtime_error& ex) {
            cout << ex.what();
        }
    }
}

int main() {
    Database db;
    string command;
    while (getline(cin, command)) {
        try {
            CommandTry(command, db);
        } catch (runtime_error& ex) {
            cout << ex.what() << endl;
        }
    }
    return 0;
}
