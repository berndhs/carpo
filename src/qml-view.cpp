#include "qml-view.h"
#include <QGesture>
#include <QGestureEvent>
#include <QEvent>
#include <QDebug>

namespace deliberate
{

QmlView::QmlView (QWidget *parent)
  :QDeclarativeView (parent)
{
     grabGesture(Qt::PanGesture);
     grabGesture(Qt::PinchGesture);
     grabGesture(Qt::SwipeGesture);
}

bool
QmlView::event (QEvent *event)
{
  if (event->type() == QEvent::Gesture) {
    QGestureEvent * gev = static_cast <QGestureEvent*> (event);
    QGesture *swipe = gev->gesture(Qt::SwipeGesture);
    if (swipe) {
      qDebug () << " QmlView  :: event saw swipte " << swipe;
    }
  }
  return QDeclarativeView::event (event);
}

} // namespace