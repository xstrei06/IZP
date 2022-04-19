
//Author: Jaroslav Streit, xstrei06

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int rule_1(char[]);
int rule_2(char[], long);
int rule_3(char[], long);
int rule_4(char[], long);
void stats(int, double, double, bool[], int);
void nchars(char[], bool[]);
void min_len(char[], int*);
void avg(char[], double*, double*);
int stats_compare(char* []);

//function returning error message after invalid (unexpected) input
int errors(int argc, long *LEVEL, long *PARAM, char *argv[]){
    char *g; //variable used to store remainder of string in the argument LEVEL after strtol
    char *h; //variable used to store remainder of string in the argument PARAM after strtol
    if(argc < 3) {
        fprintf(stderr,"Error: Invalid input! Insufficient number of arguments.\n");
        return 1;
    }
    *LEVEL = strtol((const char*) argv[1], &g, 10);
    *PARAM = strtol((const char*) argv[2], &h, 10);
    if(argc > 4) {
        fprintf(stderr, "Error: Invalid input! Too many arguments.\n");
        return 1;
    }
    if(*LEVEL > 4){
        fprintf(stderr,"Error: Invalid input! Argument LEVEL is too high.\n");
        return 1;
    }
    if(*LEVEL < 1) {
        fprintf(stderr, "Error: Invalid input! Argument LEVEL is too low.\n");
        return 1;
    }
    if(*PARAM < 1){
        fprintf(stderr,"Error: Invalid input! Argument PARAM is too low.\n");
        return 1;
    }
    if(argc == 4 && stats_compare(argv) == 0){
        fprintf(stderr, "Error: Invalid input! Wrong format of third argument. Try --stats.\n");
        return 1;
    }
    if(*g != '\0'){
        fprintf(stderr, "Error: Invalid input! Argument LEVEL contains characters other than numbers.\n");
        return 1;
    }
    if(*h != '\0'){
        fprintf(stderr, "Error: Invalid input! Argument PARAM contains characters other than numbers.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    long LEVEL = 0;
    long PARAM = 0;

    int err = errors(argc, &LEVEL, &PARAM, argv);
    if(err){
        return err;
    }

    bool ascii[127] = {0};   //boolean array needed for statistics
    int chars = 0;           //variable needed for statistics
    double pwd_count = 0;    //variable needed for statistics
    double chars_count = 0;  //variable needed for statistics
    int min = 100;           //variable needed for statistics

    char c[102];
    while(fgets(c, sizeof(c), stdin) != NULL) { //reading passwords from file
        //error for too long password
        for(int i = 0; c[i] != '\n'; i++){
            if(i > 100) {
                fprintf(stderr, "Error: Invalid input! File contains password(s) longer than 100 characters.\n");
                return 1;
            }
        }
        if (LEVEL == 1) {
            int a = rule_1(c);
            if (a == 1) {
                printf("%s", c);
            }
        }
        if (LEVEL == 2) {
            int b = rule_2(c, PARAM);
            if (b == 1) {
                int a = rule_1(c);
                if (a == 1) {
                    printf("%s", c);
                }
            }
        }
        if (LEVEL == 3) {
            int d = rule_3(c, PARAM);
            if (d == 1) {
                int b = rule_2(c, PARAM);
                if (b == 1) {
                    int a = rule_1(c);
                    if (a == 1) {
                        printf("%s", c);
                    }
                }
            }
        }
        if (LEVEL == 4) {
            int e = rule_4(c, PARAM);
            if(e == 1){
                int d = rule_3(c, PARAM);
                if (d == 1) {
                    int b = rule_2(c, PARAM);
                    if (b == 1) {
                        int a = rule_1(c);
                        if (a == 1) {
                            printf("%s", c);
                        }
                    }
                }
            }
        }
        if (argc == 4 && stats_compare(argv) == 1) { //checks for argument [--stats]
            nchars(c, ascii);
            avg(c, &pwd_count, &chars_count);
            min_len(c, &min);
        }
    }
    if (argc == 4 && stats_compare(argv) == 1) { //checks for argument [--stats]
        stats(chars, pwd_count, chars_count, ascii, min);
    }
    return 0;
}

//function controlling whether password passes through first rule
int rule_1(char c[]){
    int uppercase = 0;
    int lowercase = 0;
    for(int i = 0; c[i] != '\0'; i++){
        if(c[i] >='a' && c[i] <= 'z'){ //checks for lowercase letters
            lowercase++;
        }
        else if(c[i] >='A' && c[i] <='Z'){ //checks for uppercase letters
            uppercase++;
        }
    }
    if(lowercase >= 1 && uppercase >= 1){
        return 1;
    }
    return 0;
}

//function controlling whether password passes through second rule
int rule_2(char c[], long PARAM){
    int lowercase = 0;
    int uppercase = 0;
    int num = 0;
    int spec_sym = 0;
    for(int i =0; c[i] != '\0'; i++){
        if(c[i] >= 'a' && c[i] <= 'z'){
            lowercase++;
        }
        else if(c[i] >= 'A' && c[i] <= 'Z'){
            uppercase++;
        }
        else if(c[i] >= '0' && c[i] <= '9'){
            num++;
        }
        else if(c[i] >= 32 && c[i] <= 47){
            spec_sym++;
        }
        else if(c[i] >= 58 && c[i] <= 64){
            spec_sym++;
        }
        else if(c[i] >= 91 && c[i] <= 96){
            spec_sym++;
        }
        else if(c[i] >= 123 && c[i] <= 126){
            spec_sym++;
        }
    }
    //Passwords without lowercase and uppercase letter can never pass rule 1, hence not taken in account.
    if(lowercase >= 1 && uppercase >= 1 && num >= 1 && spec_sym >= 1 && PARAM >= 4){
        return 1;
    }
    else if(lowercase >= 1 && uppercase >= 1 && num >= 1 && PARAM == 3){
        return 1;
    }
    else if(lowercase >= 1 && uppercase >= 1 && spec_sym >= 1 && PARAM == 3){
        return 1;
    }
    else if(lowercase >= 1 && uppercase >= 1 && PARAM == 2){
        return 1;
    }
    return 0;
}

//function controlling whether password passes through third rule
int rule_3(char c[], long PARAM) {
    int len = 1;
    int max = 0;
    for(int i = 0; c[i] != '\0'; i++) {
        if (c[i] == c[i + 1]) {
            len++;
        } else if (c[i] != c[i + 1]) {
            len = 1;
        }
        if (len > max) {
            max = len;
        }
    }
    if(max >= PARAM){
        return 0;
    }
    return 1;
}

//function controlling whether password passes through fourth rule
int rule_4(char c[], long PARAM) {
    int len = 0;
    for(int i = 0; c[i] != '\0'; i++) {
        len++;
    }
    if(len - 1 <= PARAM){
        return 1;
    }
    int count;
    for(int i = 0; c[i+PARAM] != '\0'; i++) {
        for(int j = 1; c[i+j+PARAM] != '\0'; j++) {
            count = 0;
            for(int k = 0; k < PARAM; k++) {
                if(c[i+k] == c[i+j+k]) {
                    count++;
                }
                if(count == PARAM) {
                    return 0;
                }
            }
        }
    }
    return 1;
}
void stats(int chars, double pwd_count, double chars_count, bool ascii[], int min){ //function printing statistics
    //loop using boolean array from function nchars to count unique ascii characters present across all passwords on input
    for(int i = 0; i < 126; i++) {
        if(ascii[i] == 1) {
            chars+= 1;
        }
    }
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", chars);
    printf("Minimalni delka: %d\n", min);
    //average length of password calculated by dividing variables chars_count and pwd_count from function avg
    printf("Prumerna delka: %0.1f\n", chars_count/pwd_count);
}

//statistic function checking presence of each ascii character and writing it into boolean array
void nchars(char c[], bool ascii[]) {
    for(int i = 0; c[i] != '\n'; i++) {
        int m = (int) c[i];
        ascii[m] = true;
    }
}

//statistic function checking for shortest password from all passwords on input
void min_len(char c[], int* min){
    int i = 0;
    for(;c[i] != '\n'; i++){}
    if(i < *min){
        *min = i;
    }
}

//statistic function counting total number of characters and passwords on input
void avg(char c[], double* pwd_count, double* chars_count) {
    int i = 0;
    for(; c[i] != '\n'; i++){
        *chars_count+= 1;
    }
    if(c[i] == '\n'){
        *pwd_count+= 1;
    }
}

//function checking whether argument --stats matches user input
int stats_compare(char* argv[]) {
    char stats[8] = "--stats";
    for(int i = 0; argv[3][i] == stats[i]; i++) {
        if(i == 7) {
            return 1;
        }
    }
    return 0;
}