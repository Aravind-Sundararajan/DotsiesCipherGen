#include "base.h"
#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "parser.h"
#include "ciphertext.h"
#include <bitset>
#include <chrono>
using namespace std::chrono;

using namespace std;
int main()
{
	auto start = high_resolution_clock::now();

	//bitset<5> in;
	//letter l(in);
	//ciphertext c();
	//kvp k('a',in);

	/*	*/
	table t;
	char lol[] = "abcdefghijklmnopqrstuvwxyz;',.! ";
	for (int x = 0; x < 32; x++){
		bitset<5> in(x+1);
		t.put(lol[x],in);
	}

	char test[] = "sphinx of black quartz, judge my vow.";

	parser ps(t);
	ciphertext out;
	std::size_t sz = strlen(test);
	out = ps.translate(test);

	char plaintext[sz];

	for (int i =0; i < 10; i ++){
		for (int j =0; j < 5; j ++){
			out.row_shift(j);
		}
	}
	ps.translate(out,plaintext);
	std::cout << plaintext << std::endl;

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time: "
	         << duration.count() << " microseconds" << endl;

	return 0;
}
