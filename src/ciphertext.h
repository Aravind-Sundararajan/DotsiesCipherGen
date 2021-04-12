#ifndef CIP_H_
#define CIP_H_
#include "base.h"
#include "letter.h"
using namespace std;

class ciphertext
{

public:
  letter *phrase{};
  std::size_t size{};

	//destructor
	~ciphertext();

	ciphertext();

  //size constructor
	ciphertext(std::size_t s);

	//copy constructor
	ciphertext(const ciphertext& c);

	//copy assignment constructor
	ciphertext& operator=(const ciphertext& l);

	//getters
	letter& operator()(std::size_t x) const;

	letter& get(std::size_t x) const;

	void set(std::size_t X, letter l);

  ciphertext append(letter);

  void set_all(const ciphertext& c);
};
#endif
