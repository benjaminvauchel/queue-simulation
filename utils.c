#include "utils.h"
#include <string.h>

int questionFermee(char *question) {
	int ansi;
	char ans;
	
	do {
		printf("%s (o/n) ", question);
		scanf(" %c", &ans);
		ansi = ans;
	} while ((ansi != 89 && (ansi != 121) && (ansi != 110) && (ansi != 111) && (ansi != 78) && (ansi != 79) && (ansi != 48) && (ansi != 49)));
	
	if ((ansi == 49) || (ansi == 79) || (ansi == 89) || (ansi == 111) || (ansi == 121)) {
		return 1;
	}
	else {
		return 0;
	}
}

void intToTemps(Temps* temps, int tps) {
	temps->h = tps/60;
	temps->min = tps%60;
}
