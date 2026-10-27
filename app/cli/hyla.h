#pragma once

#include <QStringList>

// Options used when Hyla Launcher drives this client. The contract is
// documented in README.md.
namespace Hyla
{

// Returns the arguments before QCoreApplication exists, decoded without loss
// on Windows, where argv uses the ANSI code page.
QStringList earlyArguments(int argc, char *argv[]);

// The --data-dir value, or the HYLA_STREAM_DATA_DIR environment variable,
// as an absolute path. Empty when neither is set.
QString dataDirectory(const QStringList &args);

}
