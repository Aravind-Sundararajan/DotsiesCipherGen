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

char table::get(bitset<5> value)
{
	int k = hash(value);
	int h = (k % TABLE_SIZE);
	return t[h]->key;
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
	return hash;
};

int table::hash(bitset<5> b)
{
	for (int x =0; x < TABLE_SIZE; x ++){
		if (t[x] != NULL){
			if (t[x]->value == b)
				return hash(t[x]->key);
		}
	}
	return hash(ERRORC);
};

void table::print()
{
	for (int x =0; x < TABLE_SIZE; x ++){
		if (t[x] != NULL){
			std::cout << x << " : " << t[x]->key << std::endl;
		}
	}
};
