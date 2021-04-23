#ifndef PARSER_H_
#define PARSER_H_
#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "ciphertext.h"
#include "mat.h"
using namespace std;
class parser
{
	public:
		table* tab;
		//default constructor
		parser(table& tin);

		//destructor
		~parser();

		//copy constructor
		parser(const parser& p);

		//copy assignment
		parser& operator=(const parser& p);

		ciphertext translate(char* plaintext);

		void translate(const ciphertext& c, char* oStr);
};

#endif
