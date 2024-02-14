#include "lib.h"

int main() {

    // Create Socket

    int servsock = socket(AF_INET, SOCK_STREAM, 0);
    if (servsock == INVALID_SOCKET) {
        cout << "Error : cannot create Socket" << endl;
        return 0;
    }

    int flags = fcntl(servsock, F_GETFL, 0);
    fcntl(servsock, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(12345);

    if (bind(servsock, (sockaddr*)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        cout << "bind() error" << endl;
        return 0;
    }

    if (listen(servsock, SOMAXCONN) == SOCKET_ERROR) {
        cout << "listen() error" << endl;
        return 0;
    }

    while (true) {
        sockaddr_in cliaddr;
        uint addrlen = sizeof(cliaddr);
        int clisock = accept(servsock, (sockaddr*)&cliaddr, &addrlen);
        if (clisock == INVALID_SOCKET) {
            // 블로킹 일때는 바로 문제가 되지만,
            // cout << "accept() error" << endl;
            // return 0;

            // 논블로킹 일때는 한번 더 확인해야 한다
            if (errno == EWOULDBLOCK) {
                continue;
            }
            else {
                cout << "accept() error: " << endl;
                return 0;
            }
        }

        cout << "Client Connected" << endl;

        char buf[1024] = "";
        char fileBuf[1024] = "";
        char scriptBuf[1024] = "";

        while (true) {
            int recvlen;
            // 논블로킹 소켓은 send()도 루프를 돌면서 될 때까지 계속 시도해야함
            while (true) {
                recvlen = recv(clisock, buf, sizeof(buf), 0);
                if (recvlen == SOCKET_ERROR) {
                    // 논블로킹 소켓은 recv()에서 한번 더 체크해줘야함
                    if (errno == EINPROGRESS || errno == EWOULDBLOCK) {
                        continue;
                    }
                    else {
                        cout << "recv() error" << endl;
                        return 0;
                    }
                }
                else {
                    break;
                }
            }

            char resBuf[2048];
            //resBuf[0] = '\0';

            if (strstr(buf, "GET / HTTP/1.1") != NULL) {
                FILE* file = fopen("html/index.html", "r");

                if(file != nullptr) {
                    fread(fileBuf, sizeof(char), sizeof(fileBuf), file);
                    fclose(file);

                    sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s",fileBuf);
                }
            } else if (strstr(buf, "GET /js/index.js HTTP/1.1") != NULL) {
                FILE* file = fopen("js/index.js", "r");

                if(file != nullptr) {
                    fread(scriptBuf, sizeof(char), sizeof(scriptBuf), file);
                    fclose(file);

                    sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n%s",scriptBuf);
                }
            } else if (strstr(buf, "GET /login HTTP/1.1") != NULL) {
                FILE* file = fopen("html/login.html", "r");

                if(file != nullptr) {
                    fread(fileBuf, sizeof(char), sizeof(fileBuf), file);
                    fclose(file);

                    sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s",fileBuf);
                }
            } else if (strstr(buf, "GET /js/login.js HTTP/1.1") != NULL) {
                FILE* file = fopen("js/login.js", "r");

                if(file != nullptr) {
                    fread(scriptBuf, sizeof(char), sizeof(scriptBuf), file);
                    fclose(file);

                    sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n%s",scriptBuf);
                }
            } else if (strstr(buf, "GET /register HTTP/1.1") != NULL) {
                FILE* file = fopen("html/register.html", "r");

                if(file != nullptr) {
                    fread(fileBuf, sizeof(char), sizeof(fileBuf), file);
                    fclose(file);
                    
                    char* cookieHeader = strstr(buf,"Cookie: ");
                    if(cookieHeader != NULL) {
                        char* cookieStart = strstr(cookieHeader, "userName");
                        if(cookieStart != NULL) {
                            sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<script>alert('Already Login');window.location.href = '/';</script>");
                        } else {
                            sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s",fileBuf);
                        }
                    } else {
                        sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s",fileBuf);
                    }
                }
            } else if (strstr(buf, "GET /js/register.js HTTP/1.1") != NULL) {
                FILE* file = fopen("js/register.js", "r");

                if(file != nullptr) {
                    fread(scriptBuf, sizeof(char), sizeof(scriptBuf), file);
                    fclose(file);

                    sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n\r\n%s",scriptBuf);
                }
            } else if (strstr(buf, "GET /list HTTP/1.1") != NULL) {
                FILE* file = fopen("html/list.html", "r");

                if(file != nullptr) {
                    fread(fileBuf, sizeof(char), sizeof(fileBuf), file);
                    fclose(file);

                    char* cookieHeader = strstr(buf,"Cookie: ");
                    if(cookieHeader != NULL) {
                        char* cookieStart = strstr(cookieHeader, "userName");
                        if(cookieStart != NULL) {
                            sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s",fileBuf);
                        } else {
                            sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<script>alert('You must need Login');window.location.href = '/';</script>");
                        }
                    } else {
                        sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<script>alert('You must need Login');window.location.href = '/';</script>");
                    }
                }
            } else if (strstr(buf, "GET /admin HTTP/1.1") != NULL) {
                FILE* file = fopen("html/admin.html", "r");

                if(file != nullptr) {
                    fread(fileBuf, sizeof(char), sizeof(fileBuf), file);
                    fclose(file);

                    char* cookieHeader = strstr(buf,"Cookie: ");
                    if(cookieHeader != NULL) {
                        char* cookieStart = strstr(cookieHeader, "userName");
                        if(cookieStart != NULL) {
                            cookieStart += strlen("userName=");
                            size_t findAdmin = strcspn(cookieStart, " \n");
                            string adminStr = string(cookieStart, findAdmin);
                            adminStr.erase(0,adminStr.find_first_not_of(" \n"));
                            adminStr.erase(adminStr.find_last_not_of(" \n"));
                            if(adminStr == "admin") {
                                sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n%s",fileBuf);
                            } else {
                                sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<script>alert('You must need admin account');window.location.href = '/';</script>");
                            }
                        } else {
                            sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<script>alert('You must need admin account');window.location.href = '/';</script>");
                        }
                    } else {
                        sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<script>alert('You must need admin account');window.location.href = '/';</script>");
                    }
                }
            } else if (strstr(buf, "GET /logout HTTP/1.1") != NULL) {
                sprintf(resBuf, "HTTP/1.1 200 OK\r\nSet-Cookie: userName=; Path=/; Expires=Thu, 01 Jan 1970 00:00:00 GMT\r\nContent-Type: text/html\r\n\r\n<script>window.location.href='/';</script>");
            } else if (strstr(buf, "POST / HTTP/1.1") != NULL) {
                char userName[1024];

                const char* jsonStart = strstr(buf, "{");
                if(jsonStart != NULL) {
                    const char* jsonEnd = strstr(buf, "}");
                    if(jsonEnd != NULL) {
                        int jsonLength = jsonEnd - jsonStart + 1;
                        char jsonData[1024];
                        strncpy(jsonData, jsonStart, jsonLength);
                        jsonData[jsonLength] = '\0';

                        if(sscanf(jsonData, "{\"userName\":\"%[^\"]\"}", userName) == 1) {
                            char cookieBuf[1024];
                            sprintf(cookieBuf, "Set-Cookie: userName=%s; Path=/\r\n", userName);

                            sprintf(resBuf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n%s\r\n", cookieBuf);
                        }
                    }
                }
            }

            buf[recvlen] = '\0';

            cout << "Recv: " << buf << endl;

            int sendlen;
            // 논블로킹 소켓은 send()도 루프를 돌면서 될 때까지 계속 시도해야함
            while (true) {
                sendlen = send(clisock, resBuf, strlen(resBuf), 0);
                if (sendlen == SOCKET_ERROR) {
                    // 논블로킹 소켓은 send()에서 한번 더 체크해줘야함
                    if (errno == EINPROGRESS || errno == EWOULDBLOCK) {
                        continue;
                    }
                    else {
                        cout << "send() error" << endl;
                        return 0;
                    }
                }
                else {
                    break;
                }
            }

            close(clisock); // 소켓을 닫고
            cout << "Client Disconnected" << endl;
            break; // 루프를 빠져나가서 다음 클라이언트를 받는다
        }
    }

    // 소켓 종료
    close(servsock);
    return 0;
}