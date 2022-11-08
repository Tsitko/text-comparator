#include "Line.h"

Line::Line(){}

Line::Line(const std::string& text): line_(text){}

Line::Line(const std::string& text, const std::set<char> valid_symbols): line_(text), valid_symbols_(valid_symbols){}

std::vector<std::string> Line::SplitWordsToVec() {
		std::string clean_text = std::move(ClearText(line_));
		std::string temp = "";
		if(words_vec_.empty()){
			for(const char c: clean_text){
				if(c == ' ' && !temp.empty()){
					if(temp != " "){
						words_vec_.push_back(temp);
					}
					temp = "";
				} else{
				if(c!=' '){
					temp += c;
				}
				}
			}
			if(!temp.empty() && temp != " "){
				words_vec_.push_back(temp);
			}
		}
		return words_vec_;
	}

std::set<std::string> Line::SplitWordsToSet() {
		if(words_set_.empty()){
			for(const std::string& word: SplitWordsToVec()){
				words_set_.insert(word);
			}
		}
		
		return words_set_;
	}

bool Line::IsValidSymbol(const char c) {
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

std::string Line::ClearText(const std::string& text, char replacement) {
		std::string clean_text = "";
		for(const char c: text){
			if(IsValidSymbol(c)){
				clean_text += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			} else{
				if(!clean_text.empty() && clean_text[clean_text.size()-1] != replacement){
					clean_text += replacement;
				}
			}
		}
		return clean_text;
	}