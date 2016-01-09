// *****************************************************************
// * FONCTION: Calcule les points de base d'un personnage dans     *
// *           le jeu de role Advanced Dungeons and Dragons        *
// * FICHIER:  AD&DCC.CPP                                          *
// * PAR:      Marie-Jos‚e Blais                                   *
// * DATE:     janvier 1994                                        *
// *****************************************************************

#include <stdlib.h>
#include <iostream.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

#define BOOLEAN unsigned char
#define UBYTE unsigned char
#define BYTE  char
#define UWORD unsigned
#define WORD int
#define TRUE  1
#define FALSE 0
#define MALE  0
#define FEMALE 1
#define randomize srand( (UWORD)(biostime( 0, 0) / 24) )
//*****************************************************************
//*                     Class, struct et union                    *
//*****************************************************************
class ability
{
UBYTE Str;
UBYTE Excstr;
UBYTE Dex;
UBYTE Con;
UBYTE Int;
UBYTE Wis;
UBYTE Cha;

public:
ability(){}
void init ( UBYTE s, UBYTE ex, UBYTE d, UBYTE co, UBYTE i, UBYTE w, UBYTE ch )
{ Str = s; Excstr = ex; Dex = d; Con = co; Int = i; Wis = w; Cha = ch; }
void lancer_abilite_1();
void lancer_abilite_2();
void lancer_abilite_5();
void imprime()
{ cout << "Str: " << (int)Str << "Excstr: " << (int)Excstr
       << "Dex: " << (int)Dex << "Con: "    << (int)Con
       << "Int: " << (int)Int << "Wis: "    << (int)Wis
       << "Cha: " << (int)Cha << '\n'; }
friend character;
};

class strength
{
BYTE  hit_prob,
      dmg_adj;
UWORD wgt_all,
      light,
      moderate,
      heavy,
      severe,
      max_press;
UBYTE op_doors,
      op_mag_doors,
      bars_gates;

public:
friend tableau_ability;
strength(){}
void init( BYTE hp, BYTE da, UWORD wa, UWORD lt, UWORD mt,
	   UWORD hv, UWORD mp, UBYTE od, UBYTE omd, UBYTE bg )
{ hit_prob = hp; dmg_adj      = da ; wgt_all    = wa; light = lt;
  moderate = mt; heavy        = hv ; max_press  = mp;
  op_doors = od; op_mag_doors = omd; bars_gates = bg; }
void imprime()
{ cout << (int)hit_prob    << ' ' << (int)dmg_adj    << ' ' << wgt_all << ' '
       << max_press        << ' ' << (int)op_doors   << ' '
       << (int)op_mag_doors<< ' ' << (int)bars_gates << '\n';
}
};


class thief_skill
{
BYTE pick_pockets,
     op_locks,
     find_rem_traps,
     mov_silently,
     hide_shadow,
     det_noise,
     climb_walls,
     read_lang;
public:
thief_skill(){}
void init(BYTE pp,BYTE ol,BYTE ft,BYTE ms,BYTE hs,BYTE dn,BYTE cw, BYTE rl )
{
  pick_pockets = pp; op_locks  = ol; find_rem_traps = ft; mov_silently = ms;
  hide_shadow  = hs; det_noise = dn; climb_walls    = cw; read_lang    = rl;
}
};


class dexterity
{
BYTE        rct_adj,
	    miss_att_adj,
	    def_adj;
thief_skill thief;

public:
friend tableau_ability;
dexterity(){}
void init( BYTE ra, BYTE maa, BYTE da, BYTE pp,
	   BYTE ol, BYTE ft,  BYTE ms, BYTE hs )
{
  rct_adj = ra; miss_att_adj = maa; def_adj = da ;
  thief.init( pp, ol, ft, ms, hs, 0, 0, 0 );
}
void imprime()
{ cout << (int)rct_adj << ' ' << (int)miss_att_adj << ' '
       << (int)def_adj << '\n';
}
};

class constitution
{
BYTE  ht_pt_adj[2];
UBYTE syst_shock,
      res_surv;
BYTE  poison_sav;
UBYTE reg;

public:
friend tableau_ability;
constitution(){}
void init( BYTE hpa1, BYTE hpa2, UBYTE ss, UBYTE rs, BYTE ps, UBYTE r )
{ ht_pt_adj[0] = hpa1; ht_pt_adj[1] = hpa2; syst_shock = ss;
  res_surv = rs; poison_sav = ps; reg = r;
}
void imprime()
{ cout << (int)ht_pt_adj[0] << ' ' << (int)ht_pt_adj[1] << ' '
       << (int)syst_shock   << ' ' << (int)res_surv     << ' '
       << (int)poison_sav   << ' ' << (int)reg          << '\n';
}
};


class intelligence
{
UBYTE lang,
      spell_lvl,
      learn_spell,
      max_spell_lvl,
      spell_imm;
public:
friend tableau_ability;
intelligence(){}
void init( UBYTE l, UBYTE sl, UBYTE ls, UBYTE msl, UBYTE si )
{
  lang = l; spell_lvl = sl; learn_spell = ls;
  max_spell_lvl = msl; spell_imm = si;
}
void imprime()
{
  cout << (int)lang          << ' ' <<  (int)spell_lvl    << ' '
       << (int)learn_spell   << ' ' << (int)max_spell_lvl << ' '
       << (int)spell_imm << '\n';
}
};


class wisdom
{
BYTE  mag_def_adj;
UBYTE bon_spells1,
      bon_spells2,
      spell_fail;

public:

friend tableau_ability;
wisdom(){}
void init( BYTE mda, UBYTE bs1, UBYTE bs2, UBYTE sf )
{ mag_def_adj = mda; bon_spells1 = bs1; bon_spells2 = bs2; spell_fail = sf; }
void imprime()
{
  cout << (int)mag_def_adj << ' ' << (int)bon_spells1 << ' '
       << (int)bon_spells2 << ' ' << (int)spell_fail  << '\n';
}
};

class charisma
{
UBYTE henchmen;
BYTE  loyalty,
      react_adj;
public:
friend tableau_ability;
charisma(){}
void init( UBYTE hm, BYTE l, BYTE ra )
{ henchmen = hm; loyalty = l; react_adj = ra; }
void imprime()
{
  cout << (int)henchmen  << ' ' << (int)loyalty << ' '
       << (int)react_adj << '\n';
}
};

class caracteristique
{
UWORD base_male,
      base_female,
      no_des,
      type_des;
public:
caracteristique(){}
void init( UWORD bm, UWORD bf, UWORD nd, UWORD td )
{ base_male = bm; base_female = bf; no_des = nd; type_des = td; }
};

class movement
{
UWORD light,
      moderate,
      heavy,
      severe;
public:
movement(){}
};

class sav_throw
{
UBYTE par_pois_death,
      rod_staff_wand,
      pet_poly,
      breath_weapon,
      spells;
public:
sav_throw(){}
};


class priest_spell
{
BYTE level_caster;
BYTE level_spell[7];
public:
priest_spell(){}
void init( BYTE lc, BYTE l1, BYTE l2, BYTE l3,
		  BYTE l4, BYTE l5, BYTE l6, BYTE l7)
{
level_caster = lc;
level_spell[0] = l1; level_spell[1] = l2; level_spell[2] = l3;
level_spell[3] = l4; level_spell[4] = l5; level_spell[5] = l6;
level_spell[6] = l7;
}
};


class wizard_spell
{
BYTE level_spell[9];
public:
wizard_spell(){}
void init( BYTE l1, BYTE l2, BYTE l3, BYTE l4, BYTE l5,
		  BYTE l6, BYTE l7, BYTE l8, BYTE l9 )
{ level_spell[0] = l1; level_spell[1] = l2; level_spell[2] = l3;
  level_spell[3] = l4; level_spell[4] = l5; level_spell[5] = l6;
  level_spell[6] = l7; level_spell[7] = l8; level_spell[8] = l9;
}
};


class hit_dice
{
UBYTE dice,
      level,
      increment;
public:
hit_dice(){}
void init( UBYTE d, UBYTE l, UBYTE i )
{ dice = d; level = l; increment = i; }
};


class prof_slots
{
BYTE wp_init,
     wp_level,
     wp_penalty,
     nwp_init,
     nwp_level;
public:
prof_slots(){}
void init( BYTE wpi, BYTE wpl, BYTE wpp, BYTE nwpi, BYTE nwpl )
{ wp_init=wpi; wp_level=wpl; wp_penalty=wpp; nwp_init=nwpi; nwp_level=nwpl; }
};


class race
{
public:
char            nom[40];
ability         req_min,
		req_max,
		adj;
BYTE            const_st;
UBYTE		b_mov_rate;
sav_throw	modifier;
caracteristique height,
		weigth,
		age,
		max_age;
thief_skill     thief_race_adj;
BYTE            armor_class;
public:
race(){}
BYTE init( char *name, UBYTE fonction, BYTE value )
{
for( UBYTE i = 0; i < 6; i++ )
{
  if ( !strcmp( RACE[i].nom, name ) )
  {
    switch( fonction )
    {
    case 1 : if( value > RACE.
    }
  }
}
}
};


class class_def
{
public:
char      name[40];
ability   requirements;
sav_throw sav_throw_modifier;
char      races_all[40];
BYTE      thief_skills;
BYTE      back_stab;
BYTE      priest_spell;
BYTE      spheres;
BYTE      mage_spell;
BYTE      learn_spell;
BYTE      psionic_powers;
BYTE      leader;
BYTE      followers;
BYTE      elite_units;
BYTE      melee_att;
BYTE      exp_level1;
BYTE      exp_level2;
BYTE      exp_level3;
BYTE      hit_dice;
public:
class_def(){}
};

class character
{
public:
char          joueur[40],
	      personnage[40];
ability       Abi;
strength Str;
dexterity Dex;
constitution Con;
intelligence Int;
wisdom Wis;
charisma Cha;
char race[40];
BOOLEAN       sexe;
UWORD         age,
	      grandeur,
	      poids;
char          classe[40],
	      sous_classe[40];
UBYTE         level1,
	      level2,
	      level3;
UWORD         pt_vie;
unsigned long argent;
public:
character(){}
void imprime( void )
{ Abi.imprime(); Str.imprime(); Dex.imprime(); Con.imprime();
  Int.imprime(); Wis.imprime(); Cha.imprime();
  grandeur =  }
void input( void );
void player_ability( ability );
};


//*****************************************************************
//*                     Prototypes de fonctions                   *
//*****************************************************************
UWORD lancer_des( UBYTE, UBYTE,
		     UBYTE* );
void init( void );
race load_race( BYTE* );
//classe load_classe( BYTE* );
//tableau_ability calcule_tableau_abilite( class ability );
//void imprime( class character );
//*****************************************************************
//*                     VARIABLES GLOBALES                        *
//*****************************************************************
strength     STRENGTH[30];
dexterity    DEXTERITY[25];
constitution CONSTITUTION[25];
intelligence INTELLIGENCE[25];
wisdom       WISDOM[25];
charisma     CHARISMA[25];
race         RACE_RECORD;
UWORD        EXP_LVL[7][20];
priest_spell PRIEST_SPELL[4][20];
wizard_spell WIZARD_SPELL[2][20];
hit_dice     HIT_DICE[4];
prof_slots   PROF_SLOTS[5];
race         RACE[6];
thief_skill  THIEF_SKILL_BASE_SCORE;

//*****************************************************************
//*                            MAIN                               *
//*****************************************************************
void main( void )
{
clrscr();
init();
character player1;
player1.input();
player1.Abi.lancer_abilite_1();
player1.player_ability( player1.Abi );
player1.imprime();
getch();
}

UWORD lancer_des( UBYTE nombre, UBYTE face,
		  UBYTE* plus_petit )
{
UBYTE i, des;
UWORD total;

total = 0;
*plus_petit = 0;
for( i = 0; i < nombre; i++ )
{
  des = random( face ) + 1;
  if( *plus_petit == 0 )
    (*plus_petit) = des;
  else
    if( *plus_petit > des )
      *plus_petit = des;
  total = total + des;
}
return( total );
}


void ability::lancer_abilite_1( void )
{
BYTE temp;

Str    = lancer_des( 3, 6, &temp );
Excstr = (Str == 18) ? lancer_des( 1, 100, &temp ) : 0;
Dex    = lancer_des( 3, 6, &temp );
Con    = lancer_des( 3, 6, &temp );
Int    = lancer_des( 3, 6, &temp );
Wis    = lancer_des( 3, 6, &temp );
Cha    = lancer_des( 3, 6, &temp );
}


void ability::lancer_abilite_2( void )
{
BYTE temp, a, b;

Str    = (a = lancer_des( 3, 6, &temp )) >
	 (b = lancer_des( 3, 6, &temp )) ? a : b;
Excstr = Excstr = (Str == 18) ? lancer_des( 1, 100, &temp ) : 0;
Dex    = (a = lancer_des( 3, 6, &temp )) >
	 (b = lancer_des( 3, 6, &temp )) ? a : b;
Con    = (a = lancer_des( 3, 6, &temp )) >
	 (b = lancer_des( 3, 6, &temp )) ? a : b;
Int    = (a = lancer_des( 3, 6, &temp )) >
	 (b = lancer_des( 3, 6, &temp )) ? a : b;
Wis    = (a = lancer_des( 3, 6, &temp )) >
	 (b = lancer_des( 3, 6, &temp )) ? a : b;
Cha    = (a = lancer_des( 3, 6, &temp )) >
	 (b = lancer_des( 3, 6, &temp )) ? a : b;
}


void ability::lancer_abilite_5( void )
{
BYTE temp,
     minimum;

Str    = lancer_des( 4, 6, &minimum ) - minimum;
Excstr = Excstr = (Str == 18) ? lancer_des( 1, 100, &temp ) : 0;
Dex    = lancer_des( 4, 6, &minimum ) - minimum;
Con    = lancer_des( 4, 6, &minimum ) - minimum;
Int    = lancer_des( 4, 6, &minimum ) - minimum;
Wis    = lancer_des( 4, 6, &minimum ) - minimum;
Cha    = lancer_des( 4, 6, &minimum ) - minimum;
}


void character::input( void )
{
cout<<"Joueur: ";
cin>>this->joueur;
cout<<"Personnage: ";
cin>>this->personnage;
cout<<"Race: ";
cin>>this->race;
cout<<"Sexe: ";
char sexe;
cin>>sexe;
if(sexe == 'M' || sexe == 'm')
  this->sexe = MALE;
else if(sexe == 'F' || sexe == 'f')
  this->sexe = FEMALE;
cout<<"Classe: ";
cin>>this->classe;
cout<<"Sous_classe: ";
cin>>this->sous_classe;
}


void character::player_ability( ability player )
{
switch( player.Str )
{
case 18:
	if ( player.Excstr == 0 )
	  this->Str = STRENGTH[18];
	else if ( player.Excstr <=50 )
	  this->Str = STRENGTH[19];
	else if ( player.Excstr <= 75)
	  this->Str = STRENGTH[20];
	else if ( player.Excstr <= 90)
	  this->Str = STRENGTH[21];
	else if ( player.Excstr <= 99)
	  this->Str = STRENGTH[22];
	else if ( player.Excstr == 100)
	  this->Str = STRENGTH[23];
	break;
default:
	if (player.Str < 18 )
	  this->Str = STRENGTH[player.Str - 1];
	else
	  this->Str = STRENGTH[player.Str + 5];
}
this->Dex = DEXTERITY[player.Dex-1];
this->Con = CONSTITUTION[player.Con-1];
this->Int = INTELLIGENCE[player.Int-1];
this->Wis = WISDOM[player.Wis-1];
this->Cha = CHARISMA[player.Cha-1];
}

void init( void )
{
randomize;
STRENGTH[ 0].init( -5, -4,    1,    1,    1,    2,    3,  1,  0,  0 );
STRENGTH[ 1].init( -3, -2,    1,    2,    3,    4,    5,  1,  0,  0 );
STRENGTH[ 2].init( -3, -1,    5,    6,    7,    9,   10,  2,  0,  0 );
STRENGTH[ 3].init( -2, -1,   10,   13,   16,   19,   25,  3,  0,  0 );
STRENGTH[ 4].init( -2, -1,   10,   13,   16,   19,   25,  3,  0,  0 );
STRENGTH[ 5].init( -1,  0,   20,   29,   38,   46,   55,  4,  0,  0 );
STRENGTH[ 6].init( -1,  0,   20,   29,   38,   46,   55,  4,  0,  0 );
STRENGTH[ 7].init(  0,  0,   35,   50,   65,   80,   90,  5,  0,  1 );
STRENGTH[ 8].init(  0,  0,   35,   50,   65,   80,   90,  5,  0,  1 );
STRENGTH[ 9].init(  0,  0,   40,   58,   76,   96,  115,  6,  0,  2 );
STRENGTH[10].init(  0,  0,   40,   58,   76,   96,  115,  6,  0,  2 );
STRENGTH[11].init(  0,  0,   45,   69,   93,  117,  140,  7,  0,  4 );
STRENGTH[12].init(  0,  0,   45,   69,   93,  117,  140,  7,  0,  4 );
STRENGTH[13].init(  0,  0,   55,   85,  115,  145,  170,  8,  0,  7 );
STRENGTH[14].init(  0,  0,   55,   85,  115,  145,  170,  8,  0,  7 );
STRENGTH[15].init(  0,  1,   70,  100,  130,  160,  195,  9,  0, 10 );
STRENGTH[16].init(  1,  1,   85,  121,  157,  193,  220, 10,  0, 13 );
STRENGTH[17].init(  1,  2,  110,  149,  188,  227,  255, 11,  0, 16 );
STRENGTH[18].init(  1,  3,  135,  174,  213,  252,  280, 12,  0, 20 );
STRENGTH[19].init(  2,  3,  160,  199,  238,  277,  305, 13,  0, 25 );
STRENGTH[20].init(  2,  4,  185,  224,  263,  302,  330, 14,  0, 30 );
STRENGTH[21].init(  2,  5,  235,  274,  313,  352,  380, 15,  3, 35 );
STRENGTH[22].init(  3,  6,  335,  374,  413,  452,  480, 16,  6, 40 );
STRENGTH[23].init(  3,  7,  485,  523,  561,  599,  640, 16,  8, 50 );
STRENGTH[24].init(  3,  8,  535,  576,  617,  658,  700, 17, 10, 60 );
STRENGTH[25].init(  4,  9,  635,  678,  721,  764,  810, 17, 12, 70 );
STRENGTH[26].init(  4, 10,  785,  831,  877,  923,  970, 18, 14, 80 );
STRENGTH[27].init(  5, 11,  935,  983, 1031, 1079, 1130, 18, 16, 90 );
STRENGTH[28].init(  6, 12, 1235, 1286, 1337, 1388, 1440, 19, 17, 95 );
STRENGTH[29].init(  7, 14, 1535, 1588, 1641, 1694, 1750, 19, 18, 99 );

DEXTERITY[ 0].init( -6, -6,  5, -99, -99, -99, -99, -99 );
DEXTERITY[ 1].init( -4, -4,  5, -99, -99, -99, -99, -99 );
DEXTERITY[ 2].init( -3, -3,  4, -99, -99, -99, -99, -99 );
DEXTERITY[ 3].init( -2, -2,  3, -99, -99, -99, -99, -99 );
DEXTERITY[ 4].init( -1, -1,  2, -99, -99, -99, -99, -99 );
DEXTERITY[ 5].init(  0,  0,  1, -99, -99, -99, -99, -99 );
DEXTERITY[ 6].init(  0,  0,  0, -99, -99, -99, -99, -99 );
DEXTERITY[ 8].init(  0,  0,  0, -15, -10, -10, -20, -10 );
DEXTERITY[ 9].init(  0,  0,  0, -10,  -5, -10, -15,  -5 );
DEXTERITY[10].init(  0,  0,  0,  -5,   0,  -5, -10,   0 );
DEXTERITY[11].init(  0,  0,  0,   0,   0,   0,  -5,   0 );
DEXTERITY[12].init(  0,  0,  0,   0,   0,   0,   0,   0 );
DEXTERITY[13].init(  0,  0,  0,   0,   0,   0,   0,   0 );
DEXTERITY[14].init(  0,  0, -1,   0,   0,   0,   0,   0 );
DEXTERITY[15].init(  1,  1, -2,   0,   5,   0,   0,   0 );
DEXTERITY[16].init(  2,  2, -3,   5,  10,   0,   5,   5 );
DEXTERITY[17].init(  2,  2, -4,  10,  15,   5,  10,  10 );
DEXTERITY[18].init(  3,  3, -4,  15,  20,  10,  15,  15 );
DEXTERITY[19].init(  3,  3, -4,  15,  20,  10,  15,  15 );
DEXTERITY[20].init(  4,  4, -5,  15,  20,  10,  15,  15 );
DEXTERITY[21].init(  4,  4, -5,  15,  20,  10,  15,  15 );
DEXTERITY[22].init(  4,  4, -5,  15,  20,  10,  15,  15 );
DEXTERITY[23].init(  5,  5, -6,  15,  20,  10,  15,  15 );
DEXTERITY[24].init(  5,  5, -6,  15,  20,  10,  15,  15 );

CONSTITUTION[ 0].init( -3, 0,  25,  30, -2, 0 );
CONSTITUTION[ 1].init( -2, 0,  30,  35, -1, 0 );
CONSTITUTION[ 2].init( -2, 0,  35,  40,  0, 0 );
CONSTITUTION[ 3].init( -1, 0,  40,  45,  0, 0 );
CONSTITUTION[ 4].init( -1, 0,  45,  50,  0, 0 );
CONSTITUTION[ 5].init( -1, 0,  50,  55,  0, 0 );
CONSTITUTION[ 6].init(  0, 0,  55,  60,  0, 0 );
CONSTITUTION[ 7].init(  0, 0,  60,  65,  0, 0 );
CONSTITUTION[ 8].init(  0, 0,  65,  70,  0, 0 );
CONSTITUTION[ 9].init(  0, 0,  70,  75,  0, 0 );
CONSTITUTION[10].init(  0, 0,  75,  80,  0, 0 );
CONSTITUTION[11].init(  0, 0,  80,  85,  0, 0 );
CONSTITUTION[12].init(  0, 0,  85,  90,  0, 0 );
CONSTITUTION[13].init(  0, 0,  88,  92,  0, 0 );
CONSTITUTION[14].init(  1, 0,  90,  94,  0, 0 );
CONSTITUTION[15].init(  2, 0,  95,  96,  0, 0 );
CONSTITUTION[16].init(  2, 3,  97,  98,  0, 0 );
CONSTITUTION[17].init(  2, 4,  99, 100,  0, 0 );
CONSTITUTION[18].init(  2, 5,  99, 100,  1, 0 );
CONSTITUTION[19].init(  2, 5,  99, 100,  1, 6 );
CONSTITUTION[20].init(  2, 6,  99, 100,  2, 5 );
CONSTITUTION[21].init(  2, 6,  99, 100,  2, 4 );
CONSTITUTION[22].init(  2, 6,  99, 100,  3, 3 );
CONSTITUTION[23].init(  2, 7,  99, 100,  3, 2 );
CONSTITUTION[24].init(  2, 7, 100, 100,  4, 1 );

INTELLIGENCE[ 0].init(  0, 0,   0,   0, 0 );
INTELLIGENCE[ 1].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 2].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 3].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 4].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 5].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 6].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 7].init(  1, 0,   0,   0, 0 );
INTELLIGENCE[ 8].init(  2, 4,  35,   6, 0 );
INTELLIGENCE[ 9].init(  2, 5,  40,   7, 0 );
INTELLIGENCE[10].init(  2, 5,  45,   7, 0 );
INTELLIGENCE[11].init(  3, 6,  50,   7, 0 );
INTELLIGENCE[12].init(  3, 6,  55,   9, 0 );
INTELLIGENCE[13].init(  4, 7,  60,   9, 0 );
INTELLIGENCE[14].init(  4, 7,  65,  11, 0 );
INTELLIGENCE[15].init(  5, 8,  70,  11, 0 );
INTELLIGENCE[16].init(  6, 8,  75,  14, 0 );
INTELLIGENCE[17].init(  7, 9,  85,  18, 0 );
INTELLIGENCE[18].init(  8, 9,  95, 127, 1 );
INTELLIGENCE[19].init(  9, 9,  96, 127, 2 );
INTELLIGENCE[20].init( 10, 9,  97, 127, 3 );
INTELLIGENCE[21].init( 11, 9,  98, 127, 4 );
INTELLIGENCE[22].init( 12, 9,  99, 127, 5 );
INTELLIGENCE[23].init( 15, 9, 100, 127, 6 );
INTELLIGENCE[24].init( 20, 9, 100, 127, 7 );

WISDOM[ 0].init( -6, 0, 0, 80 );
WISDOM[ 1].init( -4, 0, 0, 60 );
WISDOM[ 2].init( -3, 0, 0, 50 );
WISDOM[ 3].init( -2, 0, 0, 45 );
WISDOM[ 4].init( -1, 0, 0, 40 );
WISDOM[ 5].init( -1, 0, 0, 35 );
WISDOM[ 6].init( -1, 0, 0, 30 );
WISDOM[ 7].init(  0, 0, 0, 25 );
WISDOM[ 8].init(  0, 0, 0, 20 );
WISDOM[ 9].init(  0, 0, 0, 15 );
WISDOM[10].init(  0, 0, 0, 10 );
WISDOM[11].init(  0, 0, 0,  5 );
WISDOM[12].init(  0, 1, 0,  0 );
WISDOM[13].init(  0, 1, 0,  0 );
WISDOM[14].init(  1, 2, 0,  0 );
WISDOM[15].init(  2, 2, 0,  0 );
WISDOM[16].init(  3, 3, 0,  0 );
WISDOM[17].init(  4, 4, 0,  0 );
WISDOM[18].init(  4, 1, 4,  0 );
WISDOM[19].init(  4, 2, 4,  0 );
WISDOM[20].init(  4, 3, 5,  0 );
WISDOM[21].init(  4, 4, 5,  0 );
WISDOM[22].init(  4, 5, 5,  0 );
WISDOM[23].init(  4, 6, 6,  0 );
WISDOM[24].init(  4, 6, 7,  0 );

CHARISMA[ 0].init(   0, -8, -7 );
CHARISMA[ 1].init(   1, -7, -6 );
CHARISMA[ 2].init(   1, -6, -5 );
CHARISMA[ 3].init(   1, -5, -4 );
CHARISMA[ 4].init(   2, -4, -3 );
CHARISMA[ 5].init(   2, -3, -2 );
CHARISMA[ 6].init(   3, -2, -1 );
CHARISMA[ 7].init(   3, -1,  0 );
CHARISMA[ 8].init(   4,  0,  0 );
CHARISMA[ 9].init(   4,  0,  0 );
CHARISMA[10].init(   4,  0,  0 );
CHARISMA[11].init(   5,  0,  0 );
CHARISMA[12].init(   5,  0,  1 );
CHARISMA[13].init(   6,  1,  2 );
CHARISMA[14].init(   7,  3,  3 );
CHARISMA[15].init(   8,  4,  5 );
CHARISMA[16].init(  10,  6,  6 );
CHARISMA[17].init(  15,  8,  7 );
CHARISMA[18].init(  20,  10, 8 );
CHARISMA[19].init(  25,  12, 9 );
CHARISMA[20].init(  30,  14, 10 );
CHARISMA[21].init(  35,  16, 11 );
CHARISMA[22].init(  40,  18, 12 );
CHARISMA[23].init(  45,  20, 13 );
CHARISMA[24].init(  50,  20, 14 );

EXP_LVL[ 0][ 0] =       0;
EXP_LVL[ 0][ 1] =    2000;
EXP_LVL[ 0][ 2] =    4000;
EXP_LVL[ 0][ 3] =    8000;
EXP_LVL[ 0][ 4] =   16000;
EXP_LVL[ 0][ 5] =   32000;
EXP_LVL[ 0][ 6] =   64000;
EXP_LVL[ 0][ 7] =  125000;
EXP_LVL[ 0][ 8] =  250000;
EXP_LVL[ 0][ 9] =  500000;
EXP_LVL[ 0][10] =  750000;
EXP_LVL[ 0][11] = 1000000;
EXP_LVL[ 0][12] = 1250000;
EXP_LVL[ 0][13] = 1500000;
EXP_LVL[ 0][14] = 1750000;
EXP_LVL[ 0][15] = 2000000;
EXP_LVL[ 0][16] = 2250000;
EXP_LVL[ 0][17] = 2500000;
EXP_LVL[ 0][18] = 2750000;
EXP_LVL[ 0][19] = 3000000;

EXP_LVL[ 1][ 0] =       0;
EXP_LVL[ 1][ 1] =    2250;
EXP_LVL[ 1][ 2] =    4500;
EXP_LVL[ 1][ 3] =    9000;
EXP_LVL[ 1][ 4] =   18000;
EXP_LVL[ 1][ 5] =   36000;
EXP_LVL[ 1][ 6] =   75000;
EXP_LVL[ 1][ 7] =  150000;
EXP_LVL[ 1][ 8] =  300000;
EXP_LVL[ 1][ 9] =  600000;
EXP_LVL[ 1][10] =  900000;
EXP_LVL[ 1][11] = 1200000;
EXP_LVL[ 1][12] = 1500000;
EXP_LVL[ 1][13] = 1800000;
EXP_LVL[ 1][14] = 2100000;
EXP_LVL[ 1][15] = 2400000;
EXP_LVL[ 1][16] = 2700000;
EXP_LVL[ 1][17] = 3000000;
EXP_LVL[ 1][18] = 3300000;
EXP_LVL[ 1][19] = 3600000;

EXP_LVL[ 2][ 0] =       0;
EXP_LVL[ 2][ 1] =    2500;
EXP_LVL[ 2][ 2] =    5000;
EXP_LVL[ 2][ 3] =   10000;
EXP_LVL[ 2][ 4] =   20000;
EXP_LVL[ 2][ 5] =   40000;
EXP_LVL[ 2][ 6] =   60000;
EXP_LVL[ 2][ 7] =   90000;
EXP_LVL[ 2][ 8] =  135000;
EXP_LVL[ 2][ 9] =  250000;
EXP_LVL[ 2][10] =  375000;
EXP_LVL[ 2][11] =  750000;
EXP_LVL[ 2][12] = 1125000;
EXP_LVL[ 2][13] = 1500000;
EXP_LVL[ 2][14] = 1875000;
EXP_LVL[ 2][15] = 2250000;
EXP_LVL[ 2][16] = 2625000;
EXP_LVL[ 2][17] = 3000000;
EXP_LVL[ 2][18] = 3375000;
EXP_LVL[ 2][19] = 3750000;

EXP_LVL[ 3][ 0] =       0;
EXP_LVL[ 3][ 1] =    1500;
EXP_LVL[ 3][ 2] =    3000;
EXP_LVL[ 3][ 3] =    6000;
EXP_LVL[ 3][ 4] =   13000;
EXP_LVL[ 3][ 5] =   27500;
EXP_LVL[ 3][ 6] =   55000;
EXP_LVL[ 3][ 7] =  110000;
EXP_LVL[ 3][ 8] =  225000;
EXP_LVL[ 3][ 9] =  450000;
EXP_LVL[ 3][10] =  675000;
EXP_LVL[ 3][11] =  900000;
EXP_LVL[ 3][12] = 1125000;
EXP_LVL[ 3][13] = 1350000;
EXP_LVL[ 3][14] = 1575000;
EXP_LVL[ 3][15] = 1800000;
EXP_LVL[ 3][16] = 2700000;
EXP_LVL[ 3][17] = 2025000;
EXP_LVL[ 3][18] = 2475000;
EXP_LVL[ 3][19] = 2700000;

EXP_LVL[ 4][ 0] =       0;
EXP_LVL[ 4][ 1] =    2000;
EXP_LVL[ 4][ 2] =    4000;
EXP_LVL[ 4][ 3] =    7500;
EXP_LVL[ 4][ 4] =   12500;
EXP_LVL[ 4][ 5] =   20000;
EXP_LVL[ 4][ 6] =   35000;
EXP_LVL[ 4][ 7] =   60000;
EXP_LVL[ 4][ 8] =   90000;
EXP_LVL[ 4][ 9] =  125000;
EXP_LVL[ 4][10] =  200000;
EXP_LVL[ 4][11] =  300000;
EXP_LVL[ 4][12] =  750000;
EXP_LVL[ 4][13] = 1500000;
EXP_LVL[ 4][14] = 3000000;
EXP_LVL[ 4][15] = 3500000;
EXP_LVL[ 4][16] =  500000;
EXP_LVL[ 4][17] = 1000000;
EXP_LVL[ 4][18] = 1500000;
EXP_LVL[ 4][19] = 2000000;

EXP_LVL[ 5][ 0] =       0;
EXP_LVL[ 5][ 1] =    1250;
EXP_LVL[ 5][ 2] =    2500;
EXP_LVL[ 5][ 3] =    5000;
EXP_LVL[ 5][ 4] =   10000;
EXP_LVL[ 5][ 5] =   20000;
EXP_LVL[ 5][ 6] =   40000;
EXP_LVL[ 5][ 7] =   70000;
EXP_LVL[ 5][ 8] =  110000;
EXP_LVL[ 5][ 9] =  160000;
EXP_LVL[ 5][10] =  220000;
EXP_LVL[ 5][11] =  440000;
EXP_LVL[ 5][12] =  660000;
EXP_LVL[ 5][13] =  880000;
EXP_LVL[ 5][14] = 1100000;
EXP_LVL[ 5][15] = 1320000;
EXP_LVL[ 5][16] = 1540000;
EXP_LVL[ 5][17] = 1760000;
EXP_LVL[ 5][18] = 1980000;
EXP_LVL[ 5][19] = 2200000;

EXP_LVL[ 6][ 0] =       0;
EXP_LVL[ 6][ 1] =    2200;
EXP_LVL[ 6][ 2] =    4400;
EXP_LVL[ 6][ 3] =    8800;
EXP_LVL[ 6][ 4] =   16500;
EXP_LVL[ 6][ 5] =   30000;
EXP_LVL[ 6][ 6] =   55000;
EXP_LVL[ 6][ 7] =  100000;
EXP_LVL[ 6][ 8] =  200000;
EXP_LVL[ 6][ 9] =  400000;
EXP_LVL[ 6][10] =  600000;
EXP_LVL[ 6][11] =  800000;
EXP_LVL[ 6][12] = 1000000;
EXP_LVL[ 6][13] = 1200000;
EXP_LVL[ 6][14] = 1800000;
EXP_LVL[ 6][15] = 1800000;
EXP_LVL[ 6][16] = 2100000;
EXP_LVL[ 6][17] = 2400000;
EXP_LVL[ 6][18] = 2700000;
EXP_LVL[ 6][19] = 3000000;

PRIEST_SPELL[ 0][ 0].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 1].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 2].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 3].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 4].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 5].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 6].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 7].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 8].init(  1, 1, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][ 9].init(  2, 2, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][10].init(  3, 2, 1, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][11].init(  4, 2, 2, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][12].init(  5, 2, 2, 1, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][13].init(  6, 3, 2, 1, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][14].init(  7, 3, 2, 1, 0, 0, 0, 0 );
PRIEST_SPELL[ 0][15].init(  8, 3, 3, 2, 1, 0, 0, 0 );
PRIEST_SPELL[ 0][16].init(  9, 3, 3, 3, 1, 0, 0, 0 );
PRIEST_SPELL[ 0][17].init(  9, 3, 3, 3, 1, 0, 0, 0 );
PRIEST_SPELL[ 0][18].init(  9, 3, 3, 3, 2, 0, 0, 0 );
PRIEST_SPELL[ 0][19].init(  9, 3, 3, 3, 3, 0, 0, 0 );

PRIEST_SPELL[ 1][ 0].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 1].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 2].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 3].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 4].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 5].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 6].init(  0, 0, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 7].init(  1, 1, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 8].init(  2, 2, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][ 9].init(  3, 2, 1, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][10].init(  4, 2, 2, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][11].init(  5, 2, 2, 1, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][12].init(  6, 3, 2, 1, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][13].init(  7, 3, 2, 2, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][14].init(  7, 3, 2, 1, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][15].init(  8, 3, 3, 2, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][16].init(  9, 3, 3, 3, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][17].init(  9, 3, 3, 3, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][18].init(  9, 3, 3, 3, 0, 0, 0, 0 );
PRIEST_SPELL[ 1][19].init(  9, 3, 3, 3, 0, 0, 0, 0 );

PRIEST_SPELL[ 2][ 0].init(  1, 1, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 2][ 1].init(  2, 2, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 2][ 2].init(  3, 2, 1, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 2][ 3].init(  4, 3, 2, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 2][ 4].init(  5, 3, 3, 2, 0, 0, 0, 0 );
PRIEST_SPELL[ 2][ 5].init(  6, 3, 3, 2, 0, 0, 0, 0 );
PRIEST_SPELL[ 2][ 6].init(  7, 3, 3, 2, 1, 0, 0, 0 );
PRIEST_SPELL[ 2][ 7].init(  8, 3, 3, 3, 2, 0, 0, 0 );
PRIEST_SPELL[ 2][ 8].init(  9, 4, 4, 3, 2, 1, 0, 0 );
PRIEST_SPELL[ 2][ 9].init( 10, 4, 4, 3, 2, 2, 0, 0 );
PRIEST_SPELL[ 2][10].init( 11, 5, 4, 4, 3, 2, 1, 0 );
PRIEST_SPELL[ 2][11].init( 12, 6, 5, 5, 3, 2, 2, 0 );
PRIEST_SPELL[ 2][12].init( 13, 6, 6, 6, 4, 2, 2, 0 );
PRIEST_SPELL[ 2][13].init( 14, 6, 6, 6, 5, 3, 2, 1 );
PRIEST_SPELL[ 2][14].init( 15, 6, 6, 6, 6, 4, 2, 1 );
PRIEST_SPELL[ 2][15].init( 16, 7, 7, 7, 6, 4, 3, 1 );
PRIEST_SPELL[ 2][16].init( 17, 7, 7, 7, 7, 5, 3, 2 );
PRIEST_SPELL[ 2][17].init( 18, 8, 8, 8, 8, 6, 4, 2 );
PRIEST_SPELL[ 2][18].init( 19, 9, 9, 8, 8, 6, 4, 2 );
PRIEST_SPELL[ 2][19].init( 20, 9, 9, 9, 8, 7, 5, 2 );

PRIEST_SPELL[ 3][ 0].init(  1, 1, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 3][ 1].init(  2, 2, 0, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 3][ 2].init(  3, 2, 1, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 3][ 3].init(  4, 3, 2, 0, 0, 0, 0, 0 );
PRIEST_SPELL[ 3][ 4].init(  5, 3, 3, 2, 0, 0, 0, 0 );
PRIEST_SPELL[ 3][ 5].init(  6, 3, 3, 2, 0, 0, 0, 0 );
PRIEST_SPELL[ 3][ 6].init(  7, 3, 3, 2, 1, 0, 0, 0 );
PRIEST_SPELL[ 3][ 7].init(  8, 3, 3, 3, 2, 0, 0, 0 );
PRIEST_SPELL[ 3][ 8].init(  9, 4, 4, 3, 2, 1, 0, 0 );
PRIEST_SPELL[ 3][ 9].init( 10, 4, 4, 3, 2, 2, 0, 0 );
PRIEST_SPELL[ 3][10].init( 11, 5, 4, 4, 3, 2, 1, 0 );
PRIEST_SPELL[ 3][11].init( 12, 6, 5, 5, 3, 2, 2, 0 );
PRIEST_SPELL[ 3][12].init( 13, 6, 6, 6, 4, 2, 2, 0 );
PRIEST_SPELL[ 3][13].init( 14, 6, 6, 6, 5, 3, 2, 1 );
PRIEST_SPELL[ 3][14].init( 15, 6, 6, 6, 6, 6, 6, 6 );
PRIEST_SPELL[ 3][15].init( 14, 6, 6, 6, 5, 3, 2, 1 );
PRIEST_SPELL[ 3][16].init( 14, 6, 6, 6, 5, 3, 2, 1 );
PRIEST_SPELL[ 3][17].init( 14, 6, 6, 6, 5, 3, 2, 1 );
PRIEST_SPELL[ 3][18].init( 14, 6, 6, 6, 5, 3, 2, 1 );
PRIEST_SPELL[ 3][19].init( 14, 6, 6, 6, 5, 3, 2, 1 );

WIZARD_SPELL[ 0][ 0].init(  1, 0, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 1].init(  2, 0, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 2].init(  2, 1, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 3].init(  3, 2, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 4].init(  4, 2, 1, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 5].init(  4, 2, 2, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 6].init(  4, 3, 2, 1, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 7].init(  4, 3, 3, 2, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 8].init(  4, 3, 3, 2, 1, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][ 9].init(  4, 4, 3, 2, 2, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][10].init(  4, 4, 4, 3, 3, 0, 0, 0, 0 );
WIZARD_SPELL[ 0][11].init(  4, 4, 4, 4, 4, 1, 0, 0, 0 );
WIZARD_SPELL[ 0][12].init(  5, 5, 5, 4, 4, 2, 0, 0, 0 );
WIZARD_SPELL[ 0][13].init(  5, 5, 5, 4, 4, 2, 1, 0, 0 );
WIZARD_SPELL[ 0][14].init(  5, 5, 5, 5, 5, 2, 1, 0, 0 );
WIZARD_SPELL[ 0][15].init(  5, 5, 5, 5, 5, 3, 2, 1, 0 );
WIZARD_SPELL[ 0][16].init(  5, 5, 5, 5, 5, 3, 3, 2, 0 );
WIZARD_SPELL[ 0][17].init(  5, 5, 5, 5, 5, 3, 3, 2, 1 );
WIZARD_SPELL[ 0][18].init(  5, 5, 5, 5, 5, 3, 3, 3, 1 );
WIZARD_SPELL[ 0][19].init(  5, 5, 5, 5, 5, 4, 3, 3, 2 );

WIZARD_SPELL[ 1][ 0].init(  0, 0, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 1].init(  1, 0, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 2].init(  2, 0, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 3].init(  2, 1, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 4].init(  3, 1, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 5].init(  3, 2, 0, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 6].init(  3, 2, 1, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 7].init(  3, 3, 1, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 8].init(  3, 3, 2, 0, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][ 9].init(  3, 3, 2, 1, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][10].init(  3, 3, 3, 1, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][11].init(  3, 3, 3, 2, 0, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][12].init(  3, 3, 3, 2, 1, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][13].init(  3, 3, 3, 3, 1, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][14].init(  3, 3, 3, 3, 2, 0, 0, 0, 0 );
WIZARD_SPELL[ 1][15].init(  4, 3, 3, 3, 2, 1, 0, 0, 0 );
WIZARD_SPELL[ 1][16].init(  4, 4, 3, 3, 3, 1, 0, 0, 0 );
WIZARD_SPELL[ 1][17].init(  4, 4, 4, 3, 3, 2, 0, 0, 0 );
WIZARD_SPELL[ 1][18].init(  4, 4, 4, 4, 3, 2, 0, 0, 0 );
WIZARD_SPELL[ 1][19].init(  4, 4, 4, 4, 4, 3, 0, 0, 0 );

HIT_DICE[0].init( 10, 10, 3 );
HIT_DICE[1].init(  4, 11, 1 );
HIT_DICE[2].init(  8, 10, 2 );
HIT_DICE[3].init(  6, 11, 2 );
HIT_DICE[4].init(  6,  9, 2 );

PROF_SLOTS[0].init( 4, 3, -2, 3, 3 );
PROF_SLOTS[1].init( 1, 6, -5, 4, 3 );
PROF_SLOTS[2].init( 2, 4, -3, 4, 3 );
PROF_SLOTS[3].init( 2, 4, -3, 3, 4 );
PROF_SLOTS[4].init( 2, 5, -4, 3, 3 );

strcpy( RACE[0].nom, "dwarf" );
RACE[0].req_min.init(  8,   0,  3, 11,  3,  3,  3 );
RACE[0].req_max.init( 18, 100, 17, 18, 18, 18, 17 );
RACE[0].adj.init    (  0,   0,  0,  1,  0,  0, -1 );
RACE[0].const_st = 1;
RACE[0].b_mov_rate = 6;
RACE[0].height.init (  43,  41, 1, 10  );
RACE[0].weigth.init ( 130, 105, 4, 10  );
RACE[0].age.init    (  40,  40, 5,  6  );
RACE[0].max_age.init( 250, 250, 2, 100 );
RACE[0].thief_race_adj.init( 0, 10, 15, 0, 0, 0, -10, -5 );

THIEF_SKILL_BASE_SCORE.init( 15, 10, 5, 10, 5, 15, 60, 0 );

strcpy( RACE[1].nom, "elf" );
RACE[1].req_min.init(  3,   0,  6,  7,  8,  3,  8 );
RACE[1].req_max.init( 18, 100, 18, 18, 18, 18, 18 );
RACE[1].adj.init    (  0,   0,  1, -1,  0,  0,  0 );
RACE[1].const_st = 0;
RACE[1].b_mov_rate = 12;
RACE[1].height.init (  55,  50, 1, 10  );
RACE[1].weigth.init (  90,  70, 3, 10  );
RACE[1].age.init    ( 100, 100, 5,  6  );
RACE[1].max_age.init( 350, 350, 4, 100 );
RACE[1].thief_race_adj.init( 5, -5, 0, 5, 10, 5, 0, 0 );

strcpy( RACE[2].nom, "gnome" );
RACE[2].req_min.init(  6,   0,  3,  8,  6,  3,  3 );
RACE[2].req_max.init( 18, 100, 18, 18, 18, 18, 18 );
RACE[2].adj.init    (  0,   0,  0,  0,  1, -1,  0 );
RACE[2].const_st = 1;
RACE[2].b_mov_rate = 6;
RACE[2].height.init (  38,  36, 1,  6 );
RACE[2].weigth.init (  72,  68, 5,  4 );
RACE[2].age.init    (  60,  60, 3, 12 );
RACE[2].max_age.init( 200, 200, 3, 100 );
RACE[2].thief_race_adj.init( 0, 5, 10, 5, 5, 10, -15, 0 );

strcpy( RACE[3].nom, "half-elf" );
RACE[3].req_min.init(  3,   0,  6,  6,  4,  3,  3 );
RACE[3].req_max.init( 18, 100, 18, 18, 18, 18, 18 );
RACE[3].adj.init    (  0,   0,  0,  0,  0,  0,  0 );
RACE[3].const_st = 0;
RACE[3].b_mov_rate = 12;
RACE[3].height.init (  60,  58, 2,  6 );
RACE[3].weigth.init ( 110,  85, 3, 12 );
RACE[3].age.init    (  15,  15, 1,  6 );
RACE[3].max_age.init( 125, 125, 3, 20 );
RACE[3].thief_race_adj.init( 10, 0, 0, 0, 5, 0, 0, 0 );

strcpy( RACE[4].nom, "halfling" );
RACE[4].req_min.init(  7, 0,  7, 10,  6,  3,  3 );
RACE[4].req_max.init( 18, 0, 18, 18, 18, 17, 18 );
RACE[4].adj.init    ( -1, 0,  1,  0,  0,  0,  0 );
RACE[4].const_st = 1;
RACE[4].b_mov_rate = 6;
RACE[4].height.init (  38,  36, 1,   6 );
RACE[4].weigth.init (  72,  68, 5,   4 );
RACE[4].age.init    (  60,  60, 3,  12 );
RACE[4].max_age.init( 200, 200, 3, 100 );
RACE[4].thief_race_adj.init( 0, 5, 10, 5, 5, 10, -15, 0 );

strcpy( RACE[5].nom, "human" );
RACE[5].req_min.init(  1,   1,  1,  1,  1,  1,  1 );
RACE[5].req_max.init( 18, 100, 18, 18, 18, 18, 18 );
RACE[5].adj.init    (  0,   0,  0,  0,  0,  0,  0 );
RACE[5].const_st = 0;
RACE[5].b_mov_rate = 12;
RACE[5].height.init (  60,  59, 2,  10 );
RACE[5].weigth.init ( 140, 100, 6,  10 );
RACE[5].age.init    (  15,  15, 1,   4 );
RACE[5].max_age.init(  90,  90, 2,  20 );
RACE[5].thief_race_adj.init( 0, 5, 10, 5, 5, 10, -15, 0 );

}
/*
class race load_race( BYTE *nom_race )
{
WORD  filehandle;
UWORD nombre;
UWORD offset;
BYTE  race_name[40];
class race race_classure;

// open file READ_ONLY in binary
filehandle = _open( "races.add", O_RDONLY );
_read( filehandle, &nombre, 2 );
// search_name
for ( offset = 0; strcmp( nom_race, race_name ) || offset == nombre
      ; offset++ )
_read( filehandle, race_name, 40 );
// load_classure
lseek( filehandle, (long)(2 + nombre * 40 + offset*sizeof(class race)),
       SEEK_SET );
_read( filehandle, &race_classure, sizeof(class race) );
return( race_classure );
}


class classe load_classe( BYTE *nom_classe )
{
WORD      filehandle;
UWORD nombre;
UWORD offset;
BYTE     classe_name[40];
class classe classe_classure;

// open file READ_ONLY in binary
filehandle = _open( "classes.add", O_RDONLY );
_read( filehandle, &nombre, 2 );
// search_name
for ( offset = 0; strcmp( nom_classe, classe_name ) || offset == nombre
      ; offset++ )
  _read( filehandle, classe_name, 40 );
 load_classure
 lseek( filehandle, (long)(2 + nombre * 40 + offset*sizeof(class classe)),
       SEEK_SET );
_read( filehandle, &classe_classure, sizeof(class classe) );
return( classe_classure );
}


void change_classe( class character *personnage, BYTE *nom_race )
{
UBYTE temp;
UWORD base_age,
      base_grandeur,
      base_poids;

RACE_RECORD = load_race( nom_race );
if( personnage->sexe == MALE )
{
  base_grandeur = RACE_RECORD.grandeur.base_male;
  base_poids    = RACE_RECORD.poids.base_male;
}
else
{
  base_grandeur = RACE_RECORD.grandeur.base_female;
  base_poids    = RACE_RECORD.poids.base_female;
}
base_age = RACE_RECORD.age.base_male;
personnage->age      = base_age      + lancer_des( RACE_RECORD.age.no_des,
				     RACE_RECORD.age.type_des,
				     &temp );
personnage->grandeur = base_grandeur + lancer_des( RACE_RECORD.grandeur.no_des,
				     RACE_RECORD.grandeur.type_des,
				     &temp );
personnage->poids    = base_poids    + lancer_des( RACE_RECORD.poids.no_des,
				     RACE_RECORD.poids.type_des,
				     &temp );
}

void change_level( class character *personnage )
{
}
*/