#include <cmath>
#include <numeric>
#include <algorithm>
#include <map>

#include "Line.h"
#include "StopWords.h"


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
	explicit TextComparator(const std::string& stop_words_string);
	template <typename StringContainer>
	explicit TextComparator(const StringContainer& stop_words);	
	
	void Feed(const std::string& text);
	
	ComparisonStatisctics Compare(const std::string& text1, const std::string& text2);

private:
	std::map<std::string, std::set<int>> words_to_texts_;
	std::map<std::string, std::set<int>> ngrams_to_texts_;
	StopWords stop_words_;
	std::set<char> valid_symbols_;
	int texts_id_ = 0;
		
	void SetValidSymbols();
	
	std::vector<std::string> SplitnIntoWordsNoStop(Line& text);
	
	std::vector<std::string> SplitnIntoNgramsNoStop(Line& text) const;
	
	double ComputeWordIdf (const std::string& word) const;
	
	double ComputeNgramIdf (const std::string& ngram) const;
	
	double ComputeWordFreq(const std::string& word,  
			const std::vector<std::string>& words) const;
	
	double ComputeNgramFreq(const std::string& ngram, 
			const std::vector<std::string>& ngrams) const;
	
	double ComputeRelevance(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const;
	
	double ComputeNgramRelevance(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const;
	
	double ComputeJordanMeasure(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const;
	
	double ComputeNgramJordanMeasure(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const;
};

template <typename StringContainer>
TextComparator::TextComparator(const StringContainer& stop_words){
		SetValidSymbols();
		std::string stop_words_string = "";
		for(const std::string& word: stop_words){
			stop_words_string += word;
		}
		stop_words_ = StopWords(stop_words_string);
	}	