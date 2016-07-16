import QtQuick 2.0

Item {
    id: stickersTest
    objectName: "stickersTest"

    signal startTests
    property var errorCount;
    width: 1280
    height: 720

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
        ;
    }

    function onSetHeader(msg) {
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

    function testsStarted() {
        console.log("Tests starting");
        dotFill.testedPoints = [];
        dotFill.failurePoints = [];
        dotFill.currentPoint = null;
        dotFill.requestPaint()
    }

    function testFinished() {
        dotFill.currentPoint = null;
        dotFill.requestPaint()
    }

    function addPoint(pt, typ) {
        if (typ == 0) {
            dotFill.currentPoint = pt;
        }
        else if (typ == 1) {
            if (!dotFill.testedPoints)
                dotFill.testedPoints = [];
            dotFill.testedPoints.push(pt);
        }
        else if (typ == 2) {
            if (!dotFill.failurePoints)
                dotFill.failurePoints = [];
            dotFill.failurePoints.push(pt);
        }
        dotFill.requestPaint()
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


    Connections {
        target: activationArea
        onClicked: userInteraction()
    }

    Text {
        id: resultsText
        x: 68
        y: 369
        opacity: 0
        font.pixelSize: 48
    }




    states: [
        State {
            name: "testing"
        },
        State {
            name: "pass"

            PropertyChanges {
                target: resultsText
                text: qsTr("Pass")
                opacity: 1
            }
        },
        State {
            name: "fail"

            PropertyChanges {
                target: resultsText
                text: qsTr("Fail")
                opacity: 1
            }
        }
    ]

    MouseArea {
        id: activationArea
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
    }

    Text {
        id: stepList
        x: 889
        y: 115
        width: 354
        height: 559
        text: qsTr("Text")
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 12
    }

    Text {
        id: logOutput
        x: 0
        y: 80
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 12
    }

    Image {
        id: boardImage
        x: 74
        y: 115
        width: 750
        height: 385
        fillMode: Image.PreserveAspectFit
        source: "../images/ltcsticker.png"
    }







}
