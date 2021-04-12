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

void parser::translate(char* plaintext)
{
	for (std::size_t x = 0; x < strlen(plaintext) ; x++){
		//cout << plaintext[x] << " : " << tab->get(plaintext[x]) << endl;
		letter l(tab->get(plaintext[x]));
		l.printb();
	}
};
