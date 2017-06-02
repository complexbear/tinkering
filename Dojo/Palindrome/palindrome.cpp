#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

using Matrix = vector<vector<int>>;

void print(const Matrix& m) {
	for(size_t i=0; i<m.size(); ++i) {
		for(size_t j=0; j<m.size(); ++j) {
			cout << m[i][j] << ' ';
		}
		cout << endl;
	}
}

int longestPalindrome(int n, string s) {
	if(n == 1)
		return 1;

	Matrix results(n, vector<int>(n, 0));

	for(int i=0; i<n; ++i) {
		results[i][i] = 1;
	}
//	print(results);

	for(int l=2; l<=n; ++l) {

//		cout << "-------------------" << endl;
		for(int r=0; r<(n-l+1); ++r) {

			int c = r + l - 1;

			if(s[r] == s[c] && l == 2) {
				results[r][c] = 2;
			} else if(s[r] == s[c]) {
				results[r][c] = results[r+1][c-1] + 2;
			} else {
				results[r][c] = max(results[r][c-1], results[r+1][c]);
			}
		}
//		print(results);
	}

	return results[0][n-1];
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
        test("tjkcalswqqhvyimheopjzdkmouegzbphmgoxqwlbdlcumdgixjbcqvztzdjqmadthtdmvaqcagsyqggcfiifcoktjpjxcjyiwchyicqibxdgkqtgqiwpdklhumzwljmgdmicmunnnpdbamofynykqvsijsnfkpfyptlmqpoyjmeqvhcrvgmqmqopusqktdthpvztfzxrnqbsqtipqonxtsnasonzvxbpipyhhbtopsuqomfjrdyislifqgbndakaqbbwnkxnwpzeoohlxuhrtbfnqorfguvomeepxoffghmatidzfpfnwbfujdonlvyvwcwcchvsasdylbrrxfcztzqopdihybrhodjnmoqkijywkoncvrjozdphbfaalexgtpdtkzvsebevsopjvciwljfkrcumyacqdapwczenvmabkapuoudipbozezeygljfftvycbvazmzbndlfvlsqiaixdtbhqvlzdmffjfbfsvthrfmkoxbhokcvethucgqyvopafyttrhesvlbgihetenaiqyufxffdwqvruhvuzxukffmudygjavemzdccamymhppwofwgjkykmqfbuujzxhlnckmmcuflzandlltowjpwsaljtfapfvrmezbsjxsswiwjscisoqlhahzaplclkwlrgcdqbcdwbwafgadgtcpgowefkaqjuffuguqepwnfhbnbuinlicpdxfesoxcfqclfnrhgsxkhxgxrcorfygjxpiqmwtqjwbhghmlocnfkglorkkvxznlzdhucvayrzfazefobxutitrfkrlrstkcbtikklmhmggklsbphcejuylhxnraoenhdpzngyodiqlchxyycxzrrbhfwohfvxkrzolrglgnvcpjesdzygyoifoygphqqevqjugiuahmvrxgaujnyclcjqxevhyfnlohavrjlbyyvhghxhywekedhvwytysljsqminajfipdsprglnpxfqwuvcibpynkxgxsupmbntqrlwyampdgunigxldhlhqdyjcfhuqjfymrbafmyocttyjmnhlfnrtzddlazixtlxyvmvfbiguhsfuqoerhymsnoprkbdlchswocbbwwdvastaiamnepwkyaqmpliruphedkxjqzgpexlwulswtvotlgotlncpvnrrzerzdygeraoozbttnyyifkindeouuagqgztvqdolfrzrlzddjzcshvdgkhxkdxflwxmejkkszylbhoaacicwqiizickgcdxstpzkskjwdqegrearjrncmmkdelekbesuhquepjrnrzbllldgdmyrpglrhllwnqozfudjpchowhwevbqvjjezsmhylnjpktbspxnktxkmcfxwiaqqbhqjwufmwresfsfaokeaaadziobioqmtsvjgzkgkhbzpaeuexyudhrioicyqsjmngrwqpoherwuvdgeruffmlgphwbxzovyewtfazfpmvfvyguqditjlxnoixsyghyfrdngjfblyveibcapetpmeaidrtpnwwiifhpfgsptkvhhwkzvtvlhhbipjxurgqbtldimsarncplkeweoirjenakyprzzphewoprwyvfpjyglzrmhfqpdubheeqtgrmbxlcmnbtaqakgimuapsbuxzokhwnykughmwrlkjsvtdlzwpbhcsbvjnomutffmjggrmyilgschgwrrztnmvndmuahvnlwpwtglvbtkegzjstpiwllpgnlpcnezqscxkelindfvurtxsezrwzvzechocnvfaimxrqnyiqlybugjsblhzfravznkbtgcpqwovwpxzgxgodlhmixisfzdknoxzasscewgzvqkxuqrsqxsfwdwneyqisozqjfgrvhfsfeuspujvqhydwveadosqafyxbmzgrotyffqblolplosqnfcwxiuwkldpuenodlvotbqiizudxqjvfnkyicjcywjkfvukvveqhjrxdcigwbjdftdyelydzyummmtzuvfmaicednymxoaiuhigfkfavgeemcgofnaefganqoniqebfibigljbceulicojzjfrceigxprunjncymbrljfqmwciqtyncafzjyagimmzuejrttefhdwqcizyiurxvfbwrjxffzbjexshvwrxwrmhrbdxjwipsdfhtknfaswfrvxqdkhbwwefbvqsghhhutdgethcwupzrtsbhbjnbqwpccoaonxwvkhowbzhaoscgykzbiltlwqqatyeczzceuuwgvjxzonhkkfjcbwsezdmifdegsyjtswselknxweimxlnzohgtqthlftjblnphtdwwvsscbhjmruuqscwjpynxbkoomwdecqkrtbxgzgcxhppcjnqtcfqttkkolzcfikwblxkimijeglxsvcrkcqjjwcwuhvzydmegubqavlqffhrzzrhiwxrgftittdxcaybczncsyjlzlxyyklcppoxcgnexmaajzuhabdhaccuualacylsmtkewbprsmoncggqvrvsqqoffmwkplsgbrpurgioajuppvqluhbdetzdzwwzaelmopafumtqugecywglucmugwqiizveswrjevfzdnxlbkakrpzcejvxzeoybbtfdsvewjsivwswzjhudtpqsfclzcmukotirrxxtzksmxnjumofzhhowyftzfzbotrokaxaryxlkmueolqkrdxmzhoqnzvudeowcjloesfmqgvxwfyhnpbepbvbgjtbvqpoeugoqynkbfivmfewjjscjreixyqssxzsydgllfzmobnurxkwcypctjkabigwqtldwhjouaswtovdtkrlonzgeyddkqwuhnimzffxviyvsktqrfafqujhdepvczzhiyjlkxmeplnrvxgshdykehkefpkvepcxhozpjxtreynyliguhuxudbnhmfojordxzmmklgohcmmbukzshyrmooliaobbnlapadutnbetocxylceyxywdsjegdfcrsblbxhjsgcuoxmqftytngzdcmsrfyjjumcbxoleldazwzwtdjoyuqeqjfxazjarqgfmsfxyfqbeoktcctnfqrsjchxpqiltaqzawhguusgenjcfxriyfikpgdvwhbyumthkiktbecjwalsxqjyajrkcvysicisabtbrdeumbvtviihjwxdczpdnrrgmemxydgxlrjzucxyidwcdvdpvjlagwmgcngnpxjkximsogvyhnchlimsxcjwtrijtizpezjlcqekojrrjzvtvhnqvieqmdiedzqswrsnfmnneltcutqfcqyrrwmnwtelvsqrruwfjwrhjcrtbcytnyqmqxceuohpiffaqnrmoybqjjgdyotpmxttqftypfexlvdtgxqafiqbqjlnpbflkgaynkddbzllecdctccvslpdcurkxfoimnwdfvnyqkzlheruxrmiiygnzfovnrwcoqsgoameiunvzemmxtklistlxuynrwsjaxzwmhktdayzzoxbbemejgosgcynfapykbcnzhesvnctfvdspqkuyrjhykpadahbhjbatvsxlngbtxmqrbwqromfqzyabzrcoorbbyklndcqdjzcnsmgmwmpbgjqoykrvnafhqrwfajnfahyqocdbgfnlaozmffvvmoymbmytjvfcgttcijyyljdgwjbztlpswmalgbgpaplqgzqchflxtypttkrpfvxrtkzpzrscwbxqbwfqqvjcukjbsginxqmxryxieyavnzdiludhthpreyelgcnpsoqswsreogbmveofvusdsxcjvdrjkhxkqrqzjbrpljwuzplxczqeevvbvcwhnvzuszqvhlmaptylyufqzyldwwgezlqurzvorxwgdtgjilgydqvpkntwjbgihlwckorgrmxixylxjmlfvhbpoaboylcedyzeayigfagitkrextlipvqqhdmoerchoqxbypihdvpdtjrrkxroowtrexuhucxpiibvmuyarjwqpcoywawqwdmfpwvamisnskbhopoqdsrefjtmnmvkyccremktrniqwoofoeenbzvvlrfcsftapyujmwdupdqikocrdyexjbcxwqeaumqoklsqebuzeziiswzfzgyhvnwjcnshdybjywyaodsywmlmuueocfruntpztlbggsuzctocyilfzpszgrgjsimqypqopfumpmsjvmsbrcfwretbiekoxpmplpcgewmqigymreqevdydvgmuyytguexnycqhwialkbjgzcklmgxgijjyjlplrsxznfkoklxlvpxrbasbznvxasxztwnqbhqultkyfqxsaeqztxbuwpzdpkngnuoayckfykdlarmuwprgtghfxfbgpucqwbihemixqmypjedyedimaanpbdrxpvtoxmxzimgfaouzlteobnadoshnxnufgmypupeikfqrqyexoljhqsaneicvaerqxcngmsidszagpiuiiasrezakiuroadojxmvgqgdodwwjszyeruwnupqgmyrjxigaskpjmzpsbhgokwtfcisjzvepyebwrcpafxzhbdgjykhzemfomgxjphcscuxjnywoiwbdvusywqlsccvsropvrnxtmkgiwfvqfkotpdznjinpobubzbvstkkvubuucilxyhjbcilfldibmyebrlcnvnuuqfvhwxoorbyyiigppuftqswpksfdxicemjbftgoabwrqdoudfbyjutkeqakoarruhwuznnlydivfelxvtggkkkjmxlwbkbklbwfsvrbadvraqovanlsmklnfjxxvzdjlpyurxpjcssebtpznwoytjefobdynaiukctgrzjxzirzosjuncvhfhcnhhrajahitnbkvwtifcoqepqyzwyazzkddxywvgqxofsyxngevsdbatvzmziqpmjewiyorpzsupxoflqpbyzhtvvpjdcqnlvlzrzakvmxzbaeipmwouvpvgtek", 1608);
        return 0;
    }

    int m = longestPalindrome(s.size(), s);

    cout << "Result " << m << endl;
    return 0;
}

