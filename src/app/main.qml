import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: mainWindow
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Chibitronics Test")

    TestWindow {
        id: stickersTest
        objectName: "stickersTest"
        anchors.fill: parent
    }

    /* LtC sticker:
       1)  Toggle power off
       2)  Wait 100ms
       3)  Toggle power on
       4)  Program LtC sticker
       5)  Prompt operator to press/hold "reset button"
       6)  Program test .ino sketch via audio port
       7)  Toggle A0
       8)  Toggle A1
       9)  Toggle A3
       10) Toggle D0
       11) Toggle D1
       12) Check for A2 being "High"
       13) Prompt operator to press/hold Reset button
       14) Load physical programming .ino file
       15) Check that Program Fail LED is not lit
       16) Toggle power to sticker
       17) Verify red LED is not on
       18) Toggle power off
    */
    property var tests: [
        {
            testName: "ShellCmd",
            params: {
                command: "dir",

                timeout: 1000,
                success: "shellcmd"
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
