#include "Adafruit_NeoPixel.h"

#define LED_COUNT 10    //  количество лампочек

#define LED_PIN 2       //куда лампочки впаяны
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);    // 

int pinDatS = A3;                                                // Объявляем пин для работы с датчиком звука.
int pinPot = A5;                                                 // Объявляем пин для работы с потенциометром.                                                                 //
float mass[44];                                                  // Массив для хранения полученных данных с датчика звука.
const float average = 2;                                         // Переменная усреднения показаний датчика (чем выше значение, тем выше инерционность выводимых показаний).
int potValue;                                                    // Переменная для считывания показания потенциометра.
float readDatS;                                                  // переменная  со значения датчика звука
int yellowSpace = 5;                                             // переменная  yellowSpace со значением жёлтого диапазона
int minimumSoundValue = 5;                                       // минимум с которого зажигаються лампочки
int maximumSoundValue = 45;                                      // максимум с которого зажигаеться всё
int diapozoni[4];                                                // массив с границами зелёного красного  и желгого 


void setup()                                                      
{                                                                
  Serial.begin(9600);                                            //запуск печати в консоль
  strip.begin();                                                 //запуск ленты      
}                                                                

void loop()                                                      //начало бесконечного цикла
{                                                              
  int ledsToFire;                                                //переменная сколько лампочек включить
  potValue = map(analogRead(pinPot),0,1024, minimumSoundValue, maximumSoundValue); //считывание значения потенциометра в переменную potValue в диапазоне от минимального звука до максимального
  diapozoni[0]=minimumSoundValue;                                //добавляем в массив diapozoni минимум
  diapozoni[1]=potValue-yellowSpace;                             //добавляем в массив diapozoni границу между жёлтым и зелёным
  diapozoni[2]=potValue+yellowSpace;                             //добавляем в массив diapozoni границу между жёлтым и красным 
  diapozoni[3]=maximumSoundValue;                                //добавляем в массив diapozoni  максимум
  
  readDatS =  map(analogRead(pinDatS), 0, 1024, 0, 64);          //считывание значения громкосьти с микрофона
  Serial.print("potValue=");
  Serial.print(potValue);
  Serial.print("\t");
  Serial.print("SoundValue=");
  Serial.print(readDatS);
  Serial.print("\t");
 
  int Prev = mass[0];                                            // Дополнительная переменная для переноса массива, присваиваем ей значение первого элемента массива.
  int Prev2;                                                     // Вторая дополнительные переменные для переноса массива.
  // Формула аппроксимации.                                      //
  mass[0] *= average-1;                                          // Умножаем предыдущее значение датчика звука на коэффициент усреднения-1.
  mass[0] += readDatS;                                           // Добавляем к полученному значению новые показания датчика звука.
  mass[0] /= average;                                            // Делим полученное значение на коэффициент усреднения.
                                                                 //
  for (int i = 1; i <= 43; i++)                                  // Переносим каждый элемент массива по очереди с помощью цикла.
  {                                                              //
    Prev2 = mass[i];                                             // Присваиваем второй дополнительное переменной новый элемент массива.
    mass[i] = Prev;                                              // Присваиваем новому элементу массива предыдущий элемента массива.
    Prev = Prev2;                                                // Присваиваем дополнительным переменным вторую дополнительную переменную.
  }                                                              //
  Serial.print("SoundAvarage=");
  Serial.print( mass[0]);
  Serial.print("\t"); 
  ledsToFire = howManyLedsToFire(diapozoni,mass[0]);            //вычисляем сколько зажечь лампочек при помощи функции
  fireLeds(ledsToFire);                                         // зажигаем лампочки
  Serial.print("LEDS=");
  Serial.print(ledsToFire);
  Serial.print("\t"); 
  Serial.println();                     
}              

//Функции
int howManyLedsToFire(int diapozoni[4], int soundValue) {                           //обьявляем функцию howManyLedsTofire
  if (soundValue<diapozoni[0]) {return 0;}                                          //если уровень звука меньше минимума передаём 0
  if (soundValue>diapozoni[3]) {return 10;}                                         //если уровень звука больше максимума передаём 10
  if ((soundValue>diapozoni[0]) and (soundValue<diapozoni[1])) {                    //если уровень звука больше минимума и меньше нижней границы жёлтого то 
    return map(soundValue,diapozoni[0],diapozoni[1], 1, 3);                         // возвращаем колиество лампочек
  }
  if ((soundValue>diapozoni[1]) and (soundValue<diapozoni[2])) {                    
    return map(soundValue,diapozoni[1],diapozoni[2], 1, 3)+3;                      // возвращаем количество лампочек + зелёные лампочки
  }
    if ((soundValue>diapozoni[2]) and (soundValue<diapozoni[3])) {                 // возвращаем количество лампочек + зелёные и жёлтые лампочки
    return map(soundValue,diapozoni[2],diapozoni[3], 1, 4)+6;
  }
}

void fireLeds(int ledsTofire) {
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 0));
    strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(5, strip.Color(0, 0, 0));
    strip.setPixelColor(6, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.setPixelColor(8, strip.Color(0, 0, 0));
    strip.setPixelColor(9, strip.Color(0, 0, 0));
  
  
  if ((ledsTofire>0) and (ledsTofire<9)) {                         // Функция которая включает лампочки
    strip.setPixelColor(0, strip.Color(0, 50, 0));
  }
  if ((ledsTofire>1) and (ledsTofire<9)) {
    strip.setPixelColor(1, strip.Color(0, 100, 0));
  }
  if ((ledsTofire>2) and (ledsTofire<9)) {
    strip.setPixelColor(2, strip.Color(0, 150, 0));
  }
  if ((ledsTofire>3) and (ledsTofire<9)) {
    strip.setPixelColor(3, strip.Color(50, 50, 0));
  }
  if ((ledsTofire>4) and (ledsTofire<9)) {
    strip.setPixelColor(4, strip.Color(100, 100, 0));
  }
  if ((ledsTofire>5) and (ledsTofire<9)) {
    strip.setPixelColor(5, strip.Color(200, 200, 0));
  }
  if ((ledsTofire>6) and (ledsTofire<9)) {
    strip.setPixelColor(6, strip.Color(50, 0, 0));
  }
  if ((ledsTofire>7) and (ledsTofire<9)) {
    strip.setPixelColor(7, strip.Color(100, 0, 0));
  }
  if (ledsTofire>8) {
    strip.setPixelColor(0, strip.Color(150, 0, 0));
    strip.setPixelColor(1, strip.Color(150, 0, 0));
    strip.setPixelColor(2, strip.Color(150, 0, 0));
    strip.setPixelColor(3, strip.Color(150, 0, 0));
    strip.setPixelColor(4, strip.Color(150, 0, 0));
    strip.setPixelColor(5, strip.Color(150, 0, 0));
    strip.setPixelColor(6, strip.Color(150, 0, 0));
    strip.setPixelColor(7, strip.Color(150, 0, 0));
    strip.setPixelColor(8, strip.Color(150, 0, 0));
  }
  if (ledsTofire>9) {
    strip.setPixelColor(0, strip.Color(200, 0, 0));
    strip.setPixelColor(1, strip.Color(200, 0, 0));
    strip.setPixelColor(2, strip.Color(200, 0, 0));
    strip.setPixelColor(3, strip.Color(200, 0, 0));
    strip.setPixelColor(4, strip.Color(200, 0, 0));
    strip.setPixelColor(5, strip.Color(200, 0, 0));
    strip.setPixelColor(6, strip.Color(200, 0, 0));
    strip.setPixelColor(7, strip.Color(200, 0, 0));
    strip.setPixelColor(8, strip.Color(200, 0, 0));
    strip.setPixelColor(9, strip.Color(200, 0, 0));
  }
  strip.show();
  delay(100);
}
// конец функций
