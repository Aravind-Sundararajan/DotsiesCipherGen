#include "table.h"
using namespace std;
table::table()
{
  t = new kvp*[TABLE_SIZE];                          //allocate an array of kvp(), this is perhaps bad / unsafe design
  for (int i = 0; i < TABLE_SIZE; i++){              //iterate over the max table size
    t[i] = NULL;                                     //set each table entry to NULL
  }
};

                                                     //destructor
table::~table()
{
  for (int i = 0; i < TABLE_SIZE; i++){              //iterate over the max table size
    if (t[i] != NULL){                               //if the table entry is not NULL
  delete t[i];                                       // delete the kvp (table is an array of kvp pointers)
    }
  }
  delete[] t;                                        //finally, delete the table
};

                                                     //copy constructor
table::table(const table& tab)
{
  t = tab.t;                                         //this is probably not correct, just setting this table() using t in the other table()
};

                                                     //copy assignment
table& table::operator=(const table& t)
{
  return *this;                                      //set this table() to other table()
};

                                                     //getters
bitset<5> table::get(char key)
{
  int k = hash(key);                                 //get the position in the table using the hashing function
  int h = (k % TABLE_SIZE);                          //modulo to get circular indexing
  while (t[h] != NULL && t[h]->key != key)           //if the table entry at h is NULL or if the key is not the key we are looking for
  h = (h+1) % TABLE_SIZE;                            //increment h (circular indexing by table size)
  if (t[h] == NULL){                                 //if table entry is NULL
  return ERROR;                                      //return ERROR bitset (this is perhaps a bad design but it works for now, we can reimplement this)
  }
  else {                                             //if we found the key
  return t[h]->value;                                //return the bitset we found
  }
};

char table::get(bitset<5> value)
{
  int k = hash(value);                               //use the overloaded hash function to get the index
  int h = (k % TABLE_SIZE);                          //circular indexing
  return t[h]->key;                                  //return the key at index h
};


                                                     //put a value in the table
void table::put(char key,bitset<5> val)
{
  int k = hash(key);                                 //use hash function to get the index
  int h = (k % TABLE_SIZE);                          //circular indexing using the modulo operator
  while (t[h] != NULL && t[h]->key != key){          //look for the next available table entry
  h = (h+1) % TABLE_SIZE;
  }
  if (t[h] != NULL){                                 //safe, delete this entry if it's not null
  delete t[h];
  }
  t[h] = new kvp(key, val);                          //create a new key value
};


                                                     //"hash"ing functions
int table::hash(char c)
{
  int hash = (int)c;                                 //this is not very smart
  return hash;
};

int table::hash(bitset<5> b)
{
  for (int x =0; x < TABLE_SIZE; x ++){              //iterate over elements in table
  if (t[x] != NULL){                                 //if it's not NULL
  if (t[x]->value == b)                              //value at position x matches the parameter bitset
  return hash(t[x]->key);                            //return the key (integer index)
  }
  }
  return hash(ERRORC);                               //otherwise return the error integer
};

                                                     //print function
void table::print()
{
  for (int x =0; x < TABLE_SIZE; x ++){              //iterate the entries in the table
  if (t[x] != NULL){                                 //if an entry is not null
  std::cout << x << " : " << t[x]->key << std::endl; //cout the key and integer index
  }
  }
};
