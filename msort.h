#ifndef MERGE_H
#define MERGE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>

//using namespace std;

struct AlphaStrComp
{
  bool operator () (const WebPage  lhs, const WebPage  rhs)
  {				// Uses string's built in operator< 
    // to do lexicographic (alphabetical) comparison
    return lhs.filename () < rhs.filename ();
  }
};

struct RankComp
{
  bool operator () (const WebPage  lhs, const WebPage  rhs)
  {
	//compare the ranks of two webpages
    return lhs.getRank () > rhs.getRank ();
  }
};



template < class T, class Comparator > class Merge
{

public:

  void mergeSort (vector < T > &mylist, Comparator c)
  {

    msort (mylist, 0, mylist.size () - 1, c);

  }

private:


  void merge (vector < T > &input, int l, int r, int m, Comparator c)
  {
    vector < T > temp;
    int i = l;
    int j = m + 1;
    int mid = floor ((l + r) / 2);
    while (i <= mid && j <= r)
      {

	if (c (*input[i], *input[j])){
	  temp.push_back (input[i]);
		i++;
	}
	else{
	  temp.push_back (input[j]);
		j++;
}
      }

    while (j <= r){
      temp.push_back (input[j]);
j++;
}
	while (i <= mid){
      temp.push_back (input[i]);
i++;
}

    int k = 0;

    for (int index = l; index <= r; index++)
      {
	input[index] = temp[k++];
      }

  }

  void msort (vector < T > &mylist, int start, int end, Comparator c)
  {
    // base case 
    if (start >= end)
      return;
    // recursive 
    int mid = (start + end) / 2;
    msort (mylist, start, mid, c);
    msort (mylist, mid + 1, end, c);
    // merge 
    merge (mylist, start, end, mid, c);

  }

};

#endif
