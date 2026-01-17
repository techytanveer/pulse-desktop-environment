import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Window {
    width: 800
    height: 600
    visible: true
    title: "Pulse Shell Test"
    
    Rectangle {
        anchors.fill: parent
        color: "#1e293b"
        
        Text {
            anchors.centerIn: parent
            text: "Pulse Shell - Development Build"
            color: "white"
            font.pixelSize: 24
            font.bold: true
        }
        
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            text: "Exit"
            onClicked: Qt.quit()
        }
    }
}
