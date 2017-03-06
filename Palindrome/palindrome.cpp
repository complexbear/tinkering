#include <iostream>
#include <string>

using namespace std;

std::pair<int, int> largestDistPair(const string s) {
	// Find pair the furthest apart
	size_t dist = 0;
	size_t idx = 0;
	int start;

	while(idx < s.size()) {
		auto pos = s.substr(idx).find_last_of(s[idx]);
		if(pos != string::npos && pos > dist) {
			dist = pos;
			start = idx;
			if(dist >= (s.size()-idx))
				break;
		}
		++idx;
	}

	return make_pair(start, dist);
}

int search(int n, string s) {
    int pLen = 0;
    while(s.size()) {
        
    	auto startDist = largestDistPair(s);
    	if(startDist.first == 0 && startDist.second == 0) {
    		// no pairs in string
    		s = s.substr(1);
    	} else {
    		// +2 to count
    		pLen += 2;
    		s = s.substr(startDist.first+1, startDist.second-1);
    	}

        if(s.size() == 1) {
            pLen += 1;
            break;
        }
        
    }
    return pLen;
}


int longestPalindrome(int n, string s) {
    if(n == 1 || n == 2) return 1;

    return search(n, s);
}

void test(string s, int expected) {
    int l = longestPalindrome(s.size(), s);
    cout << s << " -> " << l << ", expected " << expected << endl;
}

int main(int c, const char* argv[]) {

    string s(argv[1]) ;

    if(s == "test") {
        test("a", 1);
        test("ba", 1);
        test("banana", 5);
        test("banxana", 5);
        test("kxyqvnrtys", 3);
        test("xyziiyiiz", 7);
        test("xyzziiyiiz", 7);
        test("frzrmzlygfveulqfpdbhlqdqrrcrwdnxeuoqqeklaitgdphcspijthbsfyfvladzpbfudkklrwqaozmixrpifeffeclhbvfukbye", 31);
        return 0;
    }

    int m = longestPalindrome(s.size(), s);

    cout << "Result " << m << endl;
    return 0;
}

