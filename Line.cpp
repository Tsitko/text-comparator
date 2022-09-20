#include <set>
#include <string>
#include <vector>

#define LINE

class Line{
public:
	Line(){}

	explicit Line(const std::string& text): line_(text){}
	
	explicit Line(const std::string& text, const std::set<char> valid_symbols): line_(text), valid_symbols_(valid_symbols){}
	
	std::vector<std::string> SplitWordsToVec() {
		std::string clean_text = ClearText(line_);
		std::string temp="";
		if(words_vec_.empty()){
			for(const char c: clean_text){
				if(c==' ' && !temp.empty()){
					if(temp != " "){
						words_vec_.push_back(temp);
					}
					temp="";
				} else{
				if(c!=' '){
					temp+=c;
				}
				}
			}
			if(!temp.empty() && temp != " "){
				words_vec_.push_back(temp);
			}
		}
		return words_vec_;
	}
		
	std::set<std::string> SplitWordsToSet() {
		if(words_set_.empty()){
			for(const std::string& word: SplitWordsToVec()){
			words_set_.insert(word);
			}
		}
		
		return words_set_;
	}
	
private:
	std::string line_;
	std::vector<std::string> words_vec_;
	std::set<std::string> words_set_;
	std::set<char> valid_symbols_;
	
	bool IsValidSymbol(const char c) {
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
	
	
	std::string ClearText(const std::string& text, char replacement = ' ') {
		std::string clean_text="";
		for(const char c: text){
			if(IsValidSymbol(c)){
				clean_text += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			} else{
				clean_text += replacement;
			}
		}
		return clean_text;
	}
};