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
	if (odinHelpLeft > 0 || isArthur || isPaladin || isLancelot && LancelotBehavior || currentWeapon != Excalipoor && (theKnight.level > levelO || currentWeapon == Excalibur)) {
		theKnight.level = (theKnight.level + 1) > 10 ? 10 : (theKnight.level + 1);
		if (odinHelpLeft) {
			odinHelpLeft = odinHelpLeft - 1;
			useOdinHelpThisTurn = true;
		}  
	}
	else if (theKnight.level < levelO || currentWeapon == Excalipoor) {
		if (isWearMythirl) return;
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
	int b = event % 10;
	int levelO = event > 6 ? (b > 5 ? b : 5) : b;
	if (odinHelpLeft > 0 || isArthur || isLancelot && LancelotBehavior || isPaladin || theKnight.level > levelO) {
		theKnight.level = (theKnight.level + 2) > 10 ? 10 : (theKnight.level + 2);
		if (odinHelpLeft) {
			odinHelpLeft = odinHelpLeft - 1;
			useOdinHelpThisTurn = true;
		}
	}
	else if (theKnight.level < levelO) {
		switch(opponent) {
			case Shaman: 
				if(!isWearMythirl) theKnight.HP = theKnight.HP < 5 ? 1 : (theKnight.HP / 5);
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
int nextFibonacci(int HP) { 
    double a = HP * (1 + sqrt(5)) / 2.0; 
    return round(a);
}
void odinHelpCheck(){
	if(!useOdinHelpThisTurn) {
		odinHelpLeft = odinHelpLeft - 1;
	}
	else useOdinHelpThisTurn = false;			
}
bool isPaladincheck(int hp){
	int m = hp / 2;
	for (int i = 2; i < m; i++) {
		if (hp % i == 0) return false;
	}
	return true;
}
void process(knight &theKnight, int nEvent, int *arrEvent, int &nOut){
	int maxHP = theKnight.HP;
	for (int i = 0; i < nEvent; i++) { 
		// cout << "Event: " << i << '\n';
		// cout << "Knight index: " << theKnight.HP << " " << theKnight.level << " " << theKnight.remedy << " " << theKnight.maidenkiss << " " << theKnight.phoenixdown << '\n';
		// cout << "Check status: " << currentStatus << '\n';
		// cout << "currentWeapon: " << currentWeapon << '\n';
		// cout << "Odin help: " << odinHelpLeft << '\n';
		if (isLancelot) LancelotBehavior = (theKnight.level % 2 == 0) ? likeNormal : likeArthur;  
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
				if (currentStatus == tiny || currentStatus == frog) break;
				dealWithShaman_Vajsh(theKnight, maxHP, i+1, Shaman);
				break;
			case SirenVajsh:
				if (currentStatus == tiny || currentStatus == frog) break;
				dealWithShaman_Vajsh(theKnight, maxHP, i+1, SirenVajsh);
				break;
			case findExcalibur:
				currentWeapon = Excalibur;
				break;
			case findMythirl:
				isWearMythirl = true;
				break;
			case findExcalipoor:
				if (odinHelpLeft || isArthur || isLancelot && LancelotBehavior || isPaladin) {
					odinHelpLeft = odinHelpLeft - 1;
					useOdinHelpThisTurn = true;
				}
				else currentWeapon = theKnight.level >= 5 ? currentWeapon : Excalipoor;
				break;
			case MushMario:
				theKnight.HP = (theKnight.HP + 50) > maxHP ? maxHP : (theKnight.HP + 50);
				break;
			case MushFibo:
				theKnight.HP = (nextFibonacci(theKnight.HP) > maxHP) ? maxHP : nextFibonacci(theKnight.HP);
				break;
			case MushGhost:
				if (odinHelpLeft || isPaladin) {
					odinHelpLeft = odinHelpLeft - 1;
					useOdinHelpThisTurn = true;
				}
				else theKnight.HP = theKnight.HP < 51 ? 1 : (theKnight.HP - 50);
				break;
			case MushKnight:
				maxHP = (maxHP + 50) > 999 ? 999 : (maxHP + 50);
				theKnight.HP = maxHP;
				break;
			case Remedy:
				if (currentStatus == tiny) {
					currentStatus = normal;
					statusTime = 0;
					theKnight.HP = (theKnight.HP * 5) > maxHP ? maxHP : (theKnight.HP * 5);
				}
				else theKnight.remedy = (theKnight.remedy + 1) > 99 ? 99 : (theKnight.remedy + 1);				
				break;
			case MaidenKiss:
				if (currentStatus == frog) {
					currentStatus = normal;
					statusTime = 0;
					theKnight.level = levelBeforeChangeToFrog;
				}
				else theKnight.maidenkiss = (theKnight.maidenkiss + 1) > 99 ? 99 : (theKnight.maidenkiss + 1);
				break;
			case PhoenixDown:
				theKnight.phoenixdown = theKnight.phoenixdown + 1;
				break;
			case Merlin:
				if (currentStatus == tiny || currentStatus == frog) {
					theKnight.level = (currentStatus == frog) ? levelBeforeChangeToFrog : theKnight.level;
					currentStatus = normal;
					statusTime = 0;
				}
				theKnight.level = (theKnight.level + 1) > 10 ? 10 : (theKnight.level + 1);
				theKnight.HP = maxHP;
				break;
			case Abyss:
				if (odinHelpLeft) {
					odinHelpLeft = odinHelpLeft - 1;
					useOdinHelpThisTurn = true;
				}
				else if (theKnight.level < 7) {
					nOut = -1;
					return;
				} 
				break;
			case Guinevere:
				{	
					meetGuinevere = true;
					int numberOfEventPassed = i;
					int *arrEventInvert = new int[numberOfEventPassed];
					for (int index = 0; index < numberOfEventPassed; index++) {
						arrEventInvert[index] = arrEvent[i - 1];
						i--;
					}
					process(theKnight, numberOfEventPassed, arrEventInvert, nOut);
					delete[] arrEventInvert;
					return;
				}
			case LightWing:
				if (odinHelpLeft) odinHelpLeft = 0;
				if ((nEvent - i) < 3 || meetGuinevere) {
					nOut = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
					return;
				}
				else {
					for (int j = i + 1; j < i + 4; j++) {
						if (arrEvent[j] == GuinevereReturn || arrEvent[j] == Guinevere) {
							nOut = theKnight.HP + theKnight.level + theKnight.remedy + theKnight.maidenkiss + theKnight.phoenixdown;
							return; 
						}
					}
					i = i + 3;
					if (currentStatus) statusTime = statusTime + 3;
				}
				break;
			case Odin:
				if (!isHelpByOdin) {
					odinHelpLeft = 4;
					isHelpByOdin = true;
				}
				break;
			case findDragonSowrd:
				if (isDragonKnight) currentWeapon = DragonSowrd;
				break;
			case Bowser:
				if (isArthur || isLancelot || isPaladin && theKnight.level >= 8 || theKnight.level == 10) break;
				else {
					nOut = -1;
					return;
				}
		}
		statusCheck(statusTime, theKnight, maxHP);
		if(odinHelpLeft) odinHelpCheck();
		
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

	if (theKnight.HP == ArthurHP) isArthur = true;
	else if (theKnight.HP == LanceLotHP) isLancelot = true;
	else if (isPaladincheck(theKnight.HP)) isPaladin = true;
	process(theKnight, nEvent, arrEvent, nOut);
	//cout << theKnight.HP << " " << theKnight.level << " " << theKnight.remedy << " " << theKnight.maidenkiss << " " << theKnight.phoenixdown << '\n';
    
	display(nOut);
	return 0;
}