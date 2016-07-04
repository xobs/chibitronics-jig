import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: mainWindow
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Chibitronics Test")

    MainForm {
        anchors.fill: parent
    }
}
