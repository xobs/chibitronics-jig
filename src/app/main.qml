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

    property var tests: [
        {
            testName: "Header",
            params: {
                message: "Hello there",
                other: "wut",
                val: false
            }
        },
        {
            testName: "Delay",
            params: {
                msecs: 1000
            }
        },
        {
            testName: "Header",
            params: {
                message: "Step 1"
            }
        },
        {
            testName: "Delay",
            params: {
                msecs: 2000
            }
        },
        {
            testName: "Header",
            params: {
                message: "Second step"
            }
        },
        {
            testName: "Delay",
            params: {
                msecs: 4000
            }
        },
        {
            testName: "Header",
            params: {
                message: "Third step"
            }
        },
        {
            testName: "Delay",
            params: {
                msecs: 400
            }
        },
        {
            testName: "Header",
            params: {
                message: "Finnish"
            }
        }
    ]
}
