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
	/*
	letter l(in);
	word w(l);
	phrase p(w);
	kvp<char,bool*> k('a',in);
	*/
	table t;
	char lol[] = "abcdefghijklmnopqrstuvwxyz;',.! ";
	for (int x = 1; x < 33; x++){
		bitset<5> in(x);
		t.put(lol[x-1],in);
	}
	char test[] = "This is a test!";
	parser ps(t);
	ps.translate(test);

	return 1;
}
