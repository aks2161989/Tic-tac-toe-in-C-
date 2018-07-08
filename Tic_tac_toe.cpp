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
		enum chosenDifficulty { EASY, MEDIUM, IMPOSSIBLE };
		chosenDifficulty m_Difficulty;
		vector<int> playerMovesOuterIndexes;
		vector<int> playerMovesInnerIndexes;
			
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
				int randomIndex = getRandomNumber( 0, allVacanciesOuterIndexes.size()-1 );
				mSquareContents[ allVacanciesOuterIndexes[randomIndex] ][ allVacanciesInnerIndexes[randomIndex] ] = computerChar;
				return true;
			}
			return false;		
		}
		bool blockWinningStrategy()
		{
			for(int outer = 0; outer < 3; outer++) // First scan the grid to find out how many moves the player has made and save these moves in a vector
			{
				for(int inner = 0; inner < 3; inner++)
				{
					if(mSquareContents[outer][inner] == playerChar)
					{
						playerMovesOuterIndexes.push_back(outer);
						playerMovesInnerIndexes.push_back(inner);
					}	
				}
			}
			
			for(int index = 0; index < playerMovesOuterIndexes.size(); index++) // Scan the vector and remove any moves that have been stored repeatedly
			{
				for(int compareWith = index + 1; compareWith < playerMovesOuterIndexes.size(); compareWith++)
				{
					if(playerMovesOuterIndexes[index] == playerMovesOuterIndexes[compareWith] &&
					   playerMovesInnerIndexes[index] == playerMovesInnerIndexes[compareWith])
					   {
						playerMovesOuterIndexes.erase( playerMovesOuterIndexes.begin() +compareWith );
						playerMovesInnerIndexes.erase( playerMovesInnerIndexes.begin() +compareWith );
					   }
				}
			}
					
			if(playerMovesOuterIndexes.size() > 2) //If more than 2 moves have been made by player, he/she cannot implement strategy anymore, or his strategy cannot be blocked anymore, so exit this function
			{
				playerMovesOuterIndexes.clear();
				playerMovesInnerIndexes.clear();
				return false;
			}
			
			/* STRATEGY ONE: When player first move is the central square */
			if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 1 )
			{/* If player has marked the central square, mark a corner square... */
				int cornerSquares[][2] = { {0, 0}, {2, 2}, {0, 2}, {2, 0} };
				int randomIndex = getRandomNumber(0, 3);
				if(playerMovesOuterIndexes.size() == 1)
				{
					mSquareContents[ cornerSquares[randomIndex][0] ][ cornerSquares[randomIndex][1] ] = computerChar;
					return true;
				}	
				
				if(playerMovesOuterIndexes.size() == 2)
				{/* ...If player marks the other corner square of the diagonal in the next turn, mark any remaining corner square */
					if( (playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 2) && 
					    mSquareContents[0][0] == computerChar )
					{
						randomIndex = getRandomNumber(2, 3);
						mSquareContents[ cornerSquares[randomIndex][0] ][ cornerSquares[randomIndex][1] ] = computerChar;
						return true;
					}
					if( playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 0 && mSquareContents[0][2] == computerChar )
					{
						randomIndex = getRandomNumber(0, 1);
						mSquareContents[ cornerSquares[randomIndex][0] ][ cornerSquares[randomIndex][1] ] = computerChar;
						return true;					
					}
					if( playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 0 && mSquareContents[2][2] == computerChar )
					{
						randomIndex = getRandomNumber(2, 3);
						mSquareContents[ cornerSquares[randomIndex][0] ][ cornerSquares[randomIndex][1] ] = computerChar;
						return true;					
					}
					if( playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 2 && mSquareContents[2][0] == computerChar )
					{
						randomIndex = getRandomNumber(0, 1);
						mSquareContents[ cornerSquares[randomIndex][0] ][ cornerSquares[randomIndex][1] ] = computerChar;					
						return true;
					}					
				}
			}
			
			/* STRATEGY TWO: When player's first move is a corner square */	 
			if(playerMovesOuterIndexes.size() == 1 && playerMovesOuterIndexes[0] != 1 && playerMovesInnerIndexes[0] != 1) // Corner squares cannot have 1 as an outer or inner index
			{/* If player has marked a corner square, mark the central square... */
				mSquareContents[1][1] = computerChar;
				return true;
			}
			else if( playerMovesOuterIndexes.size() == 2 && playerMovesOuterIndexes[1] != 1 && playerMovesInnerIndexes[1] != 1 && mSquareContents[1][1] == computerChar && playerMovesOuterIndexes[0] != 1 && playerMovesInnerIndexes[0] != 1)
			{/* ...If player marks diagonally opposite corner square in next turn, mark the central square of any side or... */
				int centralSquaresOfSides[][2] = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};
				int randomIndex = getRandomNumber(0, 3);
				mSquareContents[ centralSquaresOfSides[randomIndex][0] ][ centralSquaresOfSides[randomIndex][1] ] = computerChar;
				return true;  		
			}
			else if( playerMovesOuterIndexes.size() == 2 && mSquareContents[1][1] == computerChar && playerMovesOuterIndexes[0] != 1 && playerMovesInnerIndexes[0] != 1)
			{/* ...If player marks the central square of a side 2 rows or 2 columns away from player's first move, mark any remaining square of that side*/
				if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]+2 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]+1 ) // 2 rows down and one column right
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) // Is a vacancy available in the same row as player's second move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}
				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]-2 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]-1 ) // 2 rows up and 1 column left
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) //Is there a vacancy on the same row as player's second move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}

				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]+1 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]+2 ) // 1 row below and 2 columns right  
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's second move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				
				}				 
				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]-1 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]-2 ) // 1 row above and 2 columns left
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's second move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				
				}
				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]-1 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]+2 ) // 1 row above and 2 columns right 
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's second move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				
				}
				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]-2 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]+1 ) // 2 rows above and 1 column right
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) //Is there a vacancy on the same row as player's second move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				
				}
				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]+1 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]-2 ) // 1 row below and 2 columns left
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's second move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				
				}
				else if( playerMovesOuterIndexes[1] == playerMovesOuterIndexes[0]+2 && playerMovesInnerIndexes[1] == playerMovesInnerIndexes[0]-1 ) // 2 rows down and one column left
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) // Is a vacancy available in the same row as player's second move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[1] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}
												 										 									 				
			}
			/* STRATEGY THREE: When player's first move is central square of any side */
			if(playerMovesOuterIndexes.size() == 1)
			{/* If player has marked a central square of any side, mark the central square... */
				if( playerMovesOuterIndexes[0] == 0 && playerMovesInnerIndexes[0] == 1 )
				{
					mSquareContents[1][1] = computerChar;
					return true;
				}	
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 0 )
				{
					mSquareContents[1][1] = computerChar;
					return true;
				}	
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 2 )
				{
					mSquareContents[1][1] = computerChar;
					return true;
				}	
				else if( playerMovesOuterIndexes[0] == 2 && playerMovesInnerIndexes[0] == 1 )
				{
					mSquareContents[1][1] = computerChar;
					return true;
				}	
			}
			else if(playerMovesOuterIndexes.size() == 2)
			{/* ...and if player next marks a central square of a side making an L shape with his/her earlier move, mark the corner square between player's two moves OR... */
				if( playerMovesOuterIndexes[0] == 0 && playerMovesInnerIndexes[0] == 1 &&
				playerMovesOuterIndexes[1] == 1 && playerMovesInnerIndexes[1] == 0 )
				{
					mSquareContents[0][0] = computerChar;
					return true;
				}
				else if( playerMovesOuterIndexes[0] == 0 && playerMovesInnerIndexes[0] == 1 &&
				playerMovesOuterIndexes[1] == 1 && playerMovesInnerIndexes[1] == 2 )
				{
					mSquareContents[0][2] = computerChar;
					return true;
				}					
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 0 &&
				playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 1 )
				{
					mSquareContents[0][0] = computerChar;
					return true;
				}
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 0 &&
				playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 1 )
				{
					mSquareContents[2][0] = computerChar;
					return true;
				}					
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 2 && 
				playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 1 )
				{
					mSquareContents[0][2] = computerChar;
					return true;
				}
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 2 && 
				playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 1 )
				{
					mSquareContents[2][2] = computerChar;
					return true;
				}					
				else if( playerMovesOuterIndexes[0] == 2 && playerMovesInnerIndexes[0] == 1 && 
				playerMovesOuterIndexes[1] == 1 && playerMovesInnerIndexes[1] == 0 )
				{
					mSquareContents[2][0] = computerChar;
					return true;
				}
				else if( playerMovesOuterIndexes[0] == 2 && playerMovesInnerIndexes[0] == 1 && 
				playerMovesOuterIndexes[1] == 1 && playerMovesInnerIndexes[1] == 2 )
				{
					mSquareContents[2][2] = computerChar;
					return true;
				}
				
				/* ...if player in second turn marks a corner square 2 rows or 2 columns away from the first move, mark any of the 2 vacant squares on same row/column of player's first move */
				if( playerMovesOuterIndexes[0] == 0 && playerMovesInnerIndexes[0] == 1 &&
				playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 0 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) //Is there a vacancy on the same row as player's first move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}
				else if( playerMovesOuterIndexes[0] == 0 && playerMovesInnerIndexes[0] == 1 &&
				playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 2 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) //Is there a vacancy on the same row as player's first move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}					
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 0 &&
				playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 2 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's first move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 0 &&
				playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 2 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's first move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}					
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 2 && 
				playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 0 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's first move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}
				else if( playerMovesOuterIndexes[0] == 1 && playerMovesInnerIndexes[0] == 2 && 
				playerMovesOuterIndexes[1] == 2 && playerMovesInnerIndexes[1] == 0 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < innerIndexes.size(); index++ ) //Is there a vacancy on the same column as player's first move?
					{
						if( innerIndexes[index] == playerMovesInnerIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same column the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}					
				else if( playerMovesOuterIndexes[0] == 2 && playerMovesInnerIndexes[0] == 1 && 
				playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 0 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) //Is there a vacancy on the same row as player's first move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}
				else if( playerMovesOuterIndexes[0] == 2 && playerMovesInnerIndexes[0] == 1 && 
				playerMovesOuterIndexes[1] == 0 && playerMovesInnerIndexes[1] == 2 )
				{
					vector<vector<int>> vacantSquaresOnSameLine;
					for( size_t index = 0; index < outerIndexes.size(); index++ ) //Is there a vacancy on the same row as player's first move?
					{
						if( outerIndexes[index] == playerMovesOuterIndexes[0] )
							vacantSquaresOnSameLine.push_back( { outerIndexes[index], innerIndexes[index] } );
					}
					if(	vacantSquaresOnSameLine.size() == 2 ) //If 2 vacancies exist on the same row the player has marked the central square of, mark a random square among them
					{
						int randomIndex = getRandomNumber(0, 1);
						mSquareContents[ vacantSquaresOnSameLine[randomIndex][0] ][ vacantSquaresOnSameLine[randomIndex][1] ] = computerChar; 
						return true;
					}
				}														
			}
			return false;
		}
		void levelImpossible()
		{
			/* Priority 1: If computer can win by a single move, make that move*/
			if( checkPotentialVictory( COMPUTER ) )
				return;
			
			/* Priority 2: If player can win by a single move, block that move*/
			if( checkPotentialVictory( PLAYER ) )
				return;
				
			/* Priority 3: If player has started\can start a winning strategy, block it*/
			if( blockWinningStrategy() )
				return;
			
			/* Priority 4: If computer can start\continue a winning strategy, do it*/			
//			applyWinningStrategy();
			
			/* Priority 5: If computer has marked a single square earlier, continue marking on the same line to win in future turns*/
			if( markOnSameLine() )
				return;
			
			/* Priority 6: Mark a random square*/
			int randomIndex = getRandomNumber( 0, outerIndexes.size() - 1 );
			mSquareContents[ outerIndexes[randomIndex] ][ innerIndexes[randomIndex] ] = computerChar;			
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
				levelEasy();
				break;
			case MEDIUM:
				levelMedium();
				break;
			case IMPOSSIBLE:
				levelImpossible();
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
	while( difficulty != 'E' && difficulty != 'e' && difficulty != 'M' && difficulty != 'm' && difficulty != 'I' && difficulty != 'i')
	{
		cout << "Choose a difficulty level (E-EASY, M-MEDIUM, I-IMPOSSIBLE): ";
		cin >> difficulty;
		cin.ignore(32767, '\n');
		if( cin.fail() )
		{
			cin.clear();
			cin.ignore(32767, '\n');
		}
	}
	if( difficulty == 'E'|| difficulty == 'e' )
		intDifficulty = 0;
	else if( difficulty == 'M' || difficulty == 'm' )
		intDifficulty = 1;
	else if( difficulty == 'I' || difficulty == 'i' )
		intDifficulty = 2;
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
