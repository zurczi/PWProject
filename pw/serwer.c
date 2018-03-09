#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


 int id;
 
struct wiadomosc{
long typ;
char text[200];
int PIDKlienta;
int coZrobic;
char nazwaGrupy[10];  //uzytkowanika tez
char dataNadania[30];
int PIDuzytkownika;
//int twojNr;
} wiadomoscW,wiadomoscO;

typedef struct osoba{
    int PID;
    char nazwa[10];
    int zalogowany;
}osoba;

typedef struct grupa{
    char nazwa[10]; 
    int ktoNalezy[3]; // 3 osoby do grupy naleza i numer pid 
}grupa;

int main(){
 
   time_t curtime;
   time(&curtime);
   osoba *tabOsob=malloc(9* sizeof(osoba));
    grupa *tabGrup=malloc(3* sizeof(grupa));
    int osoby1,num;
    for(osoby1=0;osoby1<9;osoby1++){
        tabOsob[osoby1].PID=19;}  //uzupelniam tab osob jakas losowa wartoscia
    int grupy1,kto;
    for(grupy1=0;grupy1<3;grupy1++){
        for (kto=0;kto<3;kto++){
        tabGrup[grupy1].ktoNalezy[kto]=0;} 
        for(num=0;num<10;num++){ 
        tabGrup[grupy1].nazwa[num]='0';}
    }
    
    
   
    FILE *plik;
    int g;
     plik=fopen("127227_plik.txt","r");
   for(g=0;g<12;g++){
       if (g < 9){
           fgets(tabOsob[g].nazwa,10,plik);
    } 
       else {
           int i;
           for(i=0;i<3;i++){
           fgets(tabGrup[i].nazwa,10,plik);
        }
           
    }}
    int i1,j1;
    for (i1=0;i1<9;i1++){
        for(j1=0;j1<9;j1++){
            if(tabOsob[i1].nazwa[j1] >172 ||  tabOsob[i1].nazwa[j1]<97){
            tabOsob[i1].nazwa[j1]=0;
            }}}
       
  for (i1=0;i1<3;i1++){
        for(j1=0;j1<9;j1++){
            if(tabGrup[i1].nazwa[j1] >57 ||  tabGrup[i1].nazwa[j1]<49){
            tabGrup[i1].nazwa[j1]=0;
      
            }}}
       


    int i=0;

     int nr; 

    id= msgget(777777, 0666|IPC_CREAT);
 
    if (id == -1){
        printf("Utworzenie kolejki komunikatów nie powiodło się \n");
    }
    int czyPowiodlo;
    int petla=1;
    while(petla==1){
        
        int czyNalezy=0;
      
        int otrzymanie=msgrcv(id,&wiadomoscO,sizeof(wiadomoscO)-sizeof(long),1,0);

         strcpy(wiadomoscW.dataNadania,ctime(&curtime));
         
        if(otrzymanie==-1){
            printf("Nie otrzymano wiadomosci\n");
        } else {
          
            int i;
            for(i=0;i<sizeof(tabOsob);i++){
                if((wiadomoscO.text[0]==tabOsob[i].nazwa[0])){ //szukam klienta o podanej nazwie
                czyNalezy=1;
                nr=i; 
                }}}
        //Obsluga klientów
  
        switch (wiadomoscO.coZrobic){
            case 1:
                //ZALOGUJ 
                if(czyNalezy==1){
                  
               wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                tabOsob[nr].zalogowany=1;
               tabOsob[nr].PID=wiadomoscO.PIDKlienta;
            
                strcpy(wiadomoscW.text,"Zalogowano poprawnie");
             
                wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                   wiadomoscW.typ=wiadomoscO.PIDKlienta;
                czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");
             
                }
                 
                }else {
       
                wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                strcpy(wiadomoscW.text,"Zalogowanie nie powiodło się");
                   wiadomoscW.typ=wiadomoscO.PIDKlienta;
                czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}}
                     
                break;
           case 2:
               //WYLOGUJ DZIALA
                if(czyNalezy==1){
                wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                tabOsob[nr].zalogowany=0;
                strcpy(wiadomoscW.text,"Wylogowanie powiodło się");
                   wiadomoscW.typ=wiadomoscO.PIDKlienta;
                czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}}
                       
                break;
            case 3:
                //zalogowani uzytkownicy  
            { int j,k,iterator;
                iterator=0;
                char tabZalogowanych[200];
                for(j=0;j<sizeof(tabOsob);j++){
                    if(tabOsob[j].zalogowany==1){
                           strcat(tabZalogowanych,tabOsob[j].nazwa);
                           strcat(tabZalogowanych,"\n");
                    }}
                strcpy(wiadomoscW.text,tabZalogowanych);
               
        wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
          wiadomoscW.typ=wiadomoscO.PIDKlienta;
         czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                break;
             
            }
            case 4:
                //podglad listy zaisanych do danej grupy tamatycznej  
            {  int z,b,iterator,k,osoba;
                char tabNalezacych[200];
               for(z=0;z<3;z++){ //szukam nazwy grupy 
                 
                if(strcmp(wiadomoscO.nazwaGrupy,tabGrup[z].nazwa)==0){
                    for(b=0;b<3;b++){  //po osobach nalezacych 
         
                            osoba=tabGrup[z].ktoNalezy[b];
                        
                        for (iterator=0;iterator<9;iterator++){
                            
                            if(osoba==tabOsob[iterator].PID){
                               
                                strcat(tabNalezacych,tabOsob[iterator].nazwa);
                                
                            }}}}}
                
 
                
                 strcpy(wiadomoscW.text,tabNalezacych);
        wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
       wiadomoscW.typ=wiadomoscO.PIDKlienta;
         czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                break;}
            case 5:
                //zapisanie sie do grupy    
            { int j,w;
                int nalezacy;
                     for(j=0;j<3;j++){         
                    if((strcmp(tabGrup[j].nazwa,wiadomoscO.nazwaGrupy)==0)){    
                    
                        for(nalezacy=0;nalezacy<3;nalezacy++){
                        
                            if(tabGrup[j].ktoNalezy[nalezacy]==0){ 
                              
                                tabGrup[j].ktoNalezy[nalezacy]=wiadomoscO.PIDKlienta; 
                               
                                  strcpy(wiadomoscW.text,"Dołączyłeś do grupy");
                                  break;}}}}
                       
                    wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                        wiadomoscW.typ=wiadomoscO.PIDKlienta;
                    czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                break;
                
            }
                
            case 6:
            {  //Wypisz sie z grupy 
                int j;
                int nalezacy;
                for(j=0;j<3;j++){
                    if((strcmp(wiadomoscO.nazwaGrupy,tabGrup[j].nazwa))==0){
                        for(nalezacy=0;nalezacy<3;nalezacy++){
                         
                            if(tabGrup[j].ktoNalezy[nalezacy]==wiadomoscO.PIDKlienta){
                         
                                tabGrup[j].ktoNalezy[nalezacy]=0;
                            
                                  strcpy(wiadomoscW.text,"Zostałeś wypisany z grupy");
                                  break;
                            }
                          
                        }}}
                    wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                        wiadomoscW.typ=wiadomoscO.PIDKlienta;
                    czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                break;
                
            }
            
            case 7: 
                //Podglad listy dostepnych grup        
            { char tabDostepnychGrup[200];
                int i,iterator,j;
                for(i=0;i<3;i++){
                    if(tabGrup[i].nazwa[0]=='0'){ //0 bo wtedy nie istnieje 
                    }else {
                      
                            strcat(tabDostepnychGrup,tabGrup[i].nazwa);
                            strcat(tabDostepnychGrup,"\n");
                        
                        }
                     
                    }
                    
                    strcpy(wiadomoscW.text,tabDostepnychGrup);
                    wiadomoscW.PIDKlienta=wiadomoscO.PIDKlienta;
                   
                  wiadomoscW.typ=wiadomoscO.PIDKlienta; 
         
                     czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                break;
           
            }
            case 8:
                //wysylanie wiadomosci do grupy 
            { int i,j;
                strcpy( wiadomoscW.text,wiadomoscO.text);
                for(i=0;i<3;i++){
                    if((strcmp(tabGrup[i].nazwa,wiadomoscO.nazwaGrupy))==0){
                        for(j=0;j<3;j++){
                            if(!(tabGrup[i].ktoNalezy[j]==0)){
                                wiadomoscW.PIDKlienta=tabGrup[i].ktoNalezy[j];
                              
                               wiadomoscW.typ=tabGrup[i].ktoNalezy[j];
                                czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                                 if (czyPowiodlo==-1){
                                        printf("Nie powiodło się\n");
                                }else {
                                 printf("Powiodło się\n");}
                            }}}}
              break;
                
            }
                            
            case 9:
            {   //wysylanie wiadomosci do uzytkowanika
                 int i,j;
              
                                wiadomoscW.PIDKlienta=wiadomoscO.PIDuzytkownika;
                                strcpy(wiadomoscW.text,wiadomoscO.text);
                           
                                 wiadomoscW.typ=wiadomoscO.PIDuzytkownika;
                                czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                                 if (czyPowiodlo==-1){
                                        printf("Nie powiodło się\n");
                                }else {
                                    printf("Powiodło się\n");}
                           
                break;
             
        }
            
        }
            
    }

    
}
