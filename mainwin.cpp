#include "mainwin.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

MainWin::MainWin (QWidget * parent):QWidget (parent)
{
  search = new QPushButton ("&Search", this);
  quit = new QPushButton ("&Quit", this);
  input = new QLineEdit (this);
  scroll = new QScrollArea ();

  scroll->setWidgetResizable (true);
  q = new QListWidget ();
  scroll->setWidget (q);

  QVBoxLayout *
  right = new QVBoxLayout;	//the box that has the search and quit buttons
  right->addWidget (search);
  right->addWidget (quit);

  sort = new QComboBox ();
  sort->addItem ("Alphabetically", 0);
  sort->addItem ("PageRank", 1);

  selection = new QComboBox ();
  selection->addItem ("AND", 0);	//add the and option
  selection->addItem ("OR", 1);	//add the or option
  selection->addItem ("Single Word", 2);	//add the single word option

  QVBoxLayout *
  menu = new QVBoxLayout;
  menu->addWidget (selection);
  menu->addWidget (sort);

  QHBoxLayout *
  query = new QHBoxLayout;	//the box that has the search buttons/input field
  query->addLayout (menu);
  query->addWidget (input);
  query->addLayout (right);

  page = new QVBoxLayout;	//main layout
  page->addLayout (query);	//the input area
  page->addWidget (scroll);	//the output area

  connect (search, SIGNAL (clicked ()), this, SLOT (searchFunc ()));	//search if clicked on search
  connect (quit, SIGNAL (clicked ()), this, SLOT (quitClicked ()));	//quit if clicked on quit
  connect (q, SIGNAL (itemDoubleClicked (QListWidgetItem *)), this, SLOT (open ()));	//open new page if double clicked on a result

  setLayout (page);
  setWindowTitle ("Search Engine");	//change title of the main window

this->setMinimumSize(500, 500);
}


void
MainWin::searchFunc ()
{


  string t = input->text ().toStdString ();	//get the word(s) to search
  vector < string > words;	//vector in case there are multiple words
  bool space = true;
  int start = 0;
  Set < WebPage * >result;
  WebPage temp;			//to convert strings to lowercase

  for (unsigned int i = 0; i < t.length (); i++)
    {

      //get the substring up to that index when there is a space
      if (isspace (t.at (i)))
	{
	  if (!space)
	    words.push_back (temp.lower (t.substr (start, i - start)));

	  start = i + 1;
	  space = true;
	}
      else
	{
	  space = false;
	  //get the substring if at the end of string and it isn't a space
	  if (i == t.length () - 1)
	    words.push_back (temp.lower (t.substr (start)));
	}
    }

  //if the search option was "and"
  if (selection->currentText () == "AND")
    {

      //search only if there are multiple words
      if (words.size () > 1)
	{

	  //don't look at the other words if first word isn't in a file
	  if (maps.find (words.at (0)) != maps.end ())
	    {

	      result = maps.at (words.at (0));
	      for (unsigned int i = 1; i < words.size (); i++)
		{

		  Set < WebPage * >newset;

		  //if word is in the map
		  if (maps.find (words.at (i)) != maps.end ())
		    {

		      newset = maps.at (words.at (i));
		      result = result.setIntersection (newset);	//get the intersection of two webpage pointer sets
		    }
		  else
		    {
		      result.clear ();	//empty the result if word isn't in a file
		      break;	//break out of the loop
		    }
		}
	    }
	}
    }

  //if the search option was or
  else if (selection->currentText () == "OR")
    {

      //search only if there are multiple words
      if (words.size () > 1)
	{

	  //keep looking even if first word isn't in the map
	  if (maps.find (words.at (0)) != maps.end ())
	    {

	      result = maps.at (words.at (0));
	    }
	  for (unsigned int i = 1; i < words.size (); i++)
	    {

	      Set < WebPage * >newset;
	      if (maps.find (words.at (i)) != maps.end ())
		{

		  newset = maps.at (words.at (i));
		  result = result.setUnion (newset);	//get the union of two sets

		}
	    }
	}
    }


  //if the search option was single word
  else
    {

      //search only if there one word
      if (words.size () == 1)
	{

	  //if word is in a page
	  if (maps.find (words.at (0)) != maps.end ())
	    result = maps.at (words.at (0));
	}
    }
  Set < WebPage * >setout;

  for (Set < WebPage * >::iterator it = result.begin (); it != result.end ();
       ++it)
    {
	//add the outgoing links to the results
      vector < string > outgoing = (*it)->allOutgoingLinks ();
      for (vector < string >::iterator a = outgoing.begin ();
	   a != outgoing.end (); ++a)
	{

	  if (result.find (link[*a]) == result.end ())
	    setout.insert (link[*a]);

	}
	//add the incoming links to the results
      vector < string > in = (*it)->allIncomingLinks ();
      for (vector < string >::iterator a = in.begin (); a != in.end (); ++a)
	{

	  if (result.find (link[*a]) == result.end ())
	    setout.insert (link[*a]);

	}
    }

  result = result.setUnion (setout);
  vector < WebPage * >list;
  if (sort->currentText () == "Alphabetically")
    {


      for (Set < WebPage * >::iterator it = result.begin ();
	   it != result.end (); ++it)
	list.push_back (*it); //copy the set items to a vector to be able to call mergesort
      AlphaStrComp a;
      Merge < WebPage *, AlphaStrComp > m;
      m.mergeSort (list, a);
    }
  else
    {


      map < WebPage *, vector < string > >v;
      double remaining;

      for (Set < WebPage * >::iterator a = result.begin ();
	   a != result.end (); ++a)
	{
	//create the vertices for the graph
	  vector < string > s = (*a)->allOutgoingLinks ();
	  vector < string > vertex;
	  for (unsigned int out = 0; out < s.size(); out++)
	    {

	      if (result.find (link[s[out]]) != result.end ())
		vertex.push_back (s[out]);

	    }
	  v[*a] = vertex;
	}


      int count = 0;
      for (map < WebPage *, vector < string > >::iterator it = v.begin ();
	   it != v.end (); ++it)
	{
	  it->first->setRank ((double) 1 / (double) v.size ());

	}


      if (v.size () > 1)
	{
	  while (count < 30)
	    {
	      map < string, double >r;
	      for (map < WebPage *, vector < string > >::iterator it3 =
		   v.begin (); it3 != v.end (); ++it3)
		{

		  vector < string > s = it3->second;

		  for (unsigned int out = 0; out < s.size(); out++)
		    {


		      r[s[out]] +=
			(1 - 0.15) * (it3->first->getRank()) / s.size ();


		    }

		  if (s.empty ())
		    {

		      remaining = it3->first->getRank () / (v.size ());

		    }
		  else
		    {

		      remaining =
			0.15 * (it3->first->getRank ()) / (v.size ());
		    }
		  for (map < WebPage *, vector < string > >::iterator ite =
		       v.begin (); ite != v.end (); ++ite)
		    {

		      r[ite->first->filename ()] += remaining;

		    }

		}
	      count++;

	      for (map < WebPage *,
		   vector < string >
		   >::iterator it2 = v.begin (); it2 != v.end (); ++it2)
		{
		  string name = it2->first->filename ();

		  it2->first->setRank (r[name]);

		}

	    }

	}


      for (map < WebPage *,
	   vector < string >
	   >::iterator it2 = v.begin (); it2 != v.end (); ++it2)
	{
	  list.push_back (it2->first);
	}
      RankComp rank;
      Merge < WebPage *, RankComp > m;
      m.mergeSort (list, rank);
    }

  q->clear ();			//clear the previous results
  for (vector < WebPage * >::iterator it = list.begin (); it != list.end ();
       ++it)
    {
      QString str = QString::fromStdString ((*it)->filename ());
      q->addItem (str);		//add the results one by one to the list
    }

}

void
MainWin::setMaps (map <
		  string, WebPage * >l, map < string, Set < WebPage * > >m)
{

  link = l;
  maps = m;
}

void
MainWin::quitClicked ()
{
  close ();			//close the window
}

void
MainWin::open ()
{

  w.setName (q->currentItem ()->text (), link, maps);	//set the filename of the webpage and pass in the maps
  w.show ();			//show the webpage
}
