#include <Servo.h>            //сервы
#include <Adafruit_GFX.h>     //экранчик
#include <Adafruit_PCD8544.h> //экранчик

//--------------------------------------------------------//
//----------------------- Таймер -------------------------//
//--------------------------------------------------------//
const int ticketACTION = 0; //шаг срабатывания события
int ticket = 0;             //переменная для вывода данных через заданное количество действий

//--------------------------------------------------------//
//---------------- Данные для обработки ------------------//
//--------------------------------------------------------//
int* data;                   //данные чтения
int posData = 0;              //позиция нынешней записи в массив data
long srd = 0;                 //значение среднего
int* drawGraphLCD;           //данные о последних средних
int posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD

//--------------------------------------------------------//
//---------------------- Константы -----------------------//
//--------------------------------------------------------//
#define debug false             //для дебага
#define workWithLCD false       //отрисовывать на экранчике NOKIA 5110
#define window LCDWIDTH//сейчас 128, но можно увеличить до 200;  //размер окна

//--------------------------------------------------------//
//------------------------ Пины --------------------------//
//--------------------------------------------------------//
#define A_PIN_READ 5              //аналоговый пин чтения данных с фильтра
#define A_PIN_SETTING_RESTOR 4    //аналоговый пин чтения резистора управления

#define D_PIN_GREEN_LED 13        //цифровой пин для зелёного светодиода
#define D_PIN_YELLOW_LED 12       //цифровой пин для жёлтого светодиода
#define D_PIN_RED_LED 11          //цифровой пин для красного светодиода
#define D_PIN_KEY 8               //цифровой пин чтения кнопки
#define D_PIN_LED 2               //цифровой пин подсветки
///========================================================///
///================ Подключаемая переферия ================///
///========================================================///
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 lcd = Adafruit_PCD8544(3, 4, 5, 6, 7); //дисплей от Nokia5110

char* textLCD = "This is textLCD";

Servo s;  //бинарная серва-тест
Servo s2; //интерполяционная серва-тест

const int SERVO_1 = 5;  //цифровой пин вывода сервы, действующей на бинарном методе
const int SERVO_2 = 6;  //цифровой пин вывода, действующей на интерполяционном методе

///========================================================///
///================== Инициализация и цикл ================///
///========================================================///
void setup() {
  Serial.begin(115200);

  pinMode(D_PIN_GREEN_LED, OUTPUT);       //цифровой пин для зелёного светодиода на вывод
  pinMode(D_PIN_YELLOW_LED, OUTPUT);      //цифровой пин для жёлтого светодиода на вывод
  pinMode(D_PIN_RED_LED, OUTPUT);         //цифровой пин для красного светодиода на вывод

  pinMode(D_PIN_LED, OUTPUT);       //пин подсветки экрана на вывод
  pinMode(D_PIN_KEY, INPUT);        //пин кнопки включитиь на приём

  digitalWrite(D_PIN_LED, HIGH);    //включить подсветку, пустив на него сигнал

  initialization(); //инициализируем переменные

  startSetting();

  delay(1000);
  //drawLCD();
}

void initialization() { //инициализация данных
  //дисплей
  lcd.begin();              // Инициализация дисплея
  lcd.setContrast(60);      // Устанавливаем контраст
  lcd.setTextColor(BLACK);  // Устанавливаем цвет текста
  lcd.setTextSize(1);       // Устанавливаем размер текста
  lcd.clearDisplay();       // Очищаем дисплей
  lcd.display();
  delay(1000);
  //сервы
  s.attach(SERVO_1);    //бинарная серва
  s2.attach(SERVO_2);   //интерполяционная серва
  //данные
  data = new int[window];          //выдклил память под массив считываемых значений
  drawGraphLCD = new int[LCDWIDTH]; //выдклил память под массив значений среднего считываемых значений
  zeroArrays(); //обнуление массивов на отрисовку
}

void loop() {
  ticket++;     //счётчик тиков

  debuger();

  readValue();  //считывает значение и заносит параметр
  actionServ(); //совершение действия для серв
  draw();       //отрисовка

  if (ticket >= ticketACTION) //обнуление тикетов на событии
  {
    ticket = 0;
  }
}

///========================================================///
///====================== Вычисления ======================///
///========================================================///
void readValue() {
  if (debug) {
    Serial.print("A5 value = ");
    Serial.println(analogRead(A_PIN_READ));
    delay(1000);
  }
  *(data + posData) = analogRead(A_PIN_READ);          //запись значения в массив
  if (++posData >= window) {                        //перевод указателя в начало после конца заполнения массива
    posData = 0;
  }
  *(drawGraphLCD + posDaraDrawGraphLCD) = getSrd(); //вычисление значения среднего и занесение в массив
  if (++posDaraDrawGraphLCD >= LCDWIDTH) {          //перевод указателя в начало после конца заполнения массива
    posDaraDrawGraphLCD = 0;
  }
}
void zeroArrays() { //обнуление массивов на отриссовку
  //обнуляем значения даты
  for (int i = 0; i < window; i++)
  {
    *(data + i) = 0;
  }
  //обнуляем значение на отрисовку
  for (int i = 0; i < LCDWIDTH; i++)  {
    *(drawGraphLCD + i) = 0;
  }
}
long getSrd() { //вычисление среднего по окну
  srd = 0;
  if (debug) {
    Serial.println("=============================================================================");
    Serial.println("============================    Data array start   ==========================");
    Serial.println("=============================================================================");

    for (int z = 0; z < window; z++)    {
      Serial.print("data[");
      Serial.print(z);
      Serial.print("] = ");
      Serial.println(*(data + z));
    }

    Serial.println("=============================================================================");
    Serial.println("============================    Data array end   ============================");
    Serial.println("=============================================================================");
  }
  for (int i = 0; i < window; i++)  {
    if (debug) {
      Serial.print(srd);
      Serial.print(" + ");
      Serial.print(*(data + i));
      Serial.print(" = ");
    }
    srd += *(data + i);
    if (debug) {
      Serial.println(srd);
    }
  }
  srd /= window;

  if (debug)  {
    Serial.println("=============================================================================");
    Serial.println(srd);
    delay(1000);
  }
  return srd;
}

void debuger() {
  while (true)
  {
    while (Serial.available() > 0)
    {
      char pin = Serial.read();
      if (pin > 1 && pin < 10) {
        Serial.print("=============================================================================");
        Serial.println(pin);
        Serial.print("=============================================================================");
      }
    }
    break;
  }
}

///========================================================///
///================ Алгоритм работы серв ==================///
///========================================================///
int ACTION = 70;      //переменная для действия сжал/разжал
int servStart = 0;    //"сжаая серва" - значение минимального угла для управляющей сервы
int servEnd = 180;    //"разжатая серва" - значение минимального угла для управляющей сервы
int actionStart = 0;  //старт для периода "интерполяции сервы"
int actionEnd = 500;  //конец для периода "интерполяции сервы"

void binaryAct(Servo servo) { //бинарный метод активации сервы
  //действие да/нет
  if (srd > ACTION)
  {
    servo.write(180);
  }
  if (srd < ACTION)
  {
    servo.write(0);
  }
}
void interpolationAct(Servo servo) { //интерполяционный метод активации сервы
  //интерполяция
  if (srd > actionStart && srd < actionEnd)
  {
    servo.write(map(srd, actionStart, actionEnd, servStart, servEnd));
  }
}
void actionServ() { //действие сервы
  binaryAct(s);
  interpolationAct(s2);
}
///========================================================///
///================= Настройки протеза ====================///
///========================================================///
bool debugLed = false;

const int iDelay = 500;//задержка в милисекундах при переходе из режима в режим

void startSetting() {
  actionStart = 1024;
  actionEnd = 0;

  if(debugLed) Serial.println("Входим в режим");
  ledControl(RED, ONLY);      //включаем красный светодиод
  reader(workWithLCD, 0);     //вход в режим
  ledControl(RED, BLINK);     //моргнули и оставили красный
  if(debugLed) Serial.println("Секунда старт");
  delay(1000);
  if(debugLed) Serial.println("Секунда енд");
  
  if(debugLed) Serial.println("Начали чтение минимума");
  reader(workWithLCD, 1);     //чтение минимума
  ledControl(YELLOW, ONLY);
  ledControl(YELLOW, BLINK);  //включаем жёлтый светодиод - конец чтения минимума
  if(debugLed) Serial.println("Секунда старт");
  delay(1000);
  if(debugLed) Serial.println("Секунда енд");
  
  if(debugLed) Serial.println("Начали чтение Максимума");
  reader(workWithLCD, 2);     //чтение максимума
  ledControl(YELLOW, REMOVE);
  onLed(GREEN, ONLY);         //включаем зелёный, устройство готово к работе
  if(debugLed) Serial.println("Готово!");
  delay(1000);
}

void reader(bool bDrawLCD, byte state) {//метод считывания данных по окну
  for (int i = 0; i < LCDWIDTH*LCDWIDTH; i++)  {
    readValue();  //считывает значение и заносит параметр
    switch (state) {
      case 1: { //нахождение минимума
          if (getSrd() < actionStart) {
            actionStart = srd;
          }
          break;
        }
      case 2: {
          if (getSrd() > actionEnd) {
            actionEnd = srd;
          }
          break;
        }
    }
    if (bDrawLCD)
      drawLCD();       //отрисовка
  }
}

void readValueSettingResistor()
{
  /*val = digitalRead(D_PIN_KEY);
    if(
    int analogRead(A_PIN_SETTING_RESTOR);*/
}


///========================================================///
///====================== Индикация =======================///
///========================================================///
void draw() { //метод для отрисовки
  if (ticket >= ticketACTION)
  {
    //методы на отрисовку графиков
    drawGraph();  //метод на работу с SerialPorts
    //drawGame(); //метод на работу с Unity
    //drawMyGraph();  //отрисовка "моего графика"

    drawLCD();    //метод на отрисовку через LCD
  }
}

//--------------------------------------------------------//
//------------- Режимы свечения светодиодов --------------//
//--------------------------------------------------------//
#define ADD 0    //включить, не выключая остальные
#define REMOVE 1 //выключить, оставив остальные
#define ONLY 2   //включить, выключив остальные
#define BLINK 3  //моргать
#define ALL_ON 4 //включить все
#define ALL_OFF 5//выключить все

//--------------------------------------------------------//
//---------------------- Светодиоды ----------------------//
//--------------------------------------------------------//
#define ALL 0         //все
#define RED 1         //красный
#define YELLOW 2      //жёлтый
#define GREEN 3       //зелёный

void onLed(byte nomber, bool state) { //включение светодиодов
  switch (nomber)
  {
    case 0: //выключить все
      {
        digitalWrite(D_PIN_GREEN_LED, state);
        digitalWrite(D_PIN_YELLOW_LED, state);
        digitalWrite(D_PIN_RED_LED, state);
        break;
      }
    case 1: //зелёный
      {
        digitalWrite(D_PIN_RED_LED, state);
        break;
      }
    case 2: //жёлтый
      {
        digitalWrite(D_PIN_YELLOW_LED, state);
        break;
      }
    case 3: //зелёный
      {
        digitalWrite(D_PIN_GREEN_LED, state);
        break;
      }
  }
}

void ledControl(byte led, byte mode) {
  switch (mode)
  {
    case ADD: { //включить, не выключая остальные
        onLed(led, HIGH);
        break;
      }
    case REMOVE: {  //выключить, оставив остальные
        onLed(led, LOW);
        break;
      }
    case ONLY: { //включить, выключив остальные
        onLed(ALL, LOW);
        onLed(led, HIGH);
        break;
      }
    case BLINK: { //моргать
        onLed(led, HIGH);
        delay(iDelay);
        onLed(led, LOW);
        delay(iDelay);
        onLed(led, HIGH);
        break;
      }
    case ALL_ON: { //включить все
        onLed(ALL, HIGH);
        break;
      }
    case ALL_OFF: { //выключить все
        onLed(ALL, LOW);
        break;
      }
  }
}

//--------------------------------------------------------//
//------------------ Экран от NOKIA5110 ------------------//
//--------------------------------------------------------//
int firstY = 0;

void drawGraph(int* graph) {
  for (int i = 0; i < LCDWIDTH; i++)
  {
    //long* drawGraphLCD данные о последних средних
    //posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD
    int y = LCDHEIGHT - 1 - map(*(graph + i), 0, 1023, 0, LCDHEIGHT);

    if (i != 0 && i != LCDWIDTH - 1)
    {
      lcd.drawLine(i - 1, firstY, i, y, BLACK); // x0, y0, x1, y1, color
    }
    else
    {
      lcd.drawPixel(i, y, BLACK); //отрисовка крайних сторон
    }
    firstY = y;
  }
}

void drawLCD() {
  lcd.clearDisplay();
  lcd.println(textLCD);
  lcd.drawLine(posDaraDrawGraphLCD, 0, posDaraDrawGraphLCD, LCDHEIGHT - 1, BLACK);

  drawGraph(data);
  drawGraph(drawGraphLCD);

  lcd.display();
}

//--------------------------------------------------------//
//------------------ Взаимодействие с PC -----------------//
//--------------------------------------------------------//
void drawGame() { //метод на работу с Unity
  Serial.println("$");
  //Serial.println(getSrd());
  Serial.println(sin(++ticket));  //вывод синусоиды для настройки
  Serial.println(";");
}
void drawGraph() { //метод на работу с SerialPorts
  Serial.println("$");
  Serial.println(analogRead(A_PIN_READ));
  Serial.println(" ");
  Serial.println(getSrd());
  Serial.println(" ");
  Serial.println(ACTION);//map(getSrd(), 0, 1023, 0, 180));
  Serial.println(";");
}
void drawMyGraph() { //отрисовка "моего графика"
  Serial.print(analogRead(A_PIN_READ));
  Serial.print(" ");
  Serial.print(getSrd());
  Serial.print(" ");
  Serial.print(map(getSrd(), 0, 1023, 0, 180));
}
