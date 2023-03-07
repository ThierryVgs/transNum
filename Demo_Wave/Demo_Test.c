/****************************************************************************************
* Traitement du son via fichier wave
*
* Generation et filtrage de signaux : Routines de test
*
* Transmission numerique 2020 - Bosly
****************************************************************************************/
#include "Wave.h"

/****************************************************************************************
* Generation de signaux carres et copie de fichier : test du nombre de canaux
****************************************************************************************/
void Tst_01(S_Wav *s)
{
    Alloc_Output(s, SECONDE, 5, 8000, 4);

    Signal_Carre(s, 0, 256);
    Signal_Carre(s, 1, 128);
    Signal_Carre(s, 2,  64);
    Signal_Carre(s, 3,  32);

    Open_Wave(s, "Signal", "Rep_Carre_01", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    // -----------------------------------------------------------

    Alloc_Output(s, LONGUEUR, 8192, 16000, 6);

    Signal_Carre(s, 0, 256);
    Signal_Carre(s, 1, 128);
    Signal_Carre(s, 2,  64);
    Signal_Carre(s, 3,  32);
    Signal_Carre(s, 4,  16);
    Signal_Carre(s, 5,   8);

    Open_Wave(s, "Signal", "Rep_Carre_02", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    // -----------------------------------------------------------

    Open_Wave(s, "Original", "Mandoline", "", READ, SIGNAL);

    Alloc_Output(s, INPUT, 0, 0, 1);

    Copy_Input(s, 0, 0, 0, 0, s->nro);

    Open_Wave(s, "Output", "Mandoline_Copy", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    return;
}

/****************************************************************************************
* Filtre en flux : Re-quantification par rotation de bits
****************************************************************************************/
void Tst_02(S_Wav *s)
{
    int i;
    char Ori_File_Name[40] = "Percussion"; // nom de fichier
    char File_Ext[40]; // variable temporaire

    for (i=1; i<=15; i++)
    {
        sprintf(File_Ext, "Rot_%02d", i);

        Open_Flux_Wave(Ori_File_Name, File_Ext); // ouvre le fichier avec son extention en mode flux

        Filtre_Flux_Rot(i); // ici l'instruction spécifique ( filtre )

        Close_Flux_Wave(); // ferme le fichier
    }

    return;
    /*
        lire dans le fichier d'entré et écrire dans le fichier de sortie
        rotation de bits : " 00001111 " -> " 00011110 " égale à 2x le nombre précédent. gauche multiplication droite division.
        il re quantifie (remise à zéro) du signal.

        Filtre_Flux_Rot(i) :> filtre_BP ( coder un filtre passe bas )
    */
}

/****************************************************************************************
* Filtre recursif dans tableau : attention frequences de coupure / frequence echantillonnage
****************************************************************************************/
void Tst_03(S_Wav *s)
{
    char Ori_File_Name[40] = "Mandoline";

    Open_Wave(s, "Original", Ori_File_Name, "", READ, SIGNAL);

    Alloc_Output(s, INPUT, 0, 0, 6);

    Copy_Input(s, 0, 0, 0, 0, s->nri);
    Filtre_HP_02(s, 1, 5000.0);
    Filtre_HP_02(s, 2, 2000.0);
    Filtre_HP_02(s, 3, 1000.0);
    Filtre_HP_02(s, 4, 500.0);
    Filtre_HP_02(s, 5, 200.0);

    Open_Wave(s, "Output", Ori_File_Name, "HP", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    return;
}

/****************************************************************************************
* Test lecture/ecriture de tres gros fichiers
****************************************************************************************/
void Tst_04(S_Wav *s)
{
    Open_Wave(s, "Original", "Test_Big", "", READ, SIGNAL);

    Alloc_Output(s, INPUT, 0, 0, 6); // 6 = le nombre de piste

    Copy_Input(s, 0, 0, 0, 0, s->nri);      // Original dans 0
    Copy_Input(s, 1, 0, 0, 0, s->nri);      // Original dans 1
    Copy_Input(s, 2, 0, 0, 0, s->nri);      // Original dans 2
    Copy_Input(s, 3, 0, 0, 0, s->nri);      // Original dans 3
    Copy_Input(s, 4, 0, 0, 0, s->nri);      // Original dans 4
    Copy_Input(s, 5, 0, 0, 0, s->nri);      // Original dans 5

    Open_Wave(s, "Output", "Test_Big_6", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    return;
}

/****************************************************************************************
*
****************************************************************************************/
void Tst_05(S_Wav *s)
{
    Alloc_Output(s, SECONDE, 5, 8000, 4);

    Signal_Carre(s, 0, 256);
    Signal_Carre(s, 1, 128);
    Signal_Carre(s, 2,  64);
    Signal_Carre(s, 3,  32);

    Open_Wave(s, "Signal", "Rep_Carre_01", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    // -----------------------------------------------------------

    Alloc_Output(s, LONGUEUR, 8192, 16000, 6);

    Signal_Carre(s, 0, 256);
    Signal_Carre(s, 1, 128);
    Signal_Carre(s, 2,  64);
    Signal_Carre(s, 3,  32);
    Signal_Carre(s, 4,  16);
    Signal_Carre(s, 5,   8);

    Open_Wave(s, "Signal", "Rep_Carre_02", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    // -----------------------------------------------------------

    Open_Wave(s, "Original", "Mandoline", "", READ, SIGNAL);

    Alloc_Output(s, INPUT, 0, 0, 1);

    Copy_Input(s, 0, 0, 0, 0, s->nro);

    Open_Wave(s, "Output", "Mandoline_Copy", "", WRITE, SIGNAL);

    Write_Wave(s, PTO);

    Free_Wave(s, ALL);

    return;
}

/****************************************************************************************
*
****************************************************************************************/
void Tst_06(S_Wav *s)
{

    return;
}

/****************************************************************************************
*
****************************************************************************************/
void Tst_07(S_Wav *s)
{

    return;
}

/****************************************************************************************
*
****************************************************************************************/
void Tst_08(S_Wav *s)
{

    return;
}

/****************************************************************************************
*
****************************************************************************************/
void Tst_09(S_Wav *s)
{

    return;
}
