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

class Line{
public:
	explicit Line(std::string& text): line_(text){}
	
	std::vector<std::string> SplitWordsToVec() {
		std::string clean_text = ClearText(line_);
		std::string temp="";
		if(words_vec_.empty()){
			for(const char c: clean_text){
				if(c==' ' && !temp.empty()){
					words_vec_.push_back(temp);
					temp="";
				} else{
					temp+=c;
				}
			}
			if(!temp.empty()){
				words_vec_.push_back(temp);
			}
		}
		return words_vec_;
		
	std::set<std::string> SplitWordsToSet() {
		if(words_set_.empty()){
			for(const std::string& word: SplitWordsToVec(line_)){
			words_set_.insert(word);
			}
		}
		
		return words_set_;
	}
	
private:
	std::string line_;
	std::vector<std::string> words_vec_;
	std::set<std::string> words_set_;
	
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
	
	
	std::string ClearText(const std::string& text, std::string replacement = " "s) const{
		std::string clean_text=""s;
		for(const char c: text){
			if(IsValidSymbol(c)){
				clean_text += c;
			} else{
				clean_text += replacement;
			}
		}
		return clean_text;
	}
};

class StopWords{
public:
	explicit StopWords(Line& line): stop_words_line_(line){
		stop_words_set_ = stop_words_line_.SplitWordsToSet()
	}
	
	std::vector<std::string> SplitnIntoWordsNoStop(const Line& text) const{
		std::vector<std::string> result;
		for(const std::string& word: text.SplitWordsToVec()){
			if(!stop_words_set_.count(word)){
				result.push_back(word);
			}
		}
		return result;
	}
	
private:
	Line stop_words_line_;
	std::set<std::string> stop_words_set_;
};

class TextComparator{
	
public:
	explicit TextComparator(const std::string& stop_words_string)
	: stop_words_(StopWords(stop_words_string))
	{
	}
	
	template <typename StringContainer>
	explicit TextComparator(const StringContainer& stop_words){
		std::string stop_words_string = ""s;
		for(const std::string& word: stop_words){
			stop_words_string += word;
		}
		stop_words_ = StopWords(stop_words_string);
	}		
	
	void Feed(const std::string& text){
		++texts_id_;
		std::vector<std::string> words = SplitnIntoWordsNoStop(Line(text));
		for(const std::string& word: words){
			words_to_texts_[word] = texts_id_;
		}
		std::vector<std::string> ngrams = SplitnIntoNgramsNoStop(Line(text));
		for(const std::string& ngram: ngrams){
			ngrams_to_texts_[ngram] = texts_id_;
		}
	}
	
	ComparisonStatisctics Compare(const std::string& text1, const std::string& text1){
		ComparisonStatisctics result;
		result.relevance = ComputeRelevance(SplitnIntoWordsNoStop(Line(text1)), SplitnIntoWordsNoStop(Line(text2)));
		result.ngram_relevance = ComputeNgramRelevance(SplitnIntoNgramsNoStop(Line(text1)), SplitnIntoNgramsNoStop(Line(text2)));
		result.jordan_measure = ComputeJordanMeasure(SplitnIntoWordsNoStop(Line(text1)), SplitnIntoWordsNoStop(Line(text2)));
		result.ngram_jordan_measure = ComputeNgramJordanMeasure(SplitnIntoNgramsNoStop(Line(text1)), SplitnIntoNgramsNoStop(Line(text2)));
		return result;
	}

private:
	std::map<std::string, int> words_to_texts_;
	std::map<std::string, int> ngrams_to_texts_;
	StopWords stop_words_;
	std::set<int> valid_symbols_;
	int texts_id_=0;
		
	std::vector<std::string> SplitnIntoWordsNoStop(const Line& text) const{
		return stop_words_.SplitnIntoWordsNoStop(text);
	}
	
	
	
	std::vector<std::string> SplitnIntoNgramsNoStop(const Line& text) const{
		std::vector<std::string> result;
		std::string ngram_string = ""s;
		for(const std::string& word: stop_words_.SplitnIntoWordsNoStop(text)){
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
			Idf = log(1.0*texts_id_/words_to_texts_.at(word).size());
		} else{
			Idf = log(texts_id_);
		}
		return Idf;
	}
	
	double ComputeNgramIdf (const std::string& ngram) const{
		double Idf = 0.0;
		if(ngrams_to_texts_.count(ngram)){
			Idf = log(1.0*texts_id_/ngrams_to_texts_.at(ngram).size());
		} else{
			Idf = log(texts_id_);
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
			const std::vector<std::string>& ngrams2) const{
				
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
	
	double ComputeNgramRelevance(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const{
		double relevance = 0.0;
		for(const std::string& ngram: ngrams1){
			if(count(ngrams2.begin(), ngrams2.end(), word) > 0){
				relevance += ComputeNgramFreq(ngram, ngrams1, ngrams2)*ComputeNgramIdf(ngram);
			}
		}			
		return relevance;
	}
	
	double ComputeJordanMeasure(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const{
		//TODO: method realization
	}
	
	double ComputeNgramJordanMeasure(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const{
		//TODO: method realization
	}
};