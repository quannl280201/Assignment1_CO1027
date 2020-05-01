#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <math.h>

using namespace std;

#define MadBearDamage  1.0
#define BanditDamage  1.5
#define LordLupinDamage  4.5
#define ElfDamage  6.5
#define TrollDamage  8.5
const int EVENT_SIZE = 100;
const int MAX_CHARACTER_EACH_LINE = 250;
enum EVENT{
	GuinevereReturn,
	MadBear,
	Bandit,
	LordLupin,
	Elf,
	Troll,
	Shaman,
	SirenVajsh,
	findExcalibur,
	findMythirl,
	findExcalipoor,
	MushMario,
	MushFib,
	MushGhost,
	MushKnight,
	Remedy,
	MaidenKiss,
	PhoenixDown,
	Merlin,
	Abyss,
	Guinevere,
	LightWing,
	Odin,
	DragonSowrd,
	Bowser = 99
};
enum STATUS{
	normal,
	tiny,
	frog
};
enum WEAPON{
	Excalibur = 1,
	Mythirl,
	Excalipoor
};
int currentStatus = 0;
int statusTime = 0;
int levelBeforeChangeToFrog = 0;
int currentWeapon = 0;
struct knight
{
   int HP;
   int level;
   int remedy;
   int maidenkiss;
   int phoenixdown;
};

// read data from input file to corresponding variables
// return 1 if successfully done, otherwise return 0
int readFile(const char* filename, knight& theKnight, int& nEvent, int* arrEvent)
{
	const char* file_name = filename;
	FILE* f = 0;
	char* str = new char[MAX_CHARACTER_EACH_LINE];
	int num;

	f = fopen(file_name, "r");
	if (f == NULL)	//file not found || cannot read
		return 0;

	fgets(str, MAX_CHARACTER_EACH_LINE, f);
	if (feof(f))
		return 0;

	int start = 0;
	int len = strlen(str);
	// read HP
	while (start < len && str[start] == ' ')
		start ++;
	num = 0;
	while (start < len && str[start] <= '9' && str[start] >= '0'){
		num = num * 10 + str[start] - '0';
		start++;
	}
	theKnight.HP = num;
	if (theKnight.HP < 1 || theKnight.HP > 999)
		return 0;

	// read level
	while (start < len && (str[start] > '9' || str[start] < '0'))
		start ++;
	num = 0;
	while (start < len && str[start] <= '9' && str[start] >= '0'){
		num = num * 10 + str[start] - '0';
		start++;
	}
	theKnight.level = num;
	if (theKnight.level < 1 || theKnight.level > 10)
		return 0;

	// read remedy
	while (start < len && (str[start] > '9' || str[start] < '0'))
		start ++;
	num = 0;
	while (start < len && str[start] <= '9' && str[start] >= '0'){
		num = num * 10 + str[start] - '0';
		start++;
	}
	theKnight.remedy = num;
	if (theKnight.remedy < 0 || theKnight.remedy > 99)
		return 0;

	// read maidenkiss
	while (start < len && (str[start] > '9' || str[start] < '0'))
		start ++;
	num = 0;
	while (start < len && str[start] <= '9' && str[start] >= '0'){
		num = num * 10 + str[start] - '0';
		start++;
	}
	theKnight.maidenkiss = num;
	if (theKnight.maidenkiss < 0 || theKnight.maidenkiss > 99)
		return 0;

	// read phoenixdown
	while (start < len && (str[start] > '9' || str[start] < '0'))
		start ++;
	num = 0;
	while (start < len && str[start] <= '9' && str[start] >= '0'){
		num = num * 10 + str[start] - '0';
		start++;
	}
	theKnight.phoenixdown = num;
	if (theKnight.phoenixdown < 0 || theKnight.phoenixdown > 99)
		return 0;

	// read events
	nEvent = 0;
	while (1){
		if (feof(f))
			break;
		fgets(str, MAX_CHARACTER_EACH_LINE, f);
		if (str == NULL)
			break;
		start = 0;
		len = strlen(str);
    // cout << "\'" << str << "\'" << "\t" << len << endl;
		while (start < len){
			while (start < len && (str[start] > '9' || str[start] < '0'))
				start++;
			if (start >= len)
				break;
			arrEvent[nEvent] = 0;
			while (start < len && str[start] <= '9' && str[start] >= '0'){
				arrEvent[nEvent] = arrEvent[nEvent] * 10 + str[start] - '0';
				start++;
			}
			nEvent++;
		}
    delete[] str;
	}
	fclose(f);
	return 1;
}

void display(int &nOut)
{
  if (nOut)
    cout << nOut;
  else
    cout << "Error!";
}

void combat(knight &theKnight, int maxHP, int event, int opponent, float baseDamage, int &nOut){
	int b = event % 10;
	int levelO = event > 6 ? (b > 5 ? b : 5) : b;
	//cout << "LevelO: " << levelO << '\n';
	if (theKnight.level > levelO || currentWeapon == Excalibur) {
		theKnight.level = (theKnight.level + 1) > 10 ? 10 : (theKnight.level + 1);  
	}
	else if (theKnight.level < levelO) {
		float damage = baseDamage * levelO * 10; 
		theKnight.HP = theKnight.HP - damage;
		if (theKnight.HP < 0) {
			if (theKnight.phoenixdown) {
				currentStatus = normal;
				theKnight.HP = maxHP;
				theKnight.phoenixdown = theKnight.phoenixdown - 1;
			}
			else nOut = -1;
		} 
	}
}
void dealWithShaman_Vajsh(knight &theKnight, int maxHP, int event, int opponent){
	if (currentStatus == tiny || currentStatus == frog) return;
	int b = event % 10;
	int levelO = event > 6 ? (b > 5 ? b : 5) : b;
	if (theKnight.level > levelO) {
		theKnight.level = (theKnight.level + 2) > 10 ? 10 : (theKnight.level + 2);  
	}
	else if (theKnight.level < levelO) {
		switch(opponent) {
			case Shaman: 
				theKnight.HP = theKnight.HP < 5 ? 1 : (theKnight.HP / 5);
				currentStatus = tiny;
				statusTime = 4;
				if (theKnight.remedy) {
					theKnight.remedy = theKnight.remedy - 1;
					theKnight.HP = (theKnight.HP * 5) > maxHP ? maxHP : (theKnight.HP * 5);
					currentStatus = normal;
					statusTime = 0;
				}
				break;
			case SirenVajsh:
				levelBeforeChangeToFrog = theKnight.level;
				theKnight.level = 1;
				currentStatus = frog;
				statusTime = 4;
				if(theKnight.maidenkiss) {
					theKnight.maidenkiss = theKnight.maidenkiss - 1;
					theKnight.level = levelBeforeChangeToFrog;
					currentStatus = normal;
					statusTime = 0;
				}
				break;
		}
	}
}
void statusCheck(int & statusTime, knight &theKnight, int maxHP){
	if (statusTime > 1) {
		statusTime = statusTime - 1;
	}
	else if(statusTime == 1) {
		switch (currentStatus)
		{
			case tiny:
				statusTime = statusTime  - 1;
				theKnight.HP = (theKnight.HP * 5 > maxHP) ? maxHP : (theKnight.HP * 5);
				currentStatus = normal; 
				break;
			case frog:
				statusTime = statusTime  - 1;
				theKnight.level = levelBeforeChangeToFrog;
				currentStatus = normal;

		}
	}
}
void process(knight &theKnight, int nEvent, int *arrEvent, int &nOut){
	int maxHP = theKnight.HP;
	for (int i = 0; i < nEvent; i++) { 
		switch(arrEvent[i]) {
			case GuinevereReturn:
				nOut = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
				return;
			case MadBear:
				combat(theKnight, maxHP, i+1, MadBear, MadBearDamage, nOut);
				if (nOut == -1) return;
				break;
			case Bandit: 
				combat(theKnight, maxHP, i+1, Bandit, BanditDamage, nOut);
				if (nOut == -1) return;
				break;
			case LordLupin:
				combat(theKnight, maxHP, i+1, LordLupin, LordLupinDamage, nOut);
				if (nOut == -1) return;
				break;
			case Elf: 
				combat(theKnight, maxHP, i+1, Elf, ElfDamage, nOut);
				if(nOut == -1) return;
				break;
			case Troll:
				combat(theKnight, maxHP, i+1, Troll, TrollDamage, nOut);
				if (nOut == -1) return;
				break;
			case Shaman:
				dealWithShaman_Vajsh(theKnight, maxHP, i+1, Shaman);
				break;
			case SirenVajsh:
				dealWithShaman_Vajsh(theKnight, maxHP, i+1, SirenVajsh);
				break;
			case findExcalibur:
				currentWeapon = Excalibur;
				break;
		}
		statusCheck(statusTime, theKnight, maxHP);
	}
	nOut = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
}

int main(int argc, char** argv)
{
    if (argc < 2) return 1;
    const char* filename = argv[1];;

   struct knight theKnight;
   int nEvent; //number of events
   int* arrEvent = new int[EVENT_SIZE]; // array of events
   int nOut = 0;                           // final result

	readFile(filename, theKnight, nEvent, arrEvent);
	process(theKnight, nEvent, arrEvent, nOut);
	//cout << theKnight.HP << " " << theKnight.level << " " << theKnight.remedy << " " << theKnight.maidenkiss << " " << theKnight.phoenixdown << '\n';
    display(nOut);
	return 0;
}

