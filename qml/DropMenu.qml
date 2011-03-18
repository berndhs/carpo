
import QtQuick 1.0

Rectangle {
  id: dropMenu
  property real itemHeight: 32
  property real itemWidth: width
  property real floatHeight: childrenRect.height
  property real floatWidth: childrenRect.width
  property real rollDelay: 125
  property real initialYScale: 0

  signal leave ()

  function hide () {
    rollupScale.yScale = 0
    console.log ("shink running ")
  }
  function show () {
    rollupScale.yScale = 1
  }
  transform: Scale {
    id: rollupScale
    xScale: 1
    yScale: initialYScale
    Behavior  on yScale {
      NumberAnimation { duration: rollDelay }
    }
  }

  width: 200
  height: 200
  color: "green"
}