/*********************************************************************/
/*  Wio-Terminal greenhouse monitor – v3.3                           */
/*  2025-04-27 – RTC epoch, stats position, tunable layout,           */
/*              2 s screen refresh, fixed SCD4x communication        */
/*********************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>          // Seeed_Arduino_LCD
#include <RTC_SAMD51.h>        // on-chip RTC
#include <SparkFun_SCD4x_Arduino_Library.h>
#include <Seeed_FS.h>
#include <SD/Seeed_SD.h>
#include "config.h"

TFT_eSPI   tft;
RTC_SAMD51 rtc;
/* tell the driver we have an SCD41 (works fine for the Qwiic SCD40 too) */
SCD4x      scd40(SCD4x_SENSOR_SCD41);
File       logFile;
uint32_t   nextLog  = 0, nextDraw = 0;
uint32_t   scdDeadline = 0;                      // when to give up waiting

/* ---------- simple rolling statistics -------------------------- */
struct RunStats {
  static const uint16_t SZ = 120;
  float buf[SZ]; uint16_t idx = 0, cnt = 0;
  void  push(float v){ buf[idx]=v; idx=(idx+1)%SZ; if(cnt<SZ)cnt++; }
  float mean() const { float s=0; for(uint16_t i=0;i<cnt;i++) s+=buf[i]; return s/cnt; }
  float sd()   const { float m=mean(),s=0; for(uint16_t i=0;i<cnt;i++){float d=buf[i]-m; s+=d*d;} return sqrtf(s/cnt); }
  float mn()   const { float m=buf[0]; for(uint16_t i=1;i<cnt;i++) if(buf[i]<m) m=buf[i]; return m; }
  float mx()   const { float m=buf[0]; for(uint16_t i=1;i<cnt;i++) if(buf[i]>m) m=buf[i]; return m; }
};
RunStats rCO2,rT,rH,rLux,rSoil;

/* ------------------------- SD helpers -------------------------- */
void initSD(){
  if(SD.begin(SDCARD_SS_PIN)){
    logFile = SD.open("/env_log.csv", FILE_WRITE);
    if(logFile && logFile.size()==0)
      logFile.println("Date,Time,CO2,Temp,Hum,Lux,Soil");
  }
}

/* -------------- drawing helpers (no layout logic) -------------- */
void printValue(float v,int16_t x,int16_t y,uint32_t col,uint8_t font){
  tft.setTextFont(font); tft.setTextColor(col,TFT_BLACK);
  char b[12]; dtostrf(v,0,1,b); tft.drawString(b,x,y);
}

void drawStatsColumn(const RunStats& r,uint16_t colX,uint16_t rowY){
  char b[12];
  tft.setTextFont(1); tft.setTextColor(TFT_CYAN,TFT_BLACK);

  dtostrf(r.mean(),0,1,b); tft.drawString(b,colX,rowY);
  dtostrf(r.sd()  ,0,1,b); tft.drawString(b,colX,rowY+12);
  dtostrf(r.mn()  ,0,1,b); tft.drawString(b,colX,rowY+24);
  dtostrf(r.mx()  ,0,1,b); tft.drawString(b,colX,rowY+36);
}

/* ----------- one-time static background (boxes, labels) -------- */
void drawStatic(){
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);

  /* title */
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextFont(4);
  tft.drawString("Greenhouse Monitor", 0, Y_TITLE);

  /* labels */
  tft.setTextFont(2);
  uint16_t y = Y_LABELS;
  tft.drawString("CO2 :", X_LABELS, y);
  tft.drawString("Soil:", X_SOIL_LABEL, y);
  y+=22;
  tft.drawString("Temp:", X_LABELS, y); y+=22;
  tft.drawString("Hum :", X_LABELS, y); y+=22;
  tft.drawString("Lux :", X_LABELS, y);

  /* statistics header */
  tft.setTextColor(TFT_CYAN,TFT_BLACK);
  tft.setTextFont(1);
  y = Y_STATS_BASE - 14;
  tft.drawString("  Mean   SD    Min   Max",0,y);

  /* extra info */
  y = Y_STATS_BASE + 52;
  tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  tft.setTextFont(1);
  tft.drawString("CO2 ok : 400-1000 ppm",0,y);
}

/* ------------ dynamic screen refresh (values + stats) ---------- */
void drawScreen(const DateTime& now,float co2,float T,float H,float lux,float soil){
  /* date & time – top-right */
  tft.setTextDatum(TR_DATUM); tft.setTextFont(2); tft.setTextColor(TFT_GREEN,TFT_BLACK);
  char buf[20];
  sprintf(buf,"%02u:%02u:%02u",now.hour(),now.minute(),now.second());
  tft.drawString(buf,tft.width()-2, Y_TITLE + 4);
  sprintf(buf,"%04u-%02u-%02u",now.year(),now.month(),now.day());
  tft.drawString(buf,tft.width()-2, Y_TITLE + 26);
  tft.setTextDatum(TL_DATUM);

  /* live readings */
  uint16_t y = Y_LABELS;
  uint32_t colCO2  = (co2<400||co2>1000)?TFT_RED:TFT_GREEN;
  uint32_t colSoil = (soil<300||soil>800)?TFT_RED:TFT_GREEN;
  printValue(co2 , X_VALS     , y, colCO2 ,2);
  printValue(soil, X_SOIL_VAL , y, colSoil,2);
  tft.setTextFont(1); tft.setTextColor(TFT_YELLOW,TFT_BLACK);
  tft.drawString("300-800", X_SOIL_VAL, y+14);

  y+=22;
  printValue(T   , X_VALS     , y, TFT_ORANGE,2); y+=22;
  printValue(H   , X_VALS     , y, TFT_CYAN,2);   y+=22;
  printValue(lux , X_VALS     , y, TFT_YELLOW,2);

  /* statistics */
  uint16_t baseY = Y_STATS_BASE;
  drawStatsColumn(rCO2 ,  50 ,baseY);
  drawStatsColumn(rT   , 110 ,baseY);
  drawStatsColumn(rH   , 170 ,baseY);
  drawStatsColumn(rLux , 230 ,baseY);
  drawStatsColumn(rSoil, 290 ,baseY);
}

/* ------------------------------ setup -------------------------- */
void setup(){
  pinMode(WIO_LIGHT,INPUT);
  pinMode(SOIL_PIN ,INPUT);

  Serial.begin(115200);
  tft.begin(); tft.setRotation(3);
  drawStatic();

  rtc.begin();
  rtc.adjust(DateTime(2025,4,27,11,55,0));   // first boot only – comment afterwards

  Wire.begin();
  if( !scd40.begin(false,true,false) ){       // (periodic OFF, autoCal ON, skipStop ON)
    Serial.println("SCD4x not detected – check wiring.");
    while(1);
  }
  scd40.startPeriodicMeasurement();           // 5 s internal cycle
  scdDeadline = millis() + SCD_TIMEOUT_MS;

  initSD();
}

/* ------------------------------ loop --------------------------- */
void loop(){
  static float co2=0,T=0,H=0;  // last valid reading

  /* non-blocking fetch of fresh SCD data */
  if(scd40.readMeasurement()){                 // true when new sample ready
    co2 = scd40.getCO2();
    T   = scd40.getTemperature();
    H   = scd40.getHumidity();
    scdDeadline = millis() + SCD_TIMEOUT_MS;   // reset watchdog
  }
  else if(millis() > scdDeadline){             // sensor silent → restart it
    scd40.stopPeriodicMeasurement(); delay(500);
    scd40.startPeriodicMeasurement();
    scdDeadline = millis() + SCD_TIMEOUT_MS;
  }

  /* other sensors */
  uint16_t rawLux  = analogRead(WIO_LIGHT);
  float    lux     = rawLux * 0.976f;          // crude scaling
  uint16_t rawSoil = analogRead(SOIL_PIN);

  /* feed statistics */
  rCO2.push(co2); rT.push(T); rH.push(H); rLux.push(lux); rSoil.push(rawSoil);

  /* screen refresh */
  if(millis() >= nextDraw){
    nextDraw += SCREEN_INT_MS;
    drawScreen(rtc.now(),co2,T,H,lux,rawSoil);
  }

  /* SD logging */
  if(millis() >= nextLog){
    nextLog += LOG_INTERVAL_MS;
    DateTime now = rtc.now();
    if(logFile){
      char dateBuf[11], timeBuf[9];
      sprintf(dateBuf,"%04u-%02u-%02u",now.year(),now.month(),now.day());
      sprintf(timeBuf,"%02u:%02u:%02u",now.hour(),now.minute(),now.second());
      logFile.printf("%s,%s,%.1f,%.1f,%.1f,%.0f,%u\r\n",
                     dateBuf,timeBuf,co2,T,H,lux,rawSoil);
      logFile.flush();
    }
  }
}