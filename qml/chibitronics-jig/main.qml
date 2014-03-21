import QtQuick 1.1

Rectangle {
    id: stickersTest
    width: 800
    height: 480
    color: "white"
    state: "startScreen"

    signal buttonClick
    signal startTest
    signal nextStep
    signal testsFinished
    signal setHeader(string header)
    signal setSticker(int stickerNum)

    onTestsFinished: {
        console.log("Tests finished");
        stickersTest.state = "finished"
    }

    onSetSticker: {
        console.log("Picking sticker " + stickerNum)
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
        console.log("State started at: " + stickersTest.state);
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

        console.log("State is now: " + stickersTest.state);
    }

    onSetHeader: {
        titleText.text = header;
    }

    onButtonClick: {
        console.log("User input.  State started at: " + stickersTest.state);
        if(stickersTest.state == "finished")
            stickersTest.state = "startScreen";
        else if (stickersTest.state == "startScreen") {
            stickersTest.state = "burning";
            startTest();
        }
        console.log("User input.  State ended at: " + stickersTest.state);
    }

    MouseArea {
        anchors.fill: parent
        onClicked: buttonClick()
    }

    focus: true
    Keys.onPressed: {
        buttonClick()
    }

    Item {
        width: 800
        height: 480
        anchors.centerIn: parent
        clip: true

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
            id: titleText
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 48
        }
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
                console.log(">> State is now: " + stickersTest.state);
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
