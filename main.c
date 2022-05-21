#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

struct suffixNode{
    char *kenarKarakterleri;
    int nodeNumarasi;
    struct suffixNode* parent;
    int dalSayisi;
    struct suffixNode* dallar[256];
};

FILE *dosya;
ALLEGRO_DISPLAY *ekran;
ALLEGRO_FONT *font;
struct suffixNode *root;
char *sKatari,*pKatari;
int nodeAdet;
int yaziKaydirma;
int yukseklik,genislik;
int *katarTekrarSayilari;
int katarSayac;

void suffixTreeCizdir(struct suffixNode* node,int nodeKonumX, int nodeKonumY,int dalUzakligi,int kardesUzakligi){
    al_flip_display();
    if(node->dallar[0] == NULL)
        return;
    // dal sayisi cift ise
    if(node->dalSayisi%2 == 0){
        for(int i = 0; i < node->dalSayisi; i = i + 2){
            // node cizdir
            al_draw_filled_circle(nodeKonumX - ((i+1) *kardesUzakligi) ,nodeKonumY + dalUzakligi,25,al_map_rgb(255,255,255));
            al_draw_filled_circle(nodeKonumX + ((i+1) *kardesUzakligi) ,nodeKonumY + dalUzakligi,25,al_map_rgb(255,255,255));

            // nodelar arasi baglari cizdir
            al_draw_line(nodeKonumX,nodeKonumY,nodeKonumX - ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi,al_map_rgb(255,255,255),1.0);
            al_draw_line(nodeKonumX,nodeKonumY,nodeKonumX + ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi,al_map_rgb(255,255,255),1.0);

            // nodelarin numaralarini cizdir.
            if(node->dallar[i]->nodeNumarasi != 0)
                al_draw_textf(font,al_map_rgb(0,0,0), nodeKonumX - ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi-10,ALLEGRO_ALIGN_CENTRE,"%d",node->dallar[i]->nodeNumarasi);
            if(node->dallar[i+1]->nodeNumarasi != 0)
                al_draw_textf(font,al_map_rgb(0,0,0), nodeKonumX + ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi-10,ALLEGRO_ALIGN_CENTRE,"%d",node->dallar[i+1]->nodeNumarasi);

            // nodelarin arasindaki cizgiye karakterleri cizdir
            // sol node
            int x = abs((nodeKonumX - ((i+1) *kardesUzakligi)) - nodeKonumX) / (strlen(node->dallar[i]->kenarKarakterleri) +2);
            int y = abs((nodeKonumY + dalUzakligi) - nodeKonumY) / (strlen(node->dallar[i]->kenarKarakterleri) +2);
            for(int j = 0; j < strlen(node->dallar[i]->kenarKarakterleri); j++){
                al_draw_textf(font,al_map_rgb(255,0,0), nodeKonumX - (x * (j+1) + 20) ,nodeKonumY + (y * (j+1)), ALLEGRO_ALIGN_CENTRE,"%c",node->dallar[i]->kenarKarakterleri[j]);
            }
            // sag node
            for(int k = 0; k < strlen(node->dallar[i+1]->kenarKarakterleri); k++){
                al_draw_textf(font,al_map_rgb(0,255,0), nodeKonumX + (x * (k+1) + 20)  ,nodeKonumY + (y * (k+1)), ALLEGRO_ALIGN_CENTRE,"%c",node->dallar[i+1]->kenarKarakterleri[k]);
            }

            suffixTreeCizdir(node->dallar[i],nodeKonumX - ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi,dalUzakligi,kardesUzakligi / 1.5);
            suffixTreeCizdir(node->dallar[i+1],nodeKonumX + ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi,dalUzakligi,kardesUzakligi / 1.5);
        //printf("Node Id: %d - %d. cocugu node numarasi: %d kenar karakterleri: %s dal sayisi %d\n",node->nodeNumarasi,i,node->dallar[i]->nodeNumarasi,node->dallar[i]->kenarKarakterleri,node->dalSayisi);
        //suffixTreeCizdir(node->dallar[i]);
        }

    }else{
        // dal sayisi tek ise once ilk elemani ortaya cizdir kalanlari cift cidirir gibi devam ettir

        // node cizdir
        al_draw_filled_circle(nodeKonumX, nodeKonumY + dalUzakligi,25,al_map_rgb(255,255,255));

        // nodelar arasi baglari cizdir
        al_draw_line(nodeKonumX,nodeKonumY,nodeKonumX,nodeKonumY + dalUzakligi,al_map_rgb(255,255,255),1.0);

        // nodelarin numaralarini cizdir.
        if(node->dallar[0]->nodeNumarasi != 0)
            al_draw_textf(font,al_map_rgb(0,0,0), nodeKonumX,nodeKonumY + dalUzakligi - 10,ALLEGRO_ALIGN_CENTRE,"%d",node->dallar[0]->nodeNumarasi);

        // nodelarin arasindaki cizgiye karakterleri cizdir
        // orta nodesuffixTreeCizdir(node->dallar[i],nodeKonumX - ((i+1) *100),nodeKonumY + 100);
        //suffixTreeCizdir(node->dallar[0],nodeKonumX + ((i+1) *100),nodeKonumY + 100);

        int x = abs((nodeKonumX) - nodeKonumX) / (strlen(node->dallar[0]->kenarKarakterleri) +2);
        int y = abs((nodeKonumY + dalUzakligi) - nodeKonumY) / (strlen(node->dallar[0]->kenarKarakterleri) +2);
        for(int l = 0; l < strlen(node->dallar[0]->kenarKarakterleri); l++){
            al_draw_textf(font,al_map_rgb(0,255,0), nodeKonumX - (x * (l+1)) ,nodeKonumY + (y * (l+1)), ALLEGRO_ALIGN_CENTRE,"%c",node->dallar[0]->kenarKarakterleri[l]);
        }

        suffixTreeCizdir(node->dallar[0],nodeKonumX,nodeKonumY + dalUzakligi,dalUzakligi,kardesUzakligi);

        // orta node cizdirildikten sonra kalan nodelari cift node gibi cizdir

        for(int i = 1; i < node->dalSayisi; i = i + 2){
            // node cizdir
            al_draw_filled_circle(nodeKonumX - ((i) *kardesUzakligi) ,nodeKonumY + dalUzakligi,25,al_map_rgb(255,255,255));
            al_draw_filled_circle(nodeKonumX + ((i) *kardesUzakligi) ,nodeKonumY + dalUzakligi,25,al_map_rgb(255,255,255));

            // nodelar arasi baglari cizdir
            al_draw_line(nodeKonumX,nodeKonumY,nodeKonumX - ((i) *kardesUzakligi),nodeKonumY + dalUzakligi,al_map_rgb(255,255,255),1.0);
            al_draw_line(nodeKonumX,nodeKonumY,nodeKonumX + ((i) *kardesUzakligi),nodeKonumY + dalUzakligi,al_map_rgb(255,255,255),1.0);

            // nodelarin numaralarini cizdir.
            if(node->dallar[i]->nodeNumarasi != 0)
                al_draw_textf(font,al_map_rgb(0,0,0), nodeKonumX - ((i) *kardesUzakligi),nodeKonumY + dalUzakligi - 10,ALLEGRO_ALIGN_CENTRE,"%d",node->dallar[i]->nodeNumarasi);
            if(node->dallar[i+1]->nodeNumarasi != 0)
                al_draw_textf(font,al_map_rgb(0,0,0), nodeKonumX + ((i) *kardesUzakligi),nodeKonumY + dalUzakligi - 10,ALLEGRO_ALIGN_CENTRE,"%d",node->dallar[i+1]->nodeNumarasi);

            // nodelarin arasindaki cizgiye karakterleri cizdir
            // sol node
            int x = abs((nodeKonumX - ((i) *kardesUzakligi)) - nodeKonumX) / (strlen(node->dallar[i]->kenarKarakterleri) +2);
            int y = abs((nodeKonumY + dalUzakligi) - nodeKonumY) / (strlen(node->dallar[i]->kenarKarakterleri) +2);
            for(int j = 0; j < strlen(node->dallar[i]->kenarKarakterleri); j++){
               al_draw_textf(font,al_map_rgb(0,255,0), nodeKonumX - (x * (j+1) + 20) ,nodeKonumY + (y * (j+1)), ALLEGRO_ALIGN_CENTRE,"%c",node->dallar[i]->kenarKarakterleri[j]);
            }
            // sag node
            for(int k = 0; k < strlen(node->dallar[i+1]->kenarKarakterleri); k++){
                al_draw_textf(font,al_map_rgb(0,255,0), nodeKonumX + (x * (k+1) + 20)  ,nodeKonumY + (y * (k+1)), ALLEGRO_ALIGN_CENTRE,"%c",node->dallar[i+1]->kenarKarakterleri[k]);
            }

            suffixTreeCizdir(node->dallar[i],nodeKonumX - ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi,dalUzakligi,kardesUzakligi / 1.5);
            suffixTreeCizdir(node->dallar[i+1],nodeKonumX + ((i+1) *kardesUzakligi),nodeKonumY + dalUzakligi,dalUzakligi,kardesUzakligi / 1.5);
        }
    }

}

void sKatari_En_Uzun_Tekrar_Eden_AltKatar(char* altKatar,int islem){

    int tumAltKatarSayisi = strlen(altKatar) * (strlen(altKatar)+1) /2;
    katarSayac = 0;
    for(int x = 0; x < tumAltKatarSayisi; x++)
        katarTekrarSayilari[x] = 0;

    for (int len = 1; len <= strlen(sKatari); len++)
    {
        for (int i = 0; i <= strlen(sKatari) - len; i++)
        {
            int j = i + len - 1;
            int n = 0;
            pKatari = (char*)malloc(sizeof(char)*j+1);
            for (int k = i; k <= j; k++)
                pKatari[n++] = sKatari[k];
            pKatari[n] = '\0';
            pKatari_sKatarinda_Varmi(pKatari,root,0);
        }
    }
    int index = 0;
    int maxTekrar = 0;
    if(islem == 0){
        // katarTekrarSayilari dizisini tersten oku ve en yüksek olan indexi bul
        maxTekrar =  INT_MAX;
        for(int y = tumAltKatarSayisi -1; y >= 0 ; y--){
            if(katarTekrarSayilari[y] < maxTekrar && katarTekrarSayilari[y] > 1){
                // en yuksek tekrar eden katarin indexi bulundu
                maxTekrar = katarTekrarSayilari[y];
                index = y;
            }
        }
    }

    // en çok tekrar eden alt katar bulma
    if(islem == 1){
        maxTekrar = INT_MIN;
        for(int y = tumAltKatarSayisi -1; y >= 0 ; y--){
            if(katarTekrarSayilari[y] > maxTekrar){
                // en yuksek tekrar eden katarin indexi bulundu
                maxTekrar = katarTekrarSayilari[y];
                index = y;
            }
        }
    }


    // en yüksek tekrar bulundu ekrana yazdir
    int c = 0;
    for (int len = 1; len <= strlen(sKatari); len++)
    {
        for (int i = 0; i <= strlen(sKatari) - len; i++)
        {
            int j = i + len - 1;
            int n = 0;
            pKatari = (char*)malloc(sizeof(char)*j+1);
            for (int k = i; k <= j; k++)
                pKatari[n++] = sKatari[k];
            pKatari[n] = '\0';

            //en uzun tekrar eden katar bulundu
            if(c == index){
                if(islem == 0)
                    al_draw_textf(font,al_map_rgb(255,255,255),10 ,yukseklik - 20,ALLEGRO_ALIGN_LEFT,"%s katari icinde en uzun en cok tekrar eden alt katar %s tekrar adedi %d",sKatari,pKatari,maxTekrar);
                if(islem == 1)
                    al_draw_textf(font,al_map_rgb(255,255,255),10 ,yukseklik - 20,ALLEGRO_ALIGN_LEFT,"%s katari icinde en cok tekrar eden alt katar %s tekrar adedi %d",sKatari,pKatari,maxTekrar);
                al_flip_display();
            }
            c++;
        }
    }

}

void pKatari_sKatarinda_Varmi(char* arananKatar,struct suffixNode* node,int cizdir){

    if(arananKatar == NULL)
        return;

    int bulundu = 0;
    //root dan baslayarak tum cocuklara bak
    for(int i = 0; i < node->dalSayisi; i++){
        // daha once bulundu ise donguyu kir
        if(bulundu == 1){
            break;
        }
        //nodun cocugunun karakterlerinde kendi katarini ara
        int j;
        for(j = 0; j < strlen(node->dallar[i]->kenarKarakterleri); j++){
            if(arananKatar[j] == node->dallar[i]->kenarKarakterleri[j]){
                // sonraki karakterin de esit olup olmadigini bulmak icin karakterler esit olamyana dongu devam eder.
                continue;
            }else{
                if(j != 0){
                    // mevcut node icinde aranan katar bulundu.
                    bulundu = 1;
                    printf("\n%s katari %s katarinda %d numarali node baglantisinda bulundu ve %d kere tekrar ediyor",pKatari,sKatari,node->dallar[i]->nodeNumarasi,(node->dallar[i]->dalSayisi < 1 ) ? 1 :node->dallar[i]->dalSayisi);
                    if(cizdir == 1){
                        al_draw_textf(font,al_map_rgb(255,255,255),10 ,yukseklik - 20,ALLEGRO_ALIGN_LEFT,"%s katari %s katarinda %d numarali node baglantisinda bulundu ve %d kere tekrar ediyor",pKatari,sKatari,node->dallar[i]->nodeNumarasi,(node->dallar[i]->dalSayisi < 1 ) ? 1 :node->dallar[i]->dalSayisi);
                        al_flip_display();
                    }

                    katarTekrarSayilari[katarSayac++] = (node->dallar[i]->dalSayisi < 1 ) ? 1 :node->dallar[i]->dalSayisi;
                    break;
                }
                else{
                    // bu cocukta bu katar yok
                    //
                    break;
                }
            }
        }
        // bu cocukta bu katar yok
        if(j == 0)
            continue;

        // daha once bulundumu
        if(bulundu == 1)
            break;

        // eger j pKatarinin uzunluguna esitse p katarinin tamami bulunmustur.
        if(j == strlen(arananKatar)){
            bulundu = 1;
            printf("\n%s katari %s katarinda %d numarali node baglantisinda bulundu ve %d kere tekrar ediyor",pKatari,sKatari,node->dallar[i]->nodeNumarasi,(node->dallar[i]->dalSayisi < 1 ) ? 1 :node->dallar[i]->dalSayisi);
            if(cizdir == 1){
                al_draw_textf(font,al_map_rgb(255,255,255),10 ,yukseklik - 20,ALLEGRO_ALIGN_LEFT,"%s katari %s katarinda %d numarali node baglantisinda bulundu ve %d kere tekrar ediyor",pKatari,sKatari,node->dallar[i]->nodeNumarasi,(node->dallar[i]->dalSayisi < 1 ) ? 1 :node->dallar[i]->dalSayisi);
                al_flip_display();
            }
            katarTekrarSayilari[katarSayac++] = (node->dallar[i]->dalSayisi < 1 ) ? 1 :node->dallar[i]->dalSayisi;
        }
        // eger j pKatarinin uzunluguna esit degilse suanki nodun alt cocuklarindan birinde aranan katar devam etmektedir
        else{
            char* kalanKatar = (char*)malloc(sizeof(char)*(strlen(arananKatar)-j));
            int k;
            for(k = 0; k < strlen(arananKatar)-j; k++)
                kalanKatar[k] = arananKatar[k+j];
            kalanKatar[k] = '\0';
            pKatari_sKatarinda_Varmi(kalanKatar,node->dallar[i],cizdir);
            bulundu = 1;
        }
    }
    if(bulundu == 0){
        printf("\nAranan katar Suffix Treede bulunamadi");
        if(cizdir == 1){
            al_draw_text(font,al_map_rgb(255,255,255),10 ,yukseklik - 20,ALLEGRO_ALIGN_LEFT,"Aranan katar Suffix Treede bulunamadi");
            al_flip_display();
        }
    }

}



void nodeEkle(char *karakterler ,struct suffixNode* eklenecekNode, struct suffixNode* yeniNode, int dalAdedi,int hangiDal){
    // Yeni node bilgileri doldur
    for(int i = 0; i < 256; i++)
        yeniNode->dallar[i] = NULL;

    if(dalAdedi == 0)
        yeniNode->nodeNumarasi = ++nodeAdet;
    else
        yeniNode->nodeNumarasi = 0;
    yeniNode->dalSayisi = dalAdedi;
    yeniNode->kenarKarakterleri = (char*)malloc(sizeof(char)*strlen(karakterler));
    strcpy(yeniNode->kenarKarakterleri,karakterler);
    //printf("Yeni node eklendi karakterleri: %s\n",yeniNode->kenarKarakterleri);
    yeniNode->parent = eklenecekNode;

    // Eklenecek nodu guncelle
    if(dalAdedi == 0){
        eklenecekNode->dalSayisi = eklenecekNode->dalSayisi + 1;
        eklenecekNode->dallar[eklenecekNode->dalSayisi-1] = yeniNode;
    }
    else{
        yeniNode->dallar[0] = eklenecekNode->dallar[hangiDal];
        eklenecekNode->dallar[hangiDal]->parent = yeniNode;
        eklenecekNode->dallar[hangiDal] = yeniNode;
    }

}

void suffixTreeOlustur(struct suffixNode* node, char* katar){
    // Cikis sarti katarýn bitmis olmasý
    if(katar == NULL)
        return;
    int eklendi = 0;
    struct suffixNode *geciciNode = node;
    // tüm cocuklari gez
    for(int i = 0; i < node->dalSayisi; i++){
        if(eklendi == 1)
            break;
        //node karakterlerinin icinde arama
        for(int j = 0; j < strlen(node->dallar[i]->kenarKarakterleri); j++){
            if(katar[j] == node->dallar[i]->kenarKarakterleri[j]){
                // sonraki karakterin de esit olup olmadigini bulmak icin karakterler esit olamyana dongu devam eder.
                continue;
            }
            else{
                if(j != 0){
                    // en az bir karakter esitse
                    // araya node ekleme
                    // katarin kac karakteri esit ise o kadar yeni katar olustur
                    char* yeniKatar = (char*)malloc(sizeof(char)*j);
                    int k;
                    for(k = 0; k < j; k++)
                        yeniKatar[k] = katar[k];
                    yeniKatar[k] = '\0';

                    // mevcut nodun katarini guncelle
                    char* yeniKatar3 = (char*)malloc(sizeof(char)*(strlen(node->dallar[i]->kenarKarakterleri) - j));
                    int m;
                    for(m = 0; m < strlen(node->dallar[i]->kenarKarakterleri) - j; m++)
                        yeniKatar3[m] = node->dallar[i]->kenarKarakterleri[m+j];
                    yeniKatar3[m] = '\0';
                    //if(yeniKatar3[0] != '\0')
                    if(strlen(yeniKatar3) != 0)
                        strcpy(node->dallar[i]->kenarKarakterleri,yeniKatar3);

                    // eger eslesmeyen bir katar var ise onuda yeniNode ekle
                    char* yeniKatar2 = (char*)malloc(sizeof(char)* (strlen(katar)-j));
                    int l;
                    for(l = 0; l < strlen(katar)-j; l++)
                        yeniKatar2[l] = katar[l+j];
                    yeniKatar2[l] = '\0';


                    struct suffixNode *yeniNode = (struct suffixNode*)malloc(sizeof(struct suffixNode));
                    if(strlen(yeniKatar) != 0)
                        nodeEkle(yeniKatar,node->dallar[i]->parent,yeniNode,1,i);


                    struct suffixNode *yeniNode2 = (struct suffixNode*)malloc(sizeof(struct suffixNode));
                    if(strlen(yeniKatar3) != 0)
                        nodeEkle(yeniKatar2,yeniNode,yeniNode2,0,i);

                    // node eklendi bir sonraki katara gecmek icin donguyu kir
                    eklendi = 1;
                    break;
                }
                else{
                    // en az bir karakter esit degilse bu donguyu kir ve nodun diger cocuguna gec
                    break;
                }
            }
        }
    }
    // daha once dongu icinde eklenmediyse katarda bulunamadi icin yeni node ekle
    if(eklendi == 0){
        // katar dallarda bulunamadý.
        //yeni node olustur.
        struct suffixNode *yeniNode = (struct suffixNode*)malloc(sizeof(struct suffixNode));
        nodeEkle(katar,node,yeniNode,0,1);
    }
    // uzunluk 1 ise bu son elemandýr bir sonraki olusuma NULL gitmelidir.
    if(strlen(katar) == 1){
            suffixTreeOlustur(node,NULL);
        }
        else{
            // katari bir saga kaydýr.
            char* yeniKatar = (char*)malloc(sizeof(char)*(strlen(katar)-1));
            int i;
            for(i = 0; i < strlen(katar) -1; i++)
                yeniKatar[i] = katar[i+1];
            yeniKatar[i] = '\0';
            suffixTreeOlustur(node,yeniKatar);
        }
}

void ekranTemizle(){
    al_flip_display();
    al_rest(0.1);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_rest(0.1);
}

int dosyaKontrol(char dosyaAdi[]){
    dosya = fopen(dosyaAdi,"r");
    if(dosya != NULL){
        printf("\nDosya acildi");
        return 1;
    }
    else{
        printf("\nDosya mevcut degil");
        return 0;
    }

}

 int suffixTreeOlusturulabilirMi(){
    char dosyaAdi[50];
    printf("\nDosya Adini Girin: ");
    scanf("%s",dosyaAdi);
    if(dosyaKontrol(dosyaAdi)){

        // Satir sonuna kadar okuma
        int satirUzunlugu = 256;
        char *tempSatir = (char *)malloc(sizeof(char) * satirUzunlugu);
        char karakter = getc(dosya);
        int adet = 0;
        while((karakter != '\n') && (karakter != EOF)){
            if(adet == satirUzunlugu){
                satirUzunlugu += 10;
                tempSatir = realloc(tempSatir,satirUzunlugu);
            }
            tempSatir[adet] = karakter;
            adet++;
            karakter = getc(dosya);
        }
        tempSatir[adet] = '\0';
        char satir[adet +1];
        strncpy(satir,tempSatir,(adet+1));
        free(tempSatir);
        // Satir sonuna kadar okuma bitis printf("\n%s karakter sayisi: %d",satir,adet);

        char suffix[adet][adet+2];
        char prefix[adet][adet+2];

        // Prefix ve Suffix dizilerini olusturma ve ekrana yazma
        yaziKaydirma = (adet < 15 ) ? 15 : adet;

        al_draw_text(font,al_map_rgb(255,255,255),10 + (yaziKaydirma * 10 / 2),10,ALLEGRO_ALIGN_CENTRE,"Prefix Dizisi");
        al_draw_text(font,al_map_rgb(255,255,255), (yaziKaydirma * 15),10,ALLEGRO_ALIGN_CENTRE,"Suffix Dizisi");
        al_flip_display();
        for(int i =0; i < adet; i++){
            strncpy(prefix[i],satir,i+1);
            prefix[i][i+1] = '\0';

            al_draw_textf(font,al_map_rgb(255,255,255),10 + (yaziKaydirma * 10 / 2),40 + (i*20),ALLEGRO_ALIGN_CENTRE,"%s",prefix[i]);
            for(int j = 0; j < adet; j++){
                suffix[i][j] = satir[j+i];
            }
            suffix[i][adet-i] = '\0';

            al_draw_textf(font,al_map_rgb(255,255,255),(yaziKaydirma * 15),40 + (i*20),ALLEGRO_ALIGN_CENTRE,"%s",suffix[i]);
        }

        // suffix tree olusabilir kontrolu
        for(int i = 1; i < adet; i++){
            for(int j = 0; j < adet; j++){
                if(strcmp(suffix[i],prefix[j]) == 0){

                    al_draw_textf(font,al_map_rgb(255,0,0),10 + (yaziKaydirma * 10 / 2),40 + (j*20),ALLEGRO_ALIGN_CENTRE,"%s",prefix[j]);
                    al_draw_textf(font,al_map_rgb(255,0,0),(yaziKaydirma * 15),40 + (i*20),ALLEGRO_ALIGN_CENTRE,"%s",suffix[i]);
                    al_draw_text(font,al_map_rgb(255,0,0),10,40 + (adet*20),ALLEGRO_ALIGN_LEFT,"Suffix tree olusturulamaz. Lutfen string sonuna \$ ekleyip tekrar deneyin");
                    al_flip_display();
                    return 0;
                }
            }
        }
        al_draw_text(font,al_map_rgb(0,255,0),10,40 + (adet*20),ALLEGRO_ALIGN_LEFT,"Suffix tree olusturulabilir.");
        al_flip_display();
        sKatari = (char*)malloc(sizeof(char)*strlen(suffix[0]));
        strcpy(sKatari,suffix[0]);
        // Root node olustur
        root = (struct suffixNode*)malloc(sizeof(struct suffixNode));
        root->dalSayisi = 0;
        root->nodeNumarasi = 0;
        root->kenarKarakterleri = NULL;
        root->parent = NULL;
        for(int i = 0; i < 256; i++)
            root->dallar[i] = NULL;

        nodeAdet = 0;

        int tumAltKatarSayisi = strlen(sKatari) * (strlen(sKatari)+1) /2;
        katarTekrarSayilari = (int*)malloc(sizeof(int)*tumAltKatarSayisi);

        // Suffix Tree olustur
        suffixTreeOlustur(root,sKatari);

        // root nodu ekrana cizdir
        al_draw_text(font,al_map_rgb(255,255,255), 700 + (yaziKaydirma * 15),10,ALLEGRO_ALIGN_CENTRE,"Suffix Tree");
        al_draw_filled_circle(700 + (yaziKaydirma * 15),70,25,al_map_rgb(255,255,255));
        al_draw_text(font,al_map_rgb(0,0,0), 700 + (yaziKaydirma * 15),60,ALLEGRO_ALIGN_CENTRE,"root");
        suffixTreeCizdir(root,700 + (yaziKaydirma * 15),60,200,(900 - (yaziKaydirma * 15 + 100)) / root->dalSayisi); // (900 - (yaziKaydirma * 15 + 100)) / root->dalSayisi
        return 1;
    }
    return 0;
}



int main()
{
    yukseklik = 800;
    genislik = 1920;
    if(!al_init())
    {
        al_show_native_message_box(NULL,NULL,NULL,"Allegro 5 Yuklenemedi.",NULL,NULL);
    }
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    ekran = al_create_display(genislik,yukseklik);
    al_set_window_position(ekran,10,10);
    al_set_window_title(ekran,"Prolab 1 Proje 2 Suffix Tree Uygulamasi");
    if(!ekran){
        al_show_native_message_box(ekran,"Uyari","Ekran Ayarlari","Ekran acilmadi.",NULL,ALLEGRO_MESSAGEBOX_ERROR);
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    font = al_load_font("OpenSans-Regular.ttf",16,NULL);

    int secim = 0;
    do{
        al_flip_display();
        ekranTemizle();
        printf("\n------------------------------------------------\n");
        printf("\t[1] Suffix Tree olusturulabilir mi?\n");
        printf("\t[2] s katari icinde p katari geciyor mu? Geciyorsa ilk bulundugu yerin baslangic pozisyonu nedir? Kac kez tekrar etmektedir?\n");
        printf("\t[3] s katari icinde tekrar eden en uzun altkatar nedir? Kac kez tektar etmistir?\n");
        printf("\t[4] s katari icinde en cok tekrar eden altkatar nedir? Kac kez tekrar etmistir?\n");
        printf("\t[0] Cikis Yap");
        printf("\n------------------------------------------------\n");
        printf("Lutfen secim yapiniz: ");
        scanf("%d",&secim);
        switch(secim){
            case 1: suffixTreeOlusturulabilirMi(); break;
            case 2:
                if(suffixTreeOlusturulabilirMi()) {
                    char katar[256];
                    printf("\nAramak istediginiz katari girin: ");
                    scanf("%s",katar);
                    pKatari = katar;
                    pKatari_sKatarinda_Varmi(pKatari,root,1);
                }
                break; //suffixTreeOlustur(); break;
            case 3:
                if(suffixTreeOlusturulabilirMi()) {
                    sKatari_En_Uzun_Tekrar_Eden_AltKatar(sKatari,0);
                }
                break; //suffixTreeOlustur(); break;
            case 4:
                if(suffixTreeOlusturulabilirMi()) {
                    sKatari_En_Uzun_Tekrar_Eden_AltKatar(sKatari,1);
                }
                break;
            case 0: printf("\nCikis yapiliyor!\n"); break;
            default: printf("\nLutfen geceri bir deger girin!"); break;
        }
        if(secim != 0){
            printf("\n\tYeni islem icin bir tusa basin");
            getch();
        }
    }while(secim != 0);
    al_destroy_display(ekran);
    return 0;
}
