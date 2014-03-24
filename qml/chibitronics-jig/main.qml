import QtQuick 1.1

Rectangle {
    id: stickersTest
    width: 1920
    height: 1080
    color: "white"
    state: "startScreen"

    signal buttonClick
    signal startTest
    signal nextStep
    signal testsFinished
    signal setHeader(string header)
    signal setSticker(int stickerNum)
    signal appendLog(string txt)

    onTestsFinished: {
        stickersTest.state = "finished"
    }

    onSetSticker: {
        if (stickerNum == 1)
            downArrow.state = "sticker1"
        else if (stickerNum == 2)
            downArrow.state = "sticker2"
        else if (stickerNum == 3)
            downArrow.state = "sticker3"
        else if (stickerNum == 4)
            downArrow.state = "sticker4"
        else
            downArrow.state = ""
    }

    onNextStep: {
        if (stickersTest.state == "finished")
            ;

        else if (stickersTest.state == "startScreen")
            stickersTest.state = "burning";

        else if(stickersTest.state == "burningSticker1")
            stickersTest.state = "burningSticker2";
        else if(stickersTest.state == "burningSticker2")
            stickersTest.state = "burningSticker3";
        else if(stickersTest.state == "burningSticker3")
            stickersTest.state = "burningSticker4";
        else if(stickersTest.state == "burningSticker4")
            stickersTest.state = "finished";
    }

    onSetHeader: {
        titleText.text = header;
    }

    onButtonClick: {
        if(stickersTest.state == "finished")
            stickersTest.state = "startScreen";
        else if (stickersTest.state == "startScreen") {
            stickersTest.state = "burning";
            startTest();
        }
    }

    onAppendLog: {
        debugLog.text = txt + "\n" + debugLog.text;
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
        source: "test-effects.jpg"
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
                    property: "opacity";
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
        },

        State {
            name: "burning"
            id: burning
            signal animationFinished
            onAnimationFinished: {
                stickersTest.state = "burningSticker1"
            }
        },

        State {
            name: "finished"
            PropertyChanges {
                target: testEffectsImage
                opacity: 0
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
                ScriptAction { script: burning.animationFinished() }
            }
        },
        Transition {
            to: "finished"
            ParallelAnimation {
                NumberAnimation { target: downArrow; property: "opacity"; duration: 500 }
                NumberAnimation { target: testEffectsImage; property: "opacity"; duration: 500 }
            }
        }
    ]

} // stickersTest
