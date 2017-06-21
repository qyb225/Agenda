#include <iostream>
#include <iomanip>
#include "AgendaUI.hpp"

using namespace std;

AgendaUI::AgendaUI() {

}

string AgendaUI::getOperation() {
    string opcode = "";
    cin >> opcode;
    return opcode;
}

void AgendaUI::OperationLoop() {
    string opcode = "";
    while (!executeOperation(opcode)) {
        cout << endl;
        for (int i = 0; i < 10; ++i) {
            cout << "---";
        }
        cout << " Agenda ";
        for (int i = 0; i < 10; ++i) {
            cout << "---";
        }
        cout << endl;
        cout << "Action:" << endl;
        cout << left << setw(4) << "l" << "- log in Agenda by username and password" << endl;
        cout << left << setw(4) << "r" <<  "- register an Agenda account" << endl;
        cout << left << setw(4) << "q" << "- quit Agenda" << endl;
        for (int i = 0; i < 22; ++i) {
            cout << "---";
        }
        cout << "--" << endl << endl;
        cout << "Agenda : ~$ ";
        opcode = getOperation();
        if (opcode == "r") {
            userRegister();
        }
        else if (opcode == "l") {
            userLogIn();
        }
        else if (opcode == "q") {
            cout << "Bye~" << endl;
        }
        else {
            cout << "[error] cannot find command " << opcode << endl;
        }
    }
    quitAgenda();
}

bool AgendaUI::executeOperation(std::string t_operation) {
    return t_operation == "q";
}

void AgendaUI::startAgenda() {
    string startOpcode = "";
    while (startOpcode != "o") {
        cout << endl;
        for (int i = 0; i < 10; ++i) {
            cout << "---";
        }
        cout << " Agenda ";
        for (int i = 0; i < 10; ++i) {
            cout << "---";
        }
        cout << endl;
        cout << left << setw(4) << "o" << "- log out Agenda" << endl;
        cout << left << setw(4) << "dc" << "- delete Agenda account" << endl;
        cout << left << setw(4) << "lu" << "- list all users" << endl;
        cout << left << setw(4) << "cm" << "- create a meeting" << endl;
        cout << left << setw(4) << "la" << "- list all meetings" << endl;
        cout << left << setw(4) << "las" << "- list all sponsor meetings" << endl;
        cout << left << setw(4) << "lap" << "- list all participate meetings" << endl;
        cout << left << setw(4) << "qm" << "- query meeting by title" << endl;
        cout << left << setw(4) << "qt" << "- query meeting by time interval" << endl;
        cout << left << setw(4) << "dm" << "- delete meeting by title" << endl;
        cout << left << setw(4) << "da" << "- delete all meetings" << endl;

        for (int i = 0; i < 22; ++i) {
            cout << "---";
        }
        cout << "--" << endl << endl;
        cout << "Agenda@" << m_userName << " : ~# ";
        startOpcode = getOperation();
        if (startOpcode == "o") {
            userLogOut();
        }
        else if (startOpcode == "dc") {
            deleteUser();
        }
        else if (startOpcode == "lu") {
            listAllUsers();
        }
        else if (startOpcode == "cm") {
            createMeeting();
        }
        else if (startOpcode == "la") {
            listAllMeetings();
        }
        else if (startOpcode == "las") {
            listAllSponsorMeetings();
        }
        else if (startOpcode == "lap") {
            listAllParticipateMeetings();
        }
        else if (startOpcode == "qm") {
            queryMeetingByTitle();
        }
        else if (startOpcode == "qt") {
            queryMeetingByTimeInterval();
        }
        else if (startOpcode == "dm") {
            deleteMeetingByTitle();
        }
        else if (startOpcode == "da") {
            deleteAllMeetings();
        }
        else {
            cout << "[error] cannot find command " << startOpcode << endl;
        }
    }
}

void AgendaUI::userLogIn() {
    string usernameIn = "", passwordIn = "";
    cout << endl << "[log in] [user name] [password]" << endl;
    cout << "[log in] ";
    cin >> usernameIn >> passwordIn;
    if (m_agendaService.userLogIn(usernameIn, passwordIn)) {
        m_userName = usernameIn;
        m_userPassword = passwordIn;
        cout << "[log in] succeed!" << endl << endl;

        startAgenda();
    }
    else {
        cout << "[error]  log in fail" << endl << endl;
    }
}

void AgendaUI::userRegister() {
    cout << endl << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string username, password, email, tel;
    cin >> username >> password >> email >> tel;
    if (m_agendaService.userRegister(username, password, email, tel)) {
        cout << "[register] succeed!" << endl << endl;
    }
    else {
        cout << "[error] register fail!(Username has been used)" << endl << endl;
    }
}

void AgendaUI::quitAgenda() {
    m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut() {
    m_userName = "";
    m_userPassword = "";
    cout << "[log out] succeed!" << endl;
}

void AgendaUI::deleteUser(void) {
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
        cout << endl << "[delete agend account] succeed!" << endl;
    }
    else {
        cout << endl << "[error] detele fail!" << endl;
    }
}

void AgendaUI::listAllUsers() {
    cout << endl << "[list all users]" << endl << endl;
    list<User> allUsers = m_agendaService.listAllUsers();
    cout << "+------------+--------------------+--------------------+" << endl; 
    cout << "| " << left << setw(10) <<"name" << " | " << left << setw(18) << "email" 
         << " | " << left << setw(18) << "phone" << " |" << endl;
    cout << "+------------+--------------------+--------------------+" << endl;
    for(auto user : allUsers) {
        cout << "| " << left << setw(10) << user.getName() << " | " << left << setw(18) << user.getEmail()
             << " | " << left << setw(18) << user.getPhone() << " |" << endl;
    }
    cout << "+------------+--------------------+--------------------+" << endl;
}

void AgendaUI::createMeeting() {
    int numOfPartic;
    vector<string> particSet;
    cout << endl << "[create meeting] " << "[the number of participators]" << endl;
    cout << "[create meeting] ";
    cin >> numOfPartic;
    for (int i = 0; i < numOfPartic; ++i) {
        string particName;
        cout << "[create meeting] " << "[please enter the participator " << i << " ]" << endl;
        cout << "[create meeting] ";
        cin >> particName;
        particSet.push_back(particName);
    }
    string title, startDate, endDate;
    cout << "[create meeting] " << "[title][start time(yyyy-mm-dd/hh:mm)][end time(yyy-mm-dd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    cin >> title >> startDate >> endDate;
    if (m_agendaService.createMeeting(m_userName, title, startDate, endDate, particSet)) {
        cout << "[create meeting] succeed!" << endl;
    }
    else {
        cout << "[create meeting] error!" << endl;
    }
}

void AgendaUI::listAllMeetings() {
    cout << endl << "[list all meetings]" << endl << endl;
    printMeetings(m_agendaService.listAllMeetings(m_userName));
}

void AgendaUI::listAllSponsorMeetings(void) {
    cout << endl << "[list all sponsor meetings]" << endl << endl;
    printMeetings(m_agendaService.listAllSponsorMeetings(m_userName));
}

void AgendaUI::listAllParticipateMeetings() {
    cout << endl << "[list all participate meetings]" << endl << endl;
    printMeetings(m_agendaService.listAllParticipateMeetings(m_userName));
}

void AgendaUI::queryMeetingByTitle() {
    string title;
    cout << endl << "[query meeting] [title]" << endl;
    cout << "[query meeting] ";
    cin >> title;
    printMeetings(m_agendaService.meetingQuery(m_userName, title));
}

void AgendaUI::queryMeetingByTimeInterval() {
    cout << endl << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    string startTime, endTime;
    cin >> startTime >> endTime;
    printMeetings(m_agendaService.meetingQuery(m_userName, startTime, endTime));
}

void AgendaUI::deleteMeetingByTitle() {
    cout << endl << "[delete meeting] [title]" << endl;
    string title;
    cout << "[delete meeting] ";
    cin >> title;
    if (m_agendaService.deleteMeeting(m_userName, title)) {
        cout << endl << "[delete meeting by title] succeed!" << endl;
    }
    else cout << endl << "[error] detele meeting fail!" << endl;
}

void AgendaUI::deleteAllMeetings() {
    if (m_agendaService.deleteAllMeetings(m_userName)) {
        cout << endl << "[delete all meetings] succeed!" << endl;
    }
    else { 
        cout << endl << "[error] detele meetings fail!" << endl;
    }
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    cout << "+------------+-----------------+------------------+------------------+"
         << "-----------------+" << endl;
    cout << "| " << left << setw(10) << "Title" << " | " << left << setw(15) << "Sponsor" << " | " 
         << left << setw(16) << "Star time" << " | " << left << setw(16) << "End time" << " | "
         << left << setw(15) << "Participators" << " |" << endl;
    cout << "+------------+-----------------+------------------+------------------+"
         << "-----------------+" << endl;

    for (auto meeting : t_meetings) {
        vector<string> allPart = meeting.getParticipator();
        for (int i = 0; i < allPart.size(); ++i) {
            if (0 == i) {
                cout << "| " << left << setw(10) << meeting.getTitle() << " | "
                     << left << setw(15) << meeting.getSponsor() << " | "
                     << left << setw(16) << Date::dateToString(meeting.getStartDate()) << " | "
                     << left << setw(16) << Date::dateToString(meeting.getEndDate()) << " | "
                     << left << setw(15) << allPart[i] << " |" << endl;
            }
            else {
                cout << "| " << left << setw(10) << "" << " | "
                     << left << setw(15) << "" << " | "
                     << left << setw(16) << "" << " | "
                     << left << setw(16) << "" << " | "
                     << left << setw(15) << allPart[i] << " |" << endl;
            }
        }
        cout << "+------------+-----------------+------------------+------------------+"
             << "-----------------+" << endl;
    }
}