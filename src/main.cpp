// Standard libraries
#include <M5Core2.h>
#include <WiFi.h>
#include "time.h"

// WiFi credentials
#include "secrets.h"

// UI libraries
#include "lv_setup.h"
#include "ui/ui.h"
#include "ui/ui_helpers.h"

// ================ WiFi and Time Configuration ================
const char* ntpServer = "time1.aliyun.com"; // NTP server for time synchronization
const long gmtOffset_sec = 7 * 3600;        // Bangkok time zone (GMT+7)
const int daylightOffset_sec = 0;           // No daylight saving time in Bangkok

// ================ Display Positions ================
#define TIME_XPOS 160  // Center of screen
#define TIME_YPOS 85   // Y position for main time
#define DATE_YPOS 140  // Y position for date
#define SEC_XPOS 260   // Right side for seconds
#define SEC_YPOS 95    // Aligned with main time
#define AMPM_YPOS 95   // Y position for AM/PM indicator

// ================ Global Variables ================
// RTC variables
RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;

// Display strings
char timeStrbuff[64];
char dateStrbuff[64];

// Text conversion arrays
const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* monthNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// Button press tracking
bool syncInProgress = false;
const unsigned long SYNC_PRESS_DURATION = 5000; // 5 seconds in milliseconds

// ================ Function Prototypes ================
void flushTime();
void setRTCfromNTP();
void connectWiFiAndSyncNTP();
int calculateDayOfWeek(int year, int month, int day);
void checkButtonForSync();
void showSyncMessage(bool starting);

// ================ Helper Functions ================
/**
 * Calculate day of week using Zeller's Congruence algorithm
 * Returns: 0=Sunday, 1=Monday, ..., 6=Saturday
 */
int calculateDayOfWeek(int year, int month, int day) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int dayOfWeek = (day + 13*(month+1)/5 + k + k/4 + j/4 + 5*j) % 7;
    // Convert from Zeller's result (0=Saturday) to standard (0=Sunday)
    return (dayOfWeek + 6) % 7;
}

/**
 * Updates the UI with current time and date from RTC
 */
void flushTime() {
    // Get current time and date from RTC
    M5.Rtc.GetTime(&RTCtime);
    M5.Rtc.GetDate(&RTCDate);
  
    // Calculate day of week using proper algorithm
    int dayOfWeek = calculateDayOfWeek(RTCDate.Year, RTCDate.Month, RTCDate.Date);

    // Convert to 12-hour format for display
    int hour12 = RTCtime.Hours % 12;
    if (hour12 == 0) hour12 = 12;
    const char* ampm = RTCtime.Hours >= 12 ? "PM" : "AM";
  
    // Format time and date strings
    char mainTime[16], seconds[8];
    sprintf(mainTime, "%02d:%02d", hour12, RTCtime.Minutes);
    sprintf(seconds, "%02d", RTCtime.Seconds);
    sprintf(dateStrbuff, "%s, %s %02d", 
            dayNames[dayOfWeek], 
            monthNames[RTCDate.Month - 1], 
            RTCDate.Date);

    // Update UI elements
    lv_label_set_text(ui_TimeLabel, mainTime);
    lv_label_set_text(ui_SecLabel, seconds);
    lv_label_set_text(ui_DateLabel, dateStrbuff);
    lv_label_set_text(ui_amPMLabel, ampm);
}

/**
 * Shows sync status message on screen
 */
void showSyncMessage(bool starting) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.setTextSize(2);
    
    if (starting) {
        M5.Lcd.println("Syncing time with NTP...");
        M5.Lcd.println("Please wait...");
    } else {
        M5.Lcd.println("Time sync complete!");
        M5.Lcd.println("Returning to clock...");
        delay(2000);
        M5.Lcd.fillScreen(BLACK);
    }
}

/**
 * Check if button B is being held for time sync
 * Uses the built-in pressedFor() method from M5Stack library
 */
void checkButtonForSync() {
    // Check if button B has been pressed for the required duration and sync not in progress
    if (M5.BtnB.pressedFor(SYNC_PRESS_DURATION) && !syncInProgress) {
        // Mark sync as in progress to prevent multiple triggers
        syncInProgress = true;
        
        // Show status and perform sync
        showSyncMessage(true);
        connectWiFiAndSyncNTP();
        showSyncMessage(false);
        
        // Reset sync flag
        syncInProgress = false;
        
        // Wait for button release to prevent multiple triggers
        while (M5.BtnB.isPressed()) {
            M5.update();
            delay(10);
        }
    }
}

/**
 * Synchronizes the RTC with NTP time
 */
void setRTCfromNTP() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time from NTP");
        M5.Lcd.println("Failed to obtain time from NTP");
        return;
    }

    RTCtime.Hours = timeinfo.tm_hour;
    RTCtime.Minutes = timeinfo.tm_min;
    RTCtime.Seconds = timeinfo.tm_sec;
    if (!M5.Rtc.SetTime(&RTCtime)) {
        Serial.println("Failed to set time in RTC!");
        M5.Lcd.println("Failed to set time in RTC!");
    }

    RTCDate.Year = timeinfo.tm_year + 1900; // tm_year is years since 1900
    RTCDate.Month = timeinfo.tm_mon + 1;   // tm_mon is 0-11, RTC wants 1-12
    RTCDate.Date = timeinfo.tm_mday;
    if (!M5.Rtc.SetDate(&RTCDate)) {
        Serial.println("Failed to set date in RTC!");
        M5.Lcd.println("Failed to set date in RTC!");
    }

    Serial.println("RTC set from NTP time.");
    M5.Lcd.println("RTC set from NTP time.");
}

/**
 * Connects to WiFi and synchronizes time with NTP server
 */
void connectWiFiAndSyncNTP() {
    M5.Lcd.printf("\nConnecting to %s", ssid);
    WiFi.begin(ssid, password);
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED) {
        retryCount++;
        if (retryCount > 10) {
            M5.Lcd.println("\nFailed to connect to WiFi");
            delay(1000);
            M5.Lcd.clear();
            return;
        }
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.println("\nCONNECTED!");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    setRTCfromNTP(); // Set the RTC after NTP sync
    delay(1000);
    M5.Lcd.clear();

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

/**
 * Setup function - initializes hardware and connections
 */
void setup() {
    M5.begin();  // Init M5Core2.  
    lv_begin();  
    ui_init();
    Serial.begin(115200); 
    delay(1000);
  
    // Set text datum to middle center
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.fillScreen(BLACK);  // Initial clear only
    
    // Display initial instructions
    M5.Lcd.setCursor(0, 100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Hold middle button (B)");
    M5.Lcd.println("for 5 seconds to sync time");
    delay(3000);
    M5.Lcd.fillScreen(BLACK);
}

/**
 * Main loop function
 */
void loop() {
    M5.update();  // Update button state
    
    // Check for button press to sync time
    checkButtonForSync();
    
    lv_handler(); 
    flushTime();
    delay(50);  // Reduced delay for more responsive button detection
}