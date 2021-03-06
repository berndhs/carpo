
import QtQuick 1.1

Rectangle {
  id: bigBox
  signal textEntered(string theText)
  property alias textValue: lineText.text
  property alias labelText: lineLabelText.text
  property alias labelWidth: lineLabel.width
  property alias labelFloatRect: lineLabel.childrenRect

  width: parent.width - parent.urlMargin
  anchors {
    horizontalCenter: parent.horizontalCenter
    top: parent.top
    topMargin: 2
  }
  color: "tan"
  height: 48
  radius: parent.radius - 1
  Rectangle { 
    height: 32
    id: lineLabel  
    width: 200
    color:parent.color
    Text { id: lineLabelText; text: "Address:"; 
           anchors.verticalCenter: parent.verticalCenter }
    anchors.left: parent.left 
    anchors.verticalCenter: parent.verticalCenter
  }
  Rectangle {
    height: lineLabel.height
    width: parent.width - lineLabel.width - anchors.leftMargin -2
    anchors.left: lineLabel.right
    anchors.leftMargin: 2
    anchors.verticalCenter: parent.verticalCenter
    color: "white"
    radius: parent.radius - 1
    TextInput {
      id: lineText
      horizontalAlignment: TextEdit.AlignLeft
      autoScroll: true
      Keys.onEnterPressed: {
        bigBox.textEntered(lineText.text)
      }
      Keys.onReturnPressed: {
        bigBox.textEntered(lineText.text)
      }
      anchors {
        left: parent.left; right: parent.right; leftMargin: 2; rightMargin:2 
        verticalCenter: parent.verticalCenter
      }
    }
  }
}
