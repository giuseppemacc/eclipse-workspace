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



void ricerca_ricettaSottstr(char *nome, t_ricetta ricette[], int* n_ricette, FILE *file_ricette) {
  t_ricetta ricetta_corrente;

  *n_ricette = 0;
  strToUpper(nome);

  rewind(file_ricette);

  fread(&ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);

  while ( !feof(file_ricette) ) {
    if ( sottostrEqual(ricetta_corrente.nome, nome) ) { // se hanno una sottostringa in comune
      ricette[ *n_ricette ] = ricetta_corrente;
      (*n_ricette)++;
    }
    fread(&ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);

  }
}


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

  puts("--------------------------------------------------");
  printf("Ricetta: [%s]\n", ricetta.nome);
  printf("\tcategoria: %s\n", returnCategoria(ricetta.categoria) );//TODO: far visualizzare la categoria come stringa
  printf("\tprocedimento: \"%s\"\n", ricetta.procedimento);
  printf("\tpreparata negli ultimi 7 giorni: %d\n", ricetta.preparata);
  printf("\tpreparata %d giorni fa\n", ricetta.counter_giorni);
  printf("\tvalutazione: %d \n", ricetta.valutazione);
  printf("\tingredienti:\n");
  for(int i=0; i<ricetta.n_ingredienti; i++){
	  printf("\t");
	  print_alimento(ricetta.ingredienti[i]);
  }
  puts("--------------------------------------------------");
}


void print_arr_ricette(t_ricetta ricette[], int n_ricette){
  int i;

  for(i=0; i< n_ricette; i++){
    print_ricetta(ricette[i]);
  }
  puts("");
}


void print_ricette(char* filename_ricette){
	FILE* file_ricette;
	t_ricetta ricetta_corrente;

	ordinaFileRicette_AZ(filename_ricette);

	if( apriFile(&file_ricette, filename_ricette, "rb+") ){

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
	ricetta.preparata = 0;
	ricetta.counter_giorni = 8;


  //---NOME---

  inputRicetta_nome(&ricetta, &(*flag_home));



  //--- CATEGORIA ---

  if (!(*flag_home)) { // se non � stato detto di tornare alla home
	// input unit� misura
	inputRicetta_categoria(&ricetta, &(*flag_home));
  }


  // ---- PROCEDIMENTO -----
  if(!(*flag_home)){
	  inputRicetta_procedimento(&ricetta, &(*flag_home));
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






void inputRicetta_procedimento(t_ricetta* ricetta, int* flag_home){
	int flag_errore;
	do{
		printf("\tprocedimento >> ");
		inputStr(ricetta->procedimento, 200, &flag_errore, &(*flag_home) );

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}



void inputRicetta_categoria(t_ricetta* ricetta, int* flag_home){
	// input categoria
	int flag_errore;
	do {
	  printf("\tcategoria ricetta>> ");

	  ricetta->categoria = inputCategoria(&flag_errore, &(*flag_home));

	  if(flag_errore){
		printf("\tcategorria non trovata (disponibili:CARNE,PESCE,VERDURE,PASTA)\n");
	  }

	} while ((flag_errore == 1) && ((*flag_home) == 0));

}



void inputRicetta_nome(t_ricetta* ricetta, int* flag_home){

	int flag_errore;

	  do{
		printf("\tnome >> ");
		inputStr(ricetta->nome, 50, &flag_errore, &(*flag_home) );


		if( ! (*flag_home) ){ // se non � stato detto di tornare alla home

		  // controlla se sono avvenuti errori
		  if( ! flag_errore ){
			strToUpper(ricetta->nome);
		  }

		}

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}





t_ricetta modificaRicetta(t_ricetta ricetta, int* flag_home){

	int flag_errore;
	int input;
	FILE* file_ricette;
	t_ricetta ricetta_trovata;
	t_ricetta ricetta_modificata = ricetta;
	int flag_continua_modifiche;

	do{
		printf("1) elimina ricetta\n2) modifica ricetta\n >> ");
		input = inputInt(&flag_errore, &(*flag_home));

		if(!flag_errore && !(*flag_home)){
			if(!(input==1 || input==2 )){
				flag_errore=1;
			}
		}

	}while(flag_errore && (*flag_home==0));

	if(!(*flag_home)){

		if(input == 1){
			strcpy(ricetta_modificata.nome,"");

		}else if(input == 2){

			do{
				printf("\nQuale campo vuoi modificare?\n");
				puts("\t[1] nome" );
				puts("\t[2] categoria");
				puts("\t[3] ingredienti");
				puts("\t[4] procedimento");
				puts("\t >> ");

				input = inputInt(&flag_errore, &(*flag_home));

				if(!flag_errore && !(*flag_home)){
					if(!( (input>=1) && (input<=4) )){
						flag_errore=1;
					}
				}
			}while(flag_errore && (*flag_home==0));


			if(!(*flag_home)){

				if (input == 1 ){

					do{
						inputRicetta_nome(&ricetta_modificata, &(*flag_home) );

						if(!(*flag_home)){

							if(apriFile(&file_ricette, FILENAME_RICETTE, "rb+")){
								if( ricerca_ricetta(ricetta_modificata.nome, &ricetta_trovata, file_ricette)){ // se esiste già un alimento con il nuovo nome
									printf("impossibile modificare il nome, esiste già una ricetta con lo stesso nome\n");

									flag_errore = 1;

								}else{
									flag_errore = 0;
								}

								fclose(file_ricette);
							}
						}

					}while(flag_errore && !(*flag_home));

				}else if (input == 2){

					inputRicetta_categoria(&ricetta_modificata, &(*flag_home));

				}else if (input == 3){

					do{
						flag_continua_modifiche = 1; // fa continuare le modifiche finchè non viene detto di uscire


						do{
							printf("1) modifica/rimuovi ingredienti\n2) aggiungi ingredienti\n3) salva modifiche ed esci\n >>");
							input = inputInt(&flag_errore, &(*flag_home));

							if(!flag_errore && !(*flag_home)){
								if(!( (input==1) || (input==2) || (input==3) )){
									flag_errore=1;
								}
							}
						}while(flag_errore && (*flag_home==0));


						if(!(*flag_home)){

							if(input == 1){

								puts("Ingredienti Ricetta");

								for(int i=0; i < ricetta_modificata.n_ingredienti; i++){
									  printf("\t%d)",i+1);
									  print_alimento(ricetta_modificata.ingredienti[i]);
								}

								do{
									printf("quale ingrediente vuoi modificare/rimuovere ? ");
									input = inputInt( &flag_errore, &(*flag_home) );

									if(!( (input >= 1) && (input <= ricetta_modificata.n_ingredienti) )){
										flag_errore=1;
									}

								}while(flag_errore && (!*(flag_home)) );

								if(!(*flag_home)){

									ricetta_modificata.ingredienti[input-1] = modificaAlimento(ricetta_modificata.ingredienti[input-1], &(*flag_home));

									if(!(*flag_home)){
										if(strEqual(ricetta_modificata.ingredienti[input-1].nome,"")){// se vuol far rimuovere un ingrediente

											if(ricetta_modificata.n_ingredienti >= 2){// ci deve essere sempre almeno un ingrediente
												// scambia l'ingrediente rimosso con quello in ultima posizione
												t_alimento ingrediente_rimosso = ricetta_modificata.ingredienti[input-1];
												t_alimento ingrediente_ultimapos = ricetta_modificata.ingredienti[ricetta_modificata.n_ingredienti-1];

												ricetta_modificata.ingredienti[input-1] = ingrediente_ultimapos;
												ricetta_modificata.ingredienti[ricetta_modificata.n_ingredienti-1] = ingrediente_rimosso;

												// decrementa n_ingredienti
												ricetta_modificata.n_ingredienti--;
											}else{
												puts("impossibile rimuovere l'ingrediente! la ricetta deve avere almeno un ingrediente!");
												flag_errore = 1;
											}
										}
									}
								}

							}else if(input == 2){

								ricetta_modificata.n_ingredienti++;
								// vai ad aggiungere un ingrediente in ultima posizione
								ricetta_modificata.ingredienti[ricetta_modificata.n_ingredienti-1] = inputAlimento(&(*flag_home));

							}else if(input == 3){
								flag_continua_modifiche = 0;
							}

						}


					}while(!(*flag_home) && (flag_continua_modifiche) );


				}else if (input == 4){

					inputRicetta_procedimento(&ricetta_modificata, &(*flag_home) );
				}
			}
		}
	}


	return ricetta_modificata;
}


// sostituisce nel file_ricette ricetta con ricetta_modificata
void sovrascrivi_ricetta(char* nome_ricetta, t_ricetta ricetta_modificata){

	FILE* file_ricette;
	t_ricetta ricetta_trovata;

	if(apriFile(&file_ricette, FILENAME_RICETTE, "rb+")){

		if( ricerca_ricetta(nome_ricetta, &ricetta_trovata, file_ricette) ){ // posiziona il puntatore sull alimento da moficare

			fwrite(&ricetta_modificata, sizeof(t_ricetta), 1, file_ricette);

			if( strEqual(ricetta_modificata.nome, "")){
				printf("ricetta eliminata!\n");
			}else{
				printf("ricetta modificata!\n");
			}

		}else{
			printf("impossibile modificare/eliminare, ricetta non esistente!\n");
		}

		fclose(file_ricette);
	}

}




// alimento1 <= alimento2 ===> 1
int isAlimento_compatibile(t_alimento alimento1, t_alimento alimento2, int ignora_quantita){

  if( !strEqual(alimento1.nome,"") &&  !strEqual(alimento2.nome,"") ){ // se nessuno dei due � nullo
    return ( strEqual(alimento1.nome, alimento2.nome) &&
    (alimento1.unita == alimento2.unita) &&  (ignora_quantita ? 1 : (alimento1.quantita <= alimento2.quantita) ) );
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

				if(isAlimento_compatibile(ricetta.ingredienti[i_alimenti_ricetta], alimento_trovato, 0)){
					flag_preparabile = 1;
				}

			}

			i_alimenti_ricetta++;
		}

		fclose(file_alimenti);
	}



	return flag_preparabile;
}


// tutti gli alimenti di alimenti1 sono presenti in alimenti2 e sono in quantità minore
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
			if( isAlimento_compatibile(alimenti1[i_alimenti1], alimenti2[i_alimenti2], 1) ){
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

			if(  ! strEqual(ricetta_corrente.nome, "") ){
				if( isPreparabile(ricetta_corrente, FILENAME_ALIMENTI)  ){
					 ricette_preparabili[*n_ricette_preparabili] = ricetta_corrente;
					 (*n_ricette_preparabili)++;
				 }
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
			// tutti gli alimenti[] devono essere presenti in ricetta.ingredienti[]
			// ed in quantità maggiore
			 if( isPreparabile(ricetta_corrente, FILENAME_ALIMENTI)
					&& isContained( alimenti, ricetta_corrente.ingredienti, n_alimenti, ricetta_corrente.n_ingredienti)){
				 ricette_compatibili[*n_ricette_compatibili] = ricetta_corrente;
				 (*n_ricette_compatibili)++;
			 }


		  fread( &ricetta_corrente, sizeof(t_ricetta), 1, file_ricette);
		}

		fclose(file_ricette);
	}
}



void printStatistica_voti(){
  FILE* file_ricette;
	t_ricetta ricetta_corrente;
  int n_valutazioni[6] = {0,0,0,0,0,0}; // 0=non valutata,    [1-5]=valutazione


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


