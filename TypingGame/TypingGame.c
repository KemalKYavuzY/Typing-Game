//Typing Game by Kemal Yavuz

//gcc -o FinalProject FinalProject.c -lncurses
//./FinalProject wordList.txt

/////////////////////////////////
//C Standard Library Functions//
///////////////////////////////
#include <stdio.h>	   
#include <string.h>    
#include <stdlib.h>   
#include <ncurses.h>
#include <time.h>
#include <ctype.h>
#include <math.h> 
#include <windows.h> // for Sleep function
#define MAXWORDS 10000
#define ROW 20
#define COL 80

//////////////////////
//Struct Definition//
////////////////////
struct Words{ //struct of the words that is on the board
	char Word[50];
};

/////////////////////////////////////
//User Defined Functions Prototype//
///////////////////////////////////
int ReadWords(char* WL[MAXWORDS], char* FileName);

void SpaceDeleter(char* Word);

void DrawBoard(void);

void GenerateBoard(void);

void EndLine(void);

void GameOver(void);

/////////////////////
//Global Variables//
///////////////////
FILE* fp; // File declaration 

char* PlayingBoard[10*ROW][10*COL]; // Created the board to keep track of the positions of the remaining words

//////////////////
//Main Function//
////////////////
int main(int argc, char* argv[]){
	char* WordList[MAXWORDS];
	int WordCount, Difficulty, RandSize = 0, LOSE = 0, NumWord, Score = 0;
	char UserInput[50], AddWord[50];
	
	WordCount = ReadWords(WordList, argv[1]); // Function call to get the number of the words and read the words
	printf("\nWelcome to the Typing Game!\nIt is recommended to set the screen size to (80x40) before starting to play.\n");
	printf("Enter (R) whenever you are ready: "); // Asks if the user is ready or not
	scanf("%s", UserInput);
	
	while (strcmp(UserInput, "R") != 0){ // Conditional until the user enters the input in correct format
		printf("\nInvalid input! Please enter a single capital letter (R): ");
		scanf("%s", UserInput);
	}
	
	printf("\nEvery second a random word will appear on the screen.\n");
	printf("You need to type the appeared words before it touches the bottom of the screen.\n");
	printf("\nYou currently have %d words in your word list.\n", WordCount);
	printf("\nPlease enter (W) to add a word to the wordlist or (P) to play: ");
	scanf("%s", UserInput);

	do {
		if ((strcmp(UserInput, "W") != 0) && (strcmp(UserInput, "P") != 0)){ // Conditional until the user enters the input in correct format
			printf("\nInvalid input! Please enter a single capital letter (W or P): ");
			scanf("%s", UserInput);
		} else if (strcmp(UserInput, "W") == 0){ // Conditional for adding words
			fp = fopen(argv[1], "r+"); // Opens and writes and reads the text file

			printf("\nPlease enter the word that you want to add the wordlist: ");
			scanf("%s", AddWord);
			WordList[WordCount] = AddWord; // Adds the new word to the word list array
			
			for (int i = 0; i < WordCount; i++){ // writes all the words that were in the text file before
				fprintf(fp,"%s", WordList[i]);
			}
			
			fprintf(fp,"\n%s", AddWord); // writes the new word to the text file
			WordCount += 1;
			printf("\nThere are %d words in the word list now.\n", WordCount);
				
			fclose(fp);
			printf("\nPlease enter (W) to add a word to the wordlist or (P) to play: ");
			scanf("%s", UserInput);
		} else if (strcmp(UserInput, "P") == 0){ // conditional for playing the game
			printf("\nPlease set the difficulty by entering the minimum word length: ");
			scanf("%d", &Difficulty); // Sets difficulty based on length of words
			struct Words SetWordLength[WordCount];
			fp = fopen(argv[1], "r");
			
			for (int i = 0; i < WordCount; i++){
				fscanf(fp,"%s", WordList[i]); // Reads the words again to not have unnecessary empty spaces
				
				if (strlen(WordList[i]) >=  Difficulty){ //Finds the words that is longer than the difficulty
					strcpy(SetWordLength[RandSize].Word,WordList[i]);
					RandSize += 1; // number of words that is longer than difficulty
				}
			}
			
			fclose(fp);
			
			// Losing conditions
			if(Difficulty > 40){
				LOSE = 3;
				break;
			} else if (RandSize == 0){
				LOSE = 2;
				break;
			} else if (RandSize <= 10){
				LOSE = 4;
				break;
			}
			
			srand(time(NULL)); // random number
			struct Words SelectedWord[RandSize];
			int Y[RandSize], X[RandSize], K = 1; // coordinates
			
			printf("Enter (R) whenever you are ready: ");
			scanf("%s", UserInput);
			while (strcmp(UserInput, "R") != 0){ 
			printf("\nInvalid input! Please enter a single capital letter (R): ");
			scanf("%s", UserInput);
			}
			for (int i = 3; i > 0; i--){ // Countdown before the game starts
			printf("\n\n\n\n\t\t\t\t    %d\n\n", i);
			Sleep(1000); // 1 second delay (<windos.h> library
			}
			
			time_t start, end; // time variables
			start = time(NULL);
			Sleep(1000); // 1 second delay so the following loop iterates once on the first iteration
			end = time(NULL);
			
			initscr(); // creates the screen
			// generates and prints the playing board
			GenerateBoard();
			DrawBoard();
	
			while (LOSE == 0){ // Proccess of the game as long as the player has not lost
				for (int t = 0; t < difftime(end, start); t++){ // iterates the number of difftime (difftime = the time that has been passed)
					NumWord = rand() % RandSize; //random word 
					X[K] = rand() % (COL-strlen(SetWordLength[NumWord].Word)); // random x coordinate
					
					// if statements to make the words not touch the left and right walls
					if (X[K] < strlen(SetWordLength[NumWord].Word)){
						X[K] = strlen(SetWordLength[NumWord].Word);
					} else if (X[K] + strlen(SetWordLength[NumWord].Word) > COL - 1){
						X[K] = COL - 2 * strlen(SetWordLength[NumWord].Word);
					}
					
					strcpy(SelectedWord[K].Word, SetWordLength[NumWord].Word);
					Y[K] = K; // Y coordinate
					
					for (int i = 0; i < K; i++){ // The for loop that prints all the words that has been created
						for (int j = 1; j < 79; j++){ // Trace deleter
							PlayingBoard[Y[K-i]][j] = "";
							mvprintw(Y[K-i], j, "%c", ' ');
							refresh();
						}
						
						if (strcmp(SelectedWord[i+1].Word, "") == 0){ // for loop skips the deleted word
							continue;
						}
						
						PlayingBoard[Y[K-i]][X[i+1]] = SelectedWord[i+1].Word; // Generate the word in the board so we can compare it in the future
						mvprintw(Y[K-i], X[i+1], "%s", SelectedWord[i+1].Word);
						refresh();
					}
					
					for (int i = 1; i < COL; i++){ //Lose or not for loop
						if (LOSE == 1){
							break;
						}
						
						for (int k = -1; k < ROW + 20; k++){ // checks if there is a word under the game over line
							if((PlayingBoard[ROW+k][i] != "_" && PlayingBoard[ROW+k][i] != "")){
								LOSE = 1;
								GameOver(); 
								Sleep(2000); // 2 second delay to let the user see the losing condition
								break;
							} else {
								LOSE = 0; // if any of this does not happen the game continues
							}
						}
					}
					
					if (LOSE == 1){
						break;
					}
					
					K += 1; // next array
					Score += 1; // the time has been passed = score 
				}	
			
				if (LOSE == 1){
					break;
				}
				
				EndLine(); // if the end line is deleted it prints the endline again each iteration
				start = time(NULL); // beginning time
				mvscanw((ROW + 2), 11, "%s", UserInput);
				refresh();
				
				for (int j = 11; j < 79; j++){ // cleanes the user input line
					mvprintw(ROW+2, j, "%c", ' ');
					refresh();
				}
				
				end = time(NULL); // the amount of time it took user to input
				GenerateBoard();
				
				for (int i = 0; i < K; i++){
						if (strcmp(UserInput, SelectedWord[K-i].Word) == 0){ // If the user input and a word on the board is equal it deletes the word
							for (int j = 1; j < 79; j++){
							PlayingBoard[Y[K-i]][j] = "";
							mvprintw(Y[K-i], j, "%c", ' ');
							refresh();
							strcpy(SelectedWord[K-i].Word, "");
						}
					}
				}
				
				DrawBoard();	
			}
		}
		
	} while (LOSE == 0);
	
	endwin(); // closing the screen
	
	// Losing conditions outputs
	if(LOSE == 1){
		printf("\n\n\n\n\t\t\t\tGAME OVER!\n\n\t\t\tYou Survived %d Seconds\n\n\n\n", Score);
	} else if (LOSE == 2){
		printf("\n\n\nThere are no words in the wordlist that is longer than %d.\n\nAdd words to the word list in this length to play the game with this difficulty.\n\n\n\n", Difficulty);
	} else if (LOSE == 3){
		printf("\n\nOur Program does not support %d word length.\n\nMaximum word lenght our program supports is 40.\n\nPlease enter a number that is lower than 40.\n\n\n\n", Difficulty);
	} else if (LOSE == 4){
		printf("\n\n\nThere are not enough words in this difficulty.\n\nAdd words to the word list in this length to play the game with this difficulty.\n\n\n\n");
	}
	
	return 0;
}

///////////////////////////////////////
//User Defined Functions' Definition//
/////////////////////////////////////

void SpaceDeleter(char* Word){ 
	int WordLength = strlen(Word), X = WordLength;
	if (WordLength == 0) return;
	
	while (isspace(Word[X]) && (X >= 0)){
		Word[X] = '\0';
		X -= 1;
	}
}

int ReadWords(char* WL[MAXWORDS], char* FileName){ // read words form the word list and counts them
	int Read = 0;
	char Line[50];
	char *p;
	fp = fopen(FileName, "r");
	
	while (!feof(fp)){
		p = fgets(Line, 50, fp);
		
		if (p != NULL){
			SpaceDeleter(Line);
			WL[Read] = (char *)malloc(strlen(Line) + 1);
			strcpy(WL[Read], Line);
			Read++;
		}
	}
	
	fclose(fp);
	return Read;
}

void GenerateBoard(void){  // Generates Board
	for (int i = 0; i < ROW; i++){
		PlayingBoard[i][0] = "|";
		PlayingBoard[i][COL - 1] = "|";
	}
	
	for (int j = 0; j < COL; j++){
		for (int k = -1; k < ROW + 20; k++){
		PlayingBoard[0][j] = "_";
		PlayingBoard[ROW+k][j] = "_";
		}
	}
}

void DrawBoard(void){ // Board print
	for (int i = 0; i < ROW; i++){
		mvprintw(i, 0, "%c", '|');
		refresh();
		mvprintw(i, COL - 1, "%c", '|');
		refresh();
	}
	
	for (int j = 0; j < COL; j++){
		mvprintw(0, j, "%c", '_');
		refresh();
		mvprintw(ROW - 1, j, "%c", '_');
		refresh();
		mvprintw(ROW, j, "%c", '_');
		refresh();
	}
	
	mvprintw(ROW + 2, 0, "%s", "Type Here: ");
	refresh();

}

void EndLine(void){ // Endline print
	for (int j = 0; j < COL; j++){
		mvprintw(ROW - 1, j, "%c", '_');
		refresh();
		mvprintw(ROW, j, "%c", '_');
		refresh();
	}
	
	mvprintw(ROW + 2, 0, "%s", "Type Here: ");
	refresh();
}

void GameOver(void){ // Game End print
	for (int j = 0; j < 2; j++){
		mvprintw(ROW - 1, j, "%c", '_');
		refresh();
		mvprintw(ROW, j, "%c", '_');
		refresh();
	}
	
	for (int j = COL; j > COL - 2; j--){
		mvprintw(ROW - 1, j, "%c", '_');
		refresh();
		mvprintw(ROW, j, "%c", '_');
		refresh();
	}
	
	mvprintw(ROW + 2, 0, "%s", "\t\t\t\tGAME OVER");
	refresh();
}