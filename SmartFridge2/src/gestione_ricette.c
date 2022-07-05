#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "file_names.h"
#include "types.h"
#include "utils.h"
#include "gestione_alimenti.h"
#include "menu_utente.h"
#include "check_input.h"



void ordinaFileRicette_AZ(char *file_name){
	FILE* file;

  t_ricetta j_ricetta;
  t_ricetta i_ricetta;
  t_ricetta min_ricetta;


  int i=0;
  int i_min;
  int j;


  if( apriFile(&file, file_name, "rb+") ){
	rewind(file);


	//-----
	fread(&i_ricetta, sizeof(t_ricetta), 1, file);
	min_ricetta = i_ricetta;
	//-----

	while (!feof(file) ){
	  //printf("->%s  i:%d\n", i_ricetta.nome, i);


	  fread(&j_ricetta, sizeof(t_ricetta), 1, file);
	  j = i+1;
	  min_ricetta = i_ricetta;
	  i_min = i;

	  while(!feof(file)){

		//printf("\t%s  j:%d\n", j_ricetta.nome, j);

		if( strMin(j_ricetta.nome, min_ricetta.nome) ){
		  min_ricetta = j_ricetta;
		  i_min = j;

		}

		fread(&j_ricetta, sizeof(t_ricetta), 1, file);
		j++;
	  }

	  //printf("\tmin_ric:%s  i_min:%d\n", min_ricetta.nome, i_min);


	  // file[i_min] = i_ricetta
	  fseek(file, i_min * sizeof(t_ricetta), SEEK_SET);
	  fwrite(&i_ricetta, sizeof(t_ricetta), 1, file);

	  // file[i] = min_ricetta
	  fseek(file, (i) * sizeof(t_ricetta), SEEK_SET);
	  fwrite(&min_ricetta, sizeof(t_ricetta), 1, file);

	  //----

	  i++;
	  fseek(file, i * sizeof(t_ricetta), SEEK_SET);
	  fread(&i_ricetta, sizeof(t_ricetta), 1, file);
	  min_ricetta = i_ricetta;
	  //-----
	}


	fclose(file);
  }
}

//TODO: funzione che da categoria ritorna la stringa


void print_ricetta(t_ricetta ricetta){
  puts("====================");
  printf("\tnome: %s\n", ricetta.nome);
  printf("\tcategoria: %d\n", ricetta.categoria);//TODO: far visualizzare la categoria come stringa
  printf("\tprocedimento: %s\n", ricetta.procedimento);

  for(int i=0; i<ricetta.n_ingredienti; i++){
	  printf("\tingrediente %d\n",i+1);
	  print_alimento(ricetta.ingredienti[i]);
  }
  puts("====================");
}


void print_arr_ricette(t_ricetta ricette[], int n_ricette){
  int i;

  for(i=0; i< n_ricette; i++){
    print_ricetta(ricette[i]);
  }
  puts("");
}


void print_ricette(){
	FILE* file_ricette;
	t_ricetta ricetta_corrente;

	if( apriFile(&file_ricette, FILENAME_RICETTE, "rb+") ){

		rewind(file_ricette);
		fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);

		 while(!feof(file_ricette)){
			 if(strcmp(ricetta_corrente.nome,"")!=0){
				print_ricetta(ricetta_corrente);
			 }
		  fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);
		}

		fclose(file_ricette);
	}
}


t_ricetta input_ricetta(){
	t_ricetta ricetta;
	int input;
	char str[50];
	t_alimento alimento;

	ricetta.n_ingredienti = 0;
	ricetta.valutazione = 0;
	ricetta.proposta = 0;

	  printf("Inserisci nome ricetta >> ");
	  scanf("%s", ricetta.nome);
      strToUpper(ricetta.nome);

	  do{
		  printf("Inserisci categoria ricetta >> ");
		  scanf("%s", str);
		  ricetta.categoria = getCategoria(str);
	  }while( ricetta.categoria == NONE_CAT );


	  printf("Inserisci procedimento ricetta >> ");
	  scanf("%s", ricetta.procedimento);

	  printf("Inserisci ingredienti >> ");
	  do{
		  printf("n%d:\n",ricetta.n_ingredienti+1);
		  //alimento = inputAlimento(flag_home);

		  printf("questo alimento si trova in dispensa? [0:no, 1:si] >> ");
		  scanf("%d", &input);

		  if(input == 1){
			  alimento.dispensa = 1;
		  }

		  ricetta.ingredienti[ ricetta.n_ingredienti ] = alimento;
		  ricetta.n_ingredienti++;

		  printf("Aggiungere un altro alimento? [si:1, no:0] >> ");
		  scanf("%d", &input);

	  }while(input==1);


	return ricetta;
}


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
	ricetta.proposta = 0;


  //---NOME---

  do{
	printf("\tnome >> ");
	inputStr(ricetta.nome, 50, &flag_errore, &(*flag_home) );


	if( ! (*flag_home) ){ // se non � stato detto di tornare alla home

	  // controlla se sono avvenuti errori
	  if( ! flag_errore ){
		strToUpper(ricetta.nome);
	  }

	}

  }while( (flag_errore == 1) && ((*flag_home) == 0) );



  //--- CATEGORIA ---

  if (!(*flag_home)) { // se non � stato detto di tornare alla home
	// input unit� misura
	do {
	  printf("\tcategoria ricetta>> ");

	  ricetta.categoria = inputCategoria(&flag_errore, &(*flag_home));

	  if(flag_errore){
		printf("\tcategorria non trovata (disponibili:CARNE,PESCE,VERDURA,PASTA)\n");
	  }

	} while ((flag_errore == 1) && ((*flag_home) == 0));
  }


  // ---- PROCEDIMENTO -----
  if(!(*flag_home)){
	  do{
	  	printf("\tprocedimento >> ");
	  	inputStr(ricetta.procedimento, 200, &flag_errore, &(*flag_home) );

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
  }


  //--- INGREDIENTI ---

  if (!(*flag_home)) { // se non � stato detto di tornare alla home
	// input quantit�
	int flag_continue;
	t_alimento ingrediente;
	do {
	  printf("  Inserisci ingredienti : \n");
	  ingrediente = inputAlimento(  &(*flag_home));

	  if (!(*flag_home)){
		  do{
			  printf("  Questo ingrediente si trova in dispensa? >> ");
			  ingrediente.dispensa = inputBool(&flag_errore, &(*flag_home));

		  }while( (flag_errore == 1) && (*flag_home == 0) );

		  if( !(*flag_home) ){

			  ricetta.ingredienti[ ricetta.n_ingredienti ] = ingrediente;
			  ricetta.n_ingredienti++;

			  do{
				  printf("  inserire altri ingredienti? >> ");
				  flag_continue = inputBool(&flag_errore, &(*flag_home));

			  }while( (flag_errore == 1) && (*flag_home == 0) );

		  }
	  }

	} while ( (flag_continue==1) && ((*flag_home) == 0));
  }

  return ricetta;

}


void caricaRicette(){
  int flag_home;
  int flag_continue;
  int flag_errore;
  t_ricetta ricetta;
  t_ricetta ricetta_trovata;
  FILE* file_ricette;

  do {
	flag_home = 0;
	flag_continue = 0;
	flag_errore = 0;

	printf("inserisci ricetta:\n");
	ricetta = inputRicetta(&flag_home);

	if (!flag_home) {
		if( apriFile(&file_ricette, FILENAME_RICETTE, "rb+") ){

			if( ricerca_ricetta(ricetta.nome, &ricetta_trovata, file_ricette) ){

				flag_errore = 1;
				flag_continue = 1; // cioè che glielo fa rinserire

			}else{
				fseek(file_ricette, 0, SEEK_END); // posiziona alla fine
			}

		   if (!flag_errore ) {

				fwrite(&ricetta, sizeof(t_ricetta), 1, file_ricette);

				do{
				  printf("  inserire altre ricette? >> ");
				  flag_continue = inputBool(&flag_errore, &flag_home);

			  }while( (flag_errore == 1) && (flag_home == 0) );

			}

			fclose(file_ricette);
		}

	}

  } while ((flag_continue == 1) && (flag_home == 0));
}

void aggiungi_ricette(){
	FILE* file_ricette;
	int input;
	t_ricetta ricetta;

	if( apriFile(&file_ricette, FILENAME_RICETTE, "rb+") ){

		fseek(file_ricette, 0, SEEK_END); // posiziona alla fine

		do{
			puts("Inserisci ricetta:");
			ricetta = input_ricetta();
			fwrite( &ricetta, sizeof(t_ricetta), 1, file_ricette);

			printf("Caricare un altra ricetta? [si:1, no:0] >> ");
			scanf("%d", &input);

		}while(input == 1);

	  	fclose(file_ricette);
	}
}


int ricerca_ricetta(char* nome, t_ricetta* ricetta, FILE* file_ricette){
	int flag_ricetta_trovato=0;
	t_ricetta ricetta_corrente;

  strToUpper(nome);

	rewind(file_ricette);
	fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);

	 while(!feof(file_ricette) && (flag_ricetta_trovato==0)){
	  if( strcmp(ricetta_corrente.nome, nome)==0 ){
		  flag_ricetta_trovato=1;
		  *ricetta = ricetta_corrente;
		  fseek(file_ricette, -1*sizeof(t_ricetta), SEEK_CUR);//sposta di uno indietro
	  }else{
		  fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);
	  }
	}
	return flag_ricetta_trovato;
}

void modifica_ricette(){
	int input;
	char str[50];
	t_ricetta ricetta;
	FILE* file_ricette; //ricettario

	do{

		printf("Inserisci il nome della ricetta da modificare/rimuovere >> ");
		scanf("%s", str);

		if( apriFile(&file_ricette, FILENAME_RICETTE, "rb+") ){

			if( ricerca_ricetta(str, &ricetta, file_ricette) ){

				print_ricetta(ricetta);
				puts("[1] elimina ricetta");
				puts("[2] modifica ricetta");
				scanf("%d",&input);


				// elimina alimento
				if(input == 1){
					strcpy(ricetta.nome,"");
				}

				// modifica alimento
				if(input == 2){
					//TODO: chiedere qui all utente cosa e di quanto modificare
					strcpy(ricetta.procedimento,"");
					ricetta.categoria = PASTA;
					printf("%s\n",ricetta.procedimento);
				}

				fwrite( &ricetta, sizeof(t_ricetta), 1, file_ricette);

			}else{
				puts("ricetta non trovato");
			}

			fclose(file_ricette);

		}

		print_ricette();

		printf("Modificare/Eliminare un altra ricetta? [si:1, no:0] >> ");
		scanf("%d", &input);

	}while(input==1);
}



//TODO: fare il cristo di dio che considera la dispensa





// alimento1 <= alimento2 ===> 1
int isAlimento_compatibile(t_alimento alimento1, t_alimento alimento2){

  if( !strEqual(alimento1.nome,"") &&  !strEqual(alimento2.nome,"") ){ // se nessuno dei due � nullo
    return ( strEqual(alimento1.nome, alimento2.nome) &&
    (alimento1.unita == alimento2.unita) &&
    (alimento1.quantita <= alimento2.quantita) );
  }else{
    return 0;
  }

}


int isPreparabile(t_ricetta ricetta, char* filename_alimenti){

	int flag_preparabile;
	FILE* file_alimenti;
	t_alimento alimento_trovato;
	int i_alimenti_ricetta;

	if( apriFile(&file_alimenti, filename_alimenti, "rb+") ){

		i_alimenti_ricetta=0;
		flag_preparabile=1;

		while ( (i_alimenti_ricetta<ricetta.n_ingredienti) && (flag_preparabile==1) ) {
			flag_preparabile = 0;

      // se ricetta.ingredienti[i_alimenti_ricetta].dispensa == true:
      //    preparabile = 1
      // else:�

      if(ricetta.ingredienti[i_alimenti_ricetta].dispensa){

        flag_preparabile = 1;

      }else if(ricerca_alimento(ricetta.ingredienti[i_alimenti_ricetta].nome, &alimento_trovato, file_alimenti)){

				if(isAlimento_compatibile(ricetta.ingredienti[i_alimenti_ricetta], alimento_trovato)){
					flag_preparabile = 1;
				}

			}

			i_alimenti_ricetta++;
		}

		fclose(file_alimenti);
	}



	return flag_preparabile;
}


int isContained(t_alimento alimenti1[], t_alimento alimenti2[], int n_alimenti1, int n_alimenti2){

	int flag_contained;
	int i_alimenti1;
	int i_alimenti2;

	flag_contained=1;
	i_alimenti1=0;

	while ( (i_alimenti1<n_alimenti1) && (flag_contained==1) ){

		flag_contained = 0;

		i_alimenti2=0;
		while ( (i_alimenti2<n_alimenti2) && (flag_contained==0) ){
			if( isAlimento_compatibile(alimenti1[i_alimenti1], alimenti2[i_alimenti2]) ){
				flag_contained = 1;
			}
			i_alimenti2++;
		}

		i_alimenti1++;
	}


	return flag_contained;
}



void ricettePreparabili( t_ricetta ricette_preparabili[], int* n_ricette_preparabili, char* filename_ricette){
	FILE* file_ricette;
	t_ricetta ricetta_corrente;

	if( apriFile(&file_ricette, filename_ricette, "rb+") ){

		*n_ricette_preparabili = 0;
		rewind(file_ricette);
		fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);

		while(!feof(file_ricette)){

			 if( isPreparabile(ricetta_corrente, FILENAME_ALIMENTI)  ){
				 ricette_preparabili[*n_ricette_preparabili] = ricetta_corrente;
				 (*n_ricette_preparabili)++;
			 }


		  fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);
		}

		fclose(file_ricette);
	}
}

void ricetteCompatibili( t_alimento alimenti[], int n_alimenti, t_ricetta ricette_compatibili[], int* n_ricette_compatibili, char* filename_ricette){
	FILE* file_ricette;
	t_ricetta ricetta_corrente;

	if( apriFile(&file_ricette, filename_ricette, "rb+") ){

		*n_ricette_compatibili = 0;
		rewind(file_ricette);
		fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);

		while(!feof(file_ricette)){

			 if( isPreparabile(ricetta_corrente, FILENAME_ALIMENTI)
					&& isContained(alimenti, ricetta_corrente.ingredienti, n_alimenti, ricetta_corrente.n_ingredienti)){
				 ricette_compatibili[*n_ricette_compatibili] = ricetta_corrente;
				 (*n_ricette_compatibili)++;
			 }


		  fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);
		}

		fclose(file_ricette);
	}
}

//TODO: � lo stesso codice di print_ricette
void print_cronologia_ricette(){
  FILE* file_cronologia;
  t_ricetta ricetta_corrente;

  if(apriFile(&file_cronologia, FILENAME_CRONOLOGIA_RICETTE,"rb+")){

    rewind(file_cronologia);
		fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_cronologia);

		 while(!feof(file_cronologia)){
			 if(strcmp(ricetta_corrente.nome,"")!=0){
				print_ricetta(ricetta_corrente);
			 }
		  fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_cronologia);
		}

    fclose(file_cronologia);
  }
}


void printStatistica_voti(){
  FILE* file_ricette;
	t_ricetta ricetta_corrente;
  int n_valutazioni[6]; // 0=non valutata,    [1-5]=valutazione


  if( apriFile(&file_ricette, FILENAME_RICETTE, "rb+") ){

    rewind(file_ricette);
    fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);


    while(!feof(file_ricette)){
      if(  !strEqual(ricetta_corrente.nome,"") ){
        n_valutazioni[ricetta_corrente.valutazione]++;
      }

      fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);
    }

    fclose(file_ricette);
  }


  for(int i=1; i<6; i++){
    printf("VOTO %d  ", i);
    for(int j=0; j<n_valutazioni[i]; j++){
      printf("#");
    }
    puts("");
  }
  printf("\nRicette non valutate: %d\n\n", n_valutazioni[0]);
}


void valuta_ricette(){
  FILE* file_ricette;
  t_ricetta ricetta_trovata;
  char str[20];
  int input;
  if( apriFile(&file_ricette, FILENAME_RICETTE, "rb+") ){

    do{
      printf("Inserisci nome ricetta da valutare >> ");
      scanf("%s", str);

      if(  ricerca_ricetta(str, &ricetta_trovata, file_ricette) ){
        printf("Inserisci valutazione >> ");
        scanf("%d", &ricetta_trovata.valutazione );

        fwrite(&ricetta_trovata, sizeof(t_ricetta), 1, file_ricette);
      }
      printf("valutare un altra ricetta? [1:si, 0:no] >> ");
      scanf("%d", &input);
    }while(input == 1);

    fclose(file_ricette);
  }

}