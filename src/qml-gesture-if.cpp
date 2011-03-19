#include "qml-gesture-if.h"

#include <QGraphicsObject>
#include <QString>
#include <QGesture>
#include <QDebug>

namespace deliberate
{

GestureInterface::GestureInterface (QObject *parent)
  :QObject (parent)
{
  clock.start ();
}

void
GestureInterface::SetQmlRoot (QGraphicsObject * qmlRootPtr)
{
  qmlRoot = qmlRootPtr;
}

void
GestureInterface::pressed (const QString & objName, double x, double y)
{
  if (qmlRoot) {
    QObject* obj = qmlRoot->findChild<QObject*> (objName);
    lastPress[obj] = Spot (QVector2D (x,y), clock.elapsed());
    #if DRSS_GESTURE_TRACKING
    QGraphicsObject * gobj = qobject_cast<QGraphicsObject*> (obj);
    if (gobj) {  
      QMetaObject::invokeMethod (gobj, "setTracking",
                 Q_ARG (QVariant, true));
    }
    #endif
  }
}

void
GestureInterface::exited (const QString & objName, double x, double y)
{
  if (qmlRoot) {
    QObject* obj = qmlRoot->findChild<QObject*> (objName);
    QGraphicsObject * gobj = qobject_cast<QGraphicsObject*> (obj);
    if (gobj) {  
      QMetaObject::invokeMethod (gobj, "setTracking",
                 Q_ARG (QVariant, false));
      checkSwiped (gobj, QVector2D (x,y), clock.elapsed() );
    }          
    #if DRSS_GESTURE_TRACKING
    moveSteps[obj].clear ();
    #endif
  }
}

void
GestureInterface::released (const QString & objName, double x, double y)
{
  #if !DRSS_GESTURE_TRACKING
  Q_UNUSED (x)
  Q_UNUSED (y)
  #endif
  if (qmlRoot) {
    QObject* obj = qmlRoot->findChild<QObject*> (objName);
    QGraphicsObject * gobj = qobject_cast<QGraphicsObject*> (obj);
    if (gobj) {  
      QMetaObject::invokeMethod (gobj, "setTracking",
                 Q_ARG (QVariant, false));
    }          
    #if DRSS_GESTURE_TRACKING
    moveSteps[obj].clear ();
    #endif
  }
}

void
GestureInterface::checkSwiped (QGraphicsObject * gobj, 
                               const QVector2D  exitPos,
                                     int        exitTime)
{
  qDebug () << "GestureInterface :: checkSwiped ";
  Spot origSpot = lastPress[qobject_cast<QObject*>(gobj)];
  double dist = (exitPos - origSpot.pos()).length();
  double dX = exitPos.x() - origSpot.pos().x();
  double dY = exitPos.y() - origSpot.pos().y();
  qDebug () << "    dX " << dX << "  dY " << dY << "  dist " << dist;
  if (qAbs (dX) > qAbs(dY)) {
    if (dist > 50.0) {
      QMetaObject::invokeMethod (gobj, "swiped");
    }
  }
}

void
GestureInterface::moved (const QString & objName, double x, double y)
{
  #if DRSS_GESTURE_TRACKING
  if (qmlRoot) {
    QObject* obj = qmlRoot->findChild<QObject*> (objName);
    moveSteps[obj].append (Spot (QVector2D (x,y), clock.elapsed()));
  }
  #else
  Q_UNUSED (objName)
  Q_UNUSED (x)
  Q_UNUSED (y)
  #endif
}

void
GestureInterface::HandleGesture (QGesture * observed)
{
  qDebug () << "GestureInterface   ::HandleGesture " << observed;
}


QDebug
operator<<(QDebug dbg, const Spot & sp)
{
  dbg.nospace () << " ( Spot pos " << sp.pos()
                 << " time " << sp.time()
                 << " ) ";
  return dbg.space ();
}

} // namespace
