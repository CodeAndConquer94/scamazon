#include "Date.h"
#include <array> 
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <ctime>

using namespace std;

Date::Date() {
    std::time_t now = std::time(nullptr);        
    std::tm* local = std::localtime(&now);        

    month = local->tm_mon + 1; 
    day   = local->tm_mday;
    year  = local->tm_year + 1900; 
}

Date::Date( int mn, int dy, int yr )
{

    if ( mn > 0 && mn <= monthsPerYear)
        month = mn;
    else
        throw std::invalid_argument( "month must be 1-12" );

    year = yr;
    day = checkDay( dy ); 
    cout << endl;
}


Date::Date(string& dtString){
    istringstream iss(dtString);
    string token;

    getline(iss, token, '/');
    month = stoi(token);
    getline(iss, token, '/');
    day = stoi(token);
    getline(iss, token, '/');
    year = stoi(token);
    }

string Date::dateString() 
{ 
    // Returns date formatted as MM/DD/YYYY string
    return to_string(month) + "/" +  to_string(day) + "/" + to_string(year);
} 

Date::~Date()
{

} 
unsigned int Date::checkDay( int testDay )
{
    // Validates day value against month/year with leap year handling
    static const array< int, monthsPerYear + 1 > daysPerMonth =
        {0, 31, 28, 31, 30,31, 30, 31, 31, 30,31,30, 31};

    if ( testDay> 0 && testDay <= daysPerMonth[ month ] )
        return testDay;
    if ( month == 2 && testDay ==29 && ( year % 400 == 0 ||
       ( year% 4 == 0&& year% 100 != 0 ) ) )
        return testDay;

    throw std::invalid_argument( "Invalid day for current month and year");
} 


bool Date::operator>(const Date& other) const {
    if (year > other.year) return true;
    if (year < other.year) return false;

    if (month > other.month) return true;
    if (month < other.month) return false;

    return day > other.day;
}
