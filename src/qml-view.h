#ifndef DELIBERATE_QML_VIEW_H
#define DELIBERATE_QML_VIEW_H

#include <QDeclarativeView>

namespace deliberate
{

class QmlView : public QDeclarativeView
{
Q_OBJECT

public:

  QmlView (QWidget *parent=0);

protected:

  bool event (QEvent *event);

};

} // namespace


#endif
