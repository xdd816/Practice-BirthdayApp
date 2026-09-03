#include "eventlistmodel.h"

EventListModel::EventListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_events.append(MyEvent("Мама", QDate(1975, 5, 12)));
    m_events.append(MyEvent("Брат", QDate(2005, 11, 30)));
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

    return QVariant();
}

QHash<int, QByteArray> EventListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DateRole] = "date";
    return roles;
}

void EventListModel::addEvent(const QString &name, const QDate &date)
{
    beginInsertRows(QModelIndex(), m_events.count(), m_events.count());
    m_events.append(MyEvent(name, date));
    endInsertRows();
}