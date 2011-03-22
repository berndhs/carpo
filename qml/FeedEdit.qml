

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
    siteTopics.textValue = ""
    choiceButtons.visible = true
    feedDetails.visible = false
    saveButtonRow.hide ()
  }
  function checkAddress () {
    var addr = addrInput.urlString
    if (addr == "") {
      popup.showIn (width, height, "Empty Address")
    } else if (controlIF.addressKnown (addr)) {
      popup.showIn (width, height, "Address Known")
    } else {
      popup.showIn (width, height, "Unknown Feed Address")
    }
  }
  signal startNewFeed (string url)
  signal deleteFeed (string ident)
  signal probeFeed (string url)
  signal saveFeed (string ident, string feedUrl, string title, string siteUrl, 
                   string nick, string descr, string topics)
  id: feedEdit
  property real normalWidth: parent.width
  property real urlMargin: 6
  property string feedId: ""
  property real normalRadius: 5
  width: 200
  height: 200
  radius: normalRadius
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
      objectName: "FeedEditSaveButton"
      labelText: "Save Feed"
      height: parent.height
      color: "green"
      onClicked: {
        console.log ("Save Feed")
        saveFeed (feedId, addrInput.urlString, feedTitle.textValue, siteUrl.textValue,
                feedNick.textValue, feedDescription.textValue, siteTopics.textValue)
      }
    } 
    ChoiceButton {
      id:deleteButton
      objectName: "FeedEditDeleteButton"
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
      objectName: "FeedEditProbeButton"
      labelText: "Probe Feed"
      height: parent.height
      color: "yellow"
      anchors { left: deleteButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        console.log ("Probe Feed")
        probeFeed (addrInput.urlString)
      }
    }
    ChoiceButton {
      id:checkButton
      objectName: "FeedEditProbeButton"
      labelText: "Check Address"
      height: parent.height
      color: "cyan"
      anchors { left: probeButton.right; verticalCenter: deleteButton.verticalCenter }
      onClicked: {
        console.log ("Check Address")
        checkAddress ()
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
      objectName: "FeedEditNewButton"
      labelText: qsTr("New Feed")
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
