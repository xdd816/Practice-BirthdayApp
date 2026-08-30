#include "eventlistmodel.h"

EventListModel::EventListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // Тимчасово додамо пару записів прямо тут, щоб переконатися, що C++ передає дані в QML
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

    // Повертаємо дані залежно від того, що просить QML
    if (role == NameRole)
        return event.getName();
    else if (role == DateRole)
        return event.getDate().toString("dd.MM.yyyy"); // Красиво форматуємо дату

    return QVariant();
}

QHash<int, QByteArray> EventListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name"; // Це слово ми використовуємо в QML (text: name)
    roles[DateRole] = "date"; // Це слово ми використовуємо в QML (text: date)
    return roles;
}

void EventListModel::addEvent(const QString &name, const QDate &date)
{
    // Правильне додавання рядка, щоб інтерфейс автоматично оновився
    beginInsertRows(QModelIndex(), m_events.count(), m_events.count());
    m_events.append(MyEvent(name, date));
    endInsertRows();
}