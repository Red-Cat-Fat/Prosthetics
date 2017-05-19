#include <Servo.h>            //сервы
#include <Adafruit_GFX.h>     //экранчик
#include <Adafruit_PCD8544.h> //экранчик

const int ticketACTION = 0;
int ticket = 0;//переменная для вывода данных через заданное количество действий

bool debug = true; //для дебага

long* data;                   //данные чтения
int posData = 0;              //позиция нынешней записи в массив data
long srd = 0;                 //значение среднего
long* drawGraphLCD;           //данные о последних средних
int posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD

const int window = 400; //размер окна
const int sizeLCD = 48; //ширина экранчика
const int PINREAD = 5;  //аналоговый пин чтения данных

const static unsigned char PROGMEM logoBmp[] =
{
  B11111111, B11111111, B10000000,
  B11111111, B11111111, B10000000,
  B11111111, B11111111, B10000000,
  B11111100, B00000011, B10000000,
  B11111000, B00000001, B10000000,
  B11111100, B00000011, B10000000,
  B11111111, B11000011, B10000000,
  B11111111, B10000111, B10000000,
  B11111111, B10001111, B10000000,
  B11111111, B00001111, B10000000,
  B11111110, B00011111, B10000000,
  B11111110, B00011111, B10000000,
  B11111100, B00111111, B10000000,
  B11111100, B01111111, B10000000,
  B11111000, B00000011, B10000000,
  B11111000, B00000001, B10000000
};

///========================================================///
///================ Подключаемая переферия ================///
///========================================================///

// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 lcd = Adafruit_PCD8544(3, 4, 5, 6, 7); //дисплей от Nokia5110

Servo s;  //бинарная серва-тест
Servo s2; //интерполяционная серва-тест

const int SERVO_1 = 5;  //цифровой пин вывода сервы, действующей на бинарном методе
const int SERVO_2 = 6;  //цифровой пин вывода, действующей на интерполяционном методе

///========================================================///
///================== Инициализация и цикл ================///
///========================================================///

void setup() {
  Serial.begin(115200);
  initialization(); //инициализируем переменные
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
  data = new long[window];          //выдклил память под массив считываемых значений
  drawGraphLCD = new long[sizeLCD]; //выдклил память под массив значений среднего считываемых значений
  zeroArrays(); //обнуление массивов на отрисовку
}

void loop() {
  ticket++;     //счётчик тиков

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
  *(data + posData) = analogRead(PINREAD);  //запись значения в массив
  if(debug) {
    Serial.print("A5 value = ");
    Serial.println(analogRead(PINREAD));
    delay(1000);
  }
  if (++posData >= window)                  //перевод указателя в начало после конца заполнения массива
  {
    posData = 0;
  }
  *(drawGraphLCD + posDaraDrawGraphLCD) = getSrd();//вычисление значения среднего и занесение в массив
  if (++posDaraDrawGraphLCD >= sizeLCD)             //перевод указателя в начало после конца заполнения массива
  {
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
  for (int i = 0; i < sizeLCD; i++)
  {
    *(drawGraphLCD + i) = 0;
  }
}
long getSrd() { //вычисление среднего по окну
  srd = 0;
  for (int i = 0; i < window; i++)
  {
    /*if (debug)
    {
      Serial.print(srd);
      Serial.print(" + ");
      Serial.print(*(data + i));
      Serial.print(" = ");
    }*/
    srd += *(data + i);
    //if (debug)Serial.println(srd);
  }
  srd /= window;
  
  /*if (debug)
  {
    Serial.print("=============================================================================");
    Serial.print(srd);
    delay(1000);
  }*/
  return srd;
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
///================= Отрисовка графиков ===================///
///========================================================///

void draw() { //метод для отрисовки
  if (ticket >= ticketACTION)
  {
    //методы на отрисовку графиков
    drawGraph();  //метод на работу с SerialPorts
    //drawGame(); //метод на работу с Unity
    //drawMyGraph();  //отрисовка "моего графика"

    //drawLCD();    //метод на отрисовку через LCD
  }
}

int firstY = 0;
void drawLCD() {
  lcd.clearDisplay();
  for (int i = 0; i < sizeLCD; i++)
  {
    //long* drawGraphLCD данные о последних средних
    //posDaraDrawGraphLCD = 0;  //позиция нынешней записи в массив drawGraphLCD
    int y = map(*(drawGraphLCD + i), 0, 1023, 0, LCDHEIGHT);

    if (i != 0 && i != sizeLCD - 1)
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
