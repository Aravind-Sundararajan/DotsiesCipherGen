#include "table.h"
using namespace std;
table::table()
{
	t = new kvp*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++){
		  t[i] = NULL;
	}
};

//destructor
table::~table()
{
	for (int i = 0; i < TABLE_SIZE; i++){
		  if (t[i] != NULL){
				delete t[i];
		  }
	}		
	delete[] t;
};

//copy constructor
table::table(const table& tab)
{
	t = tab.t;
};

//copy assignment
table& table::operator=(const table& t)
{
	return *this;
};

bitset<5> table::get(char key)
{
	int k = hash(key);
	int h = (k % TABLE_SIZE);
	while (t[h] != NULL && t[h]->key != key)
		h = (h+1) % TABLE_SIZE;
	if (t[h] == NULL){
		return ERROR;
	}
	else {
		return t[h]->value;
	}
};

void table::put(char key,bitset<5> val)
{
	int k = hash(key);
	int h = (k % TABLE_SIZE);
	while (t[h] != NULL && t[h]->key != key){
		h = (h+1) % TABLE_SIZE;
	}
	if (t[h] != NULL){
		delete t[h];
	}
	t[h] = new kvp(key, val);
};

int table::hash(char c)
{
	int hash = (int)c;	
	//std::cout << "hash:" << hash << std::endl;
	return hash;
	
};

void table::print()
{
	
};