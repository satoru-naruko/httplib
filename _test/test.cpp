
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <signal.h>
#include <netinet/in.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <SslContext.h>
#include <HttpsClient.h>

int main(void) {

    SslContext globalContext;
    auto ret = globalContext.Init();
    if (ret < 0){
        printf ("Init error¥n");
        return EXIT_FAILURE;
    }

    std::string host = "localhost";
    std::string path = "/";
    HttpsClient client(host, &globalContext, 8080);

    std::function<int()> resultCallback = [](){ return 1; };
    client.Get(path, resultCallback);

    globalContext.Term();

    return EXIT_SUCCESS;
}
