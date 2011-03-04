

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
 import QtWebKit 1.0


 Rectangle {
     id: displayArea

     property string urlString : "http://fiji.reflective-computing.com"

     width: 800; height: 600
     color: "#f0f020"
     Component {
         id: storyItemDelegate
         Item {
             width: 160; height: 40
             Column {
                 Text { text: '<b>Day:</b> ' + day }
                 Text { text: '<b>Meal:</b> ' + meal }
             }
         }
     }

     ListView {
        id: storyList
        height: 60
        anchors.top:  displayArea.top
        anchors.topMargin: 20
        orientation: ListView.Vertical
        model: Menu {}
        delegate: storyItemDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
     }

     WebView {
        id: storyView
        anchors.top: storyList.bottom
        anchors.topMargin: 80
       
        preferredHeight: 500
        preferredWidth:  700
        settings.autoLoadImages: true
        html: "<p>This is <b>html</b>.</p>"
     }
     function setHtml (theHtml) {
        webView.html = theHtml
     }

     Item { id: headerSpace; width: parent.width; height: 62 }

     
 }