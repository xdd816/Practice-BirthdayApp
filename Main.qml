import QtQuick
import QtQuick.Controls
import Qt.labs.platform

ApplicationWindow {
    id: window
    visible: true
    width: 400
    height: 620 // Збільшили висоту для комфортного розміщення нотаток
    title: "Birthday Reminder"
    color: "#0f172a"

    onClosing: function(close_event) {
        close_event.accepted = false
        window.hide()
    }

    SystemTrayIcon {
        id: trayIcon
        visible: true
        tooltip: "Birthday Reminder"
        icon.source: "qrc:/graphics/birthday-cake.ico"

        menu: Menu {
            MenuItem {
                text: "Відкрити"
                onTriggered: { window.show(); window.raise(); window.requestActivate() }
            }
            MenuItem {
                text: "Вийти повністю"
                onTriggered: Qt.quit()
            }
        }
        onActivated: function(reason) {
            if (reason === SystemTrayIcon.Trigger) { window.show(); window.raise(); window.requestActivate() }
        }
    }

    Timer {
        id: checkTimer
        interval: 360000
        running: true
        repeat: true
        onTriggered: checkAndNotify()
    }

    Component.onCompleted: checkAndNotify()

    function checkAndNotify() {
        var message = myEventModel.checkTodaysBirthdays()
        if (message !== "") {
            trayIcon.showMessage("Дні народження сьогодні! 🎂", message, SystemTrayIcon.Information, 5000)
        }
    }

    // Popup для додавання / редагування
    Popup {
            id: eventDialog
            width: 320
            height: 330
            anchors.centerIn: Overlay.overlay
            modal: true
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

            property bool isEditMode: false
            property int editIndex: -1

            background: Rectangle { color: "#1e293b"; radius: 12; border.color: "#334155" }

            Column {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 12

                Text {
                    text: eventDialog.isEditMode ? "Редагувати свято" : "Нове свято"
                    font.pixelSize: 20
                    font.bold: true
                    color: "#f8fafc"
                }

                TextField {
                    id: nameInput
                    width: parent.width
                    placeholderText: "Ім'я"
                    color: "#f8fafc"                 // Білий колір введеного тексту
                    placeholderTextColor: "#94a3b8"  // Світло-сірий колір підказки
                    background: Rectangle { color: "#0f172a"; border.color: "#334155"; radius: 8 }
                }

                TextField {
                    id: dateInput
                    width: parent.width
                    placeholderText: "РРРР-ММ-ДД"
                    color: "#f8fafc"
                    placeholderTextColor: "#94a3b8"
                    background: Rectangle { color: "#0f172a"; border.color: "#334155"; radius: 8 }
                }

                TextField {
                    id: notesInput
                    width: parent.width
                    placeholderText: "Нотатка"
                    color: "#f8fafc"
                    placeholderTextColor: "#94a3b8"
                    background: Rectangle { color: "#0f172a"; border.color: "#334155"; radius: 8 }
                }

                Button {
                    text: "Зберегти"
                    width: parent.width
                    height: 40
                    background: Rectangle { color: "#2563eb"; radius: 8 }
                    contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                    onClicked: {
                        if (eventDialog.isEditMode) {
                            // Логіка редагування
                        } else {
                            myEventModel.saveNewEvent(nameInput.text, dateInput.text, notesInput.text)
                        }
                        eventDialog.close()
                    }
                }
            }
        }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "Дні народження 🎂"
            font.pixelSize: 26
            font.bold: true
            color: "#f8fafc"
        }

        ListView {
            id: birthdayList
            width: parent.width
            height: 430
            clip: true
            spacing: 10
            model: myEventModel

            delegate: Rectangle {
                width: parent.width
                height: 85 // Збільшено висоту картки для нотатки
                color: "#1e293b"
                radius: 10
                border.color: "#334155"

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    spacing: 3

                    Text { text: name; font.bold: true; font.pixelSize: 18; color: "#f8fafc" }
                    Text { text: date; font.pixelSize: 13; color: "#94a3b8" }

                    // НОВА ФІЧА: Відображення нотатки у списку
                    Text {
                        text: notes !== undefined ? notes : ""
                        font.pixelSize: 12;
                        color: "#60a5fa";
                        font.italic: true
                        visible: text !== ""
                    }
                }

                Row {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 5

                    Button {
                        text: "✏️"
                        width: 40; height: 40
                        background: Rectangle { color: "transparent" }
                        onClicked: {
                            eventDialog.isEditMode = true
                            eventDialog.editIndex = index
                            nameInput.text = name
                            dateInput.text = date
                            notesInput.text = notes !== undefined ? notes : ""
                            eventDialog.open()
                        }
                    }
                    Button {
                        text: "🗑️"
                        width: 40; height: 40
                        background: Rectangle { color: "transparent" }
                        onClicked: myEventModel.deleteEvent(index)
                    }
                }
            }
        }

        Button {
            text: "Додати нове свято"
            width: parent.width
            height: 50
            background: Rectangle { color: "#2563eb"; radius: 8 }
            contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 16; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
            onClicked: {
                eventDialog.isEditMode = false
                nameInput.text = ""
                dateInput.text = ""
                notesInput.text = ""
                eventDialog.open()
            }
        }
    }
}