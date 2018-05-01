// A game of 'Tic tac toe' written by Akshay M Chavan

#include <iostream>
#include <sstream>

using namespace std;

class TicTacToe
{
	private:
		char mSquareContents[ 3 ][ 3 ];
		char playerChoice;
		char computerChoice;
		char playerChar;
		char digitArr[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
			
	public:
		TicTacToe()
		{
			int squareValue = 1;
			for( int indexOuter = 0; indexOuter < 3; indexOuter++ )
			{
				for(int indexInner = 0; indexInner < 3; indexInner++) 
				{
					ostringstream os;
					os << squareValue;
					mSquareContents[indexOuter][indexInner] = (os.str())[0];// Since it is a null-terminated string, the first index has the character
					squareValue++;
				}	
			}
			
			playerChar = 'X';
		}
		friend ostream& operator<<(ostream& out, const TicTacToe& t);
		void updateGrid(char character)
		{
			for(int indexOuter = 0; indexOuter < 3; indexOuter++)
			{
				for(int indexInner = 0; indexInner < 3; indexInner++)
				{
					if(playerChoice == mSquareContents[indexOuter][indexInner])
						mSquareContents[indexOuter][indexInner] = character;
				}
			}
		}
		void takePlayerChoice()
		{
			int count = 0;
			
			cout << "Enter appropriate choice: ";
			cin >> playerChoice;
			cin.ignore(32767, '\n');
	
			for(int indexOuter = 0; indexOuter < 3; indexOuter++)
			{
				for(int indexInner = 0; indexInner < 3; indexInner++)
				{
					if(playerChoice == mSquareContents[indexOuter][indexInner])
						count++;
				}
			}
			if(count == 0)
			{
				cout << "Error: Invalid input.\n";
				this->takePlayerChoice();
			}
		}
		void play()
		{
			cout << *this;
			this->takePlayerChoice();
			this->updateGrid(playerChar);
		}
		bool isFull()
		{
			int count = 0;
			for(int indexOuter = 0; indexOuter < 3; indexOuter++)
			{
				for(int indexInner = 0; indexInner < 3; indexInner++)
				{
					for(auto digit: digitArr)
					{
						if(digit == mSquareContents[indexOuter][indexInner])
							count++;
					}
				}
			}
			return (count == 0) ? true : false;		
		}
}; 		
ostream& operator<<(ostream& out, const TicTacToe& t)
{
	out << '\n';
	for( int indexOuter = 0; indexOuter < 3; indexOuter++ )
			{
				for(int indexInner = 0; indexInner < 3; indexInner++)
				{
					if(indexInner == 0)
					{
						out << ' ' << t.mSquareContents[indexOuter][indexInner] <<
			 			" | " << t.mSquareContents[indexOuter][indexInner + 1] << " | " <<  
			 			t.mSquareContents[indexOuter][indexInner + 2] << '\n';
	 					for(int num = 1; num <= 11 && indexOuter != 2; num++)
	 					{
	 						if(num % 4 == 0)
	 							out << '+';
 							else
 								out << '-';
						}
						out << '\n';
					}
				}
			}
	out << '\n';
	return out;

}	    		


int main()
{
	TicTacToe t;
	while(!t.isFull())
		t.play();
	cout << t;
			
	return 0;
}
