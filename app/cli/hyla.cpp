#include "hyla.h"

#include "cli/pair.h"
#include "cli/startstream.h"
#include "streaming/session.h"

#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QMap>

#include <cstdio>

#ifdef Q_OS_WIN32
#include <qt_windows.h>
#include <shellapi.h>
#endif

namespace Hyla
{

QStringList earlyArguments(int argc, char *argv[])
{
    QStringList args;
#ifdef Q_OS_WIN32
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    int count = 0;
    LPWSTR *wideArgs = CommandLineToArgvW(GetCommandLineW(), &count);
    if (wideArgs != nullptr) {
        for (int i = 0; i < count; i++) {
            args.append(QString::fromWCharArray(wideArgs[i]));
        }
        LocalFree(wideArgs);
    }
#else
    for (int i = 0; i < argc; i++) {
        args.append(QString::fromLocal8Bit(argv[i]));
    }
#endif
    return args;
}

QString dataDirectory(const QStringList &args)
{
    QString dir;
    for (int i = 1; i < args.size(); i++) {
        const QString &arg = args.at(i);

        // QCommandLineParser accepts long options with one or two dashes
        for (const QString &name : {QStringLiteral("--data-dir"), QStringLiteral("-data-dir")}) {
            if (arg == name && i + 1 < args.size()) {
                dir = args.at(i + 1);
            }
            else if (arg.startsWith(name + '=')) {
                dir = arg.mid(name.length() + 1);
            }
        }
    }

    if (dir.isEmpty()) {
        dir = qEnvironmentVariable("HYLA_STREAM_DATA_DIR");
    }

    return dir.isEmpty() ? QString() : QDir(dir).absolutePath();
}

bool isStatusRequested(const QStringList &args)
{
    return args.contains("--hyla") || args.contains("-hyla");
}

int exitCodeForError(const QString &code)
{
    static const QMap<QString, int> exitCodes = {
        {"host_unreachable", ExitHostUnreachable},
        {"not_paired", ExitNotPaired},
        {"already_paired", ExitAlreadyPaired},
        {"app_not_found", ExitAppNotFound},
        {"app_running", ExitAppRunning},
        {"pairing_failed", ExitPairingFailed},
        {"stream_failed", ExitStreamFailed},
    };

    Q_ASSERT(exitCodes.contains(code));
    return exitCodes.value(code, ExitStreamFailed);
}

StatusReporter::StatusReporter(bool enabled, QObject *parent)
    : QObject(parent),
      m_Enabled(enabled)
{
}

bool StatusReporter::isEnabled() const
{
    return m_Enabled;
}

void StatusReporter::watchStream(CliStartStream::Launcher *launcher, const QString &host, const QString &app)
{
    if (!m_Enabled) {
        return;
    }

    connect(launcher, &CliStartStream::Launcher::searchingComputer, this, [this, host, app] {
        report("starting", {{"host", host}, {"app", app}});
    });
    connect(launcher, &CliStartStream::Launcher::failed, this, [this, launcher](QString text) {
        fail(launcher->failureCode(), text);
    });
    // The GUI asks whether to quit the running app; Hyla decides that itself
    connect(launcher, &CliStartStream::Launcher::appQuitRequired, this, [this](QString appName) {
        fail("app_running", QObject::tr("%1 is already running on the host").arg(appName));
    });
    connect(launcher, &CliStartStream::Launcher::sessionCreated, this, [this](QString, Session *session) {
        connect(session, &Session::connectionStarted, this, [this] {
            report("streaming");
        });
    });
}

void StatusReporter::watchPair(CliPair::Launcher *launcher)
{
    if (!m_Enabled) {
        return;
    }

    connect(launcher, &CliPair::Launcher::pairing, this, [this](QString pcName, QString pin) {
        report("pairing", {{"host", pcName}, {"pin", pin}});
    });
    connect(launcher, &CliPair::Launcher::success, this, [this] {
        report("paired");
        QCoreApplication::exit(ExitSuccess);
    });
    connect(launcher, &CliPair::Launcher::failed, this, [this, launcher](QString text) {
        fail(launcher->failureCode(), text);
    });
}

void StatusReporter::sessionFinished(const QString &errorText)
{
    if (errorText.isEmpty()) {
        report("ended");
        QCoreApplication::exit(ExitSuccess);
    }
    else {
        fail("stream_failed", errorText);
    }
}

void StatusReporter::report(const QString &status, QJsonObject fields)
{
    fields.insert("status", status);
    fprintf(stdout, "%s\n", QJsonDocument(fields).toJson(QJsonDocument::Compact).constData());
    fflush(stdout);
}

void StatusReporter::fail(const QString &code, const QString &message)
{
    report("error", {{"code", code}, {"message", message}});
    QCoreApplication::exit(exitCodeForError(code));
}

}
