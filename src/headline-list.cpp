#include "headline-list.h"
#include <QHash>
#include <QByteArray>
#include <QDebug>

namespace deliberate
{

HeadlineList::HeadlineList (QObject *parent)
  :QAbstractListModel(parent)
{
  QHash<int, QByteArray> roles;
  roles[Type_Day] = "day";
  roles[Type_Meal] = "meal";
  setRoleNames(roles);
}

int 
HeadlineList::rowCount (const QModelIndex & parent) const
{
  qDebug () << "HeadlineList::rowCount " << parent << days.count();
  return days.count();
}

QString
HeadlineList::rowCountText ()
{
  qDebug () << "HeadlineList::rowCountText" ;
  return QString::number(days.count());
}


QVariant 
HeadlineList::data (const QModelIndex & index, int role) const
{
  qDebug () << " HeadlineList::data " << index << role;
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  if (role == Qt::DisplayRole) {
    return QString ("%1: %2").arg (days.value(row)).arg(meals.value(row));
  } else if (role == int (Type_Day)) {
    return days.value (row);
  } else if (role == int (Type_Meal)) {
    return meals.value (row);
  } else {
    return QVariant ();
  } 
}

void
HeadlineList::addLine (const QString & day, const QString & meal)
{
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  days << day;
  meals << meal;
  endInsertRows ();
  qDebug () << "HealineList::addLine " << day << meal;
}

} // namespace
