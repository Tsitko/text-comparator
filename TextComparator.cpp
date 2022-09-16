#ifndef NGRAM_SIZE
#define NGRAM_SIZE 3
#endif

struct ComparisonStatisctics{
	//TODO: constructor
	
	double relevance = 0.0;
	double ngram_relevance = 0.0;
	double jordan_measure = 0.0;
	double ngram_jordan_measure = 0.0;
}

class TextComparator{
	
public:
	// TODO: constructor
	
	void Feed(const string& text){
		//TODO: method realization
	}
	
	ComparisonStatisctics Compare(const string& text1, const string& text1){
		//TODO: method realization 
	}

private:
	map<string, int> words_to_texts_;
	map<string, int> ngrams_to_texts_;
	set<string> stop_words_;
	int texts_count_;
	
	bool IsValidSymbol(const char c) const{
		//TODO: method realization
	}
	
	
	string CleanText(const string& text) const{
		//TODO: method realization
	}
	
	vector<string> SplitnIntoWordsNoStop(const string& text) const{
		//TODO: method realization
	}
	
	vector<string> SplitnIntoNgramsNoStop(const string& text) const{
		//TODO: method realization
	}
	
	double ComputeRelevance(const vector<string>& text1, const vector<string> text2) const{
		//TODO: method realization
	}
	
	double ComputeNgramRelevance(const vector<string>& text1, const vector<string> text2) const{
		//TODO: method realization
	}
	
	double ComputeJordanMeasure(const vector<string>& text1, const vector<string> text2) const{
		//TODO: method realization
	}
	
	double ComputeNgramJordanMeasure(const vector<string>& text1, const vector<string> text2) const{
		//TODO: method realization
	}
	
	double ComputeWordFreq(const string& word, const vector<string>& text) const{
		//TODO: method realization
	}
	
	double ComputeNgramFreq(const string& ngram, const vector<string>& text) const{
		//TODO: method realization
	}
};