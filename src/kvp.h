#ifndef KVP_H_
#define KVP_H_
#include <bitset>
using namespace std;
class kvp //Key Value Pair
{
public:
	char key;
	bitset<5> value;
	kvp(){
		this->key = 'a';
	}
	kvp(char f, bitset<5> s)
	{
		this->key = f;
		this->value = s;
	}
	kvp(const kvp& k)
	{
		key = k.key;
		value = k.value;
	}
	kvp& operator=(const kvp& k)
	{
		(*this).key = k.key;
		(*this).value = k.value;
		return *this;
	}
	virtual ~kvp(){};
};
#endif