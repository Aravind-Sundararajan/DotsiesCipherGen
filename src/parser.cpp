#include "parser.h"
using namespace std;
//default constructor
parser::parser(table& tin)
{
	tab = &tin;
};

//destructor
parser::~parser()
{
//delete tab;
};

//copy constructor
parser::parser(const parser& p)
{
	tab = p.tab;
};

//copy assignment
parser& parser::operator=(const parser& p)
{
	tab = p.tab;
	return *this;
};

ciphertext parser::translate(char* plaintext)
{
	std::size_t slen = strlen(plaintext);
	ciphertext c(slen);
	for (std::size_t x = 0; x < slen ; x++){
		letter l(tab->get(plaintext[x]));
		c.set(x,l);
	}
	return c;
};

void parser::translate(const ciphertext& c, char* oStr)
{
	for (std::size_t x = 0; x < c.size ; x++){
		oStr[x] = tab->get(c.get(x).b);
	}
	oStr[c.size] = 0;
};
