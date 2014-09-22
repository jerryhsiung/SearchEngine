#include "lib/web.h"

WebPage::WebPage (string f, map < string, WebPage * >&m)
{
  rank = 0;
  fstream file;
  file.open (f.c_str ());

  //if file can't be opened
  if (file.fail ())
    {

      fname = "";
      throw invalid_argument ("Text file can't be opened!");	//throw exception if file can't be opened
    }
  fname = f;			//store filename
  bool getName = false;
  string content, text, link;
  while (!file.eof ())
    {

      if (isspace (file.peek ()))
	file.ignore (1);	//get rid of whitespace

      else if (file.peek () == '[')
	{			//if there's a link
	  file.ignore (1, '[');
	  getline (file, text, ']');	//get the anchor link until ]
	  words.push_back (text);	//put the anchor text to the word list
	  getName = true;	//start looking for filename
	}
      else if (getName)
	{

	  if (file.peek () == '(')
	    file.ignore (1, '(');
	  else
	    {
	      getline (file, link, ')');	//get the link(filename)      
	      bool contains = false;
	      for (unsigned int i = 0; i < out.size(); i++)
		{

		  if (out[i] == link)
		    {

		      contains = true;
		      break;
		    }

		}
	      if (!contains)
		out.push_back (link);	//put the link to the outgoing link list
	      getName = false;	//stop looking for filename
	    }
	}
      else
	{

	  file >> content;	//get the word if it's not a link
	  words.push_back (content);	//put the word to the list

	}
    }


  m[fname] = this;		//map the filename to the webpage pointer

}

WebPage::~WebPage ()
{
//no pointers to delete
}

Set < string > WebPage::allWords () const
{

  Set < string > wordlist;

  unsigned int
    index = 0;
  bool
    space = false;		//consider special characters as space

  int
    size = words.size ();
  string
    word;
  for (int a = 0; a < size; a++)
    {
      for (unsigned int i = 0; i < words.at (a).length (); i++)
	{
	  char
	    c = words.at (a).at (i);

	  if (c < 48 || (c > 57 && c < 65) || (c > 90 && c < 97) || c > 122)	//if there is a special character
	    {

	      if (!space)
		{

		  word = words.at (a).substr (index, i - index);	// get the substring of the whole string
		  if (wordlist.find (word) == wordlist.end ())	// if word doesn't exist in the list
		    wordlist.insert (lower (word));	//convert it to lowercase
		  space = true;	//start looking for another word
		}
	    }
	  else			//if it isn't a special character
	    {
	      if (space)
		{

		  space = false;
		  index = i;
		}

	      else if (i == words.at (a).length () - 1)
		{

		  word = words.at (a).substr (index, i - index + 1);
		  if (wordlist.find (word) == wordlist.end ())	//if it's not in the set
		    wordlist.insert (lower (word));	//add it to the set

		}
	    }
	}

      space = false;
      index = 0;
    }

  return wordlist;
}

ostream & operator<< (ostream & os, const WebPage & page)
{

  for (unsigned int i = 0; i < page.words.size (); i++)
    os << page.words.at (i);	//print the content without brackets and links
  //also prints out the anchor text

  return os;
}

string
WebPage::lower (string s) const
{

  string newstr = "";
  for (unsigned int i = 0; i < s.length (); i++)
    {

      char c = s.at (i);
      if (c > 64 && c < 91)	//if it's uppercase
	newstr += (c + 32);	//convert it to lowercase
      else
	newstr += c;		//keep it the same if it's not uppercase

    }

  return newstr;
}

bool
WebPage::operator== (const WebPage & web)
{

  return fname == web.filename ();	//check if they have the same filename

}



vector < string > WebPage::allOutgoingLinks () const
{

  return out;			//return outgoing link set

}


vector < string > WebPage::allIncomingLinks () const
{

  return in;			//return incoming link set

}


string
WebPage::filename () const
{

  return fname;			//return filename

}

void
WebPage::parse (map < string, WebPage * >&list)
{


  //iterate through outgoing link set
  for (vector < string >::iterator it = out.begin (); it != out.end (); ++it)
    {

      //add incoming link to the webpages on the outgoing list
      list.at (*it)->addIncomingLink (list.at (fname));

    }

}

void
WebPage::addIncomingLink (WebPage * start)
{

  in.push_back (start->filename ());	//insert filename of webpage to the incoming list

}

vector < string > WebPage::getWords ()
{

  return words;
}

double
WebPage::getRank () const
{

  return rank;

}


void
WebPage::setRank (double r)
{

  rank = r;

}



WebPage
WebPage::operator< (const WebPage & w)
{

  if (filename () < w.filename ())	//compare webpages by their filenames
    return *this;
  else
    return w;


}
