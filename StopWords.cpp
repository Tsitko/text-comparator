#include <set>
#include <string>

#ifndef LINE
#include "Line.cpp"
#endif

#define STOPWORDS

class StopWords{
public:
	StopWords(){}
	
	StopWords(Line& line){
		stop_words_line_=line;
		stop_words_set_ = stop_words_line_.SplitWordsToSet();
	}
	
	std::vector<std::string> SplitnIntoWordsNoStop(Line& text) const{
		std::vector<std::string> result;
		for(const std::string& word: text.SplitWordsToVec()){
			if(!stop_words_set_.count(word)){
				result.push_back(word);
			}
		}
		return result;
	}
	
private:
	Line stop_words_line_=Line();
	std::set<std::string> stop_words_set_;
};