#include "TextComparator.h"

TextComparator::TextComparator(const std::string& stop_words_string){
		SetValidSymbols();
		stop_words_ = std::move(StopWords(stop_words_string, valid_symbols_));
	}

void TextComparator::Feed(const std::string& text){
		++texts_id_;
		Line text_line = std::move(Line(text, valid_symbols_));
		std::vector<std::string> words = std::move(SplitnIntoWordsNoStop(text_line));
		for(const std::string& word: words){
			words_to_texts_[word].insert(texts_id_);
		}
		std::vector<std::string> ngrams = std::move(SplitnIntoNgramsNoStop(text_line));
		for(const std::string& ngram: ngrams){
			ngrams_to_texts_[ngram].insert(texts_id_);
		}
	}

ComparisonStatisctics TextComparator::Compare(const std::string& text1, const std::string& text2){
		ComparisonStatisctics result;
		Line text_line1 = std::move(Line(text1, valid_symbols_));
		Line text_line2 = std::move(Line(text2, valid_symbols_));
		result.relevance = ComputeRelevance(SplitnIntoWordsNoStop(text_line1), SplitnIntoWordsNoStop(text_line2));
		result.ngram_relevance = ComputeNgramRelevance(SplitnIntoNgramsNoStop(text_line1), SplitnIntoNgramsNoStop(text_line2));
		result.jordan_measure = ComputeJordanMeasure(SplitnIntoWordsNoStop(text_line1), SplitnIntoWordsNoStop(text_line2));
		result.ngram_jordan_measure = ComputeNgramJordanMeasure(SplitnIntoNgramsNoStop(text_line1), SplitnIntoNgramsNoStop(text_line2));
		return result;
	}

void TextComparator::SetValidSymbols(){
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

std::vector<std::string> TextComparator::SplitnIntoWordsNoStop(Line& text) {
		return stop_words_.SplitnIntoWordsNoStop(text);
	}

std::vector<std::string> TextComparator::SplitnIntoNgramsNoStop(Line& text) const{
		std::vector<std::string> result;
		std::string ngram_string = "";
		for(const std::string& word: stop_words_.SplitnIntoWordsNoStop(text)){
			ngram_string += word;
		}
		for(int i=0; i<ngram_string.size()- NGRAM_SIZE; ++i){
			std::string temp = "";
			for(const char c: ngram_string.substr(i, ngram_string.size()-i)){
				if(temp.size() == NGRAM_SIZE){
					result.push_back(temp);
					temp = c;
				} else{
					temp += c;
				}
			}
			if(!temp.empty()){
				result.push_back(temp);
			}
		}
		return result;
	}

double TextComparator::ComputeWordIdf (const std::string& word) const{
		double Idf = 0.0;
		if(words_to_texts_.count(word)){
			Idf = log(1.0*texts_id_/words_to_texts_.at(word).size());
		} else{
			Idf = log(texts_id_);
		}
		return Idf;
	}

double TextComparator::ComputeNgramIdf (const std::string& ngram) const{
		double Idf = 0.0;
		if(ngrams_to_texts_.count(ngram)){
			Idf = log(1.0*texts_id_/ngrams_to_texts_.at(ngram).size());
		} else{
			Idf = log(texts_id_);
		}
		return Idf;
	}

double TextComparator::ComputeWordFreq(const std::string& word,  
			const std::vector<std::string>& words) const{
				
		return 1.0*(std::count(words.begin(), words.end(), word))/words.size();
	}

double TextComparator::ComputeNgramFreq(const std::string& ngram, 
			const std::vector<std::string>& ngrams) const{
				
		return 1.0*(std::count(ngrams.begin(), ngrams.end(), ngram))/ngrams.size();
	}

double TextComparator::ComputeRelevance(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const{
		double relevance = 0.0;
		for(const std::string& word: words1){
			if(std::count(words2.begin(), words2.end(), word) > 0){
				relevance += ComputeWordFreq(word, words2)*ComputeWordIdf(word);
			}
		}			
		return relevance;
	}

double TextComparator::ComputeNgramRelevance(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const{
		double relevance = 0.0;
		for(const std::string& ngram: ngrams1){
			if(count(ngrams2.begin(), ngrams2.end(), ngram) > 0){
				relevance += ComputeNgramFreq(ngram, ngrams2)*ComputeNgramIdf(ngram);
			}
		}			
		return relevance;
	}

double TextComparator::ComputeJordanMeasure(const std::vector<std::string>& words1, const std::vector<std::string>& words2) const{
		double intersection = 0.0;
		std::set<std::string> words1_set(words1.begin(), words1.end());
		std::set<std::string> words2_set(words2.begin(), words2.end());
		for(auto word: words1_set){
			intersection += words2_set.count(word);
		}
		return 1.0*intersection/(words1_set.size() + words2_set.size() - intersection);
	}

double TextComparator::ComputeNgramJordanMeasure(const std::vector<std::string>& ngrams1, const std::vector<std::string>& ngrams2) const{
		return ComputeJordanMeasure(ngrams1, ngrams2);
	}