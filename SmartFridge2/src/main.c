#include "gestione_ricette.h"
#include "gestione_alimenti.h"
#include "menu_utente.h"
#include "utils.h"
#include "piano_settimanale.h"
#include "check_input.h"
#include "file_names.h"
#include "types.h"

#include <stdio.h>
#include <string.h>


int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);

	//TODO: funzione che controlla l esistenza dei file e li crea altrimenti

	//print_alimenti();

	//caricaAlimenti();

	//print_alimenti();


	menuu();

	/*
	int flag_home=0;
	int flag_errore=0;

	do{
		flag_home = 0;
		//int n = inputInt(&flag_errore, &flag_home);
		t_ricetta ric = inputRicetta(&flag_home);
		print_ricetta(ric);
		//char str[20];

		//inputStr(str, 20, &flag_errore, &flag_home);
		//isNotNum(str, &flag_errore);

		printf("\n  errore:%d   home:%d\n",  flag_errore, flag_home);

	}while( 1 );*/

	return 0;
}




//menu();

	/*
  int flag_home=0;
  int flag_errore=0;
  char str[20];

  do{
	  caricaAlimenti();
  }while(1);
	*/

  /*
  do{
    flag_home = 0;
    flag_errore = 0;
    char str[20];
    scanf("%s", str);
    isNotNum(str, &flag_errore);
    printf("!!errore: %d\n", flag_errore);
  }while(1);*/

  //unita_misura unit  = inputUnitaMisura(&flag_errore, &flag_home);
  //printf("\n%d  errore:%d   home:%d\n", unit, flag_errore, flag_home);
  /*
  do{
    float qua  = inputFloat(&flag_errore, &flag_home);
    printf("\n%f  errore:%d   home:%d\n\n", qua, flag_errore, flag_home);
  }while(1);*/
  /*
  do{
    flag_home = 0;
    flag_errore = 0;
    t_alimento al = inputAlimento(&flag_home);
    //t_alimento al = inputAlimento(&flag_home);
    //inputStr(str,50, &flag_errore, &flag_home);

    puts("----alimento---");
    print_alimento(al);
    puts("---------------");
    printf("errore:%d   home:%d\n\n\n", flag_errore, flag_home);
  }while(1);*/