#include "mainwin.h"
#include <string>
#include <iostream>

using namespace std;

Window::Window (QWidget * parent):QWidget (parent)
{
  left = new QVBoxLayout;
  linkBox = new QVBoxLayout;
  scroll1 = new QScrollArea ();
  scroll2 = new QScrollArea ();
  scroll3 = new QScrollArea ();

  scroll1->setWidgetResizable (true);
  incoming = new QListWidget ();
  scroll1->setWidget (incoming);

  scroll2->setWidgetResizable (true);
  outgoing = new QListWidget ();
  scroll2->setWidget (outgoing);

  scroll3->setWidgetResizable (true);

  in.setText ("Incoming");	//label for incoming links
  out.setText ("Outgoing");	//label for outgoing

  linkBox->addWidget (&in);
  linkBox->addWidget (scroll1);	//add the incoming links
  linkBox->addWidget (&out);
  linkBox->addWidget (scroll2);	//add the outgoing links

  webpage = new QHBoxLayout;
  content = new QLabel ();
  scroll3->setWidget (content);	//content in the scroll box area

  left->addWidget (&nm);
  left->addWidget (scroll3);
  webpage->addLayout (left);
  webpage->addLayout (linkBox);

  //update page when double clicked on an incoming link
  connect (incoming, SIGNAL (itemDoubleClicked (QListWidgetItem *)), this,
	   SLOT (updateIn ()));

  //update page when double clicked on an outgoing link
  connect (outgoing, SIGNAL (itemDoubleClicked (QListWidgetItem *)), this,
	   SLOT (updateOut ()));
  setLayout (webpage);

this->setMinimumSize(1000, 500);
}

void
Window::setName (QString n, map < string, WebPage * >l, map < string,
		 Set < WebPage * > >m)
{

  setWindowTitle (n);		//set title to be the filename
  name = n.toStdString ();
  nm.setText (n);		//set the filename label text
  link = l;			//get the passed in maps with input data
  maps = m;
  string output;		//string to display content of the webpage

  //get all the words of the webpage (without getting rid of the special characters)
  vector < string > word = l[name]->getWords ();
  for (unsigned int i = 0; i < word.size (); i++)
    {

      output += " " + word.at (i);
    }

  content->setText (QString::fromStdString (output));	//update the text to display content of webpage
  content->setWordWrap (true);	//so that content isn't one long line
  incoming->clear ();		//get rid of previous incoming links
  outgoing->clear ();		//get rid of previous outgoing links
  vector < string > i = l[name]->allIncomingLinks ();
content->setAlignment(Qt::AlignTop);
  //add all incoming links of webpage to the incoming widget
  for (vector < string >::iterator it = i.begin (); it != i.end (); ++it)
    {

      QString str = QString::fromStdString (*it);
      incoming->addItem (str);
    }

  //add all outgoing links of webpage to the incoming widget
  vector < string > o = l[name]->allOutgoingLinks ();
  for (vector < string >::iterator it = o.begin (); it != o.end (); ++it)
    {

      QString aa = QString::fromStdString (*it);
      outgoing->addItem (aa);
    }
}

void
Window::updateIn ()
{

  //set the new filename to be the chosen link in the incoming list
  setName (incoming->currentItem ()->text (), link, maps);

}


void
Window::updateOut ()
{

  //set the new filename to be the chosen link in the outgoings list
  setName (outgoing->currentItem ()->text (), link, maps);

}
