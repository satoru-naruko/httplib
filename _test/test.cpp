
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

#include <gtest/gtest.h>

class HttpLibTest : public ::testing::Test {
protected:

    virtual void SetUp() {
        m_num = 2;
    }
    int m_num;

    int calc(int num){ return num * m_num;}
};

TEST_F(HttpLibTest, Function1Test) {
    auto result = calc(5);
    ASSERT_EQ(10, result);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret_gtest = RUN_ALL_TESTS();

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
