#include "sessionmanager.h"

SessionManager& SessionManager::instance() {
    static SessionManager instance;
    return instance;
}

SessionManager::SessionManager() : userId(-1), isLoggedIn(false) {}

SessionManager::~SessionManager() {}

void SessionManager::login(int userId, const QString& email) {
    this->userId = userId;
    this->userEmail = email;
    this->isLoggedIn = true;
}

void SessionManager::logout() {
    userId = -1;
    userEmail.clear();
    isLoggedIn = false;
}

int SessionManager::getUserId() const {
    return userId;
}

QString SessionManager::getUserEmail() const {
    return userEmail;
}

bool SessionManager::isUserLoggedIn() const {
    return isLoggedIn;
}
