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
		void takeComputerChoice()
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
		void checkVictory() //WORKING ON THIS
		{
			/*
			Victory occurs when:
				1. Horizontal match is found: array1[outer][inner] == array2[outer][inner+1] == array3[outer][inner+2], or
				2. Vertical match is found: array1[outer][inner] == array2[outer+1][inner] == array3[outer+2][inner], or
				3. Right sloping diagonal is found: array1[outer][inner] == array2[outer+1][inner+1] == array3[outer+2][inner+2], or
				4. Left sloping diagonal is found: array1[outer][inner] == array2[outer+1][inner-1] == array3[outer+2][inner-2]
			*/
			
			for(int outerIndex = 0; outerIndex < 3; outerIndex++) //Checking victory condition 1
			{
				if( mSquareContents[outerIndex][0] == mSquareContents[outerIndex][1] && mSquareContents[outerIndex][1]== mSquareContents[outerIndex][2] )
				{
					if( mSquareContents[outerIndex][0] == playerChar )
						cout << "\n\tYOU WIN!\n";
					else
						cout << "\n\tYOU LOSE!\n";
					throw 0;
				}
			}
			
			for(int innerIndex = 0; innerIndex < 3; innerIndex++) //Checking victory condition 2
			{
				if( mSquareContents[0][innerIndex] == mSquareContents[1][innerIndex] && mSquareContents[1][innerIndex] == mSquareContents[2][innerIndex] )
				{
					if( mSquareContents[0][innerIndex] == playerChar )
						cout << "\n\tYOU WIN!\n";
					else
						cout << "\n\tYOU LOSE!\n";
					throw 1;
				}
			}
			
			if( mSquareContents[0][0] == mSquareContents[1][1] && mSquareContents[1][1] == mSquareContents[2][2] ) //Checking victory condition 3
			{
				if( mSquareContents[0][0] == playerChar )
					cout << "\n\tYOU WIN!\n";
				else
					cout << "\n\tYOU LOSE!\n";
				throw 2;
			}
			
			if( mSquareContents[0][2] == mSquareContents[1][1] && mSquareContents[1][1] == mSquareContents[2][0] ) //Checking victory condition 4
			{
				if( mSquareContents[0][2] == playerChar )
					cout << "\n\tYOU WIN!\n";
				else
					cout << "\n\tYOU LOSE!\n";
				throw 3;
			}
			
			/* If all above conditions fail, i.e., if no one wins and if all vacancies are full, the game is a draw*/
			if( this->isFull() )
			{
				cout << "\n\tTHE GAME IS A DRAW!\n";
				throw 4;
			}
		}
		void play()
		{
			cout << *this;
			
			if(!this->isFull())
			{
				this->takePlayerChoice();
				this->updateGrid(playerChar);
				this->checkVictory();
			}
			
			if(!this->isFull())
			{
				this->takeComputerChoice();
				this->updateGrid(computerChar);
				this->checkVictory();
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
	try
	{
		while(!t.isFull())
			t.play();
		cout << t;
	}
	catch(int)
	{
		cout << t;
	}		
	return 0;
}
