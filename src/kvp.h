#ifndef KVP_H_
#define KVP_H_
#include <bitset>
using namespace std;
class kvp //Key Value Pair
{
public:
	char key;
	bitset<5> value;
	//member functions
	
	kvp();
	kvp(char f, bitset<5> s);
	kvp(const kvp& k);
	kvp& operator=(const kvp& k);
	virtual ~kvp();
};
#endif