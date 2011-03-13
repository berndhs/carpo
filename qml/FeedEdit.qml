

import QtQuick 1.0

Rectangle {
  function displayNew (url) {
    console.log ("Start NEw Feed " + url)
    displayEditFeed ("", url, "", "", "", "")
    saveButtonRow.show ()
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
    saveButtonRow.show ()
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
    saveButtonRow.hide ()
  }
  signal startNewFeed (string url)
  signal deleteFeed (string ident)
  signal probeFeed (string url)
  signal saveFeed (string ident, string feedUrl, string title, string siteUrl, 
                   string nick, string descr)
  id: feedEdit
  property real normalWidth: parent.width
  property real urlMargin: 6
  property string feedId: ""
  width: 200
  height: 200
  radius: 5
  color: "palegoldenrod"
  Rectangle {
    function show () { height = 32; visible = true }
    function hide () { height = 0; visible = false }
    id: saveButtonRow
    height: 0;
    visible: false;
    width: childrenRect.width;
    anchors.top: parent.top
    anchors.topMargin: 10
    color: "transparent"
    ChoiceButton {
      id:saveButton
      labelText: "Save Feed"
      height: parent.height
      color: "green"
      onClicked: {
        console.log ("Save Feed")
        saveFeed (feedId, addrInput.urlString, feedTitle.textValue, siteUrl.textValue,
                feedNick.textValue, feedDescription.textValue)
      }
    } 
    ChoiceButton {
      id:deleteButton
      labelText: "Delete Feed"
      height: parent.height
      color: "red"
      anchors { left: saveButton.right; verticalCenter: saveButton.verticalCenter }
      onClicked: {
        console.log ("Delete Feed")
        deleteFeed (feedId)
      }
    }
    ChoiceButton {
      id:probeButton
      labelText: "Probe Feed"
      height: parent.height
      color: "yellow"
      anchors { left: deleteButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        console.log ("Probe Feed")
        probeFeed (addrInput.urlString)
      }
    }
  }
  AddressInput { 
    id: addrInput
    labelText: " Address "
    width: parent.width - parent.urlMargin
    anchors.top: parent.top
    anchors.topMargin: saveButton.height + 10
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
      labelWidth: feedDescription.labelWidth
    }
    LineInput {
      id: feedTitle
      width:parent.width
      anchors.top: feedNick.bottom
      labelText: " Title "
      labelWidth: feedDescription.labelWidth
    }
    LineInput {
      id: siteUrl
      width:parent.width
      anchors.top: feedTitle.bottom
      labelText: " Site Url "
      labelWidth: feedDescription.labelWidth
    }
    BoxInput {
      id: feedDescription
      width:parent.width
      anchors.top: siteUrl.bottom
      labelText: " Description "
      labelWidth: labelFloatRect.width
    }
  }
  Connections {
    target: addrInput
    onUrlEntered: {
      console.log ("entered URL " + url) 
    }
  } 
}
