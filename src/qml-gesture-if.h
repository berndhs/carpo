#ifndef DELIBERATE_QML_GESTURE_IF_H
#define DELIBERATE_QML_GESTURE_IF_H

#include <QObject>
#include <QVector2D>
#include <QMap>
#include <QTime>
#include <QDebug>

#define DRSS_GESTURE_TRACKING 0

class QGraphicsObject;
class QString;
class QGesture;

namespace deliberate
{

class Spot
{
public:

  Spot (const QVector2D &thePos, int theTime) 
    :mPos(thePos), mTime (theTime)
  {}

  Spot ()
    :mPos (QVector2D(0,0)), mTime(0)
  {}

  QVector2D pos () const { return mPos; }
  int       time () const { return mTime; }

  void setPos (const QVector2D & thePos) { mPos = thePos; }
  void setTime (int t) { mTime = t; }

private:

  QVector2D   mPos;
  int         mTime;
}; 

QDebug 
operator<<(QDebug dbg, const Spot &sp);

class GestureInterface : public QObject
{
Q_OBJECT

public:
 
  GestureInterface (QObject *parent=0);

  void SetQmlRoot (QGraphicsObject * qmlRootPtr);

  Q_INVOKABLE void pressed (const QString & objName, double x, double y);
  Q_INVOKABLE void exited    (const QString & objName, double x, double y);
  Q_INVOKABLE void released    (const QString & objName, double x, double y);
  Q_INVOKABLE void moved    (const QString & objName, double x, double y);

  void HandleGesture (QGesture * observed);

private:

  void checkSwiped (QGraphicsObject * gobj, 
                    const QVector2D  exitPost,
                          int        exitTime);

  QGraphicsObject    * qmlRoot;
  QTime                clock;

  QMap <QObject*, Spot>   lastPress;
  #if DRSS_GESTURE_TRACKING
  QMap <QObject*, QList<Spot> >   moveSteps;
  #endif

}; 

} // namespace

#endif
