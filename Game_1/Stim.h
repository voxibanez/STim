typedef struct enemy{
	int Position[2][2];

	char* NAME;
	int MAXHP;
	int HP;
	int LEVEL;
	int ATK;
	int DEF;
	int MATK;
	int MDEF;
	int ACC;
	int LCK;
	int WMOD;

	int IsSkeleton;
	int drop_rareity;
}EnemySize, *Enemy;

typedef struct player{
	int Position[2][2];

	char NAME[64];
	int MAXHP;
	int HP;
	int LEVEL;
	int ATK;
	int DEF;
	int MATK;
	int MDEF;
	int ACC;
	int LCK;
	int CURRENCY;

}PlayerSize, *Player;

typedef struct animation{
	int frameCount;
	int sizeX;
	int sizeY;
	int speed;
	char*** frames;


}Animation, *AnimationPtr;


void warrior(void);
void mage(void);
void cleric(void);
void rogue(void);
void character_select(void);
void menu(void);
void inventory(void);
void stats();
void save();
void quit();
void meditate();
void drop_main_weapon();
void drop_off_weapon();
void drop_armor();
void isdual();
void use_potion();
void damage_range();
void main_hand();
void off_hand();
void armor();
void mage_reroll();
double calc_dmg();
void your_main_weapon();
void your_off_weapon();
void your_armor();
void who_are_you();
int dumb_user(int select, int upper, int lower, int i);
char dumb_user_yn(char answer, int i);