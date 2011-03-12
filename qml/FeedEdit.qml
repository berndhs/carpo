

import QtQuick 1.0

Rectangle {
  function startNew (url) {
    console.log ("Start NEw Feed " + url)
    displayEditFeed (url, "", "", "")
  }
  function displayEditFeed (theFeedUrl, theTitle, theSite, theNick) {
    feedNick.textValue = theNick
    feedUrl.textValue = theSite
    addrInput.urlString = theFeedUrl
    feedTitle.textValue = theTitle
    choiceButtons.visible = false
    feedDetails.visible = true
  }
  function clear () {
    choiceButtons.visible = true
    feedDetails.visible = false
    feedNick.textValue = ""
    feedUrl.textValue = ""
    addrInput.urlString = ""
    feedTitle.textValue = ""
  }
  id: feedEdit
  property real normalWidth: parent.width
  property real urlMargin: 6
  width: 200
  height: 200
  radius: 5
  color: "palegoldenrod"
  AddressInput { 
    id: addrInput
    labelText: "Addr:"
    width: parent.width - parent.urlMargin
  }
  Flow {
    id:choiceButtons
    visible:true
    anchors { 
      top: parent.top; topMargin: addrInput.height + 6 ; 
      horizontalCenter: parent.horizontalCenter
    }
    width: childrenRect.width
    spacing: 4
    ChoiceButton {
      id: choiceNew
      labelText: "New Feed"
      onClicked: {
        startNew (addrInput.urlString)
      }
    }
  }
  Rectangle {
    id: feedDetails
    color: "transparent"
    visible: false
    width: parent.width
    anchors {
      top: parent.top; topMargin: addrInput.height + 6 
    }
    LineInput {
      id: feedNick
      width:parent.width
      labelText: "Nick"
    }
    LineInput {
      id: feedTitle
      width:parent.width
      anchors.top: feedNick.bottom
      labelText: "Title"
    }
    LineInput {
      id: feedUrl
      width:parent.width
      anchors.top: feedTitle.bottom
      labelText: "Site Url"
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
