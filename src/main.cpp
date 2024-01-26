// Copyright (c) 2020-2022 Cesanta Software Limited
// All rights reserved
#include <Arduino.h>
#include <WiFi.h>
#include "mongoose.h"
extern "C" {
#include "net.h"
}

static struct mg_mgr mgr;

extern unsigned long get_rand_32(void);
// fixup for lwIP compilation.
extern "C" unsigned long __lwip_rand(void) { return get_rand_32(); }

void log_func_mongoose_to_arduino(char c, void * ctx) {
    (void) ctx;
    Serial.print(c);
}

void setup() {
    Serial.begin(9600);
    // Block until serial monitor is connected. For debugging.
    while(!Serial) {}
    Serial.println("Firmware start! Connecting to: " + String(WIFI_SSID));
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.printf("\nConnected to WiFi\n\nConnect to server at http://%s\n", WiFi.localIP().toString().c_str());
    mg_log_set_fn(&log_func_mongoose_to_arduino, NULL);
    mg_mgr_init(&mgr);
    web_init(&mgr);
}

void loop() {
    mg_mgr_poll(&mgr, 10);
}