

import QtQuick 1.0

Rectangle {
  function displayNew (url) {
    console.log ("Start NEw Feed " + url)
    displayEditFeed ("", url, "", "", "", "")
    saveButton.show ()
    deleteButton.show ()
  }
  function displayEditFeed (theFeedId, theFeedUrl, theTitle, theSite, theNick, theDescr) {
    feedId = theFeedId
    feedNick.textValue = theNick
    siteUrl.textValue = theSite
    addrInput.urlString = theFeedUrl
    feedTitle.textValue = theTitle
    feedDescription.textValue = theDescr
    choiceButtons.visible = false
    feedDetails.visible = true
    saveButton.show ()
    deleteButton.show ()
  }
  function clear () {
    feedId = ""
    feedNick.textValue = ""
    siteUrl.textValue = ""
    addrInput.urlString = ""
    feedTitle.textValue = ""
    feedDescription.textValue = ""
    choiceButtons.visible = true
    feedDetails.visible = false
    saveButton.hide ()
    deleteButton.hide ()
  }
  signal startNewFeed (string url)
  signal deleteFeed (string ident)
  signal saveFeed (string ident, string feedUrl, string title, string siteUrl, string nick, string descr)
  id: feedEdit
  property real normalWidth: parent.width
  property real urlMargin: 6
  property string feedId: ""
  width: 200
  height: 200
  radius: 5
  color: "palegoldenrod"
  ChoiceButton {
    id:saveButton
    labelText: "Save Feed"
    height: 0
    visible: false
    color: "green"
    function show () { height = 32; visible = true }
    function hide () { height = 0; visible = false }
    onClicked: {
      console.log ("Save Feed")
      saveFeed (feedId, addrInput.urlString, feedTitle.textValue, siteUrl.textValue,
                feedNick.textValue, feedDescription.textValue)
    }
  }
  ChoiceButton {
    id:deleteButton
    labelText: "Delete Feed"
    height: 0
    visible: false
    color: "red"
    anchors { left: saveButton.right; verticalCenter: saveButton.verticalCenter }
    function show () { height = 32; visible = true }
    function hide () { height = 0; visible = false }
    onClicked: {
      console.log ("Delete Feed")
      deleteFeed (feedId)
    }
  }
  AddressInput { 
    id: addrInput
    labelText: "Addr:"
    width: parent.width - parent.urlMargin
    anchors.top: parent.top
    anchors.topMargin: saveButton.height
  }
  Flow {
    id:choiceButtons
    visible:true
    anchors { 
      top: addrInput.bottom; topMargin: 6 
      horizontalCenter: parent.horizontalCenter
    }
    width: childrenRect.width
    spacing: 4
    ChoiceButton {
      id: choiceNew
      labelText: "New Feed"
      onClicked: {
        startNewFeed (addrInput.urlString)
      }
    }
  }
  Rectangle {
    id: feedDetails
    color: "transparent"
    visible: false
    width: parent.width
    height: childrenRect.height
    anchors {
      top: addrInput.bottom; topMargin:  6 
    }
    LineInput {
      id: feedNick
      width:parent.width
      labelText: " Nick "
    }
    LineInput {
      id: feedTitle
      width:parent.width
      anchors.top: feedNick.bottom
      labelText: " Title "
    }
    LineInput {
      id: siteUrl
      width:parent.width
      anchors.top: feedTitle.bottom
      labelText: " Site Url "
    }
    BoxInput {
      id: feedDescription
      width:parent.width
      anchors.top: siteUrl.bottom
      labelText: " Description "
    }
  }
  Connections {
    target: addrInput
    onUrlEntered: {
      console.log ("entered URL " + url) 
    }
  } 
}
