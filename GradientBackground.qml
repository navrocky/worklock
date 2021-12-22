import QtQuick 2.0
import QtQuick.Shapes 1.12

Shape {
    property color color1: '#7a92e9'
    property color color2: '#cebff9'

    id: shape
    ShapePath {
        strokeWidth: 0
        strokeColor: 'transparent'


        fillGradient: LinearGradient {
            x1: 0
            y1: shape.height
            x2: shape.width
            y2: 0
            GradientStop { position: 0; color: shape.color1 }
            GradientStop { position: 1; color: shape.color2 }
        }
        startX: 0; startY: 0
        PathLine {
            x: shape.width
            y: 0
        }
        PathLine {
            x: shape.width
            y: shape.height
        }
        PathLine {
            x: 0
            y: shape.height
        }
    }
}
