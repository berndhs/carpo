

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
  *  Parts of this file are from the QtDeclarative module (Qt4.7.1),
  *  used under the conditions of the LGPL version 2.
  */

 import QtQuick 1.0
 import QtWebKit 1.0


Flickable {

  property alias storyHtml: theWebView.html
  property alias title: theWebView.title
  property alias icon: theWebView.icon
  property alias progress: theWebView.progress
  property alias url: theWebView.url
  property alias back: theWebView.back
  property alias stop: theWebView.stop
  property alias reload: theWebView.reload
  property alias forward: theWebView.forward

  id: flickable

  function back () { console.log ("Back!!"); theWebView.back.trigger() }
  function forward () { console.log ("Forward ->>>" ); theWebView.forward.trigger () }
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
    settings.autoLoadImages: true
    settings.pluginsEnabled: true
    html: "<p>default <b>html</b>.</p>"
    property bool isLoadFinished : false
    onAlert: {
      controlIF.checkAlert (message)
      console.log(message)
    }

    Keys.onLeftPressed: theWebView.contentsScale -= 0.1
    Keys.onRightPressed: theWebView.contentsScale += 0.1

    preferredWidth: flickable.width
    preferredHeight: flickable.height
    contentsScale: 1
    onContentsSizeChanged: {
      // zoom out
      if (isLoadFinished) {
        contentsScale = Math.min(1,flickable.width / contentsSize.width)
      }
    }
    onUrlChanged: {
      // got to topleft
      isLoadFinished = false
      flickable.contentX = 0
      flickable.contentY = 0
    }
    onDoubleClick: {
      if (!heuristicZoom(clickX,clickY,2.5)) {
        var zf = flickable.width / contentsSize.width
        if (zf >= contentsScale) zf = 2.0/zoomFactor // zoom in (else zooming out)
        doZoom(zf,clickX*zf,clickY*zf)
      }
    } 
    onLoadFinished: { isLoadFinished = true }
    onLoadFailed: { isLoadFinished = true }
    onLoadStarted: { isLoadFinished = false }
  }
}
