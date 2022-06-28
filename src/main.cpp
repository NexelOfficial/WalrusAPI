#include <drogon/drogon.h>

static const int port = 3000;

int main()
{
    // CORS Policy
    drogon::app().registerPostHandlingAdvice(
    [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
    });

    std::cout << "Walrus is now running on port " << port << std::endl;

    drogon::app().addListener("0.0.0.0", port)
    .setThreadNum(8)
    .run();

    return 0;
}
