// A game of 'Tic tac toe' written by Akshay M Chavan

#include <iostream>
#include <sstream>

using namespace std;

class TicTacToe
{
	private:
		char mSquareContents[ 9 ];

	public:
		TicTacToe()
		{
			for( int index = 0; index < 9; index++ )
			{
				ostringstream os;
				os << (index + 1);
				mSquareContents[index] = (os.str())[0];// Since it is a null-terminated string, the first index has the character
			}
		}
		friend ostream& operator<<(ostream& out, const TicTacToe& t);
}; 		
ostream& operator<<(ostream& out, const TicTacToe& t)
{
	out << '\n';
	for(int index = 0; index < 9; index++)
	{
		if(index % 3 == 0)
		{
			out << ' ' << t.mSquareContents[index] <<
			 " | " << t.mSquareContents[index + 1] << " | " <<  
			 t.mSquareContents[index + 2] << '\n';
	 		for(int num = 1; num <= 11 && index != 6; num++)
	 		{
	 			if(num % 4 == 0)
	 				out << '+';
 				else
 					out << '-';
			}
		out << '\n';
		}
	}
	out << '\n';
	return out;
}	    		


int main()
{
	TicTacToe t;
	cout << t;
	
	return 0;
}
