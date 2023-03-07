/****************************************************************************************
* Traitement du son via fichier wave
*
* Generation et filtrage de signaux : programme principal
*
* Transmission numerique 2020 - Bosly
****************************************************************************************/
#include "Wave.h"

/****************************************************************************************
* Prototypes : Routines de test
****************************************************************************************/
void Tst_01(S_Wav *s);
void Tst_02(S_Wav *s);
void Tst_03(S_Wav *s);
void Tst_04(S_Wav *s);
void Tst_05(S_Wav *s);
void Tst_06(S_Wav *s);
void Tst_07(S_Wav *s);
void Tst_08(S_Wav *s);
void Tst_09(S_Wav *s);

/****************************************************************************************
* Programme principal : Changer le Nr de fonction pour activer le test
*************************************************************************************** */

#define NR_TST 2

int main(void)
{
	S_Wav s;

	int *pt;

	printf("Pt : %d\n", (int)sizeof(pt)); // 4 si 32 bits - 8 si 62 bits la valeur du pointeur

	memset(&s, 0, sizeof(s));

	printf("Start test %d \n\n", NR_TST);

	switch (NR_TST)
	{
	    case  1:    Tst_01(&s);     break;
	    case  2:    Tst_02(&s);     break;
	    case  3:    Tst_03(&s);     break;
	    case  4:    Tst_04(&s);     break;
	    case  5:    Tst_05(&s);     break;
	    case  6:    Tst_06(&s);     break;
	    case  7:    Tst_07(&s);     break;
	    case  8:    Tst_08(&s);     break;
	    case  9:    Tst_09(&s);     break;
	}

	printf("\nEnd test %d \n\n", NR_TST);

	return 0;
}
