#include "eventlistmodel.h"
#include "work_with_json/jsonfilemanager.h"
#include "work_with_json/json_work.h"
#include <QJsonObject>
#include <QJsonArray>

EventListModel::EventListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadEvents();
}

void EventListModel::loadEvents()
{
    beginResetModel();
    m_events.clear();

    JSONFileManager jsonManager(QIODevice::ReadOnly);
    QJsonArray jArr = jsonManager.readFromJsonArray();

    for (int i = 0; i < jArr.size(); ++i) {
        QJsonObject obj = jArr[i].toObject();

        QString name = obj.value("Name").toString();
        QString dateStr = obj.value("Date").toString();
        QString notes = obj.value("Notes").toString();

        QDate date = QDate::fromString(dateStr, Qt::ISODate);
        if (!date.isValid()) {
            date = QDate::fromString(dateStr, "yyyy-MM-dd");
        }

        MyEvent event(name, date);
        event.setNotes(notes);
        m_events.append(event);
    }

    endResetModel();
}

int EventListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_events.count();
}

QVariant EventListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_events.count())
        return QVariant();

    const MyEvent &event = m_events[index.row()];

    if (role == NameRole)
        return event.getName();
    else if (role == DateRole)
        return event.getDate().toString("dd.MM.yyyy");
    else if (role == NotesRole)
        return event.getNotes();

    return QVariant();
}

QHash<int, QByteArray> EventListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DateRole] = "date";
    roles[NotesRole] = "notes";
    return roles;
}

void EventListModel::addEvent(const QString &name, const QDate &date)
{
    beginInsertRows(QModelIndex(), m_events.count(), m_events.count());
    m_events.append(MyEvent(name, date));
    endInsertRows();
}

void EventListModel::saveNewEvent(const QString &name, const QString &dateStr, const QString &notes)
{
    QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
    if (!date.isValid()) {
        date = QDate::fromString(dateStr, "dd.MM.yyyy");
    }

    if (date.isValid() && !name.isEmpty()) {
        JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
        QJsonArray jArr = jsonManager.readFromJsonArray();

        QJsonObject obj;
        obj["Name"] = name;
        obj["Date"] = date.toString("yyyy-MM-dd");
        obj["Notes"] = notes;

        jArr.append(obj);
        jsonManager.writeJsonArray(jArr);

        beginInsertRows(QModelIndex(), m_events.count(), m_events.count());
        MyEvent newEvent(name, date);
        newEvent.setNotes(notes);
        m_events.append(newEvent);
        endInsertRows();
    }
}

void EventListModel::deleteEvent(int index)
{
    if (index < 0 || index >= m_events.count())
        return;

    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArr = jsonManager.readFromJsonArray();

    if (index < jArr.size()) {
        jArr.removeAt(index);
        jsonManager.writeJsonArray(jArr);
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_events.removeAt(index);
    endRemoveRows();
}

QString EventListModel::checkTodaysBirthdays()
{
    JSONFileManager jsonManager(QIODevice::ReadOnly | QIODevice::Text);
    QJsonArray jArr = jsonManager.readFromJsonArray();
    QString detailsText = "";
    int counter = 0;
    QDate dateNow = QDate::currentDate();

    for (const QJsonValue& value : jArr) {
        QJsonObject obj = value.toObject();
        QDate dateFromJson = QDate::fromString(obj.value("Date").toString(), "yyyy-MM-dd");

        if (dateFromJson.isValid() && dateFromJson.month() == dateNow.month() && dateFromJson.day() == dateNow.day()) {
            QString name = obj.value("Name").toString();
            QString notes = obj.value("Notes").toString();

            detailsText += "• " + name;
            if (!notes.isEmpty()) {
                detailsText += " (" + notes + ")";
            }
            detailsText += "\n";
            ++counter;
        }
    }

    if (counter == 0) {
        return "";
    } else if (counter == 1) {
        return "Привітай сьогодні:\n" + detailsText;
    } else {
        return "Привітай друзів:\n" + detailsText;
    }
}