#include "AgendaService.hpp"
#include "Storage.hpp"
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::find;
using std::list;
using std::string;

/**
   * constructor
   */
  AgendaService::AgendaService() {
  	startAgenda();
  }

  /**
   * destructor
   */
  AgendaService::~AgendaService() {
  	  if(m_storage != nullptr)
  	      quitAgenda();
  }

  /**
   * check if the username match password
   * @param userName the username want to login
   * @param password the password user enter
   * @return if success, true will be returned
   */
  bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
      auto filter = [&](const User &it) -> bool {return (it.getName() == userName 
      	&& it.getPassword() == password);};
      if(m_storage->queryUser(filter).empty())
          return false;
      return true;
  }

  /**
   * regist a user
   * @param userName new user's username
   * @param password new user's password
   * @param email new user's email
   * @param phone new user's phone
   * @return if success, true will be returned
   */
  bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone) {

        auto filter = [&](const User &it) -> bool {return (it.getName() == userName);};
  	    if(!m_storage->queryUser(filter).empty()) return false;
  	    User user(userName,password,email,phone);
  	    m_storage->createUser(user);
  	    return true;
  }

  /**
   * delete a user
   * @param userName user's username
   * @param password user's password
   * @return if success, true will be returned
   */
  bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
  	  auto filter = [&](const User &it) -> bool {return (it.getName() == userName 
  	  	&& it.getPassword() == password);};
      list<User> tmp(m_storage->queryUser(filter));
      if(tmp.empty()) return false;
      
      
      //delete sponsor and participator with 0 meeting
      auto effect1 = [&](const Meeting &it) -> bool {return ((it.getSponsor() == userName) 
      	|| (it.isParticipator(userName) && (it.getParticipator().size() == 1)));};
      m_storage->deleteMeeting(effect1);
      //delete participator (not 0)
      auto effect2 = [&](const Meeting &it) -> bool {return (it.isParticipator(userName) 
      	&& (it.getParticipator().size() > 1));};
         auto switcher = [&](Meeting &it) { it.removeParticipator(userName);};
      m_storage->updateMeeting(effect2,switcher);
      int total = m_storage->deleteUser(filter);
      if(total == 0) return false;
      return true;
      
  }

  /**
   * list all users from storage
   * @return a user list result
   */
  std::list<User> AgendaService::listAllUsers(void) const {
      auto filter = [&](const User &it) -> bool {return true;};
      return m_storage->queryUser(filter);
  }

  /**
   * create a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @param startData the meeting's start date
   * @param endData the meeting's end date
   * @return if success, true will be returned
   */
  bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator) {
  	  Date sDate(startDate),eDate(endDate);
      if(!Date::isValid(sDate) || !Date::isValid(eDate)) return false;
  	  if(sDate >= eDate) return false;
      //same title
      if(!m_storage->queryMeeting([&](const Meeting &it) -> bool {return it.getTitle() == title;}).empty()) return false;
      //exist
      if(m_storage->queryUser([&](const User &it) -> bool {return it.getName() == userName;}).empty()) return false;
      for(auto iter = participator.begin(); iter != participator.end(); iter++) {
          if(*iter == userName) return false;
          if(m_storage->queryUser([&](const User &it) -> bool {return it.getName() == *iter;}).empty())
              return false;
      }
      //repeat sponsor or participator
      //if(find(participator.begin(),participator.end(),userName) != participator.end()) return false;
      
      for(auto iter = participator.begin(); iter != participator.end(); iter++) {
          if(find(iter+1,participator.end(),(*iter)) != participator.end()) return false;
          //participator time
          auto filtera = [&](const Meeting &it) -> bool {
              if(it.getSponsor() == *iter || it.isParticipator(*iter)) {
                  /*if(it.getStartDate() <= sDate && it.getEndDate() > sDate)
                      return true;
                  if(it.getStartDate() < eDate && it.getEndDate() >= eDate)
                      return true;
                  if(it.getStartDate() >= sDate && it.getEndDate() <= eDate)
                      return true;*/
                    if(!(it.getStartDate() >= eDate || it.getEndDate() <= sDate)) return true;
                  return false;
              }
              return false;
          };
         if(!m_storage->queryMeeting(filtera).empty()) return false;
         // if(!m_storage->queryMeeting(filter).empty()) return false;
      }
       
      //sponsor time
      auto filter = [&](const Meeting &it) -> bool {
      if(it.getSponsor() == userName || it.isParticipator(userName)) {
          if(it.getStartDate() <= sDate && it.getEndDate() > sDate)
            return true;
          if(it.getStartDate() < eDate && it.getEndDate() >= eDate)
            return true;
          if(it.getStartDate() >= sDate && it.getEndDate() <= eDate)
            return true;
          return false;
      }
      return false;
      };
     if(!m_storage->queryMeeting(filter).empty()) return false;


      /*auto filter = [&](const Meeting it) -> bool {
          //time
          //if(!(it.getStartDate() >= eDate || it.getEndDate() <= sDate))
            //  return false;
          //sponsor time
          if(userName == it.getSponsor() || it.isParticipator(userName))
          	  return false;
          //participator time
          for(auto iter = participator.begin(); iter != participator.end(); iter++) {
              if(*iter == it.getSponsor() || it.isParticipator(*iter))
                  return false;
          }
          return true;
      };*/
      
  	  Meeting meeting(userName,participator,sDate,eDate,title);
      m_storage->createMeeting(meeting);
      return true;
  }

  /**
   * add a participator to a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returned
   */
  bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator) {
      auto filter = [&](const Meeting &it) -> bool {return (it.getSponsor() == userName 
      	&& it.getTitle() == title && !it.isParticipator(participator));};
      list<Meeting> temp(m_storage->queryMeeting(filter));
      if(temp.size() != 1) return false;
      
      //exist
      if(m_storage->queryUser([&](const User &it) -> bool {return it.getName() == participator;}).empty())
         return false;
      Date sDate(temp.front().getStartDate()),eDate(temp.front().getEndDate());
      
      //time
      auto timefilter = [&](const Meeting &it) -> bool {
      if(it.getSponsor() == participator || it.isParticipator(participator)) {
          if(it.getStartDate() <= sDate && it.getEndDate() > sDate)
            return true;
          if(it.getStartDate() < eDate && it.getEndDate() >= eDate)
            return true;
          if(it.getStartDate() >= sDate && it.getEndDate() <= eDate)
            return true;
          return false;
      }
      return false;
      };
     
      if(!m_storage->queryMeeting(timefilter).empty()) return false;
      
      auto switcher = [&](Meeting &it) { it.addParticipator(participator);};
      int total = m_storage->updateMeeting(filter,switcher);
      if(total == 0) return false;
      return true;
      
  }

  /**
   * remove a participator from a meeting
   * @param userName the sponsor's userName
   * @param title the meeting's title
   * @param participator the meeting's participator
   * @return if success, true will be returnedmpru-
   */
  bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator) {
      auto filter = [&](const Meeting &it) -> bool {return (it.getSponsor() == userName 
      	&& it.getTitle() == title && it.isParticipator(participator));};
      list<Meeting> temp(m_storage->queryMeeting(filter));
      if(temp.empty()) return false;
      if(temp.front().getParticipator().size() == 1)
        	m_storage->deleteMeeting(filter);
      else {
          auto switcher = [&](Meeting &it) { it.removeParticipator(participator);};
          int total = m_storage->updateMeeting(filter,switcher);
          if(total == 0) return false;
      }
      
      return true;
  }

  /**
   * quit from a meeting
   * @param userName the current userName. no need to be the sponsor
   * @param title the meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
       
       auto filter = [&](const Meeting &it) -> bool {return it.isParticipator(userName)
        && it.getTitle() == title;};
        list<Meeting> temp(m_storage->queryMeeting(filter));
        if(temp.empty() || temp.front().getSponsor() == userName) return false;
       //delete 0 paticipator meeting
        if(temp.front().getParticipator().size() == 1)
        	m_storage->deleteMeeting(filter);
      	else {
          	auto switcher = [&](Meeting &it) { it.removeParticipator(userName);};
      	    int total = m_storage->updateMeeting(filter,switcher);
      	    if(total == 0) return false;
      	}
        
        return true;
  }

  /**
   * search a meeting by username and title
   * @param uesrName the sponsor's userName
   * @param title the meeting's title
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const {
      auto filter = [&](const Meeting &it) -> bool {
      	return ((it.getSponsor() == userName || it.isParticipator(userName)) && it.getTitle() == title);};
      return m_storage->queryMeeting(filter);
  }
  /**
   * search a meeting by username, time interval
   * @param uesrName the sponsor's userName
   * @param startDate time interval's start date
   * @param endDate time interval's end date
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const {
  	Date sDate(startDate),eDate(endDate);
  	list<Meeting> empty;
  	if(!Date::isValid(sDate) || !Date::isValid(eDate))
  		return empty;
  	if(sDate > eDate) return empty;
    auto filter = [&](const Meeting &it) -> bool {
    	if(it.getSponsor() == userName || it.isParticipator(userName)) {
          if(it.getStartDate() <= sDate && it.getEndDate() >= sDate)
            return true;
          if(it.getStartDate() <= eDate && it.getEndDate() >= eDate)
            return true;
          if(it.getStartDate() >= sDate && it.getEndDate() <= eDate)
            return true;
          return false;
      }
      return false;
     //&& !(it.getStartDate() > eDate || it.getEndDate() < sDate));};
    };
     return m_storage->queryMeeting(filter);
  }

  /**
   * list all meetings the user take part in
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
      auto filter = [&](const Meeting &it) -> bool {return (it.getSponsor() == userName 
      	|| it.isParticipator(userName));};
      return m_storage->queryMeeting(filter);
  }

  /**
   * list all meetings the user sponsor
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
      auto filter = [&](const Meeting &it) -> bool {return it.getSponsor() == userName;};
      return m_storage->queryMeeting(filter);
  }

  /**
   * list all meetings the user take part in and sponsor by other
   * @param userName user's username
   * @return a meeting list result
   */
  std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const {
      auto filter = [&](const Meeting &it) -> bool {return it.isParticipator(userName);};
      return m_storage->queryMeeting(filter);
  }

  /**
   * delete a meeting by title and its sponsor
   * @param userName sponsor's username
   * @param title meeting's title
   * @return if success, true will be returned
   */
  bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
      auto filter = [&](const Meeting &it) -> bool {return it.getSponsor() == userName 
      	&& it.getTitle() == title;};
      	int total = m_storage->deleteMeeting(filter);
      	if(total == 0) return false;
      	return true;
  }

  /**
   * delete all meetings by sponsor
   * @param userName sponsor's username
   * @return if success, true will be returned
   */
  bool AgendaService::deleteAllMeetings(const std::string &userName) {
      auto filter = [&](const Meeting &it) -> bool {return it.getSponsor() == userName;};
      int total = m_storage->deleteMeeting(filter);;
      if(total == 0) return false;
      return true;
  }

  /**
   * start Agenda service and connect to storage
   */
  void AgendaService::startAgenda(void) {
      m_storage = Storage::getInstance();
  }

  /**
   * quit Agenda service
   */
  void AgendaService::quitAgenda(void) {
      m_storage->sync();
      m_storage = nullptr;
  }
