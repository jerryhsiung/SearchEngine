#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <iostream>
#include <vector>
#include "set.h"
#include <map>
#include <fstream>
#include <stdexcept>

using namespace std;


class WebPage
{
public:

  WebPage ()
  {
  }
  WebPage (string f, map < string, WebPage * >&m);
  /* constructor that initializes the page from the given file.
     Should throw an exception if the file does not exist
     or has other problems. */
  ~WebPage ();			// destructor

  Set < string > allWords ()const;
  /* Returns a set containing all individual words on the web page. */

  friend ostream & operator<< (ostream & os, const WebPage & page);
  /* Declares an operator we can use to print the web page. */

  bool operator== (const WebPage & web);
  //checks if the webpages are the same

  string lower (string s) const;
  //returns the lowercase version of the string

  vector < string > allOutgoingLinks ()const;
  /* Returns "pointers" to all webpages that this page has links to.
     As discussed above, this could be as a set or via an iterator,
     and it could be as actual pointers, or as strings,
     or possibly other. */

  vector < string > allIncomingLinks ()const;
  /* Returns "pointers" to all webpages that link to this page.
     Same consideration as previous function. */

  string filename () const;
  /* Returns the filename from which this page was read. */

  /* Based on which overall architecture you choose, you may
     also need/want some or all of the following functions. */

  void parse (map < string, WebPage * >&list);
  /* actually parses the content of the filename that was passed
     to the constructor into the object. */

  void addIncomingLink (WebPage * start);
  /* Adds a link from start to the current page
     in the current page's storage. */

  WebPage operator< (const WebPage &);
  //comparator function

  vector < string > getWords ();
  //return the word list of the webpage

  double getRank () const;

  void setRank (double r);


private:
  // you get to decide what goes here.
  vector < string > words;
  string fname;
  vector < string > in, out;
  double rank;

};


#endif
