#ifndef EVENTLISTMODEL_H
#define EVENTLISTMODEL_H

#include <QAbstractListModel>
#include "myevent.h"

class EventListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EventRoles {
        NameRole = Qt::UserRole + 1,
        DateRole,
        NotesRole
    };

    explicit EventListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addEvent(const QString &name, const QDate &date);
    Q_INVOKABLE void loadEvents();
    Q_INVOKABLE void saveNewEvent(const QString &name, const QString &dateStr, const QString &notes);
    //met for delete
    Q_INVOKABLE void deleteEvent(int index);
    Q_INVOKABLE QString checkTodaysBirthdays();

private:
    QList<MyEvent> m_events;
};

#endif