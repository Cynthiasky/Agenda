#include "AgendaUI.hpp"
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <csignal>
//#include <conio.h>
#include <cstdio>
#include <regex>

using std::list;
using std::vector;
using std::string;
using std::stringstream;
using std::left;
using std::cout;
using std::cin;
using std::endl;
using std::setw;




string printPart(vector<string> v) {
	string res = "";
    for(auto it = v.begin(); it != v.end(); it++) {
    	if(it != v.begin())
    		res += ",";
    	res += *it;
    }
	return res;
}



  AgendaUI::AgendaUI() {
      startAgenda();
  }
  void AgendaUI::OperationLoop(void) {
      while(true) {
        for(int i = 0; i < 37; i++) cout << "-";
        	cout << "Agenda";
        for(int i = 0; i < 37; i++) cout << "-";
            cout << endl;
        cout << "Action :" << endl;
        if(m_userName == "") {
        	cout << "l   - log in Agenda by user name and password" << endl;
        	cout << "r   - register an Agenda account" << endl;
        	cout << "q   - quit Agenda" << endl;
        }
        else {
        	cout << "o   - log out Agenda" << endl;
        	cout << "dc  - delete Agenda account" << endl;
        	cout << "lu  - list all Agenda user" << endl;
        	cout << "cm  - create a meeting" << endl;
        	cout << "amp - add meeting participator" << endl;
        	cout << "rmp - remove meeting participator" << endl;
        	cout << "rqm - request to quit meeting" << endl;
        	cout << "la  - list all meetings" << endl;
        	cout << "las - list all sponsor meetings" << endl;
        	cout << "lap - list all participator meetings" << endl;
        	cout << "qm  - query meetings by title" << endl;
        	cout << "qt  - query meetings by time interval" << endl;
        	cout << "dm  - delete meeting by title" << endl;
        	cout << "da  - delete all meetings" << endl;

        }
        for(int i = 0; i < 80; i++) cout << "-";
        cout << endl;
        cout << endl;
        cout << "Agenda";
        if(m_userName == "" && m_userPassword == "")
            cout << " :~$ ";
        else
            cout << "@" << m_userName << " :~# ";
        
        //signal(SIGINT, signalHandler);
        if(!executeOperation(getOperation())) break;
      }
  }



/**
   * constructor
   */
  void AgendaUI::startAgenda(void) {
    m_agendaService.startAgenda();
    m_userName = m_userPassword = "";
  }

  /**
   * catch user's operation
   * @return the operation
   */
  std::string AgendaUI::getOperation() {
    string oper;
    cin >> oper;
    return oper;
  }

  /**
   * execute the operation
   * @return if the operationloop continue
   */
  bool AgendaUI::executeOperation(const std::string &t_operation) {
      bool logged = true;
      if(m_userName == "") logged = false;

      if(t_operation == "") return true;
      if(t_operation == "l" && !logged) {
          userLogIn();
          return true;
      }
      else if(t_operation == "r" && !logged) {
      	userRegister();
      	return true;
      }
      else if(t_operation == "q" && !logged) {
      	quitAgenda();
      	return false;
      }
      else if(t_operation == "o" && logged) {
      	userLogOut();
      	return true;
      }
      else if(t_operation == "dc" && logged) {
      	deleteUser();
      	return true;
      }
      else if(t_operation == "lu" && logged) {
      	listAllUsers();
      	return true;
      }
      else if(t_operation == "cm" && logged) {
      	createMeeting();
      	return true;
      }
      else if(t_operation == "amp" && logged) {
      	addMeetingParticipator();
      	return true;
      }
      else if(t_operation == "rmp" && logged) {
      	removeMeetingParticipator();
      	return true;
      }
      else if(t_operation == "rqm" && logged) {
      	quitMeeting();
      	return true;
      }
      else if(t_operation == "la" && logged) {
      	listAllMeetings();
      	return true;
      }
      else if(t_operation == "las" && logged) {
      	listAllSponsorMeetings();
      	return true;
      }
      else if(t_operation == "lap" && logged) {
      	listAllParticipateMeetings();
      	return true;
      }
      else if(t_operation == "qm" && logged) {
          queryMeetingByTitle();
          return true;
      }
      else if(t_operation == "qt" && logged) {
          queryMeetingByTimeInterval();
          return true;
      }
      else if(t_operation == "dm" && logged) {
          deleteMeetingByTitle();
          return true;
      }
      else if(t_operation == "da" && logged) {
          deleteAllMeetings();
          return true;
      }
      else {
          cout << "\nPlease enter correct command!\n" << endl;
          return true;
      }
  }

  /**
   * user Login
   */
  void AgendaUI::userLogIn(void) {
  	cout << "\n[log in] [username] [password]" << endl;
  	cout << "[log in] [username] ";
  	//cin >> m_userName >> m_userPassword;
    



    cin >> m_userName;
    getchar();
    cout << "[log in] [password] ";
    m_userPassword = "";
    char temp;
    int len = 0;
    while(true) {
    	system("stty -echo");
    	temp = getchar();
    	system("stty echo");
    	if(temp == '\n') break;
    	if(temp == '\b') {
    		if(len != 0) {
    			//system("stty echo");
    			cout << "\b \b";
    			m_userPassword = m_userPassword.substr(0,len-1);
    			len--;
    		}
    	}
    	else {
    		//system("stty echo");
    	    cout << "*";
    	    m_userPassword += temp;
    	    len++;
    	}
    }
    cout << endl;


    for(int i = 0; i < m_userPassword.length(); i++)
    	m_userPassword[i]++;



  	if(m_agendaService.userLogIn(m_userName,m_userPassword))
  		cout << "[log in] succeed!" << endl;
  	else {
  		cout << "[log in] Password error or user doesn't exist" << endl;
  		m_userName = "";
  		m_userPassword = "";
  	}
  	cout << endl;
  }

  /**
   * user regist
   */
  void AgendaUI::userRegister(void) {
  	cout << "\n[register] [username] [password] [email] [phone]" << endl;
  	cout << "[register] ";
  	string name,pass,email,phone;
  	cin >> name >> pass >> email >> phone;
  	/*
  	std::regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
  	if(!regex_match(email,pattern)) {
        cout << "[register] wrong email format! email should be xxx@xxx.xxx" << endl;
        return;
  	}
    */
    for(int i = 0; i < pass.length(); i++)
    	pass[i]++;



  	if(m_agendaService.userRegister(name,pass,email,phone))
  		cout << "[register] succeed!" << endl;
  	else
  		cout << "[register] This username has been registered!" << endl;
  	cout << endl;
  }

  /**
   * user logout
   */
  void AgendaUI::userLogOut(void) {
    m_userName = "";
    m_userPassword = "";
    cout << "\n\n\n";
  }

  /**
   * quit the Agenda
   */
  void AgendaUI::quitAgenda(void) {
      m_agendaService.quitAgenda();
  }

  /**
   * delete a user from storage
   */
  void AgendaUI::deleteUser(void) {
  	cout << endl;
  	m_agendaService.deleteUser(m_userName,m_userPassword);
  	m_userName = "";
  	m_userPassword = "";
  	cout << "[delete Agenda account] succeed!" << endl;
  	cout << endl;
  }

  /**
   * list all users from storage
   */
  void AgendaUI::listAllUsers(void) {
      cout <<"\n[list all users]\n" << endl;
      cout << left << setw(20) <<"name" <<setw(20)<<"email"<<"phone"<<endl;
      list<User> temp(m_agendaService.listAllUsers());
      for(auto it = temp.begin(); it != temp.end(); it++) {
      	cout << setw(20) <<it->getName();
      	cout << setw(20) <<it->getEmail();
      	cout <<it->getPhone()<<endl;
      }
      cout << endl;
  }

  /**
   * user create a meeting with someone else
   */
  void AgendaUI::createMeeting(void) {
      cout <<"\n[create meeting] [the number of participators]" << endl;
      cout <<"[create meeting] ";
      int num;
      cin >> num;
      vector<string> temp;
      for(int i = 1; i <= num; i++) {
      	string no;
      	stringstream ss;
      	ss << i;
      	ss >> no;
      	cout <<"[create meeting] [please enter the participator "+ no +" ]"<<endl;
      	cout << "[create meeting] ";
      	string in;
      	cin >> in;
      	temp.push_back(in);
      }
      cout <<"[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
      	cout << "[create meeting] ";
      	string title,start,end;
      	cin >> title>>start>>end;
      	if(m_agendaService.createMeeting(m_userName,title,start,end,temp))
      		cout <<"[create meeting] success!" << endl;
      	else
      		cout <<"[create meeting] error!" << endl;
      	cout << endl;
  }

  /**
   * sponsor add a participator to the meeting
   */
  void AgendaUI::addMeetingParticipator(void) {
  	cout << "\n[add participator] [meeting title] [participator username]" << endl;
  	cout <<"[add participator] ";
  	string title,part;
  	cin >> title >> part;
  	if(m_agendaService.addMeetingParticipator(m_userName,title,part))
  		cout << "[add participator] succeed!" << endl;
  	else
  		cout <<"[add participator] error!" << endl;
  	cout << endl;
  }

  /**
   * sponsor add a participator to the meeting
   */
  void AgendaUI::removeMeetingParticipator(void) {
  	cout << "\n[remove participator] [meeting title] [participator username]" << endl;
  	cout <<"[remove participator] ";
  	string title,part;
  	cin >> title >> part;
  	if(m_agendaService.removeMeetingParticipator(m_userName,title,part))
  		cout << "[remove participator] succeed!" << endl;
  	else
  		cout <<"[remove participator] error!" << endl;
  	cout << endl;
  }

  /**
   * user quit from meeting
   */
  void AgendaUI::quitMeeting(void) {
  	cout << "\n[quit meeting] [meeting title]" << endl;
  	cout <<"[quit meeting] ";
  	string title;
  	cin >> title;
  	if(m_agendaService.quitMeeting(m_userName,title))
  		cout << "[quit meeting] succeed!" << endl;
  	else
  		cout <<"[quit meeting] error!" << endl;
  	cout << endl;
  }

  /**
   * list all meetings from storage
   */
  void AgendaUI::listAllMeetings(void) {
  	list<Meeting> res(m_agendaService.listAllMeetings(m_userName));
    cout << "\n[list all meetings]\n" << endl;
    if(res.empty())
    	cout << "none" << endl;
    else
    	printMeetings(res);
    cout << endl;
  }

  /**
   * list all meetings that this user sponsored
   */
  void AgendaUI::listAllSponsorMeetings(void) {
  	list<Meeting> res(m_agendaService.listAllSponsorMeetings(m_userName));
    cout << "\n[list all sponsor meetings]\n" << endl;
    if(res.empty())
    	cout << "none" << endl;
    else
    	printMeetings(res);
    cout << endl;
  }

  /**
   * list all meetings that this user take part in
   */
  void AgendaUI::listAllParticipateMeetings(void) {
  	list<Meeting> res(m_agendaService.listAllParticipateMeetings(m_userName));
    cout << "\n[list all participator meetings]\n" << endl;
    if(res.empty())
    	cout << "none" << endl;
    else
    	printMeetings(res);
    cout << endl;
  }

  /**
   * search meetings by title from storage
   */
  void AgendaUI::queryMeetingByTitle(void) {
  	cout << "\n[query meetings] [title]:" << endl;
  	cout << "[query meetings] ";
  	string title;
  	cin >> title;
  	cout << "[query meetings]\n" << endl;
    list<Meeting> res(m_agendaService.meetingQuery(m_userName,title));
    if(res.empty())
    	cout << "none" << endl;
    else
    	printMeetings(res);
    cout << endl;
  }

  /**
   * search meetings by timeinterval from storage
   */
  void AgendaUI::queryMeetingByTimeInterval(void) {
  	cout << "\n[query meetings] [start time(yyyy-mm-dd/hh:mm)] " 
  	     << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
  	cout << "[query meetings] ";
  	string start,end;
  	cin >> start >> end;
  	cout << "[query meetings]\n" << endl;
    list<Meeting> res(m_agendaService.meetingQuery(m_userName,start,end));
    if(res.empty())
    	cout << "none" << endl;
    else
    	printMeetings(res);
    cout << endl;
  }

  /**
   * delete meetings by title from storage
   */
  void AgendaUI::deleteMeetingByTitle(void) {
  	cout << "\n[delete meeting] [title]" << endl;
  	cout << "[delete meeting] ";
  	string meeting;
  	cin >> meeting;
    if(m_agendaService.deleteMeeting(m_userName,meeting))
    	cout << "[delete meeting] succeed!" << endl;
    else
    	cout << "[error] delete meeting fail!" << endl;
    cout << endl;
  }

  /**
   * delete all meetings that this user sponsored
   */
  void AgendaUI::deleteAllMeetings(void) {
  	cout << endl;
    if(m_agendaService.deleteAllMeetings(m_userName))
    	cout << "[delete all meetings] succeed!" << endl;
    else
    	cout << "[error] delete all meeting fail!" << endl;
    cout << endl;
    
  }

  /**
   * show the meetings in the screen
   */
  void AgendaUI::printMeetings(const std::list<Meeting> &t_meetings) {
      cout << left <<setw(15) <<"title" << setw(15) << "sponsor" << setw(18)
           << "start time" << setw(18) << "end time" << "participators" << endl;
       for(auto it = t_meetings.begin(); it != t_meetings.end(); it++) {
       	cout << setw(15) << it->getTitle()
       	     <<setw(15) << it->getSponsor()
       	     <<setw(18) << Date::dateToString(it->getStartDate())
       	     <<setw(18) << Date::dateToString(it->getEndDate())
       	     << printPart(it->getParticipator()) << endl;
       }
       cout << endl;
  }

  