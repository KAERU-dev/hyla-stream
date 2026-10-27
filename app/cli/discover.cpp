#include "discover.h"

#include "backend/computermanager.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

#include <cstdio>

namespace CliDiscover
{

Launcher::Launcher(int timeoutSeconds, bool printJson, QObject *parent)
    : QObject(parent),
      m_TimeoutSeconds(timeoutSeconds),
      m_PrintJson(printJson),
      m_Browser(nullptr)
{
}

Launcher::~Launcher()
{
    qDeleteAll(m_Pending);
    delete m_Browser;
}

void Launcher::execute()
{
    m_Server.reset(new QMdnsEngine::Server());
    m_Browser = new QMdnsEngine::Browser(m_Server.data(), "_nvstream._tcp.local.");
    connect(m_Browser, &QMdnsEngine::Browser::serviceAdded,
            this, [this](const QMdnsEngine::Service &service) {
        auto pending = new MdnsPendingComputer(m_Server, service);
        connect(pending, &MdnsPendingComputer::resolvedHost,
                this, [this](MdnsPendingComputer *computer, QVector<QHostAddress> &addresses) {
            QJsonArray addressList;
            for (const QHostAddress &address : std::as_const(addresses)) {
                if (!addressList.contains(address.toString())) {
                    addressList.append(address.toString());
                }
            }

            // A host is announced once per network interface; keep one entry per name
            QString hostname = computer->hostname();
            if (hostname.endsWith('.')) {
                hostname.chop(1);
            }
            m_Hosts.insert(hostname, QJsonObject {
                {"hostname", hostname},
                {"port", computer->port()},
                {"addresses", addressList},
            });
        });
        m_Pending.append(pending);
    });

    QTimer::singleShot(m_TimeoutSeconds * 1000, this, &Launcher::finish);
}

void Launcher::finish()
{
    if (m_PrintJson) {
        QJsonArray hosts;
        for (const QJsonObject &host : std::as_const(m_Hosts)) {
            hosts.append(host);
        }
        fprintf(stdout, "%s\n", QJsonDocument(QJsonObject {{"hosts", hosts}}).toJson(QJsonDocument::Compact).constData());
    }
    else {
        for (const QJsonObject &host : std::as_const(m_Hosts)) {
            fprintf(stdout, "%s\t%s\n", qPrintable(host["hostname"].toString()),
                    qPrintable(host["addresses"].toVariant().toStringList().join(", ")));
        }
    }
    fflush(stdout);

    QCoreApplication::exit(0);
}

}
