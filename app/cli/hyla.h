#pragma once

#include <QJsonObject>
#include <QObject>
#include <QStringList>

namespace CliStartStream
{
class Launcher;
}

namespace CliPair
{
class Launcher;
}

// Options and status output used when Hyla Launcher drives this client.
// The contract (options, status lines and exit codes) is documented in README.md.
namespace Hyla
{

// Exit codes for --hyla runs. 1 stays the code for invalid arguments.
enum ExitCode {
    ExitSuccess = 0,
    ExitHostUnreachable = 10,
    ExitNotPaired = 11,
    ExitAlreadyPaired = 12,
    ExitAppNotFound = 13,
    ExitAppRunning = 14,
    ExitPairingFailed = 15,
    ExitStreamFailed = 16,
};

// Returns the arguments before QCoreApplication exists, decoded without loss
// on Windows, where argv uses the ANSI code page.
QStringList earlyArguments(int argc, char *argv[]);

// The --data-dir value, or the HYLA_STREAM_DATA_DIR environment variable,
// as an absolute path. Empty when neither is set.
QString dataDirectory(const QStringList &args);

bool isStatusRequested(const QStringList &args);

int exitCodeForError(const QString &code);

class StatusReporter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled CONSTANT)

public:
    explicit StatusReporter(bool enabled, QObject *parent = nullptr);

    bool isEnabled() const;

    void watchStream(CliStartStream::Launcher *launcher, const QString &host, const QString &app);
    void watchPair(CliPair::Launcher *launcher);

    // Called by StreamSegue.qml instead of showing its error dialog.
    Q_INVOKABLE void sessionFinished(const QString &errorText);

private:
    void report(const QString &status, QJsonObject fields = {});
    void fail(const QString &code, const QString &message);

    bool m_Enabled;
};

}
