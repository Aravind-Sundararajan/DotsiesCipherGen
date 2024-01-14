#include "base.h"
#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "parser.h"
#include "ciphertext.h"
#include <bitset>
#include <chrono>
#include <iostream>
using namespace std::chrono;

using namespace std;
int main()
{
	
	//bitset<5> in;
	//letter l(in);
	//ciphertext c();
	//kvp k('a',in);

	/*	*/
	table t;
	char all_graphemes[] = "abcdefghijklmnopqrstuvwxyz;',.! ";

	for (int x = 0; x < 32; x++){
		bitset<5> in(x+1);
		t.put(all_graphemes[x],in);
	}

	char test[] = "sphinx of black quartz, judge my vow.";
	
	parser ps(t);
	ciphertext out;

	out = ps.translate(test);
	char* plaintext = test;
	

	for (int i =0; i < 10; i ++){
		for (int j =0; j < 5; j ++){
			out.row_shift(j);
		}
	}
	
	auto start = high_resolution_clock::now();

	ps.translate(out,plaintext);

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	
	std::cout << "Time: " << duration.count() << " microseconds" << std::endl;
	
	std::cout << plaintext << std::endl;

	return 0;
}
