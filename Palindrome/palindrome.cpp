#include <iostream>
#include <string>

using namespace std;

int search(int n, string s) {
    int pLen = 0;
    string pStr;

    while(s.size()) {
        char ch = s[0];
        size_t dupPos = s.find_last_of(ch, n);

        if(dupPos == 0) {
            s = s.substr(1);
        }
        else {
            pLen += 2;
            // recurse.... by stepping +n places forwards until
            // a better matching pair is found
            s = s.substr(1, dupPos-1);
        }
        
        if(s.size() == 1) {
            pLen += 1;
            break;
        }
        
    }
    return pLen;
}


int longestPalindrome(int n, string s) {
    if(n == 1) return 1;

    int pLen = 0;
    while(s.size() < pLen) {
        int l = search(s.size(), s);
        pLen = max(pLen, l);

        s = s.substr(1);
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
        test("frzrmzlygfveulqfpdbhlqdqrrcrwdnxeuoqqeklaitgdphcspijthbsfyfvladzpbfudkklrwqaozmixrpifeffeclhbvfukbye", 31)
        return 0;
    }

    int m = longestPalindrome(s.size(), s);

    cout << "Result " << m << endl;
    return 0;
}

