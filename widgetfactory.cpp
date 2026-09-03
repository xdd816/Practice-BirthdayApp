#include "widgetfactory.h"

#include <QJsonArray>
#include <QJsonObject>

MainWindow *WidgetFactory::mainWindow = nullptr;
MyEvent *WidgetFactory::myEvent = nullptr;
MyEventConfigurationForm *WidgetFactory::myEventConfigurationForm = nullptr;

WidgetFactory::WidgetFactory(MainWindow *mainWindow, MyEvent *myEvent, MyEventConfigurationForm *myEventConfigurationForm)
{
    WidgetFactory::mainWindow = mainWindow;
    WidgetFactory::myEvent = myEvent;
    WidgetFactory::myEventConfigurationForm = myEventConfigurationForm;
}

QWidget *WidgetFactory::getNewEventWidget(const QString &nameUser, const QString &dateUser, std::function<void()> deleteBtnActions, std::function<void()> editBtnActions)
{
    std::unique_ptr<QGridLayout> layOneUser = std::make_unique<QGridLayout>();
    std::unique_ptr<QFrame> frLayWithData = std::make_unique<QFrame>();

    QDate currentDay = QDate::currentDate();
    QDate dateFromString = QDate::fromString(dateUser, "yyyy-MM-dd");

    QString formattedDate = dateFromString.toString("dd.MM");
    QString daysToBirthday = " (Days to Birthday: " + QString::number(currentDay.daysTo(dateFromString)) + ")";

    std::unique_ptr<QLabel> lblUserName = std::make_unique<QLabel>(nameUser);
    std::unique_ptr<QLabel> lblUserDate = std::make_unique<QLabel>(formattedDate + daysToBirthday);

    QFont userNameFont = lblUserName->font();
    userNameFont.setBold(true);
    lblUserName->setFont(userNameFont);

    std::unique_ptr<QPushButton> deleteButton = std::make_unique<QPushButton>();
    deleteButton->setText("Delete");
    deleteButton->setMinimumSize(80, 20);
    deleteButton->setMaximumSize(81, 20);
    deleteButton->setStyleSheet(StyleHelper::listStyles());

    QObject::connect(deleteButton.get(), &QPushButton::clicked, [deleteBtnActions]() {
        deleteBtnActions();
    });

    std::unique_ptr<QPushButton> editButton = std::make_unique<QPushButton>();
    editButton->setText("Edit");
    editButton->setMinimumSize(80, 20);
    editButton->setMaximumSize(81, 20);
    editButton->setStyleSheet(StyleHelper::listStyles());

    QObject::connect(editButton.get(), &QPushButton::clicked, [editBtnActions]() {
        editBtnActions();
    });

    layOneUser->addWidget(lblUserName.release(), 0, 0);
    layOneUser->addWidget(deleteButton.release(), 0, 1);
    layOneUser->addWidget(lblUserDate.release(), 1, 0);
    layOneUser->addWidget(editButton.release(), 1, 1);

    frLayWithData->setLayout(layOneUser.release());

    return frLayWithData.release();
}

void WidgetFactory::generateWidgetsFromJson(QLayout *targetLayout, bool isLimitedCount)
{
    QLayoutItem* wItem;
    while ((wItem = targetLayout->takeAt(0)) != nullptr) {
        QWidget* widgetToRemove = wItem->widget();
        if (widgetToRemove) {
            widgetToRemove->deleteLater();
        }
        delete wItem;
    }

    JSONFileManager jsonManager;
    QJsonArray jArr = jsonManager.readFromJsonArray();
    QString nameUser, dateUserStr;
    QDate dateUser;

    int row = 0;
    int col = 0;
    const int maxCols = 3;

    for (const QJsonValue &value : jArr)
    {
        if (targetLayout->count() >= 6 && isLimitedCount) return;
        QJsonObject jsonObj = value.toObject();
        nameUser = jsonObj.value("Name").toString();
        dateUserStr = jsonObj.value("Date").toString();
        dateUser = QDate::fromString(dateUserStr, "yyyy-MM-dd");

        QWidget* eventWidget = WidgetFactory::getNewEventWidget(nameUser, dateUserStr;
            [dateUserStr, nameUser, targetLayout]() {
                JSONWork::deleteFromJson(nameUser, dateUserStr);
                generateWidgetsFromJson(targetLayout);
            },
            [dateUser, dateUserStr, nameUser, targetLayout]() {
                myEvent->setName(nameUser);
                myEvent->setDate(dateUser);
                myEventConfigurationForm->updateInputInfo();
                JSONWork::deleteFromJson(nameUser, dateUserStr);
                mainWindow->onBtnAddClicked();
                generateWidgetsFromJson(targetLayout);
            }
        );
        if (eventWidget) {
            if (QGridLayout *gridLayout = qobject_cast<QGridLayout*>(targetLayout)) {
                gridLayout->addWidget(eventWidget, row, col);
                col++;
                if (col >= maxCols) {
                    col = 0;
                    row++;
                }
            } else {
                targetLayout->addWidget(eventWidget);
            }
        }
    }
}
