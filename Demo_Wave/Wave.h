/****************************************************************************************
* Traitement du son via fichier wave
*
* Getion des fichiers wave : librairie principale
*
* Transmission numerique 2020 - Bosly
****************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>

typedef unsigned int uint;
typedef unsigned short int ushr;
typedef unsigned long int ulng;
typedef unsigned char uchar;

enum file_param
{
    READ,
    WRITE,
    SIGNAL,
    PTO,
    INPUT,
    OUTPUT,
    ALL,
    SECONDE,
    LONGUEUR
};

typedef struct Header_Wave
{
    uint FileTypeBlocID;
    uint FileSize;
    uint FileFormatID;
    uint FormatBlocID;
    uint BlocSize;
    ushr AudioFormat;
    ushr NbrCanaux;
    uint Frequence;
    uint BytePerSec;
    ushr BytePerBloc;
    ushr BitsPerSample;
    uint DataBlocID;
    uint DataSize;
} H_Wav;

typedef struct Son_Wave
{
    H_Wav hi;
    H_Wav ho;
    H_Wav hc;
    H_Wav hh;
    FILE *fi;
    FILE *fo;
    FILE *fc;
    FILE *fh;
    int nri;
    int nro;
    int nrc;
    int nrh;
    double **pti;
    double **pto;
    double *ptc;
    double *pth;
} S_Wav;

/****************************************************************************************
* Prototypes : Allocation dynamique
****************************************************************************************/
double ** Alloc_Pt(int lignes, int colonnes);
int Free_Pt(double ** t, int lignes, int colonnes);

void Alloc_Output(S_Wav *s, int mode, int val, int fr_ech, int nr_can);
void Free_Wave(S_Wav *s, int mode);

/****************************************************************************************
* Prototypes : Manipulation des fichiers
****************************************************************************************/
void Open_Wave(S_Wav *s, char *rep, char *bas, char *ext, int mod, int typ);
void Load_Wave(S_Wav *s);
void Write_Wave(S_Wav *s, int mod);
void Head_Wave(H_Wav *h, int nr_ech, int fr_ech, int nr_can);
void Print_Wave_Header(H_Wav h);
char * Disp_Uint(uint a);

/****************************************************************************************
* Prototypes : Manipulation des fichiers en flux
****************************************************************************************/
void Open_Flux_Wave(char *nm, char *ext);
void Close_Flux_Wave(void);
int Read_Flux_Wave(short int *data);
void Write_Flux_Wave(short int data);

/****************************************************************************************
* Prototypes : Operations sur signaux
****************************************************************************************/
void Signal_Add(S_Wav *s, int nr_res, int nr_dat);
void Signal_Norme(S_Wav *s, int nr_res);
void Signal_Ampli(S_Wav *s, int nr_res, double fac);
void Copy_Input(S_Wav *s, int nr_res, int nr_ori, int o_min, int i_min, int nr_ech);
void Signal_Carre(S_Wav *s, int nr_can, int mod);
void Filtre_Flux_Rot(int n);
void Filtre_HP_02(S_Wav *s, int nr_can, double frc);
