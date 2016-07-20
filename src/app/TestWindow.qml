import QtQuick 2.0
import QtQuick.Particles 2.0

Item {
    id: testDisplay
    objectName: "testOutput"
    signal startTests
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
        x: 0
        y: 58
        width: 875
        height: 432
        fillMode: Image.PreserveAspectFit
        source: "../images/ltcsticker.png"
    }

    Text {
        id: stepList
        objectName: "stepList"
        x: 863
        y: 58
        width: 401
        height: 668
        text: qsTr("[Test List]")
        font.family: "Times New Roman"
        textFormat: Text.RichText
        style: Text.Normal
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        font.pixelSize: 32
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


    ParticleSystem {
        id: root
        width: 320
        height: 480

        Emitter {
            emitRate: 400
            lifeSpan: 8000
            size: 24
            sizeVariation: 16
            velocity: PointDirection {x: root.width/10; y: root.height/10;}
            acceleration: PointDirection {x: -root.width/40; y: -root.height/40; xVariation: -root.width/20; yVariation: -root.width/20}
        }

        CustomParticle {
            vertexShader:"
                uniform lowp float qt_Opacity;
                varying lowp float fFade;
                varying highp vec2 fPos;

                void main() {
                    qt_TexCoord0 = qt_ParticleTex;
                    highp float size = qt_ParticleData.z;
                    highp float endSize = qt_ParticleData.w;

                    highp float t = (qt_Timestamp - qt_ParticleData.x) / qt_ParticleData.y;

                    highp float currentSize = mix(size, endSize, t * t);

                    if (t < 0. || t > 1.)
                    currentSize = 0.;

                    highp vec2 pos = qt_ParticlePos
                    - currentSize / 2. + currentSize * qt_ParticleTex          // adjust size
                    + qt_ParticleVec.xy * t * qt_ParticleData.y         // apply velocity vector..
                    + 0.5 * qt_ParticleVec.zw * pow(t * qt_ParticleData.y, 2.);

                    gl_Position = qt_Matrix * vec4(pos.x, pos.y, 0, 1);

                    highp float fadeIn = min(t * 20., 1.);
                    highp float fadeOut = 1. - max(0., min((t - 0.75) * 4., 1.));

                    fFade = fadeIn * fadeOut * qt_Opacity;
                    fPos = vec2(pos.x/320., pos.y/480.);
                }
            "
            fragmentShader: "
                varying highp vec2 fPos;
                varying lowp float fFade;
                varying highp vec2 qt_TexCoord0;
                void main() {//*2 because this generates dark colors mostly
                    highp vec2 circlePos = qt_TexCoord0*2.0 - vec2(1.0,1.0);
                    highp float dist = length(circlePos);
                    highp float circleFactor = max(min(1.0 - dist, 1.0), 0.0);
                    gl_FragColor = vec4(fPos.x*2.0 - fPos.y, fPos.y*2.0 - fPos.x, fPos.x*fPos.y*2.0, 0.0) * circleFactor * fFade;
                }"

        }
    }
}
