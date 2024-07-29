#include "burro_server.h"

DNSServer dnsServer;
AsyncWebServer server(80);

void updateDNS() {
    dnsServer.processNextRequest();
}

void notFound(AsyncWebServerRequest *request) {
    wink();
    request->send(404, "text/plain", "Not found");
}


String readHTMLContent(const char* html) {
    String content;
    while (pgm_read_byte(html) != 0) {
        content += (char)pgm_read_byte(html++);
    }
    return content;
}

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request){
        if (
            request->url() == "/index" ||
            request->url() == "/index.html" ||
            request->url() == "/home" ||
            request->url() == "/color" ||
            request->url() == "/pattern" ||
            request->url() == "/rgb" ||
            request->url() == "/color" ||
            request->url() == "/flag" ||
            request->url() == "/reset" ||
            request->url() == "/en-us/search/search/search/admin/manger/console/admin.php"        
        ) {
            return false;
        }
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
        wink();
        
        String htmlContent = readHTMLContent(index_head);
        htmlContent += readHTMLContent(index_colors);
        htmlContent += readHTMLContent(index_patterns);
        if (flags.flagSGS || flags.flagKyle || flags.flagUnlockIt) {
            htmlContent += "<li><a href='#' onclick='sendPatternRequest(10); return false;'>Random Colors</a></li>";
        }
        if (flags.flagUser || flags.flagKyle || flags.flagUnlockIt) {
            htmlContent += "<li><a href='#' onclick='sendPatternRequest(9); return false;'>Rainbow</a></li>";
        }
        if (flags.flagPass || flags.flagKyle || flags.flagUnlockIt) {
            htmlContent += "<li><a href='#' onclick='sendPatternRequest(11); return false;'>Color Wheel</a></li>";
        }
        if (flags.flagLogin || flags.flagKyle || flags.flagUnlockIt) {
            htmlContent += "<li><a href='#' onclick='sendPatternRequest(12); return false;'>More Colors</a></li>";
        }
        htmlContent += "</ul><br>";
        htmlContent += readHTMLContent(index_flag_form);
        htmlContent += "<p>Flags Captured:";
        int points = 0;
        if (flags.flagSGS) {
            htmlContent += " SGS";
            points += 1;
        }
        if (flags.flagKyle) {
            htmlContent += " blametheotherkyle";
            points += 9;
        }
        if (flags.flagUser) {
            htmlContent += " the_username_is_burroadmin";
            points += 2;
        }
        if (flags.flagPass) {
            htmlContent += " sUPErZ3cR3t1337PA5SW0Rd!";
            points += 3;
        }
        if (flags.flagLogin) {
            htmlContent += " Ic@nL0g1n!";
            points += 4;
        }
        if (flags.flagUnlockIt) {
            htmlContent += " unlock_it";
        }
        htmlContent += "</p><p>Total Points: " + String(points) + "</p>";
        htmlContent += readHTMLContent(index_challenges);
        htmlContent += readHTMLContent(index_end); 
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print(htmlContent);
        request->send(response);
    }

};

void initServer() {

    dnsServer.start(53, "*", WiFi.softAPIP());

    server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        request->send(200, "text/plain", "leave now");
    });

    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        resetPattern();
        request->send(200, "text/plain", "lgtm");
    });

    server.on("/pattern", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        if (request->hasParam("id")) {
            int id = request->getParam("id")->value().toInt();
            setPattern(id);
            request->send(200, "text/plain", "lgtm");
        } else {
            request->send(400, "text/plain", "wat?");
        }
    });

    server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        if (request->hasParam("id")) {
            int id = request->getParam("id")->value().toInt();
            setColor(id);
            request->send(200, "text/plain", "lgtm");
            saveColor(id);
        } else {
            request->send(400, "text/plain", "wat?");
        }
    });

    server.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        if (request->hasParam("r") && request->hasParam("g") && request->hasParam("b")) {
            int r = request->getParam("r")->value().toInt();
            int g = request->getParam("g")->value().toInt();
            int b = request->getParam("b")->value().toInt();
            setRGB(r, g, b);
            request->send(200, "text/plain", "lgtm");
        } else {
            request->send(400, "text/plain", "wat?");
        }
    });

    server.on("/flag", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        if (request->hasParam("flag")) {
            String flag = request->getParam("flag")->value();
            setFlag(flag);
            unlockLEDPattern(flag);
            setPoints();
            request->send(200, "text/plain", "lgtm");
        } else {
            request->send(400, "text/plain", "wat?");
        }
    });

    server.on("/en-us/search/search/search/admin/manger/console/admin.php", HTTP_GET, [](AsyncWebServerRequest *request){
        wink();
        if (request->hasParam("username") && request->hasParam("password")) {
            String username = request->getParam("username")->value();
            String password = request->getParam("password")->value();
            if ((username.indexOf("burroadmin") != -1 || username == "burroadmin" || username == "the_username_is_burroadmin") 
                    && (password.indexOf("1337") != -1 || password == "sUPErZ3cR3t1337PA5SW0Rd!" || password == "sUPErZ3cR3t1337PA5SW0Rd%21")
                ) {
                    request->send(200, "text/plain", "flag{Ic@nL0g1n!}");
                    return;
            }
        }
        String htmlContent = readHTMLContent(login_form);
        AsyncResponseStream *response = request->beginResponseStream("text/html");
        response->print(htmlContent);
        request->send(response);
    });
    
    server.onNotFound(notFound);

    server.begin();
}