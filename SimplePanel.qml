import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 800
    height: 40
    color: "#0f172a"
    radius: 8
    
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        text: "Pulse Desktop"
        color: "white"
        font.pixelSize: 16
    }
    
    Text {
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        text: new Date().toLocaleTimeString(Qt.locale(), "hh:mm")
        color: "#94a3b8"
    }
}
