#ifndef REFRESH_TOKEN_H
#define REFRESH_TOKEN_H

// Khai báo hàm refresh_token_api
struct MemoryStruct {
    char *memory;
    size_t size;
};
size_t WriteCallbackRefreshtoken(void *ptr, size_t size, size_t nmemb, struct MemoryStruct *mem);
void RefreshTokenApi(char **access_token, char **refresh_token_str);


#endif // REFRESH_TOKEN_H
