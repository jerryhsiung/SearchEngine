#ifndef WINDOW_H
#define WINDOW_H
#include <QString>
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "lib/set.h"
#include <map>
#include "lib/web.h"
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>


using namespace std;

class Window:public QWidget
{
Q_OBJECT public:
  Window (QWidget * parent = NULL);

  //set filename of webpage and pass in the maps with input data
  void setName (QString n, map < string, WebPage * >li, map < string,
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

  private slots:
    //refresh the page when clicked on an incoming link
  void updateIn ();

  //refresh the page when clicked on an outgoing link
  void updateOut ();

private:

    QVBoxLayout * linkBox, *left;	//linkbox for incoming and  outgoing links, left for content
  QHBoxLayout *webpage;		//main layout
  QLabel *content;		//displays the content of the webpage
  string name;			//name of the file
    map < string, Set < WebPage * > >maps;
    map < string, WebPage * >link;
  QScrollArea *scroll1;		//scroll area for incoming links
  QScrollArea *scroll2;		//scroll area for outgoing links
  QScrollArea *scroll3;		//scroll area for content
  QListWidget *incoming;	//incoming links list
  QListWidget *outgoing;	//outgoing links list
  QLabel in, out, nm;		//incoming, outgoing, filename labels
};

#endif
