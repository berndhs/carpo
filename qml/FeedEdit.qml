

import QtQuick 1.0

Rectangle {
  function startNew (url) {
    console.log ("Start NEw Feed " + url)
  }
  function editOld () {
    console.log ("Edit Old Feed")
  }
  id: feedEdit
  property real normalWidth: parent.width
  property real urlMargin: 6
  width: 200
  height: 200
  radius: 5
  color: "lightcoral"
  AddressInput { 
    id: addrInput
    labelText: "Addr:"
    width: parent.width - parent.urlMargin
  }
  Flow {
    anchors { 
      top: parent.top; topMargin: addrInput.height + 6 ; 
      horizontalCenter: parent.horizontalCenter
    }
    width: childrenRect.width
    spacing: 4
    ChoiceButton {
      id: choiceOld
      labelText: "Choose From List"
      onClicked: {
        editOld ()
      }
    }
    ChoiceButton {
      id: choiceNew
      labelText: "New Feed"
      onClicked: {
        startNew (addrInput.urlString)
      }
    }
  }
  Text { anchors.centerIn: parent; text: "Edit Feed Element" }
  Connections {
    target: addrInput
    onUrlEntered: {
      console.log ("entered URL " + url) 
    }
  } 
}
