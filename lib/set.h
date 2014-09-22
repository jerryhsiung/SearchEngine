#ifndef SET_H
#define SET_H

#include <iostream>
#include <set>

using namespace std;

template < typename T > class Set:public set < T >
{
public:
  Set < T > setIntersection (const Set < T > &other) const;
  /* Returns the intersection of the current set with other.
     That is, returns the set of all items that are both in this
     and in other. */

  Set < T > setUnion (const Set < T > &other) const;
  /* Returns the union of the current set with other.
     That is, returns the set of all items that are in this set
     or in other (or both).
     The resulting set should not contain duplicates. */

};

template < typename T >
  Set < T > Set < T >::setIntersection (const Set < T > &other) const
{

  Set < T > intersection;

  //iterate through all items in the first set
  for (typename set < T >::iterator it = this->begin (); it != this->end ();
       ++it)
    {

      //if the other set contains item too, add item to intersection set
      if (other.find (*it) != other.end ())
	intersection.insert (*it);

    }

  return intersection;

}

template < typename T > Set < T > Set < T >::setUnion (const Set < T > &other) const
{
  Set < T > newset;

  //add all items in set 1 to the union set
  for (typename set < T >::iterator it = this->begin (); it != this->end ();
       ++it)
    {

      newset.insert (*it);

    }

  //add all items in set 2 to the union set
  for (typename set < T >::iterator it = other.begin (); it != other.end ();
       ++it)
    {

      newset.insert (*it);

    }

  //return union set
  return newset;
}


#endif
