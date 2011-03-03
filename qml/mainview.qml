import QtQuick 1.0
import QtWebKit 1.0

Rectangle {

  id: mainView

  property string mainUrl : "http://192.168.1.152/test/nada.html"
  width: 600
  height: 400
  color: "transparent"


  WebView {
    id: mainWebView
    url: mainView.mainUrl
  }

  Item { id: headerSpace; width: parent.width; height: 62 }

  Header {
    id: header
    editUrl: mainWebView.url
    width: headerSpace.width; height: headerSpace.height
  }

}
