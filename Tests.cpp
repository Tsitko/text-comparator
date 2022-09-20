#include "TestingFramework.cpp"
#include "Line.cpp"
#include "StopWords.cpp"
#include "TextComparator.cpp"

// -------------------------- Testing Line class ----------------------------------------------

void TestLineSplitWords(){
	//To vector
	{
		Line line = Line("");
		ASSERT_HINT(line.SplitWordsToVec().empty(), "empty vector");
		
		std::string s = "aaa";
		line = Line(s);
		std::vector<std::string> v;
		v.push_back(s);
		ASSERT_EQUAL_HINT(line.SplitWordsToVec(), v, "one word in vec");
		
		std::string s1 = "aaa";
		std::string s2 = "bbb";
		std::string s3 = "ccc";
		s = "aaa bbb ccc";
		line = Line(s);
		v.clear();
		v.push_back(s1);
		v.push_back(s2);
		v.push_back(s3);
		ASSERT_EQUAL_HINT(line.SplitWordsToVec(), v, "multiple words in vec");
	}
	
	//To set
	
	{
		Line line = Line("");
		ASSERT_HINT(line.SplitWordsToSet().empty(), "empty set");
		
		std::string s = "aaa";
		line = Line(s);
		std::set<std::string> v;
		v.insert(s);
		ASSERT_EQUAL_HINT(line.SplitWordsToSet(), v, "one word in set");
		
		std::string s1 = "aaa";
		std::string s2 = "aaa";
		std::string s3 = "ccc";
		s = "aaa aaa ccc";
		line = Line(s);
		v.clear();
		v.insert(s1);
		v.insert(s2);
		v.insert(s3);
		ASSERT_EQUAL_HINT(line.SplitWordsToSet(), v, "multiple words in set");
	}
}

void TestLineValidation(){
	// default validation
	{
		Line line = Line("Haha & is here and ?!# also");
		std::vector<std::string> v = {"haha", "is", "here", "and", "also"};
		ASSERT_EQUAL_HINT(line.SplitWordsToVec(), v, "symbols like &?!# must be deleted");
	}
	// custom validation
	{
		std::set<char> valid_symbols;
		valid_symbols.insert('&');
		valid_symbols.insert('a');
		Line line = Line("Haha & is here and ?!# also", valid_symbols);
		std::vector<std::string> v = {"a", "a", "&", "a", "a"};
		ASSERT_EQUAL_HINT(line.SplitWordsToVec(), v, "the only valid symbols are & and a");
	}
}

void TestLine(){
	RUN_TEST(TestLineSplitWords);
	RUN_TEST(TestLineValidation);
}

// -------------------------- Testing StopWords class ----------------------------------------------

void TestStopWordsSplit(){
	return;
	//empty stop words
	
	//non-empty stop words
}

void TestStopWordsInit(){
	return;
	//stop words string
	
	//stop words string set
	
	//stop words vector
}

void TestStopWords(){
	RUN_TEST(TestStopWordsSplit);
	RUN_TEST(TestStopWordsInit);
}

// -------------------------- Testing TextComparator class ----------------------------------------------

void TestTextComparatorRelevance(){
	return;
}

void TestTextComparatorNgramRelevance(){
	return;
}

void TestTextComparatorJordan(){
	return;
}

void TestTextComparatorNgramJordan(){
	return;
}

void TestTextComparator(){
	RUN_TEST(TestTextComparatorRelevance);
	RUN_TEST(TestTextComparatorNgramRelevance);
	RUN_TEST(TestTextComparatorJordan);
	RUN_TEST(TestTextComparatorNgramJordan);
}

// -------------------------- Run all tests ----------------------------------------------

void TestAll(){
	TestLine();
	TestStopWords();
	TestTextComparator();
}


int main(){
	TestAll();
}
