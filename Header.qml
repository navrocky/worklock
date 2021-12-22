import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    height: 100

    property bool paused: true

    id: root

    property color color1: '#7a92e9'
    property color color2: '#cebff9'

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: background
        replaceEnter:  Transition {
            PropertyAnimation {
                property: "showFactor"
                from: 0
                to: 1
                duration: 200
            }
        }
        replaceExit: Transition {
            PropertyAnimation {
                from: 1
                to: 1
                duration: 200
            }
        }
    }

    Component {
        id: background
        RippleMask {
            id: mask
            property color color1: root.color1
            property color color2: root.color2
            rippleX: root.width - 30 - 150 / 2
            rippleY: root.height
            GradientBackground {
                anchors.fill: parent
                color1: mask.color1
                color2: mask.color2
            }
        }
    }

    property real openFactor: (height - 100) / 100

    Text {
        x: 20
        color: 'white'
        text: "Title"
        font.pixelSize: Math.min(60, 40 + 20 * openFactor)
        font.bold: true
        anchors.verticalCenter: parent.verticalCenter
    }

    TimerButton {
        id: button
        color1: root.color1
        color2: root.color2
        anchors.bottom: header.bottom
        anchors.right: header.right
        anchors.bottomMargin: -size / 2
        anchors.rightMargin: 30
        shadeSize: 10
        borderSize: 10
        size: 150
        onClicked: {
            root.paused = !root.paused

            if (root.paused) {
                stackView.replace(background, {
                                      color1: '#7a92e9',
                                      color2: '#cebff9'
                                  })
            } else {
                stackView.replace(background, {
                                      color1: '#d47182',
                                      color2: '#fbb2a6'
                                  })
            }
        }
    }

    states: [
        State {
            when: root.paused
            PropertyChanges {
                target: root
                color1: '#7a92e9'
                color2: '#cebff9'
            }
        },
        State {
            when: !root.paused
            PropertyChanges {
                target: root
                color1: '#d47182'
                color2: '#fbb2a6'
            }
        }
    ]
    transitions: Transition {
        ColorAnimation { properties: "color1,color2"; duration: 200; easing.type: Easing.InOutQuad }
    }

}
