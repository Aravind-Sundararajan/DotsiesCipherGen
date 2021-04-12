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
  phrase = new letter [size]{};
	for (std::size_t x = 0; x < 5; x++){
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

}
