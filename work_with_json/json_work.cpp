#include "json_work.h"
#include "jsonfilemanager.h"


JSONWork::JSONWork(QObject *parent)
    : QObject(parent)
{

}

JSONWork::~JSONWork()
{

}


void JSONWork::writeToJson(const QString& event_name, const QDate& event_date)
{
    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArrToSort = jsonManager.readFromJsonArray();

    QJsonObject recordObject;
    recordObject.insert("Name", QJsonValue::fromVariant(event_name));
    recordObject.insert("Date", QJsonValue::fromVariant(event_date));

    jArrToSort.append(recordObject);
    sortJsonData(jArrToSort);

    jsonManager.writeJsonArray(jArrToSort);
}

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

void JSONWork::sortJsonData(QJsonArray &jarrToSort)
{

    std::sort(jarrToSort.begin(), jarrToSort.end(), [](const QJsonValue& v1, const QJsonValue& v2) {
        auto getNextBday = [](const QJsonObject& obj) {
            QString dateStr = obj.value("Date").toString();

            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            if (!date.isValid()) {
                date = QDate::fromString(dateStr, "dd.MM.yyyy");
            }

            QDate today = QDate::currentDate();

            if (date.isValid()) {
                QDate thisYearBday(today.year(), date.month(), date.day());

                if (thisYearBday < today) {
                    return thisYearBday.addYears(1);
                }
                return thisYearBday;
            }
            return QDate(2100, 1, 1);
        };

        return getNextBday(v1.toObject()) < getNextBday(v2.toObject());
    });
}

void JSONWork::deleteFromJson(const QString& nameUser, const QString& dateUser)
{
    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArr = jsonManager.readFromJsonArray();

    for (int i = 0; i < jArr.size(); ++i)
    {
        QJsonObject recordObject = jArr[i].toObject();
        QString name = recordObject.value("Name").toString();
        QString date = recordObject.value("Date").toString();
        if (name == nameUser && date == dateUser)
        {
            jArr.removeAt(i);
            break;
        }
    }

    jsonManager.writeJsonArray(jArr);
}

void JSONWork::updateJsonDataBySorting()
{
    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArrToSort = jsonManager.readFromJsonArray();
    sortJsonData(jArrToSort);
    jsonManager.writeJsonArray(jArrToSort);
}
