#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
 
 //struktury
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
struct osoba{
    int PID;
    char nazwa[10];
} osoba;

struct grupa{
    char nazwa[10];
    int ktonalezy[10];
} grupa;

int main(){
    char nazwaK[10];
  
    //tworzenie kolejki

    id= msgget(777777, 0666|IPC_CREAT);
   // printf("Id %d",id);
    if (id== -1){
        printf("Utworzenie kolejki komunikatów nie powiodło się \n");
    }
    //pobranie nazwy
    printf("Podaj nazwę\n");
    scanf("%s",nazwaK);
   // wiadomoscW.PIDKlienta=getpid();
   // wiadomoscW.typ=1;
    int czyPowiodlo;
    int operacja;
    
   while(1){
        wiadomoscW.PIDKlienta=getpid();
        printf("Twoj PID to %d\n",getpid());
           wiadomoscW.typ=1;
       printf("Możliwe operacje:\n");
        printf("Obsługa konta użytkownika:\n");
        printf("1.Zaloguj\n2.Wyloguj\n3.Zalogowani użytkownicy\n4.Lista użytkowników należących do danej grupy tematycznej\n");
        printf("Obsługa grup tematycznych\n");
        printf("5.Zapis do grupy\n6.Wypis z grupy\n7.Podgląd listy dostępnych grup\n");
        printf("Wiadomości\n");
        printf("8.Wysłanie wiadomości do grupy\n9.Wysłanie wiadomości do użytkownika\n10.Odebranie wiadomości");
        printf("\n11.Zakoncz\n");
        
       scanf("%d",&operacja);
        
        switch (operacja){
             case 1:
                  //Logowanie
                wiadomoscW.coZrobic=1;
                strcpy(wiadomoscW.text,nazwaK);
                wiadomoscW.typ=1;
                czyPowiodlo = msgsnd(id, &wiadomoscW, sizeof(wiadomoscW)-sizeof(long), 0);
                if (czyPowiodlo == -1) {
                    printf("Logowanie nie powiodło się\n");
                }else {
                    printf("Jesteś zalogowany\n");}
                  
               break;
             case 2:
                  //Wylogowywanie
                wiadomoscW.coZrobic=2;
                strcpy(wiadomoscW.text,nazwaK);
                czyPowiodlo = msgsnd(id, &wiadomoscW, sizeof(wiadomoscW)-sizeof(long), 0);
                if (czyPowiodlo == -1) {
                    printf("Wylogowanie nie powiodło się\n");
                }else {
                    printf("Jesteś wylogowany\n");}
                break;
              case 3:
                  //Wyswietlanie zalogowanych uzytkownikow 
                wiadomoscW.coZrobic=3;  
            
                czyPowiodlo = msgsnd(id, &wiadomoscW, sizeof(wiadomoscW)-sizeof(long), 0);
                if (czyPowiodlo == -1) {
                    printf("Nie powiodło się\n");
                }else {
                    printf("powiodło się\n");}
                  break;
              case 4:
                 //podglad listy zapisanych do danej grupy tematycznej
                wiadomoscW.coZrobic=4;
                printf("Podaj nazwe grupy\n");
                scanf("%s",wiadomoscW.nazwaGrupy);
                czyPowiodlo = msgsnd(id, &wiadomoscW, sizeof(wiadomoscW)-sizeof(long), 0);
                if (czyPowiodlo == -1) {
                    printf("Nie powiodło się\n");
                }else {
                    printf("powiodło się\n");}
                  break;
              case 5:
                  //Zapis do grupy 
                  wiadomoscW.coZrobic=5;
                  printf("Podaj nazwe grupy do której chcesz się zapisac");
                  scanf("%s",wiadomoscW.nazwaGrupy);
                  czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                      break;
              case 6:
                  //Wypisz się z grupy
                  wiadomoscW.coZrobic=6;
                  printf("Podaj nazwe grupy z której chcesz się wypisac\n");
                  scanf("%s",wiadomoscW.nazwaGrupy);
                  czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                      break;
              case 7:
                  //Podglad listy dostepnych grup
                  wiadomoscW.coZrobic=7;
                  czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                      break;
              case 8:
                  //Wyslij wiadomosc do grupy
                  wiadomoscW.coZrobic=8;
                  printf("Do jakiej grupy chcesz wysłać\n");
                  scanf("%s",wiadomoscW.nazwaGrupy);
                  printf("Podaj co chcesz przekazac grupie\n");
                  scanf("%s",wiadomoscW.text);
                  czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                      break;
              case 9:
                  wiadomoscW.coZrobic=9;
                  //wiadomosc do uzytkowanika
                  printf("Podaj pid uzytkowanika\n");
                  scanf("%d",&wiadomoscW.PIDuzytkownika);
                  printf("Co chcesz przekazać użytkownikowi?");
                  scanf("%s",wiadomoscW.text);
                  czyPowiodlo=msgsnd(id,&wiadomoscW,sizeof(wiadomoscW)-sizeof(long),0);
                  if (czyPowiodlo==-1){
                      printf("Nie powiodło się\n");
                  }else {
                      printf("Powiodło się\n");}
                      break;
             case 10:
                  // Odbieranie wiadomosci 
                     czyPowiodlo= msgrcv(id,&wiadomoscO,sizeof(wiadomoscO)-sizeof(long), getpid(),0);
                        if(czyPowiodlo==-1){
                       printf("Nie odebrano wiadomości");
                    }
                   if ((czyPowiodlo != -1) && (wiadomoscO.PIDKlienta==getpid())) {
                        printf("%s\n", wiadomoscO.text);
                       printf("%s\n", wiadomoscO.dataNadania);
                      } 
            
             break;
            
        
                    
          case 11:
                exit(1);
                break;
            default:
                break;
                }
                
    }
        

    
   return 0;
    
}
