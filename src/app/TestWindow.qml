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

    function testsStarted() {
        console.log("Tests starting");
        dotFill.testedPoints = [];
        dotFill.failurePoints = [];
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
        id: resultsText
        x: 68
        y: 369
        opacity: 0
        font.pixelSize: 48
    }

    Canvas {
        id: dotFill
        objectName: "dotFill"
        anchors.fill: parent;
        property var testedPoints
        property var failurePoints
        property var currentPoint;
        onPaint: {
            var ctx = getContext("2d");
            var ptIdx;
            var x;
            var y;

            ctx.reset();

            // Draw "success" points
            ctx.fillStyle = Qt.rgba(0, 1, 0, 1);
            if (testedPoints) {
                for (ptIdx = 0; ptIdx < testedPoints.length; ptIdx++) {
                    x = testedPoints[ptIdx].x;
                    y = testedPoints[ptIdx].y;
                    ctx.fillRect(x - 3, y - 3, x + 3, y + 3);
                }
            }

            // Draw "failure" points
            ctx.fillStyle = Qt.rgba(1, 0, 0, 1);
            if (failurePoints) {
                for (ptIdx = 0; ptIdx < failurePoints.length; ptIdx++) {
                    x = failurePoints[ptIdx].x;
                    y = failurePoints[ptIdx].y;
                    ctx.fillRect(x - 3, y - 3, x + 3, y + 3);
                }
            }

            // Draw current point
            if (currentPoint) {
                ctx.fillStyle = Qt.rgba(0, 1, 1, 1);
                x = currentPoint.x;
                y = currentPoint.y;
                ctx.fillRect(0, 0, currentPoint.x, currentPoint.y);
            }
        }
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
        anchors.fill: parent
    }

}
