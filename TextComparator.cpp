#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <numeric>
#include <iostream>
#include <algorithm>

#ifndef LINE
#include "Line.cpp"
#endif
#ifndef STOPWORDS
#include "StopWords.cpp"
#endif


#ifndef NGRAM_SIZE
#define NGRAM_SIZE 3
#endif


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





class TextComparator{
public:
	explicit TextComparator(const std::string& stop_words_string){
		SetValidSymbols();
		Line stop_words_line = Line(stop_words_string, valid_symbols_);
		stop_words_= StopWords(stop_words_line);
	}
	
	template <typename StringContainer>
	explicit TextComparator(const StringContainer& stop_words){
		SetValidSymbols();
		std::string stop_words_string = "";
		for(const std::string& word: stop_words){
			stop_words_string += word;
		}
		stop_words_ = StopWords(stop_words_string);
	}		
	
	void Feed(const std::string& text){
		++texts_id_;
		Line text_line= Line(text, valid_symbols_);
		std::vector<std::string> words = SplitnIntoWordsNoStop(text_line);
		for(const std::string& word: words){
			words_to_texts_[word].insert(texts_id_);
		}
		std::vector<std::string> ngrams = SplitnIntoNgramsNoStop(text_line);
		for(const std::string& ngram: ngrams){
			ngrams_to_texts_[ngram].insert(texts_id_);
		}
	}
	
	ComparisonStatisctics Compare(const std::string& text1, const std::string& text2){
		ComparisonStatisctics result;
		Line text_line1= Line(text1, valid_symbols_);
		Line text_line2= Line(text2, valid_symbols_);
		result.relevance = ComputeRelevance(SplitnIntoWordsNoStop(text_line1), SplitnIntoWordsNoStop(text_line2));
		result.ngram_relevance = ComputeNgramRelevance(SplitnIntoNgramsNoStop(text_line1), SplitnIntoNgramsNoStop(text_line2));
		result.jordan_measure = ComputeJordanMeasure(SplitnIntoWordsNoStop(text_line1), SplitnIntoWordsNoStop(text_line2));
		result.ngram_jordan_measure = ComputeNgramJordanMeasure(SplitnIntoNgramsNoStop(text_line1), SplitnIntoNgramsNoStop(text_line2));
		return result;
	}

private:
	std::map<std::string, std::set<int>> words_to_texts_;
	std::map<std::string, std::set<int>> ngrams_to_texts_;
	StopWords stop_words_;
	std::set<char> valid_symbols_;
	int texts_id_=0;
		
	void SetValidSymbols(){
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
	
	std::vector<std::string> SplitnIntoWordsNoStop(Line& text) {
		return stop_words_.SplitnIntoWordsNoStop(text);
	}
	
	std::vector<std::string> SplitnIntoNgramsNoStop(Line& text) const{
		std::vector<std::string> result;
		std::string ngram_string = "";
		for(const std::string& word: stop_words_.SplitnIntoWordsNoStop(text)){
			ngram_string += word;
		}
		std::string temp = "";
		for(int i=0; i<ngram_string.size()- NGRAM_SIZE; ++i){
			for(const char c: ngram_string.substr(i, ngram_string.size()-i)){
				if(temp.size() == NGRAM_SIZE){
					result.push_back(temp);
					temp = "";
					if(c!=' '){
						temp += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					}
				} else{
					if(c!=' '){
						temp += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					}
				}
			}
			if(!temp.empty()){
				result.push_back(temp);
			}
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
				
		return 1.0*(std::count(words1.begin(), words1.end(), word) + count(words2.begin(), words2.end(), word))/(words1.size() + words2.size());
	}
	
	double ComputeNgramFreq(const std::string& ngram, 
			const std::vector<std::string>& ngrams1, 
			const std::vector<std::string>& ngrams2) const{
				
		return 1.0*(std::count(ngrams1.begin(), ngrams1.end(), ngram) + std::count(ngrams2.begin(), ngrams2.end(), ngram))/(ngrams1.size() + ngrams2.size());
	}
	
	double ComputeRelevance(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const{
		double relevance = 0.0;
		for(const std::string& word: words1){
			if(std::count(words2.begin(), words2.end(), word) > 0){
				relevance += ComputeWordFreq(word, words1, words2)*ComputeWordIdf(word);
			}
		}			
		return relevance;
	}
	
	double ComputeNgramRelevance(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const{
		double relevance = 0.0;
		for(const std::string& ngram: ngrams1){
			if(count(ngrams2.begin(), ngrams2.end(), ngram) > 0){
				relevance += ComputeNgramFreq(ngram, ngrams1, ngrams2)*ComputeNgramIdf(ngram);
			}
		}			
		return relevance;
	}
	
	double ComputeJordanMeasure(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const{
		double jordan_measure=0.0;
		return jordan_measure;
	}
	
	double ComputeNgramJordanMeasure(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const{
		double ngram_jordan_measure = 0.0;
		return ngram_jordan_measure;
	}
};

