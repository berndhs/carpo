

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

/**
  *  Small parts of this file are from the QtDeclarative module (Qt4.7.1),
  *  used under the conditions of the LGPL version 2.
  */

 import QtQuick 1.0
 import QtWebKit 1.0


Flickable {

  id: storyViewBox

  property alias storyHtml: theWebView.html
  property alias title: theWebView.title
  property alias icon: theWebView.icon
  property alias progress: theWebView.progress
  property alias url: theWebView.url
  property alias back: theWebView.back
  property alias stop: theWebView.stop
  property alias reload: theWebView.reload
  property alias forward: theWebView.forward
  property alias scrollXStep: theWebView.scrollXStep
  property alias scrollYStep: theWebView.scrollYStep
  property bool isWeb: false

  signal quitit ()

  //function back () { console.log ("Back!!"); theWebView.back.trigger() }
  //function forward () { console.log ("Forward ->>>" ); theWebView.forward.trigger () }
  function setTheHtml (theHtml) {
    theWebView.isLoadFinished = true
    theWebView.html = theHtml
  }
  function deux () {
    if (interactive) {
      interactive = false
      theWebView.pressGrabTime = 0
    } else {
      interactive = true
      theWebView.pressGrabTime = 400
    }
    console.log ("changed grab time to " + theWebView.pressGrabTime)
  }
  function wheelTurned (theX, theY, theOrientation, theDelta) {
   var steps = theDelta * 0.0166667 
    if (theOrientation == 1) {
      contentX += scrollXStep * steps
    } else if (theOrientation == 2) {
      contentY += scrollYStep * steps
    }
  }
  anchors.left: parent.left
  anchors.right: parent.right
  width: parent.width
  height: parent.height
  contentWidth: Math.max(parent.width,theWebView.width)
  contentHeight: Math.max(parent.height,theWebView.height)
  onWidthChanged : {
    // Expand (but not above 1:1) if otherwise would be smaller that available width.
    if (width > theWebView.width*theWebView.contentsScale && theWebView.contentsScale < 1.0)
      theWebView.contentsScale = width / theWebView.width * theWebView.contentsScale;
  }

  WebView {
    id: theWebView
    objectName: "StoryWebView"
    settings.autoLoadImages: true
    settings.pluginsEnabled: true
    html: "<p>default <b>html</b>.</p>"
    property bool isLoadFinished : false
    property real origScale: 1
    property real scrollXStep: -5
    property real scrollYStep: -5

    Keys.onEscapePressed: storyViewBox.quitit()
    Keys.onLeftPressed: storyView.contentX += scrollXStep 
    Keys.onRightPressed: storyView.contentX -= scrollXStep 
    Keys.onUpPressed: storyViewBox.contentY +=  scrollYStep
    Keys.onDownPressed: storyViewBox.contentY -= scrollYStep
    Keys.onSpacePressed: { storyViewBox.contentX = 0; storyViewBox.contentY = 0 }
    Keys.onPressed: {
       if (event.text == "+") { theWebView.contentsScale += 0.1 } 
       else if (event.text == "-") {theWebView.contentsScale -= 0.1 }
       else if (event.text == "0") {theWebView.contentsScale = origScale }
    }
    preferredWidth: storyViewBox.width
    preferredHeight: storyViewBox.height
    contentsScale: 1
    onContentsSizeChanged: {
      // zoom out
      if (isLoadFinished) {
        contentsScale = Math.min(1,storyViewBox.width / contentsSize.width)
      }
    }
    onAlert: {
      controlIF.checkAlert (message)
      console.log(message)
    }
    onUrlChanged: {
      // got to topleft
      isLoadFinished = false
      storyViewBox.contentX = 0
      storyViewBox.contentY = 0
      storyViewBox.isWeb = ! controlIF.isEmptyUrl (url)
    }
    onDoubleClick: {
      storyViewBox.deux ()
    } 
    onLoadFinished: { 
      origScale = contentsScale; 
      isLoadFinished = true; 
      loadIndicator.visible = false
    }
    onLoadFailed: { isLoadFinished = true ; loadIndicator.visible = false }
    onLoadStarted: { 
      if (!storyViewBox.isWeb) controlIF.pushHtml()
      isLoadFinished = false
      loadIndicator.visible = true 
    }
  }
  Rectangle {
    id: loadIndicator
    width: 52; height: 26
    radius: height * 0.25
    anchors { top: theWebView.top; right: theWebView.right }
    opacity: 0.5
    color: "red"
    Text { 
      anchors { 
        verticalCenter: parent.verticalCenter
        horizontalCenter: parent.horizontalCenter 
      }
      color: "black"
      text: qsTr ("Load") 
    }
    visible: false
  }
}
