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
void battleSequence(Enemy en, Player user);

void updateScreen();

void addAnimation(AnimationPtr animate, int x, int y);

void updatePlayerPosition(Player user);

void updateEnemyPosition(Enemy en, Player user);

void warriornextlevel(Player user);

void titleMusic();

char** loadArt(char* filename);

SalesMan addSalesman();

void moveSalesman(SalesMan storeman);

void menuGraphics(Player user);

AnimationPtr initAnimation(frames, x, y);
void initPlayer();
WeaponPtr initWeapon(char* name, double weaponModMin, double weaponModMax, double attackMod, double AccMod, int isPhysical, char* DESCRIPTION);

PotionPtr initPotion(char* NAME, int MAXHPRAISE, int HPRAISE, int ATKRAISE, int DEFRAISE, int MATKRAISE, int MDEFRAISE, int ACCRAISE, int LCKRAISE,char* DESCRIPTION);

void inventoryGraphics(Player user);

char screen[20][80] = { { 176 } };
int playerPosition[2][2] = { 0 };
int screenCounter;
char ground;
char*** characters;
char** playerSprite;
Enemy* enemies;
WeaponPtr* weapons;
PotionPtr* potions;
SalesMan storeman;

HANDLE wHnd;    // Handle to write to the console.
HANDLE rHnd;    // Handle to read from the console.
AnimationPtr boxes;

int main(int argc, char* argv[]){
	int i, j, k;
	Player mainChar = malloc(sizeof(PlayerSize));
	ItemPtr tempItem = malloc(sizeof(Item));
	char key_code;
	boxes = initAnimation(80, 20, 80);
	characters = malloc(5 * sizeof(char*));
	characters[0] = loadArt("Unknown.txt");
	characters[1] = loadArt("Skeleton.txt");
	characters[2] = loadArt("Goblin.txt");
	characters[3] = loadArt("Orc.txt");
	characters[4] = loadArt("Troll.txt");
	playerSprite = loadArt("Player.txt");
	storeman = NULL;
	mainChar->BATTLES = 1;

	


	weapons = malloc(sizeof(WeaponPtr*) * 5);
	weapons[0] = initWeapon("Empty", 0.0, 0.0, 1.0, 100, 1,"Nothing to see here");
	weapons[1] = initWeapon("Wooden Sword", 0.0, 0.7, 1.3, 90, 1,"The strongest of all wooden swords");
	weapons[2] = initWeapon("Fire Rune", 2.0, 0.8, 1.2, 70, 0,"HOT");
	weapons[3] = initWeapon("Wood Club", 3.0, 0.6, 1.4, 80, 1,"Big stick");
	weapons[4] = initWeapon("Chipped Dagger", -1.0, 0.9, 1.1, 70, 1,"Sucks to suck");

	potions = malloc(sizeof(PotionPtr*) * 1);
	potions[0] = initPotion("Potion",0,5,0,0,0,0,0,0,"A Simple Healing Potion");
	//PotionPtr initPotion(char* name, int MAXHPRAISE, int HPRAISE, int ATKRAISE, int DEFRAISE, int MATKRAISE, int MDEFRAISE, int ACCRAISE, int LCKRAISE);



	mainChar->weaponLeft = weapons[4];
	mainChar->weaponRight = weapons[0];

	tempItem->POTION = potions[0];
	tempItem->QUANTITY = 2;
	//addItem(mainChar, tempItem);

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

	character_select(mainChar);

	system("cls");
	printf("\n\n\n\n\t\t\t%s, Your journey begins here...\n", mainChar->NAME);
	Sleep(1000);
	system("cls");
	srand(time(NULL));
	ground = 176;

	


	for (i = 0; i < 20; i++)
		screen[i][79] = NULL;

	for (i = 0; i < 20; i++){
		for (j = 0; j < 79; j++)
			screen[i][j] = ground;
	}

	updateEnemyPosition(enemies, mainChar);

	warriornextlevel(mainChar);

	updateScreen();

	while (1){
		//getch();
		//if (kbhit()){
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
			updateEnemyPosition(enemies, mainChar);
			moveSalesman(storeman, mainChar);
			updateScreen();

		}
		if (key_code == 27)
			menuGraphics(mainChar);




		key_code = NULL;



		//}
		//else
		//continue;
	}

}

void titleScreen(){
	int i, j;
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

	for (screenCounter = 0; screenCounter < 20; screenCounter++)
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

	if (rand() % 20 == 0){
		for (i = 0; i < 3; i++){
			if (enemies[i] == NULL){
				enemies[i] = lv1_pick_monster();
				break;
			}

		}
	}
	if (rand() % 100 == 0 && storeman == NULL)
		storeman = addSalesman();


	screen[user->Position[0][0]][user->Position[0][1]] = ground;
	screen[user->Position[1][0]][user->Position[1][1]] = 219;

	user->Position[0][0] = user->Position[1][0];
	user->Position[0][1] = user->Position[1][1];


}

void updateEnemyPosition(Enemy* en, Player user){
	int i;
	for (i = 0; i<3; i++){
		if (en[i] != NULL){
			en[i]->LEVEL = user->BATTLES;
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

void addAnimation(AnimationPtr animate, int x, int y){
	int i, j, k;
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
	int i, j, k;
	int enemyIndex;
	char temp[64];
	int offsetX = 0;
	int offsetY = 0;
	int maxOffset = 1;

	if (en->NAME == "Skeleton"){
		enemyIndex = 1;
		maxOffset = 48;
	}
	else if (en->NAME == "Goblin"){
		enemyIndex = 2;
		maxOffset = 25;
	}
	else if (en->NAME == "Orc"){
		enemyIndex = 3;
		maxOffset = 20;
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
		if (offsetX > 0 && offsetX < maxOffset - 1){
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

			screen[i + 10][j] = playerSprite[i][j];

		}

	}
	updateScreen();

	getch();

	for (i = 0; i < strlen(temp); i++)
		screen[1][i + 3] = ground;
	updateScreen();

	encounter(en, user, screen);

}

char** loadArt(char* filename){
	int i, j, k;
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

	temp = malloc(sizeof(char) * 20);


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

WeaponPtr initWeapon(char* name, double weaponMod, double attackModMin, double attackModMax, double AccMod, int isPhysical, char* DESCRIPTION){
	WeaponPtr temp;
	temp = malloc(sizeof(Weapon));
	//temp->name = malloc(sizeof(char) * 20);

	temp->NAME = name;
	temp->weaponMod = weaponMod;
	temp->attackModMin = attackModMin;
	temp->attackModMax = attackModMax;
	temp->AccMod = AccMod;
	temp->isPhysical = isPhysical;
	temp->DESCRIPTION = DESCRIPTION;

	return temp;
}

void moveSalesman(SalesMan storeman, Player user){
	if (storeman != NULL && user != NULL){
		int r = rand() % 4;
		if (r == 0)
			storeman->Position[1][1]++;
		if (r == 1)
			storeman->Position[1][1]--;
		if (r == 2)
			storeman->Position[1][0]++;
		if (r == 3)
			storeman->Position[1][0]--;




		if (storeman->Position[1][0] < 0)
			storeman->Position[1][0] = 0;
		if (storeman->Position[1][1] < 0)
			storeman->Position[1][1] = 0;

		if (storeman->Position[1][0] > 20)
			storeman->Position[1][0] = 19;
		if (storeman->Position[1][1] > 78)
			storeman->Position[1][1] = 78;




		screen[storeman->Position[0][0]][storeman->Position[0][1]] = ground;
		screen[storeman->Position[1][0]][storeman->Position[1][1]] = '$';

		if (user->Position[1][0] == storeman->Position[1][0] && user->Position[1][1] == storeman->Position[1][1]){
			printf("Welcome to my humble shop...\n\n");
		}


		storeman->Position[0][0] = storeman->Position[1][0];
		storeman->Position[0][1] = storeman->Position[1][1];
	}
}

SalesMan addSalesman(){
	SalesMan temp = malloc(sizeof(SalesManSize));
	temp->Position[1][0] = rand() % 20;
	temp->Position[1][1] = rand() % 80;

	temp->Position[0][0] = temp->Position[1][0];
	temp->Position[0][1] = temp->Position[1][1];

	return temp;
}

void menuGraphics(Player user){
	char tempScreen[20][80];
	char temp[20];
	int cursor[3];
	int i, j, k;
	int key_code = 0;

	for (i = 0; i < 20; i++){
		for (j = 0; j < 40; j++)
			tempScreen[i][j] = screen[i][j];
	}


	for (i = 9; i > 0; i--){
		for (j = 2; j < 22; j++){
			screen[i][j] = ' ';
			screen[18 - i][j] = ' ';
		}
		if (i == 1)
		{
			for (j = 2; j < 22; j++)
				screen[1][j] = 219;
		}
		if (i > 1){
			screen[18 - i][2] = 219;
			screen[18 - i][21] = 219;
			screen[i][2] = 219;
			screen[i][21] = 219;
		}
		if (i == 1)
		{
			screen[17][2] = 219;
			screen[17][21] = 219;
			for (j = 2; j < 22; j++)
				screen[18][j] = 219;
		}
		Sleep(20);
		updateScreen();
	}
	sprintf(temp, "Items");
	for (i = 0; i < strlen(temp); i++)
	{
		screen[4][i + 5] = temp[i];
	}

	sprintf(temp, "%s", user->NAME);
	for (i = 0; i < strlen(temp); i++)
	{
		screen[6][i + 5] = temp[i];
	}
	updateScreen();


	cursor[0] = 4;
	cursor[1] = 4;
	cursor[2] = 0;

	screen[cursor[0]][cursor[1]] = 219;

	updateScreen();

	while (key_code != 13 && key_code != 27){
		if (kbhit()){
			key_code = getch();
			if (key_code == 'w' && cursor[2] > 0){
				screen[cursor[0]][cursor[1]] = ' ';
				cursor[0] -= 2;
				cursor[2] --;
			}
			if (key_code == 's' && cursor[2] < 2){
				screen[cursor[0]][cursor[1]] = ' ';
				cursor[0] += 2;
				cursor[2] ++;
			}
			if (key_code == 13){
				switch (cursor[2]){
				case 0:
					inventoryGraphics(user);
					break;
				case 1:
					printf("You are very handsome\n");
					break;
				case 2:
					break;
				}

			}
			screen[cursor[0]][cursor[1]] = 219;
			updateScreen();
		}

	}

	for (i = 0; i < 20; i++){
		for (j = 0; j < 40; j++)
			screen[i][j] = tempScreen[i][j];
		updateScreen();
		Sleep(10);
	}


}

void inventoryGraphics(Player user){
	int i, j, k;
	char temp[40];
	ItemPtr tempItem = user->INVENTORY->head;

	for (i = 0; i < 25; i++){
		for (j = 0; j < 80; j++)
			screen[i][j] = ' ';
	}

	sprintf(temp, "Left Hand: %.20s\0", user->weaponLeft->NAME);

	for (i = 0; i < 32 && temp[i] != '\0'; i++){
		screen[1][i + 10] = temp[i];
	}

	sprintf(temp, "Right Hand: %.20s\0", user->weaponRight->NAME);
	for (i = 0; i < 32 && temp[i] != '\0'; i++){
		screen[3][i + 10] = temp[i];
	}

	i = 0;
	while (tempItem->next != NULL)
	{
		if (tempItem->POTION != NULL){
			sprintf(temp, "%.20s x%d\0", tempItem->POTION->NAME, tempItem->QUANTITY);
		}
		else if (tempItem->WEAPON != NULL){
			sprintf(temp, "%.20s x%d\0", tempItem->POTION->NAME, tempItem->QUANTITY);
		}
		for (j = 0; j < 21; j++){
			screen[k][i] = temp[j];
			i++;
		}
		if (i > 39){
			i = 0;
			k++;
		}


	}
	updateScreen();
	getch();
}

PotionPtr initPotion(char* NAME, int MAXHPRAISE, int HPRAISE, int ATKRAISE, int DEFRAISE, int MATKRAISE, int MDEFRAISE, int ACCRAISE, int LCKRAISE, char* DESCRIPTION){
	PotionPtr temp = malloc(sizeof(Potion));

	temp->NAME = NAME;
	temp->MAXHPRAISE = MAXHPRAISE;
	temp->HPRAISE = HPRAISE;
	temp->ATKRAISE = ATKRAISE;
	temp->DEFRAISE = DEFRAISE;
	temp->MATKRAISE = MATKRAISE;
	temp->MDEFRAISE = MDEFRAISE;
	temp->ACCRAISE = ACCRAISE;
	temp->LCKRAISE = LCKRAISE;
	temp->DESCRIPTION = DESCRIPTION;

	return temp;
}
