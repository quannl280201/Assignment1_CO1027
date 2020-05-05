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
#define ArthurHP 999
#define LanceLotHP 888
#define likeArthur true
#define likeNormal false
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
	MushFibo,
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
	findDragonSowrd,
	Bowser = 99
};
enum STATUS{
	normal,
	tiny,
	frog
};
enum WEAPON{
	Excalibur = 1,
	Excalipoor,
	DragonSowrd
};

struct knight
{
	int HP;
	int level;
	int remedy;
	int maidenkiss;
	int phoenixdown;
	int currentStatus = 0;
	int statusTime = 0;
	int levelBeforeChangeToFrog = 0;
	int currentWeapon = 0;
	int odinHelpLeft = 0;
	bool isWearMythirl = false;
	bool isHelpByOdin = false;
	bool useOdinHelpThisTurn = false;
	bool isDragonKnight = false;
	bool meetGuinevere = false;
	bool isArthur = false;
	bool isLancelot = false;
	bool isPaladin = false;
	bool LancelotBehavior = false;
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

void display(int &result)
{
  if (result)
    cout << result;
  else
    cout << "Error!";
}

void combat(knight &theKnight, int maxHP, int event, int opponent, float baseDamage, int &result){
	int b = event % 10;
	int levelO = event > 6 ? (b > 5 ? b : 5) : b;
	if (theKnight.isDragonKnight && theKnight.currentWeapon == DragonSowrd || theKnight.odinHelpLeft > 0 || theKnight.isArthur || theKnight.isPaladin || theKnight.isLancelot && theKnight.LancelotBehavior || theKnight.currentWeapon != Excalipoor && (theKnight.level > levelO || theKnight.currentWeapon == Excalibur)) {
		theKnight.level = (theKnight.level + 1) > 10 ? 10 : (theKnight.level + 1);
		if (theKnight.odinHelpLeft) {
			theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
			theKnight.useOdinHelpThisTurn = true;
		}  
	}
	else if (theKnight.level < levelO || theKnight.currentWeapon == Excalipoor) {
		if (theKnight.isWearMythirl) return;
		float damage = baseDamage * levelO * 10; 
		theKnight.HP = theKnight.HP - damage;
		if (theKnight.HP < 0) {
			if (theKnight.phoenixdown) {
				theKnight.currentStatus = normal;
				theKnight.HP = maxHP;
				theKnight.phoenixdown = theKnight.phoenixdown - 1;
			}
			else result = -1;
		} 
	}
}
void dealWithShaman_Vajsh(knight &theKnight, int maxHP, int event, int opponent){
	int b = event % 10;
	int levelO = event > 6 ? (b > 5 ? b : 5) : b;
	if (theKnight.isDragonKnight && theKnight.currentWeapon == DragonSowrd || theKnight.odinHelpLeft > 0 || theKnight.isArthur || theKnight.isLancelot && theKnight.LancelotBehavior || theKnight.isPaladin || (theKnight.level > levelO && theKnight.currentWeapon != Excalipoor)) {
		theKnight.level = (theKnight.level + 2) > 10 ? 10 : (theKnight.level + 2);
		if (theKnight.odinHelpLeft) {
			theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
			theKnight.useOdinHelpThisTurn = true;
		}
	}
	else if (theKnight.level < levelO || theKnight.currentWeapon == Excalipoor) {
		switch(opponent) {
			case Shaman: 
				if(!theKnight.isWearMythirl) theKnight.HP = theKnight.HP < 5 ? 1 : (theKnight.HP / 5);
				theKnight.currentStatus = tiny;
				theKnight.statusTime = 4;
				if (theKnight.remedy) {
					theKnight.remedy = theKnight.remedy - 1;
					theKnight.HP = (theKnight.HP * 5) > maxHP ? maxHP : (theKnight.HP * 5);
					theKnight.currentStatus = normal;
					theKnight.statusTime = 0;
				}
				break;
			case SirenVajsh:
				theKnight.levelBeforeChangeToFrog = theKnight.level;
				theKnight.level = 1;
				theKnight.currentStatus = frog;
				theKnight.statusTime = 4;
				if(theKnight.maidenkiss) {
					theKnight.maidenkiss = theKnight.maidenkiss - 1;
					theKnight.level = theKnight.levelBeforeChangeToFrog;
					theKnight.currentStatus = normal;
					theKnight.statusTime = 0;
				}
				break;
		}
	}
}
void statusCheck(knight &theKnight, int maxHP){
	if (theKnight.statusTime > 1) {
		theKnight.statusTime = theKnight.statusTime - 1;
	}
	else if(theKnight.statusTime == 1) {
		switch (theKnight.currentStatus)
		{
			case tiny:
				theKnight.statusTime = theKnight.statusTime  - 1;
				theKnight.HP = (theKnight.HP * 5 > maxHP) ? maxHP : (theKnight.HP * 5);
				theKnight.currentStatus = normal; 
				break;
			case frog:
				theKnight.statusTime = theKnight.statusTime  - 1;
				theKnight.level = theKnight.levelBeforeChangeToFrog;
				theKnight.currentStatus = normal;
		}
	}
}
int nextFibonacci(int HP) { 
    double a = HP * (1 + sqrt(5)) / 2.0; 
    return round(a);
}
void odinHelpCheck(knight &theKnight){
	if(!theKnight.useOdinHelpThisTurn) {
		theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
	}
	else theKnight.useOdinHelpThisTurn = false;			
}
bool isPaladincheck(int hp){
	int m = hp / 2;
	for (int i = 2; i < m; i++) {
		if (hp % i == 0) return false;
	}
	return true;
}
bool isDragonKnightCheck(int HP){ 
    for (int i = 1; i <= HP / 3; i++) { 
        for (int j = i + 1; j <= HP / 2; j++) { 
            int k = HP - i - j; 
            if (i * i + j * j == k * k) return true;             
        }
    }
    return false;
} 
void process(knight &theKnight, int nEvent, int *arrEvent, int &result, int nEventPassed){
	int maxHP = theKnight.HP;
	for (int i = 0 + nEventPassed; i < nEvent; i++) { 
		if (theKnight.isLancelot) theKnight.LancelotBehavior = (theKnight.level % 2 == 0) ? likeNormal : likeArthur;  
		switch(arrEvent[i]) {
			case GuinevereReturn:
				result = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
				return;
			case MadBear:
				combat(theKnight, maxHP, i+1, MadBear, MadBearDamage, result);
				if (result == -1) return;
				break;
			case Bandit: 
				combat(theKnight, maxHP, i+1, Bandit, BanditDamage, result);
				if (result == -1) return;
				break;
			case LordLupin:
				combat(theKnight, maxHP, i+1, LordLupin, LordLupinDamage, result);
				if (result == -1) return;
				break;
			case Elf: 
				combat(theKnight, maxHP, i+1, Elf, ElfDamage, result);
				if(result == -1) return;
				break;
			case Troll:
				combat(theKnight, maxHP, i+1, Troll, TrollDamage, result);
				if (result == -1) return;
				break;
			case Shaman:
				if (theKnight.currentStatus == tiny || theKnight.currentStatus == frog) break;
				dealWithShaman_Vajsh(theKnight, maxHP, i+1, Shaman);
				break;
			case SirenVajsh:
				if (theKnight.currentStatus == tiny || theKnight.currentStatus == frog) break;
				dealWithShaman_Vajsh(theKnight, maxHP, i+1, SirenVajsh);
				break;
			case findExcalibur:
				if (theKnight.isDragonKnight && theKnight.currentWeapon == DragonSowrd)break;
				theKnight.currentWeapon = Excalibur;
				break;
			case findMythirl:
				theKnight.isWearMythirl = true;
				break;
			case findExcalipoor:
				if (theKnight.isDragonKnight || theKnight.odinHelpLeft || theKnight.isArthur || theKnight.isLancelot && theKnight.LancelotBehavior || theKnight.isPaladin) {
					if (theKnight.odinHelpLeft) {
						theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
						theKnight.useOdinHelpThisTurn = true;
					}
				}
				else theKnight.currentWeapon = theKnight.level >= 5 ? theKnight.currentWeapon : Excalipoor;
				break;
			case MushMario:
				theKnight.HP = (theKnight.HP + 50) > maxHP ? maxHP : (theKnight.HP + 50);
				break;
			case MushFibo:
				theKnight.HP = (nextFibonacci(theKnight.HP) > maxHP) ? maxHP : nextFibonacci(theKnight.HP);
				break;
			case MushGhost:
				if (theKnight.odinHelpLeft || theKnight.isPaladin || theKnight.isDragonKnight && theKnight.currentWeapon == DragonSowrd) {
					if (theKnight.odinHelpLeft) {
						theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
						theKnight.useOdinHelpThisTurn = true;
					}
				}
				else theKnight.HP = theKnight.HP < 51 ? 1 : (theKnight.HP - 50);
				break;
			case MushKnight:
				maxHP = (maxHP + 50) > 999 ? 999 : (maxHP + 50);
				theKnight.HP = maxHP;
				break;
			case Remedy:
				if (theKnight.currentStatus == tiny) {
					theKnight.currentStatus = normal;
					theKnight.statusTime = 0;
					theKnight.HP = (theKnight.HP * 5) > maxHP ? maxHP : (theKnight.HP * 5);
				}
				else theKnight.remedy = (theKnight.remedy + 1) > 99 ? 99 : (theKnight.remedy + 1);				
				break;
			case MaidenKiss:
				if (theKnight.currentStatus == frog) {
					theKnight.currentStatus = normal;
					theKnight.statusTime = 0;
					theKnight.level = theKnight.levelBeforeChangeToFrog;
				}
				else theKnight.maidenkiss = (theKnight.maidenkiss + 1) > 99 ? 99 : (theKnight.maidenkiss + 1);
				break;
			case PhoenixDown:
				theKnight.phoenixdown = (theKnight.phoenixdown + 1) > 99 ? 99 : (theKnight.phoenixdown + 1);
				break;
			case Merlin:
				if (theKnight.currentStatus == tiny || theKnight.currentStatus == frog) {
					theKnight.level = (theKnight.currentStatus == frog) ? theKnight.levelBeforeChangeToFrog : theKnight.level;
					theKnight.currentStatus = normal;
					theKnight.statusTime = 0;
				}
				theKnight.level = (theKnight.level + 1) > 10 ? 10 : (theKnight.level + 1);
				theKnight.HP = maxHP;
				break;
			case Abyss:
				if (theKnight.odinHelpLeft || theKnight.isDragonKnight && theKnight.currentWeapon == DragonSowrd) {
					if (theKnight.odinHelpLeft) {
						theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
						theKnight.useOdinHelpThisTurn = true;
					}
				}
				else if (theKnight.level < 7) {
					result = -1;
					return;
				} 
				break;
			case Guinevere:
				{	
					theKnight.meetGuinevere = true;
					int numberOfEventUpdated = i * 2 + 1;
					int *arrEventInvert = new int[numberOfEventUpdated];
					int index;
					int temp = i - 1;
					for (index = 0; index < i; index++) {
						arrEventInvert[index] = arrEvent[index];
					}
					index = index + 1;
					for (; index < numberOfEventUpdated; index++) {
						arrEventInvert[index] = arrEvent[temp];
						temp--;
					}
					process(theKnight, numberOfEventUpdated, arrEventInvert, result, i + 1);
					delete[] arrEventInvert;
					return;
				}
			case LightWing:
				if (theKnight.odinHelpLeft) theKnight.odinHelpLeft = 0;
				if ((nEvent - i) < 3 || theKnight.meetGuinevere) {
					result = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
					return;
				}
				else {
					for (int j = i + 1; j < i + 4; j++) {
						if (arrEvent[j] == GuinevereReturn || arrEvent[j] == Guinevere) {
							result = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
							return; 
						}
					}
					i = i + 3;
					if (theKnight.currentStatus) theKnight.statusTime = theKnight.statusTime + 3;
				}
				break;
			case Odin:
				if (!theKnight.isHelpByOdin) {
					theKnight.odinHelpLeft = 4;
					theKnight.isHelpByOdin = true;
				}
				break;
			case findDragonSowrd:
				if (theKnight.isDragonKnight) theKnight.currentWeapon = DragonSowrd;
				break;
			case Bowser:
				if (theKnight.isArthur || theKnight.odinHelpLeft || theKnight.isDragonKnight && theKnight.currentWeapon == DragonSowrd || theKnight.isLancelot || theKnight.isPaladin && theKnight.level >= 8 || (theKnight.level == 10 && theKnight.currentWeapon != Excalipoor)){
					if (theKnight.odinHelpLeft) {
						theKnight.odinHelpLeft = theKnight.odinHelpLeft - 1;
						theKnight.useOdinHelpThisTurn = true;
					}
					theKnight.level = 10;
				}
				else {
					result = -1;
					return;
				}
				break;
		}
		statusCheck(theKnight, maxHP);
		if(theKnight.odinHelpLeft) odinHelpCheck(theKnight);
	}
	result = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
}

int main(int argc, char** argv)
{
    if (argc < 2) return 1;
    const char* filename = argv[1];;

   	struct knight theKnight;
   	int nEvent; //number of events
   	int* arrEvent = new int[EVENT_SIZE]; // array of events
   	int *nOut;                           // final result

	readFile(filename, theKnight, nEvent, arrEvent);

	int result = 0;
	nOut = &result;
	if (theKnight.HP == ArthurHP) theKnight.isArthur = true;
	else if (theKnight.HP == LanceLotHP) theKnight.isLancelot = true;
	else if (isPaladincheck(theKnight.HP)) theKnight.isPaladin = true;
	else theKnight.isDragonKnight = isDragonKnightCheck(theKnight.HP);
	process(theKnight, nEvent, arrEvent, result, 0);
	//cout << theKnight.HP << " " << theKnight.level << " " << theKnight.remedy << " " << theKnight.maidenkiss << " " << theKnight.phoenixdown << '\n';
    
	display(result);
	return 0;
}