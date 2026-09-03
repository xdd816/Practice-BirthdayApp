#ifndef MYEVENT_H
#define MYEVENT_H
#include <QDate>
#include <QString>

class MyEvent
{
public:
    MyEvent();
    MyEvent(QString name, QDate date);
    MyEvent(QString name, QDate date, QString notes);

    QString getName() const;
    void setName(const QString &newName);

    QDate getDate() const;
    void setDate(const QDate &newDate);

    QString getNotes() const;
    void setNotes(const QString &newNotes);

private:
    QString name;
    QDate date;
    QString notes;
};

#endif