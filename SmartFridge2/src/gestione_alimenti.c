#include "file_names.h"
#include "types.h"
#include "check_input.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void ricerca_alimentoSottstr(char *nome, t_alimento alimenti[], int* n_alimenti, FILE *file_alimenti) {
  t_alimento alimento_corrente;

  *n_alimenti = 0;
  strToUpper(nome);

  rewind(file_alimenti);

  fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);

  while (!feof(file_alimenti) ) {
    if ( sottostrEqual(alimento_corrente.nome, nome) ) { // se hanno una sottostringa in comune
      alimenti[ *n_alimenti ] = alimento_corrente;
      (*n_alimenti)++;
    }
    fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);

  }
}


int ricerca_alimento(char *nome, t_alimento *alimento, FILE *file_alimenti) {
  int flag_alimento_trovato = 0;
  t_alimento alimento_corrente;


  strToUpper(nome);

  // cerca nel file alimenti e ritorna 1/0 se lo trova e lo setta in *alimento
  rewind(file_alimenti);
  fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);

  while (!feof(file_alimenti) && (flag_alimento_trovato == 0)) {
    if (strcmp(alimento_corrente.nome, nome) == 0) {
      flag_alimento_trovato = 1;
      *alimento = alimento_corrente;
      fseek(file_alimenti, -1 * sizeof(t_alimento),
            SEEK_CUR); // sposta di uno indietro
    } else {
      fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);
    }
  }
  return flag_alimento_trovato;
}

t_alimento inputAlimento(int *flag_home) {
  t_alimento alimento;

  *flag_home = 0;

  strcpy(alimento.nome, "");
  alimento.quantita = 0;
  alimento.unita = NONE_UNIT;
  alimento.dispensa = 0;

  //---NOME---

  inputAlimento_nome(&alimento, &(*flag_home));


  //---UNITA DI MISURA---

  if (!(*flag_home)) { // se non � stato detto di tornare alla home
    // input unit� misura
    inputAlimento_unita(&alimento, &(*flag_home));
  }


  //---QUANTITA'---

  if (!(*flag_home)) { // se non � stato detto di tornare alla home
    // input quantit�
    inputAlimento_quantita(&alimento, &(*flag_home));
  }

  return alimento;
}

void ordinaFileAlimenti_AZ(char *file_name) {

  FILE *file;

  t_alimento j_alimento;
  t_alimento i_alimento;
  t_alimento min_alimento;


  int i = 0;
  int i_min;
  int j;

  if (apriFile(&file, file_name, "rb+")) {
    rewind(file);

    //-----
    fread(&i_alimento, sizeof(t_alimento), 1, file);
    min_alimento = i_alimento;
    //-----

    while (!feof(file)) {
      // TODO: levare//printf("->%s  i:%d\n", i_alimento.nome, i);

      fread(&j_alimento, sizeof(t_alimento), 1, file);
      j = i + 1;
      min_alimento = i_alimento;
      i_min = i;

      while (!feof(file)) {

        // TODO: levare//printf("\t%s  j:%d\n", j_alimento.nome, j);

        if (strMin(j_alimento.nome, min_alimento.nome)) {
          min_alimento = j_alimento;
          i_min = j;
        }

        fread(&j_alimento, sizeof(t_alimento), 1, file);
        j++;
      }

      // TODO: levare//printf("\tmin_al:%s  i_min:%d\n", min_alimento.nome,
      // i_min);

      // file[i_min] = i_alimento
      fseek(file, i_min * sizeof(t_alimento), SEEK_SET);
      fwrite(&i_alimento, sizeof(t_alimento), 1, file);

      // file[i] = min_alimento
      fseek(file, (i) * sizeof(t_alimento), SEEK_SET);
      fwrite(&min_alimento, sizeof(t_alimento), 1, file);

      //----

      i++;
      fseek(file, i * sizeof(t_alimento), SEEK_SET);
      fread(&i_alimento, sizeof(t_alimento), 1, file);
      min_alimento = i_alimento;
      //-----
    }

    fclose(file);
  }
}

void print_alimento(t_alimento alimento) {
  printf("\t%s:", alimento.nome);
  // se g/ml>=1000 si stampa convertito in kg/lt
  if ((alimento.unita == PESO_GR) || (alimento.unita == PESO_ML)) {
    if (alimento.quantita >= 1000) {
      printf("  %.3f %s", (float)(((float)alimento.quantita) / 1000.0),
             returnUnita(alimento.unita + 1));
    } else
      printf("  %d %s", alimento.quantita, returnUnita(alimento.unita));
  } else
    printf("  %d %s", alimento.quantita, returnUnita(alimento.unita));

  if(alimento.dispensa){
	  printf("  dispensa");
  }
  puts("");
}


void print_alimenti(char* filename_alimenti) {
  t_alimento alimento_corrente;
  FILE *file_alimenti;


  ordinaFileAlimenti_AZ(filename_alimenti);

  if (apriFile(&file_alimenti, filename_alimenti, "rb+")) {
    rewind(file_alimenti);
    fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);

    while (!feof(file_alimenti)) {
      if (strcmp(alimento_corrente.nome, "") != 0) {
        print_alimento(alimento_corrente);
      }
      fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);
    }

    fclose(file_alimenti);
  }
}


void inputAlimento_unita(t_alimento* alimento, int* flag_home){
	int flag_errore;

	do {
	  printf("\tunita di misura>> ");

	  alimento->unita = inputUnitaMisura(&flag_errore, &(*flag_home));

	  if(flag_errore){
		printf("\tunit� di misura non trovata (disponibili:g,kg,ml,l,n)\n");
	  }

	} while ((flag_errore == 1) && ((*flag_home) == 0));
}



void inputAlimento_quantita(t_alimento* alimento, int* flag_home){
	float f_quantita;
	int flag_errore;

	do {
	  printf("\tquantità>> ");

	  f_quantita = inputFloat(&flag_errore, &(*flag_home));


	  if( ! (*flag_home) ){ // se non � stato detto di tornare alla home

				  // controlla se sono avvenuti errori
		//NULL
		if( ! flag_errore ){

		  // altri controlli cio� i vari check
		  checkQuantita(f_quantita,  &flag_errore  ); // controlla che sia > 0

		  if( ! flag_errore ){
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



void inputAlimento_nome(t_alimento* alimento, int* flag_home){

	int flag_errore;

	  do{
		printf("\tnome >> ");
		inputStr(alimento->nome, 50, &flag_errore, &(*flag_home) );


		if( ! (*flag_home) ){ // se non � stato detto di tornare alla home

		  // controlla se sono avvenuti errori
		  if( ! flag_errore ){
			strToUpper(alimento->nome);
		  }

		}

	  }while( (flag_errore == 1) && ((*flag_home) == 0) );
}



// chiede all utente di modificare l alimento
t_alimento modificaAlimento(t_alimento alimento, int* flag_home){
	int flag_errore;
	int input;
	FILE* file_alimenti;
	t_alimento alimento_trovato;
	t_alimento alimento_modificato = alimento;

	do{
		printf("1) elimina alimento\n2) modifica alimento\n >> ");
		input = inputInt(&flag_errore, &(*flag_home));

		if(!flag_errore && !(*flag_home)){
			if(!(input==1 || input==2 )){
				flag_errore=1;
			}
		}

	}while(flag_errore && (*flag_home==0));


	if(!(*flag_home)){

		if(input == 1){
			strcpy(alimento_modificato.nome,"");
		}else if (input == 2){

			do{
				printf("\nQuale campo vuoi modificare?\n");
				puts("\t[1] nome" );
				puts("\t[2] unita' di misura e quantita");
				//todo: aggiungere il campo dispensa
				printf("\t >> ");

				input = inputInt(&flag_errore, &(*flag_home));

				if(!flag_errore && !(*flag_home)){
					if(!(input==1 || input==2 )){
						flag_errore=1;
					}
				}
			}while(flag_errore && (*flag_home==0));

			if( !(*flag_home) ){

				if(input == 1){
					do{
						inputAlimento_nome(&alimento_modificato, &(*flag_home) );

						if(!(*flag_home)){

							if(apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+")){
								if( ricerca_alimento(alimento_modificato.nome, &alimento_trovato, file_alimenti)){ // se esiste già un alimento con il nuovo nome
									printf("impossibile modificare il nome, esiste già un alimento con lo stesso nome\n");

									flag_errore = 1;

								}else{
									flag_errore = 0;
								}

								fclose(file_alimenti);
							}
						}

					}while(flag_errore && !(*flag_home));


				}else if(input == 2){

					inputAlimento_unita(&alimento_modificato, &(*flag_home) );

					if (!(*flag_home)){
						inputAlimento_quantita(&alimento_modificato, &(*flag_home)  );
					}
				}
			}
		}
	}

	return alimento_modificato;
}







void sovrascrivi_alimento(char* nome_alimento, t_alimento alimento_modificato){

	FILE* file_alimenti;
	t_alimento alimento_trovato;

	if(apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+")){

		if( ricerca_alimento(nome_alimento, &alimento_trovato, file_alimenti) ){ // posiziona il puntatore sull alimento da moficare

			fwrite(&alimento_modificato, sizeof(t_alimento), 1, file_alimenti);

			if( strEqual(alimento_modificato.nome, "")){
				printf("alimento eliminato!\n");
			}else{
				printf("alimento modificato!\n");
			}

		}else{
			printf("impossibile modificare/eliminare, alimento non esistente!\n");
		}

		fclose(file_alimenti);
	}

}




void caricaAlimenti() {
  int flag_home;
  int flag_continue;
  int flag_errore;
  t_alimento alimento;
  t_alimento alimento_trovato;
  FILE* file_alimenti;


  do {
    flag_home = 0;
    flag_continue = 0;
    flag_errore = 0;

    printf("inserisci alimento:\n");
    alimento = inputAlimento(&flag_home);

    if (!flag_home) {
    	if( apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+") ){

    		if( ricerca_alimento(alimento.nome, &alimento_trovato, file_alimenti) ){
				if( alimento.unita == alimento_trovato.unita ){
					alimento.quantita += alimento_trovato.quantita;
				}else{
					flag_errore = 1;
					flag_continue = 1; // cioè che glielo fa rinserire
					 printf("ERRORE! %s gia'  esistente espresso in unita'  di misura differente\n",alimento.nome);
				}
			}else{
				fseek(file_alimenti, 0, SEEK_END); // posiziona alla fine
			}

		   if (!flag_errore ) {

				fwrite(&alimento, sizeof(t_alimento), 1, file_alimenti);

				do{
				  printf("  inserire altri alimenti? >> ");
				  flag_continue = inputBool(&flag_errore, &flag_home);

			  }while( (flag_errore == 1) && (flag_home == 0) );

			}

    		fclose(file_alimenti);
    	}

    }

  } while ((flag_continue == 1) && (flag_home == 0));
}


// TODO: aggiungere ricerca su sottostringa (?? e forse cercavi ... ??)
/*dato che dobbiamo visualizzare gli alimenti in ordine alfabetico possiamo
 * pensare di ordinare direttamente il file_alimenti e usare qui la ricerca
 * binaria*/


