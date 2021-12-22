import QtQuick 2.15
import QtGraphicalEffects 1.15
import QtQuick.Shapes 1.12

Item {
    property var size: 200
    property int shadeSize: 10
    property int borderSize: 20
    property color color1: '#7a92e9'
    property color color2: '#7a92e9'
    signal clicked

    id: root
    width: size
    height: size

    Component {
        id: shadeCircle
        Item {
            property var size: Math.min(width, height)
            Rectangle {
                color: Qt.rgba(1, 1, 1, 0.15)
                radius: width / 2
                width: size
                height: size
                anchors.centerIn: parent
            }
        }
    }

    Loader {
        sourceComponent: shadeCircle
        anchors.fill: parent
        Loader {
            sourceComponent: shadeCircle
            anchors.margins: shadeSize
            anchors.fill: parent
        }
    }

    Item {
        id: button
        anchors.fill: parent
        visible: false

        property var size: Math.min(width, height) - shadeSize * 4

        onSizeChanged: console.log(size)
        layer.enabled: true
        layer.samples: 4

        Rectangle {
            width: parent.size
            height: parent.size
            anchors.centerIn: parent
            radius: parent.size / 2
            color: "white"

            Shape {
                property var size: button.size - root.borderSize * 2
                property var halfSize: size / 2

                id: shape

                anchors.centerIn: parent
                width: size
                height: size

                ShapePath {
                    strokeWidth: 0
                    strokeColor: 'transparent'

                    fillGradient: LinearGradient {
                        x1: 0
                        y1: shape.size
                        x2: shape.size
                        y2: 0
                        GradientStop { position: 0; color: root.color1 }
                        GradientStop { position: 1; color: root.color2 }
                    }

                    startX: shape.halfSize; startY: 0
                    PathArc {
                        x: shape.halfSize
                        y: shape.size
                        radiusX: shape.halfSize
                        radiusY: shape.halfSize
                    }
                    PathArc {
                        x: shape.halfSize
                        y: 0
                        radiusX: shape.halfSize
                        radiusY: shape.halfSize
                    }
                }
            }
        }
    }

    DropShadow {
        source: button
        anchors.fill: button
        radius: 10
        verticalOffset: 10
        color: Qt.rgba(0, 0, 0, 0.06)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
