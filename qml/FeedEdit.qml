

import QtQuick 1.0

Rectangle {

  id: bigBox
  signal urlEntered(string url)
  signal urlChanged
  property real normalWidth: parent.width
  property real urlMargin: 6
  width: 200
  height: 200
  radius: 5
  color: "lightcoral"

  Rectangle {
    id: addressInputBox

    width: parent.width - parent.urlMargin
    anchors {
      horizontalCenter: parent.horizontalCenter
      top: parent.top
      topMargin: 2
    }
    color: "wheat"
    height: 48
    radius: parent.radius - 1
    Text { 
      height: 32
      id: labelText
      text: "Address:"
      anchors.left: parent.left 
      //anchors.verticalCenter: parent.verticalCenter
      anchors.top: parent.top
      anchors.topMargin: 2*bigBox.urlMargin + 2
    }
    Rectangle {
      height: labelText.height
      width: parent.width - labelText.width - anchors.leftMargin -2
      anchors.left: labelText.right
      anchors.leftMargin: 2
      anchors.verticalCenter: parent.verticalCenter
      color: "white"
      radius: parent.radius - 1

      TextInput {
        id: urlText
        horizontalAlignment: TextEdit.AlignLeft

        //onTextChanged: bigBox.urlChanged()
        Keys.onEnterPressed: {
          bigBox.urlEntered(urlText.text)
        }
        Keys.onReturnPressed: {
          bigBox.urlEntered(urlText.text)
        }
        anchors {
          left: parent.left; right: parent.right; leftMargin: 18; rightMargin: 18
          verticalCenter: parent.verticalCenter
        }
      }
    }
  }

  Text { anchors.centerIn: parent; text: "Feed Edit Element" }
}
