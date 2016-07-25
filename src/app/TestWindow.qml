import QtQuick 2.2
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1

Item {
    id: testDisplay
    objectName: "testOutput"
    width: 1280
    height: 720

    Text {
        id: statusText
        objectName: "statusText"
        height: 44
        text: qsTr("Click to begin")
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 32
    }


    Connections {
        target: activationArea
        onClicked: userInteraction()
    }

    states: [
        State {
            name: "testing"
        },
        State {
            name: "pass"

            PropertyChanges {
                target: resultsText
                color: "#0a7e11"
                text: qsTr("Pass")
            }
        },
        State {
            name: "fail"

            PropertyChanges {
                target: resultsText
                color: "#a90000"
                text: qsTr("Fail")
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

    Image {
        id: boardImage
        objectName: "boardImage"
        x: 0
        y: 58
        width: 875
        height: 432
        fillMode: Image.PreserveAspectFit
        source: "../images/ltcsticker.png"
    }

    property color lightFontColor: "#222"

    FontLoader {
        id: openSans
        source: "qrc:/fonts/OpenSans-Regular.ttf"
     }

    Text {
        id: textSingleton
    }

    StackView {
        id: stepList
        objectName: "stepList"
        x: 863
        y: 58
        width: 401
        height: 668

        initialItem: ListView {
            model: ListModel {
            }

            delegate: Button {
                width: stepList.width
                height: stepList.height * 0.125
                text: testTitle
                state: testState

                style: TestItemStyle {
                    fontColor: testDisplay.lightFontColor
                    leftAlignedIconSource: "qrc:/../images/icon-go.png"
                }
            }
        }

        /*
        text: qsTr("[Test List]")
        font.family: "Times New Roman"
        textFormat: Text.RichText
        style: Text.Normal
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 32
        */
    }

    Text {
        id: logOutput
        objectName: "logOutput"
        x: 8
        y: 496
        width: 855
        height: 216
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 12
    }

    Text {
        id: resultsText
        objectName: "resultsText"
        x: 91
        y: 99
        text: ""
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        style: Text.Normal
        opacity: 1
        font.pixelSize: 300
    }
}
