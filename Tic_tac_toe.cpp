// A game of 'Tic tac toe' written by Akshay M Chavan

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (RAND_MAX + 1.0);
	return min + static_cast<int>((max - min + 1) * (rand() * fraction));
}

class TicTacToe
{
	private:
		char mSquareContents[ 3 ][ 3 ];
		char playerChoice;
		char computerChoice;
		char playerChar;
		char computerChar;
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
			computerChar = 'O';
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
		void takeComputerChoice()//WORKING ON THIS
		{
			srand(static_cast<unsigned int>(time(0)));
			int vacancyCount = 0;// Number of vacant squares in grid
			vector<int> outerIndexes;
			vector<int> innerIndexes;
			for(int indexOuter = 0; indexOuter < 3; indexOuter++)
			{
				for(int indexInner = 0; indexInner < 3; indexInner++)
				{
					if(mSquareContents[indexOuter][indexInner] != 'X' && mSquareContents[indexOuter][indexInner] != 'O')
					{
						vacancyCount++;
						outerIndexes.push_back(indexOuter);
						innerIndexes.push_back(indexInner);
					}
				}
			}
			int randomIndex = getRandomNumber ( 0, vacancyCount - 1);
			mSquareContents[ outerIndexes[randomIndex] ] [ innerIndexes[randomIndex] ] = computerChar; 	
		}
		void play()
		{
			cout << *this;
			
			if(!this->isFull())
			{
				this->takePlayerChoice();
				this->updateGrid(playerChar);
			}
			if(!this->isFull())
			{
				this->takeComputerChoice();
				this->updateGrid(computerChar);
			}
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
