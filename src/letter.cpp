#include "letter.h"
using namespace std;

//destructor
letter::~letter(){};

//default constructor
letter::letter()
{
  for (int x =0; x < 5; x++)       //bitwise only ever has 5 elements
  b[x] = true;   //assign every element to true
}

//bitset constructor
letter::letter(const bitset<5> bin)
{
  for (int x =0; x < 5; x++)
  b[x] = bin[x];
};

//copy constructor
letter::letter(const letter& l)
{
  for (int x = 0; x < 5; x++)
  b[x] = l.b[x];
};


//copy assignment constructor
letter& letter::operator=(const letter& l)
{
  for (int x = 0; x < 5; x++)
  b[x] = l.b[x];


  return *this;  //return reference to this letter()
};

//getters
bool letter::operator()(size_t x) const
{
  return b[x];
}

bool letter::get(size_t x) const
{
  return b[x];
}

//setters
void letter::set(size_t x, bool v)
{
  b[x] = v;
}

//print as bool array
void letter::printb()
{
//cout << "printing letter:" << endl;
  for (int x = 0; x < 5; x++){     //iterate over each of the 5 bits in bitset
  if (b.test(x)){//if the current bit is true
  cout << "#";   //print '#'
  }else{
  cout << " ";   //else print ' '
  }
  }
  cout << endl;  //return carriage
};

void letter::circle_shift()
{
  bool start = b[0];               //get the first element of the bitset into a temp bool variable
  for (int x = 0; x < 5; x++)      //iterate over the elements of bitset
  b.set(x % 5,b[(x + 1) % 5]);     // set bitset value to next value, also using circular indexing via modulo operator
  b.set(4,start);//finally take temp start bool value and set the final bitset position to start
}

void letter::circle_shift(bool dir)
{
  if (dir){      //clockwise rotation (default rotation)
    circle_shift();
  }
  else{         //anti-clockwise rotation of the bitset
    bool start = b[4];               //save the final bit in the bitset

    for (int x = 4; x > -1; x--){     //iterate through bits in the bitset in reverse starting from the final bit
      b.set(x % 5,b[(x - 1) % 5]);     // set bitset value to previous value, also using circular indexing via modulo operator
    }

    b.set(0,start);//finally set the first bit to the saved temp final bit
  }
}


//circle_shift where you can rotate by an amount
void letter::circle_shift(bool dir, int amount)
{
  for (int x =0; x < amount; x++){ //simple implementation to loop circle_shift amount X, this is particularly inefficient. rewrite this one
    circle_shift(dir);
  }
}

//flip
void letter::flip()
{
  b = b.flip();  //flip every bit in the bitset
}

//swap
void letter::swap(std::size_t p1, std::size_t p2)
{
  bool temp;     //save a temp boolean
  temp = b[p1];  //assign temp to the value in the bitset at position p1
  b.set(p1,b[p2]);                 //set the bit at p1 to the value of the bit at position p2
  b.set(p2,temp);                 //set the bit at p2 to the value of the temp boolean
}
