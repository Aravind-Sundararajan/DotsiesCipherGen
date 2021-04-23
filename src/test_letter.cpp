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

	//cout << "table:" << endl;
	//t.print();

	std::cout << "printing the plaintext:" << std::endl;
	std::cout << test << std::endl;

	parser ps(t);
	ciphertext out;
	std::size_t sz = strlen(test);
	out = ps.translate(test);

	// ciphertext bet;
	// bet = ps.translate(lol);
	// bet.print();

	std::cout << "reconstituting the plaintext:" << std::endl;
	char plaintext[sz];
	plaintext[0] = 0;
	ps.translate(out,plaintext);
	cout << plaintext << endl;

	//out.print();


	std::cout << "circleshift clockwise 1:" << std::endl;
	for (int i =0; i < 5; i ++){
		out.circle_shift(true);
		char plaintext_cs[sz];
		ps.translate(out,plaintext_cs);
		std::cout << plaintext_cs << std::endl;
	}

	std::cout << "circleshift CC 1:" << std::endl;
	for (int i =0; i < 5; i ++){
		out.circle_shift(false);
		char plaintext_cs[sz];
		ps.translate(out,plaintext_cs);
		std::cout << plaintext_cs << std::endl;
	}


	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time: "
	         << duration.count() << " microseconds" << endl;

	return 0;
}
