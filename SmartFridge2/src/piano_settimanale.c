#include "types.h"
#include <stdio.h>
#include "piano_settimanale.h"
#include "utils.h"
#include "file_names.h"
#include "inputs.h"


/**
 * @fn void default_piano_settimanale()
 * @brief resetta e imposta il file piano settimanale con dei valori di default
 * @post file piano settimanale reimpostato con 8 elementi di cui il primo indica il giorno corrente
 *       e gli altri 7 le categoria associate ai giorni della settiamna
 */
void default_piano_settimanale(){
  FILE* file_pianosettimanale;
  t_categoria cat;
  t_giorno giorno;

  if( apriFile(&file_pianosettimanale, FILENAME_PIANO_SETTIMANALE, "wb+") ){

	// il primo elemento del file indica il giorno corrente
	giorno = MER;
	fwrite(&giorno, sizeof(t_giorno), 1, file_pianosettimanale);

	// tutti gli altri indicano il piano settimanale
    cat = CARNE;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    cat = VERDURA;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    cat = PESCE;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    cat = PASTA;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    cat = CARNE;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    cat = VERDURA;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    cat = PESCE;
    fwrite(&cat, sizeof(t_categoria), 1, file_pianosettimanale);

    fclose(file_pianosettimanale);
  }

}


/**
 * @fn void print_piano_settimanale()
 * @brief visualizza a schermo gli elementi corrispondeti alle categorie dei 7 giorni della settimana presenti in piano setttimanale
 * @post piano settimanale stampato a schermo
 */
void print_piano_settimanale(){

  FILE* file_pianosettimanale;
  t_categoria categoria_corrente;
  int giorno=1;

  if(apriFile(&file_pianosettimanale, FILENAME_PIANO_SETTIMANALE, "rb+")){

    fseek(file_pianosettimanale,sizeof(t_giorno), SEEK_SET); //posiziona il puntatore dopo il giorno attuale

    fread(&categoria_corrente, sizeof(t_categoria), 1, file_pianosettimanale);
    while (!feof(file_pianosettimanale)) {

    	printf("%s:\t%s\n", returnGiorno(giorno),returnCategoria(categoria_corrente));
    	giorno++;

        fread(&categoria_corrente, sizeof(t_categoria), 1, file_pianosettimanale);
    }

    fclose(file_pianosettimanale);
  }
}

/**
 * @fn void modificaPiano_settimanale()
 * @brief permette all'utente di modificare le 7 categorie associate ai 7 giorni della settimana contenute nel file piano settimanel
 * 	      - non permette di aggiungere o rimuovere elementi ma solo di modifcarli!
 * @post file piano settimanale con le categorie modificate dall'utente
 */
void modificaPiano_settimanale(){

	FILE* file_pianosettimanale;
	t_giorno giorno;
	t_categoria categoria;

	int flag_home;
	int flag_errore;
	int input=0;

	flag_home = 0;

	puts("piano settimanale");
	print_piano_settimanale();

	do{

		printf("inserisci il giorno da modificare ( LUN, MAR, MER, GIO, VEN, SAB, DOM )\n\t");

		giorno = input_giorno(&flag_home);


		if(!flag_home){

			do{
				printf("inserisici la categoria ( CARNE, PESCE, VERDURA, PASTA ) >> ");
				categoria = inputCategoria(&flag_errore, &flag_home);
			}while(flag_errore && !(flag_home));

			if(!flag_home){

				if(apriFile(&file_pianosettimanale, FILENAME_PIANO_SETTIMANALE, "rb+")){

					// posiziona il puntatore sulla cateogira associata al giorno inserito
					fseek(file_pianosettimanale, (giorno) * sizeof(t_categoria), SEEK_SET);
					fwrite(&categoria, sizeof(t_categoria), 1, file_pianosettimanale);

					fclose(file_pianosettimanale);
				}

				puts("piano settimanale aggiornato!");
				print_piano_settimanale();

				do{
					printf("vuoi modificare un' altro giorno? ");
					input = inputBool(&flag_errore, &flag_home);
				}while(flag_errore && !(flag_home));
			}
		}

	}while((input==1) && !flag_home);

}



/**
 * @fn t_giorno get_giorno_attuale()
 * @brief ritorna il giorno corrente
 * 		  - legge dal file piano settimanale il primo alimento (che corrisponde al giorno attuale) e lo restituisce
 * @return giorno corrente
 */
t_giorno get_giorno_attuale(){
	FILE* file_pianosettimanale;
	t_giorno giorno;

	if(apriFile(&file_pianosettimanale, FILENAME_PIANO_SETTIMANALE, "rb+")){

		rewind(file_pianosettimanale);
		// legge il primo elemento che corrisponde al giorno attuale
		fread(&giorno, sizeof(t_giorno), 1, file_pianosettimanale);

		fclose(file_pianosettimanale);
	}

	return giorno;
}


/**
 * @fn t_categoria return_categoria_giorno_attuale()
 * @brief restituisce la categoria associata al giorno della settimana attuale corrispondente nel piano settiamanale
 * @return categoria
 */
t_categoria return_categoria_giorno_attuale(){
	FILE* file_pianosettimanale;
	t_categoria cat;
	t_giorno giorno;

	if(apriFile(&file_pianosettimanale, FILENAME_PIANO_SETTIMANALE, "rb+")){

		rewind(file_pianosettimanale);
		fread(&giorno, sizeof(t_giorno), 1, file_pianosettimanale);

		// posiziona il puntatore sulla categoria associata al giorno attuale
		fseek(file_pianosettimanale, giorno*sizeof(t_categoria), SEEK_SET);
		fread(&cat, sizeof(t_categoria), 1, file_pianosettimanale);


		fclose(file_pianosettimanale);
	}

	return cat;
}



