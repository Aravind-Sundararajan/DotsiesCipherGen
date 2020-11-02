#include "kvp.h"
using namespace std;

kvp::kvp(){
	this->key = 'a';
}
kvp::kvp(char f, bitset<5> s)
{
	this->key = f;
	this->value = s;
}
kvp::kvp(const kvp& k)
{
	key = k.key;
	value = k.value;
}
kvp& kvp::operator=(const kvp& k)
{
	(*this).key = k.key;
	(*this).value = k.value;
	return *this;
}
kvp::~kvp(){};