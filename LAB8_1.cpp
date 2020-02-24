#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define NUM_P 6
#define NUM_LOGI 4

typedef struct{
    int page_No;
    char Offset[100];
}Logical;

typedef struct{
    int page_No;
    char Offset[100];
}Process_page;

char str[]="";
int memoryPage = 8;
int systemBus  = 24;
Process_page PageTable[NUM_P+1] = {{},
                                 {1,"0000010100101001"},
                                 {2,"0000010100100011"},
                                 {3,"0000000100100100"},
                                 {4,"0000110101101011"},
                                 {5,"0000011101101100"},
                                 {6,"0000111101100111"}};
Logical logi[NUM_LOGI+1] = {{},
                    {1, "25"},
                    {2, "43"},
                    {3, "7"},
                    {5, "50"}};


void binTOhex(char *str, char bin[]){
    int dec = 0;
    char numchar;

    for (int i = 0; i < (strlen(bin)) ; i+=4) {         // แปลงฐาน 2 เป็น ฐาน 10
        for (int j = 0; j <=3 ; j++) {
            dec += (bin[i+j]-'0') * (int)pow(2,3-j);   // ตามสูตรแปลงเลขฐาน 2 เป็น 10
        }
        switch(dec)                                    // แปลงฐาน 10 เป็น ฐาน 16
        {
            case 15 : strcat(str,"F"); break;
            case 14 : strcat(str,"E"); break;
            case 13 : strcat(str,"D"); break;
            case 12 : strcat(str,"C"); break;
            case 11 : strcat(str,"B"); break;
            case 10 : strcat(str,"A"); break;
            default : numchar = dec+'0'; strncat(str, &numchar,1);
        }
        dec = 0;
    }
}

void hexTobin(char*str, char hex[]){
    for (int i = 0; i < strlen(hex); ++i) {           //แปลงฐาน 16 เป็น ฐาน 2 โดยเทียบจากตาราง
        switch(hex[i])
        {
            case '0' : strcat(str,"0000"); break;
            case '1' : strcat(str,"0001"); break;
            case '2' : strcat(str,"0010"); break;
            case '3' : strcat(str,"0011"); break;
            case '4' : strcat(str,"0100"); break;
            case '5' : strcat(str,"0101"); break;
            case '6' : strcat(str,"0110"); break;
            case '7' : strcat(str,"0111"); break;
            case '8' : strcat(str,"1000"); break;
            case '9' : strcat(str,"1001"); break;
            case 'A' : strcat(str,"1010"); break;
            case 'B' : strcat(str,"1011"); break;
            case 'C' : strcat(str,"1100"); break;
            case 'D' : strcat(str,"1101"); break;
            case 'E' : strcat(str,"1110"); break;
            case 'F' : strcat(str,"1111"); break;

        }
    }
}

char* Convert(char *str, char number[], char base[]){  //function แปลงเลขฐาน
    strcpy(str, "");
    if(strcmp(base,"2")==0){              //แปลงเป็นเลขฐาน 2
        hexTobin(str, number);
    } else if(strcmp(base,"16")==0){     //แปลงเป็นเลขฐาน 16
        binTOhex(str,number);
    }else{
        return "error!!!";              // base ที่ส่งมาไม่ใช่ "2" or "16" -> error
    }
    return str;                         // return เลขฐานเมื่อแปลงเล๘ฐานเสร็จ
}

void show(){
    printf("Logical Address to Physical Address Method Paging\n");
    printf("--------------------------------------------------------------\n");
    printf("Logical Adress\t\t");
    printf("Process page table\n");
    for (int i = 1; i <NUM_P+1; ++i) {
        if(logi[i].page_No != 0)
            printf("%02d %3sH\t\t\t", logi[i].page_No, logi[i].Offset);
        else
            printf("\t\t\t");
        printf("page: %d  %sB", PageTable[i].page_No, PageTable[i].Offset);
        printf("\n");
    }
    printf("--------------------------------------------------------------\n");
    printf("Memory per Page(256 Byte) = (%d^%d) = %2d bit n bitoffset\n",2,memoryPage,memoryPage);
    printf("CPU Adress bus                    = %d bit m bit all\n", systemBus);
    printf("bit for frame             = (%d-%d)= %2d bit m-n bit page num\n", systemBus, memoryPage,(systemBus - memoryPage));
    printf("--------------------------------------------------------------\n");
}
void Physical(){
    for (int i = 1; i < NUM_LOGI+1 ; ++i) {               //LOOP logical
        for (int j = 1; j < NUM_P+1 ; ++j) {              //LOOP PAGE TABLE
            if(logi[i].page_No == PageTable[j].page_No){  // ถ้า page NO. (logical) เท่ากับ page NO.(page table)
                printf("Logical\t%sH\t\t%024s B\n",logi[i].Offset,Convert(str,logi[i].Offset,"2"));
                printf("\t%02d\t=\tPage NO.%d\n", logi[i].page_No, logi[i].page_No);
                printf("\t%2s\t=\tPage off.%s\n", logi[i].Offset, logi[i].Offset);
                printf("--------------------------------------------------------------\n");
                printf("Physical\t\tf = %s B\n", PageTable[j].Offset);
                printf("\t\t\t%7sFrame NO. | Page Off.\n","");
                printf("Phycical Adress\t\t%s | %s B\n",PageTable[j].Offset,Convert(str,logi[i].Offset,"2"));
                printf("\t\t\t%11s %s | %s H\n","",Convert(str, PageTable[j].Offset, "16"),logi[i].Offset);
                printf("--------------------------------------------------------------\n");
                break;
            }
        }
    }
}

int main(){
    show();
    Physical();
    return 0;
}
