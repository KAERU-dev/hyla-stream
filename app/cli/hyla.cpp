#include "hyla.h"

#include <QDir>

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

}
