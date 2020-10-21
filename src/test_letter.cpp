#include "base.h"
#include "letter.h"
#include "word.h"
#include "phrase.h"
#include "kvp.h"
#include "table.h"
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
	char test[] = "this is a test!";
	for (int x = 1; x < 33; x++){
		bitset<5> in(x);
		t.put(lol[x-1],in);
	}
	cout << test << endl;
	for (int x = 0; x < strlen(test) ; x++){
		letter l(t.get(test[x]));
		l.printb();
	}
	return 1;
}
