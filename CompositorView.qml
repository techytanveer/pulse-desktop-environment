import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import Pulse 1.0

Window {
    id: root
    width: 1920
    height: 1080
    visible: true
    title: "Pulse Compositor - Window Manager"
    color: "#1a1a1a"
    
    property var compositor: null
    
    // Background grid
    Canvas {
        id: gridCanvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.strokeStyle = "#333333";
            ctx.lineWidth = 1;
            
            // Draw grid lines
            for (var x = 0; x < width; x += 50) {
                ctx.beginPath();
                ctx.moveTo(x, 0);
                ctx.lineTo(x, height);
                ctx.stroke();
            }
            for (var y = 0; y < height; y += 50) {
                ctx.beginPath();
                ctx.moveTo(0, y);
                ctx.lineTo(width, y);
                ctx.stroke();
            }
        }
    }
    
    // Window renderers
    Repeater {
        model: compositor ? compositor.windowManager.windows : []
        delegate: WindowItem {
            window: modelData
            compositor: root.compositor
        }
    }
    
    // Control panel
    Rectangle {
        id: controlPanel
        width: 300
        height: 200
        color: "#2a2a2a"
        radius: 8
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 20
        
        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8
            
            Text {
                text: "Window Manager"
                color: "white"
                font.bold: true
                font.pixelSize: 16
            }
            
            Text {
                text: "Windows: " + (compositor ? compositor.windowManager.windowCount : 0)
                color: "#aaaaaa"
            }
            
            Text {
                text: "Active: " + (compositor && compositor.windowManager.activeWindow ? 
                                   compositor.windowManager.activeWindow.title : "None")
                color: "#aaaaaa"
            }
            
            Button {
                text: "Tile Windows"
                width: parent.width
                onClicked: if (compositor) compositor.tileWindows()
            }
            
            Button {
                text: "Cascade Windows"
                width: parent.width
                onClicked: if (compositor) compositor.cascadeWindows()
            }
            
            Button {
                text: "Close Active"
                width: parent.width
                onClicked: if (compositor) compositor.closeActiveWindow()
            }
            
            Button {
                text: "Toggle Maximize"
                width: parent.width
                onClicked: if (compositor) compositor.toggleMaximizeActiveWindow()
            }
        }
    }
    
    // Info text
    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        text: "Pulse Compositor - Drag windows to move, resize with edges"
        color: "#666666"
    }
}
