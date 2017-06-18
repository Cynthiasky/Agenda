#include <iostream>
#include <vector>
#include "User.hpp"
#include "Date.hpp"
//#include "Meeting.hpp"
//#include "Storage.hpp"
//#include "AgendaService.hpp"

using namespace std;

void testUser() {
	User test1;
	string in;
	cin >> in;
	test1.setEmail(in);
	cin >> in;
	test1.setPhone(in);
	cin >> in;
	test1.setName(in);
	cin >> in;
	test1.setPassword(in);
	cout << test1.getName() << "    " << test1.getPassword() << "    " << test1.getEmail() << "    " << test1.getPhone() << endl;
}

void testDate() {
	
    bool flag;
    Date date(2016,7,7,10,01);
    Date date1 = date;
    (date == date1) ? flag = 1 : flag = 0;
    cout << flag << endl;
    (date > date1) ? flag = 1 : flag = 0;
    cout << flag << endl;
    (date < date1) ? flag = 1 : flag = 0;
    cout << flag << endl;
    (date >= date1) ? flag = 1 : flag = 0;
    cout << flag << endl;
    (date <= date1) ? flag = 1 : flag = 0;
    cout << flag << endl;
    /*(test1 > test2) ? flag = 1 : flag = 0;
    cout << flag << endl;
    (Date("9999-10-10/10:10") > Date("9999-13-10/09:09")) ? flag = 1 : flag = 0;
    cout << flag << endl;
    (Date("9999-10-10/10:10") < Date("9999-12-10/09:09")) ? flag = 1 : flag = 0;
     cout << flag << endl;*/
    //(Date("9999-10-10/10:10") == Date("9999-10-10/10:10")) ? flag = 1 : flag = 0;
    //cout << flag << endl;
    /*(Date("9999-10-10/10:10") >= Date("9999-12-10/09:09")) ? flag = 1 : flag = 0;
   cout << flag << endl;
   (Date("9999-10-10/10:10") <= Date("9999-12-10/09:09")) ? flag = 1 : flag = 0;
   cout << flag << endl;*/
}

/*void testMeeting() {
    Meeting test1;
    vector<string> v{"b","c","d"};
    test1.setParticipator(v);
    cout << test1.getParticipator()[2] << endl;

}

void testStorage() {
    shared_ptr<Storage> test1 = Storage::getInstance();
    vector<string> v{"b","c","d"};
    //vector<string> v;
    Date date1("1111-01-01/11:11");
    Date date2("2222-02-02/22:22");
    Meeting testm("aaa",v,date1,date2,"haha");
    test1->createMeeting(testm);
    test1->createUser(User("a","1111","123@456","131313"));
    test1->sync();

    shared_ptr<Storage> test2 = Storage::getInstance();
    test2->deleteMeeting([&](Meeting it){return it.getSponsor() == "maomao";});
    //test2->sync();
}

void testService() {
    AgendaService test;
    cout << "success" << endl;
}*/

int main() {
	//testUser();
	testDate();
	//testMeeting();
	//testStorage();
    //testService();
	return 0;
}
