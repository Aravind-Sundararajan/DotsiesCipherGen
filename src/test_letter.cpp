#include "base.h"
#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "parser.h"
#include "ciphertext.h"
#include <bitset>
using namespace std;
int main()
{
	//bitset<5> in;
	//letter l(in);
	//ciphertext c();
	//kvp k('a',in);

	/*	*/
	table t;
	char lol[] = "abcdefghijklmnopqrstuvwxyz;',.! ";
	for (int x = 1; x < 33; x++){
		bitset<5> in(x);
		t.put(lol[x-1],in);
	}
	char test[] = "This is a test!";
	parser ps(t);
	ciphertext out;
	out = ps.translate(lol);
	out.print();
	return 1;
}
