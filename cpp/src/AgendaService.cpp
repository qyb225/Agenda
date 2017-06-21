#include <functional>
#include "AgendaService.hpp"
#include "Date.hpp"
#include "Meeting.hpp"
using namespace std;

AgendaService::AgendaService() {
    startAgenda();
}

void AgendaService::startAgenda() {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda() {
    m_storage->sync();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

bool AgendaService::userLogIn(const string &userName, const string &password) {
    if (m_storage->queryUser([=](const User& user)->bool {
        return user.getName() == userName;
    }).empty()) {
        return false;
    }
    if (m_storage->queryUser([=](const User& user)->bool {
        return user.getName() == userName && user.getPassword() == password;
    }).empty()) {
        return false;
    }
    return true;
}

bool AgendaService::userRegister(const string &userName, const string &password,
                    const string &email, const string &phone) {
    if (!m_storage->queryUser([=](const User& user)->bool {
        return user.getName() == userName;
    }).empty()) {
        return false;
    }
    m_storage->createUser(User(userName, password, email, phone));
    return true;
}

bool AgendaService::deleteUser(const string &userName, const string &password) {
    if (m_storage->deleteUser([=](const User& user)->bool {
        return user.getName() == userName && user.getPassword() == password;
    }) == 0) {
        return false;
    }
    m_storage->deleteMeeting([=](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName;
    });
    m_storage->updateMeeting([=](const Meeting& meeting)->bool {
        return meeting.isParticipator(userName);
    }, [=](Meeting& meeting) {
        meeting.removeParticipator(userName);
    });
    m_storage->deleteMeeting([](const Meeting& meeting)->bool {
        return meeting.getParticipator().size() == 0;
    });
    return true;
}

list<User> AgendaService::listAllUsers() const {
    return m_storage->queryUser([](const User& user)->bool {
        return true;
    });
}

bool AgendaService::createMeeting(const string &userName, const string &title,
                     const string &startDate, const string &endDate, const vector<string> &participator) {
    Date newStart(startDate);
    Date newEnd(endDate);
    if (!Date::isValid(newStart) || !Date::isValid(newEnd) || newStart >= newEnd) {
        return false;
    }
    if (!m_storage->queryMeeting([&title](const Meeting& meeting)->bool {
        return meeting.getTitle() == title;
    }).empty()) {
        return false;
    }
    if (m_storage->queryUser([&userName](const User& user)->bool {
        return user.getName() == userName;
    }).empty()) {
        return false;
    }
    for (int i = 0; i < participator.size() - 1; ++i) {
        for (int j = i + 1; j < participator.size(); ++j) {
            if (participator[i] == participator[j]) {
                return false;
            }
        }
    }
    for (auto part : participator) {
        if (userName == part) {
            return false;
        }
        if (m_storage->queryUser([&part](const User& user)->bool {
            return user.getName() == part;
        }).empty()) {
            return false;
        }
        list<Meeting> allPartMeetings = listAllMeetings(part);
        for (auto partMeeting : allPartMeetings) {
            if (partMeeting.getStartDate() >= newStart && partMeeting.getStartDate() < newEnd) {
                return false;
            }
            if (partMeeting.getEndDate() > newStart && partMeeting.getEndDate() <= newEnd) {
                return false;
            }
            if (partMeeting.getStartDate() <= newStart && partMeeting.getEndDate() >= newEnd) {
                return false;
            }
        }
    }
    list<Meeting> allSponsorMeetings = listAllMeetings(userName);
    for (auto sponsorMeeting : allSponsorMeetings) {
        if (sponsorMeeting.getStartDate() >= newStart && sponsorMeeting.getStartDate() < newEnd) {
            return false;
        }
        if (sponsorMeeting.getEndDate() > newStart && sponsorMeeting.getEndDate() <= newEnd) {
            return false;
        }
        if (sponsorMeeting.getStartDate() <= newStart && sponsorMeeting.getEndDate() >= newEnd) {
            return false;
        }
    }
    m_storage->createMeeting(Meeting(userName, participator, newStart, newEnd, title));
    return true;
}

bool AgendaService::addMeetingParticipator(const string &userName,
                              const string &title, const string &participator) {
    if (userName == participator) {
        return false;
    }
    if (m_storage->queryUser([&participator](const User& user)->bool {
        return user.getName() == participator;
    }).empty()) {
        return false;
    }

    list<Meeting> theMeeting = m_storage->queryMeeting([=](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName && meeting.getTitle() == title;
    });
    if (theMeeting.size() != 1) {
        return false;
    }
    Meeting thisMeeting = theMeeting.front();
    Date newStart = thisMeeting.getStartDate();
    Date newEnd = thisMeeting.getEndDate();

    list<Meeting> allPartMeetings = listAllMeetings(participator);
    for (auto partMeeting : allPartMeetings) {
        if (partMeeting.getStartDate() >= newStart && partMeeting.getStartDate() < newEnd) {
            return false;
        }
        if (partMeeting.getEndDate() > newStart && partMeeting.getEndDate() <= newEnd) {
            return false;
        }
        if (partMeeting.getStartDate() <= newStart && partMeeting.getEndDate() >= newEnd) {
            return false;
        }
    }
    return m_storage->updateMeeting([=](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName && meeting.getTitle() == title 
             && !meeting.isParticipator(participator);
    }, [=](Meeting& meeting) {
        meeting.addParticipator(participator);
    });
}

bool AgendaService::removeMeetingParticipator(const string &userName,
                         const string &title, const string &participator) {
    if (m_storage->updateMeeting([=](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName && meeting.getTitle() == title
               && meeting.isParticipator(participator);
    }, [participator](Meeting& meeting) {
        meeting.removeParticipator(participator);
    })) {
        m_storage->deleteMeeting([](const Meeting& meeting)->bool {
            return meeting.getParticipator().size() == 0;
        });
        return true;
    }
    return false;
}

bool AgendaService::quitMeeting(const string &userName, const string &title) {
    if (m_storage->updateMeeting([userName, title](const Meeting& meeting)->bool {
        return meeting.isParticipator(userName) && meeting.getTitle() == title;
    }, [&userName](Meeting& meeting) {
        meeting.removeParticipator(userName);
    })) {
        m_storage->deleteMeeting([](const Meeting& meeting)->bool {
            return meeting.getParticipator().size() == 0;
        });
        return true;
    }
    return false;
}

list<Meeting> AgendaService::meetingQuery(const string &userName, const string &title) const {
    return m_storage->queryMeeting([userName, title](const Meeting& meeting)->bool {
        return (meeting.getSponsor() == userName || meeting.isParticipator(userName))
                && meeting.getTitle() == title;
    });
}

list<Meeting> AgendaService::meetingQuery(const string &userName,
                     const string &startDate, const string &endDate) const {
    Date startTime(startDate);
    Date endTime(endDate);
    list<Meeting> ans;
    if (startTime > endTime) {
        return ans;
    }
    list<Meeting> allMeetings = listAllMeetings(userName);
    for (auto meeting : allMeetings) {
        if (meeting.getStartDate() >= startTime && meeting.getStartDate() <= endTime) {
            ans.push_back(meeting);
        }
        else if (meeting.getEndDate() >= startTime && meeting.getEndDate() <= endTime) {
            ans.push_back(meeting);
        }
        else if (meeting.getStartDate() <= startTime && meeting.getEndDate() >= endTime) {
            ans.push_back(meeting);
        }
    }
    return ans;
}

list<Meeting> AgendaService::listAllMeetings(const string &userName) const {
    return m_storage->queryMeeting([=](const Meeting& meeting) {
        return meeting.getSponsor() == userName || meeting.isParticipator(userName);
    });
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string &userName) const {
    return m_storage->queryMeeting([=](const Meeting& meeting) {
        return meeting.getSponsor() == userName;
    });
}

list<Meeting> AgendaService::listAllParticipateMeetings(const string &userName) const {
    return m_storage->queryMeeting([=](const Meeting& meeting) {
        return meeting.isParticipator(userName);
    });
}

bool AgendaService::deleteMeeting(const string &userName, const string &title) {
    return m_storage->deleteMeeting([=](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName && meeting.getTitle() == title;
    });
}

bool AgendaService::deleteAllMeetings(const string &userName) {
    return m_storage->deleteMeeting([=](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName;
    });
}