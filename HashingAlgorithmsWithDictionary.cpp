

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

int tabSize=5801;
//table size daha büyük seç
int Hash (char *key)  
{
  int sum = 0, j=0;

 for(j=0; j<tabSize; j+=2) 
 	sum = sum + (7*key[j] + 11*key[j+1]) ;
	sum = sum % tabSize;
	
	return sum ;
}

int HashTwo (char *key)  
{
  int sum = 0, j=0;

 for(j=0; j<tabSize; j+=2) 
 	sum = sum + (3*key[j] + 11*key[j+1]) ;
	sum = sum % tabSize;
	
	return sum ;
}

char *sil(char *metin){
  
   char dizi[36];
   int i = 0, j = 0;
   
   while (metin[i] != '\0')
   {
      if (!(metin[i] == ' ' && metin[i+1] == ' ')) {
        dizi[j] = metin[i];
        j++;
      }
      i++;
   }
 
   dizi[j-1] = '\0';
 
   return dizi; 
}
 
 
FILE *dic, *rel;
char c;
char ingilizce[36];  
char turkce[80];   
char sorgu[36];
char kel[36],ingKel[36],turKel[80];
int Adr, Adrtwo,SorAdr,EkAdr;
int Temp;
long size;

//------------lineer probing---------------
void LineerRelativeOlustur() 
{      
	
	clock_t sure;
	sure=clock();                      
	dic = fopen("dictionary.txt", "r");
	rel = fopen("lineer.txt", "w+");
	int sayac=0;
	
	for(int i=0; i<tabSize; i++)  
	{
		fseek(rel,i*116,0);
		fputc('*', rel );
	}
	
	for(int i=0; i<5721; i++)  
	{
		
		fgets (ingilizce  , 36 , dic);
		fgets (turkce  , 80 , dic);
		
		Adr = Hash(ingilizce);		
		fseek(rel,Adr*116,0);
		c=fgetc(rel);		
		
		// LINEAR PROBING
		Temp=Adr;
		while( c != '*')
		{
			
			sayac++;
			Adr = (Adr + 1) % tabSize   ;
			
			if(Adr==Temp)
			{
				printf("DOSYA DOLU ! \n");
				break;
			}

			fseek(rel,Adr*116,0);
			c=fgetc(rel);
			
		}	

		fseek(rel,Adr*116,0); 
		fprintf(rel, "%s", ingilizce);
		fprintf(rel, " ");
		fprintf(rel, "%s", turkce); 
		
	}
	fseek(rel, 0, SEEK_END);
	size=ftell (rel); //http://www.cplusplus.com/reference/cstdio/ftell/
	sure=clock()-sure;	
	printf("Cakisma Sayisi : %d", sayac);
	printf("\nSure : %.2f sn", (float)sure/CLOCKS_PER_SEC);
	printf("\nLineer.txt Dosya Boyutu : %d KB",size/1024);
	
	
	printf("\n BAGIL DOSYA OLUSTURULDU \n");
	
	fclose(dic);		
	fclose(rel);
	getch();
}

void LineerKelimeSorgula(){

	int temp ;
	int x = 1, say=0,j=0,i=0;
	char dizi[36];
	
	if ((rel=fopen("lineer.txt", "r")) == NULL) {
		printf("dosya acilamadi!\n");
		exit(1);
	}
	
	printf("\n\tAranmak istenen kelimeyi giriniz : ");
	gets(sorgu);
	
	SorAdr=Hash(sorgu);
	temp=SorAdr;
	
	fseek(rel,SorAdr*116,0 );
	fgets (ingilizce  , 36	 , rel);
	
//	temp=SorAdr;
		
	while(strcmp(sil(ingilizce), sorgu) != 0  ) {
		
		SorAdr = (SorAdr +1) % tabSize;
		
		fseek(rel,SorAdr*116,0 );
		fgets (ingilizce  , 36	, rel);
		
		if(temp == SorAdr){
				
			x=0;
			break;
		}	
	}
		if(x==1){
			fseek(rel,(SorAdr*116+36),0 );
			fgets (turkce  ,80	 , rel);	    
			printf("\n\tAranmak istenen kelimenin Turkce karsiligi :%s",turkce);
		}else
			printf("\n\tbulunamadi... ! \n");
	
	fclose(dic);		
	fclose(rel);
	getch();
//	fputs(turkce,stdout);
}

void LineerKelimeEkle() {
	
	int x=0, y=0;
	int temp;
	rel=fopen("lineer.txt","r+");
	
	printf("\n\n\tEklenecek ingilizce kelimeyi giriniz : ");
	gets(ingKel);
	printf("\n\tEklenecek Turkce kelimeyi giriniz :");
	gets(turKel);
	
	EkAdr=Hash(ingKel);
	temp=EkAdr;
	fseek(rel,EkAdr*116,0);
	fgets (ingilizce  , 36	, rel);
	
	c=fgetc(rel);
	
	
	while(c!='*') {
		
		EkAdr = (EkAdr + 1) % tabSize ;
		
		if(temp==EkAdr)
			{
				printf("\n\tDOSYA DOLU ! \n");
				break;
			}

		fseek(rel,EkAdr*116,0);
		c=fgetc(rel);
			
	}
	
	x=strlen(ingKel);	
	
	while(x<36){
		
		strncat(ingKel," ",(36-x));	
		x++;	
	}
	
	for(y=0; y<80; y++) 
		strncat(turKel," ",(80-y));
	
	fseek(rel,EkAdr*116,0);
	fputs(ingKel,rel);
	
	fseek(rel,EkAdr*116+36,0);
	fputs(turKel,rel);
	
	fclose(rel);
	getch();
		
}
void LineerKelimeSil() {

	int temp ;
	int x = 1, say=0,j=0,i=0;
	char dizi[36];
	
	if ((rel=fopen("lineer.txt", "r+")) == NULL) {
		printf("dosya acilamadi!\n");
		exit(1);
	}
	
	printf("\n\tSilmek istediðiniz kelimeyi giriniz : ");
	gets(sorgu);
	
	SorAdr=Hash(sorgu);
	temp=SorAdr;
	
	fseek(rel,SorAdr*116,0 );
	fgets (ingilizce  , 36	 , rel);
	
//	temp=SorAdr;
		
	while(strcmp(sil(ingilizce), sorgu) != 0  ) {
		
		SorAdr = (SorAdr +1) % tabSize;
		
		fseek(rel,SorAdr*116,0 );
		fgets (ingilizce  , 36	, rel);
		
		if(temp == SorAdr){
				
			x=0;
			printf("bulunamadi... ! \n");
			break;
		}	
	}
		
		if(x==1){
			fseek(rel,(SorAdr*116),0 );
			fprintf(rel, "%s" , "*");
			
			for(i=0; i<116; i++)
				fprintf(rel,"%s"," ");
			    
			printf("\n\t%s kelimesi silindi...",sorgu);
		}
			
	
	fclose(dic);		
	fclose(rel);
	getch();
}

//-----------------Quadratic probing----
void QuadraticRelativeOlustur() {
	
	clock_t sure;
	sure=clock(); 
	dic = fopen("dictionary.txt", "r");
	rel = fopen("Quadratic.txt", "w+");
	int sayac=0,t;
	
	for(int i=0; i<tabSize; i++)  
	{
		fseek(rel,i*116,0);
		fputc('*', rel );
	}
	
	for(int i=0; i<5721; i++)  
	{
		t=0;
		fgets (ingilizce  , 36 , dic);
		fgets (turkce  , 80 , dic);
		
		Adr = Hash(sil(ingilizce));		
		fseek(rel,Adr*116,0);
		c=fgetc(rel);		
			
		Temp=Adr;
		while( c != '*')
		{	
			t++;
			sayac++;
			Adr = (Temp + t*t) % tabSize  ;
			
			if(Adr==Temp)
			{
				printf("\n\tDOSYA DOLU ! \n");
				break;
			}

			fseek(rel,Adr*116,0);
			c=fgetc(rel);
			
		}	

		fseek(rel,Adr*116,0); 
		fprintf(rel, "%s", ingilizce);
		fprintf(rel, "");
		fprintf(rel, "%s", turkce); 
		
	}
	
	fseek(rel, 0, SEEK_END);
	size=ftell (rel); //http://www.cplusplus.com/reference/cstdio/ftell/
	sure=clock()-sure;	
	printf("\n\nCakisma Sayisi : %d", sayac);
	printf("\nSure : %.2f sn", (float)sure/CLOCKS_PER_SEC);
	printf("\nQuadratic.txt Dosya Boyutu : %d KB",size/1024);
	printf("\nBAÐIL DOSYA OLUÞTURULDU.");
	fclose(dic);		
	fclose(rel);
	getch();
}


void QuadraticKelimeSorgula() {
	
	int temp ;
	int x = 1, say=0,j=0,i=0,t=0;
	char dizi[36];
	
	if ((rel=fopen("Quadratic.txt", "r")) == NULL) {
		printf("dosya acilamadi!\n");
		exit(1);
	}
	
	printf("\n\tAranmak istenen kelimeyi giriniz : ");
	gets(sorgu);
	
	SorAdr=Hash(sorgu);
	temp=SorAdr;
	
	fseek(rel,SorAdr*116,0 );
	fgets (ingilizce  , 36	 , rel);
	
	strcpy(ingilizce, sil(ingilizce));

	while(strcmp(ingilizce, sorgu) != 0  ) {
		t++;
		SorAdr = (temp + t*t) % tabSize;
		
		fseek(rel,SorAdr*116,0 );
		fgets (ingilizce  , 36	, rel);
		
		if(temp==SorAdr){
			
			x=0;
			break;		
		}
	}
	
	if(x==1){
		fseek(rel,(SorAdr*116+36),0 );
		fgets (turkce  ,80	 , rel);	    
		printf("\n\tAranmak istenen kelimenin Turkce karsiligi :%s",turkce);
	}else
			printf("\n\tbulunamadi... ! \n");
		
		fclose(dic);		
		fclose(rel);
		getch();
}

void QuadraticKelimeEkle() {
	
	int x=0, y=0,t=0;
	rel=fopen("Quadratic.txt","r+");
	
	printf("\n\n\tEklenecek ingilizce kelimeyi giriniz : ");
	gets(ingKel);
	printf("\n\tKarsiligi olacak Turkce kelimeyi giriniz : ");
	gets(turKel);
	
	EkAdr=Hash(ingKel);
	
	fseek(rel,EkAdr*116,0);
	fgets (ingilizce  , 36	, rel);
	
	c=fgetc(rel);
	int temp=EkAdr;
	while(c!='*') {
		
		t++;
		EkAdr = (temp + t*t) % tabSize ;
		
		if(EkAdr==Temp)
			{
				printf("\n\tDOSYA DOLU ! \n");
				break;
			}

		fseek(rel,EkAdr*116,0);
		c=fgetc(rel);
				
	}
	
	x=strlen(ingKel);	
	
	while(x<36){
		
		strncat(ingKel," ",(36-x));	
		x++;	
	}
	
	while(y<80){
		
		strncat(turKel," ",(80-y));	
		y++;	
	}
	
	fseek(rel,EkAdr*116,0);
	fputs(ingKel,rel);
	
	fseek(rel,EkAdr*116+36,0);
	fputs(turKel,rel);
	
	fclose(rel);
	getch();
}

void QuadraticKelimeSil() {
	
	int temp ;
	int x = 1, say=0,j=0,i=0,t=0;
	char dizi[36];
	
	if ((rel=fopen("Quadratic.txt", "r+")) == NULL) {
		printf("\n\tdosya acilamadi!\n");
		exit(1);
	}
	
	printf("\n\tSilmek istediðiniz kelimeyi giriniz : ");
	gets(sorgu);
	
	SorAdr=Hash(sorgu);
	temp=SorAdr;
	
	fseek(rel,SorAdr*116,0 );
	fgets (ingilizce  , 36	 , rel);
	
//	temp=SorAdr;
		
	while(strcmp(sil(ingilizce), sorgu) != 0  ) {
		t++;
		SorAdr = (SorAdr + t*t) % tabSize;
		
		fseek(rel,SorAdr*116,0 );
		fgets (ingilizce  , 36	, rel);
		
		if(temp == SorAdr){
				
			x=0;
			printf("\n\tbulunamadi... ! \n");
			break;
		}	
	}
		
		if(x==1){
			fseek(rel,(SorAdr*116),0 );
			fprintf(rel, "%s" , "*");
			
			for(i=0; i<116; i++)
				fprintf(rel,"%s"," ");
			    
			printf("\n\t%s kelimesi silindi...",sorgu);
		}
			
	
	fclose(dic);		
	fclose(rel);
	getch();
}

//--------------------Double hashing-------

void DoubleRelativeOlustur() {

	int sayac=0,k;
	clock_t sure;
	sure=clock(); 
	
	dic = fopen("dictionary.txt", "r");
	rel = fopen("DoubleRel.txt", "w+");
	
	
	for(int i=0; i<tabSize; i++)  
	{
		fseek(rel,i*116,0);
		fputc('*', rel );
	}
	
	for(int i=0; i<tabSize; i++)  
	{
		k=0;
		
		fgets (ingilizce  , 36 , dic);
		fgets (turkce  , 80 , dic);
		
		Adr = Hash(sil(ingilizce));
		Adrtwo = HashTwo(ingilizce);		
		fseek(rel,Adr*116,0);
		c=fgetc(rel);		
			
		Temp=Adr;
		while( c != '*')
		{	
			k++;
			sayac++;
		    Adr = (Temp + k*Adrtwo) % tabSize  ;
			
			if(Adr==Temp)
			{
				printf("DOSYA DOLU ! \n");
				break;
			}

			fseek(rel,Adr*116,0);
			c=fgetc(rel);
			
		}	

		fseek(rel,Adr*116,0); 
		fprintf(rel, "%s", ingilizce);
		fprintf(rel, "");
		fprintf(rel, "%s", turkce); 
		
	}

	printf("\n\nCakisma Sayisi : %d", sayac);
	printf("\nSure : %.2f sn", (float)sure/CLOCKS_PER_SEC);
	printf("\nDoubleRel.txt Dosya Boyutu : %d KB",size/1024);
	printf("\nBAÐIL DOSYA OLLUÞTURULDU");
	
	fclose(dic);
	fclose(rel);
	getch();

}


void DoubleKelimeSorgula() {
	
	int temp ;
	int x = 1, say=0,j=0,i=0,k=0;
	char dizi[36];
	
	if ((rel=fopen("DoubleRel.txt", "r")) == NULL) {
		printf("dosya acilamadi!\n");
		exit(1);
	}
	
	printf("\n\tAranmak istenen kelimeyi giriniz : ");
	gets(sorgu);
	
	SorAdr=Hash(sorgu);
	Adrtwo = HashTwo(sorgu);
	temp=SorAdr;
	
	fseek(rel,SorAdr*116,0 );
	fgets (ingilizce  , 36	 , rel);
	
		
	while(strcmp(sil(ingilizce), sorgu) != 0  ) {
		k++;
		SorAdr = (temp + k*Adrtwo) % tabSize;
		
		fseek(rel,SorAdr*116,0 );
		fgets (ingilizce  , 36	, rel);
		
		if(SorAdr==temp){
			x=0;
			break;
		}	
	}
	
	if(x==1){
		fseek(rel,(SorAdr*116+36),0 );
		fgets (turkce  ,80	 , rel);	    
		printf("\n\tAranmak istenen kelimenin Turkce karsiligi :%s",turkce);
	}else
		printf("bulunamadi... ! \n");
	
	fclose(dic);		
	fclose(rel);
	getch();
}

void DoubleKelimeEkle() {
	
	int x=0, y=0;
	int temp,k=0;
	rel=fopen("DoubleRel.txt","r+");
	
	printf("\n\n\tEklenecek ingilizce kelimeyi giriniz : ");
	gets(ingKel);
	printf("\n\tEklenecek Turkce kelimeyi giriniz :");
	gets(turKel);
	
	EkAdr=Hash(ingKel);
	Adrtwo=HashTwo(ingKel);
	temp=EkAdr;
	
	fseek(rel,EkAdr*116,0);
	fgets (ingilizce  , 36	, rel);
	
	c=fgetc(rel);
	
	while(c!='*') {
		k++;
		EkAdr = (EkAdr +  k*Adrtwo ) % tabSize ;
		
		if(temp==EkAdr)
			{
				printf("DOSYA DOLU ! \n");
				break;
			}

		fseek(rel,EkAdr*116,0);
		c=fgetc(rel);
			
	}
	
	x=strlen(ingKel);	
	
	while(x<36){
		
		strncat(ingKel," ",(36-x));	
		x++;	
	}
	
	for(y=0; y<80; y++) 
		strncat(turKel," ",(80-y));
	
	fseek(rel,EkAdr*116,0);
	fputs(ingKel,rel);
	
	fseek(rel,EkAdr*116+36,0);
	fputs(turKel,rel);
	
	fclose(rel);
	getch();
}

void DoubleKelimeSil() {
	
	int temp ;
	int x = 1, say=0,j=0,i=0,k=0;
	char dizi[36];
	
	if ((rel=fopen("DoubleRel.txt", "r+")) == NULL) {
		printf("dosya acilamadi!\n");
		exit(1);
	}
	
	printf("\n\tSilmek istediðiniz kelimeyi giriniz : ");
	gets(sorgu);
	
	SorAdr=Hash(sorgu);
	Adrtwo=HashTwo(sorgu);
	temp=SorAdr;
	
	fseek(rel,SorAdr*116,0 );
	fgets (ingilizce  , 36	 , rel);
	
//	temp=SorAdr;
		
	while(strcmp(sil(ingilizce), sorgu) != 0  ) {
		k++;
		SorAdr = (SorAdr +k*Adrtwo) % tabSize;
		
		fseek(rel,SorAdr*116,0 );
		fgets (ingilizce  , 36	, rel);
		
		if(temp == SorAdr){
				
			x=0;
			printf("\tBulunamadi... ! \n");
			break;
		}	
	}
		
		if(x==1){
			fseek(rel,(SorAdr*116),0 );
			fprintf(rel, "%s" , "*");
			
			for(i=0; i<116; i++)
				fprintf(rel,"%s"," ");
			    
			printf("\n\t%s kelimesi silindi...",sorgu);
		}
			
	
	fclose(dic);		
	fclose(rel);
	getch();
}

//------------------------
void HashBuyuklukSec() {
	
	printf("\n\t\\*_ _ _ _ _ _ _ _HASH YÖNTEMÝ ÝLE SÖZLÜK UYGULAMASI_ _ _ _ _ _ _ */");	
	printf("\n\n");
	printf("\n\tHash Tablo Boyutu Seç  \n");
	printf("\n\t[1]NORMAL ÝÞLEM HASH TABLO BOYUTU ");
	printf("\n\t[2]1/5 oranýnda : 1151 ");
	printf("\n\t[3]1/2 oranýnda : 2867 ");
	printf("\n\t[4]x2  oranýnda : 11443 ");
	
	printf("\n\n\tSecimi Giriniz : ");
	char sec=getch();
	printf("%c",sec);
	
	if(sec=='1')
		tabSize = 5801;
		
	else if(sec=='2') 
		tabSize = 1151;
		
	else if(sec=='3')
		tabSize = 2867;
	
	else if(sec=='4')
		tabSize = 11443;
		
}

void RelativeOlustur() {

	char s;
	LineerRelativeOlustur();
	QuadraticRelativeOlustur();
	DoubleRelativeOlustur();
	
	do
	{
		system("cls");

		printf(" \n");
		printf(" BAGIL DOSYA SEC \n");
		printf(" [1] LINEER \n");
		printf(" [2] QUADRATIC \n");
		printf(" [3] DOUBLE\n");
		printf(" \n SECIMINIZ [1..3] = ");

		s=getch();

		switch(s)
		{
			case '1' :
			
			do
			{
				system("cls");

			printf(" \n");
			printf(" SOZLUK UYGULAMASI\n\n");
			printf(" [1] KELIME SORGULA \n");
			printf(" [2] KELIME EKLE\n");
			printf(" [3] KELIME SIL \n");
			printf(" [4] EXIT \n");
			printf(" \n SECIMINIZ [1..3] = ");

				s=getch();
		
				switch(s)
				{
					case '1' :	
						
						LineerKelimeSorgula();
					
					case '2' :	
						LineerKelimeEkle();
						
					case '3' :	
						LineerKelimeSil();
				
					case '4' :
						exit(1);
					
				}
		
			}  while(1);
				
			break;
				
			case '2' :		
			do
			{
				system("cls");

			printf(" \n");
			printf(" SOZLUK UYGULAMASI\n\n");
			printf(" [1] KELIME SORGULA \n");
			printf(" [2] KELIME EKLE\n");
			printf(" [3] KELIME SIL \n");
			printf(" [4] EXIT \n");
			printf(" \n SECIMINIZ [1..3] = ");
	
			s=getch();
	
			switch(s)
			{
				case '1' :	
					QuadraticKelimeSorgula();
					break;
				
				case '2' :	
					QuadraticKelimeEkle();
					break;
					
				case '3' :	
					QuadraticKelimeSil();
					break;
					
				case '4' :	
					exit(1);
			}
			
			}  while(1);
		
			break;
			
			case '3' :
			do
			{
				system("cls");

			printf(" \n");
			printf(" SOZLUK UYGULAMASI\n\n");
			printf(" [1] KELIME SORGULA \n");
			printf(" [2] KELIME EKLE\n");
			printf(" [3] KELIME SIL \n");
			printf(" [4] EXIT \n");
			printf(" \n SECIMINIZ [1..3] = ");
	
			s=getch();
	
			switch(s)
			{
				case '1' :
					DoubleKelimeSorgula();
					break;
					
				case '2' :	
					DoubleKelimeEkle();
					break;
					
				case '3' :	
					DoubleKelimeSil();
					break;
					
				case '4' :	
					exit(1);
			}
			
			}  while(1);
			
			break;	
		}
	}  while(1);

}

int main(int argc, char* argv[])
{ 
	setlocale(LC_ALL,"Turkish");
	
	char s;
	
	HashBuyuklukSec();
	printf("\n");
	RelativeOlustur();
	
	return 0;
}


