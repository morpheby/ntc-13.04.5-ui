import QtQuick 2.4

Rectangle {
    id: rectangle1
    property alias mouseArea: mouseArea

    width: 360
    height: 360

    Text {
        id: text1
        y: 337
        height: 15
        text: qsTr("Text")
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        font.pixelSize: 12
    }
}
