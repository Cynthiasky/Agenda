#include "Date.hpp"
#include <sstream>

using std::stringstream;

  Date::Date() {
  	m_year = 0;
  	m_month = 0;
  	m_day = 0;
  	m_hour = 0;
  	m_minute = 0;
  }

  /**
  * @brief constructor with arguments
  */
  Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
  	m_year = t_year;
  	m_month = t_month;
  	m_day = t_day;
  	m_hour = t_hour;
  	m_minute = t_minute;
  }

  /**
  * @brief constructor with a string
  */
  Date::Date(const std::string &dateString) {(*this) = stringToDate(dateString);}
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
  int Date::getYear(void) const {return m_year;}

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
  void Date::setYear(const int t_year) {m_year = t_year;}

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
  int Date::getMonth(void) const {return m_month;}

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
  void Date::setMonth(const int t_month) {m_month = t_month;}

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
  int Date::getDay(void) const {return m_day;}

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
  void Date::setDay(const int t_day) {m_day = t_day;}

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
  int Date::getHour(void) const {return m_hour;}

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
  void Date::setHour(const int t_hour) {m_hour = t_hour;}

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
  int Date::getMinute(void) const {return m_minute;}

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
  void Date::setMinute(const int t_minute) {m_minute = t_minute;}

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
  //static
  bool Date::isValid(const Date &t_date) {
  	  if(t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
  	  if(t_date.getMonth() < 1 || t_date.getMonth() > 12) return false;
  	  if(t_date.getHour() < 0 || t_date.getHour() >= 24) return false;
  	  if(t_date.getMinute() < 0 || t_date.getMinute() >= 60) return false;

      int num_of_days[14] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
      int y = t_date.getYear();
      if((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) num_of_days[2]++;
      if(t_date.getDay() <= 0 || t_date.getDay() > num_of_days[t_date.getMonth()]) return false;
      return true;
  }

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  //static
  Date Date::stringToDate(const std::string &t_dateString) {
  	  bool flag = true;
  	  Date empty;
  	  if(t_dateString.length() != 16) flag = false;
  	  if(t_dateString[4] != '-' || t_dateString[7] != '-' || t_dateString[10] != '/' || t_dateString[13] != ':') flag = false;
  	  for(int i = 0; i < 16; i++) {
  	  	if(i == 4 || i == 7 || i == 10 || i == 13) continue;
  	  	if(t_dateString[i] < '0' || t_dateString[i] > '9') {
  	  	    flag = false;
  	  	    break;
  	  	}
  	  }
  	  if(!flag) return empty;

  	  int year,month,day, hour, minute;
      stringstream ss;
      ss << t_dateString;
      ss >> year;
      ss.ignore(20,'-');
      ss >> month;
      ss.ignore(20,'-');
      ss >> day;
      ss.ignore(20,'/');
      ss >> hour;
      ss.ignore(20,':');
      ss >> minute;
      ss.clear();
  //std::cout <<"test" <<year<<" "<<month<<" "<<day<<" "<<hour<<" "<<minute<<" "<<std::endl;
      Date res(year,month,day,hour,minute);
      //if(!isValid(res)) return empty;
      return res;
  }

  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  */
  //static 
  std::string Date::dateToString(const Date &t_date) {
  	if(!isValid(t_date)) return "0000-00-00/00:00";
 
    char data_string[20];
    snprintf(data_string,20,"%04d-%02d-%02d/%02d:%02d",t_date.m_year,t_date.m_month,t_date.m_day,t_date.m_hour,t_date.m_minute);
  	return std::string(data_string);
  }

  /**
  *  @brief overload the assign operator
  */
  Date &Date::operator=(const Date &t_date) {
  	this->setYear(t_date.getYear());
  	this->setMonth(t_date.getMonth());
  	this->setDay(t_date.getDay());
  	this->setHour(t_date.getHour());
  	this->setMinute(t_date.getMinute());
  	return (*this);
  }

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool Date::operator==(const Date &t_date) const {
  	return (m_year == t_date.getYear() && m_month == t_date.getMonth() &&
  	m_day == t_date.getDay() && m_hour == t_date.getHour() && m_minute == t_date.getMinute());
  }

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool Date::operator>(const Date &t_date) const {
      
  	  return (dateToString(*this) > dateToString(t_date));
  	//return (vector<int> {m_year,m_month,m_day,m_hour,m_minute} > 
  		//vector<int> {t_date.m_year,t_date.m_month,t_date.m_day,t_date.m_hour,t_date.m_minute});
  }

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool Date::operator<(const Date &t_date) const {
  	return !((*this > t_date) || (*this == t_date));
  	//return (dateToString(*this) < dateToString(t_date));

  }

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool Date::operator>=(const Date &t_date) const {
  	return ((*this) == t_date || (*this) > t_date);
  	//return (dateToString(*this) >= dateToString(t_date));
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const {
  	return ((*this) < t_date || (*this) == t_date);
    //return (dateToString(*this) <= dateToString(t_date));
  }