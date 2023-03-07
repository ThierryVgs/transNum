/****************************************************************************************
* Traitement du son via fichier wave
*
* Generation et filtrage de signaux : filtres et fonctions simples de d�monstration
*
* Transmission numerique 2020 - Bosly
****************************************************************************************/
#include "Wave.h"

/****************************************************************************************
* Addition de 2 canaux
*   nr_res : numero du canal contenant le resultat 0 - 5
*   nr_dat : numero du canal a ajouter au canal nr_res 0 - 5
*****************************************************************************************/
void Signal_Add(S_Wav *s, int nr_res, int nr_dat)
{
    int i;

    for (i=0; i<s->nro; i++)
    {
        s->pto[nr_res][i] = s->pto[nr_res][i] + s->pto[nr_dat][i] ;
    }

    return;
}

/****************************************************************************************
* Amplification d'un canal
*   nr_res : numero du canal contenant le resultat 0 - 5
*   fac    : facteur d'amplification
****************************************************************************************/
void Signal_Ampli(S_Wav *s, int nr_res, double fac)
{
    int i;

    for (i=0; i<s->nro; i++)
    {
        s->pto[nr_res][i] = fac * s->pto[nr_res][i];
    }
}

/****************************************************************************************
* Normalisation d'un canal
*   nr_res : numero du canal contenant le resultat 0 - 5
****************************************************************************************/
void Signal_Norme(S_Wav *s, int nr_res)
{
    int i;
    double fac, s_max=0.0;

    for (i=0; i<s->nro; i++)
    {
        if (fabs(s->pto[nr_res][i]) > s_max) s_max = fabs(s->pto[nr_res][i]);
    }

    fac = 0.9 / s_max;

    Signal_Ampli(s, nr_res, fac);
}

/****************************************************************************************
* Copier une partie de l'entree dans un canal de sortie a partir de o_min
*   nr_res : numero du canal contenant le resultat 0 - 5
*   nr_ori : numero du canal d'entree a copier 0 - 5
*   o_min  : indice minimum en output
*   i_min  : indice minimum en input
*   nr_ech : nombre d'echantillons copies
****************************************************************************************/
void Copy_Input(S_Wav *s, int nr_res, int nr_ori, int o_min, int i_min, int nr_ech)
{
    int i, j;

    for (i=i_min, j=o_min; i<i_min+nr_ech; i++, j++)
    {
        if (i>=0 && i<s->nri && j>=0 && j<s->nro)
            s->pto[nr_res][j] = s->pti[nr_ori][i];
    }
}

/****************************************************************************************
* Signal carre
*   nr_can : numero du canal contenant le signal 0 - 5
*   mod    : module determinant la frequence
*****************************************************************************************/
/*
Le premier extrait de code est une fonction en C appel�e "Alloc_Output" qui prend en entr�e une structure "S_Wav"
(qui est probablement une structure de donn�es pour stocker des fichiers audio au format WAV)
ainsi que plusieurs param�tres qui d�terminent comment allouer de la m�moire pour stocker la sortie audio.
En fonction du mode sp�cifi� (INPUT, SECONDE ou LONGUEUR),
la fonction alloue de la m�moire pour stocker la sortie audio avec une taille calcul�e en fonction des autres param�tres
(par exemple, le nombre de canaux, la fr�quence d'�chantillonnage et la dur�e de l'audio).
Enfin, la fonction remplit la sortie audio allou�e avec des z�ros.

Le deuxi�me extrait de code est une fonction en C appel�e "Tst_01" qui effectue plusieurs op�rations sur des
fichiers audio stock�s dans des structures S_Wav. La fonction commence par appeler "Alloc_Output" pour allouer
de la m�moire pour stocker la sortie audio en forme de signal carr�. Ensuite, la fonction appelle "Signal_Carre"
pour g�n�rer des signaux carr�s pour chaque canal audio.
La fonction appelle ensuite "Open_Wave" pour ouvrir un fichier audio de sortie,
"Write_Wave" pour �crire la sortie audio dans le fichier et "Free_Wave" pour lib�rer la m�moire allou�e pour la sortie audio.

La fonction "Tst_01" appelle ensuite "Alloc_Output" � nouveau pour allouer de la m�moire pour
une autre sortie audio en forme de signal carr�, cette fois avec un nombre diff�rent de canaux
et une fr�quence d'�chantillonnage diff�rente. La fonction appelle ensuite "Signal_Carre" � nouveau
pour g�n�rer des signaux carr�s pour chaque canal audio, "Open_Wave" pour ouvrir un fichier audio de sortie,
"Write_Wave" pour �crire la sortie audio dans le fichier et "Free_Wave" pour lib�rer la m�moire allou�e pour la sortie audio.

Enfin, la fonction "Tst_01" appelle "Open_Wave" pour ouvrir un fichier audio d'entr�e,
"Alloc_Output" pour allouer de la m�moire pour la sortie audio en forme de copie de l'entr�e,
"Copy_Input" pour copier l'audio d'entr�e dans la sortie audio,
"Open_Wave" pour ouvrir un fichier audio de sortie,
"Write_Wave" pour �crire la sortie audio dans le fichier et
"Free_Wave" pour lib�rer la m�moire allou�e pour la sortie audio.*/

void Signal_Carre(S_Wav *s, int nr_can, int mod)
{
    int i, k=0;

    for (i=0; i<s->nro; i++)
    {
        if (i%mod == 0) k = 1-k;

        if (k)    s->pto[nr_can][i] = 0.8;
        else      s->pto[nr_can][i] = -0.8;
    }

    return;
}

/****************************************************************************************
* Filtre division du niveau par rotation de bits : requantification
*   n : nombre de pas de rotation, correspondant a une division par 2^n : 1 - 15
****************************************************************************************/
void Filtre_Flux_Rot(int n)
{
    short int x0, y0, flux;

    flux = Read_Flux_Wave(&x0); //r�sultat du read valeur bool (le return oui ou non feof)

    while (flux) // (d�s que eof on stop) d�terminer par flux = Read_Flux_Wave(&x0) = feof de la fonction du m�me nom
    {
        y0 = x0 >> n; // la sortie ne d�pend que de l'entr�e ( il faut donc amorcer le pipepline )

        Write_Flux_Wave(y0);

        flux = Read_Flux_Wave(&x0); // non pr�dictif, doit arriver � la fin pour pouvoir s'arr�ter.
    }

    return;
}

/****************************************************************************************
* Filtre passe haut ordre 2 : Equation : Y(n) = a . [ X(n) - X(n-1) ] + b . Y(n-1)
*   ( filtre le canal input pti[0] et met le resultat dans pto[nr_can] )
*   nr_can : canal filtre 0 - 5
*   frc    : frequence de coupure en Hz
****************************************************************************************/
void Filtre_HP_02(S_Wav *s, int nr_can, double frc)
{
    int i;
    double fn, pn, a, b;

    fn = frc / s->ho.Frequence;
    pn = 2.0 * M_PI * fn;

    a = 2.0 / (2.0 + pn);

    b = (2.0 - pn) / (2.0 + pn);

    for (i=1; i<s->nro; i++)
    {
        s->pto[nr_can][i] = a * (s->pti[0][i] - s->pti[0][i-1]) + b * s->pto[nr_can][i-1];
    }

    return;
}
