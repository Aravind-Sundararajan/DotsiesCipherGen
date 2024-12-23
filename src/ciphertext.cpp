#include "ciphertext.h"


                                                         //destructor
ciphertext::~ciphertext()
{
  delete[] phrase;                                       //we have to free the array phrase (technically it's bad to do this with new and delete and you should just use the cpp stdlib)
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
  size = s;                                              //set this size to size_t s
  phrase = new letter [size]{};                          //allocate a new letter array of size 'size'
}

                                                         //copy constructor
ciphertext::ciphertext(const ciphertext& c)
{
  phrase = new letter [c.size]{};                        //allocate new letter() array 'phrase'
	for (std::size_t x = 0; x < c.size; x++)                //iterate over the size of other ciphertext()
		phrase[x] = c.phrase[x];                               //assign position x of this ciphertext() to value at position x of other ciphertext()
}

                                                         //copy assignment constructor
ciphertext& ciphertext::operator=(const ciphertext& c)
{
  size = c.size;                                         //set this ciphertext() size to other ciphertext() size
  delete[] phrase;                                       //free the letter array size we are going to reconstruct it with a new size
  phrase = new letter [size]{};                          //allocate a new letter array 'phrase' of size 'size'
	for (std::size_t x = 0; x < size; x++){                 //iterate from 0 to the size of the ciphertext()
    phrase[x] = c.phrase[x];                             //set position x of this ciphertext() to the value at position x of the other ciphertext()
  }

	return *this;                                           //return reference to this ciphertext() (copy assignment)
};

                                                         //getters
letter& ciphertext::operator()(std::size_t x) const      //operator overload for () so you can access letter()s of a ciphertext() by c(x)
{
	return phrase[x];                                       //return value of this ciphertext() at the position x
}

letter& ciphertext::get(std::size_t x) const
{
	return phrase[x];                                       //return the letter() at position x
}

                                                         //setter
void ciphertext::set(std::size_t X, letter l)
{
    phrase[X] = l;                                       //assign position x in the ciphertext array to the letter() l
}

                                                         //append
ciphertext ciphertext::append(letter l)
{
  ciphertext u(size+1);                                  //to append to the ciphertext in this implementation we have to use the size constructor, perhaps it would be optimal to use the stdlib and vector
  u.set_all(*this);                                      //take the value of this ciphertext and use it to populate a new ciphertext() with size+1
  u(size) = l;                                           //set the final element in ciphertext to the letter() l
  return u;                                              //return the new ciphertext()
}

                                                         //set_all function for assigning values from an other ciphertext to this ciphertext
void ciphertext::set_all(const ciphertext& c)
{
  std::size_t sz;                                        //initialize a size_t for keeping track of the current ciphertext() size, it may not be appropriate to do this with size_t, maybe just use int
  if (c.size < size){                                    //if other size is less than my size
    sz = c.size;                                         //set sz to other size
  }else{
    sz = size;                                           //othersize set sz to my size
  }
  for (std::size_t X = 0; X < sz; X++){                  //iterate from 0 to sz
    set(X,c(X));                                         //use the set function to set this ciphertext values to values in c at position X
  }
  size = sz;                                             //update this ciphertext size

}

                                                         //simple printer function that wraps printb() for all letters in the ciphertext()
void ciphertext::print()
{
  for (std::size_t x = 0; x < size; x++){                //iterate from zero to the number of letters in this ciphertext()
    phrase[x].printb();                                  //get the letter() at position x and printb() (this just prints the bitwise as a combination of ' ' and '#')
  }
}

                                                         //column_shift, operation on the ciphertext that rotates about the x axis in a clockwise rotation.
                                                         //imagine this is a rubik's cube doing an R or L rotation
void ciphertext::column_shift()                          //this is not yet implemented.
{
  for (std::size_t x = 0; x < size; x++){                //iterate from 0 to the number of letter()s in the ciphertext()
    phrase[x].circle_shift();                            //circle_shift each letter()  (this does the clockwise rotation)
  }
}

                                                         //function overload column_shift() with selectable shift direction
void ciphertext::column_shift(bool dir)
{
  for (std::size_t x = 0; x < size; x++){                // iterate over the number of letter()s in the ciphertext()
    phrase[x].circle_shift(dir);                         //perform the circle_shift() in the direction dir for each letter()
  }
}

                                                         //function overload column_shift with selectable shift direction and shift amount
void ciphertext::column_shift(bool dir,int amount)       //this maybe should be size_t or maybe make everything just int
{
  for (std::size_t x = 0; x < size; x++){                //iterate over the number of letter()s in the ciphertext()
    phrase[x].circle_shift(dir,amount);                  //for each letter() circle_shift() by amount in the direction 'dir'
  }
}

                                                         //operation to row_shift (this is like a rubik's cube U or D operation)
void ciphertext::row_shift(std::size_t r)
{
  bool temp = phrase[0].b[r];                            //create a temp bool to store the bitwise at index r for the
  for (std::size_t x = 0; x < size; x++){                //iterate over the number of letter()s in the ciphertext
    phrase[x % size].b.set(r,phrase[(x+1) % size].b[r]); // we are rotating the bitwise here
  }
  phrase[size-1].b.set(r,temp);                          //set the final value with the temp we stored before.
}

void ciphertext::row_shift(std::size_t r, bool direction)
{
    if (direction) {
        // Shift up
        bool temp = phrase[0].b[r]; // Create a temp bool to store the bitwise at index r
        for (std::size_t x = 0; x < size - 1; x++) { // Iterate over the number of letters in the ciphertext
            phrase[x].b.set(r, phrase[x + 1].b[r]); // Rotate the bitwise up
        }
        phrase[size - 1].b.set(r, temp); // Set the final value with the temp we stored before
    } else {
        // Shift down
        bool temp = phrase[size - 1].b[r]; // Create a temp bool to store the bitwise at index r
        for (std::size_t x = size - 1; x > 0; x--) { // Iterate over the number of letters in the ciphertext
            phrase[x].b.set(r, phrase[x - 1].b[r]); // Rotate the bitwise down
        }
        phrase[0].b.set(r, temp); // Set the final value with the temp we stored before
    }
}
                                                         //operation to swap 2 rows of the ciphertext()
void ciphertext::row_swap(size_t p1,size_t p2)
{
  for (std::size_t x = 0; x < size; x++){                //iterate over the letter()s of the ciphertext()
    phrase[x].swap(p1,p2);                               //use the letter() swap() function to swap the positions p1 and p2 on the bitwise
  }
}

                                                         //operation to swap 2 columns of the ciphertext()
void ciphertext::column_swap(size_t p1,size_t p2)
{
  letter temp;                                           //create a temp letter
  temp = phrase[p1];                                     //assign temp to the ciphertext() at position p1
  phrase[p1] = phrase[p2];                               //set the ciphertext() letter() at position p1 to the letter() at p2.
  phrase[p2] = temp;                                     //set the ciphertext() letter() at position p2 to the value temp
}
