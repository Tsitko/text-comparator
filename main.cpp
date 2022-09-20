



#include "Line.cpp"
#include "StopWords.cpp"
#include "TextComparator.cpp"




void print(const std::vector<std::string>& s){
	for(const std::string w: s){
		std::cout<<w<<" ";
	}
	std::cout<<std::endl;
}

void print(const std::map<std::string, std::set<int>>& s){
	for(const auto [w, m]: s){
		std::cout<<w<<": ";
		for(int i: m){
			std::cout<<i<<" ";
		}
	}
	std::cout<<std::endl;
}

void print(ComparisonStatisctics s){
	std::cout<<s.relevance<<" "<<s.ngram_relevance<<std::endl;
}

int main(){
	std::string s= "in the as a is am are with";
	TextComparator t_c = TextComparator(s);
	t_c.Feed("computer with 1tb ssd");
	t_c.Feed("a bottle of whisky");
	ComparisonStatisctics s1 = t_c.Compare("notebook with 512GB ssd", "computer with 1tb ssd");
	ComparisonStatisctics s2 = t_c.Compare("notebook with 512GB ssd", "a bottle of whisky");
	ComparisonStatisctics s3 = t_c.Compare("cmputer with 512GB ssd", "computer with 1tb ssd");
	print(s1);
	print(s2);
	print(s3);
}