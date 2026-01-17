import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: windowItem
    property var window: null
    property var compositor: null
    
    x: window ? window.geometry.x : 0
    y: window ? window.geometry.y : 0
    width: window ? window.geometry.width : 100
    height: window ? window.geometry.height : 100
    
    color: "transparent"
    border.width: 2
    border.color: window && window.focused ? "#4a90e2" : "#666666"
    radius: 4
    
    // Title bar
    Rectangle {
        id: titleBar
        width: parent.width
        height: 30
        color: window && window.focused ? "#357ae8" : "#444444"
        radius: 4
        
        // Title text
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: window ? window.title : "Window"
            color: "white"
            elide: Text.ElideRight
            width: parent.width - 120
        }
        
        // Window controls
        Row {
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5
            
            Button {
                width: 20
                height: 20
                text: "−"
                onClicked: if (compositor && window) compositor.windowManager.minimizeWindow(window)
            }
            
            Button {
                width: 20
                height: 20
                text: window && window.state === 1 ? "🗗" : "🗖"
                onClicked: if (compositor && window) compositor.toggleMaximizeActiveWindow()
            }
            
            Button {
                width: 20
                height: 20
                text: "×"
                onClicked: if (compositor && window) compositor.closeActiveWindow()
            }
        }
    }
    
    // Client area
    Rectangle {
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#f0f0f0"
        
        Text {
            anchors.centerIn: parent
            text: "PID: " + (window && window.surface ? window.surface.client.processId : "N/A")
            color: "#333333"
        }
    }
    
    // Resize handles
    Rectangle {
        width: 10
        height: 10
        color: "#4a90e2"
        visible: window && window.focused
        
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeFDiagCursor
            drag.target: parent
            onPositionChanged: if (window) {
                var newWidth = Math.max(100, window.geometry.width + drag.x)
                var newHeight = Math.max(100, window.geometry.height + drag.y)
                window.geometry = Qt.rect(window.geometry.x, window.geometry.y, newWidth, newHeight)
            }
        }
    }
    
    // Make window draggable via title bar
    MouseArea {
        anchors.fill: titleBar
        drag.target: windowItem
        onPositionChanged: if (window) {
            window.geometry = Qt.rect(windowItem.x, windowItem.y, window.geometry.width, window.geometry.height)
        }
        onPressed: if (compositor && window) {
            compositor.windowManager.setActiveWindow(window)
        }
    }
    
    Behavior on x { NumberAnimation { duration: 100 } }
    Behavior on y { NumberAnimation { duration: 100 } }
    Behavior on width { NumberAnimation { duration: 100 } }
    Behavior on height { NumberAnimation { duration: 100 } }
}
