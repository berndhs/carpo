import QtQuick 1.0

Rectangle {
  id: button
  signal clicked ()
  property alias labelText : label.text
  property real commonMargin: 4
  width: 100
  height: 100
  radius: 5
  color: "grey"
  anchors { 
    topMargin: commonMargin; bottomMargin: commonMargin; 
    leftMargin: commonMargin; rightMargin: commonMargin
  }
  MouseArea {
    anchors.fill: parent
    onClicked: { 
      console.log("Click Button " + mouseX + " " + mouseY)
      parent.clicked ()
    }
  }
  Text { 
    id: label
    text: "Button"
    wrapMode:Text.Wrap
    width: parent.width
    anchors.centerIn: parent 
    horizontalAlignment: Text.AlignHCenter
 }
}