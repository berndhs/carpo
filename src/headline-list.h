#ifndef DELIBERATE_HEADLINE_LIST_H
#define DELIBERATE_HEADLINE_LIST_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QStringList>
#include <QString>

namespace deliberate
{

class HeadlineList : public QAbstractListModel
{
Q_OBJECT

public:
 
  HeadlineList (QObject *parent=0);

  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
  void addLine (const QString & day, const QString & meal);

  Q_INVOKABLE QString rowCountText ();

private:

  enum DataType {
     Type_Day = Qt::UserRole+1,
     Type_Meal = Qt::UserRole+2
  };

  QStringList   days;
  QStringList   meals;
  
};

} // namespace

#endif
