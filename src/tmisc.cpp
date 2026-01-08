#include "tmisc.h"

QString basename(const QString& path)
{
    qsizetype pos = path.lastIndexOf('/');

    if (pos < 0)
        return path;

    return path.mid(pos + 1);
}

QString pathname(const QString& path)
{
    qsizetype pos = path.lastIndexOf('/');

    if (pos < 0)
        return ".";

    return path.left(pos);
}
