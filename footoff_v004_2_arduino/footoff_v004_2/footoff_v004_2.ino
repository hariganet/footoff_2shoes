
// http://hello-world.blog.so-net.ne.jp/2012-11-04
// とりあえず、SDカードのWAVファイルを再生してみる。
// 16MHzのArduino で 256 clock毎に割り込むので 62.5kHz(8bit Monoral)が理想だが
// ボクの持っているソフトが対応していないので、32kHz(8bit Stereo)で代用
#include <SD.h>
#include <SPI.h>

#define SE_01 "01.wav"
#define SE_02 "02.wav"
#define SE_03 "03.wav"
#define SE_04 "04.wav"

#define BUF_SIZE 384          // バッファ・サイズ

boolean soundPlayFlag;        // SE再生のフラグ
int playSoundNum = 1;             // 現在設定されているSEナンバ
String inputString = "";           //シリアル通信された文字列

volatile uint8_t              // グローバル変数
    buf[2][BUF_SIZE],           // バッファ
    buf_page,                   // バッファ・ページ
    buf_flg;                    // バッファ読み込みフラグ
volatile uint16_t buf_index;    // バッファ位置
volatile uint16_t read_size[2]; // バッファ読み込みサイズ

int swCount = 0;
boolean swState;

#define CSWITCH 2

char Msg[100];

boolean FootCenterSwitchFlag = true;
unsigned long FootCenterOnTheAirTime;
unsigned long FootCenterOnTheGroundTime;
boolean FootCenterCheckTimeFlag;

const int FootOnTheAirVal = 300;
int MyStatus = 0;

void setup() {
  pinMode(10, OUTPUT);                             // SDライブラリ使用時のお約束
  //while( !SD.begin(10) );                          // ライブラリとSDカードを初期化
  SD.begin(10);
  // PWM初期化
  DDRD  |=  B00001000;                             // PD3 (OC2B) (Arduino D3)
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);  // 8bit高速PWM
  TCCR2B = _BV(CS20);                              // 分周なし
  
  // シリアルポート初期化
  Serial.begin(9600);
  Serial.flush();
  
  //pinset
  soundPlayFlag = false;
  pinMode(2,INPUT);
  swState = false;
  playSoundNum = 1;
}

void loop() {  
  
  Walk();
  
  // 再生する曲の設定
  checkSoundNum();
  //Serial.println("set number:");
  // サウンド再生の設定
  if(soundPlayFlag == true){
    soundlay();
  }

  // スイッチの判定
  if(digitalRead(CSWITCH) == HIGH){
    swCount++;
  }else{
    swCount = 0;
  }
  if(swCount == 100){
    swState = true;
  }
  if(swState == true){
    Serial.println("SWITCH ON!!");
    soundPlayFlag = true;
    swCount = 0;
    swState = false;
  }
}

void Walk(){
  /* 足が浮いた瞬間から時間計測 */
  if(digitalRead(CSWITCH) == LOW && FootCenterSwitchFlag == true){
     //Serial.println("CSWITCH AIR!!!");
     FootCenterOnTheAirTime = millis();
     FootCenterOnTheGroundTime = FootCenterOnTheAirTime;
     FootCenterSwitchFlag = false;
  }
  
  /* 足がついた瞬間に時間計測 */  
  if(digitalRead(CSWITCH) == HIGH && FootCenterSwitchFlag == false){
     //Serial.println("CSWITCH GROUND!!!");
     FootCenterOnTheGroundTime = millis();
     FootCenterCheckTimeFlag = true;
  }
  
  /* 滞空時間が指定通りならば歩いたとみなしてメッセージを送る */
  if((FootCenterOnTheGroundTime - FootCenterOnTheAirTime) >= FootOnTheAirVal && FootCenterCheckTimeFlag == true){
    sprintf(Msg, "C%d", MyStatus);
    Serial.println(Msg);
    FootCenterOnTheAirTime = 0;
    FootCenterOnTheGroundTime = FootCenterOnTheAirTime;
    FootCenterCheckTimeFlag = false;
    FootCenterSwitchFlag = true;
  }
}


// 再生する曲の設定
void checkSoundNum(){
  
  //if(Serial.available() > 0){
    while(Serial.available()){
      char inChar = (char)Serial.read();
      inputString += inChar;
      //Serial.println("get char");
      //Serial.println(inputString);
    }
    
//    if(inputString.length() > 0){
//      char cVal = inputString[2];
//      playSoundNum = atoi(&cVal);
//      //Serial.print("Sound Set:");
//      Serial.println(inputString); 
//    }     
  //}
  //Serial.println(inputString);
  
  if(inputString.length() >= 4){
    char cVal = inputString[2];
    //playSoundNum = atoi(&cVal);
    noInterrupts();
    playSoundNum = atoi(&cVal);
    inputString = "";
//    if(cVal == '1'){
//      playSoundNum = 1;
//      Serial.println(cVal);
//      inputString = "";
//    }else if(cVal == '2'){
//      playSoundNum = 2;
//      Serial.println(cVal);
//      inputString = "";
//    } 
    interrupts();
  } 
}

// サウンド再生
void soundlay(){
  File dataFile;
  
  swCount = 0;
  swState = false;
  
  //Serial.println("set number:");
  //Serial.println(playSoundNum);
//  if ( !(dataFile = SD.open( "01.wav" )) )  return;   // error opening wavefile
  noInterrupts();
  if (playSoundNum==1){
    dataFile = SD.open( SE_01 );
    //playSoundNum++;
  }else if(playSoundNum==2){
    dataFile = SD.open( SE_02 );
    //playSoundNum++;
  }else if(playSoundNum==3){
    dataFile = SD.open( SE_03 );
    //playSoundNum = 1;
  }//else{
//    dataFile = SD.open( SE_04 );
//    playSoundNum = 1;
//  }
  interrupts();
  //boolean openFlag = true;
//  switch (playSoundNum) {
//    case 1:
//      if ( !(dataFile = SD.open( SE_01 )) )  openFlag = false;
//      break;
//    case 2:
//      if ( !(dataFile = SD.open( SE_02 )) )  openFlag = false;
//      break;
//    case 3:
//      if ( !(dataFile = SD.open( SE_03 )) )  openFlag = false;
//      break;
//    case 4:
//      if ( !(dataFile = SD.open( SE_04 )) )  openFlag = false;
//      break;
//    default:
//      if ( !(dataFile = SD.open( SE_01 )) )  openFlag = false;
//      break;
//  }
//  if (openFlag == false){
//    return;
//  }

  //if ( !(dataFile = SD.open( SE_01 )) )  return;
  
  //debug
  //Serial.print("set number");
  //Serial.println(playSoundNum);
  //Serial.println("file open!!");
  
  buf_index = 44;  buf_page = 0;  buf_flg = 1;    // パラメータ設定 (44byteはヘッダ)  
  read_size[buf_page] = dataFile.read( (uint8_t*)buf[buf_page], BUF_SIZE );
  TIMSK2 |= _BV(TOIE2);
  while(TIMSK2 & _BV(TOIE2)) {
    if(buf_flg) {     // データ読み込み指令のフラグが立ったら読み込む
      // debug
      //Serial.println("data read!");
      read_size[buf_page ^ 1] = dataFile.read( (uint8_t*)buf[buf_page ^ 1], BUF_SIZE );
      buf_flg = 0;    // 読み込んだらフラグを下ろす
      if(digitalRead(CSWITCH) == HIGH){
        swCount++;
      }else{
        swCount = 0;
      }
      if(swCount == 20){
        swState = true;
      }
      if(swState == true){
        Serial.println("stop playing!!");
        //stopSoundPlay();
        buf_index = 44;  buf_page = 0;  buf_flg = 1;    // パラメータ設定 (44byteはヘッダ)
        //TIMSK2 |= _BV(TOIE2);
        TIMSK2 &= ~_BV(TOIE2);
        OCR2B = 0;
        dataFile.close();
        soundPlayFlag = false;
        swState = false;
      }
    }
  }
  
  //debug
  Serial.println("file close!!");
  
  OCR2B = 0;
  dataFile.close();
  
  soundPlayFlag = false;
}

ISR(TIMER2_OVF_vect) {
  
  OCR2B = buf[buf_page][buf_index++];                  // データをPWMとして出力
  if(buf_index == read_size[buf_page]) {               // 現在のバッファの最後まで来たら...
    if(buf_index != BUF_SIZE)  TIMSK2 &= ~_BV(TOIE2);    // ファイルの最後なら,TOIE2をクリア
    buf_index = 0;  buf_page ^= 1;  buf_flg = 1;         // バッファを切り替え
  }
}

void stopSoundPlay(){
//  buf_index = 44;  buf_page = 0;  buf_flg = 1;    // パラメータ設定 (44byteはヘッダ)
//   TIMSK2 |= _BV(TOIE2);
//   OCR2B = 0;
//  dataFile.close();
//  soundPlayFlag = false;
}
