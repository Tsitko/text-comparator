#pragma once

#include <set>
#include <string>
#include <vector>

#include "Line.h"


#define STOPWORDS

class StopWords{
public:
	StopWords();
	StopWords(const std::string& line);
	StopWords(const std::string& line, const std::set<char> valid_symbols);
	
	std::vector<std::string> SplitnIntoWordsNoStop(Line& text) const;
	
private:
	Line stop_words_line_ = Line();
	std::set<std::string> stop_words_set_;
};