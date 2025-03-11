#include "parser.h"
using namespace std;
                                             //default constructor
parser::parser(table& tin)
{
  tab = &tin;
};

                                             //destructor
parser::~parser()
{
                                             //delete tab;
};

                                             //copy constructor
parser::parser(const parser& p)
{
  tab = p.tab;
};

                                             //copy assignment
parser& parser::operator=(const parser& p)
{
  tab = p.tab;
  return *this;
};

                                             //translate function - transforms the character array plaintext into a ciphertext()
ciphertext parser::translate(char* plaintext)
{
  std::size_t slen = strlen(plaintext);      //get the length of the ciphertext/plaintext
  ciphertext c(slen);                        //length constructor for the ciphertext
  for (std::size_t x = 0; x < slen; x++) {
    auto [found, bits] = tab->get(plaintext[x]);
    letter l(found ? bits : bitset<5>());  // Use empty bitset for invalid chars
    c.set(x, l);                              //set the ciphertext() index position x to letter() l
  }
  return c;                                  //return the ciphertext we just populated and constructed
};

                                             //translate function - transforms a ciphertext() into a character array plaintext
void parser::translate(const ciphertext& c, char* oStr)
{
  for (std::size_t x = 0; x < c.size(); x++) {
    auto [found, ch] = tab->get(c.get(x).b);
    oStr[x] = found ? ch : ' ';  // Use space for invalid/unmapped values
  }
  oStr[c.size()] = 0;  // Add null terminator
};
