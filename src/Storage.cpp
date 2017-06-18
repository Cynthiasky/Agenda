#include "Storage.hpp"
#include "Path.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using std::stringstream;
using std::shared_ptr;
using std::function;
using std::string;
using std::getline;
using std::vector;
using std::map;

std::shared_ptr<Storage> Storage::m_instance = nullptr;
  /**
  *   default constructor
  */
  Storage::Storage() {
  	  m_dirty = false;
      readFromFile();
  }

  /**
  *   read file content into memory
  *   @return if success, true will be returned
  */
  bool Storage::readFromFile(void) {
      std::ifstream userIn(Path::userPath,std::ifstream::in);
      std::ifstream meetingIn(Path::meetingPath,std::ifstream::in);
      if(!userIn.is_open() || !meetingIn.is_open())
      	return false;
      string userStr = "";
      while(getline(userIn,userStr)) {
      	if(userStr == "\"\",") break;
      	string name,pass,email,phone;
        
     vector<string> userTemp;
     userStr += ",";
     size_t userSize = userStr.size();
     size_t userNow = userStr.find(",");
     while(userNow != string::npos) {
         userTemp.push_back(userStr.substr(0,userNow));
         userStr = userStr.substr(userNow + 1,userSize);
         userNow = userStr.find(",");
     }
    name = userTemp[0].substr(1,userTemp[0].size() - 2);
    pass = userTemp[1].substr(1,userTemp[1].size() - 2);
    email = userTemp[2].substr(1,userTemp[2].size() - 2);
    phone = userTemp[3].substr(1,userTemp[3].size() - 2);

    //std::cout << "test!    " <<name<<pass<<email<<phone<<std::endl;
      	m_userList.push_back(User(name,pass,email,phone));
      	userStr = "";
      }
      userIn.close();

      string meetingStr = "";
      while(getline(meetingIn,meetingStr)) {
      	  if(meetingStr == "") break;
      	  string sponsor,parts,start,end,title;

         vector<string> meetTemp;
         meetingStr = meetingStr + ",";
         size_t meetSize = meetingStr.size();
         size_t meetNow = meetingStr.find(",");
         while(meetNow != string::npos) {
             meetTemp.push_back(meetingStr.substr(0,meetNow));
             meetingStr = meetingStr.substr(meetNow + 1,meetSize);
             meetNow = meetingStr.find(",");
         }
        sponsor = meetTemp[0].substr(1,meetTemp[0].size() - 2);
        parts = meetTemp[1].substr(1,meetTemp[1].size() - 2);
        start = meetTemp[2].substr(1,meetTemp[2].size() - 2);
        end = meetTemp[3].substr(1,meetTemp[3].size() - 2);
        title = meetTemp[4].substr(1,meetTemp[4].size() - 2);



          std::vector<string> partv;
          if(parts != "") {
              parts += "&";
              size_t size = parts.size();
              size_t now = parts.find("&");
              while(now != string::npos) {
          	      partv.push_back(parts.substr(0,now));
          	      parts = parts.substr(now + 1,size);
          	      now = parts.find("&");	
              }
          }
    //std::cout << "\ntest!    " <<sponsor<<start<<end<<title<<std::endl;

          Meeting res;
          res.setSponsor(sponsor);
          res.setParticipator(partv);
          res.setStartDate(Date::stringToDate(start));
          res.setEndDate(Date::stringToDate(end));
          res.setTitle(title);

          m_meetingList.push_back(res);
          meetingStr = "";

      }
      meetingIn.close();

      return true;
  }

  /**
  *   write file content from memory
  *   @return if success, true will be returned
  */
  bool Storage::writeToFile(void) {
  	   std::ofstream userOut(Path::userPath,std::ofstream::out | std::ofstream::trunc);
  	   std::ofstream meetingOut(Path::meetingPath,std::ofstream::out | std::ofstream::trunc);
  	   if(!userOut.is_open() || !meetingOut.is_open())
  	   	  return false;
       for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
           userOut << '\"' << iter->getName() << "\",\"" << iter->getPassword() << "\",\""
           << iter->getEmail() << "\",\"" << iter->getPhone() << "\"\n";
       }
       userOut.close();

       for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
       	 meetingOut << '\"' << iter->getSponsor() << "\",\"";
       	 auto part(iter->getParticipator());
       	 for(auto it = part.begin(); it != part.end(); it++) {
       	 	if(it != part.begin())
       	 		meetingOut << '&';
       	 	meetingOut << (*it);
       	 }
       	meetingOut << "\",\"" << Date::dateToString(iter->getStartDate()) << "\",\""
       	           << Date::dateToString(iter->getEndDate()) << "\",\"" << iter->getTitle()
       	           << "\"\n";

       }

       meetingOut.close();

       m_dirty = false;
       return true;
  }





  /**
  * get Instance of storage
  * @return the pointer of the instance
  */
  //static
  std::shared_ptr<Storage> Storage::getInstance(void) {
  	if(m_instance == nullptr) {
        m_instance = shared_ptr<Storage>(new Storage());
  	}
  	return m_instance;
  }

  /**
  *   destructor
  */
  Storage::~Storage() {
  	sync();
  }

  // CRUD for User & Meeting
  // using C++11 Function Template and Lambda Expressions

  /**
  * create a user
  * @param a user object
  */
  void Storage::createUser(const User &t_user) {
      m_userList.push_back(t_user);
      m_dirty = true;
  }

  /**
  * query users
  * @param a lambda function as the filter
  * @return a list of fitted users
  */
  std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
  	std::list<User> res;
  	 for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
         if(filter(*iter))
         	res.push_back((*iter));
     }
     return res;
  }

  /**adl
  * update users
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the user
  * @return the number of updated users
  */
  int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
  	  int res = 0;
      for(auto iter = m_userList.begin(); iter != m_userList.end(); iter++) {
          if(filter(*iter)) {
              switcher(*iter);
              res++;
          }
  	  }
  	  if(res) m_dirty = true;
  	  return res;
  }

  /**
  * delete users
  * @param a lambda function as the filter
  * @return the number of deleted users
  */
  int Storage::deleteUser(std::function<bool(const User &)> filter) {
  	int prev = m_userList.size();
  	m_userList.remove_if(filter);
  	int res = prev - m_userList.size();
    if(res) m_dirty = true;
  	return res;
  }

  /**
  * create a meeting
  * @param a meeting object
  */
  void Storage::createMeeting(const Meeting &t_meeting) {
  	m_meetingList.push_back(t_meeting);
  	m_dirty = true;
  }

  /**
  * query meetings
  * @param a lambda function as the filter
  * @return a list of fitted meetings
  */
  std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const {
  	 std::list<Meeting> res;
  	 for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
         if(filter(*iter))
         	res.push_back(*iter);
     }
     return res;
  }

  /**
  * update meetings
  * @param a lambda function as the filter
  * @param a lambda function as the method to update the meeting
  * @return the number of updated meetings
  */
  int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
  	  int res = 0;
      for(auto iter = m_meetingList.begin(); iter != m_meetingList.end(); iter++) {
          if(filter(*iter)) {
              switcher(*iter);
              res++;
          }
  	  }
  	  if(res) m_dirty = true;
  	  return res;
  }

  /**
  * delete meetings
  * @param a lambda function as the filter
  * @return the number of deleted meetings
  */
  int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
  	int prev = m_meetingList.size();
  	m_meetingList.remove_if(filter);
  	int res = prev - m_meetingList.size();
  	if(res) m_dirty = true;
  	return res;
  }

  /**
  * sync with the file
  */
  bool Storage::sync(void) {
  	if(m_dirty) {
  		m_dirty = false;
  		return writeToFile();
  	}
  	return true;
  }



