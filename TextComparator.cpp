#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <numeric>

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
	explicit TextComparator(const std::string& stop_words_string)
	: stop_words_(SplitStopWords(stop_words_string))
	{
	}
	
	template <typename StringContainer>
	explicit TextComparator(const StringContainer& stop_words){
		std::string stop_words_string = ""s;
		for(const std::string& word: stop_words){
			stop_words_string += word;
		}
		stop_words_ = SplitStopWords(stop_words_string);
	}		
	
	void Feed(const std::string& text){
		//TODO: method realization
	}
	
	ComparisonStatisctics Compare(const std::string& text1, const std::string& text1){
		//TODO: method realization 
	}

private:
	std::map<std::string, int> words_to_texts_;
	std::map<std::string, int> ngrams_to_texts_;
	std::set<std::string> stop_words_;
	std::set<int> valid_symbols_;
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
	
	
	std::string ClearText(const std::string& text) const{
		std::string clean_text=""s;
		for(const char c: text){
			if(IsValidSymbol(c)){
				clean_text+=c;
			}
		}
		return clean_text;
	}
	
	std::vector<std::string> SplitWords(const std::string& text) const{
		std::string clean_text = ClearText(text);
		std::string temp="";
		std::vector<std::string> result;
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
		
	std::set<std::string> SplitStopWords(const std::string& text) const{
		std::set<std::string> result;
		for(const std::string& word: SplitWords(text)){
			result.insert(word);
		}
		return result;
	}
	
	std::vector<std::string> SplitnIntoWordsNoStop(const std::string& text) const{
		std::vector<std::string> result;
		for(const std::string& word: SplitWords(text)){
			if(!stop_words_.count(word)){
				result.push_back(word);
			}
		}
		return result;
	}
	
	std::vector<std::string> SplitnIntoNgramsNoStop(const std::string& text) const{
		std::vector<std::string> result;
		std::string ngram_string = ""s;
		for(const std::string& word: SplitnIntoWordsNoStop(text)){
			ngram_string += word;
		}
		std::string temp = ""s;
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
	
	double ComputeWordIdf (const std::string& word) const{
		double Idf = 0.0;
		if(words_to_texts_.count(word)){
			Idf = log(1.0*texts_count_/words_to_texts_.at(word).size());
		} else{
			Idf = log(1.0*texts_count_);
		}
		return Idf;
	}
	
	double ComputeNgramIdf (const std::string& ngram) const{
		double Idf = 0.0;
		if(ngrams_to_texts_.count(ngram)){
			Idf = log(1.0*texts_count_/ngrams_to_texts_.at(ngram).size());
		} else{
			Idf = log(texts_count_);
		}
		return Idf;
	}
	
	double ComputeWordFreq(const std::string& word, 
			const std::vector<std::string>& words1, 
			const std::vector<std::string>& words2) const{
				
		return (count(words1.begin(), words1.end(), word) + count(words2.begin(), words2.end(), word))/(words1.size() + words2.size());
	}
	
	double ComputeNgramFreq(const std::string& ngram, 
			const std::vector<std::string>& ngrams1, 
			const std::vector<std::string>& ngrams1) const{
				
		return (count(ngrams1.begin(), ngrams1.end(), ngram) + count(ngrams2.begin(), ngrams2.end(), ngram))/(ngrams1.size() + ngrams2.size());
	}
	
	double ComputeRelevance(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const{
		double relevance = 0.0;
		for(const std::string& word: words1){
			if(count(words2.begin(), words2.end(), word) > 0){
				relevance += ComputeWordFreq(word, words1, words2)*ComputeWordIdf(word);
			}
		}			
		return relevance;
	}
	
	double ComputeNgramRelevance(const std::string& text1, const std::string text2) const{
		//TODO: method realization
	}
	
	double ComputeWordJordanMeasure(const std::string& text1, const std::string text2) const{
		//TODO: method realization
	}
	
	double ComputeNgramJordanMeasure(const std::string& text1, const std::string text2) const{
		//TODO: method realization
	}
};