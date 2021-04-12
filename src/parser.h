#ifndef PARSER_H_
#define PARSER_H_
#include "letter.h"
#include "kvp.h"
#include "table.h"
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

		void translate(char* plaintext);

};

#endif
