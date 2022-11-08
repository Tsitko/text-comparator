#include "TextComparator.h"
#include <iostream>

void print(ComparisonStatisctics s){ // printing ComparisonStatisctics
	std::cout<<"relevance = "<<s.relevance<< " "
	<<"ngram_relevance = "<<s.ngram_relevance<<" "
	<<"jordan_measure = "<<s.jordan_measure<<" "
	<<"ngram_jordan_measure = "<<s.ngram_jordan_measure<<std::endl;
}


int main(){
	std::string s= "in the as a is am are with";
	TextComparator t_c = TextComparator(s);
	t_c.Feed("computer with 1tb ssd");
	t_c.Feed("a bottle of whisky");
	ComparisonStatisctics s1 = t_c.Compare("notebook with 512GB ssd", "computer with 1tb ssd");
	ComparisonStatisctics s2 = t_c.Compare("cmputer with 512GB ssd", "computer with 1tb ssd");
	ComparisonStatisctics s3 = t_c.Compare("notebook with 512GB ssd", "a bottle of whisky");
	print(s1);
	print(s2);
	print(s3);
}