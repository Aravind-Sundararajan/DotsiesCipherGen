#ifndef LET_H_
#define LET_H_
class letter
{
	public:
		bool* b[5];		
		//destructor
		~letter()
		{
		};

		//copy constructor
		letter(letter& l)
		{
		};


		//copy assignment constructor
		letter& operator=(letter& l)
		{
			b = l.b;
		};

		//print as bool array
		void printb()
		{
			for (int x = 0; x < 5; x++)
				
		};

}
#endif
