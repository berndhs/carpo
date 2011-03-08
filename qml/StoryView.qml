

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

  anchors.left: parent.left
  anchors.right: parent.right
  width: parent.width
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
    html: "<p>default <b>html</b>.</p>"
         onAlert: console.log(message)

         function doZoom(zoom,centerX,centerY)
         {
             if (centerX) {
                 var sc = zoom*contentsScale;
                 scaleAnim.to = sc;
                 flickVX.from = flickable.contentX
                 flickVX.to = Math.max(0,Math.min(centerX-flickable.width/2,theWebView.width*sc-flickable.width))
                 finalX.value = flickVX.to
                 flickVY.from = flickable.contentY
                 flickVY.to = Math.max(0,Math.min(centerY-flickable.height/2,theWebView.height*sc-flickable.height))
                 finalY.value = flickVY.to
                 quickZoom.start()
             }
         }

         Keys.onLeftPressed: theWebView.contentsScale -= 0.1
         Keys.onRightPressed: theWebView.contentsScale += 0.1

         preferredWidth: flickable.width
         preferredHeight: flickable.height
         contentsScale: 1
         onContentsSizeChanged: {
             // zoom out
             contentsScale = Math.min(1,flickable.width / contentsSize.width)
         }
         onUrlChanged: {
             // got to topleft
             flickable.contentX = 0
             flickable.contentY = 0
             if (url != null) { header.editUrl = url.toString(); }
         }

         onDoubleClick: {
                         if (!heuristicZoom(clickX,clickY,2.5)) {
                             var zf = flickable.width / contentsSize.width
                             if (zf >= contentsScale)
                                 zf = 2.0/zoomFactor // zoom in (else zooming out)
                             doZoom(zf,clickX*zf,clickY*zf)
                          }
                        }
 SequentialAnimation {
             id: quickZoom

             PropertyAction {
                 target: theWebView
                 property: "renderingEnabled"
                 value: false
             }
             ParallelAnimation {
                 NumberAnimation {
                     id: scaleAnim
                     target: theWebView
                     property: "contentsScale"
                     // the to property is set before calling
                     easing.type: Easing.Linear
                     duration: 200
                 }
                 NumberAnimation {
                     id: flickVX
                     target: flickable
                     property: "contentX"
                     easing.type: Easing.Linear
                     duration: 200
                     from: 0 // set before calling
                     to: 0 // set before calling
                 }
                 NumberAnimation {
                     id: flickVY
                     target: flickable
                     property: "contentY"
                     easing.type: Easing.Linear
                     duration: 200
                     from: 0 // set before calling
                     to: 0 // set before calling
                 }
             }
             // Have to set the contentXY, since the above 2
             // size changes may have started a correction if
             // contentsScale < 1.0.
             PropertyAction {
                 id: finalX
                 target: flickable
                 property: "contentX"
                 value: 0 // set before calling
             }
             PropertyAction {
                 id: finalY
                 target: flickable
                 property: "contentY"
                 value: 0 // set before calling
             }
             PropertyAction {
                 target: theWebView
                 property: "renderingEnabled"
                 value: true
             }
         }
         onZoomTo: doZoom(zoom,centerX,centerY)
  }
  
 }