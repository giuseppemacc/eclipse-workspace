#include "gestione_alimenti.h"
#include "file_names.h"
#include "types.h"
#include "utils.h"
#include "inputs.h"

#include <stdio.h>
#include <string.h>




/**
 * @fn void ordinaFileAlimenti_AZ(char*)
 * @brief ordina fil_name di alimenti in ordine alfabetico
 *        usa l'ordinamento per selezione
 * @pre   file_name non ordinato
 * @post  file_name ordinato
 * @param file_name
 */
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

      fread(&j_alimento, sizeof(t_alimento), 1, file);
      j = i + 1;
      min_alimento = i_alimento;
      i_min = i;

      while (!feof(file)) {

        if (strMin(j_alimento.nome, min_alimento.nome)) {
          min_alimento = j_alimento;
          i_min = j;
        }

        fread(&j_alimento, sizeof(t_alimento), 1, file);
        j++;
      }


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


/**
 * @fn int ricerca_alimento_binaria(char*, t_alimento*, FILE*)
 * @brief ricerca un'alimento tramite il nome in file_alimenti
 * @warning usa la ricerca binaria quindi necessita che file_alimento sia ordinato!
 * @pre   file_alimenti aperto e ordinato
 * @post  puntatore del file posizionato per leggere/modificare l'alimento trovato
 * 		  se l'alimento non viene trovata il puntatore è posizionato alla fine del file
 * @param nome
 * @param alimento con valore dell alimento se trovato
 * @param file_alimenti
 * @return 0 se non è stata trovata l'alimento
 *         1 se è stato trovato e viene memorizzato in *alimento
 */
int ricerca_alimento_binaria (char *nome, t_alimento *alimento, FILE *file_alimenti){
	int flag_alimento_trovato = 0;
	int inf, sup, med;
	inf = 0;

	t_alimento alimento_corrente;

	strToUpper(nome);

	fseek(file_alimenti, 0, SEEK_END);	//posiziona il puntatore in ultima posizione

	sup = ftell(file_alimenti)/sizeof(t_alimento);	//numero di elementi nel file


	do{
		med = ( inf + sup) / 2;

		fseek(file_alimenti, (med - 1) *sizeof(t_alimento), SEEK_SET);

		fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);


		if (strcmp(alimento_corrente.nome, nome) == 0){
			flag_alimento_trovato = 1;
			*alimento = alimento_corrente;
			 fseek(file_alimenti, -1 * sizeof(t_alimento), SEEK_CUR); // sposta di uno indietro

		}else{
			if (strcmp(alimento_corrente.nome, nome) < 0){
				inf = med + 1;
			}else{
				sup = med - 1;
			}
		}
	}while (inf <= sup && !flag_alimento_trovato);

	return flag_alimento_trovato;
}


/**
 * @fn void ricerca_alimentoSottstr(char*, t_alimento[], int*, FILE*)
 * @brief cerca e restituisce tutte gli alimenti che contengono *nome per sottostringa nel loro nome
 *		  - scorre tutto il file dall'inizio e memorizza in alimenti[] tutte gli alimenti trovati nel file che contengono *nome come sottistringa nel loro corrispondente campo nome
 *		    e memorizza il numero di alimenti trovati in *n_alimenti
 * @pre  file_alimenti aperto
 * @post puntatore del file posizionato alla fine
 * @param nome
 * @param alimenti
 * @param n_alimenti
 * @param file_alimenti
 */
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


/**
 * @fn t_alimento richiediAlimento(int*)
 * @brief chiede all'utente di inserire un nome di alimento e resituisce l'alimento voluta trovato nel file alimenti
 * 		  - chiede di inserire un nome di alimento
 * 		  - se è stata trovata un alimento nel file alimenti con lo stesso nome allora lo resituisce
 * 		  - altrimenti esegue la ricerca su sottostringa e mostra all'utente l' elenco di alimenti trovati
 * 		    e chiede di selezionare quello voluto e la resituisce
 * 		  - se non viene trovata nessun alimento neanche tramite la ricerca per sottostringa allora chiede all'utente di inserire un altro nome
 * @param flag_home
 * @return
 */
t_alimento richiediAlimento(int* flag_home){

	t_alimento alimento_trovato;
	t_alimento alimenti_trovati[50];
	int n_alimenti_trovati;

	int flag_continue;
	int flag_errore;

	char nome[50];
	int input;

	FILE* file_alimenti;

	*flag_home = 0;
	do{
		flag_continue = 0;

		do{
			printf("Inserisci il nome dell'alimento >> ");
			inputStr(nome, 50, &flag_errore, &(*flag_home));

		}while(flag_errore && ((*flag_home)==0));



		if(!(*flag_home)){

			ordinaFileAlimenti_AZ(FILENAME_ALIMENTI);
			if(apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+")){

				if( !ricerca_alimento_binaria(nome, &alimento_trovato, file_alimenti) ){

					ricerca_alimentoSottstr(nome, alimenti_trovati, &n_alimenti_trovati, file_alimenti);

					if( n_alimenti_trovati > 0){

						puts("forse cercavi ...");

						for( int i = 0; i < n_alimenti_trovati; i++){
							printf("%d) %s\n", i+1, alimenti_trovati[i].nome );
						}
						printf("0) cerca un altro alimento\n");

						do{
							printf("seleziona opzione >> ");
							input = inputInt(&flag_errore, &(*flag_home));

							if(!(*flag_home) && !flag_errore){

								if( input == 0 ){
									flag_continue = 1;
								}else if ( input > n_alimenti_trovati ){
									flag_errore = 1;
								}else{
									alimento_trovato = alimenti_trovati[input-1];
								}
							}

						}while(flag_errore && ((*flag_home)==0));


					}else{
						printf("alimento non trovato\n");
						flag_continue = 1;
					}

				}

				fclose(file_alimenti);
			}
		}

	}while(flag_continue && ((*flag_home)==0) );

	return alimento_trovato;
}




/**
 * @fn void print_alimenti(char*)
 * @brief visualizza a schermo tutte gli alimenti (in ordine alfabetico se ordina_file=1) presenti in filename_alimenti
 * 		  se ordina_file = 1 ordina filename_alimenti
 * 		  stampa a schermo tutte gli alimenti del file
 * @post  dettagli alimenti presenti in filename_alimenti stampati a schermo
 * @param filename_alimenti
 */
void print_alimenti(char* filename_alimenti) {
  t_alimento alimento_corrente;
  FILE *file_alimenti;


  ordinaFileAlimenti_AZ(filename_alimenti);

  if (apriFile(&file_alimenti, filename_alimenti, "rb+")) {
    rewind(file_alimenti);
    fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);

    while (!feof(file_alimenti)) {
      if (strcmp(alimento_corrente.nome, "") != 0) { // se non è un alimento nullo
        print_alimento(alimento_corrente);
      }
      fread(&alimento_corrente, sizeof(t_alimento), 1, file_alimenti);
    }

    fclose(file_alimenti);
  }
}


/**
 * @fn void caricaAlimenti()
 * @brief permette all'utente di aggiungere degli alimenti al file alimenti
 * 		  chiede all'utente di inserire un'alimento o più alimenti con i relativi campi e lo aggiunge in coda al file alimenti
 * 		  - non permette l'aggiunta di alimenti con lo stesso nome di altri alimenti già presenti nel file alimenti
 * 		    ad eccezione dei casi in cui abbiano la stessa unità di misura, ed in questo caso non verrà aggiunto un nuovo alimento
 * 		    ma verrà aggiornata la quantità dell'alimento già presente come la somma delle quantità dei due alimenti
 * @post file alimenti con l'aggiunta dei nuovi alimenti inseriti
 */
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
    alimento = inputAlimento(0, &flag_home);

    if (!flag_home) {
    	ordinaFileAlimenti_AZ(FILENAME_ALIMENTI);
    	if( apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+") ){

    		// se gia presente un alimento con lo stesso nome nel file
    		if( ricerca_alimento_binaria(alimento.nome, &alimento_trovato, file_alimenti) ){

    			if( alimento.unita == alimento_trovato.unita ){ // se hanno la stessa quantia
					alimento.quantita += alimento_trovato.quantita;

    			}else{
					flag_errore = 1;
					flag_continue = 1; // significa che glielo fara' rinserire
					 printf("ERRORE! %s gia'  esistente espresso in unita'  di misura differente\n",alimento.nome);
				}
			}else{
				fseek(file_alimenti, 0, SEEK_END); // posiziona alla fine per poter caricare l'alimento
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



/**
 * @fn t_alimento modificaAlimento(t_alimento, int, int*)
 * @brief permette all'utente di modificare i campi di un alimento passato come argomento
 * 		  - durante la modifica non permette all'utente di cambiare il nome con un altro di un alimento già presente in file alimenti
 * 		  - durante le molteplici richieste di input considera sempre il caso che l'utente possa inserire valori errati
 * 		    o che possa inserire la costante BACK_HOME ed in quest'ultimo caso imposta *flag_home a 1 e termina la funzione
 * @param alimento
 * @param modifica_dispensa se uguale a 1 permette anche di modifcare la dispensa, altrimenti rimmarrà invariata
 * @param flag_home
 * @return alimento modificato
 */
t_alimento modificaAlimento(t_alimento alimento, int modifica_dispensa, int* flag_home){
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

				if(modifica_dispensa){
					puts("\t[3] dispensa");
				}

				printf("\t >> ");

				input = inputInt(&flag_errore, &(*flag_home));

				// controlli sull input
				if(!flag_errore && !(*flag_home)){
					if(!(input==1 || input==2 || input==3)){
						flag_errore=1;
					}
					// 3 non è valido se modifica_dispensa = 0
					if((input==3) && (modifica_dispensa==0)){
						flag_errore=1;
					}
				}
			}while(flag_errore && (*flag_home==0));

			if( !(*flag_home) ){

				if(input == 1){
					do{

						// permetti di modificare il campo nome
						inputAlimento_nome(&alimento_modificato, &(*flag_home) );

						if(!(*flag_home)){

							ordinaFileAlimenti_AZ(FILENAME_ALIMENTI);
							if(apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+")){

								// se esiste già un alimento con il nuovo nome
								if( ricerca_alimento_binaria(alimento_modificato.nome, &alimento_trovato, file_alimenti)){
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

					// permetti di modificare il campo unita
					inputAlimento_unita(&alimento_modificato, &(*flag_home) );

					if (!(*flag_home)){
						// permetti di modificare il campo quantita
						inputAlimento_quantita(&alimento_modificato, &(*flag_home)  );
					}
				}else if(input == 3){
					//permetti di modificare il campo dispensa
					inputAlimento_dispensa(&alimento_modificato, &(*flag_home));
				}
			}
		}
	}

	return alimento_modificato;
}


/**
 * @fn void sovrascrivi_alimento(char*, t_alimento)
 * @brief sovrascrive nel file alimenti l'alimento con nome nome_alimento con alimento_modificato
 * 	      - cerca nel file alimenti l'alimento con nome_alimento e lo sovrascrive con alimento_modificato
 * @post  file ricette con la ricetta_modificata
 * @param nome_alimento
 * @param alimento_modificato
 */
void sovrascrivi_alimento(char* nome_alimento, t_alimento alimento_modificato){

	FILE* file_alimenti;
	t_alimento alimento_trovato;


	ordinaFileAlimenti_AZ(FILENAME_ALIMENTI);
	if(apriFile(&file_alimenti, FILENAME_ALIMENTI, "rb+")){

		 // posiziona il puntatore sull alimento da moficare
		if( ricerca_alimento_binaria(nome_alimento, &alimento_trovato, file_alimenti) ){

			fwrite(&alimento_modificato, sizeof(t_alimento), 1, file_alimenti);

			if( strEqual(alimento_modificato.nome, "")){ // lo elimina
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





