#include "headline-list.h"
#include <QHash>
#include <QByteArray>
#include <QDebug>

namespace deliberate
{

int HeadlineList::nextId (33333);

HeadlineList::HeadlineList (QObject *parent)
  :QAbstractListModel(parent)
{
  QHash<int, QByteArray> roles;
  roles[Type_Ident] = "ident";
  roles[Type_Title] = "title";
  setRoleNames(roles);
}

int 
HeadlineList::rowCount (const QModelIndex & parent) const
{
  return idents.count();
}

void
HeadlineList::clear ()
{
  int oldRows = idents.count();
  removeRows (0,oldRows-1);
}

bool
HeadlineList::removeRows (int begin, int end, const QModelIndex & parent)
{
  beginRemoveRows (parent, begin, end);
  for (int i=end; i>= begin; i--) {
    idents.removeAt (i);
    titles.removeAt (i);
  }
  endRemoveRows ();
  return true;
}

QString
HeadlineList::rowCountText ()
{
  return QString::number(idents.count());
}


QVariant 
HeadlineList::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  QVariant retval;
  if (role == Qt::DisplayRole) {
    retval = QString ("%1: %2").arg (idents.value(row)).arg(titles.value(row));
  } else if (role == int (Type_Ident)) {
    retval = idents.value (row);
  } else if (role == int (Type_Title)) {
    retval = titles.value (row);
  } else {
    retval = QVariant ();
  } 
  return retval;
}

void
HeadlineList::addLine (const QString & ident, const QString & title)
{
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  idents << ident;
  titles << title;
  endInsertRows ();
}

QString
HeadlineList::addNewLine (const QString & title)
{
  QString ident (QString ("Story_%1").arg(nextId++));
  addLine (ident, title);
  return ident;
}

} // namespace
