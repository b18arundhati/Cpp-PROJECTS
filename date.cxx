#include "date.h"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

const char* DateFormat::getdF() {
	return dateFormat;
}

const char* DateFormat::getmF() {
	return monthFormat;
}

const char* DateFormat::getyF() {
	return yearFormat;
}

DateFormat::DateFormat(const char* dateFormat, const char* monthFormat, const char* yearFormat){
	
	if (dateFormat != NULL){
		this->dateFormat = new char[strlen(dateFormat)+1];
		strcpy(this->dateFormat,dateFormat);
	}
	else{
		/*this->dateFormat = new char[2];
		this->dateFormat[0] = '0';*/
		//this->dateFormat = "";
	}
	if (monthFormat != NULL){
		this->monthFormat = new char[strlen(monthFormat)+1];
		strcpy(this->monthFormat,monthFormat);
	}
	else{
		/*this->monthFormat = new char[2];
		this->monthFormat[0] = '0';*/
		//this->monthFormat = "";
	}
	if (yearFormat != NULL){
		this->yearFormat = new char[strlen(yearFormat)+1];
		strcpy(this->yearFormat,yearFormat);
	}
	else{
		/*this->yearFormat = new char[2];
		this->yearFormat[0] = '0';*/
		//this->yearFormat = "";
	}
	
}//input arrays must be null-terminated 

// Single C-string format where dateFormat, monthFormat, and yearFormat are separated by ’-’
// Example: "dd-mmm-yy", "d-mm-yyyy", etc
DateFormat::DateFormat(const char* format){
	int i = 0, j = 0;
	while (format[j] != '-'){
		j++;
	}

	

	if (j-i == 1)//format "d-..."
		{
			dateFormat = new char[2];
			dateFormat[0] = 'd';
			dateFormat[1] = '\0';
		}
	else if (j-i == 2)//format "dd-..."
		{
			dateFormat = new char[3];
			dateFormat[0] = 'd';
			dateFormat[1] = 'd';
			dateFormat[2] = '\0';
		}
	else if (j-i == 0){
		/*dateFormat = new char[2];
		dateFormat[0] = '0';*/
		dateFormat = new char[1];
		dateFormat[0] = '\0';
	}
	else throw exception();
	
	

	i = ++j;
	while (format[j] != '-'){
		j++;
	}

	if (j-i == 2)//format "..-mm-.."
		{
			monthFormat = new char[3];
			monthFormat[0] = 'm';
			monthFormat[1] = 'm';
			monthFormat[2] = '\0';
		}
	else if (j-i == 3)//"..-mmm-.."
		{
			monthFormat = new char[4];
			monthFormat[0] = 'm';
			monthFormat[1] = 'm';
			monthFormat[2] = 'm';
			monthFormat[3] = '\0';
		}
	else if (j-i == 0){
		/*monthFormat = new char[2];
		monthFormat[0] = '0';*/
		monthFormat = new char[1];
		monthFormat[0] = '\0';
	}
	else throw exception();
	
	i = ++j;
	while (format[j] == 'y'){
		j++;	
	}

	if (j-i == 2)
		{
			yearFormat = new char[3];
			yearFormat[0] = 'y';
			yearFormat[1] = 'y';
			yearFormat[2] = '\0';
		}
	else if (j-i == 4)
		{
			yearFormat = new char[5];
			yearFormat[0] = 'y';
			yearFormat[1] = 'y';
			yearFormat[2] = 'y';
			yearFormat[3] = 'y';
			yearFormat[4] = '\0';
		}
	else if (j-i == 0){
		/*yearFormat = new char[2];
		yearFormat[0] = '0';*/
		yearFormat = new char[1];
		yearFormat[0] = '\0';
	}
	else throw exception();

}//also handle the invalid cases here--->to be done ?? exceptions need to be raised??

// Default DateFormat created as "dd-mmm-yy"
DateFormat::DateFormat(){
	
	dateFormat = new char[3];
	monthFormat = new char[4];
	yearFormat = new char[3];

	dateFormat[0] = 'd';
	dateFormat[1] = 'd';
	dateFormat[2] = '\0';
	monthFormat[0] = 'm';
	monthFormat[1] = 'm';
	monthFormat[2] = 'm';
	monthFormat[3] = '\0';
	yearFormat[0] = 'y';
	yearFormat[1] = 'y';
	yearFormat[2] = '\0';

}

DateFormat::~DateFormat(){
	delete dateFormat;
	delete monthFormat;
	delete yearFormat;
}

//copy assignment operator
DateFormat& DateFormat::operator=(const DateFormat& copyformat){
	this->dateFormat = copyformat.dateFormat;
	this->monthFormat = copyformat.monthFormat;
	this->yearFormat = copyformat.yearFormat;
	return (*this);
}

// Construct a Date from (d, m, y)
Date::Date(Day d, Month m, Year y) throw(invalid_argument,domain_error,out_of_range): curr_day{d},curr_month{m},curr_year{y}{
 // One or more of the arguments d or m is/are invalid (27, 13, 13) ---> invalid_argument
 // (d, m, y) as a triplet does not define a valid date (30, 2, 61) --->domain_error
 // Date is out of range (4, 7, 2053) --->out_of_range

		if (d < 1 || d > 31 || m > 12 || m < 1 || y < 0)
  			throw invalid_argument("One or more of the arguments are invalid. \n");

  		if ((m == 4 || m == 6 || m == 9 || m == 11) && (d > 30))// || ( m == 2 && ((m % 4 == 0 && m % 100 != 0) || (m % 400 == 0)) && (d > 29)) || ( m == 2 && (!(m % 4 == 0 && m % 100 != 0) || (m % 400 == 0)) && (d > 28)))
  			throw domain_error("Entered values not in the domain suitable for date construction.");

  		if (curr_month == Feb && leapYear() && curr_day > 29)
			throw domain_error("Entered values not in the domain suitable for date construction.");

		if (curr_month == Feb && !leapYear() && curr_day > 28)
			throw domain_error("Entered values not in the domain suitable for date construction.");
	 		
		if (y > 2049 || y < 1950)
  			throw out_of_range("Entered year values are beyond the defined period. \n");
  		
}// but the object is still created if an exception is thrown --- > then???

// date in string format -- to be parsed as static format member
// "13-03-77" for "dd-mm-yy"
// "2-7-2018" for "d-m-yyyy"
Date::Date(const char* date) throw(invalid_argument, domain_error, out_of_range){
	
	if (date[0] == '-')
		throw invalid_argument("One or more of the arguments are invalid. \n");//0 for date not acceptable

	curr_day = static_cast<Day>(atoi(date));

	int index = 0;
	while (date[index] != '-'){
		index++;
	}	

	curr_month = static_cast<Month>(atoi(&date[++index]));

	while (date[index] != '-'){
		index++;
	}

	int yr = atoi(&date[++index]);
	if (yr >= 0 && yr <= 49)
		yr += 2000;
	else if (yr >= 50 && yr <= 99)
		yr += 1900;

	curr_year = static_cast<Year>(yr);

	//cout<<"\\\\\\"<<curr_day<<"-"<<curr_month<<"-"<<curr_year<<endl;

	if (curr_day < 1 || curr_day > 31 || curr_month > 12 || curr_month < 1 || curr_year < 0)
  		throw invalid_argument("One or more of the arguments are invalid. \n");

  	if ((curr_month == 4 || curr_month == 6 || curr_month == 9 || curr_month == 11) && (curr_day > 30))
  		throw domain_error("Entered values not in the domain suitable for date construction.");

  	if (curr_month == Feb && leapYear() && curr_day > 29)
		throw domain_error("Entered values not in the domain suitable for date construction.");

	if (curr_month == Feb && !leapYear() && curr_day > 28)
		throw domain_error("Entered values not in the domain suitable for date construction.");  		
	 		
	if (curr_year > 2049 || curr_year < 1950)
  		throw out_of_range("Entered year values are beyond the defined period. \n");

}// in case of an exception object still exists --> then??

// Default Constructor - construct ’today’ (get from system)
Date::Date() throw(domain_error, out_of_range){
	time_t nowtime = time(0);
  	struct tm * nowtimestruct;

  	nowtimestruct = localtime (&nowtime);

  	curr_day = static_cast<Day>(nowtimestruct->tm_mday);
  	curr_year = nowtimestruct->tm_year + 1900;
  	curr_month = static_cast<Month>(nowtimestruct->tm_mon + 1);

  	  	
  	if ((curr_month == 4 || curr_month == 6 || curr_month == 9 || curr_month == 11) && (curr_day > 30)) //|| ( curr_month == 2 && ((curr_month % 4 == 0 && curr_month % 100 != 0) || (curr_month % 400 == 0)) && (curr_day > 29)) || ( curr_month == 2 && (!(curr_month % 4 == 0 && curr_month % 100 != 0) || (curr_month % 400 == 0)) && (curr_day > 28)))
  		throw domain_error("Entered values not in the domain suitable for date construction.");

  	if (curr_month == Feb && leapYear() && curr_day > 29)
		throw domain_error("Entered values not in the domain suitable for date construction.");

	if (curr_month == Feb && !leapYear() && curr_day > 28)
		throw domain_error("Entered values not in the domain suitable for date construction.");
	 		
	if (curr_year > 2049 || curr_year < 1950)
  		throw out_of_range("Entered year values are beyond the defined period. \n");
}

Date::Date(const Date& copydate){ // Copy Constructor
	this->curr_day = copydate.curr_day;
	this->curr_month = copydate.curr_month;
	this->curr_year = copydate.curr_year;
	// format is a static member and hence does not need to be copied	
}

Date::~Date(){} // destructor??

Date& Date::operator=(const Date& copydate){//BASIC ASSIGNMENT OPERATOR
	if (this != &copydate){
		this->curr_day = copydate.curr_day;
		this->curr_month = copydate.curr_month;
		this->curr_year = copydate.curr_year;
	
	}

	return (*this);

}// exception safe?? -check.

// UNARY ARITHMETIC OPERATORS
Date& Date::operator++(){
// Next day
	
	if (curr_day == D31){
		curr_day = D01;
		if (curr_month == Dec){
			curr_month = Jan;
			curr_year ++; // here handle the weeknum adjustment--->needs to be set to 0 or 1---> to be done
		}
		else 
			curr_month = static_cast<Month>(curr_month + 1);
	}
	else if (curr_day == D30 && (curr_month == Apr || curr_month == Jun || curr_month == Sep || curr_month == Nov)){
		curr_day = D01;
		curr_month = static_cast<Month>(curr_month + 1) ;
	}
	else if (curr_month == Feb && curr_day == D28){
		if (leapYear()){
			curr_day = D29;
		}else{
			curr_day = D01;
			curr_month = static_cast<Month>(curr_month + 1) ;
		}
	}
	else if (curr_month == Feb && curr_day == D29){
		curr_day = D01;
		curr_month = static_cast<Month>(curr_month + 1) ;
	}
	else{
		curr_day = static_cast<Day>(curr_day + 1);
	}

	return (*this);
}


Date& Date::operator++(int){
// Same day next week
	
	if ((curr_month == Apr || curr_month == Jun || curr_month == Sep || curr_month == Nov) && (curr_day + 7 > 30)){
		curr_month = static_cast<Month>(curr_month + 1) ;
		curr_day = static_cast<Day>((curr_day + 7) % 30);
	}
	else if ((curr_month == Jan || curr_month == Mar || curr_month == May || curr_month == Jul || curr_month == Aug || curr_month == Oct) && (curr_day + 7 > 31)){
		curr_month = static_cast<Month>(curr_month + 1) ;
		curr_day = static_cast<Day>((curr_day + 7) % 31);
	}
	else if ((curr_month == Dec) && (curr_day + 7 > 31)){
		curr_month = Jan;
		curr_day = static_cast<Day>((curr_day + 7) % 31);
		curr_year ++;
	}
	else if ((curr_month == Feb) && (curr_day + 7 > 28)){
		if (leapYear() && (curr_day + 7 > 29)){
			curr_month = static_cast<Month>(curr_month + 1) ;
			curr_day = static_cast<Day>((curr_day + 7) % 29);
		}
		else if (!leapYear()) {
			curr_month = static_cast<Month>(curr_month + 1) ;
			curr_day = static_cast<Day>((curr_day + 7) % 28);
		}
		else if (leapYear() && (curr_day + 7 == 29)){
			curr_day = static_cast<Day>(29);
		}
	}
	else{
		curr_day = static_cast<Day>(curr_day + 7);
	}

	return (*this);

}

Date& Date::operator--(){
// Previous day
	if (curr_day == D01){
		if (curr_month == Feb || curr_month == Apr || curr_month == Jun || curr_month == Aug || curr_month == Sep || curr_month == Nov){
			curr_day = D31;
			curr_month = static_cast<Month>(curr_month - 1);
		}
		else if (curr_month == May || curr_month == Jul || curr_month == Oct || curr_month == Dec){
			curr_day = D30;
			curr_month = static_cast<Month>(curr_month - 1);
		}
		else if (curr_month == Mar){
			if (leapYear()){
				curr_day = D29;
			}else{
				curr_day = D28;
			}
			curr_month = Feb;	
		}
		else if (curr_month == Jan){
			curr_day = D31;
			curr_month = Dec;
			curr_year -- ;
		}

	}
	else{
		curr_day = static_cast<Day>(curr_day - 1);
	}

	return (*this);
}

Date& Date::operator--(int){
// Same day previous week
	if (curr_day <= D07){
		//it will fall in the previous month
		if (curr_month == Feb || curr_month == Apr || curr_month == Jun || curr_month == Aug || curr_month == Sep || curr_month == Nov){
			curr_day = static_cast<Day>(curr_day + 31 - 7);
			curr_month = static_cast<Month>(curr_month - 1);
		}
		else if (curr_month == May || curr_month == Jul || curr_month == Oct || curr_month == Dec){
			curr_day = static_cast<Day>(curr_day + 30 - 7);
			curr_month = static_cast<Month>(curr_month - 1);
		}
		else if (curr_month == Jan){
			curr_day = static_cast<Day>(curr_day + 31 - 7);
			curr_month = Dec;
			curr_year -- ;
		}
		else if (curr_month == Mar){
			if (leapYear()){
				curr_day = static_cast<Day>(curr_day + 29 - 7);
				curr_month = Feb;
			}
			else{
				curr_day = static_cast<Day>(curr_day + 28 - 7);
				curr_month = Feb;
			}
		}
	}
	else{
		curr_day = static_cast<Day>(curr_day - 7);
		curr_month = static_cast<Month>(curr_month - 1);
	}
	return (*this);
}

// BINARY ARITHMETIC OPERATORS
unsigned int Date::operator-(const Date& subDate){
	// Number of days between otherDate and current date
	Date otherDate(subDate);

	if (this->curr_year == otherDate.curr_year){
		if (this->curr_month == otherDate.curr_month){
			if (this->curr_day > otherDate.curr_day){
				return (this->curr_day - otherDate.curr_day);
			}
			else{
				return (otherDate.curr_day - this->curr_day);
			}
		}
		else{
			//months different, year same
			if (this->curr_month > otherDate.curr_month){
				unsigned int numofdays = 0;

				//cout<<"this->curr_month= "<<this->curr_month<<"other month: "<<otherDate.curr_month<<endl;

				if (!leapYear()){
					numofdays += monthdays[otherDate.curr_month - 1] - otherDate.curr_day ;

					otherDate.curr_month = static_cast<Month>(otherDate.curr_month + 1);
					while (otherDate.curr_month != this->curr_month){
						//cout<<"this->curr_month= "<<this->curr_month<<"other month: "<<otherDate.curr_month<<endl;

						numofdays += monthdays[otherDate.curr_month - 1];
						otherDate.curr_month = static_cast<Month>(otherDate.curr_month + 1);
					}

					//cout<<"this->curr_month= "<<this->curr_month<<"other month: "<<otherDate.curr_month<<endl;

					numofdays += this->curr_day;
					return numofdays;

				}
				else{
					numofdays += leapmonthdays[otherDate.curr_month - 1] - otherDate.curr_day ;

					otherDate.curr_month = static_cast<Month>(otherDate.curr_month + 1);
					while (otherDate.curr_month != this->curr_month){
						//cout<<"this->curr_month= "<<this->curr_month<<"other month: "<<otherDate.curr_month<<endl;

						numofdays += leapmonthdays[otherDate.curr_month - 1];
						otherDate.curr_month = static_cast<Month>(otherDate.curr_month + 1);
					}

					numofdays += this->curr_day;
					return numofdays;

				}
			}
			else{//if this->curr_month < otherDate.curr_month
				unsigned int numofdays = 0;

				Date thisdate(*this);

				if (!leapYear()){
					numofdays += monthdays[thisdate.curr_month - 1] - thisdate.curr_day ;

					thisdate.curr_month = static_cast<Month>(thisdate.curr_month + 1);
					while (thisdate.curr_month != otherDate.curr_month){

						//cout<<"thisdate.curr_month= "<<thisdate.curr_month<<"other month: "<<otherDate.curr_month<<endl;
						
						numofdays += monthdays[thisdate.curr_month - 1];
						thisdate.curr_month = static_cast<Month>(thisdate.curr_month + 1);
					}

					//cout<<"thisdate.curr_month= "<<thisdate.curr_month<<"other month: "<<otherDate.curr_month<<endl;

					numofdays += otherDate.curr_day;
					return numofdays;

				}
				else{
					numofdays += leapmonthdays[thisdate.curr_month - 1] - thisdate.curr_day ;

					thisdate.curr_month = static_cast<Month>(thisdate.curr_month + 1);
					while (thisdate.curr_month != otherDate.curr_month){
						
						//cout<<"thisdate.curr_month= "<<thisdate.curr_month<<"other month: "<<otherDate.curr_month<<endl;

						numofdays += leapmonthdays[thisdate.curr_month - 1];
						thisdate.curr_month = static_cast<Month>(thisdate.curr_month + 1);
					}

					//cout<<"thisdate.curr_month= "<<thisdate.curr_month<<"other month: "<<otherDate.curr_month<<endl;

					numofdays += otherDate.curr_day;
					return numofdays;
				}
			}
		}
	}
	else{
		//years are also different
		if (this->curr_year > otherDate.curr_year){
			unsigned int numofdays = 0;
			if (!(otherDate.leapYear())){
				numofdays += monthdays[otherDate.curr_month - 1] - otherDate.curr_day;

				while(otherDate.curr_month != Dec){

					otherDate.curr_month = static_cast<Month>(otherDate.curr_month + 1);
					numofdays += monthdays[otherDate.curr_month - 1];
				}


				/*
				for (int i = otherDate.curr_month; i < 12; i++){
					numofdays += monthdays[i];
				}*/


			}
			else{
				numofdays += leapmonthdays[otherDate.curr_month - 1] - otherDate.curr_day;

				while(otherDate.curr_month != Dec){

					otherDate.curr_month = static_cast<Month>(otherDate.curr_month + 1);
					numofdays += leapmonthdays[otherDate.curr_month - 1];
				}

				/*for (int i = otherDate.curr_month; i < 12; i++){
					numofdays += leapmonthdays[i];
				}*/
			}
				otherDate.curr_year ++;
				while (otherDate.curr_year != this->curr_year){
					if (otherDate.leapYear()){
						numofdays += 366;
					}
					else{
						numofdays += 365;
					}
					otherDate.curr_year ++;
				}

				if (leapYear()){
					for (int i = 0; i < (this->curr_month - 1); i++){
						numofdays += leapmonthdays[i];
					}
					numofdays += this->curr_day;
				}
				else{
					for (int i = 0; i < (this->curr_month - 1); i++){
						numofdays += monthdays[i];
					}
					numofdays += this->curr_day;
				}

				return numofdays;
			}
		else{//if (this->curr_year < otherDate.curr_year)
			unsigned int numofdays = 0;

			Date thisdate(*this);

			if (!leapYear()){
				numofdays += monthdays[thisdate.curr_month - 1] - thisdate.curr_day;

				while(thisdate.curr_month != Dec){

					thisdate.curr_month = static_cast<Month>(thisdate.curr_month + 1);
					numofdays += monthdays[thisdate.curr_month - 1];
				}
				/*
				for (int i = thisdate->curr_month; i < 12; i++){
					numofdays += monthdays[i];
				}*/
			}
			else{
				numofdays += leapmonthdays[thisdate.curr_month - 1] - thisdate.curr_day;

				while(thisdate.curr_month != Dec){

					thisdate.curr_month = static_cast<Month>(thisdate.curr_month + 1);
					numofdays += leapmonthdays[thisdate.curr_month - 1];
				}				

				/*for (int i = thisdate->curr_month; i < 12; i++){
					numofdays += leapmonthdays[i];
				}*/
			}

			//unsigned int tempyear = thisdate->curr_year;

			(thisdate.curr_year)++;

			while (thisdate.curr_year != otherDate.curr_year){
				if (thisdate.leapYear()){
					numofdays += 366;
				}
				else{
					numofdays += 365;
				}
				(thisdate.curr_year)++;
			}

			if (otherDate.leapYear()){
				for (int i = 0; i < (otherDate.curr_month - 1); i++){
					numofdays += leapmonthdays[i];
				}
				numofdays += otherDate.curr_day;
			}
			else{
				for (int i = 0; i < (otherDate.curr_month - 1); i++){
					numofdays += monthdays[i];
				}
				numofdays += otherDate.curr_day;
			}

			//thisdate->curr_year = tempyear;

			return numofdays;

		}

	}
}

// Day noOfDays after (before) the current date
// (sign of noOfDays decides ahead or behind)

Date Date::operator+(int noOfDays) throw(domain_error, out_of_range){
	Date tempdate = (*this);
	if (noOfDays >= 0){
		while (noOfDays > 365){
			if (tempdate.curr_month <= Feb){
				if (tempdate.leapYear()){
					noOfDays -= 366;
					tempdate.curr_year++;
				}
				else{
					noOfDays -= 365;
					tempdate.curr_year++;
				}
			}
			else{
				tempdate.curr_year++;
				if (tempdate.leapYear()){
					noOfDays -= 366;
				}
				else{
					noOfDays -= 365;
				}

			}
		}
		if (noOfDays){
			tempdate.addDays(noOfDays);
		}
	}
	else{
		noOfDays *= (-1);
		while (noOfDays > 365){
			if (tempdate.curr_month <= Feb){
				tempdate.curr_year--;
				if (tempdate.leapYear()){
					noOfDays -= 366;
				}
				else{
					noOfDays -= 365;
				}
			}
			else{
				if (tempdate.leapYear()){
					noOfDays -= 366;
					tempdate.curr_year--;
				}
				else{
					noOfDays -= 365;
					tempdate.curr_year--;
				}
			}
		}
		if (noOfDays){
			if (tempdate.curr_month <= Feb){
				tempdate.curr_year--;
				if (tempdate.leapYear()){
					tempdate.addDays(366 - noOfDays);
				}
				else{
					tempdate.addDays(365 - noOfDays);
				}
			}
			else{
				if (tempdate.leapYear()){
					tempdate.curr_year--;
					tempdate.addDays(366 - noOfDays);
				}
				else{
					tempdate.curr_year--;
					tempdate.addDays(365 - noOfDays);
				}	
			}
			
		}

	}
	if ((tempdate.curr_month == 4 || tempdate.curr_month == 6 || tempdate.curr_month == 9 || tempdate.curr_month == 11) && (tempdate.curr_day > 30))
	 	throw domain_error("Entered values not in the domain suitable for date construction.");

	if (tempdate.curr_month == 2 && tempdate.leapYear() && (tempdate.curr_day > 29)) 
		throw domain_error("Entered values not in the domain suitable for date construction.");

	if (tempdate.curr_month == 2 && !(tempdate.leapYear()) && (tempdate.curr_day > 28))
		throw domain_error("Entered values not in the domain suitable for date construction.");
 	 		
	if (tempdate.curr_year > 2049 || tempdate.curr_year < 1950)
  		throw out_of_range("Entered year values are beyond the defined period. \n");

	return tempdate;
	
}

void Date::addDays(int numdays){
	if (!leapYear()){
		if (numdays > (monthdays[this->curr_month - 1] - this->curr_day)){
			numdays -= (monthdays[this->curr_month - 1] - this->curr_day);
			this->curr_day = static_cast<Day>(monthdays[this->curr_month - 1]);
			
		}
		else{
			this->curr_day = static_cast<Day>(this->curr_day + numdays);
			return;
		}
		this->curr_month = static_cast<Month>(this->curr_month + 1);
		
		while (numdays > monthdays[this->curr_month - 1]){
			
			if (this->curr_month > 12){
				this->curr_year ++;
				this->curr_month = Jan;
				this->curr_day = D01;
				this->addDays(numdays - 1);
				return;
			}
			numdays -= monthdays[this->curr_month - 1];
			this->curr_day = static_cast<Day>(monthdays[this->curr_month - 1]);
			
			if (numdays > 0)
				this->curr_month = static_cast<Month>(this->curr_month + 1);
			
		}

		if (numdays > 0){
			if (this->curr_month > 12){
				this->curr_month = Jan;
				this->curr_year ++;
				this->curr_day = D01;
				this->addDays(numdays - 1);
				return;
			}
			this->curr_day = static_cast<Day>(numdays);//since numdays now is less than no of days in next month
		}
		

	} 
	else{//if it is a leapyear

		if (numdays > (leapmonthdays[this->curr_month - 1] - this->curr_day)){
			numdays -= (leapmonthdays[this->curr_month - 1] - this->curr_day);
			this->curr_day = static_cast<Day>(leapmonthdays[this->curr_month - 1]);
			
		}
		else{
			this->curr_day = static_cast<Day>(this->curr_day + numdays);
			return;
		}

		this->curr_month = static_cast<Month>(this->curr_month + 1);
		
		while (numdays > leapmonthdays[this->curr_month - 1]){
			
			if (this->curr_month > 12){
				this->curr_month = Jan;
				this->curr_year ++;

				this->curr_day = D01;
				this->addDays(numdays - 1);// -1 for setting D01
				return;
			}
			numdays -= leapmonthdays[this->curr_month - 1];
			this->curr_day = static_cast<Day>(leapmonthdays[this->curr_month - 1]);

			if (numdays > 0)
				this->curr_month = static_cast<Month>(this->curr_month + 1);
			}

		if (numdays > 0){
			if (this->curr_month > 12){
				this->curr_month = Jan;
				this->curr_year ++;
				this->curr_day = D01;
				this->addDays(numdays - 1);
				return;
			}
			this->curr_day = static_cast<Day>(numdays);		
		}
		
	}

}



Date::operator WeekDay() const{ 
// Cast to the day of the week of the current date
	WeekDay weekday;

	// 1st Jan 1950 = Sunday

  	Date startdate("1-1-1950");

  	unsigned int diff = startdate.operator-(*this);

  	diff = diff % 7;

  	switch(diff){
  		case 0:	
	  		weekday = Sun;
  			break;

  		case 1:
  			weekday = Mon;
  			break;

  		case 2:
  			weekday = Tue;
  			break;

  		case 3:
  			weekday = Wed;
  			break;

  		case 4:
  			weekday = Thr;
  			break;

  		case 5:
  			weekday = Fri;
  			break;

  		case 6:
  			weekday = Sat;
  			break;
  	}

  	return weekday;
}



Date::operator WeekNumber() const{
	// Cast to the week number of the year in which the current date falls
	WeekNumber weeknum;
	WeekDay weekday;

	weekday = (*this);//casting

	Date newdate(D01, Jan, this->curr_year);

  	unsigned int wknumday = newdate.operator-(*this);

  		switch (wknumday % 7){
  		case 0: 
  			if (weekday == Mon || weekday == Tue || weekday == Wed)//Jan1 was Tue, Wed or Thu
  				weeknum = static_cast<WeekNumber>(wknumday / 7 + 1);
  			else 
  				weeknum = static_cast<WeekNumber>(wknumday / 7);
  			break;

  		case 1:
  			if (weekday == Mon || weekday == Tue || weekday == Wed || weekday == Thr)
  				weeknum = static_cast<WeekNumber>(wknumday / 7 + 1);
  			else
  				weeknum = static_cast<WeekNumber>(wknumday / 7);
  			break;

  		case 2:
  			if (weekday == Mon || weekday == Tue || weekday == Wed || weekday == Thr || weekday == Fri)
  				weeknum = static_cast<WeekNumber>(wknumday / 7 + 1);
  			else
  				weeknum = static_cast<WeekNumber>(wknumday / 7);
  			break;

  		case 3:
  			if (weekday == Wed || weekday == Thr || weekday == Fri || weekday == Sat || weekday == Mon || weekday == Tue)
  				weeknum = static_cast<WeekNumber>(wknumday / 7 + 1);
  			else
  				weeknum = static_cast<WeekNumber>(wknumday / 7);
  			break;

  		case 4:
  		case 5:
  		case 6:
  			weeknum = static_cast<WeekNumber>(wknumday / 7 + 1); //always for cases 4,5,6
  			break;
  	} 

  	return weeknum;

}

Date::operator Month() const{
	// Cast to the month of the year in which the current date falls
	
	return (this->curr_month);
}

// BINARY RELATIONAL OPERATORS
bool Date::operator==(const Date& otherDate){
	if (this->curr_day == otherDate.curr_day && this->curr_month == otherDate.curr_month && this->curr_year == otherDate.curr_year)
		return true;
	else
		return false;
}

bool Date::operator!=(const Date& otherDate){
	if (this->curr_day == otherDate.curr_day && this->curr_month == otherDate.curr_month && this->curr_year == otherDate.curr_year)
		return false;
	else
		return true;
}

bool Date::operator<(const Date& otherDate){
	if (this->curr_year < otherDate.curr_year)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month < otherDate.curr_month)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month == otherDate.curr_month && this->curr_day < otherDate.curr_day)
		return true;
	else
		return false;

}

bool Date::operator<=(const Date& otherDate){
	if (this->curr_year < otherDate.curr_year)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month < otherDate.curr_month)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month == otherDate.curr_month && this->curr_day <= otherDate.curr_day)
		return true;
	else
		return false;

}

bool Date::operator>(const Date& otherDate){
	if (this->curr_year > otherDate.curr_year)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month > otherDate.curr_month)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month == otherDate.curr_month && this->curr_day > otherDate.curr_day)
		return true;
	else
		return false;

}

bool Date::operator>=(const Date& otherDate){
	if (this->curr_year > otherDate.curr_year)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month > otherDate.curr_month)
		return true;
	else if (this->curr_year == otherDate.curr_year && this->curr_month == otherDate.curr_month && this->curr_day >= otherDate.curr_day)
		return true;
	else
		return false;
}

bool Date::leapYear() const{
	if (curr_year % 4 == 0 && curr_year % 100 != 0)
		return true;
	else if (curr_year % 400 == 0)
		return true;
	else 
		return false;
}

// BASIC I/O using FRIEND FUNCTIONS
// These functions use Date::format to write or read
ostream& operator<<(ostream& os, const Date& date){

	if (!strcmp(Date::getFormat().getdF(), ""));
	else if (!strcmp(Date::getFormat().getdF(), "d")){
		os<<date.curr_day;
	}
	else if (!strcmp(Date::getFormat().getdF(), "dd")){
		os<<((date.curr_day < D10) ? "0" : "")<<date.curr_day;
	}

	os<<" ";

	if (!strcmp(Date::getFormat().getmF(),"")){
		os<<fmonthname[date.curr_month - 1];
	}
	else if (!strcmp(Date::getFormat().getmF(),"m")){
		os<<date.curr_month;
	}
	else if(!strcmp(Date::getFormat().getmF(),"mm")){
		os<<((date.curr_month < Oct) ? "0" : "")<<date.curr_month;
	}
	else if (!strcmp(Date::getFormat().getmF(),"mmm")){
		os<<monthname[date.curr_month - 1];
	}

	os<<" ";

	if(!strcmp(Date::getFormat().getyF(),""));
	else if(!strcmp(Date::getFormat().getyF(),"yy")){
		if (date.curr_year % 100 == 0)
			os<<"00";
		else
			os<<(date.curr_year % 100);
	}
	else if (!strcmp(Date::getFormat().getyF(),"yyyy")){
		os<<date.curr_year;
	}		

	
	return os;
	
}

istream& operator>>(istream& is, Date& date){
	
	string input;
	is>>input;
	date = Date(input.c_str());
	return is;
}

DateFormat Date::format;

void Date::setFormat(DateFormat& dateformat){
	format = dateformat; //need a copy assignment operator in DateFormat class
}

DateFormat& Date::getFormat(){
	return format;
}


void TestDate(){

	
	cout<<"\nFirst implementing with default dateFormat\n";
	cout<<"Today's date: ";
	try{
		Date today;
		cout<<today<<endl;
	}
	catch(domain_error &e){}
	catch(out_of_range &e){};

	cout<<"\nNew date 5-9-2020 using constructor 'explicit Date(const char* date)': \n";
	try{
		Date d1("5-9-2020");
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};

	cout<<"\nNew date 29-2-2021 using constructor 'explicit Date(const char* date)': \n";
	try{
		Date d1("29-2-2021");
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 29-15-2021 using constructor 'explicit Date(const char* date)': \n";
	try{
		Date d1("29-15-2021");
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	
	
	cout<<"\nNew date 29-15-2060 using constructor 'explicit Date(const char* date)': \n";
	try{
		Date d1("29-15-2060");
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 29-5-2060 using constructor 'explicit Date(const char* date)': \n";
	try{
		Date d1("29-5-2060");
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 13-12-2016 using 'Date(Day d, Month m, Year y)' :\n";
	try{
		Day d = static_cast<Day>(13);
		Month m = static_cast<Month>(12);
		Year y = static_cast<Year>(2016);
		Date d1(d,m,y);
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 32-12-2016 using 'Date(Day d, Month m, Year y)' :\n";
	try{
		Day d = static_cast<Day>(32);
		Month m = static_cast<Month>(12);
		Year y = static_cast<Year>(2016);
		Date d1(d,m,y);
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 29-2-1995 using 'Date(Day d, Month m, Year y)' :\n";
	try{
		Day d = static_cast<Day>(29);
		Month m = static_cast<Month>(2);
		Year y = static_cast<Year>(1995);
		Date d1(d,m,y);
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 31-4-2016 using 'Date(Day d, Month m, Year y)' :\n";
	try{
		Day d = static_cast<Day>(31);
		Month m = static_cast<Month>(4);
		Year y = static_cast<Year>(2016);
		Date d1(d,m,y);
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	

	cout<<"\nNew date 13-12-2050 using 'Date(Day d, Month m, Year y)' :\n";
	try{
		Day d = static_cast<Day>(13);
		Month m = static_cast<Month>(12);
		Year y = static_cast<Year>(2050);
		Date d1(d,m,y);
		cout<<"Date : "<<d1<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};	


	cout<<"\nUsing the copy constructor\n";
	cout<<"Date created first: ";
	
	try{
		Date dfirst("23-6-2018");
		cout<<dfirst<<"( 23-6-2018 )\n";
		Date dcopied( dfirst );
		cout<<"New date copied is: "<<dcopied<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};

	cout<<"Using the copy assignment operator\n";
	cout<<"Date created first: ";
	
	try{
		Date dfirst("29-6-2045");
		cout<<dfirst<<"( 29-6-2045 )\n";
		Date dcopied = dfirst ;
		cout<<"New date through copy assignment operator is: "<<dcopied<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};

	cout<<"\nUsing the unary operators\n";
	try{
		Date mydate("29-2-2016");
		cout<<"Date taken initially: "<<mydate<<endl;
		++mydate;
		cout<<"Next day: "<<mydate<<endl;
		--mydate;
		--mydate;
		cout<<"Yesterday: "<<mydate<<endl;
		++mydate;
		mydate++;
		cout<<"A week from the date: "<<mydate<<endl;
		mydate--;
		mydate--;
		cout<<"A week before the date: "<<mydate<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};

	cout<<"Using the binary relational operators:\n";
	Date d1 = Date();
	Date d2 = Date("15-11-2016");

	cout<<"\nusing '==' \n";
	cout<<d1<<(d1 == d2 ? "==" : "!=")<<d2<<endl;

	Date d3 = Date("19-05-2026");
	cout<<"\nusing '>' \n";
	cout<<d3<<(d3 > d2 ? ">" : "<")<<d2<<endl;

	Date d4 = Date("1-3-1992");
	cout<<"\nusing '<'\n";
	cout<<d4<<(d4 < d1 ? "<" : ">")<<d1<<endl;

	cout<<"\nusing '<='\n";
	cout<<d4<<(d4 <= d2 ? "<=" : ">")<<d2<<endl;

	cout<<"\nusing '>='\n";
	cout<<d3<<(d3 >= d1 ? ">=" : "<")<<d1<<endl;

	Date d5 = Date("29-2-2028");
	cout<<"\nusing '!='\n";
	cout<<d5<<(d5 != d1 ? "!=" : "==")<<d1<<endl;

	cout<<"\nusing cast operators\n";
	cout<<"Today is: "<<d1;
	WeekDay wkday1 = d1;
	Month month1 = d1;
	WeekNumber wknum1 = d1;

	cout<<" "<<dayname[wkday1 - 1]<<" Month: "<<fmonthname[month1 - 1]<<" Week Number: "<<wknum1<<endl<<endl;

	cout<<"Date: "<<d4;
	WeekDay wkday2 = d4;
	Month month2 = d4;
	WeekNumber wknum2 = d4;

	cout<<" "<<dayname[wkday2 - 1]<<" Month: "<<fmonthname[month2 - 1]<<" Week Number: "<<wknum2<<endl<<endl;

	cout<<"Date: "<<d3;
	WeekDay wkday3 = d3;
	Month month3 = d3;
	WeekNumber wknum3 = d3;

	cout<<" "<<dayname[wkday3 - 1]<<" Month: "<<fmonthname[month3 - 1]<<" Week Number: "<<wknum3<<endl;

	cout<<"\nusing the 'operator-' :\n";
	/*cout<<d2<<" - "<<d1<<" = "<<(d2 - d1)<<" days"<<endl;
	cout<<d4<<" - "<<d1<<" = "<<(d4 - d1)<<" days"<<endl;*/

	Date a("2-3-2016");
	Date b("29-3-2016");
	Date c("17-7-2016");
	Date e("17-7-2020");
	Date f("29-2-2000");

	cout<<a<<" - "<<b<<" "<<(a-b)<<"days"<<endl;
	cout<<c<<" - "<<a<<" "<<(c-a)<<"days"<<endl;
	cout<<b<<" - "<<e<<" "<<(b-e)<<"days"<<endl;
	cout<<e<<" - "<<f<<" "<<(e-f)<<"days"<<endl;

	cout<<"\nusing the 'operator+' :\n";

	try{
		cout<<b<<" + 365"<<" = "<<(b + 365)<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};

	try{
		cout<<Date("2-3-2048")<<" + 367 = "<<(Date("2-3-2048") + 367)<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};

	cout<<Date("2-3-2049")<<" + 367 = ";
	try{
		cout<<(Date("2-3-2049") + 367)<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};
	
	cout<<"\nusing the 'operator istream' :\n";
	cout<<"enter a date with '-' separating d,m,y: \n";
	try{
		Date trialdate;
		cin>>trialdate;
		cout<<"You entered: "<<trialdate<<endl;
	}
	catch(invalid_argument &e){
		cout<<e.what()<<endl;
	}
	catch(domain_error &e){
		cout<<e.what()<<endl;
	}
	catch(out_of_range &e){
		cout<<e.what()<<endl;
	};
	
	cout<<"\nNow testing the format:\n";

	string dateptr, monthptr, yearptr;
	cout<<"using format --yyyy :\n";
	dateptr = "";
	monthptr = "";
	yearptr = "yyyy";
	DateFormat format1(dateptr.c_str(), monthptr.c_str(), yearptr.c_str());
	Date::setFormat(format1);
	cout<<"Today's date: "<<d1<<endl;

	cout<<"using format d-m-yyyy :\n";
	dateptr = "d";
	monthptr = "m";
	yearptr = "yyyy";
	DateFormat format2(dateptr.c_str(), monthptr.c_str(), yearptr.c_str());
	Date::setFormat(format2);
	cout<<"Today's date: "<<d1<<endl;

	cout<<"using format dd-mm-yy :\n";
	dateptr = "dd";
	monthptr = "mm";
	yearptr = "yy";
	DateFormat format3(dateptr.c_str(), monthptr.c_str(), yearptr.c_str());
	Date::setFormat(format3);
	cout<<"Today's date: "<<d1<<endl;

	cout<<"using format -mmm-yyyy :\n";
	dateptr = "";
	monthptr = "mmm";
	yearptr = "yyyy";
	DateFormat format4(dateptr.c_str(), monthptr.c_str(), yearptr.c_str());
	Date::setFormat(format4);
	cout<<"Today's date: "<<d1<<endl;

	cout<<"using format dd--yyyy :\n";
	string fm = "dd--yyyy";
	DateFormat format5(fm.c_str());
	Date::setFormat(format5);
	cout<<"Today's date: "<<d1<<endl;
	

	
}

int main(int argc, char const *argv[])
{
	TestDate();
	return 0;
}

