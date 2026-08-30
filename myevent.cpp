#include "myevent.h"

MyEvent::MyEvent() : date(QDate::currentDate()), notes("") {}

MyEvent::MyEvent(QString name, QDate date) : name(name), date(date), notes("") {}

MyEvent::MyEvent(QString name, QDate date, QString notes) : name(name), date(date), notes(notes) {}

QString MyEvent::getName() const {
    return name;
}

void MyEvent::setName(const QString &newName) {
    name = newName;
}

QDate MyEvent::getDate() const {
    return date;
}

void MyEvent::setDate(const QDate &newDate) {
    date = newDate;
}

QString MyEvent::getNotes() const {
    return notes;
}

void MyEvent::setNotes(const QString &newNotes) {
    notes = newNotes;
}