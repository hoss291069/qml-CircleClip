import QtQuick 2.12
import QtQuick.Window 2.12
import Test 1.0
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    CircleClip{
        anchors.fill: parent;

        Rectangle
        {
            id:greenRect
            width:parent.width
            height: parent.height/2
            anchors.centerIn: parent
            color:"green"
        }
    }
}
