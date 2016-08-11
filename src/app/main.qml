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
        property int errorCount: 0
        property var testNames
        property var testStates
        property int currentTestIndex: 0
        property int testErrorCount: 0

        signal startTests
        signal sendMessage(string str, int type, var message, var param);
        signal setTests(var tests);
        signal quit

        function getChildNamed(n) {
            for (var d in data)
                if (data[d].objectName == n)
                    return data[d];
            return null;
        }

        function userInteraction() {
            startTests();
        }

        function onNextStep() {
            ;
        }

        function updateTestList() {
            var stepList = getChildNamed("stepList");
            var newList = stepList.currentItem;
            var newModel = newList.model;

            newModel.clear();
            for (var testIndex in testNames) {
                newModel.append({
                    testTitle: testNames[testIndex],
                    testState: testStates[testIndex]
                });
            }
        }

        function onSetHeader(msg) {
            getChildNamed("statusText").text = msg;
        }

        function onTestsFinished() {
            if (errorCount)
                state = "fail";
            else
                state = "pass";

            // On a Fatal error, reset the "current test" to a black color,
            // rather than leaving it in the "in-progress" coloring.
            if (currentTestIndex < testNames.length) {
                testStates[currentTestIndex] = 0;
                updateTestList();
            }
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
            state = "testing";
            getChildNamed("logOutput").text = "";
            errorCount = 0;
            currentTestIndex = 0;
            testErrorCount = 0;

            console.log("Tests starting");
            dotFill.testedPoints = [];
            dotFill.failurePoints = [];
            dotFill.currentPoint = null;
            for (var i = 0; i < testNames.length; i++)
                testStates[i] = 0;
            testStates[0] = 3;
            dotFill.requestPaint()
            updateTestList();
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

            // If we have further tests, set their coloring to "test in progress"
            if (currentTestIndex <= testNames.length)
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
            if (typ === 0) {
                dotFill.currentPoint = pt;
            }
            else if (typ === 1) {
                if (!dotFill.testedPoints)
                    dotFill.testedPoints = [];
                dotFill.testedPoints.push(pt);
            }
            else if (typ === 2) {
                if (!dotFill.failurePoints)
                    dotFill.failurePoints = [];
                dotFill.failurePoints.push(pt);
            }
            dotFill.requestPaint()
        }

        function onSetVariable(key, val) {
        }

        function switchToDevice(val) {
            if (val === "ltc") {
                getChildNamed("boardImage").source = "../images/ltcsticker.png";
                mainWindow.tests = mainWindow.ltc_tests;
                setTests(mainWindow.tests);
            }
            else if (val === "dataviewer") {
                getChildNamed("boardImage").source = "../images/dataviewer.png";
                mainWindow.tests = mainWindow.dataviewer_tests;
                setTests(mainWindow.tests);
            }
        }

        function onSetGlobal(key, val) {
            if (key === "devicetype") {
                switchToDevice(val);
            }
        }

        focus: true
        Keys.onPressed: {

            // s
            if (event.key === 83) {
                sendMessage("", 6, "", "");
            }

            // d
            else if (event.key === 68) {
                sendMessage("", 14, "devicetype", "dataviewer");
            }

            // l
            else if (event.key === 76) {
                sendMessage("", 14, "devicetype", "ltc");
            }

            // c
            else if (event.key === 67) {
                sendMessage("", 15, "", "");
            }

            // ESC
            else if (event.key === 16777216) {
                quit();
            }

            else
                console.log("Unrecognized key event: " + event.key);
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
    property var logPath: "/logs/";
    property var tests;
    property var ltc_tests: [
        {
            testName: "SwdProgram",
            testTitle: qsTr("Program Firmware"),
            testDescription: qsTr("Running OpenOCD"),
            params: {
                elfname: "ltc.elf",
                extras: [
                    "flash erase_address pad 0x5800 0x1a00",
                    "flash write_image physical.bin 0x5900"
                ],
                idregisters: [
                    "0x40048058",
                    "0x4004805c",
                    "0x40048060"
                ],
                timeout: 2000
            }
        },
        {
            testName: "TakePicture",
            testTitle: qsTr("Take a picture"),
            params: {
                path: "/tester/images"
            }
        },
        {
            testName: "Header",
            testTitle: qsTr("Finished"),
            params: {
                message: qsTr("Done")
            }
        }
    ];
    property var dataviewer_tests: [
        {
            testName: "SwdProgram",
            testTitle: qsTr("Program Firmware"),
            params: {
                elfname: "dataviewer.elf",
                idregisters: [
                    "0x40048058",
                    "0x4004805c",
                    "0x40048060"
                ],
                timeout: 2000
            }
        },
        {
            testName: "TakePicture",
            testTitle: qsTr("Cheese a picture"),
            params: {
                path: "/tester/images"
            }
        },
        {
            testName: "Header",
            testTitle: qsTr("Finished"),
            params: {
                message: qsTr("Done")
            }
        }
    ]
}
