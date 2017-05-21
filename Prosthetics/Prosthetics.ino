#include <Servo.h>            //сервы
#include <Adafruit_GFX.h>     //экранчик
#include <Adafruit_PCD8544.h> //экранчик

const int ticketACTION = 0; //шаг срабатывания события
int ticket = 0;             //переменная для вывода данных через заданное количество действий

bool debug = false; //для дебага

int* data;                   //данные чтения
int posData = 0;              //позиция нынешней записи в массив data
long srd = 0;                 //значение среднего
int* drawGraphLCD;           //данные о последних средних
int posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD

const int window = LCDWIDTH;// 200; //размер окна
const int PINREAD = 5;  //аналоговый пин чтения данных

///========================================================///
///================ Подключаемая переферия ================///
///========================================================///

// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 lcd = Adafruit_PCD8544(3, 4, 5, 6, 7); //дисплей от Nokia5110
const int PIN_LED = 2;
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

  pinMode(PIN_LED, OUTPUT);     //подсветка экрана
  digitalWrite(PIN_LED, HIGH);  //включить подсветку
  
  initialization(); //инициализируем переменные

  startSetting();
  
  delay(1000);
  drawLCD();
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
    Serial.println(analogRead(PINREAD));
    delay(1000);
  }
  *(data + posData) = analogRead(PINREAD);          //запись значения в массив
  if (++posData >= window){                         //перевод указателя в начало после конца заполнения массива
    posData = 0;
  }
  *(drawGraphLCD + posDaraDrawGraphLCD) = getSrd(); //вычисление значения среднего и занесение в массив
  if (++posDaraDrawGraphLCD >= LCDWIDTH){           //перевод указателя в начало после конца заполнения массива
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

void debuger(){
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
///============== Алгоритм работы протеза =================///
///========================================================///

void startSetting()
{
  actionStart = 1024;
  actionEnd = 0;

  //заполняем окно
  *textLCD = "Rasslabtes";
  for(int i = 0; i < LCDWIDTH; i++)
  {
    readValue();  //считывает значение и заносит параметр
    drawLCD();       //отрисовка
  }
  
  //считывание расслабленной руки, поиск минимума
  for(int i = 0; i < LCDWIDTH; i++)
  {
    readValue();  //считывает значение и заносит параметр
    if(getSrd() < actionStart){
      actionStart = srd;
    }
    drawLCD();       //отрисовка
  }

  *textLCD = "Naprеgite ruku";
  //считывание напряжённой руки, поиск максимума
  for(int i = 0; i < LCDWIDTH; i++) {
    readValue();  //считывает значение и заносит параметр
    if(getSrd() > actionEnd){
      actionEnd = srd;
    }
    drawLCD();       //отрисовка
  }
  
  *textLCD = "0";
}
///========================================================///
///================= Отрисовка графиков ===================///
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

int firstY = 0;

void drawLCD() {
  lcd.clearDisplay();
  lcd.println(textLCD);
  for (int i = 0; i < LCDWIDTH; i++)
  {
    //long* drawGraphLCD данные о последних средних
    //posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD
    int y = LCDHEIGHT - 1 - map(*(drawGraphLCD + i), 0, 1023, 0, LCDHEIGHT);

    if (i != 0 && i != LCDWIDTH - 1)
    {
      lcd.drawLine(i - 1, firstY, i, y, BLACK); // x0, y0, x1, y1, color
    }
    else
    {
      lcd.drawPixel(i, y, BLACK); //отрисовка крайних сторон
    }
    lcd.drawLine(posDaraDrawGraphLCD, 0, posDaraDrawGraphLCD, LCDHEIGHT - 1, BLACK);
    firstY = y;
  }

  for (int i = 0; i < LCDWIDTH; i++)
  {
    //long* drawGraphLCD данные о последних средних
    //posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD
    int y = LCDHEIGHT - 1 - map(*(data + i), 0, 1023, 0, LCDHEIGHT);

    if (i != 0 && i != LCDWIDTH - 1)
    {
      lcd.drawLine(i - 1, firstY, i, y, BLACK); // x0, y0, x1, y1, color
    }
    else
    {
      lcd.drawPixel(i, y, BLACK); //отрисовка крайних сторон
    }
    lcd.drawLine(posDaraDrawGraphLCD, 0, posDaraDrawGraphLCD, LCDHEIGHT - 1, BLACK);
    firstY = y;
  }
  
  lcd.display();
}

void drawGame() { //метод на работу с Unity
  Serial.println("$");
  //Serial.println(getSrd());
  Serial.println(sin(++ticket));  //вывод синусоиды для настройки
  Serial.println(";");
}
void drawGraph() { //метод на работу с SerialPorts
  Serial.println("$");
  Serial.println(analogRead(PINREAD));
  Serial.println(" ");
  Serial.println(getSrd());
  Serial.println(" ");
  Serial.println(ACTION);//map(getSrd(), 0, 1023, 0, 180));
  Serial.println(";");
}
void drawMyGraph() { //отрисовка "моего графика"
  Serial.print(analogRead(PINREAD));
  Serial.print(" ");
  Serial.print(getSrd());
  Serial.print(" ");
  Serial.print(map(getSrd(), 0, 1023, 0, 180));
}
