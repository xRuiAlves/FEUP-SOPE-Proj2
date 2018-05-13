#include "parser.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

u_int parse_unsigned_int(char* str) {
    errno = 0;
    char* endptr = NULL;
    long val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
        (val < 0) ||
        (errno != 0 && val == 0) ||
        (str == endptr)) {

        return UINT_MAX;
    }
    else {
        return (u_int) val;
    }
}

int parse_int(char* str) {
    errno = 0;
    char* endptr = NULL;
    long val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
        (errno != 0 && val == 0) ||
        (str == endptr)) {

        return UINT_MAX;
    }
    else {
        return (int) val;
    }
}

int split_string(const char * src_str, const char * tokens, char *** result_list, size_t * len) {
    if(src_str == NULL || tokens == NULL || result_list == NULL || len == NULL) {
        return -1;
    }

    char * saveptr = NULL;
    char ** _list = NULL;
    char ** temp;
    *result_list = NULL;
    *len  = 0;
    int error_status = 0;

    char * copy = strdup(src_str);
    if(copy == NULL) {
        return -2;
    }

    char * str = strtok_r(copy, tokens, &saveptr);
    if(str == NULL) {
        error_status = -3;
        goto free_and_exit;
    }

    _list = malloc(sizeof *_list);
    if(_list == NULL) {
        error_status = -4;
        goto free_and_exit;
    }

    _list[*len] = strdup(str);
    if(_list[*len] == NULL) {
        error_status = -5;
        //Note that the list itself is still allocated but that should be dealt with by the caller
        goto free_and_exit;
    }
    (*len)++;


    while((str = strtok_r(NULL, tokens, &saveptr))) {
        temp = realloc(_list, (sizeof *_list) * (*len + 1));
        if(temp == NULL) {
            //List reallocation unsuccessful
            error_status = -6;
            goto free_and_exit;
        }

        //Reallocation successful
        _list = temp;

        _list[*len] = strdup(str);
        if(_list[*len] == NULL) {
            //Duplication of string into list unsuccessful
            error_status = -7;
            goto free_and_exit;
        }
        (*len)++;
    }


    free_and_exit:
    *result_list = _list;
    free(copy);

    return error_status;
}
