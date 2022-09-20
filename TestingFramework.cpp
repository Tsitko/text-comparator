#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

template<typename Container>
void Print(std::ostream& out, const Container& container){
    bool first=true;
    for (auto element:container) {
        if(!first){
            out << ", ";
        }
        out<<element;
        first=false;
    }
}

template<typename Element1, typename Element2>
void Print(std::ostream& out, const std::map<Element1, Element2>& container){
    bool first=true;
    for (auto element:container) {
        if(!first){
            out << ", ";
        }
        out<<element.first<<": "<<element.second;
        first=false;
    }
}
template<typename Element>
std::ostream& operator<<(std::ostream& out, const std::vector<Element> container) {
    out<<"[";
    Print(out, container);
    out<<"]";
    return out;
}  
template<typename Element>
std::ostream& operator<<(std::ostream& out, const std::set<Element> container) {
    out<<"{";
    Print(out, container);
    out<<"}";
    return out;
}
template<typename Element1, typename Element2>
std::ostream& operator<<(std::ostream& out, const std::map<Element1, Element2> container) {
    out<<"{";
    Print(out, container);
    out<<"}";
    return out;
}


void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line,
                const std::string& hint) {
    if (!value) {
        std::cerr << file << "(" << line << "): " << func << ": ";
        std::cerr << "ASSERT(" << expr_str << ") failed.";
        if (!hint.empty()) {
            std::cerr << " Hint: " << hint;
        }
        std::cerr << std::endl;
        abort();
    }
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
                     const std::string& func, unsigned line, const std::string& hint) {
    if (t != u) {
        std::cerr << std::boolalpha;
        std::cerr << file << "(" << line << "): " << func << ": ";
        std::cerr << "ASSERT_EQUAL(" << t_str << ", " << u_str << ") failed: ";
        std::cerr << t << " != " << u << ".";
        if (!hint.empty()) {
            std::cerr << " Hint: " << hint;
        }
        std::cerr << std::endl;
        std::abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename TestFunc>
void RunTestImpl(TestFunc& func, const std::string& func_str) {
    func();
    std::cerr<<func_str<<" OK"<<std::endl;
}

#define RUN_TEST(func)  RunTestImpl(func, #func)