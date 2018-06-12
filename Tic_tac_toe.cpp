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
		int vacancyCount;
		vector<int> outerIndexes;
		vector<int> innerIndexes;
		enum playerName { COMPUTER, PLAYER };
		enum outcome { WIN, LOSE, DRAW }; //WIN: player wins, LOSE: player loses
		enum chosenDifficulty { EASY, MEDIUM };
		chosenDifficulty m_Difficulty;
			
	public:
		TicTacToe( int difficulty = 1 )
		: m_Difficulty( static_cast<chosenDifficulty>(difficulty) )
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
		void countVacancies()
		{
			outerIndexes.clear();
			innerIndexes.clear();
			vacancyCount = 0;// Number of vacant squares in grid
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
		}
		bool checkPotentialVictory(playerName pn) //Blocks player from winning in next turn OR makes computer win in next turn
		{
			char currentChar;
			char temp;
			
			if (pn == COMPUTER)
				currentChar = computerChar;
			else
				currentChar = playerChar;
				
			for (int index = 0; index < outerIndexes.size(); index++)
			{
				temp = mSquareContents[ outerIndexes[index] ] [ innerIndexes[index] ];
				mSquareContents[ outerIndexes[index] ] [ innerIndexes[index] ] = currentChar;
				switch( checkVictory() )
				{
					case WIN:
						mSquareContents[ outerIndexes[index] ] [ innerIndexes[index] ] = computerChar;
						return true;
					case LOSE:
						return true; // If the marked square results in a win or loss, let the marked square be... 
					default:
						mSquareContents[ outerIndexes[index] ] [ innerIndexes[index] ] = temp; // ...else reverse the change
				}
			}
			return false;			
		}
		bool markOnSameLine()
		{
			vector<int> allVacanciesOuterIndexes; // All vacancies of all available lines(outer indexes)
			vector<int> allVacanciesInnerIndexes; // All vacancies of all available lines(inner indexes)
			int earlierMarked[2] {100, 100};
			char temp1, temp2;
			for(int outer = 0; outer < 3; outer++)
			{
				for(int inner = 0; inner < 3; inner++)
				{
					if( mSquareContents[outer][inner] == computerChar )
					{
						earlierMarked[0] = outer; //Save the index of square marked earlier by the computer
						earlierMarked[1] = inner;
						break;
					}
				}
				if(earlierMarked[0] != 100) break;
			}
			
			for(int outer = 0; outer < outerIndexes.size(); outer++) 
			{
				for(int compareWith = outer+1; compareWith < outerIndexes.size(); compareWith++)
				{
					if(outerIndexes[outer] == outerIndexes[compareWith] && outerIndexes[outer] == earlierMarked[0]) //Horizontal vacancies from left to right
					{
							cout << "A\n";
							allVacanciesOuterIndexes.push_back(outerIndexes[outer]);
							allVacanciesInnerIndexes.push_back(innerIndexes[outer]);
							allVacanciesOuterIndexes.push_back(outerIndexes[compareWith]);
							allVacanciesInnerIndexes.push_back(innerIndexes[compareWith]);
					}
					else if(outerIndexes[compareWith] == outerIndexes[outer]+1 && outerIndexes[compareWith] == earlierMarked[0]+2)// Vertical and diagonal vacancies from top to bottom
					{
							temp1 = mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ];
							temp2 = mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ];
							mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ] = 
							mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ] = computerChar;
							if( checkVictory() == LOSE )
							{
								cout << "B\n";
								allVacanciesOuterIndexes.push_back(outerIndexes[outer]);
								allVacanciesInnerIndexes.push_back(innerIndexes[outer]);
								allVacanciesOuterIndexes.push_back(outerIndexes[compareWith]);
								allVacanciesInnerIndexes.push_back(innerIndexes[compareWith]);								
							}
							mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ] = temp1;
							mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ] = temp2;
					}
					else if(outerIndexes[compareWith] == outerIndexes[outer]+2 && outerIndexes[compareWith] == earlierMarked[0]+1)// Vertical and diagonal vacancies from top to bottom with earlied marked square in middle
					{
							temp1 = mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ];
							temp2 = mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ];
							mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ] = 
							mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ] = computerChar;
							if( checkVictory() == LOSE )
							{
								cout << "C\n";
								allVacanciesOuterIndexes.push_back(outerIndexes[outer]);
								allVacanciesInnerIndexes.push_back(innerIndexes[outer]);
								allVacanciesOuterIndexes.push_back(outerIndexes[compareWith]);
								allVacanciesInnerIndexes.push_back(innerIndexes[compareWith]);								
							}
							mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ] = temp1;
							mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ] = temp2;
					}
				}
					
			}
			for(int outer = outerIndexes.size()-1; outer >= 0 ; outer--) 
			{
				for(int compareWith = outer-1; compareWith >= 0; compareWith--)
				{
					if(outerIndexes[compareWith] == outerIndexes[outer]-1 && outerIndexes[compareWith] == earlierMarked[0]-2)// Vertical and diagonal vacancies from bottom to top
					{
							temp1 = mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ];
							temp2 = mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ];
							mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ] = 
							mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ] = computerChar;
							if( checkVictory() == LOSE )
							{
								cout << "E\n";
								allVacanciesOuterIndexes.push_back(outerIndexes[outer]);
								allVacanciesInnerIndexes.push_back(innerIndexes[outer]);
								allVacanciesOuterIndexes.push_back(outerIndexes[compareWith]);
								allVacanciesInnerIndexes.push_back(innerIndexes[compareWith]);
							}
							mSquareContents[ outerIndexes[outer] ][ innerIndexes[outer] ] = temp1;
							mSquareContents[ outerIndexes[compareWith] ][ innerIndexes[compareWith] ] = temp2;
					}
				}					
			}
			if( !allVacanciesOuterIndexes.empty() )
			{
				cout << "All vacancies on all lines are: ";
				for(int i = 0; i < allVacanciesOuterIndexes.size(); i++)
					cout << "(" << allVacanciesOuterIndexes[i] << ", " << allVacanciesInnerIndexes[i] << ") ";
				cout << '\n';
				
				int randomIndex = getRandomNumber( 0, allVacanciesOuterIndexes.size()-1 );
				mSquareContents[ allVacanciesOuterIndexes[randomIndex] ][ allVacanciesInnerIndexes[randomIndex] ] = computerChar;
				return true;
			}
			return false;		
		}
		void levelMedium()
		{
			/*First check if computer can win in next turn*/
			if (checkPotentialVictory( COMPUTER ))
				return;
			/*If not, check if player can win in next turn. If yes, block his move*/
			if (checkPotentialVictory( PLAYER ))
				return;
			
			/*If computer has played before, mark a square on the same line so that line can be completed in future turns*/
			if( markOnSameLine() )
				return;
			
			/*If none of the above is applicable, just mark a square randomly*/	
			cout << "RANDOM\n";
			cout << "Total vacancies are: ";
			for(int i = 0; i < outerIndexes.size(); i++)
				cout << "(" << outerIndexes[i] << ", " << innerIndexes[i] << ") ";
			cout << '\n';
			
			int randomIndex = getRandomNumber(0, outerIndexes.size() - 1);
			mSquareContents[ outerIndexes[randomIndex] ][ innerIndexes[randomIndex] ] = computerChar;
		}
		void levelEasy() // 'Easy' difficulty
		{
			int randomIndex = getRandomNumber ( 0, outerIndexes.size() - 1);
			mSquareContents[ outerIndexes[randomIndex] ] [ innerIndexes[randomIndex] ] = computerChar;
		}
		void takeComputerChoice()
		{
			this->countVacancies();
			
			switch( m_Difficulty )
			{
			case EASY:
				this->levelEasy();
				break;
			case MEDIUM:
				this->levelMedium();
			}	 	
		}
		outcome checkVictory() 
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
					{
						return WIN;
					}
					else
					{
						return LOSE;
					}
				}
			}
			
			for(int innerIndex = 0; innerIndex < 3; innerIndex++) //Checking victory condition 2
			{
				if( mSquareContents[0][innerIndex] == mSquareContents[1][innerIndex] && mSquareContents[1][innerIndex] == mSquareContents[2][innerIndex] )
				{
					if( mSquareContents[0][innerIndex] == playerChar )
					{
						return WIN;
					}
					else
					{
						return LOSE;
					}
				}
			}
			
			if( mSquareContents[0][0] == mSquareContents[1][1] && mSquareContents[1][1] == mSquareContents[2][2] ) //Checking victory condition 3
			{
				if( mSquareContents[0][0] == playerChar )
				{
						return WIN;
				}
				else
				{
						return LOSE;
				}
			}
			
			if( mSquareContents[0][2] == mSquareContents[1][1] && mSquareContents[1][1] == mSquareContents[2][0] ) //Checking victory condition 4
			{
				if( mSquareContents[0][2] == playerChar )
				{
						return WIN;
				}
				else
				{
						return LOSE;
				}
			}
			
			/* If all above conditions fail, i.e., if no one wins and if all vacancies are full, the game is a draw*/
			if( this->isFull() )
			{
				return DRAW;
			}
		}
		void play()
		{
			cout << *this;
			
			if(!this->isFull()) //Player's move
			{
				this->takePlayerChoice();
				this->updateGrid(playerChar);
				switch( this->checkVictory() )
				{
					case WIN:
						cout << "\n\tYOU WIN!\n";
						throw 0;
					case LOSE:
						cout << "\n\tYOU LOSE!\n";
						throw 0;
					case DRAW:
						cout << "\n\tTHE GAME IS A DRAW!\n";
						throw 0;
				}
				
			}
			
			if(!this->isFull()) //Computer's move
			{
				this->takeComputerChoice();
				this->updateGrid(computerChar);
				switch( this->checkVictory() )
				{
					case WIN:
						cout << "\n\tYOU WIN!\n";
						throw 0;
					case LOSE:
						cout << "\n\tYOU LOSE!\n";
						throw 0;
					case DRAW:
						cout << "\n\tTHE GAME IS A DRAW!\n";
						throw 0;
				}
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
	srand(static_cast<unsigned int>(time(0)));
	char difficulty;
	int intDifficulty;
	while( difficulty != 'E' && difficulty != 'e' && difficulty != 'M' && difficulty != 'm')
	{
		cout << "Choose a difficulty level (E-EASY, M-MEDIUM): ";
		cin >> difficulty;
		cin.ignore(32767, '\n');
		if( cin.fail() )
		{
			cin.clear();
			cin.ignore(32767, '\n');
		}
	}
	if( difficulty == 'E'|| difficulty == 'e')
		intDifficulty = 0;
	else if( difficulty == 'M' || difficulty == 'm')
		intDifficulty = 1;
	TicTacToe t ( intDifficulty );
		
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
