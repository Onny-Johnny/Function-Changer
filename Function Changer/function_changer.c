#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <fcntl.h>
#include <errno.h>

int my_strcmpagain(const char *x, const char *y, int num) {
    while (*x == *y && *x++ && *y++ && num-- > 0)
    if (num == 0) 
        return 0;
    return 666;
}

char *mx_strnew(const int size) {
    if (size >= 0) {
        char *str = (char*) malloc(size + 1);
            for (int i = 0; i <= size; i++) 
                str[i] = '\0';
        return str;
    }
    return NULL;
}

int mx_get_substr_index(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) 
        return -2;
    
    int sizB = strlen(str);
    int sizS = strlen(sub);

    for(int i = 0; sizB-- > 0; i++)
        if(my_strcmpagain(str++, sub, sizS) == 0) 
            return i;
    return -1;
}

int mx_count_substr(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) 
        return -1;
    
    int sizB = strlen(str);
    int sizS = strlen(sub);
    int countret = 0;
  
    while (sizB-- > 0)
        if(my_strcmpagain(str++, sub, sizS) == 0) 
            countret++;
    return countret;
}

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if(str == NULL || sub == NULL || replace == NULL) 
        return NULL;
    int subnum = mx_count_substr(str, sub);;
    char *ret = NULL;
    int size = strlen(str);
    size = (size - subnum * strlen(sub)) + subnum * strlen(replace);

    if (size > 0) {
        ret = mx_strnew(size);
        for (int i = 0; i < subnum;  i++) {
            if (i == 0) {
                for (int j = 0, c = 0, r = 0; j < size; r++) {
                    if (j == mx_get_substr_index(str, sub) && replace[c] != '\0') {
                        ret[r] = replace[c];
                        c++;
                        if (replace[c] == '\0') {
                            j = mx_get_substr_index(str, sub) + strlen(sub);
                        }
                    } else {
                        ret[r] = str[j];
                        j++;
                    }
                }
            }else if (mx_count_substr(ret, sub) > 0)
                ret = mx_replace_substr(ret, sub, replace);
        }
    }
    if (mx_count_substr(str, sub) == 0 && strlen(str) > 0)
        return NULL;
    return ret;
}
bool space_c(char c) {
    if (c == ' ' || c == '\t' ||
        c == '\n' || c == '\f' ||
        c == '\v' || c == '\r')
    {
        return true;
    }
    return false;
}
int size_of_file(const char *file) {
    int i = 0;
    char c;
    int op = open(file, O_RDWR);
    
    if(op == -1) 
        return -1;
    for (; read(op, &c, 1); i++);
    close(op);
    return i;
}
bool space_file_c(const char *file) {
    char c;
    int op = open(file, O_RDWR);
    
    if(op == -1) 
        return -1;
    for (; read(op, &c, 1); ) {
        if(!(space_c(c)))
            break;
    }
    if(read(op, &c, 1)) {
        close(op);
        return false;
    }
    close(op);
    return true;
}
char *mx_file_to_str(const char *file) {
    int op = open(file, O_RDWR);
    char *str = NULL;
    char c;

    if (op == -1) 
        return NULL;
    str = mx_strnew(size_of_file(file));
    if (space_file_c(file) == false) {
        if (str) {
            for (int i = 0; read(op, &c, 1); i++)
                str[i] = c;
        }else {
            free(str);
            str = NULL;
        }
        close(op);
        return str;
    }
    close(op);
    return NULL;
}
int main(int argc, char *argv[]) {
	char *str = NULL;
	char *str2 = NULL;
    char first[20];
    char second[20];
	int fd = 0;
            printf("%s\n", "What argument do you want to change?");
        gets(first);
        printf("%s\n", "How do you call this argument?");
        gets(second);
    for (int i = 0; i < argc; i++) {
		printf("\n%s -- %d\n", argv[i], argc);
	    fd = open(argv[i], O_RDWR, O_TRUNC);
	if (!fd)
		perror("Error");
	   str = mx_file_to_str(argv[i]);
	   printf("\nstr1 = %s\n", str);
	   str2 = mx_replace_substr(str, first, second);
	   printf("\nstr2 = %s\n", str);
	if (str2 != NULL) {
		for (int i = 0; str2[i] != '\0'; i++) {
			write(fd, &str2[i], 1);
		}
	}
	if (str != NULL) {
		free(str);
		str = NULL;
    }
	if (str2 != NULL) {
		free(str2);
		str2 = NULL;
    }
	close(fd);
}
    return 0;
}
