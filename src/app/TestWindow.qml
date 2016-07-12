import QtQuick 2.0

Item {
    id: stickersTest
    objectName: "stickersTest"

    signal startTests
    property var errorCount;

    function userInteraction() {
        if (stickersTest.state == "testing") {
            console.log("Test already running");
        }
        else {
            console.log("No test running, so starting one now.");
            stickersTest.state = "testing";
            logOutput.text = "";
            errorCount = 0;
            startTests();
        }
    }

    function onNextStep() {
        console.log("Advancing to next step");
    }

    function onSetHeader(msg) {
        console.log("Setting header to: " + msg);
        statusText.text = msg;
    }

    function onSetSticker(sticker) {
        console.log("Setting sticker: " + sticker);
    }

    function onTestsFinished() {
        if (errorCount)
            stickersTest.state = "fail";
        else
            stickersTest.state = "pass";
    }

    function onAppendLog(msg) {
        logOutput.text = logOutput.text + "\n" + msg;
    }

    function onAppendPass() {
        console.log("Appending pass");
    }

    function onAppendError(msg) {
        console.log("Appending error: " + msg);
        errorCount++;
    }

    Text {
        id: statusText
        height: 44
        text: qsTr("Click to begin")
        anchors.right: parent.right
        anchors.rightMargin: 37
        anchors.left: parent.left
        anchors.leftMargin: 37
        anchors.top: parent.top
        anchors.topMargin: 32
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 32
    }

    Image {
        id: boardImage
        anchors.right: parent.right
        anchors.rightMargin: 216
        anchors.left: parent.left
        anchors.leftMargin: 31
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 177
        anchors.top: parent.top
        anchors.topMargin: 100
        fillMode: Image.PreserveAspectFit
        source: "../images/ltcsticker.png"
    }

    MouseArea {
        id: activationArea
        anchors.fill: parent
    }

    Text {
        id: logOutput
        x: 440
        y: 106
        width: 190
        height: 357
        font.pixelSize: 12
    }

    Connections {
        target: activationArea
        onClicked: userInteraction()
    }

    Text {
        id: passText
        x: 68
        y: 369
        text: qsTr("Text")
        opacity: 0
        font.pixelSize: 12
    }

    Text {
        id: failText
        x: 25
        y: 336
        text: qsTr("Text")
        opacity: 0
        font.pixelSize: 12
    }


    states: [
        State {
            name: "testing"
        },
        State {
            name: "pass"

            PropertyChanges {
                target: passText
                x: 54
                y: 344
                text: qsTr("Pass")
                font.pixelSize: 48
                opacity: 1
            }
        },
        State {
            name: "fail"

            PropertyChanges {
                target: failText
                x: 58
                y: 341
                text: qsTr("Fail")
                font.pixelSize: 48
                opacity: 1
            }
        }
    ]

}
