
import QtQuick 1.0

Rectangle {
  id: bigBox
  signal textEntered(string theText)
  property alias textValue: boxText.text
  property alias labelText: boxLabelText.text
  property alias labelWidth: boxLabel.width
  property alias boxHeight: editBox.height

  width: parent.width - parent.urlMargin
  anchors {
    horizontalCenter: parent.horizontalCenter
    top: parent.top
    topMargin: 2
  }
  color: "tan"
  height: editBox.height
  radius: parent.radius - 1
  Rectangle { 
    height: 32
    id: boxLabel
    width: childrenRect.width
    color:parent.color
    Text { id: boxLabelText; text: "Address:"; anchors.verticalCenter: parent.verticalCenter }
    anchors.left: parent.left 
    anchors.verticalCenter: parent.verticalCenter
  }
  Rectangle {
    id: editBox
    height: 5*boxLabel.height
    width: parent.width - boxLabel.width - anchors.leftMargin -2
    anchors.left: boxLabel.right
    anchors.leftMargin: 2
    anchors.verticalCenter: parent.verticalCenter
    color: "white"
    radius: parent.radius - 1
    TextEdit {
      id: boxText
      horizontalAlignment: TextEdit.AlignLeft
      wrapMode: TextEdit.Wrap
      Keys.onEnterPressed: {
        bigBox.urlEntered(lineText.text)
      }
      Keys.onReturnPressed: {
        bigBox.urlEntered(lineText.text)
      }
      anchors {
        left: parent.left; right: parent.right; leftMargin: 2; rightMargin:2 
        verticalCenter: parent.verticalCenter
      }
    }
  }
}
