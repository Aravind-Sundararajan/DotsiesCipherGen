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

  //setters
  void set_all(const ciphertext& c);

  //printer
  void print();

  //ops
  void row_swap(size_t p1,size_t p2); //swap 2 rows of the bitsets
  void column_swap(size_t p1,size_t p2); //swap letter of the ciphertext
  void column_shift(); //rotate ciphertext
  void column_shift(bool dir); //rotate ciphertext
  void column_shift(bool dir, int amount); //rotate ciphertext
  void row_shift(std::size_t r); //rotate ciphertext
};
#endif
