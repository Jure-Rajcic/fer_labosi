#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//int y = 0;
int main(int argc, char *argv[]) {

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    short int tablice_prevodenja_procesa[N][16]; // Tablica prevođenja za svaki od N procesa
    char disk[N][16][64]; // Simulirani disk koji služi za pohranu sadržaja stranica,
    char okvir[M][64]; // Simulirani radni spremnik od M okvira veličine 64 okteta
    int zauzeti_okviri[M];

    memset(tablice_prevodenja_procesa, 0, sizeof(tablice_prevodenja_procesa));
    memset(disk, 0, sizeof(disk));
    memset(okvir, 0, sizeof(okvir));
    memset(zauzeti_okviri, 0, sizeof(zauzeti_okviri));

    short int t = 0;
    srand(time(NULL));
    while (1){
        for(int i = 0; i < N; i++){ // za svaki proces p čini
            printf("Proces: %d\n", i);
            printf("\tt: %d\n", t);
            short int x = rand() & 0x3FE; // x ← nasumična logička adresa;
            printf("\tlog. adresa: 0x%04x\n", x);
            short int index_tablice_stranicenja = (x & 0x3C0)>>6;
            short int zapis_tablice_prevodenja = tablice_prevodenja_procesa[i][index_tablice_stranicenja]; // nađi zapis tablice straničenja procesa p za adresu x;
            short int bit_prisutnosti = zapis_tablice_prevodenja & 0x20;
            short int index_okvira = 0;
            if(!bit_prisutnosti){ // ako adresa x nije prisutna onda
                printf("\tPromašaj!\n"); // ispiši promašaj;
                while(index_okvira < M && zauzeti_okviri[index_okvira]){ // pronađi i dodijeli okvir;
                    index_okvira++;
                }
                if(index_okvira < M) {
                    printf("\t\tdodijeljen okvir: 0x%04x\n", index_okvira);
                    if (t >= 31) {
                        t = 0;
                        for (int i = 0; i < N; i++) {for (int j = 0; j < 16; j++) {tablice_prevodenja_procesa[i][j] &= 0xFFE0;}}
                        zapis_tablice_prevodenja = index_okvira << 6 | 0x20 | 0x1;
                    } else {
                        zapis_tablice_prevodenja = index_okvira << 6 | 0x20 | t;
                    }
                    for (int k = 0; k < 64; k++) {okvir[index_okvira][k] = disk[i][index_tablice_stranicenja][k];} // ucitavanje sadrzaja stranice s diska u okvir
                    tablice_prevodenja_procesa[i][index_tablice_stranicenja] = zapis_tablice_prevodenja; // ažuriraj tablicu prevođenja procesa p;

                } else {
                    short int minLRU_metapodatak = 31;
                    int index_procesa, index_stranice;
                    for(int i = 0; i < N; i++){
                        for(int j = 0; j < 16; j++) {
                            short int bit_prisutnosti = tablice_prevodenja_procesa[i][j] & 0x20;
                            if (bit_prisutnosti){
                                short int LRU_metapodatak = tablice_prevodenja_procesa[i][j] & 0x1F;
                                if (LRU_metapodatak < minLRU_metapodatak) {
                                    minLRU_metapodatak = LRU_metapodatak;
                                    index_procesa = i;
                                    index_stranice = j;
                                }
                            }
                        }
                    }
                    short int LRU_zapis = tablice_prevodenja_procesa[index_procesa][index_stranice];
                    index_okvira = (LRU_zapis & 0xFFC0) >> 6;
                    for(int k = 0; k < 64; k++){ disk[index_procesa][index_stranice][k] = okvir[index_okvira][k];} // prepisat sadrzaj okvira na disk (pohranit proces)
                    tablice_prevodenja_procesa[index_procesa][index_stranice] = 0;
                    for(int k = 0; k < 64; k++){okvir[index_okvira][k] = disk[i][index_tablice_stranicenja][k];} // prepisat sadrzaj s diska na okvir (učitat proces)
                    tablice_prevodenja_procesa[i][index_tablice_stranicenja] = LRU_zapis & 0xFFE0;
                    printf("\t\tIzbacujem stranicu 0x%04x iz procesa %d\n", index_stranice<<6, index_procesa);
                    printf("\t\tlru izbacene stranice: 0x%04x\n", minLRU_metapodatak);
                    printf("\t\tdodijeljen okvir: 0x%04x\n", index_okvira);
                    if(t>=31){
                        t=0;
                        tablice_prevodenja_procesa[i][index_tablice_stranicenja] |= 0x1;
                        for(int i = 0;i < N;i++){ for(int j = 0; j < 16;j++){ tablice_prevodenja_procesa[i][j] &= 0xFFE0;}}
                    }
                    else tablice_prevodenja_procesa[i][index_tablice_stranicenja] |= t;
                }
            }
            short int pomak_unutar_okvira = x & 0x3F;
            short int konacna_adresa = index_okvira << 6 | pomak_unutar_okvira;
            printf("\tfiz. adresa: 0x%04x\n", konacna_adresa);
            printf("\tzapis tablice: 0x%04x\n", tablice_prevodenja_procesa[i][index_tablice_stranicenja]);
            printf("\tsadržaj adrese: %d\n", okvir[index_okvira][pomak_unutar_okvira]);
//            if(okvir[index_okvira][pomak_unutar_okvira] != 0) {
//                // printf("%d", y);
//                exit(1);
//            }
            okvir[index_okvira][pomak_unutar_okvira]++; // inkrementiraj dohvaćeni sadržaj i zapiši ga na adresu x;
            zauzeti_okviri[index_okvira] = 1;
            printf("\n---------------------------\n");
            t++;
//            y++;
            sleep(1);
        }
    }
}
