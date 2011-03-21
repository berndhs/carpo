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

  void addLine (const QString & ident, const QString & title, bool seenit);
  QString addNewLine (const QString & title, bool seenit);

  void setFeedTitle (const QString & t);

  Q_INVOKABLE QString feedTitle () const;

  void markRead (const QString & id, bool seenit);
  void doReset () { reset(); }

  void    clear ();

  bool removeRows (int begin, int end, 
                   const QModelIndex & parent = QModelIndex());

  int count () { return rows.count(); }

  void StartNew (const QString & feedTitle);

public slots:

  void WasReset ();

private:

  enum DataType {
     Type_Ident = Qt::UserRole+1,
     Type_Title = Qt::UserRole+2,
     Type_Seenit = Qt::UserRole+3
  };

  class HeadlineRow {
  public:
    HeadlineRow (const QString & id, const QString t, bool s)
      :ident(id), title (t), seenit(s)
    {}
    HeadlineRow ()
      :ident(""), title(""), seenit(false)
    {}
    QString ident;
    QString title;
    bool    seenit;
  };

  QList <HeadlineRow>   rows;

  static int   nextId;
  QString      theTitle;
  
};

} // namespace

#endif
