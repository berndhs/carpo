import QtQuick 1.0

Rectangle {
  id: button
  signal clicked ()
  property alias labelText : label.text
  width: 100
  height: 100
  radius: 5
  color: "red"
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