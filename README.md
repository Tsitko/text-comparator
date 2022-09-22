# Text comparator

An easy-to-use tool to compare text variables for data mining in problems such as search results relevance problem.  

## Using example:  
```cpp
#include "TextComparator.cpp"

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
```
Output:  
```
relevance = 0.231049 ngram_relevance = 1.27389 jordan_measure = 0.2 ngram_jordan_measure = 0.153846
relevance = 0.231049 ngram_relevance = 1.33009 jordan_measure = 0.2 ngram_jordan_measure = 0.380952
relevance = 0 ngram_relevance = 0 jordan_measure = 0 ngram_jordan_measure = 0
```
## Explanation:  
First we are creating a _TextComparator_ object. It is initializing with stop words (words which will be deleted from the texts we will try to compare).  
After it was created we need to _Feed_ it with texts (for example with all the descriptions of the products we have in our shop).  
And after that TextComparator we can _Compare_ texts and get _ComparisonStatisctics_ structure as a result.  

The ComparisonStatisctics structure is the following:  
```cpp
struct ComparisonStatisctics{
	ComparisonStatisctics(){}
	
	ComparisonStatisctics(double input_relevance,
		double input_ngram_relevance,
		double input_jordan_measure,
		double input_ngram_jordan_measure){
			relevance = input_relevance;
			ngram_relevance = input_ngram_relevance;
			jordan_measure = input_jordan_measure;
			ngram_jordan_measure = input_ngram_jordan_measure;
	}
	
	double relevance = 0.0;
	double ngram_relevance = 0.0;
	double jordan_measure = 0.0;
	double ngram_jordan_measure = 0.0;
};
```

### relevance  
A simpl TF*IDF for all the matched words in texts which we are trying to compare.  
### ngram_relevance  
The same as relevance, but for ngrams (ngrams for word "computer" are [com, put, er, omp, ute, r, mpu, ter, put, er, ute, r, ter]).  
As we can see relevances for texts  
 ```
("notebook with 512GB ssd", "computer with 1tb ssd")  
```
and
```  
("cmputer with 512GB ssd", "computer with 1tb ssd")  
```
are the same. But ngram_relevances are different. Thats because we made a typo in word "computer" and simple relevance can do nothing with that. But ngram_relevance can deal with such typos.
### jordan_measure  
A jordan_measure is the size of intersection of two sets devided by the size of its union.  
### ngram_jordan_measure  
The same as jordan_measure but for ngrams.