

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/



import QtQuick 1.0

Rectangle {
  id: feedEdit
  function displayNew (url) {
    console.log ("Start NEw Feed " + url)
    displayEditFeed ("", url, "", "", "", "", "")
    saveButtonRow.show ()
  }
  function displayEditFeed (theFeedId, theFeedUrl, theTitle, 
                            theSite, theNick, theDescr, theTopics) {
    feedId = theFeedId
    feedNick.textValue = theNick
    siteUrl.textValue = theSite
    addrInput.urlString = theFeedUrl
    feedTitle.textValue = theTitle
    feedDescription.textValue = theDescr
    siteTopics.textValue = theTopics
    choiceButtons.visible = true
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
    siteTopics.textValue = ""
    choiceButtons.visible = true
    feedDetails.visible = true
    saveButtonRow.show ()
  }
  function checkAddress () {
    var addr = addrInput.urlString
    if (addr == "") {
      popup.showIn (width, height, "Empty Address")
    } else if (controlIF.addressKnown (addr)) {
      loadEditFeed (addr)
      popup.showIn (width, height, "Address Known")
    } else {
      popup.showIn (width, height, "Unknown Feed Address")
    }
  }
  signal cancelEdit ()
  signal startNewFeed (string url)
  signal deleteFeed (string ident)
  signal probeFeed (string url)
  signal loadEditFeed (string addr)
  signal saveFeed (string ident, string feedUrl, string title, string siteUrl, 
                   string nick, string descr, string topics)

  property real normalWidth: parent.width
  property real urlMargin: 6
  property string feedId: ""
  property real normalRadius: 5
  width: 200
  height: childrenRect.height
  radius: normalRadius
  color: "palegoldenrod"
  Rectangle {
    function show () { height = 32; visible = true }
    function hide () { height = 0; visible = false }
    id: saveButtonRow
    height: 32;
    visible: true;
    width: childrenRect.width;
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.topMargin: 10
    color: "transparent"
    ChoiceButton {
      id:saveButton
      objectName: "FeedEditSaveButton"
      labelText: qsTr("Save Feed")
      height: parent.height
      color: "green"
      radius: height * 0.3333
      onClicked: {
        console.log ("Save Feed")
        saveFeed (feedId, addrInput.urlString, feedTitle.textValue, siteUrl.textValue,
                feedNick.textValue, feedDescription.textValue, siteTopics.textValue)
      }
    } 
    ChoiceButton {
      id:deleteButton
      objectName: "FeedEditDeleteButton"
      labelText: qstr("Delete Feed")
      height: parent.height
      color: "red"
      radius: height * 0.3333
      anchors { left: saveButton.right; verticalCenter: saveButton.verticalCenter }
      onClicked: {
        console.log ("Delete Feed")
        deleteFeed (feedId)
      }
    }
    ChoiceButton {
      id:probeButton
      objectName: "FeedEditProbeButton"
      labelText: qsTr("Probe Feed")
      height: parent.height
      color: "yellow"
      radius: height * 0.3333
      anchors { left: deleteButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        console.log ("Probe Feed")
        probeFeed (addrInput.urlString)
      }
    }
    ChoiceButton {
      id:checkButton
      objectName: "FeedEditCheckButton"
      labelText: qsTr("Check Address")
      height: parent.height
      color: "cyan"
      radius: height * 0.3333
      anchors { left: probeButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        console.log ("Check Address")
        checkAddress ()
      }
    }
    ChoiceButton {
      id: newButton
      objectName: "FeedEditNewButton"
      labelText: qsTr("New Feed")
      height: parent.height
      color: "#77ff77"
      radius: height * 0.3333
      anchors { left: checkButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        startNewFeed (addrInput.urlString)
      }
    }
    ChoiceButton {
      id:quitButton
      objectName: "FeedEditCancelButton"
      labelText: qsTr("Cancel")
      height: parent.height
      radius: height * 0.3333
      anchors { left: newButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        console.log ("Cancel Edit")
        feedEdit.cancelEdit ()
      }
    }
  }
  AddressInput { 
    id: addrInput
    objectName: "FeedEditAddressInput"
    labelText: qsTr(" Address ")
    width: parent.width - parent.urlMargin
    anchors.top: parent.top
    anchors.topMargin: saveButton.height + 10
    autoScroll: true
    selectByMouse: true
  }
  Rectangle {
    id: feedDetails
    color: "transparent"
    visible: true
    width: parent.width
    height: childrenRect.height
    anchors {
      top: addrInput.bottom; topMargin:  6 
    }
    LineInput {
      id: feedNick
      width:parent.width
      labelText: qsTr(" Nick ")
      labelWidth: feedDescription.labelWidth
    }
    LineInput {
      id: feedTitle
      width:parent.width
      anchors.top: feedNick.bottom
      labelText: qsTr(" Title ")
      labelWidth: feedDescription.labelWidth
    }
    LineInput {
      id: siteUrl
      width:parent.width
      anchors.top: feedTitle.bottom
      labelText: qsTr(" Site Url ")
      labelWidth: feedDescription.labelWidth
    }
    BoxInput {
      id: feedDescription
      width:parent.width
      anchors.top: siteUrl.bottom
      labelText: qsTr(" Description ")
      labelWidth: labelFloatRect.width
    }
    LineInput {
      id: siteTopics
      width:parent.width
      anchors.top: feedDescription.bottom
      labelText: qsTr(" Site Topics ")
      labelWidth: feedDescription.labelWidth
    }
  }
  Popup {
    id: popup
  }
  Connections {
    target: addrInput
    onUrlEntered: {
      console.log ("entered URL " + url) 
      probeFeed (addrInput.urlString)
    }
  } 
}
