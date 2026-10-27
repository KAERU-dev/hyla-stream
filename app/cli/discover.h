#pragma once

#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QSharedPointer>

class MdnsPendingComputer;

namespace QMdnsEngine
{
class Browser;
class Server;
}

namespace CliDiscover
{

// Lists GameStream hosts found over mDNS, without adding them to the saved
// hosts, so Hyla can offer them for pairing.
class Launcher : public QObject
{
    Q_OBJECT

public:
    explicit Launcher(int timeoutSeconds, bool printJson, QObject *parent = nullptr);
    ~Launcher();

    void execute();

private:
    void finish();

    int m_TimeoutSeconds;
    bool m_PrintJson;
    QSharedPointer<QMdnsEngine::Server> m_Server;
    QMdnsEngine::Browser *m_Browser;
    QList<MdnsPendingComputer *> m_Pending;
    QMap<QString, QJsonObject> m_Hosts;
};

}
