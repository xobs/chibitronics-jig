import QtQuick 1.1

Rectangle {
    id: stickersTest
    width: 1920
    height: 1080
    color: "white"
    state: "startScreen"
    property int errorCount
    property int currentSticker
    property string mode: "sensor"

    signal buttonClick
    signal startEffectsTests
    signal startSensorTests
    signal nextStep
    signal testsFinished
    signal setHeader(string header)
    signal setSticker(int stickerNum)
    signal appendLog(string txt)
    signal appendError(string txt)
    signal appendPass

    onSetHeader: {
        titleText.text = header;
    }

    onAppendLog: {
        debugLog.text = txt + "\n" + debugLog.text;
    }

    onAppendError: {
        errorText.opacity = 1;
        errorText.text = txt;
        errorCount++;

        if (currentSticker == 1)
            statusSticker1.state = "error";
        else if (currentSticker == 2)
            statusSticker2.state = "error";
        else if (currentSticker == 3)
            statusSticker3.state = "error";
        else if (currentSticker == 4)
            statusSticker4.state = "error";
        else if (currentSticker == 5)
            statusSticker5.state = "error";
        else if (currentSticker == 6)
            statusSticker6.state = "error";
        else if (currentSticker == 7)
            statusSticker7.state = "error";
        else if (currentSticker == 8)
            statusSticker8.state = "error";
        else
            console.log("Unable to find a sticker to error");
    }

    onAppendPass: {
        if (currentSticker == 1)
            statusSticker1.state = "pass";
        else if (currentSticker == 2)
            statusSticker2.state = "pass";
        else if (currentSticker == 3)
            statusSticker3.state = "pass";
        else if (currentSticker == 4)
            statusSticker4.state = "pass";
        else if (currentSticker == 5)
            statusSticker5.state = "pass";
        else if (currentSticker == 6)
            statusSticker6.state = "pass";
        else if (currentSticker == 7)
            statusSticker7.state = "pass";
        else if (currentSticker == 8)
            statusSticker8.state = "pass";
        else
            console.log("Unable to find a sticker to pass");
    }

    onTestsFinished: {
        if (errorCount == 0)
            stickersTest.state = "finished";
        else
            stickersTest.state = "error";
        errorCount = 0;
        debugLog.text = "---\n" + debugLog.text;
    }

    onSetSticker: {
        if (stickerNum == 1)
            downArrow.state = "sticker1";
        else if (stickerNum == 2)
            downArrow.state = "sticker2";
        else if (stickerNum == 3)
            downArrow.state = "sticker3";
        else if (stickerNum == 4)
            downArrow.state = "sticker4";
        else if (stickerNum == 5)
            downArrow.state = "sticker5";
        else if (stickerNum == 6)
            downArrow.state = "sticker6";
        else if (stickerNum == 7)
            downArrow.state = "sticker7";
        else if (stickerNum == 8)
            downArrow.state = "sticker8";
        else
            downArrow.state = "parked";
        currentSticker = stickerNum;
    }

    onNextStep: {
        if (stickersTest.state == "finished")
            ;

        else if (stickersTest.state == "startScreen")
            stickersTest.state = "burning";
    }

    onButtonClick: {
        if(stickersTest.state == "finished" || stickersTest.state == "error") {
            errorText.text = "";
            stickersTest.state = "startScreen";
            statusSticker1.state = "parked";
            statusSticker2.state = "parked";
            statusSticker3.state = "parked";
            statusSticker4.state = "parked";
            statusSticker5.state = "parked";
            statusSticker6.state = "parked";
            statusSticker7.state = "parked";
            statusSticker8.state = "parked";
        }
        else if (stickersTest.state == "startScreen") {
            stickersTest.state = "burning";
            if (mode == "sensor") {
                testEffectsImage.source = "test-sensors.png";
                startSensorTests();
            }
            else {
                testEffectsImage.source = "test-effects.jpg";
                startEffectsTests();
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: buttonClick()
    }

    focus: true
    Keys.onPressed: {
        buttonClick()
    }

    Image {
        id: testEffectsImage
        x: -1
        y: -1
        anchors.centerIn: parent
        opacity: 1
    }

    Image {
        id: statusSticker1
        anchors.top: testEffectsImage.bottom
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 43
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker1
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker1
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker1
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker1; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker2
        anchors.top: testEffectsImage.bottom
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 140
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker2
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker2
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker2
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker2; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker3
        anchors.top: testEffectsImage.bottom
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 238
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker3
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker3
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker3
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker3; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker4
        anchors.top: testEffectsImage.bottom
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 335
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker4
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker4
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker4
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker4; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker5
        anchors.bottom: testEffectsImage.top
        anchors.left: testEffectsImage.left
        rotation: 90
        anchors.leftMargin: -120
        anchors.bottomMargin: -200
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker5
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker5
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker5
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker5; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker6
        anchors.bottom: testEffectsImage.top
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 180
        anchors.bottomMargin: 60
        rotation: 180
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker6
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker6
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker6
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker6; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker7
        anchors.bottom: testEffectsImage.top
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 32
        anchors.bottomMargin: 60
        rotation: 180
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker7
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker7
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker7
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker7; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: statusSticker8
        anchors.bottom: testEffectsImage.top
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 340
        anchors.bottomMargin: -200
        rotation: 270
        opacity: 0
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: statusSticker8
                    opacity: 0
                    source: ""
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: statusSticker8
                    opacity: 1
                    source: "arrow-error.png"
                }
            },
            State {
                name: "pass"
                PropertyChanges {
                    target: statusSticker8
                    opacity: 1
                    source: "arrow-pass.png"
                }
            }
        ]
        transitions: [
            Transition {
                SequentialAnimation {
                    NumberAnimation { target: statusSticker8; property: "opacity"; duration: 250; }
                }
            }
        ]
    }

    Image {
        id: downArrow
        y: -1
        opacity: 0
        anchors.bottom: testEffectsImage.top
        anchors.left: testEffectsImage.left
        anchors.leftMargin: 43
        source: "downarrow.png"
        states: [
            State {
                name: "parked"
                PropertyChanges {
                    target: downArrow
                    opacity: 0
                    anchors.leftMargin: 43
                }
            },
            State {
                name: "sticker1"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    anchors.leftMargin: 43
                }
            },
            State {
                name: "sticker2"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    anchors.leftMargin: 140
                }
            },
            State {
                name: "sticker3"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    anchors.leftMargin: 238
                }
            },
            State {
                name: "sticker4"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    anchors.leftMargin: 335
                }
            },
            State {
                name: "sticker5"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    rotation: 270
                    anchors.leftMargin: -60
                    anchors.bottomMargin: -200
                }
            },
            State {
                name: "sticker6"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    anchors.leftMargin: 180
                }
            },
            State {
                name: "sticker7"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    anchors.leftMargin: 32
                }
            },
            State {
                name: "sticker8"
                PropertyChanges {
                    target: downArrow
                    opacity: 1
                    rotation: 90
                    anchors.leftMargin: 280
                    anchors.bottomMargin: -200
                }
            }
        ]
        transitions: [
            Transition {
                NumberAnimation {
                    target: downArrow;
                    property: "anchors.leftMargin";
                    duration: 250
                }
                NumberAnimation {
                    target: downArrow;
                    property: "anchors.bottomMargin";
                    duration: 250
                }
                NumberAnimation {
                    target: downArrow;
                    property: "opacity";
                    duration: 250
                }
                NumberAnimation {
                    target: downArrow;
                    property: "rotation";
                    duration: 250
                }
            }
        ]
    }

    Text {
        id: statusText
        text: qsTr("Click to begin test")
        anchors.centerIn: parent
        opacity: 0
        y: 200
    }

    Text {
        id: debugLog
        text: "Log initialized"
        opacity: 1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        font.pointSize: 12
        width: parent.width * 0.80 // 80%
        height: parent.height * 0.90 // 100%
    }

    Text {
        id: titleText
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 43
        font.pointSize: 48
    }

    Text {
        id: errorText
        anchors.top: titleText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "red"
        text: "Error Here"
        opacity: 0
        anchors.topMargin: 43
        font.pointSize: 48
    }

    states: [
        State {
            name: "startScreen"
            PropertyChanges {
                target: statusText
                opacity: 1
            }
            PropertyChanges {
                target: titleText
                opacity: 0
            }
            PropertyChanges {
                target: testEffectsImage
                opacity: 0
            }
            PropertyChanges {
                target: errorText
                opacity: 0
            }
            PropertyChanges {
                target: downArrow
                opacity: 0
                state: "parked"
            }
        },

        State {
            name: "burning"
            id: burning
        },

        State {
            name: "finished"
            PropertyChanges {
                target: downArrow
                state: "parked"
            }
        },

        State {
            name: "error"
            PropertyChanges {
                target: errorText
                opacity: 1
            }
            PropertyChanges {
                target: downArrow
                state: "parked"
            }
        }
    ]

    transitions: [
        Transition {
            from: "startScreen"
            to: "burning"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: testEffectsImage; property: "opacity"; duration: 500 }
                    NumberAnimation { target: statusText; property: "opacity"; duration: 250 }
                }
            }
        },
        Transition {
            to: "finished"
            ParallelAnimation {
                NumberAnimation { target: downArrow; property: "opacity"; duration: 500 }
                NumberAnimation { target: testEffectsImage; property: "opacity"; duration: 500 }
            }
        },
        Transition {
            to: "startScreen"
            ParallelAnimation {
                NumberAnimation { target: downArrow; property: "opacity"; duration: 500 }
                NumberAnimation { target: testEffectsImage; property: "opacity"; duration: 500 }
            }
        }
    ]

} // stickersTest
