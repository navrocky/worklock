import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property double showFactor: 1
    property double rippleX: 0
    property double rippleY: 0
    default property alias children: componentHolder.children

    property double rippleMaxRadius: Math.max(
                                         Qt.vector2d(rippleX, rippleY).length(),
                                         Qt.vector2d(rippleX, height - rippleY).length(),
                                         Qt.vector2d(width - rippleX, rippleY).length(),
                                         Qt.vector2d(width - rippleX, height - rippleY).length()
                                         )

    property double currentRippleRadius: rippleMaxRadius * showFactor
    id: root

    property var child: componentHolder.children[0]

    Item {
        anchors.fill: parent
        id: componentHolder
    }

    Item {
        id: mask
        anchors.fill: parent
        visible: false
        Rectangle {
            x: root.rippleX - root.currentRippleRadius
            y: root.rippleY - root.currentRippleRadius
            width: root.currentRippleRadius * 2
            height: root.currentRippleRadius * 2
            radius: root.currentRippleRadius
        }
    }

    OpacityMask {
        anchors.fill: parent
        source: child
        maskSource: mask
        visible: showFactor < 1.0
    }

    Component.onCompleted:  {
        updateShowFactor()
    }

    function updateShowFactor() {
        console.log(`onShowFactorChangen: ${showFactor}`)
        const child = componentHolder.children[0]
        child.opacity = showFactor < 1.0 ? 0 : 1
    }

    onShowFactorChanged: {
        updateShowFactor()
    }
}
