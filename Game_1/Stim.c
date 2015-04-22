/*PATCH 1.1 NOTES

Fixes:
Mage reroll works correctly

New Content:
your mom

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include "Stim.h"


int wooden_sword[] = { 0, 0 }, fire_rune[] = { 0, 0 }, wood_club[] = { 0, 0 }, chipped_dagger[] = { 1, 0 };
int potions = 3;
int light_armor[] = { 0 }, heavy_armor[] = { 0 }, dual = 0, isrune = 0, ismace = 0, issword = 0, isdagger = 0;
int ismain_hand = 1, isoff_hand = 0, isarmor = 0,is_magic_main=0, is_magic_off=0, is_physical_main=1, is_physical_off=0;
int mage_roll = 0, HP, MAX_HP, ATK, DEF, MATK, MDEF, ACC, LCK, iswarrior = 0, isrogue = 0, ismage = 0, iscleric = 0;
double DMG,eDMG,WMOD1,WMOD2,WMULT1,WMULT2;
//calculate damage split

int isskeleton=0, istroll=0, isgoblin=0, isorc=0;
int eHP, temp_eHP, eATK, eDEF, eMATK, eMDEF, eACC, eLCK, eWMOD, enemy_level = 1, enemy = 1, drop_rarity;
void enemy_combat(void);
void lv1drops();
int drop_roll();
void pickup_weapon(int*weapon);
void pickup_potion();
void pickup_misc(int * misc);
//to make different potion types, just make an array and each space will represent a type
void lv1_pick_monster(void);
void your_attack(void);
void enemy_attack(void);
void encounter(void);
void combat(void);
void meditate(void);
//work on meditate
void clear_buffer(void);
void skeleton(void);
void enemy_attacks(void);
void goblin(void);
void orc(void);
void troll(void);

int main(int argc, char* argv[])
{
	srand(time(NULL));
	character_select();
	for (;;)
		menu();	
	return 0;
}


void character_select(void)
{
	int pick, up=4,low=1,i;
	printf("Choose your character: \n\n");
	printf("(1) Warrior\n(2) Cleric\n(3) Mage\n(4) Rogue\n\n");
	i=scanf("%d", &pick);
	pick = dumb_user(pick,up,low,i);
	switch (pick)
	{
	case 1:
		printf("You are a Warrior! Your armor gives you strength.\n\n");
		HP = 11;
		warrior();
		break;
	case 2:
		printf("You are a Cleric! Potions are good to the last drop.\n\n");
		HP = 15;
		cleric();
		break;
	case 3:
		printf("You are a Mage! Runes are your friends!\n\n");
		HP = 10;
		mage();
		break;
	case 4:
		printf("You are a Rogue! Sorry, you can't sneak.\n\n");
		HP = 8;
		rogue();
		break;
	}
}

void warrior(void)
{
	MAX_HP = 11;
	ATK = 11;
	DEF = 13; 
	MATK = 6; 
	MDEF = 12; 
	ACC = 70;
	LCK = 8;
	issword = 0, isrune = 0, ismace = 0, isdagger = 0,isarmor=0;
	your_main_weapon();
	your_off_weapon();
	your_armor();
	DMG=calc_dmg();
	isdual();
	iswarrior = 1;
}
void mage(void)
{
	MAX_HP = 10;
	ATK = 7;
	DEF = 10;
	MATK = 15;
	MDEF = 14;
	ACC = 80;
	LCK = 10;
	issword = 0, isrune = 0, ismace = 0, isdagger = 0, isarmor = 0;
	your_main_weapon();
	your_off_weapon();
	your_armor();
	DMG = calc_dmg();
	isdual();
	mage_roll = 1;
	ismage = 1;
}
void cleric(void)
{
	MAX_HP = 15;
	ATK = 14;
	DEF = 9;
	MATK = 10;
	MDEF = 12;
	ACC = 60;
	LCK = 10;
	issword = 0, isrune = 0, ismace = 0, isdagger = 0, isarmor = 0;
	your_main_weapon();
	your_off_weapon();
	your_armor();
	DMG = calc_dmg();
	isdual();
	iscleric = 1;
}
void rogue(void)
{
	MAX_HP = 8;
	ATK = 10;
	DEF = 10;
	MATK = 9;
	MDEF = 9;
	ACC = 90;
	LCK = 15;
	issword = 0, isrune = 0, ismace = 0, isdagger = 0, isarmor = 0;
	your_main_weapon();
	your_off_weapon();
	your_armor();
	DMG = calc_dmg();
	isdual();
	isrogue = 1;
}



void lv1_pick_monster(void)
{
	//skeleton
	//enemy multiplier based on level
	int r = rand() % 20;
	r += 1;
	if (r > 0 && r <= 5)
		goblin();
	else if (r > 5 && r <= 10)
		skeleton();
	else if (r > 10 && r <= 15)
		orc();
	else if (r > 15 && r <= 20)
		troll();
}

void enemy_attack(void)
{
	if (eHP <= 0)
	{
		printf("\n\nYou have slain the monster, but the horde continues in its path of destruction\n\n\n");
		lv1drops();
		isskeleton = 0, isgoblin = 0, istroll = 0, isorc = 0;
		enemy_level += 1;
	}
	else
	{
		switch (enemy)
		{
		case 1:
			enemy_attacks();
			break;
		}
	}
	
}
void encounter(void)
{

	lv1_pick_monster();
	//surpise attack
	for (;;)
	{
		clear_buffer();
		your_attack();
		enemy_attack();
		if (eHP <= 0)
			break;
	if (HP <= 0)
		{
			printf("\nYOU DIED\n\n\nHonor rank: %d\n",enemy_level);

			exit(1);
			break;
		}
	}
	
}

void your_attack()
{
	int attack, i,up=3, low=1;
	printf("HP: %d/%d\n(1) Basic Attack\n",HP,MAX_HP);
	damage_range();
	printf("(2)Use Potion (x%d)\n",potions);
	i=scanf("%d", &attack);
	attack = dumb_user(attack, up, low, i);
	switch (attack)
	{
// attacks are determined by weapons multiplier: sword with str*.2
// DMG=(weapon*attack)-defense ifDMG<1, DMG = 1
	case 1:
		/*printf("You consider your options\n");
		if (iscleric == 1)
			
		else if (iswarrior == 1)
			
		else if (ismage == 1)
			
		else if (isrogue == 1)
			*/

		combat();
		break;
	case 2:
		if (potions == 0)
		{
			system("cls");
			printf("You have no potions!\n");
			your_attack();
		}
		else
			use_potion();
		break;
	//case 3: 
		//break;
	}
}

void combat(void)
{
	system("cls");
	int roll,min,max,loss;
	//should I incorporate luck?
	//damage range is taking into account DMG as the average damage
	if (issword == 1)
	{
		//anywhere within 30% of DMG
		min = DMG*.7;
		max = DMG*1.3;
	}
	else if (ismace == 1)
	{
		//anywhere within 40% of DMG
		min = DMG*.6;
		max = DMG*1.4;
	}
	else if (isdagger == 1)
	{
		//anywhere within 10% of DMG
		min = DMG*.9;
		max = DMG*1.1;
	}
	else if (isrune == 1)
	{
		//anywhere within 20% of DMG
		min = DMG*.8;
		max = DMG*1.2;
	}
	roll = max - min+1;
	roll = rand() % roll;
	roll += min;
	int r = rand() % 100,luck=floor(110-LCK*1.2);
	r += 1;
	if (r > ACC)
		printf("You miss!\nEnemy is at %d HP\n",eHP);
	else
	{
		r = rand() % 100;
		r += 1;
		if (r >= luck)
		{
			printf("\nYou score a critical hit!\n");
			DMG *= 2;
		}
//could implement critical modifier on weapons
		if (is_magic_main==1)
			loss = roll - eMDEF;
		if (is_physical_main==1)
			loss = roll - eDEF;
		if (loss < 1)
			loss = 1;
		eHP = eHP - loss;
		printf("You delivered %d damage. Enemy is at %d HP\n", loss, eHP);
		if (r>=luck)
			DMG /= 2;
		if (isrogue == 1)
		{
			r = rand() % 100;
			r += LCK*.2 + 1;
			if (r >= 95)
			{
				printf("\nYou have the agility to do something else!\n\n");
				your_attack();
			}
				
		}
	}
}
void clear_buffer(void)
{
	char c;
	scanf("%c", &c);
	while (c != '\n')
	{
		scanf("%c", &c);
	}
}
void enemy_combat(void)
{
	//figure out enemy ranges
	int max,luck,min,r = rand() % 100;
	r += 1;
	if (isskeleton == 1)
		max = eDMG*1.2, min = eDMG*.8;
	if (isgoblin == 1)
		max = eDMG*1.1, min = eDMG*.9;
	if (isorc == 1)
		max = eDMG*1.2, min = eDMG*.9;
	if (istroll == 1)
		max = eDMG*1.3, min = eDMG*1;
	int roll = max - min + 1;
	roll = rand() % roll;
	roll += min;
	r = rand() % 100, luck = floor(110 - LCK*1.2);
	if (r > eACC)
	{
		printf("The enemy missed!\n");
	}
	else
	{
		int loss = roll - DEF;
		if (loss < 1)
			loss = 1;
		HP = HP - loss;
		printf("Enemy delivered %d damage. You are at %d HP\n\n", loss, HP);
	}
}

void skeleton(void)
{
	printf("A Lv. %d spooky skeleton appears!\n\n",enemy_level);
	eHP = 10 + (.2*enemy_level);
	eATK = 10 + (.2*enemy_level);
	eDEF = 5+(.2*enemy_level);
	eMATK = 10 + (.2*enemy_level);
	eMDEF = 5 + (.2*enemy_level);
	eACC = 80;
	eLCK = 10 + (.2*enemy_level);
	eWMOD = 1.25;
	temp_eHP = eHP;
	isskeleton = 1;
	drop_rarity = 1;
}
void goblin(void)
{
	printf("A Lv. %d mischievous goblin appears!\n\n", enemy_level);
	eHP = 8 + (.2*enemy_level);
	eATK = 6 + (.2*enemy_level);
	eDEF = 1 + (.2*enemy_level);
	eMATK = 8 + (.2*enemy_level);
	eMDEF = 1 + (.2*enemy_level);
	eACC = 90;
	eLCK = 5 + (.2*enemy_level);
	temp_eHP = eHP;
	eWMOD = 1.15;
	isgoblin = 1;
	drop_rarity = .5;
}
void orc(void)
{
	printf("A Lv. %d menacing orc appears!\n\n",enemy_level);
	eHP = 12 + (.2*enemy_level);
	eATK = 12 + (.2*enemy_level);
	eDEF = 6 + (.2*enemy_level);
	eMATK = 8 + (.2*enemy_level);
	eMDEF = 4 + (.2*enemy_level);
	eACC = 75;
	eLCK = 10 + (.2*enemy_level);
	temp_eHP = eHP;
	eWMOD = 1.25;
	isorc = 1;
	drop_rarity = 1.5;
}
void troll(void)
{
	printf("A Lv. %d terrifying troll charges you!\n\n",enemy_level);
	eHP = 18 + (.2*enemy_level);
	eATK = 14 + (.2*enemy_level);
	eDEF = 2 + (.2*enemy_level);
	eMATK = 10 + (.2*enemy_level);
	eMDEF = 6 + (.2*enemy_level);
	eACC = 65;
	eLCK = 10 + (.2*enemy_level);
	temp_eHP = eHP;
	eWMOD = 1.3;
	istroll = 1;
	drop_rarity = 3;
}
void enemy_attacks(void)
{
	int r = rand() % 100,half_hp = temp_eHP / 2, crit=110-eLCK*1.2;
	r += 1;
	if (half_hp > eHP)
	{
		if (isskeleton == 1)
			printf("\nIt's bones are shaking\n\n");
		else if (isgoblin == 1)
			printf("\nThe goblin squeals for mercy\n\n");
		else if (isorc == 1)
			printf("\nThe orc coughs up blood\n\n");
		else if (istroll == 1)
			printf("\nThe troll spits out a tooth\n\n");
	}
	eDMG = eATK*eWMOD;
	if (r >= crit)
	{
		if (isskeleton == 1)
			printf("\nThe skeleton cackles with glee!\n\n");
		else if (isgoblin == 1)
			printf("\nThe goblin is giddy\n\n");
		else if (isorc == 1)
			printf("\nThe orc deals a heavy blow!\n\n");
		else if (istroll == 1)
			printf("\nThe troll smashes you\n\n");
		eDMG *= 2;
	}
	enemy_combat();
	if (r >= eLCK)
		eDMG /= 2;
}
void menu(void)
{
	int select, up=6, low=1,i;
	who_are_you();
	printf("What would you like to do?\n");
	printf("(1) Venture into the unknown\n(2)Check your inventory\n(3)Check your stats\n(4)Save\n(5)Quit\n");
	i=scanf("%d", &select);
	select = dumb_user(select, up, low,i);
	switch (select)
	{
	case 1:
		system("cls");
		encounter();
		break;
	case 2:
		system("cls");
		inventory();
		break;
	case 3:
		system("cls");
		stats();
		break;
	case 4:
		system("cls");
		meditate();
		break;
	case 5:
		system("cls");
		save();
		break;
	case 6:
		system("cls");
		quit();
		break;
	}
}
void inventory()
{
	main_hand();
	if (ismain_hand==1)
		printf("\n");
	off_hand();
	if(isoff_hand==1)
		printf("\n");
	armor();
	if (isarmor == 1)
		printf("\n");
	int i;
	if (potions > 0)
	{
		if (potions == 1)
			printf("x1 Potion\n");
		else
			printf("x%d Potions\n", potions);
	}
	printf("\n\n");
	
}
void stats()
{
	printf("HP: %d/%d\nATK: %d\nDEF: %d\nMATK: %d\nMDEF: %d\nACC: %d\nLCK: %d\n\n", HP,MAX_HP, ATK, DEF, MATK, MDEF, ACC, LCK);
	printf("Main hand: ");
	main_hand();
	printf("\nOff hand: ");
	off_hand();
	printf("\nArmor: ");
	armor();
	printf("\n\n");
}
void save()
{
	exit(1);
}
void quit()
{
	exit(1);
}
int dumb_user(int select, int upper, int lower,int i)
{
	for (;;)
	{
		if (i != 1)
		{
			printf("\nChoose Wisely\n");
			clear_buffer();
			i = scanf("%d", &select);
		}
		else if (select < lower || select>upper)
		{
			printf("\nChoose Wisely\n");
			clear_buffer();
			i = scanf("%d", &select);
		}
		else
			break;
	}
	return select;
}
void main_hand()
{
	if (wooden_sword[0] == 1)
		printf("Wooden Sword (+0 ATK, 1.5 MOD)");
	if (fire_rune[0] == 1)
		printf("Fire Rune (+2 MATK, 1.2 MOD)");
	if (wood_club[0] == 1)
		printf("Wood Club (+3 ATK, 1 MOD)");
	if (chipped_dagger[0] == 1)
		printf("Chipped Dagger (-1 ATK, 1.1 MOD)");
}
void off_hand()
{
	if (wooden_sword[1] == 1)
		printf("Wooden Sword (+0 ATK, 1.5 MOD)");
	if (fire_rune[1] == 1)
		printf("Fire Rune (+2 MATK, 1.2 MOD)");
	if (wood_club[1] == 1)
		printf("Wood Club (+3 ATK, 1 MOD)");
	if (chipped_dagger[1] == 1)
		printf("Chipped Dagger (-1 ATK, 1.1 MOD)");
}
void armor()
{
	if (light_armor[0] == 1)	
			printf("Light Armor (+1 DEF, +2 MDEF,+1 HP,+5 ACC)");
	if (heavy_armor[0] == 1)
			printf("Heavy Armor (+2 DEF, +1 MDEF,+3HP)");
}
int drop_roll()
{
	int r = rand() % 30;
	int bonus = floor(r - 1 + .2*LCK + drop_rarity);
	return bonus;
}
void lv1drops()
{
	ismain_hand = 0, isoff_hand = 0, isarmor = 0;
	int roll = drop_roll();
	if (roll < 5)
		printf("It dropped nothing!\n");
	if (roll >= 5 && roll < 9)
	{
		ismain_hand = 1;
		isoff_hand = 1;
		printf("The enemy dropped a chipped dagger.\n\n\nDropped Item: Chipped Dagger (-1 ATK, 1.1 MOD)\n\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll==0)
			pickup_weapon(chipped_dagger);
	}
	if (roll >= 9 && roll < 13)
	{
		ismain_hand = 1;
		isoff_hand = 1;
		printf("The enemy dropped a wood club.\n\n\nDropped Item: Wood Club (+3 ATK, 1 MOD)\n\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll==0)
			pickup_weapon(wood_club);
	}
	if (roll >= 13 && roll < 18)
	{
		printf("It dropped a potion!\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll == 0)
			pickup_potion();
	}
	if (roll >=18&&roll<22)
	{
		ismain_hand = 1;
		isoff_hand = 1;
		printf("It dropped a wooden sword!\n\n\nDropped Item: Wooden Sword (+0 ATK, 1.5 MOD)\n\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll == 0)
			pickup_weapon(wooden_sword);
	}
	if (roll>=22&&roll<25)
	{
		ismain_hand = 1;
		isoff_hand = 1;
		printf("It dropped a fire rune!!\n\n\nDropped Item: Fire Rune (+2 MATK, 1.2 MOD)\n\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll == 0)
			pickup_weapon(fire_rune);
	}
	if (roll >= 25 && roll < 28)
	{
		printf("It dropped light armor!!!\n\n\nDropped Item: Light Armor (+1 DEF, +2 MDEF,+1 HP,+5 ACC)\n\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll == 0)
			pickup_misc(light_armor);
	}
	if (roll >= 28 && roll < 31)
	{
		printf("It dropped heavy armor!!!\n\n\nDropped Item: Heavy Armor (+2 DEF, +1 MDEF,+3HP)\n\n");
		if (mage_roll == 1)
			mage_reroll();
		else if (mage_roll == 0)
			pickup_misc(heavy_armor);
	}
}
void pickup_weapon(int* weapon)
{
	int i, answer2, answer3;
	char answer1;
	printf("Your main hand: ");
	main_hand();
	printf("\nYour off Hand: ");
	off_hand();
	printf("\n\nWould you like to pick it up?(y/n)\n");
	i = scanf(" %c", &answer1);
	answer1 = dumb_user_yn(answer1, i);
	if (answer1 == 'y' || answer1 == 'Y')
		{
		if (ismain_hand == 1 && isoff_hand == 1)
		{
			printf("Would you like to equip it to your main hand(1) or off hand(2)?\n");
			clear_buffer();
			i = scanf("%d", &answer2);
			answer2 = dumb_user(answer2, 2, 1, i);
			if (answer2 == 1)
			{
				drop_main_weapon();
				weapon[0] = 1;
				ismain_hand = 1;
			}
				
			else
			{
				drop_off_weapon();
				weapon[1] = 1;
			}
				
		}
		//use else if () for other drop types				
		}
}
void pickup_potion()
{
	int i, answer2, answer3;
	char answer1;
	printf("Would you like to pick it up?(y/n)\n");
	i = scanf(" %c", &answer1);
	answer1 = dumb_user_yn(answer1, i);
	if (answer1 == 'y' || answer1 == 'Y')
	{
		printf("The potion is yours! Huzzah!\n");
		potions++;
	}
}
void pickup_misc(int* misc)
{
	int i;
	char answer1;
	printf("Your armor: ");
	armor();
	printf("\n\nWould you like to equip the armor?(y/n)\n");
	i = scanf(" %c", &answer1);
	answer1 = dumb_user_yn(answer1, i);
	if (answer1 == 'y' || answer1 == 'Y')
	{
		drop_armor();
		misc[0] = 1;
		isarmor = 1;
	}
}
char dumb_user_yn(char answer, int i)
{
	for (;;)
	{
		if (i != 1)
		{
			printf("Choose Wisely\n");
			clear_buffer();
			i = scanf("%c", &answer);
		}
		else if (answer == 'y' || answer == 'Y' || answer == 'n' || answer == 'N')
			break;
		else
		{
			printf("Choose Wisely\n");
			clear_buffer();
			i = scanf("%c", &answer);
		}
	}
	return answer;
}
void who_are_you()
{
	if (iswarrior == 1)
		warrior();
	if (ismage == 1)
		mage();
	if (iscleric == 1)
		cleric();
	if (isrogue == 1)
		rogue();
}
double calc_dmg()
{

	if(is_physical_main==1&&is_magic_off==1)
		DMG = (ATK+WMOD1)*WMULT1+(MATK+WMOD2)*WMULT2;
	if (is_magic_main==1&&is_physical_off==1)
		DMG = (MATK + WMOD1)*WMULT1 + (ATK + WMOD2)*WMULT2;
	if (is_magic_main==1&&is_magic_off==1)
		DMG = (MATK + WMOD1)*WMULT1 + (MATK + WMOD2)*WMULT2;
	if (is_physical_main==1&&is_physical_off==1)
		DMG = (ATK + WMOD1)*WMULT1 + (ATK + WMOD2)*WMULT2;
	if (is_physical_main == 1 && is_physical_off == 0 && is_magic_main == 0 && is_magic_off == 0)
		DMG = (ATK + WMOD1)*WMULT1;
	if (is_physical_main == 0 && is_physical_off == 1 && is_magic_main == 0 && is_magic_off == 0)
		DMG = (ATK/2 + WMOD2)*WMULT2;
	if (is_physical_main == 0 && is_physical_off == 0 && is_magic_main == 1 && is_magic_off == 0)
		DMG = (MATK + WMOD1)*WMULT1;
	if (is_physical_main == 0 && is_physical_off == 0 && is_magic_main == 0 && is_magic_off == 1)
		DMG = (MATK/2 + WMOD2)*WMULT2;
	return DMG;
}
void your_armor()
{
	if (light_armor[0] == 1)
	{
		DEF += 1;
		MDEF += 2;
		MAX_HP += 1;
		ACC += 5;
	}
		
	if (heavy_armor[0] == 1)
	{
		DEF += 2;
		MDEF += 1;
		MAX_HP += 3;
	}
}
void your_main_weapon()
{
	if (wooden_sword[0] == 1)
	{
	WMOD1 = 0, WMULT1 = 1.5;
	is_magic_main = 0;
	is_physical_main = 1;
	issword = 1;
	}
	if (fire_rune[0] == 1)
	{
		WMOD1 = 2, WMULT1 = 1.2;
		is_magic_main = 1;
		is_physical_main = 0;
		isrune = 1;
	}
	if (wood_club[0] == 1)
	{
		WMOD1 = 3, WMULT1 = 1;
		is_magic_main = 0;
		is_physical_main = 1;
		ismace = 1;
	}
	if (chipped_dagger[0] == 1)
	{
		WMOD1 = -1, WMULT1 = 1.1;
		is_magic_main = 0;
		is_physical_main = 1;
		isdagger = 1;
	}
}
void your_off_weapon()
{
	if (wooden_sword[1] == 1)
	{
		WMOD2 = 0, WMULT2 = 1.5;
		is_magic_off = 0;
		is_physical_off = 1;
		issword = 1;
	}
	if (fire_rune[1] == 1)
	{
		WMOD2 = 2, WMULT2 = 1.2;
		is_magic_off = 1;
		is_physical_off = 0;
		isrune = 1;
	}
	if (wood_club[1] == 1)
	{
		WMOD2 = 3, WMULT2 = 1;
		is_magic_off = 0;
		is_physical_off = 1;
		ismace = 1;
	}
	if (chipped_dagger[1] == 1)
	{
		WMOD2 = -1, WMULT2 = 1.1;
		is_magic_off = 0;
		is_physical_off = 1;
		isdagger = 1;
	}
}
void drop_main_weapon()
{
	if (wooden_sword[0] == 1)
		wooden_sword[0] = 0;
	if (fire_rune[0] == 1)
		fire_rune[0] = 0;
	if (wood_club[0] == 1)
		wood_club[0] = 0;
	if (chipped_dagger[0] == 1)
		chipped_dagger[0] = 0;
}
void drop_off_weapon()
{
	if (wooden_sword[1] == 1)
		wooden_sword[1] = 0;
	if (fire_rune[1] == 1)
		fire_rune[1] = 0;
	if (wood_club[1] == 1)
		wood_club[1] = 0;
	if (chipped_dagger[1] == 1)
		chipped_dagger[1] = 0;
}
void drop_armor()
{
	if (light_armor[0] == 1)
		light_armor[0] = 0;
	if (heavy_armor[0] == 1)
		heavy_armor[0] = 0;
}
void isdual()
{
	if (is_physical_main == 1 && is_magic_off == 1)
		dual = 1;
	if (is_magic_main == 1 && is_physical_off == 1)
		dual = 1;
	if (is_magic_main == 1 && is_magic_off == 1)
		dual = 1;
	if (is_physical_main == 1 && is_physical_off == 1)
		dual = 1;
	if (is_physical_main == 1 && is_physical_off == 0 && is_magic_main == 0 && is_magic_off == 0)
		dual = 0;
	if (is_physical_main == 0 && is_physical_off == 1 && is_magic_main == 0 && is_magic_off == 0)
		dual = 0;
	if (is_physical_main == 0 && is_physical_off == 0 && is_magic_main == 1 && is_magic_off == 0)
		dual = 0;
	if (is_physical_main == 0 && is_physical_off == 0 && is_magic_main == 0 && is_magic_off == 1)
		dual = 0;
	if (dual == 1)
	{
		DMG *= .9;
		ACC *= .8;
	}
		//this might be a huge nerf to accuracy
}
void damage_range()
{
	int min, max;
	if (issword == 1)
	{
		//anywhere within 30% of DMG
		min = DMG*.7;
		max = DMG*1.3;
	}
	else if (ismace == 1)
	{
		//anywhere within 40% of DMG
		min = DMG*.6;
		max = DMG*1.4;
	}
	else if (isdagger == 1)
	{
		//anywhere within 10% of DMG
		min = DMG*.9;
		max = DMG*1.1;
	}
	else if (isrune == 1)
	{
		//anywhere within 20% of DMG
		min = DMG*.8;
		max = DMG*1.2;
	}
	if (is_magic_main == 1)
	{
		min -= eMDEF;
		max -= eMDEF;
	}
	if (is_physical_main == 1)
	{
		min -= eDEF;
		max -= eDEF;
	}
	if (min < 1)
		min = 1;
	if (max < 1)
		max = 1;
	printf("Damage: %d - %d \n", min, max);
}
void use_potion()
{
	system("cls");
	if (potions > 0)
	{
		int temp = MAX_HP;
		int heal;
		if (iscleric == 1)
			heal = MAX_HP*.75;
		else
			heal = MAX_HP*.5;
		HP += heal;
		if (HP > temp)
			HP = temp;
		printf("You healed for %d HP!\n", heal);
		potions--;
	}
}
void mage_reroll()
{
	int i;
	char answer;
	if (ismage == 1)
	{
		printf("Would you like to cast Transform (y/n)?\n");
		clear_buffer();
		i = scanf("%c", &answer);
		answer=dumb_user_yn(answer,i);
		mage_roll = 0;
		if (answer == 'y' || answer == 'Y')
			lv1drops();
	}
}
void meditate(void)
{
	return;
}