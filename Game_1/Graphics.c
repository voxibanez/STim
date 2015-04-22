#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include "Stim.h"
#include <conio.h>
#include <dos.h>
#include <mmsystem.h>  /* multimedia functions (such as MIDI) for Windows */
#include <sys/types.h>
#include <errno.h>



void titleScreen();
void setWindow();
void battleSequence(Enemy en,Player user);

void updateScreen();

void addAnimation(AnimationPtr animate, int x, int y);

void updatePlayerPosition(Player user);

void updateEnemyPosition(Enemy en,Player user);

void warriornextlevel(Player user);

void titleMusic();

char** loadArt(char* filename);

AnimationPtr initAnimation(frames, x, y);

char screen[20][80] = {{176}};
int playerPosition[2][2] = { 0 };
int screenCounter;
char ground;
char*** characters;
char** playerSprite;
Enemy* enemies;


HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.
AnimationPtr boxes;

int main(int argc, char* argv[]){
	int i, j, k;
	Player mainChar = malloc(sizeof(PlayerSize));
	char key_code;
	boxes = initAnimation(80, 20, 80);
	characters = malloc(5 * sizeof(char*));
	characters[0] = loadArt("Unknown.txt");
	characters[1] = loadArt("Skeleton.txt");
	characters[2] = loadArt("Goblin.txt");
	characters[3] = loadArt("Orc.txt");
	characters[4] = loadArt("Troll.txt");
	playerSprite = loadArt("Player.txt");

	srand(time(NULL));

	enemies = malloc(sizeof(Enemy) * 3);
	for (i = 0; i < 3; i++)
		enemies[i] = NULL;
	

	for (i = 0; i < 79; i++)
	{
		for (j = 0; j < i; j++){
			if (i >= 10)
			{
				boxes->frames[i][19][j] = 219;
				boxes->frames[i][0][j] = 219;
			}
			else{
				boxes->frames[i][10 + i][j] = 219;
				boxes->frames[i][10 - i][j] = 219;
			}
				
				
		}
		if (i < 10)
		{
		for (k = 10 - i; k <= 10 + i; k++)
			boxes->frames[i][k][j] = 219;
		}
		else{
			for (k = 0; k < 19; k++)
				boxes->frames[i][k][j] = 219;
			boxes->frames[i][k][j] = 219;
		}
			
	
	
	}
	for (i = 0; i < 19; i++){
		for (j = 0; j < 79; j++)
			boxes->frames[79][i][j] = ' ';
		boxes->speed = 1;
	}
	

	setWindow();
	system("cls");
	getch();
	titleScreen();
	system("cls");
	srand(time(NULL));
	ground = 176;


	for (i = 0; i < 20; i++)
		screen[i][79] = NULL;

	for (i = 0; i < 20; i++){
		for (j = 0; j < 79; j++)
			screen[i][j] = ground;
	}



	updateEnemyPosition(enemies,mainChar);

	warriornextlevel(mainChar);

	updateScreen();

	while (1){
		if (kbhit()){
			key_code = getch();
			if (key_code == 'w')
				mainChar->Position[1][0] --;
			if (key_code == 's')
				mainChar->Position[1][0] ++;

			if (key_code == 'a')
				mainChar->Position[1][1] --;
			if (key_code == 'd')
				mainChar->Position[1][1] ++;
			if (key_code == 'w' || key_code == 'a' || key_code == 's' || key_code == 'd'){
				updatePlayerPosition(mainChar);
				updateEnemyPosition(enemies,mainChar);
			}
			

			updateScreen();

			key_code = NULL;
			
			
			
		}
		else
			continue;
	}
	
}

void titleScreen(){
	int i,j;
	char animation[51] = { ' ' };

	animation[50] = NULL;

	for (i = 0; i < 20; i++){
		animation[25 - i] = '-';
		animation[25 + i] = '-';
		printf("\t\t\tSTim\n");
		for (j = 0; j < 50; j++)
			printf("%c", animation[j]);
		printf("\n\n\n");
		Sleep(20);
		if (i < 19)
		system("cls");
	}
	printf("\t\tPress Any Key To Continue...\n");
	getch();
	system("cls");
}

void setWindow()
{
	// Set up the handles for reading/writing:
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	// Change the window title:
	
	// Set up the required window size:
	SMALL_RECT windowSize = { 0, 0, 500, 400 };
	SetConsoleWindowInfo(wHnd, 1, &windowSize);
	// Change the console window size:
	// Create a COORD to hold the buffer size:
	COORD bufferSize = { 70, 70 };
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	MoveWindow(GetConsoleWindow(), 100, 100, 700, 500, TRUE);
	// Exit
	SetConsoleTitle(L"STim");
	return 0;
}

void updateScreen(){
	system("cls");
	
	for (screenCounter = 0; screenCounter < 20; screenCounter ++)
		printf("%.79s\n", screen[screenCounter]);

}

void updatePlayerPosition(Player user){
	int i;

	if (user->Position[1][0] < 0)
		user->Position[1][0] = 0;
	if (user->Position[1][1] < 0)
		user->Position[1][1] = 0;

	if (user->Position[1][0] > 20)
		user->Position[1][0] = 19;
	if (user->Position[1][1] > 78)
		user->Position[1][1] = 78;

	if (rand() % 50 == 0)
		for (i = 0; i < 3; i++){
			if (enemies[i] == NULL){
				enemies[i] = lv1_pick_monster();
				break;
			}
				
		}
			

	screen[user->Position[0][0]][user->Position[0][1]] = ground;
	screen[user->Position[1][0]][user->Position[1][1]] = 219;

	user->Position[0][0] = user->Position[1][0];
	user->Position[0][1] = user->Position[1][1];


}

void updateEnemyPosition(Enemy* en,Player user){
	int i;
	for (i = 0; i<3; i++){
		if (en[i] != NULL){
			if ((rand() % 2) == 1){
				if (user->Position[1][0] > en[i]->Position[1][0])
					en[i]->Position[1][0] ++;
				else if (user->Position[1][0] < en[i]->Position[1][0])
					en[i]->Position[1][0] --;
			}
			else{
				if (user->Position[1][1] > en[i]->Position[1][1])
					en[i]->Position[1][1] ++;
				else if (user->Position[1][1] < en[i]->Position[1][1])
					en[i]->Position[1][1] --;
			}


			if (en[i]->Position[1][0] < 0)
				en[i]->Position[1][0] = 0;
			if (en[i]->Position[1][1] < 0)
				en[i]->Position[1][1] = 0;

			if (en[i]->Position[1][0] > 20)
				en[i]->Position[1][0] = 19;
			if (en[i]->Position[1][1] > 78)
				en[i]->Position[1][1] = 78;




			screen[en[i]->Position[0][0]][en[i]->Position[0][1]] = ground;
			screen[en[i]->Position[1][0]][en[i]->Position[1][1]] = 233;

			if (user->Position[1][0] == en[i]->Position[1][0] && user->Position[1][1] == en[i]->Position[1][1]){
				ground = ' ';
				addAnimation(boxes, 0, 0);
				battleSequence(en[i], user);
			}


			en[i]->Position[0][0] = en[i]->Position[1][0];
			en[i]->Position[0][1] = en[i]->Position[1][1];
		}
	}
	


}


void warriornextlevel(Player user)
{
	user->MAXHP = 11;
	user->ATK = 11;
	user->DEF = 13;
	user->MATK = 6;
	user->MDEF = 12;
	user->ACC = 70;
	user->LCK = 8;

	user->Position[0][0] = 15;
	user->Position[0][1] = 15;
	user->Position[1][0] = 15;
	user->Position[1][1] = 15;
	//issword = 0, isrune = 0, ismace = 0, isdagger = 0, isarmor = 0;
	//your_main_weapon();
	//your_off_weapon();
	//your_armor();
	//DMG = calc_dmg();
	//isdual();
	//iswarrior = 1;
}


void addAnimation(AnimationPtr animate,int x,int y){
	int i,j,k;
	for (i = 0; i < animate->frameCount; i++){
		for (j = 0; j < animate->sizeX; j++){
			for (k = 0; k < animate->sizeY; k++){
				if (i > 0 && animate->frames[i - 1][j][k] != NULL)
					screen[x + j][y + k] = ground;
				if (animate->frames[i][j][k] != NULL)
					screen[x + j][y + k] = animate->frames[i][j][k];
				
			}
				
		}
		Sleep(animate->speed);
		updateScreen();
		printf("\nSize: %dx%d", sizeof(animate->frames[i]), sizeof(animate->frames[i][j]));
	}
	


		
}

AnimationPtr initAnimation(int frames, int x, int y)
{
	int i, j, k;
	AnimationPtr temp;

	temp = malloc(sizeof(Animation));

	temp->frames = malloc(frames * sizeof(char **));
	for (i = 0; i < frames; ++i)
	{
		temp->frames[i] = malloc(x * sizeof(char *));
		for (j = 0; j < x; ++j)
		{
			temp->frames[i][j] = malloc(sizeof(char)*y);

			for (k = 0; k < y; k++)
				temp->frames[i][j][k] = NULL;
		}

		
	}
	temp->frameCount = frames;

	temp->sizeX = x;
	temp->sizeY = y;
	return temp;
}

void battleSequence(Enemy en, Player user){
	int i,j,k;
	int enemyIndex;
	char temp[64];
	int offsetX = 0;
	int offsetY = 0;
	int maxOffset = 1;




	if (en->NAME == "Skeleton"){
		enemyIndex = 1;
		maxOffset = 38;
	}
	else if (en->NAME == "Goblin"){
		enemyIndex = 2;
		maxOffset = 20;
	}
	else if (en->NAME == "Orc"){
		enemyIndex = 3;
		maxOffset = 28;
	}
	else if (en->NAME == "Troll"){
		enemyIndex = 4;
		maxOffset = 20;
	}
	else{
	enemyIndex = 0;
	maxOffset = 40;
	}


		while (offsetX < maxOffset){
			for (i = 0; characters[enemyIndex][i] != NULL && i < 20; i++){
				for (j = 0; characters[enemyIndex][i][j] != NULL && i < 78; j++){

					screen[i][j + offsetX] = characters[enemyIndex][i][j];

				}

			}
			updateScreen();
			Sleep(20);
			if (offsetX > 0 && offsetX < maxOffset-1){
				for (i = 0; characters[enemyIndex][i] != NULL && i < 20; i++){
					for (j = 0; characters[enemyIndex][i][j] != NULL && i < 78; j++){

						screen[i][j + offsetX - 1] = ground;

					}
				}
				
			}
			

			offsetX++;
		}
			sprintf(temp, "A level %d %s appeared...", en->LEVEL, en->NAME);
			for (i = 0; i < strlen(temp); i++){
				screen[1][i + 3] = temp[i];
				updateScreen();
				Sleep(20);
			}
			Sleep(1000);

		for (i = 0; playerSprite[i] != NULL && i < 10; i++){
			for (j = 0; playerSprite[i][j] != NULL && i < 78; j++){

				screen[i+10][j] = playerSprite[i][j];
				
				}
			
			}
		updateScreen();

			getch();
	
			for (i = 0; i < strlen(temp); i++)
				screen[1][i + 3] = ground;
				updateScreen();

}

char** loadArt(char* filename){
	int i,j,k;
	char** temp;
	char tempChar[80];
	FILE* fp;

	

	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("Not");

	i = 0;
	while (fscanf(fp, "%s") != EOF)
		i++;

	rewind(fp);

	temp = malloc(sizeof(char)*20);


	j = 0;

	while (j < i){
		
		temp[j] = malloc(80 * sizeof(char *));
		temp[j][0] = NULL;
		fscanf(fp, "%78[^\n]", temp[j]);
		fscanf(fp, "%c");
		//fgets(temp[j], 78, fp);
		j++;
	}

	return temp;
}