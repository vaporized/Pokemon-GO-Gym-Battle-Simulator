#include "formulas.h"


int formula_CP(double cpm, int bAtk, int bDef, int bStm, int iAtk, int iDef, int iStm) {
	int CP = (bAtk + iAtk)*sqrt((bDef + iDef)*(bStm + iStm))*cpm*cpm / 10;
	return CP > 10 ? CP : 10;
}

int formula_CP(double cAtk, double cDef, double cStm)
{
	int CP = (int)cAtk * sqrt(cDef * cStm) / 10;
	return CP > 10 ? CP : 10;
}


int formula_damage(int power, double cAtk, double cDef, double stab, double effe, bool dodged) {
	int raw_dmg = int(power * cAtk / cDef * stab * effe * 0.5) + 1;
	if (dodged) 
		return raw_dmg / DODGE_MODIFIER > 1 ? raw_dmg / DODGE_MODIFIER : 1;
	else 
		return raw_dmg;
	
}

int formula_prestige(int attacker_CP, int defender_CP)
{
	int prestige;
	if (attacker_CP > defender_CP) {
		prestige = int(310 * ((double)defender_CP / attacker_CP)) - 55;
	}
	else {
		prestige = int(500 * ((double)defender_CP / attacker_CP));
	}
	if (prestige < 100)
		prestige = 100;

	return prestige;
}

int formula_energy_gain(int HPLoss)
{
	return (HPLoss + 1) / 2;
}

double formula_cStat(double cpm, int bStat, int iStat)
{
	return (bStat + iStat)*cpm;
}

int formula_attackerHP(double cpm, int bStm, int iStm)
{
	int HP = (int)formula_cStat(cpm, bStm, iStm);
	return HP > 10 ? HP : 10;
}

int formula_defenderHP(double cpm, int bStm, int iStm)
{
	return 2 * formula_attackerHP(cpm, bStm, iStm);
}
