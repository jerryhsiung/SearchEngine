#ifndef MAINWIN_H
#define MAINWIN_H
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QComboBox>
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "lib/set.h"
#include <map>
#include "lib/web.h"
#include <QScrollArea>
#include <QListWidget>
#include "win.h"
#include "msort.h"

using namespace std;

class MainWin:public QWidget
{
Q_OBJECT public:
  MainWin (QWidget * parent = NULL);

  //function to pass in maps that contain filename/webpage pointers
  void setMaps (map < string, WebPage * >l, map < string,
		Set < WebPage * > >m);

  // Help define initial size
  QSize sizeHint () const
  {
    return QSize (300, 100);
  }
  // Not totally useful in the main widget/window
  QSize minimumSizeHint () const
  {
    return QSize (200, 150);
  }

  private slots:void searchFunc ();
  //function to search the webpages for words
  void quitClicked ();
  void open ();
  //function to open a new webpage when clicked on a link

private:

  QLineEdit * input;		//input text field
  QPushButton *search;		//search button
  QPushButton *quit;		//quit button
  QComboBox *selection;		//and/or/single word selection box
  QComboBox *sort;
  map < string, Set < WebPage * > >maps;	// words and the webpage pointers of the pages that contain them
  map < string, WebPage * >link;	//filenames and their webpage pointers
  QScrollArea *scroll;		//scrollarea for the results
  QListWidget *q;		//list to show query results
  Window w;			//window to be opened when clicked on a link
  QVBoxLayout *page;

};

#endif
