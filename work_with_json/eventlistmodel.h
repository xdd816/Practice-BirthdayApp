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
        DateRole
    };

    explicit EventListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addEvent(const QString &name, const QDate &date);

private:
    QList<MyEvent> m_events;
};

#endif