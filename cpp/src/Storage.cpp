#include <memory>
#include <fstream>
#include <iostream>
#include "Path.hpp"
#include "Storage.hpp"
using namespace std;
shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
    readFromFile();
}

bool Storage::readFromFile() {
    string fileLine;
    ifstream fin;
    fin.open(Path::userPath, ios::in);
    while (!fin) {
        ofstream createFile;
        createFile.open(Path::userPath, ios::out);
        if (!createFile) {
            return false;
        }
        createFile.close();
        fin.open(Path::userPath, ios::in);
    }
    while (getline(fin, fileLine)) {
        int i;
        string userName, passWord, eMail, telePhone;
        i = fileLine.find(",");
        userName = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.find(",");
        passWord = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.find(",");
        eMail = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.rfind("\"");
        telePhone = fileLine.substr(1, i - 1);
        fileLine.erase(0);

        m_userList.push_back(User(userName, passWord, eMail, telePhone));
    }
    fin.close();
    fileLine = "";
    fin.open(Path::meetingPath, ios::in);
    while (!fin) {
        ofstream createFile;
        createFile.open(Path::meetingPath, ios::out);
        if (!createFile) {
            return false;
        }
        createFile.close();
        fin.open(Path::meetingPath, ios::in);
    }
    while (getline(fin, fileLine)) {
        int i;
        string sponsor, participator, startDate, endDate, title;

        i = fileLine.find(",");
        sponsor = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.find(",");
        participator = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.find(",");
        startDate = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.find(",");
        endDate = fileLine.substr(1, i - 2);
        fileLine.erase(0, i + 1);

        i = fileLine.rfind("\"");
        title = fileLine.substr(1, i - 1);
        fileLine.erase(0);
        vector<string> allParticipators;
        int j;
        while ((j = participator.find("&")) != string::npos) {
            allParticipators.push_back(participator.substr(0, j));
            participator.erase(0, j + 1);
            if (participator.find("&") == string::npos) {
                allParticipators.push_back(participator);
            }
        }
        m_meetingList.push_back(Meeting(sponsor, allParticipators, Date(startDate), Date(endDate), title));
    }
    fin.close();
    return true;
}

bool Storage::writeToFile() {
    ofstream fout;
    fout.open(Path::userPath, ios::out);
    if(!fout) {
        return false;
    }
    else {
        for(auto user : m_userList) {
            fout << "\"" << user.getName() << "\",";
            fout << "\"" << user.getPassword() << "\",";
            fout << "\"" << user.getEmail() << "\",";
            fout << "\"" << user.getPhone() << "\"" << endl;
        }
    }
    fout.close();
    fout.open(Path::meetingPath, ios::out);
    if (!fout) {
        return false;
    }
    else {
        for (auto meeting : m_meetingList) {
            fout << "\"" << meeting.getSponsor() << "\",\"";
            auto allParticipators = meeting.getParticipator();
            for (int i = 0; i < allParticipators.size() - 1; ++i) {
                fout << allParticipators[i] << "&";
            }
            fout << allParticipators.back() << "\",";
            fout << "\"" << Date::dateToString(meeting.getStartDate()) << "\",";
            fout << "\"" << Date::dateToString(meeting.getEndDate()) << "\",";
            fout << "\"" << meeting.getTitle() << "\"" << endl;
        }
    }
    fout.close();
    return true;
}

shared_ptr<Storage> Storage::getInstance() {
    if (m_instance == NULL) {
        Storage* p = new Storage;
        m_instance.reset(p);
    }
    return m_instance;
}

Storage::~Storage() {
    sync();
    m_instance.reset();
}

void Storage::createUser(const User& t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

list<User> Storage::queryUser(std::function<bool(const User&)> filter) const {
    list<User> queryAns;
    for (auto user : m_userList) {
        if (filter(user)) {
            queryAns.push_back(user);
        }
    }
    return queryAns;
}

int Storage::updateUser(std::function<bool(const User&)> filter,
                   std::function<void(User&)> switcher) {
    int count = 0;
    for (auto& user : m_userList) {
        if (filter(user)) {
            switcher(user);
            ++count;
            m_dirty = true;
        }
    }
    return count;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int size_ = m_userList.size();
    m_userList.remove_if(filter);
    m_dirty = size_ - m_userList.size();
    return size_ - m_userList.size();
}

void Storage::createMeeting(const Meeting& t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}

list<Meeting> Storage::queryMeeting(function<bool(const Meeting&)> filter) const {
    list<Meeting> queryAns;
    for (auto meeting : m_meetingList) {
        if (filter(meeting)) {
            queryAns.push_back(meeting);
        }
    }
    return queryAns;
}

int Storage::updateMeeting(function<bool(const Meeting&)> filter, 
                            function<void(Meeting&)> switcher) {
    int count = 0;
    for (auto& meeting : m_meetingList) {
        if (filter(meeting)) {
            switcher(meeting);
            m_dirty = true;
            ++count;
        }
    }
    return count;
}

int Storage::deleteMeeting(function<bool(const Meeting&)> filter) {
    int size_ = m_meetingList.size();
    m_meetingList.remove_if(filter);
    m_dirty = size_ - m_meetingList.size();
    return size_ - m_meetingList.size();
}

bool Storage::sync(void) {
    if (m_dirty) {
        writeToFile();
        m_dirty = false;
        return true;
    }
    return false;
}
