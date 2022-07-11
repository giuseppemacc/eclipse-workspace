#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inputs.h"
#include "types.h"
#include "utils.h"



/**
 * @def BACK_HOME
 * @brief costante che quando viene inserita dall'utente lo porta al menu principale
 */
#ifndef HOME_VALUE
	#define BACK_HOME "home"
#endif


/**
 * @fn void inputAlimento_unita(t_alimento*, int*)
 * @brief permette all'utente di inserire alimento.unita
 * @param alimento
 * @param flag_home
 */
void inputAlimento_unita(t_alimento* alimento, int* flag_home){
	int flag_errore;

	do {
	  printf("\tunita' di misura>> ");

	  alimento->unita = inputUnitaMisura(&flag_errore, &(*flag_home));

	  if(flag_errore){
		printf("\tunita' di misura non trovata (disponibili:g,kg,ml,l,n)\n");
	  }

	} while ((flag_errore == 1) && ((*flag_home) == 0));
}


/**
 * @fn void inputAlimento_quantita(t_alimento*, int*)
 * @brief permette all'utente di inserire alimento.quantita
 * @param alimento
 * @param flag_home
 */
void inputAlimento_quantita(t_alimento* alimento, int* flag_home){
	float f_quantita;
	int flag_errore;

	do {
	  printf("\tquantita'>> ");

	  f_quantita = inputFloat(&flag_errore, &(*flag_home));


	  if( ! (*flag_home) ){ // se non e' stato detto di tornare alla home

		if( ! flag_errore ){ // se non sono avvenuti errori

		  isPositive(f_quantita,  &flag_errore  ); // controlla che f_quantita sia > 0

		  if( ! flag_errore ){ // se non sono avvenuti errori
			if ((alimento->unita == PESO_KG) || (alimento->unita == PESO_L)) { // se kg/lt si convertono in g/ml

			  alimento->quantita = (int)(f_quantita * 1000);
			  alimento->unita = alimento->unita - 1;
			} else {
			  //Ci troviamo nei casi in cui alimento.unita=g/ml/n_elementii
			  checkFloat( f_quantita, &flag_errore);

			  if( flag_errore ){
				printf("\tERRORE! Le unit� di misura g  ml  n_elementi non possono avere numeri decimali\n");
			  }
			  else{
				alimento->quantita = (int)f_quantita;
			  }

			}
		  }

		}

	  }

	} while ((flag_errore == 1) && ((*flag_home) == 0));
}


/**
 * @fn void inputAlimento_nome(t_alimento*, int*)
 * @brief permette all'utente di inserire alimento.nome
 * @param alimento
 * @param flag_home
 */
void inputAlimento_nome(t_alimento* alimento, int* flag_home){

	int flag_errore;

	  do{
		printf("\tnome >> ");
		inputStr(alimento->nome, 50, &flag_errore, &(*flag_home) );


		if( ! (*flag_home) ){ // se non e stato detto di tornare alla home

		  if( ! flag_errore ){ // se non sono avvenuti errori
			strToUpper(alimento->nome); // converti in maiuscolo il nome
		  }

		}

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}


/**
 * @fn void inputAlimento_dispensa(t_alimento*, int*)
 * @brief permette all'utente di inserire alimento.dispensa
 * @param alimento
 * @param flag_home
 */
void inputAlimento_dispensa(t_alimento* alimento, int* flag_home){
	int flag_errore;
	do{
		printf("\tquest'alimento si trova in dispensa? >> ");

		alimento->dispensa = inputBool(&flag_errore, &(*flag_home));

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}


/**
 * @fn t_alimento inputAlimento(int, int*)
 * @brief permette all'utente di inserire i vari campi dell'alimento
 * 		  - se input_dispensa = 1 verrà chiesto all'utente di inserire anche il campo dispensa
 * 		    altrimenti non verrà chiesto di inserire il campo dispensa
 * @param input_dispensa
 * @param flag_home
 * @return alimento con i campi inseriti dall'utente
 */
t_alimento inputAlimento(int input_dispensa, int *flag_home) {
  t_alimento alimento;

  *flag_home = 0;

  strcpy(alimento.nome, "");
  alimento.quantita = 0;
  alimento.unita = NONE_UNIT;
  alimento.dispensa = 0;

  //---NOME---

  inputAlimento_nome(&alimento, &(*flag_home));


  //---UNITA DI MISURA---

  if (!(*flag_home)) {
    // input unita misura
    inputAlimento_unita(&alimento, &(*flag_home));
  }


  //---QUANTITA'---

  if (!(*flag_home)) {
    // input quantit�
    inputAlimento_quantita(&alimento, &(*flag_home));
  }

  if(!(*flag_home)){
	  if(input_dispensa){
		  // input dispensa
		  inputAlimento_dispensa(&alimento, &(*flag_home));
	  }
  }

  return alimento;
}







/**
 * @fn void inputRicetta_procedimento(t_ricetta*, int*)
 * @brief permette all'utente di inserire ricetta.procedimento
 * @param ricetta
 * @param flag_home
 */
void inputRicetta_procedimento(t_ricetta* ricetta, int* flag_home){
	int flag_errore;
	do{
		printf("\tprocedimento >> ");
		inputStr(ricetta->procedimento, 200, &flag_errore, &(*flag_home) );

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}

/**
 * @fn void inputRicetta_categoria(t_ricetta*, int*)
 * @brief permette all'utente di inserire ricetta.categoria
 * @param ricetta
 * @param flag_home
 */
void inputRicetta_categoria(t_ricetta* ricetta, int* flag_home){
	// input categoria
	int flag_errore;
	do {
	  printf("\tcategoria ricetta>> ");

	  ricetta->categoria = inputCategoria(&flag_errore, &(*flag_home));

	  if(flag_errore){
		printf("\tcategorria non trovata (disponibili:CARNE,PESCE,VERDURA,PASTA)\n");
	  }

	} while ((flag_errore == 1) && ((*flag_home) == 0));

}


/**
 * @fn void inputRicetta_nome(t_ricetta*, int*)
 * @brief permette all'utente di inserire ricetta.nome
 * @param ricetta
 * @param flag_home
 */
void inputRicetta_nome(t_ricetta* ricetta, int* flag_home){

	int flag_errore;

	  do{
		printf("\tnome >> ");
		inputStr(ricetta->nome, 50, &flag_errore, &(*flag_home) );


		if( ! (*flag_home) ){ // se non e' stato detto di tornare alla home


		  if( ! flag_errore ){ // se non sono avvenuti errori
			strToUpper(ricetta->nome);
		  }

		}

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}


/**
 * @fn t_ricetta inputRicetta(int*)
 * @brief permette all'utente di inserire i vari campi della ricetta
 * @param flag_home
 * @return ricetta con i campi inseriti dall'utente
 */
t_ricetta inputRicetta(int *flag_home){

	int flag_errore;
  t_ricetta ricetta;


  flag_errore = 0;
  *flag_home = 0;


  	strcpy(ricetta.nome, "");
  	strcpy(ricetta.procedimento, "");
  	ricetta.n_ingredienti = 0;
  	ricetta.categoria = NONE_CAT;
	ricetta.valutazione = 0;
	ricetta.counter_giorni = 8;


  //---NOME---

  inputRicetta_nome(&ricetta, &(*flag_home));



  //--- CATEGORIA ---

  if (!(*flag_home)) {
	// input categoria
	inputRicetta_categoria(&ricetta, &(*flag_home));
  }


  // ---- PROCEDIMENTO -----
  if(!(*flag_home)){
	  inputRicetta_procedimento(&ricetta, &(*flag_home));
  }


  //--- INGREDIENTI ---

  if (!(*flag_home)) { // se non e' stato detto di tornare alla home
	int flag_continue;
	t_alimento ingrediente;
	do {
	  printf("  Inserisci ingredienti : \n");
	  ingrediente = inputAlimento( 1, &(*flag_home)); // 1 indica che viene fatta inserire anche la dispensa


	  if( !(*flag_home) ){

		  ricetta.ingredienti[ ricetta.n_ingredienti ] = ingrediente;
		  ricetta.n_ingredienti++;

		  do{
			  printf("  inserire altri ingredienti? >> ");
			  flag_continue = inputBool(&flag_errore, &(*flag_home));

		  }while( (flag_errore == 1) && (*flag_home == 0) );

	  }


	} while ( (flag_continue==1) && ((*flag_home) == 0));
  }

  return ricetta;

}






/**
 * @fn void inputStr(char*, int, int*, int*)
 * @brief permette all'utente di inserire una stringa che se ritenuta valida viene impostata in *str
 * 		    - la stringa vuota "" non è ritenuta valida
 * @param str
 * @param l_max indica la lunghezza massima della stringa accettata in input
 * @warning  viene considerata valida una stringa inserita di lunghezza l_max-2
 * @param flag_errore
 * @param flag_home
 */
void inputStr(char *str,int l_max, int *flag_errore, int *flag_home) {

  *flag_errore = 0;
  *flag_home = 0;
  int len;


  strcpy(str, "");

  fgets(str, l_max-1, stdin);
  fflush(stdin);

  len = strlen(str);

  if(len >= (l_max-2)){
	  *flag_errore = 1;
	  printf("la stringa inserita e' troppo grande\n");
  }else{

	  // rimuove se presente il carattere '\n' dalla stringa
	 if( str[len-1]=='\n' ){
		 str[len-1] = 0;
	 }

	if (strEqual(str, BACK_HOME)) {
	  *flag_home = 1;
	}else if( strEqual(str, "") ){ // "" non è considerata valida
		 *flag_errore = 1;
	 }
  }
}

/**
 * @fn int inputInt(int*, int*)
 * @brief permette all'utente di inserire un numero int
 * @param flag_errore
 * @param flag_home
 * @return un numero int, in caso di flag_errore/flag_home ritorna 0
 */
int inputInt(int *flag_errore, int *flag_home) {
  int num;
  char str[20];


  num=0;
  inputStr( str, 20, &(*flag_errore), &(*flag_home) );


  if(!(*flag_errore) &&  !(*flag_home)){
	  isNum(str, flag_errore);

	  if( !(*flag_errore) ){ // se str è un numero
		  num = (int)atoi(str); // converti in int
	  }

  }

  return num;
}


/**
 * @fn int inputBool(int*, int*)
 * @brief permette all'utente di inserire una conferma
 * 		  valori accettati per conferma: "si", "1", "y", "yes", "ok"
 * 		  valori accettati per non conferma: "no", "0", "n"
 * @param flag_errore
 * @param flag_home
 * @return 1 in caso di conferma, 0 in casa di non conferma
 */
int inputBool(int *flag_errore, int *flag_home) {
  int flag;
  char str[20];

  *flag_errore = 0;
  *flag_home = 0;
  flag = 0;

  inputStr(str, 20, &(*flag_errore), &(*flag_home));

  if( !(*flag_home) && !(*flag_errore) ){

	  if(strEqual(str,"si")||strEqual(str,"sì")||strEqual(str,"1")||strEqual(str,"y")||strEqual(str,"yes")||strEqual(str,"ok")){
	  	  flag = 1;

	  }else if(strEqual(str, "no")||strEqual(str, "n")||strEqual(str, "0")){
		  flag = 0;

	  }else{
		  *flag_errore = 1;

	  }
  }
  return flag;
}

/**
 * @fn float inputFloat(int*, int*)
 * @brief permette all'utente di inserire un numero float
 * @param flag_errore
 * @param flag_home
 * @return un numero float, in caso di flag_errore/flag_home ritorna 0.0
 */
float inputFloat(int *flag_errore, int *flag_home) {
  *flag_home=0;
  *flag_errore=0;
  float num=0;
  char str[20];


  inputStr( str, 20, &(*flag_errore), &(*flag_home) );


  if (! (*flag_home) ){

    isNum(str, &(*flag_errore));

    if(! (*flag_errore)){ // se è un numero
     num = (float)atof(str); // converti in float
    }

  }

  return num;
}



/**
 * @fn t_giorno input_giorno(int*)
 * @brief permette all'utente di inserire un giorno
 * @param flag_home
 * @return giorno
 */
t_giorno input_giorno(int* flag_home){
	char str[20];
	t_giorno giorno;
	int flag_errore;

	do{
		flag_errore = 0;

		do{
			printf("giorno >> ");
			inputStr(str, 20, &flag_errore, &(*flag_home));
		}while(flag_errore && (!*flag_home));


		if(!(*flag_home)){
			strToUpper(str);

			if ( strEqual(str,"LUN") )
				giorno = LUN;
			else if( strEqual(str,"MAR") )
				giorno = MAR;
			else if( strEqual(str,"MER") )
				giorno = MER;
			else if( strEqual(str,"GIO") )
				giorno = GIO;
			else if( strEqual(str,"VEN") )
				giorno = VEN;
			else if( strEqual(str,"SAB"))
				giorno = SAB;
			else if( strEqual(str,"DOM") )
				giorno = DOM;
			else{
				flag_errore=1;
				printf("ERRORE: giorno non valido\n");
			}
		}
	}while(flag_errore && !(*flag_home));

	return giorno;
}

/**
 * @fn t_unita_misura inputUnitaMisura(int*, int*)
 * @brief permette all'utente di inserire un unità di misura
 * @param flag_errore
 * @param flag_home
 * @return unità di misura
 */
t_unita_misura inputUnitaMisura(int *flag_errore, int *flag_home) {
  char str[20];
  t_unita_misura unita;

  *flag_errore=0;
  *flag_home=0;
  unita = NONE_UNIT;

  inputStr( str, 20, &(*flag_errore), &(*flag_home) );



  if ( !(*flag_home)  && !(*flag_errore) ){

    strToUpper(str);
    unita = getUnita(str); // converte da string ad t_unita_misura

    if (unita == NONE_UNIT){
      *flag_errore = 1;
    }
  }

  return unita;
}

/**
 * @fn t_categoria inputCategoria(int*, int*)
 * @brief permette all'utente di inserire una categoria
 * @param flag_errore
 * @param flag_home
 * @return categoria
 */
t_categoria inputCategoria(int *flag_errore, int *flag_home) {
	char str[20];
  t_categoria cat;

  *flag_errore=0;
  *flag_home=0;
  cat = NONE_CAT;

  inputStr( str, 20, &(*flag_errore), &(*flag_home) );



  if ( !(*flag_home)  && !(*flag_errore) ){

	strToUpper(str);
	cat = getCategoria(str); // converte da stringa a t_categoria

	if (cat == NONE_CAT){
	  *flag_errore = 1;
	}
  }

  return cat;
}




