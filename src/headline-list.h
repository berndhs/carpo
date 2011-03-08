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
  void addLine (const QString & ident, const QString & title);
  QString addNewLine (const QString & title);
  void    clear ();

  Q_INVOKABLE QString rowCountText ();

  bool removeRows (int begin, int end, 
                   const QModelIndex & parent = QModelIndex());

private:

  enum DataType {
     Type_Ident = Qt::UserRole+1,
     Type_Title = Qt::UserRole+2
  };

  QStringList   idents;
  QStringList   titles;

  static int   nextId;
  
};

} // namespace

#endif
