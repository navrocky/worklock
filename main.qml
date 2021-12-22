import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Worklock")
    color: '#f8f7fb'

    ListView {
        id: listView
        anchors.fill: parent
        model: 100
        delegate: Item {
            height: index == 0 ? 200 : 100
            width: listView.width

            Rectangle {
                anchors.fill: parent
                color: 'yellow'
                visible: index > 0
                Text {
                    text: `Item ${index - 1}`
                }
            }
        }
    }

    Header {
        id: header
        width: parent.width
        height: Math.max(200  - listView.contentY, 100)
    }
}
