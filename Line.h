#pragma once

#include <set>
#include <string>
#include <vector>

class Line{
public:
	Line();

	explicit Line(const std::string& text);
	
	explicit Line(const std::string& text, const std::set<char> valid_symbols);
	
	std::vector<std::string> SplitWordsToVec();
		
	std::set<std::string> SplitWordsToSet();
	
private:
	std::string line_;
	std::vector<std::string> words_vec_;
	std::set<std::string> words_set_;
	std::set<char> valid_symbols_;
	
	bool IsValidSymbol(const char c);
	
	std::string ClearText(const std::string& text, char replacement = ' ');
};