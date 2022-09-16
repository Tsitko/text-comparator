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
	explicit TextComparator(const string& stop_words_string)
	: stop_words_(SplitStopWords(stop_words_string))
	{
	}
	
	template <typename StringContainer>
	explicit TextComparator(const StringContainer& stop_words){
		string stop_words_string = ""s;
		for(const string& word: stop_words){
			stop_words_string += word;
		}
		stop_words_ = SplitStopWords(stop_words_string);
	}		
	
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
	set<int> valid_symbols_;
	int texts_count_;
	
	bool IsValidSymbol(const char c) const{
		if(valid_symbols_.empty()){ // initialization
			valid_symbols_.insert(static_cast<char>(32)); // space
			for(int i=48; i<=57; ++i){
				valid_symbols_.insert(static_cast<char>(i)); // 0-9
			}
			for(int i=64; i<=90; ++i){
				valid_symbols_.insert(static_cast<char>(i)); // @,A-Z
			}
			for(int i=97; i<=122; ++i){
				valid_symbols_.insert(static_cast<char>(i)); // a-z
			}
		}
		return valid_symbols_.count(c);
	}
	
	
	string ClearText(const string& text) const{
		string clean_text=""s;
		for(const char c: text){
			if(IsValidSymbol(c)){
				clean_text+=c;
			}
		}
		return clean_text;
	}
	
	vector<string> SplitWords(const string& text) const{
		string clean_text = ClearText(text);
		string temp="";
		vector<string> result;
		for(const char c: clean_text){
			if(c==' ' && !temp.empty()){
				result.push_back(temp);
				temp="";
			} else{
				temp+=c;
			}
		}
		if(!temp.empty()){
			result.push_back(temp);
		}
		return result;
		
	set<string> SplitStopWords(const string& text) const{
		set<string> result;
		for(const string& word: SplitWords(text)){
			result.insert(word);
		}
		return result;
	}
	
	vector<string> SplitnIntoWordsNoStop(const string& text) const{
		vector<string> result;
		for(const string& word: SplitWords(text)){
			if(!stop_words_.count(word)){
				result.push_back(word);
			}
		}
		return result;
	}
	
	vector<string> SplitnIntoNgramsNoStop(const string& text) const{
		vector<string> result;
		string ngram_string = ""s;
		for(const string& word: SplitnIntoWordsNoStop(text)){
			ngram_string += word;
		}
		string temp = ""s;
		for(const char c: ngram_string){
			if(temp.size() == NGRAM_SIZE){
				result.push_back(temp);
				temp = ""s;
			} else{
				if(c!=' '){
					temp += c;
				}
			}
		}
		if(!temp.empty()){
			result.push_back(temp);
		}
		return result;
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