#include <iostream>
#include <string>

using namespace std;



int longestPalindrome(int n, string s) {
	string r(s.rbegin(), s.rend());
	int pLen = 0;

	if(s.size() == 1 || s.size() == 2) return 1;

	// Find pair with shortest dist between s and r
	while(s.size() && r.size()) {

		cout << "--------------" << endl;
		cout << s << endl;
		cout << r << endl;

		if(s.size() == 1 && r.size() == 1) {
			pLen += 1;
			break;
		}

		if(s[0] == r[0]) {
			pLen += 2;
			s = s.substr(1, s.size()-2);
			r = r.substr(1, r.size()-2);
		} else {

			cout << "sIdx: " << s[0] << ", rIdx: " << r[0] << endl;
			size_t sDist = r.find_first_of(s[0]);
			size_t rDist = s.find_first_of(r[0]);

			cout << "sDist: " << sDist << ", rDist: " << rDist << endl;

			if(sDist == r.size()-1) {
				// First char of s has no match
				s = s.substr(1);
				r = r.substr(0, r.size()-1);
			} else if(rDist == s.size()-1) {
				// First char of r has no match
				r = r.substr(1);
				s = s.substr(0, s.size()-1);
			} else if (rDist < sDist) {
				//
				s = s.substr(rDist);
				r = r.substr(0, r.size()-sDist);
			} else {
				r = r.substr(sDist);
				s = s.substr(0, s.size()-sDist);
			}
		}

	}

	return pLen;
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

