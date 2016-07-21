import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: mainWindow
    visible: true
    width: 1280
    height: 720
    title: qsTr("Chibitronics Test")

    TestWindow {
        id: stickersTest
        objectName: "stickersTest"
        anchors.fill: parent
        property var errorCount: 0
        property var testNames
        property var testStates
        property var currentTestIndex: 0
        property var testErrorCount: 0

        function getChildNamed(n) {
            for (var d in data)
                if (data[d].objectName == n)
                    return data[d];
            return null;
        }

        function userInteraction() {
            if (state === "testing") {
                console.log("Test already running");
            }
            else {
                console.log("No test running, so starting one now.");
                state = "testing";
                getChildNamed("logOutput").text = "";
                errorCount = 0;
                currentTestIndex = 0;
                testErrorCount = 0;
                startTests();
            }
        }

        function onNextStep() {
            ;
        }

        function updateTestList() {
            var stepList = getChildNamed("stepList");

            stepList.text = "<ul>";
            for (var testIndex in testNames) {
                if (testStates[testIndex] === 1)
                    stepList.text += "<li><font color=\"green\">" + testNames[testIndex] + "</font></li>";
                else if (testStates[testIndex] === 2)
                    stepList.text += "<li><font color=\"red\">" + testNames[testIndex] + "</font></li>";
                else if (testStates[testIndex] === 3)
                    stepList.text += "<li><font color=\"blue\">" + testNames[testIndex] + "</font></li>";
                else
                    stepList.text += "<li>" + testNames[testIndex] + "</li>";
            }
            stepList.text += "</ul>";
        }

        function onSetHeader(msg) {
            getChildNamed("statusText").text = msg;
        }

        function onTestsFinished() {
            if (errorCount)
                state = "fail";
            else
                state = "pass";
        }

        function onAppendLog(msg) {
            var logOutput = getChildNamed("logOutput");
            logOutput.text = logOutput.text + "\n" + msg;
        }

        function onAppendPass() {
            console.log("Appending pass");
        }

        function onAppendError(msg) {
            console.log("Appending error: " + msg);
            errorCount++;
            testErrorCount++;
            updateTestList();
        }

        function testsStarted() {
            console.log("Tests starting");
            dotFill.testedPoints = [];
            dotFill.failurePoints = [];
            dotFill.currentPoint = null;
            for (var i = 0; i < testNames.length; i++)
                testStates[i] = 0;
            testStates[0] = 3;
            dotFill.requestPaint()
        }

        function testFinished() {
            dotFill.currentPoint = null;
            dotFill.requestPaint()

            /* Update the test error counts */
            if (testErrorCount)
                testStates[currentTestIndex] = 2;
            else
                testStates[currentTestIndex] = 1;
            currentTestIndex++;
            testErrorCount = 0;
            testStates[currentTestIndex] = 3;
            updateTestList();
        }

        function onTestListUpdated(_testNames) {
            testNames = []
            testStates = []
            for (var testNameIndex in _testNames) {
                testNames.push(_testNames[testNameIndex]);
                testStates.push(0);
            }
            updateTestList();
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

    }

    Canvas {
        id: dotFill
        objectName: "dotFill"
        anchors.fill: parent;
        property var testedPoints
        property var failurePoints
        property var currentPoint;
        property var frame;
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
                    ctx.fillRect(x - 3, y - 3, 6, 6);
                }
            }

            // Draw "failure" points
            ctx.fillStyle = Qt.rgba(1, 0, 0, 1);
            if (failurePoints) {
                for (ptIdx = 0; ptIdx < failurePoints.length; ptIdx++) {
                    x = failurePoints[ptIdx].x;
                    y = failurePoints[ptIdx].y;
                    ctx.fillRect(x - 3, y - 3, 6, 6);
                }
            }

            // Draw current point
            if (currentPoint) {
                if (frame & 1)
                    ctx.fillStyle = Qt.rgba(0, 1, 1, 1);
                else
                    ctx.fillStyle = Qt.rgba(1, 0, 1, 1);
                x = currentPoint.x;
                y = currentPoint.y;
                ctx.fillRect(x - 3, y - 3, 6, 6);
            }
        }

    }

    Timer {
        running: true
        interval: 500
        repeat: true
        onTriggered: {
            if (dotFill.currentPoint) {
                if (!dotFill.frame)
                    dotFill.frame = 0;
                dotFill.frame++;
                dotFill.requestPaint()
            }
        }
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
            testName: "SwdProgram",
            testTitle: qsTr("Program Firmware"),
            testDescription: qsTr("Running OpenOCD"),
            params: {
                elfname: "ltc.elf",
                idregisters: [
                    "0x40048058",
                    "0x4004805c",
                    "0x40048060"
                ],
                timeout: 1000
            }
        },
        {
            testName: "TakePicture",
            path: "/tester/images"
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
                message: qsTr("Done")
            }
        }
    ];
    property var logPath: "/logs/";
}
