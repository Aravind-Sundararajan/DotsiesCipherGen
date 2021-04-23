#include "ciphertext.h"


//destructor
ciphertext::~ciphertext()
{
  delete[] phrase;
}

//default constructor
ciphertext::ciphertext()
{
  size = (std::size_t)1;
  phrase = new letter [size]{};
}

//size constructor
ciphertext::ciphertext(std::size_t s)
{
  size = s;
  phrase = new letter [size]{};
}

//copy constructor
ciphertext::ciphertext(const ciphertext& c)
{
  phrase = new letter [c.size]{};
	for (std::size_t x = 0; x < c.size; x++)
		phrase[x] = c.phrase[x];
}

//copy assignment constructor
ciphertext& ciphertext::operator=(const ciphertext& c)
{
  size = c.size;
  delete[] phrase;
  phrase = new letter [size]{};
	for (std::size_t x = 0; x < size; x++){
    phrase[x] = c.phrase[x];
  }

	return *this;
};

//getters
letter& ciphertext::operator()(std::size_t x) const
{
	return phrase[x];
}

letter& ciphertext::get(std::size_t x) const
{
	return phrase[x];
}


void ciphertext::set(std::size_t X, letter l)
{
    phrase[X] = l;
}

ciphertext ciphertext::append(letter l)
{
  ciphertext u(size+1);
  u.set_all(*this);
  u(size) = l;
  return u;
}

void ciphertext::set_all(const ciphertext& c)
{
  std::size_t sz;
  if (c.size < size){
    sz = c.size;
  }else{
    sz = size;
  }
  for (std::size_t X = 0; X < sz; X++){
    set(X,c(X));
  }
  size = sz;

}

void ciphertext::print()
{
  for (std::size_t x = 0; x < size; x++){
    phrase[x].printb();
  }
}

void ciphertext::circle_shift()
{
  //wrapper function that calls letter swap, swaps 2 rows in the ciphertext
  for (std::size_t x = 0; x < size; x++){
    phrase[x].circle_shift();
  }
}

void ciphertext::circle_shift(bool dir)
{
  //wrapper function that calls letter swap, swaps 2 rows in the ciphertext
  for (std::size_t x = 0; x < size; x++){
    phrase[x].circle_shift(dir);
  }
}

void ciphertext::circle_shift(bool dir,int amount)
{
  //wrapper function that calls letter swap, swaps 2 rows in the ciphertext
  for (std::size_t x = 0; x < size; x++){
    phrase[x].circle_shift(dir,amount);
  }
}

void ciphertext::row_swap(size_t p1,size_t p2)
{
  //wrapper function that calls letter swap, swaps 2 rows in the ciphertext
  for (std::size_t x = 0; x < size; x++){
    phrase[x].swap(p1,p2);
  }
}

void ciphertext::column_swap(size_t p1,size_t p2)
{
  letter temp;
  temp = phrase[p1];
  phrase[p1] = phrase[p2];
  phrase[p2] = temp;
}
