#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char *process_pwd(char *argv[]);
int encrypt(int argc, char *argv[]);
int decrypt(int argc, char *argv[]);
char pswd[50];

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "usage: ./cryptogram <encrypt|decrypt> <password> <text>\n");
        return 1;
    }
    if (strcmp(argv[1], "encrypt") == 0) {
        encrypt(argc, argv);
    } else if (strcmp(argv[1], "decrypt") == 0) {
        decrypt(argc, argv);
    } else {
        fprintf(stderr, "method given is neither encrypt nor decrypt\n");
        return 1;
    }
}

char *process_pwd(char *argv[]) {
    char *password = argv[2];
    //char *pwd = (char*)calloc(50, sizeof(char));
    char *pwd = pswd;
    int pwd_len = 0;
    int i = 0;
    while (password[i] != '\0') {
        if (password[i] < 65 || (password[i] > 90 && password[i] < 97) || password[i] > 122) {
            i++;
            continue;
        }
        *pwd = password[i];
        if (*pwd > 64 && *pwd < 91) {
            *pwd = *pwd + 32;
        }
        pwd++;
        pwd_len++;
        i++;
    }
    if (pwd_len == 0) {
        *pwd = 'a';
        pwd_len++;
        pwd++;
    }
    *pwd = '\0';
    pwd = pwd - pwd_len;
    for (int j = 0; j < pwd_len; j++) {
        password[j] = pwd[j];
    }
    //free(pwd);
    return pwd;
}

int encrypt(int argc, char *argv[]) {
    if (argc == 3) {
        printf("%s\n", argv[2]);
        return 0;
    }
    char *passwrd = process_pwd(argv);
    int pwd_len = strlen(passwrd);
    //code for text processing starts
    char *text = argv[3];
    int ti = 0; //TextIndex
    int pi = 0; //PasswordIndex
    while (text[ti] != '\0') {
        if (text[ti] < 65 || (text[ti] > 90 && text[ti] < 97) || text[ti] > 122) {
        } else {
            if (text[ti] < 91) {
                text[ti] = text[ti] + (passwrd[pi] - 'a');
                if (text[ti] > 90) {
                    text[ti] = 64 + (text[ti] - 90);
                }
                pi++;
            } else {
                text[ti] = text[ti] + (passwrd[pi] - 'a');
                if (text[ti] > 122) {
                    text[ti] = 96 + (text[ti] - 122);
                } else if (text[ti] < 0) {
                    text[ti] = text[ti] + 134 + 96;
                }
                pi++;
            }
        }
        ti++;
        if (pi > pwd_len - 1) {
            pi = 0;
        }
    }
    printf("%s\n", text);
    return 0;
}

int decrypt(int argc, char *argv[]) {
    if (argc == 3) {
        printf("%s\n", argv[2]);
        return 0;
    }
    char *password = process_pwd(argv);
    int pwd_len = strlen(password);
    //code for text processing starts
    char *text = argv[3];
    int ti = 0; //TextIndex
    int pi = 0; //PasswordIndex
    while (text[ti] != '\0') {
        if (text[ti] < 65 || (text[ti] > 90 && text[ti] < 97) || text[ti] > 122) {
        } else {
            if (text[ti] < 91) {
                text[ti] = text[ti] - (password[pi] - 'a');
                if (text[ti] < 65) {
                    text[ti] = 91 - (65 - text[ti]);
                }
                pi++;
            } else {
                text[ti] = text[ti] - (password[pi] - 'a');
                if (text[ti] < 97) {
                    text[ti] = 123 - (97 - text[ti]);
                } else if (text[ti] < 0) {
                    text[ti] = text[ti] + 134 + 96;
                }
                pi++;
            }
        }
        ti++;
        if (pi > pwd_len - 1) {
            pi = 0;
        }
    }
    printf("%s\n", text);
    return 0;
}
