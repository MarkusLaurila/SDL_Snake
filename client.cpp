#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Winsock2.h>
#include <errno.h> 


//NAMESPACE

using namespace std;

//Snake game using ascii art
#define N 20
#define M 40
//Variables
#define PORT 0000 //Replace with own server Port
#define BUFLEN 512
#define SERVER "0.0.0.0" //Replace with own server IP



class HRException
{
private:
    const char *m_pMessage;
public:
    HRException() : m_pMessage("")
    {}
   virtual ~HRException(){}
   HRException(const char *pMessage) :  m_pMessage(pMessage){}
   const char * what() {return m_pMessage;}
   /*Example of how to use;
   try{

            // Connect to server
        cout << "Attempting to connect to " << inet_ntoa(sockAddr.sin_addr)
             << ":" << SERVER_PORT <<  "... ";

            if (connect(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr))!=0)
            throw HRException("could not connect.");

                 cout << "connected.\n"; 
   */
};


int i,j,Field[N][M],x,y,Gy,Head,Tail,Game,Frogs,a,b,kbVar,Direction,Score,HighScore,Speed;
FILE *f;
BOOLEAN sendpackage = false;




void snakeInit(){
    f=fopen("highscore.txt","r");
    fscanf(f,"%d",&HighScore);
    fclose(f);
    for ( i = 0; i < N; i++)
    {
        for ( j = 0; j < M; j++)
        {   Field[i][j]=0;
            
        }   
    }
    //Assign variables
    x=N/2; y=M/2; Head=5; Tail=1; Gy=y; Game = 0,Frogs = 0; Direction ='d'; Score = 0,Speed=100;

    for ( i = 0; i < Head; i++)
    {
        Gy++;
        Field[x][Gy-Head] = i+1;
    }
    
    
}
void Random(){
    srand(time(0));
    a = 1+ rand() % 18;
    b = 1+ rand() % 38;
    if(Frogs == 0&&Field[a][b] == 0)
    {
        Field[a][b] = -1;
        Frogs = 1;
        if(Speed>30 && Score !=0) Speed = Speed -10;
    }
}
void PrintGame(){
for ( i = 0; i <= M+1; i++)
 {
    if (i==0)
    {
        printf("%c",201);
    }else if(i==M+1){
        printf("%c",187);
    }else{
        printf("%c",205);
         }
    
    }
    printf("    Current Score: %d HighScore: %d",Score,HighScore);
 
    printf("\n");  

   for (i = 0; i < N;i++)
    {
        printf("%c",186);
        for (j = 0; j < M; j++)
        {
          if (Field[i][j]==0)printf(" ");
          if (Field[i][j]>0 && Field[i][j]!=Head)printf("%c",176);
          if (Field[i][j]==Head)printf("%c",178);
          if(Field[i][j]==-1)printf("%c",15);
          if(j==M-1) printf("%c\n",186);
            
        }
        
    } 
    for ( i = 0; i <= M+1; i++)
 {
    if (i==0)
    {
        printf("%c",200);
    }else if(i==M+1){
        printf("%c",188);
    }else{
        printf("%c",205);
         }
    
    }
}
//Get keyboard event 
int getCH_noBlock(){
    if(_kbhit())
        return _getch();
        else
        return -1;
}
void GameOver(){
    //Game is over
    printf("\a");
    Sleep(1500);
    system("CLS");
    //Save to highscore.txt file if playing locally
    if(Score > HighScore){
        printf("NEW HIGHSCORE: %d\n\n",Score);
       
        system("pause");
        HighScore = Score;
        f=fopen("highscore.txt","w");
       // sendpackage = true; 
        fprintf(f,"%d",Score);
        
    }
    system("CLS");

    printf("\n\n               GAME OVER");
    printf("                   Score : %d \n\n",Score);
    printf("                   Press Enter to play again or ESC to exit...\n");

    while (1)
    {
        kbVar = getCH_noBlock();
        if(kbVar == 13){
            Game = 0;
            snakeInit();
            break;
        }else if(kbVar == 27)
        {
            Game = 1;
            break;
        }
    }
    system("CLS");
    
}
void Movement(){
kbVar = getCH_noBlock();
    kbVar = tolower(kbVar);
    if(((kbVar == 'd'||kbVar == 'a' )|| (kbVar =='w'|| kbVar =='s'))
    &&(abs(Direction-kbVar)>5)) Direction = kbVar;
        if (Direction =='d')
        {
            y++;
            //Call game over if snake hits own body
            if(Field[x][y]!=0 && Field[x][y]!=-1){GameOver();}
            if(y == M-1)y = 0;
            //Eat frog
            if(Field[x][y]==-1){
                Frogs =0;  Tail -=2; Score+=5;     }
            Head++;
            Field[x][y] = Head;
        }
            if (Direction =='a')
        {
            y--;
            //Call game over if snake hits own body
            if(Field[x][y]!=0 && Field[x][y]!=-1){GameOver();}
            if(y == 0)y = M-1;
            //Eat frog
            if(Field[x][y]==-1){
                Frogs =0;  Tail -=2;  Score+=5;   }
            Head++;
            Field[x][y] = Head;
        }
            if (Direction =='w')
        {
            x--;
            //Call game over if snake hits own body
            if(Field[x][y]!=0 && Field[x][y]!=-1){GameOver();}
            if(x ==-1)x=N-1;
            //Eat frog
            if(Field[x][y]==-1){
                Frogs =0;   Tail -=2;  Score+=5;   }
            Head++;
            Field[x][y] = Head;
        }
            if (Direction =='s')
        {
            x++;
            //Call game over if snake hits own body
            if(Field[x][y]!=0 && Field[x][y]!=-1){GameOver();}
            if(x ==N-1)x=0;
            //Eat frog
            if(Field[x][y]==-1){
                Frogs =0;  Tail -=2;  Score+=5;    }
            Head++;
            Field[x][y] = Head;
        }
}
void ResetScreenPos()
{
    //Use a handle to reset screen position to start print in 0,0;
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut,Position);
}
void TailRemoval()
{
    for ( i = 0; i < N; i++)
    {
        for (j  = 0; j < M; j++)
        {
            if (Field[i][j]==Tail)
            {
                Field[i][j] = 0;
            }
            
        }
        
    }
    Tail++;
    
}









int main()
{
    system("title Snake game");

    // initialise winsock
    WSADATA ws;
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");


    // create socket
    sockaddr_in server;
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        printf("socket() failed with error code: %d", WSAGetLastError());
        return 2;
    }


    // setup address structure
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);

    system("CLS");//CLEAR
   
    // start game
    snakeInit();
    printf("\n\n\nSnake game with Linux-server recording highscore and sending last saved highscore back to user\n");
     printf("\n\n\nWeb-programming course project\n");
    system("pause");
    system("CLS");
    while (Game == 0)
    {
         
        
         ResetScreenPos();
        
            Random();
         Movement();
         TailRemoval();
        Sleep(Speed);
        PrintGame();

    if(sendpackage == true){
     
         char message[BUFLEN];
         sprintf(message,"%d",HighScore);
        printf("Sending data to server... ");
        
        
        // send the message
        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code: %d", WSAGetLastError());
            return 3;
        }
        printf("Data send successfully!");
        Sleep(1000);
        system("CLS");
        // receive a reply and print it
        // clear the answer by filling null, it might have previously received data
        char answer[BUFLEN] = {};
       
        
        printf("Waiting for server answer....");
        // try to receive some data, this is a blocking call
        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code: %d", WSAGetLastError());
            exit(0);
        }

      
        cout << answer << "\n";
        sendpackage = false;
        //Server data receive removed, due to shutting down server
        //Program runs locally saving in txt.file
        
    }
       
    }

    
 
closesocket(client_socket);
    WSACleanup();
return 0;

}

