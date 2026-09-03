#ifndef STYLEHELPER_H
#define STYLEHELPER_H

#include <QByteArray>
#include <QFile>

class StyleHelper
{
public:
    StyleHelper();
    ~StyleHelper();

    static QByteArray fileRead(const QString& fileName);
    static QByteArray inputStyles();
    static QByteArray listStyles();
    static QByteArray mainStyles();
};

#endif
