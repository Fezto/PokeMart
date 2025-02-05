#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>

class SessionManager {
public:
    static SessionManager& instance();

    void login(int userId, const QString& email);
    void logout();

    int getUserId() const;
    QString getUserEmail() const;
    bool isUserLoggedIn() const;

private:
    SessionManager();
    ~SessionManager();

    int userId;
    QString userEmail;
    bool isLoggedIn;

    // Evitar copia
    SessionManager(const SessionManager&) = delete;
    void operator=(const SessionManager&) = delete;
};

#endif // SESSIONMANAGER_H
