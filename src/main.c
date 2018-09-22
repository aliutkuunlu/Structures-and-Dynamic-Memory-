#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const char filename[] = "commands.txt";
char lineG [128];

//I use my pop method for delete P in user adding section for an easy divide
void BE_pop(char TargetStack[500])
{
    int i;
    for(i=0;i<500;i++)
    {
        char change = TargetStack[i+1];
        TargetStack[i] = change;
    }
    char replacer =' ';
    TargetStack[499]=replacer;
}

//Here is my structure, it can keep all the information i need
struct User
{
    /*Personal Information*/
    int ID;
    char Name[50];
    int Age;
    char University[70];
    /*Public Information*/
    int FriendCount;
    int* FriendsList; /*This pointer takes memory blocks dynamically for every friend. But it only keeps ID's of friends*/
    int BanCount;
    int* BannedList; /*This pointer takes memory blocks dynamically for every blocked user. But it only keeps ID's of blocked's*/
};

// Here is readLine method that ready each single line in commands.txt
static char * readLine()
{
    static FILE *file;
    if(file == NULL)
    file = fopen ( filename, "r" ); /*Open file*/
    if( file != NULL )
    {
        if( fgets ( lineG, sizeof (lineG), file ) != NULL ) /* Read a line*/
        {
            return lineG;
        }
        else
        {
            fclose ( file );
        }

    }
    else perror ( filename ); /* Why didn't the file open? */
    return NULL;
}

int main ( void )
{
    printf("Exp-5: SOCIAL NETWORK\nBerk Ecer\nHacettepe University\nComputer Engineering\n");
    char * line; /*when readline method reads a line from txt file it sends it here*/
    int UserCounter=0; /*I must count the all users for my loops*/
    int UserIDs=1000; /*I decide to give sequential ID's to users 1000,1001...etc. This is their start point*/
    struct User * UserList; /*Here is my UserList pointer that keeps every users but its memory will change dynamically*/

    int i =0;
    for(i=0; i<5000; i++)
    {
        char commandLine[500]="";
        char commandLine_WithOutCommandWord[500]="";
        line = readLine();
        if(line != NULL)
        {
            /*These are my line works i edit and copied them as what as i need*/
            strcpy(commandLine,line);
            strcpy(commandLine_WithOutCommandWord,line);
            BE_pop(commandLine_WithOutCommandWord);
            BE_pop(commandLine_WithOutCommandWord);

            /*These are about parsin the lines*/
            char *ptr;
            char* piecesOfCommandLine[50];

            int filler;
            for(filler=0;filler<50;filler++)
            {
                piecesOfCommandLine[filler]="0";
            }

            /*In here i basically use strtok to parse my string it send every piece to ptr and ptr is sending my piecesofCommandLine array*/
            int indexer =0;
            ptr=strtok(commandLine_WithOutCommandWord,",\n");
            while (ptr!= NULL)
            {
                piecesOfCommandLine[indexer]=ptr;
                ptr= strtok (NULL, ",\n");
                indexer++;

            }

            if(commandLine[0]=='P') /*ADDING USER SECTION*/
            {
                /*Initializing a new User*/
                struct User NewUser;
                NewUser.ID=UserIDs;
                NewUser.FriendCount=0;
                NewUser.BanCount=0;
                strcpy(NewUser.Name,piecesOfCommandLine[0]);
                NewUser.Age=atoi(piecesOfCommandLine[1]);
                strcpy(NewUser.University,piecesOfCommandLine[2]);
                UserIDs++;
                /*If my user counter = 0 that means UserList pointer is null. I need a memory block for first user size of a user.*/
                /*I used malloc for allocating first users memory, for the other users i used realloc to expand my UserList*/
                if(UserCounter==0)
                {
                    UserList=(struct User*)malloc(sizeof(NewUser));
                    UserList[UserCounter]=NewUser;
                    UserCounter++;
                }
                else if(UserCounter!=0)
                {
                    UserList=(struct User*)realloc(UserList,(UserCounter+1)*sizeof(NewUser));
                    UserList[UserCounter]=NewUser;
                    UserCounter++;
                }
            }
            else if(commandLine[0]=='F') /*ADDING FRIEND SECTION*/
            {
                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*Checking the first user*/
                    {
                        if(UserList[i].FriendCount==0) /*If his/her friend counts is 0, programme will give e block of memory with malloc*/
                        {
                            UserList[i].FriendsList=(int*)malloc(sizeof(int));
                            int friendID=666; /*I will define a frinedID initialize value is 666.*/

                            /* If programme find input users, it will change that friendID 66 to their ID's*/
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[1])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0; /*For checking the user is already my friend*/

                            /*if programme finds input user in friend list, it will change that 0 to 1*/
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }
                            /*If programme finds user and if user is not has already be in friendlist it will add it to friendlist*/
                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[0]=friendID;
                                UserList[i].FriendCount++;
                                printf("\n%s is now friend with %s (F):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }
                            else if(UserList[i].ID==friendID) /*If user tries to add himself/herself to his/her friendlist */
                            {
                                printf("\nYou cannot add yourself as your friend (F):\n");
                            }
                            else if(friendID==666) /*If user triest to add unknown user to his(her friendList*/
                            {
                                printf("\n%s is not our member (F):\n",piecesOfCommandLine[1]);
                            }
                            else if(isAlreadyMyFriend==1) /*If user tries to add friend again to his/her friendList*/
                            {
                                printf("\n%s has already been friend with %s (F):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }

                        }
                        else if(UserList[i].FriendCount!=0) /*If user's friend count is not 0 programme will expand the memory with realloc*/
                        {
                            UserList[i].FriendsList=realloc(UserList[i].FriendsList,(UserList[i].FriendCount+1)*sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[1])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[UserList[i].FriendCount]=friendID;
                                UserList[i].FriendCount++;
                                printf("\n%s is now friend with %s (F):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }
                            else if(UserList[i].ID==friendID)
                            {
                                printf("\nYou cannot add yourself as your friend (F):\n");
                            }
                            else if(friendID==666)
                            {
                                printf("\n%s is not our member,You cant add (F):\n",piecesOfCommandLine[1]);
                            }
                            else if(isAlreadyMyFriend==1)
                            {
                                printf("\n%s has already been friend with %s (F):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }

                        }
                    }
                    else if(strcmp(UserList[i].Name,piecesOfCommandLine[1])==0) /*If A add B to its friends automatically B must add A to its friends*/
                    {
                        if(UserList[i].FriendCount==0)
                        {
                            UserList[i].FriendsList=(int*)malloc(sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[0])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[0]=friendID;
                                UserList[i].FriendCount++;
                            }

                        }
                        else if(UserList[i].FriendCount!=0)
                        {
                            UserList[i].FriendsList=realloc(UserList[i].FriendsList,(UserList[i].FriendCount+1)*sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[0])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[UserList[i].FriendCount]=friendID;
                                UserList[i].FriendCount++;
                            }
                         }
                    }
                }
            }
            else if(commandLine[0]=='R') /*REMOVING FRIEND SECTION*/
            {
                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*Find the first user*/
                    {
                        int isMyFriend=0; /*for checking if it is users friend*/
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[1])==0)/*Find the second user*/
                            {
                                int a;
                                for(a=0;a<UserList[i].FriendCount;a++)
                                {
                                    if(UserList[i].FriendsList[a]==UserList[j].ID) /*If they are friends remove them from their friendList*/
                                    {
                                        isMyFriend++; /*if it users friends that integer will ++*/
                                        int replacer;
                                        for(replacer=a;replacer<UserList[i].FriendCount;replacer++)
                                        {
                                            UserList[i].FriendsList[a]=0; /*replace with 0*/
                                        }
                                    }
                                }
                            }
                        }
                        if(isMyFriend!=0) /*if not 0 that means friendship successfully broken*/
                        {
                            printf("\n%s and %s are not friend anymore (R).\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                        }
                        else if(isMyFriend==0) /*if it is 0 user have not been friend with second user*/
                        {
                            printf("\n%s and %s have not been friends (R).\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                        }

                    }
                    else if(strcmp(UserList[i].Name,piecesOfCommandLine[1])==0) /*If A removes B, automatticaly B must remove A*/
                    {
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[0])==0)
                            {
                                int a;
                                for(a=0;a<UserList[i].FriendCount;a++)
                                {
                                    if(UserList[i].FriendsList[a]==UserList[j].ID)
                                    {
                                        int replacer;
                                        for(replacer=a;replacer<UserList[i].FriendCount;replacer++)
                                        {
                                            UserList[i].FriendsList[a]=0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if(commandLine[0]=='B') /*BLOCKING SECTION*/
            {
                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0)
                    {
                        if(UserList[i].BanCount==0) /*ýf users ban count is 0 programme will give him a block of memory with malloc for blockeds*/
                        {
                            UserList[i].BannedList=(int*)malloc(sizeof(int));
                            /*same idea for searching friends*/
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[1])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }
                            /*same with isMyFriend*/
                            int isAlreadyBanned=0;
                            int y;
                            for(y=0;y<UserList[i].BanCount;y++)
                            {
                                if(UserList[i].BannedList[y]==friendID)
                                {
                                    isAlreadyBanned=1;
                                }
                            }
                            /*If everything is correct it will ad second user to first users bannedList*/
                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyBanned!=1)
                            {
                                UserList[i].BannedList[0]=friendID;
                                UserList[i].BanCount++;
                                printf("\n%s blocked %s (B):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }
                            else if(UserList[i].ID==friendID) /*if user tries to block itself*/
                            {
                                printf("\nYou cannot block yourself (B).\n");
                            }
                            else if(friendID==666)/*if users tries to block unknown user*/
                            {
                                printf("\n%s is not our member (B).\n",piecesOfCommandLine[1]);
                            }
                            else if(isAlreadyBanned==1) /*if user tries to block a user again*/
                            {
                                printf("\n%s has already blocked %s.You cannot block this person again (B).\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }

                        }
                        else if(UserList[i].BanCount!=0) /*if blocked users count is not 0 then programme will expand the memory block and same process will be fallowed*/
                        {
                            UserList[i].BannedList=realloc(UserList[i].BannedList,(UserList[i].BanCount+1)*sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[1])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyBanned=0;
                            int y;
                            for(y=0;y<UserList[i].BanCount;y++)
                            {
                                if(UserList[i].BannedList[y]==friendID)
                                {
                                    isAlreadyBanned=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyBanned!=1)
                            {
                                UserList[i].BannedList[UserList[i].BanCount]=friendID;
                                UserList[i].BanCount++;
                                printf("\n%s blocked %s (B):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }
                            else if(UserList[i].ID==friendID)
                            {
                                printf("\nYou cannot block yourself (B).\n");
                            }
                            else if(friendID==666)
                            {
                                printf("\n%s is not our member (B).\n",piecesOfCommandLine[1]);
                            }
                            else if(isAlreadyBanned==1)
                            {
                               printf("\n%s has already blocked %s.You cannot block this person again (B).\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                            }

                        }
                    }
                    else if(strcmp(UserList[i].Name,piecesOfCommandLine[1])==0) /*If A blockes B automattically B must be block A*/
                    {
                        if(UserList[i].BanCount==0)
                        {
                            UserList[i].BannedList=(int*)malloc(sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[0])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyBanned=0;
                            int y;
                            for(y=0;y<UserList[i].BanCount;y++)
                            {
                                if(UserList[i].BannedList[y]==friendID)
                                {
                                    isAlreadyBanned=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyBanned!=1)
                            {
                                UserList[i].BannedList[0]=friendID;
                                UserList[i].BanCount++;
                            }

                        }
                        else if(UserList[i].BanCount!=0)
                        {
                            UserList[i].BannedList=realloc(UserList[i].BannedList,(UserList[i].BanCount+1)*sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[0])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyBanned=0;
                            int y;
                            for(y=0;y<UserList[i].BanCount;y++)
                            {
                                if(UserList[i].BannedList[y]==friendID)
                                {
                                    isAlreadyBanned=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyBanned!=1)
                            {
                                UserList[i].BannedList[UserList[i].BanCount]=friendID;
                                UserList[i].BanCount++;
                            }
                         }
                    }
                }

                /*REMOVE FRIENDS FROM BOTH USERS SECTION IN BLOCKING*/
                for(i=0;i<UserCounter;i++) /*Blocked ones must not stay in friend list of both users*/
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0)
                    {
                        int isMyFriend=0;
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[1])==0)
                            {
                                int a;
                                for(a=0;a<UserList[i].FriendCount;a++)
                                {
                                    if(UserList[i].FriendsList[a]==UserList[j].ID)
                                    {
                                        isMyFriend++;
                                        int replacer;
                                        for(replacer=a;replacer<UserList[i].FriendCount;replacer++)
                                        {
                                            UserList[i].FriendsList[a]=0;
                                        }
                                    }
                                }
                            }
                        }

                    }
                    else if(strcmp(UserList[i].Name,piecesOfCommandLine[1])==0)
                    {
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[0])==0)
                            {
                                int a;
                                for(a=0;a<UserList[i].FriendCount;a++)
                                {
                                    if(UserList[i].FriendsList[a]==UserList[j].ID)
                                    {
                                        int replacer;
                                        for(replacer=a;replacer<UserList[i].FriendCount;replacer++)
                                        {
                                            UserList[i].FriendsList[a]=0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else if(commandLine[0]=='U') /*UNBLOCK SECTION*/
            {
                /*For this section programme finds the users and controls if they blocked each others*/
                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*finds first user*/
                    {
                        int isBanned=0; /*for checking unblock if they blocked each other that integer will be changed*/
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[1])==0) /*find second user*/
                            {
                                int a;
                                for(a=0;a<UserList[i].BanCount;a++)
                                {
                                    if(UserList[i].BannedList[a]==UserList[j].ID) /*if they blocked each other*/
                                    {
                                        isBanned++;/*everything is okey*/
                                        int replacer;
                                        for(replacer=a;replacer<UserList[i].BanCount;replacer++) /*unblock*/
                                        {
                                            UserList[i].BannedList[a]=0;
                                        }
                                    }
                                }
                            }
                        }
                        if(isBanned!=0)
                        {
                            printf("\n%s unblocked %s (U).\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                        }
                        else if(isBanned==0) /*if user tries to unblock a user again*/
                        {
                            printf("\n%s has already unblocked %s. You cannot unblock this person again (U). \n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                        }

                    }
                    else if(strcmp(UserList[i].Name,piecesOfCommandLine[1])==0) /*If A unblocks B autoamatically B must unblock A */
                    {
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[0])==0)
                            {
                                int a;
                                for(a=0;a<UserList[i].BanCount;a++)
                                {
                                    if(UserList[i].BannedList[a]==UserList[j].ID)
                                    {
                                        int replacer;
                                        for(replacer=a;replacer<UserList[i].BanCount;replacer++)
                                        {
                                            UserList[i].BannedList[a]=0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                /*ADDING EACH ONE TO EACH ONE's FRIEND LIST */
                /*Same process with "ADDING FRIEND SECTION" */
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0)
                    {
                        if(UserList[i].FriendCount==0)
                        {
                            UserList[i].FriendsList=(int*)malloc(sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[1])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[0]=friendID;
                                UserList[i].FriendCount++;
                            }
                        }
                        else if(UserList[i].FriendCount!=0)
                        {
                            UserList[i].FriendsList=realloc(UserList[i].FriendsList,(UserList[i].FriendCount+1)*sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[1])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[UserList[i].FriendCount]=friendID;
                                UserList[i].FriendCount++;
                            }
                        }
                    }
                    else if(strcmp(UserList[i].Name,piecesOfCommandLine[1])==0)
                    {
                        if(UserList[i].FriendCount==0)
                        {
                            UserList[i].FriendsList=(int*)malloc(sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[0])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[0]=friendID;
                                UserList[i].FriendCount++;
                            }

                        }
                        else if(UserList[i].FriendCount!=0)
                        {
                            UserList[i].FriendsList=realloc(UserList[i].FriendsList,(UserList[i].FriendCount+1)*sizeof(int));
                            int friendID=666;
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {
                                if(strcmp(UserList[x].Name,piecesOfCommandLine[0])==0)
                                {
                                    friendID=UserList[x].ID;
                                }
                            }

                            int isAlreadyMyFriend=0;
                            int y;
                            for(y=0;y<UserList[i].FriendCount;y++)
                            {
                                if(UserList[i].FriendsList[y]==friendID)
                                {
                                    isAlreadyMyFriend=1;
                                }
                            }

                            if(UserList[i].ID!=friendID && friendID!=666 &&isAlreadyMyFriend!=1)
                            {
                                UserList[i].FriendsList[UserList[i].FriendCount]=friendID;
                                UserList[i].FriendCount++;
                            }
                         }
                    }
                }
            }
            else if(commandLine[0]=='S') /*LIST OF THE FRIENDS WENT SAME SCHOOL*/
            {
                /*Search the users friends if anyone's school is same with user programme will print him if not programme will print out a warning message*/
               int i;
               for(i=0;i<UserCounter;i++)
               {
                   if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*find the user*/
                   {
                        int printer=1; /*for the numbers*/
                        int isThereCommonUniFriend=0; /*for checking the common university friends if there is anyone thath integer will be change*/
                        int y;
                        for(y=0;y<UserList[i].FriendCount;y++) /*search users friends*/
                        {
                            int z;
                            for(z=0;z<UserCounter;z++)
                            {
                                if(UserList[z].ID==UserList[i].FriendsList[y])
                                {
                                    if(strcmp(UserList[i].University,UserList[z].University)==0) /*if anyone went same university with user*/
                                    {
                                        if(printer==1)
                                        {
                                            printf("\n%s went to same university '%s' \nwith these people(S):\n",UserList[i].Name,UserList[i].University);
                                        }
                                        printf("%d. %s\n",printer,UserList[z].Name);
                                        printer++;
                                        isThereCommonUniFriend++;
                                    }
                                }
                            }
                        }
                        if(isThereCommonUniFriend==0) /*if there is not common university friend*/
                        {
                            printf("\n%s has no common university friends(S):\n",UserList[i].Name);
                        }
                   }
               }



            }
            else if(commandLine[0]=='A') /*LISTING ALL FRIENDS SECTION*/
            {
                printf("\n%s's friends(A):\n",piecesOfCommandLine[0]);

                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*finds the user*/
                    {
                        int printer=1;
                        int a;
                        for(a=0;a<UserList[i].FriendCount;a++) /*finds the another user, do this for all users*/
                        {
                            int x;
                            for(x=0;x<UserCounter;x++)
                            {

                                if(UserList[x].ID==UserList[i].FriendsList[a]&&UserList[i].FriendsList[a]!=0) /*if they were friends print*/
                                {

                                    printf("%d. %s\n",printer,UserList[x].Name);
                                    printer++;
                                }
                            }
                        }

                    }
                }
            }
            else if(commandLine[0]=='C') /*LISTING THE COMMON FRIENDS SECTION*/
            {
                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*finds the first user*/
                    {
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[1])==0) /*finds the second user*/
                            {
                                /*Search their friendlists and if programme finds common friends it will print it out*/
                                int isThereCommonFriend=0; /*if there is a common friend that wilchange*/
                                int printer =1;
                                int a;
                                for(a=0;a<UserList[i].FriendCount;a++)
                                {
                                    int b;
                                    for(b=0;b<UserList[j].FriendCount;b++)
                                    {
                                        if(UserList[i].FriendsList[a]==UserList[j].FriendsList[b] &&UserList[j].FriendsList[b]!=0 && UserList[i].FriendsList[a]!=0)
                                        {
                                            isThereCommonFriend++;
                                            int x;
                                            for(x=0;x<UserCounter;x++)
                                            {

                                                if(UserList[x].ID==UserList[i].FriendsList[a]&&UserList[i].FriendsList[a]!=0)
                                                {
                                                        if(printer==1)
                                                        {
                                                            printf("\n%s and %s have these common friends(C):\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                                                        }
                                                        printf("%d. %s\n",printer,UserList[x].Name);
                                                        printer++;
                                                }
                                            }
                                        }

                                    }
                                }
                                if(isThereCommonFriend==0) /*if there is no common friends*/
                                {
                                    printf("%s and %s have no common friends\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                                }

                            }
                        }

                    }
                }
            }
            else if(commandLine[0]=='M') /*SENDING MESSAGE SECTION*/
            {
                int i;
                for(i=0;i<UserCounter;i++)
                {
                    if(strcmp(UserList[i].Name,piecesOfCommandLine[0])==0) /*find the first user*/
                    {
                        int j;
                        for(j=0;j<UserCounter;j++)
                        {
                            if(strcmp(UserList[j].Name,piecesOfCommandLine[1])==0) /*find the second user*/
                            {
                                int isUserBanned=0; /*checking if they blocked each others*/
                                int a;
                                for(a=0;a<UserList[i].FriendCount;a++)
                                {
                                    if(UserList[i].BannedList[a]==UserList[j].ID) /*if they blocked each others*/
                                    {
                                        isUserBanned=1;
                                    }
                                }

                                if(isUserBanned==0) /*if they didnt blocked each others*/
                                {
                                    printf("\n%s send a message to %s (M).\n",piecesOfCommandLine[0],piecesOfCommandLine[1]);
                                }
                                else if(isUserBanned==1) /*if they blocked each others*/
                                {
                                    printf("\nMessage cannot be sent.%s has already blocked %s (M).\n",piecesOfCommandLine[1],piecesOfCommandLine[0]);
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                printf("Invalid input!!\n");
            }
        }
        else
        {
          break;
        }
    }

    return 0;
}
