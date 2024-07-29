#ifndef BURRO_SERVER_H
#define BURRO_SERVER_H

#include "burro_mesh.h"
#include "burro_mem.h"
#include "burro_leds.h"
#include "burro_html.h"

#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

extern AsyncWebServer server;
extern DNSServer dnsServer;

void notFound(AsyncWebServerRequest *request);
void initServer();
void updateDNS();

#endif // BURRO_SERVER_H
