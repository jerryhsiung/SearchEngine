#include <QApplication>
#include "mainwin.h"
#include "msort.h"
#include <iostream>

using namespace std;

int
main (int argc, char *argv[])
{
  map < string, WebPage * >link;	//maps the filenames to webpage pointersAlphaStrComp aa;



  fstream file;			// the main data file that contains the webpages
  map < string, Set < WebPage * > >maps;	// words mapped to webpage pointers that contain those words

  file.open (argv[1]);

  //terminate the program if can't open text file
  if (file.fail ())
    {
      cerr << "Couldn't open text file" << endl;
      return 0;
    }

  string filename;		// filename of the webpage

  WebPage *web;			//temporary webpae pointer
  Set < string > words;
  while (file >> filename)
    {
      web = new WebPage (filename, link);	// create new webpage with the filename

      Set < WebPage * >temp;
      temp.insert (web);
      words = web->allWords ();	//stores the words in the page in a temporary set

      //for all the words in the list, determine the webpage pointers
      for (Set < string >::iterator it = words.begin (); it != words.end ();
	   ++it)
	{

	  //if word is already in the map, add the pointer
	  if (maps.find (*it) != maps.end ())
	    {

	      maps.find (*it)->second.insert (web);
	    }
	  else
	    {

	      //add the word and pointer to the map
	      maps[*it] = temp;

	    }

	}
    }


  file.close ();

  file.open (argv[1]);		//open the file again to parse

  while (file >> filename)
    {

      //add the incoming links
      link.at (filename)->parse (link);

    }

  file.close ();

  QApplication app (argc, argv);
  MainWin form;
  form.setMaps (link, maps);	//pass the maps to the qt app
  form.show ();
  return app.exec ();
  return 0;
}
