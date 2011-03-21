#include "headline-list.h"
#include <QHash>
#include <QByteArray>
#include <QDebug>

namespace deliberate
{

int HeadlineList::nextId (33333);

HeadlineList::HeadlineList (QObject *parent)
  :QAbstractListModel(parent),
   theTitle ("Headlines")
{
  QHash<int, QByteArray> roles;
  roles[Type_Ident] = "ident";
  roles[Type_Title] = "title";
  roles[Type_Seenit] = "seenit";
  setRoleNames(roles);
  connect (this, SIGNAL (modelReset()), this, SLOT (WasReset()));
}

int 
HeadlineList::rowCount (const QModelIndex & parent) const
{
  Q_UNUSED (parent)
  return rows.count();
}

void
HeadlineList::clear ()
{
  int oldRows = rows.count();
  removeRows (0,oldRows-1);
}

bool
HeadlineList::removeRows (int begin, int end, const QModelIndex & parent)
{
  beginRemoveRows (parent, begin, end);
  for (int i=end; i>= begin; i--) {
    rows.removeAt (i);
  }
  endRemoveRows ();
  return true;
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
    retval = QString ("%1: %2").arg (rows[row].title);
  } else if (role == int (Type_Ident)) {
    retval = rows[row].ident;
  } else if (role == int (Type_Title)) {
    retval = rows[row].title;
  } else if (role == int (Type_Seenit)) {
    retval = rows[row].seenit;
  } else {
    retval = QVariant ();
  } 
  return retval;
}


QString 
HeadlineList::feedTitle () const
{
  return theTitle;
}

void
HeadlineList::setFeedTitle (const QString & t)
{
  theTitle = t;
  for (int col=0; col<4; col++) {
    setHeaderData (col, Qt::Horizontal, QVariant (theTitle), Qt::DisplayRole);
    setHeaderData (col, Qt::Vertical, QVariant (theTitle), Qt::DisplayRole);
    setHeaderData (col, Qt::Horizontal, QVariant (theTitle), Qt::EditRole);
    setHeaderData (col, Qt::Vertical, QVariant (theTitle), Qt::EditRole);
  }
  reset ();
}

void
HeadlineList::StartNew (const QString & feedTitle )
{
  clear ();
  setFeedTitle (feedTitle);
}

void
HeadlineList::WasReset ()
{
  qDebug () << " HeadlineList :: WasReset";
}

void
HeadlineList::addLine (const QString & ident, const QString & title, bool seenit)
{
  beginInsertRows (QModelIndex(), rowCount(), rowCount());
  rows.append (HeadlineRow (ident, title, seenit));
  endInsertRows ();
}

QString
HeadlineList::addNewLine (const QString & title, bool seenit)
{
  QString ident (QString ("Story_%1").arg(nextId++));
  addLine (ident, title, seenit);
  return ident;
}

void
HeadlineList::markRead (const QString & id, bool seenit)
{
  int rc = rows.count();
  for (int r=0; r<rc; r++) {
    if (rows.at(r).ident == id) {
      rows[r].seenit = seenit;
      QModelIndex rowIndex = createIndex (r, 0);
      emit dataChanged (rowIndex, rowIndex);
    }
  }
}

} // namespace
