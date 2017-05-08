#ifndef FORMULA_H
#define FORMULA_H

#include <cmath>
#include <string>
#include "parameters.h"
using namespace std;



// The famous CP Formula
int formula_CP(double cpm, int bAtk, int bDef, int bStm, int iAtk, int iDef, int iStm);
int formula_CP(double cAtk, double cDef, double cStm);


// The equally famous Damage Formula
int formula_damage(int power, double cAtk, double cDef, double stab, double effe, bool dodge);



// Prestige formula
int formula_prestige(int attacker_CP, int defender_CP);



// How many energy is gained due to HP Loss
int formula_energy_gain(int HPLoss);



// Calculate the combat stat (used in damage formula or CP formula form II)
double formula_cStat(double cpm, int bStat, int iStat);


// Get HP for attacker and defender
int formula_attackerHP(double cpm, int bStm, int iStm);
int formula_defenderHP(double cpm, int bStm, int iStm);





#endif