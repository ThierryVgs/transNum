/****************************************************************************************
* Traitement du son via fichier wave
*
* Getion des fichiers wave : librairie principale
*
* Transmissions numeriques 2020 - E.Bosly
****************************************************************************************/
#include "Wave.h"

/****************************************************************************************
* Variables globales du mode Flux
****************************************************************************************/
FILE *fpi, *fpo; //fichier input / fichier output
int nri, nro, nre; // Compteur de verification

/****************************************************************************************
* Ouvrir un fichier wave : ..\Data_Wave\Output\Mandoline_BP.wav
*   rep : nom du repertoire dans le repertoire principal ..\Data_Wave: Output
*   bas : nom de base du fichier : Mandoline
*   ext : extension du nom : BP
*   mod : mode d'acces : READ ou WRITE
*   typ : type de fichier : SIGNAL (TRANSFERT CONVOL)
****************************************************************************************/
void Open_Wave(S_Wav *s, char *rep, char *bas, char *ext, int mod, int typ)
{
    char txt[100];
    FILE *fp;

    strcpy(txt, "..\\Data_Wave\\");
    strcat(txt,rep);
    strcat(txt,"\\");
    strcat(txt,bas);
    if (strlen(ext)>0) {strcat(txt,"_"); strcat(txt,ext); }
    strcat(txt,".wav");

    if (mod==READ)  fp = fopen(txt, "rb");  else fp = fopen(txt, "wb");
    if (fp == NULL) puts("Erreur ouverture fichier");
    else printf("\nFichier %s ouvert\n\n", txt);

    if (mod==READ)
    {
        s->fi = fp;
        fread(&s->hi, sizeof(s->hi), 1, fp);
        Print_Wave_Header(s->hi);
        Load_Wave(s);
    }
    else
    {
        s->fo = fp;
        fwrite(&s->ho, sizeof(s->ho), 1, fp);
    }

    return;
}

/****************************************************************************************
* Allouer les tableaux Input et charger les donnees
****************************************************************************************/
void Load_Wave(S_Wav *s)
{
    int i=0, j=0;
    short int data;

    s->nri = s->hi.DataSize / s->hi.BytePerBloc;

    s->pti = Alloc_Pt(s->hi.NbrCanaux, s->nri);

    for (j=0; j<s->nri; j++)
    {
        for (i=0; i<s->hi.NbrCanaux; i++)
        {
            fread(&data, 2, 1, s->fi);
            s->pti[i][j] = (double)data / 32267.0;
        }
    }

    printf("Samples loaded : %d \n", s->nri);
}

/****************************************************************************************
* Allouer la structure de sortie
*   mode   : specifie comment calculer la taille de la sortie : INPUT SECONDE LONGUEUR
*   val    : INPUT : non utilise   SECONDE : nbre de secondes   LONGUEUR : nbre de bytes
*   fr_ech : INPUT : non utilise   SECONDE LONGUEUR : frequence d'echantillonnage
*   nr_can : nbre de canaux : 1 a 6
****************************************************************************************/
/*
Ce code alloue une structure de sortie pour stocker des données audio en fonction du mode de sortie choisi.
Il prend en entrée une structure de type S_Wav qui contient des informations sur le fichier audio,
ainsi que plusieurs paramètres : le mode de sortie, la valeur associée au mode (nombre de secondes ou de bytes),
la fréquence d'échantillonnage et le nombre de canaux.

Le code utilise ensuite un switch pour déterminer le mode de sortie choisi,
et effectue différentes opérations en conséquence. Si le mode est INPUT,
la sortie est définie de manière similaire à l'entrée. Si le mode est SECONDE ou LONGUEUR,
la sortie est définie en fonction d'un nombre de secondes ou de bytes donné.
Le nombre de canaux de la structure de sortie est également ajusté en conséquence.

Enfin, la fonction alloue de l'espace pour stocker les données de sortie et les initialise à 0.0.
*/
void Alloc_Output(S_Wav *s, int mode, int val, int fr_ech, int nr_can)
{
    int i, j;

    switch(mode)
    {
    case INPUT:
        printf("\nSortie similaire a l'entree\n\n");
        s->ho = s->hi;
        s->nro = s->ho.DataSize / s->ho.BytePerBloc;
        s->ho.NbrCanaux = nr_can;
        s->ho.BytePerBloc  = s->ho.NbrCanaux * s->ho.BitsPerSample / 8;
        s->ho.DataSize  = s->ho.BytePerBloc * s->nro;
        break;
    case SECONDE:
        printf("\nSortie definie en secondes : %d secondes\n\n", val);
        s->nro = val * fr_ech;
        Head_Wave(&s->ho, s->nro, fr_ech, nr_can);
        break;
    case LONGUEUR:
        printf("\nSortie definie en longueur : %d bytes\n\n", val);
        s->nro = val;
        Head_Wave(&s->ho, s->nro, fr_ech, nr_can);
        break;
    }

    s->pto = Alloc_Pt(s->ho.NbrCanaux, s->nro);

    for (j=0; j<s->nro; j++)
    {
        for (i=0; i<s->ho.NbrCanaux; i++)
        {
            s->pto[i][j] = 0.0;
        }
    }

    return;
}

/****************************************************************************************
* Liberer les donnees et fermer les fichiers
*   mode : INPUT OUTPUT ou ALL : permet d'ouvrir successivement plusieurs entrees ou sorties
****************************************************************************************/
void Free_Wave(S_Wav *s, int mode)
{
    char txt[40];

    switch (mode)
    {
        case INPUT:     strcpy(txt,"INPUT");    break;
        case OUTPUT:    strcpy(txt,"OUTPUT");   break;
        default:        strcpy(txt,"ALL");      break;
    }

    if (mode==INPUT || mode==ALL)
    {
        if (s->fi) fclose(s->fi);
        if (s->pti) Free_Pt(s->pti, s->hi.NbrCanaux, s->nri);
    }

    if (mode==OUTPUT || mode==ALL)
    {
        if (s->fo) fclose(s->fo);
        if (s->pto) Free_Pt(s->pto, s->ho.NbrCanaux, s->nro);
    }

    if (mode==ALL)
    {
        if (s->fc) fclose(s->fc);
        if (s->fh) fclose(s->fh);

        if (s->ptc) free(s->ptc);
        if (s->pth) free(s->pth);
    }

    printf("Free Memory and Close Files : %s \n", txt);
}

/****************************************************************************************
* Ecrire les donnees sur disque
*   mod : type de donnees a ecrire
****************************************************************************************/
void Write_Wave(S_Wav *s, int mod)
{
    int i, j, n;
    short int data;

    switch (mod)
    {
    case SIGNAL:    // Juste premier canal
        n = s->nro;
        for (i=0; i<n; i++)
        {
            data = (ushr)(s->pto[0][i] * 32267.0);
            fwrite(&data, 2, 1, s->fo);
        }
        break;
    case PTO:       // Tous les canaux
        for (j=0; j<s->nro; j++)
        {
            for (i=0; i<s->ho.NbrCanaux; i++)
            {
                data = (ushr)(s->pto[i][j] * 32267.0);
                fwrite(&data, 2, 1, s->fo);
            }
        }
        break;
    }
}

/****************************************************************************************
* Generer le header d'un fichier wave
*   nr_ech : nombre d'echantillons
*   fr_ech : frequence d'echantillonnage
*   nr_can : nombre de canqux
*
* Definition de la structure de donnees son
*
* [Bloc de déclaration d'un fichier au format WAVE]
*   FileTypeBlocID  (4 octets) : Constante «RIFF»  (0x52,0x49,0x46,0x46)
*   FileSize        (4 octets) : Taille du fichier moins 8 octets
*   FileFormatID    (4 octets) : Format = «WAVE»  (0x57,0x41,0x56,0x45)
*
* [Bloc décrivant le format audio]
*   FormatBlocID    (4 octets) : Identifiant «fmt »  (0x66,0x6D, 0x74,0x20)
*   BlocSize        (4 octets) : Nombre d'octets du bloc - 16  (0x10)
*
*   AudioFormat     (2 octets) : Format du stockage dans le fichier (1: PCM, ...)
*   NbrCanaux       (2 octets) : Nombre de canaux (de 1 à 6, cf. ci-dessous)
*   Frequence       (4 octets) : Fréquence d'échantillonnage (en hertz) [Valeurs standardisées : 11025, 22050, 44100 et éventuellement 48000 et 96000]
*   BytePerSec      (4 octets) : Nombre d'octets à lire par seconde (i.e., Frequence * BytePerBloc).
*   BytePerBloc     (2 octets) : Nombre d'octets par bloc d'échantillonnage (i.e., tous canaux confondus : NbrCanaux * BitsPerSample/8).
*   BitsPerSample   (2 octets) : Nombre de bits utilisés pour le codage de chaque échantillon (8, 16, 24)
*
* [Bloc des données]
*   DataBlocID      (4 octets) : Constante «data»  (0x64,0x61,0x74,0x61)
*   DataSize        (4 octets) : Nombre d'octets des données (i.e. "Data[]", i.e. taille_du_fichier - taille_de_l'entête  (qui fait 44 octets normalement).
*   DATAS[] : [Octets du Sample 1 du Canal 1] [Octets du Sample 1 du Canal 2] [Octets du Sample 2 du Canal 1] [Octets du Sample 2 du Canal 2]
*
* Les Canaux :
*      1 pour mono,
*      2 pour stéréo
*      3 pour gauche, droit et centre
*      4 pour face gauche, face droit, arrière gauche, arrière droit
*      5 pour gauche, centre, droit, surround (ambiant)
*      6 pour centre gauche, gauche, centre, centre droit, droit, surround (ambiant)
*****************************************************************************************/
void Head_Wave(H_Wav *h, int nr_ech, int fr_ech, int nr_can)
{
    memset(h, 0, sizeof(H_Wav));

    h->FileTypeBlocID = 0x46464952; // "RIFF" = {0x52,0x49,0x46,0x46};
    h->FileFormatID   = 0x45564157; // "WAVE" = {0x57,0x41,0x56,0x45};
    h->FormatBlocID   = 0x20746D66; // "fmt " = {0x66,0x6D, 0x74,0x20};
    h->DataBlocID     = 0x61746164; // "data" = {0x64,0x61,0x74,0x61};

    h->BlocSize      = 16;
    h->AudioFormat   = 1;
    h->NbrCanaux     = nr_can;
    h->BitsPerSample = 16;
    h->BytePerBloc   = h->NbrCanaux * h->BitsPerSample / 8;

    h->Frequence     = fr_ech;
    h->BytePerSec    = h->Frequence * h->BytePerBloc;

    h->DataSize      = nr_ech * h->BytePerBloc;
    h->FileSize      = h->DataSize + 36;

    return;
}

/****************************************************************************************
* Affichage du header d'un fichier wave
*****************************************************************************************/
void Print_Wave_Header(H_Wav h)
{
    printf("\nHeader :\n\n");
    printf("FileTypeBlocID %s\n",   Disp_Uint(h.FileTypeBlocID));
    printf("FileSize       %04u\n", h.FileSize);
    printf("FileFormatID   %s\n",   Disp_Uint(h.FileFormatID));
    printf("FormatBlocID   %s\n",   Disp_Uint(h.FormatBlocID));
    printf("BlocSize       %04u\n", h.BlocSize);
    printf("AudioFormat    %02u\n", h.AudioFormat);
    printf("NbrCanaux      %02u\n", h.NbrCanaux);
    printf("Frequence      %04u\n", h.Frequence);
    printf("BytePerSec     %04u\n", h.BytePerSec);
    printf("BytePerBloc    %02u\n", h.BytePerBloc);
    printf("BitsPerSample  %02u\n", h.BitsPerSample);
    printf("DataBlocID     %s\n",   Disp_Uint(h.DataBlocID));
    printf("DataSize       %04u\n", h.DataSize);

    return;
}

/****************************************************************************************
* Afficher un uint sous forme de caracteres
****************************************************************************************/
char * Disp_Uint(uint a)
{
    int i;
    static char txt[5];

    memset(txt,0,sizeof(txt));

    for (i=0;i<4;i++) txt[i] = (char)*((char*)&a+i);

    return txt;
}

/****************************************************************************************
* Allocation dynamique tableau de doubles a 2 dimension
****************************************************************************************/
double ** Alloc_Pt(int lignes, int colonnes)
{
	int i;
	double ** t;

	t = (double**)malloc(sizeof(*t) * lignes);

	for (i = 0; i < lignes; i++)
	{
		t[i] = (double*)malloc(sizeof(**t) * colonnes);
	}

	return t;
}

/****************************************************************************************
* Desallocation dynamique tableau de doubles a 2 dimension
****************************************************************************************/
int Free_Pt(double ** t, int lignes, int colonnes)
{
	int i;

	for(i = 0; i < lignes; i++)
	{
		free(t[i]);
	}
	free(t);

	return 0;
}

/****************************************************************************************
* Ouvrir les fichiers entree/sortie en flux : 1 seul canal
*   nm  : nom racine du fichier entree
*   ext : extension du fichier de sortie
* Dans les repertoires : Datawave\Flux_X
****************************************************************************************/
void Open_Flux_Wave(char *nm, char *ext)
{
    char txt[200];
    H_Wav h;

    strcpy(txt, "..\\Data_Wave\\Flux_In\\");
    strcat(txt,nm);
    strcat(txt,".wav");

    fpi = fopen(txt, "rb");
    if (fpi == NULL)
    {
        printf("\nOpen_Flux_Wave : Erreur ouverture entree\n\n");
        return;
    }
    else
    {
        printf("\nFlux entree ouvert %s\n", nm);
    }

    fread(&h, sizeof(h), 1, fpi);

    Print_Wave_Header(h);

    strcpy(txt, "..\\Data_Wave\\Flux_Out\\");
    strcat(txt,nm);
    strcat(txt,"_");
    strcat(txt,ext);
    strcat(txt,".wav");

    fpo = fopen(txt, "wb");
    if (fpo == NULL)
    {
        printf("\nOpen_Flux_Wave : Erreur ouverture sortie\n\n");
        return;
    }
    else
    {
        printf("Flux sortiee ouvert %s\n\n", nm);
        fwrite(&h, sizeof(h), 1, fpo);
    }

    nri = 0;
    nro = 0;
    nre = h.DataSize / h.BytePerBloc;

    return;
}

/****************************************************************************************
* Fermer les fichiers entree/sortie en Flux
****************************************************************************************/
void Close_Flux_Wave(void)
{
    fclose(fpi);
    fclose(fpo);

    printf("\nnri : %d      nro : %d      nre : %d \n\n", nri, nro, nre);

    return;
}

/****************************************************************************************
* Lire le fichier entree en Flux
*   data : pointeur vers la donnee entiere sur 16 bits
*   retourne 0 si le fichier est en EOF, 1 sinon
****************************************************************************************/
int Read_Flux_Wave(short int *data)
{
    fread(data, 2, 1, fpi);
    nri++;

    return !feof(fpi);
}

/****************************************************************************************
* Ecrire le fichier sortie
*   data : donnee entiere a ecrire sur 16 bits
****************************************************************************************/
void Write_Flux_Wave(short int data)
{
    fwrite(&data, 2, 1, fpo);
    nro++;

    return;
}
