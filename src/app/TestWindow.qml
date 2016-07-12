import QtQuick 2.0

Item {
    id: stickersTest
    objectName: "stickersTest"

    signal startTests

    function userInteraction() {
        if (stickersTest.state == "testing") {
            console.log("Test already running");
        }
        else {
            console.log("No test running, so starting one now.");
            stickersTest.state = "testing";
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
        console.log("Tests finished");
        console.log("Possible states: " + stickersTest.states);
        stickersTest.state = "";
    }

    function onAppendLog(msg) {
        logOutput.text = logOutput.text + msg;
    }

    function onAppendPass() {
        console.log("Appending pass");
    }

    function onAppendError(msg) {
        console.log("Appending error: " + msg);
    }

    Text {
        id: statusText
        x: 38
        y: 40
        width: 566
        height: 68
        text: qsTr("Click to begin")
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 32
    }

    Image {
        id: boardImage
        x: 31
        y: 157
        width: 393
        height: 203
        fillMode: Image.PreserveAspectFit
        source: "../images/ltcsticker.png"
    }

    MouseArea {
        id: activationArea
        x: -1
        y: 8
        width: 639
        height: 463

        Text {
            id: logOutput
            x: 441
            y: 98
            width: 190
            height: 357
            font.pixelSize: 12
        }
    }

    Image {
        id: statusArrow
        x: 487
        y: 216
        width: 73
        height: 77
        fillMode: Image.PreserveAspectFit
        source: "../images/downarrow.png"
    }

    Connections {
        target: activationArea
        onClicked: userInteraction()
    }


    states: [
        State {
            name: "testing"
        }
    ]

}
