#ifndef DATE_H
#define DATE_H
#include <string>

using namespace std;
    
class Date { 
    private: 
        unsigned int month; 
        unsigned int day;
        unsigned int year;
        unsigned int checkDay( int );
    
    public: 
        bool operator>(const Date& other) const;
        static const int monthsPerYear = 12; 
        Date();
        explicit Date( int mm, int dd, int yy );
        Date(string& dtString);
        string dateString();
        ~Date();

    
};
        

#endif