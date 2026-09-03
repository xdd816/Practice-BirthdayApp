#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H
#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QLayout>
#include <memory>
#include "stylehelper.h"
#include "work_with_json/json_work.h"
#include "work_with_json/jsonfilemanager.h"
#include "myevent.h"
#include "myeventconfigurationform.h"

class WidgetFactory
{
public:
    static QWidget* getNewEventWidget(const QString& nameUser, const QString& dateUser, std::function<void()> deleteBtnActions, std::function<void()> editBtnActions);
    static void generateWidgetsFromJson(QLayout *targetLayout, bool isLimitedCount = true);
private:
    static MyEvent *myEvent;
    static MyEventConfigurationForm *myEventConfigurationForm;
};

#endif
