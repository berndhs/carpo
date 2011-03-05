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
  return days.count();
}

QString
HeadlineList::rowCountText ()
{
  return QString::number(days.count());
}


QVariant 
HeadlineList::data (const QModelIndex & index, int role) const
{
qDebug () << "headline data " << index << role;
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  QVariant retval;
  if (role == Qt::DisplayRole) {
    retval = QString ("%1: %2").arg (days.value(row)).arg(meals.value(row));
  } else if (role == int (Type_Day)) {
    retval = days.value (row);
  } else if (role == int (Type_Meal)) {
    retval = meals.value (row);
  } else {
    retval = QVariant ();
  } 
  qDebug () << " HeadlineList::data returning " << retval;
  return retval;
}

void
HeadlineList::addLine (const QString & day, const QString & meal)
{
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  days << day;
  meals << meal;
  endInsertRows ();
}

} // namespace
