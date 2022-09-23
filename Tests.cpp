#define NGRAM_SIZE 3

#include "TestingFramework.cpp"
#include "Line.cpp"
#include "StopWords.cpp"
#include "TextComparator.cpp"
#include <random>
#include <chrono>

//to compare doubles
const double EPSILON = 10e-6;
//for speed test
const int WORDS_IN_TEXT = 10;
const int NUM_TEXTS = 1000;
const int WORD_SIZE = 6;

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

	//empty stop words
	{
		StopWords sw = StopWords("");
		std::string to_clean = "a small mouse in the house";
		std::vector<std::string> v;
		v={"a", "small", "mouse", "in", "the", "house"};
		Line to_clean_line = Line(to_clean);
		ASSERT_EQUAL_HINT(sw.SplitnIntoWordsNoStop(to_clean_line), v, "There are no stop words");
	}
	
	//non-empty stop words
	{
		std::string s = "a in is the with";
		StopWords sw = StopWords(s);
		std::string to_clean = "a small mouse in the house";
		std::vector<std::string> v;
		v={"small", "mouse", "house"};
		Line to_clean_line = Line(to_clean);
		ASSERT_EQUAL_HINT(sw.SplitnIntoWordsNoStop(to_clean_line), v, "stop words are " + s);
	}
	{
		std::string s = "a of in is the with";
		StopWords sw = StopWords(s);
		std::string to_clean = "A man with the gun";
		std::vector<std::string> v;
		v={"man", "gun"};
		Line to_clean_line = Line(to_clean);
		ASSERT_EQUAL_HINT(sw.SplitnIntoWordsNoStop(to_clean_line), v, "stop words are " + s);
	}
	{
		std::string s = "a of in is the with";
		StopWords sw = StopWords(s);
		std::string to_clean = "A man with small but powerfull gun";
		std::vector<std::string> v;
		v={"man", "small", "but", "powerfull", "gun"};
		Line to_clean_line = Line(to_clean);
		ASSERT_EQUAL_HINT(sw.SplitnIntoWordsNoStop(to_clean_line), v, "stop words are " + s);
	}
}

void TestStopWords(){
	RUN_TEST(TestStopWordsSplit);
}

// -------------------------- Testing TextComparator class ----------------------------------------------

void TestTextComparatorRelevance(){
	{
		TextComparator tc = TextComparator(std::string("a of in is the with"));
		tc.Feed("A man with the gun");
		tc.Feed("The large powerfull gun");
		tc.Feed("A bottle of whisky");
		
		//2 same words, 2 words in second document
		ComparisonStatisctics stat = tc.Compare("A man with small but powerfull gun", "A man with the gun");
		double relevance = stat.relevance;
		double true_relevance = (1.0/2)*std::log(3.0/1) + (1.0/2)*std::log(3.0/2);
		ASSERT_HINT(std::abs(relevance-true_relevance)<EPSILON, std::to_string(relevance) + "-" + std::to_string(true_relevance));
		
		//2 same words, 3 words in second document
		stat = tc.Compare("A man with small but powerfull gun", "The large powerfull gun");
		relevance = stat.relevance;
		true_relevance = (1.0/3)*std::log(3.0/1) + (1.0/3)*std::log(3.0/2);
		ASSERT_HINT(std::abs(relevance-true_relevance)<EPSILON, std::to_string(relevance) + "-" + std::to_string(true_relevance));
		
		//no same words
		stat = tc.Compare("A man with small but powerfull gun", "A bottle of whisky");
		relevance = stat.relevance;
		ASSERT_EQUAL(relevance,0);
	}
	
}

void TestTextComparatorNgramRelevance(){
	TextComparator tc = TextComparator(std::string("a of in is the with"));
	tc.Feed("A small computer"); // sma llc omp ute r mal lco mpu ter all com put er llc omp ute r lco mpu ter com put er omp ute r mpu ter put er ute r (32)
	tc.Feed("The notebook"); // not ebo ok ote boo k teb ook ebo ok boo k
	
	ComparisonStatisctics stat = tc.Compare("The large cmputer", "A small computer"); // lar gec mpu ter arg ecm put er rge cmp ute r gec mpu ter ecm put er cmp ute r mpu ter put er ute r (27)
	double relevance = stat.ngram_relevance;
	double true_relevance = (3.0/32)*std::log(2.0/1)*3*4 + (4.0/32)*std::log(2.0/1)*3*2; // mpu*3;3 ter*3;3 put*3;3 ute*3;4 er*3;3 r*3;4
	ASSERT_HINT(std::abs(relevance-true_relevance)<EPSILON, std::to_string(relevance) + "-" + std::to_string(true_relevance));
}

void TestTextComparatorJordan(){
	TextComparator tc = TextComparator(std::string("a of in is the with"));
	tc.Feed("A man with the gun");
	tc.Feed("The large powerfull gun");
	tc.Feed("A bottle of whisky");
	
	ComparisonStatisctics stat = tc.Compare("A man with small but powerfull gun", "A man with the gun");// {man, small, but, powerfull, gun}(5) {man, gun}(2)
	double jordan = stat.jordan_measure;
	double true_jordan = 1.0*(1+1)/(5+2-2);
	ASSERT_HINT(std::abs(jordan-true_jordan)<EPSILON, std::to_string(jordan) + "-" + std::to_string(true_jordan));
}

void TestTextComparatorNgramJordan(){
	TextComparator tc = TextComparator(std::string("a of in is the with"));
	tc.Feed("A small computer"); // sma llc omp ute r mal lco mpu ter all com put er (13)
	tc.Feed("The notebook"); // not ebo ok ote boo k teb ook ebo ok boo k
	
	ComparisonStatisctics stat = tc.Compare("The large cmputer", "A small computer"); // lar gec mpu ter arg ecm put er rge cmp ute r (12)
	double jordan = stat.ngram_jordan_measure; 
	double true_jordan = 1.0*(6)/(13+12-6); //mpu ter put ute er r (6)
	ASSERT_HINT(std::abs(jordan-true_jordan)<EPSILON, std::to_string(jordan) + "-" + std::to_string(true_jordan));
}

void TestTextComparator(){
	RUN_TEST(TestTextComparatorRelevance);
	RUN_TEST(TestTextComparatorNgramRelevance);
	RUN_TEST(TestTextComparatorJordan);
	RUN_TEST(TestTextComparatorNgramJordan);
}

// -------------------------- Speed test ----------------------------------------------

std::string random_string(std::string::size_type length)
{
    static auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::mt19937 rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string s;

    s.reserve(length);

    while(length--)
        s += chrs[pick(rg)];

    return s;
}

std::string random_string_with_spaces(){
	std::string result;
	for(int i=1; i<=WORDS_IN_TEXT; ++i){
		result += random_string(WORD_SIZE) + " ";
	}
	return result;
}

void SpeedTest(){
	TextComparator tc = TextComparator(std::string("a of in is the with"));
	std::cerr<<"--- Generating texts ---"<<std::endl<<std::endl;
	std::vector<std::string> texts_to_feed;
	for(int i=1; i<=NUM_TEXTS; ++i){
		texts_to_feed.push_back(random_string_with_spaces());
		if(i%(NUM_TEXTS/100)==0){
			std::cerr<<"|";
		}
	}
	std::cerr<<std::endl<<std::endl;

	std::cerr<<"--- Adding texts ---"<<std::endl<<std::endl;
	auto startTime = std::chrono::system_clock::now();
	for(int i=0; i<texts_to_feed.size(); ++i){
		tc.Feed(texts_to_feed[i]);
		if(i%(NUM_TEXTS/100)==0){
			std::cerr<<"|";
		}
	}
	std::cerr<<std::endl<<std::endl;
	auto endTime = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cerr<<NUM_TEXTS<<" texts with "<<WORDS_IN_TEXT<<" words in each text were added in "<<std::to_string(duration)<<" ms"<<std::endl<<std::endl;

	std::cerr<<"--- Comparing texts ---"<<std::endl<<std::endl;
	startTime = std::chrono::system_clock::now();
	for(int i=1; i<=NUM_TEXTS; ++i){
		tc.Compare(texts_to_feed[i - 1], texts_to_feed[texts_to_feed.size() - i]);
		if(i%(NUM_TEXTS/100)==0){
			std::cerr<<"|";
		}
	}
	std::cerr<<std::endl<<std::endl;
	endTime = std::chrono::system_clock::now();
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
	std::cerr<<NUM_TEXTS<<" text pairs were compared in "<<std::to_string(duration)<<" ms"<<std::endl;
}

// -------------------------- Run all tests ----------------------------------------------

void TestAll(){
	TestLine();
	TestStopWords();
	TestTextComparator();
}


int main(){
	TestAll();
	std::cerr<<"----Running speed test----"<<std::endl<<std::endl;
	SpeedTest();
}