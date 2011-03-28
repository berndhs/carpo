
import QtQuick 1.0

Rectangle {
  id: popup

  property  real parentWidth: 300
  property  real parentHeight: 200
  property  real showTime: 5000
  color: "red"
  radius: 4

  y: parent.height // off "screen"
  anchors.horizontalCenter: parent.horizontalCenter
  width: label.width + 5
  height: label.height + 5

  opacity: 0

  function show(text) {
    label.text = text
    popup.state = "visible"
    timer.start()
  }
  function showIn (wide, high, text) {
    parentWidth = wide; parentHeight = high
    show (text)
  }
  states: State {
    name: "visible"
    PropertyChanges { target: popup; opacity: 1 }
    PropertyChanges { target: popup; y: (parentHeight-popup.height)/2 }
  }

  transitions: [
    Transition { from: ""; PropertyAnimation { properties: "opacity,y"; duration: 65 } },
    Transition { from: "visible"; PropertyAnimation { properties: "opacity,y"; duration: 500 } }
  ]

  Timer {
    id: timer
    interval: showTime
    onTriggered: popup.state = ""
  }

  Text {
    id: label
    anchors.centerIn: parent
    width: parentWidth  *0.75

    color: "white"
    font.pixelSize: 20
    wrapMode: Text.WordWrap
    horizontalAlignment: Text.AlignHCenter
    smooth: true
  }
  MouseArea {
    anchors.fill: parent
    onClicked: popup.state = ""
  }
}
