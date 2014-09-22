#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include "lib/set.h"
#include "lib/web.h"

using namespace std;

int main (int argc, char *argv[])
{


  fstream file;
  ofstream output;
  output.open (argv[2]);
  file.open (argv[1]);

  if (file.fail ())
    {
      cerr << "Couldn't open text file" << endl;
      return 0;
    }

  WebPage *web;			//temporary webpage pointer
  string filename;
  map < string, WebPage * >link;	//maps the filenames to webpage pointers

  file >> filename;
  web = new WebPage (filename, link);	// create new webpage with the filename
  stack < WebPage * >st;
  st.push (web);

  Set < WebPage * >gray;
  WebPage *out;
  WebPage *web2;
  while (!st.empty ())
    {

      WebPage *u = st.top ();
      st.pop ();

      if (gray.find (u) == gray.end ())
	{
	  output << u->filename () << endl;

	  gray.insert (u);
	  vector < string > temp = u->allOutgoingLinks ();

	  if (!temp.empty ())
	    {

	      for (int i = temp.size () - 1; i >= 0; i--)
		{

		  if (link.find (temp[i]) == link.end ())
		    {
		      out = new WebPage (temp[i], link);
		      st.push (out);
		    }

		  else
		    st.push (link[temp[i]]);
		}
	    }

	  if (st.empty () && !file.eof ())
	    {

	      file >> filename;
	      while (link.find (filename) != link.end () && !file.eof ())
		{
		  file >> filename;

		}

	      if (link.find (filename) == link.end ())
		{

		  web2 = new WebPage (filename, link);	// create new webpage with the filename
		  st.push (web2);
		}


	    }


	}
    }
  file.close ();


  return 0;

}
