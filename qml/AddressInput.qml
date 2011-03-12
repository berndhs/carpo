
import QtQuick 1.0

Rectangle {
  id: bigBox
  signal urlEntered(string url)
  property alias urlString: urlText.text
  property alias labelText: urlLabelText.text

  width: parent.width - parent.urlMargin
  anchors {
    horizontalCenter: parent.horizontalCenter
    top: parent.top
    topMargin: 2
  }
  color: "wheat"
  height: 48
  radius: parent.radius - 1
  Rectangle { 
    height: 32
    id: urlLabel
    width: childrenRect.width
    color:parent.color
    Text { id: urlLabelText; text: "Address:"; anchors.verticalCenter: parent.verticalCenter }
    anchors.left: parent.left 
    anchors.verticalCenter: parent.verticalCenter
  }
  Rectangle {
    height: urlLabel.height
    width: parent.width - urlLabel.width - anchors.leftMargin -2
    anchors.left: urlLabel.right
    anchors.leftMargin: 2
    anchors.verticalCenter: parent.verticalCenter
    color: "white"
    radius: parent.radius - 1
    TextInput {
      id: urlText
      horizontalAlignment: TextEdit.AlignLeft
      Keys.onEnterPressed: {
        bigBox.urlEntered(urlText.text)
      }
      Keys.onReturnPressed: {
        bigBox.urlEntered(urlText.text)
      }
      anchors {
        left: parent.left; right: parent.right; leftMargin: 2; rightMargin:2 
        verticalCenter: parent.verticalCenter
      }
    }
  }
}
