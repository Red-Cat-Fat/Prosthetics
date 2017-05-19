#include <Servo.h>            //сервы
#include <Adafruit_GFX.h>     //экранчик
#include <Adafruit_PCD8544.h> //экранчик

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
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7); //дисплей от Nokia5110

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
  display.begin();              // Инициализация дисплея
  display.setContrast(60);      // Устанавливаем контраст
  display.setTextColor(BLACK);  // Устанавливаем цвет текста
  display.setTextSize(1);       // Устанавливаем размер текста
  display.clearDisplay();       // Очищаем дисплей
  display.display();
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
  readValue();  //считывает значение и заносит параметр
  actionServ(); //совершение действия для серв
  draw();       //отрисовка
}

///========================================================///
///====================== Вычисления ======================///
///========================================================///

void readValue() {
  *(data + posData) = analogRead(PINREAD);  //запись значения в массив
  if (++posData >= window)                  //перевод указателя в начало после конца заполнения массива
  {
    posData = 0;
  }
  *(drawGraphLCD + posDaraDrawGraphLCD) = getSrd(); //вычисление значения среднего и занесение в массив
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
    srd += *(data + i);
  }
  srd /= window;
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

int ticket = 0;//переменная для вывода данных через заданное количество действий
void draw() { //метод для отрисовки
  if (++ticket == 25)
  {
    //методы на отрисовку графиков
    //drawGame(); //метод на работу с Unity
    drawGraph();  //метод на работу с SerialPorts
    //drawMyGraph();  //отрисовка "моего графика"

    ticket = 0;
  }
  drawLCD();
}

void drawLCD() {
  display.drawBitmap(LCDWIDTH/2-8, LCDHEIGHT/2-8, logoBmp, 24, 16, BLACK); // x, y, logo, w, h, color 
  display.display();
  delay(2000);
  
    // Очищаем дисплей
  display.clearDisplay();
  display.display();
  delay(1000);
  
  // Рисуем несколько пикселей (точек)
  display.drawPixel(0, 0, BLACK);  
  display.drawPixel(1, 1, BLACK);  
  display.drawPixel(2, 2, WHITE); // Посередине белый пиксель  
  display.drawPixel(3, 3, BLACK);  
  display.drawPixel(4, 4, BLACK);  
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем диагональ
  display.drawLine(0, LCDHEIGHT-1, LCDWIDTH, 0, BLACK); // x0, y0, x1, y1, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Для рисования вертикальных и горизонтальных линий лучше использовать
  // более быстрые функции
  display.drawFastVLine(LCDWIDTH/2, 0, LCDHEIGHT, BLACK); // x, y, h, color
  display.drawFastHLine(0, LCDHEIGHT/2, LCDWIDTH, BLACK); //x, y, w, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем прямоугольник
  display.drawRect(LCDWIDTH/4, LCDHEIGHT/4, LCDWIDTH/2, LCDHEIGHT/2, BLACK); // x, y, w, h, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем закрашенный прямоугольник
  display.fillRect(LCDWIDTH/4, LCDHEIGHT/4, LCDWIDTH/2, LCDHEIGHT/2, BLACK); // x, y, w, h, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Закрашиваем весь дисплей
  display.fillScreen(BLACK);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем окружность
  display.drawCircle(LCDWIDTH/2, LCDHEIGHT/2, LCDHEIGHT/2, BLACK); // x, y, r, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем закрашенную окружность
  display.fillCircle(LCDWIDTH/2, LCDHEIGHT/2, LCDHEIGHT/2, BLACK); // x, y, r, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем треугольник
  display.drawTriangle(LCDWIDTH/4, LCDHEIGHT/4, 3*LCDWIDTH/4, LCDHEIGHT/4, LCDWIDTH/2, 3*LCDHEIGHT/4, BLACK); // x0, y0, x1, y1, x2, y2, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем закрашенный треугольник
  display.fillTriangle(LCDWIDTH/4, LCDHEIGHT/4, 3*LCDWIDTH/4, LCDHEIGHT/4, LCDWIDTH/2, 3*LCDHEIGHT/4, BLACK); // x0, y0, x1, y1, x2, y2, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем прямоугольник с закругленными углами
  display.drawRoundRect(LCDWIDTH/4, LCDHEIGHT/4, LCDWIDTH/2, LCDHEIGHT/2, 10, BLACK); // x, y, w, h, r, color
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем закрашенный прямоугольник с закругленными углами
  display.fillRoundRect(LCDWIDTH/4, LCDHEIGHT/4, LCDWIDTH/2, LCDHEIGHT/2, 10, BLACK); // x, y, w, h, r, colordisplay.display();
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Рисуем заранее подготовленное лого
  // Подготовлен массив из 16 пар байтов
  // каждый байт состоит из 8 битов, соответсвенно
  // получаем матрицу 16х16 битов, 1-черный цвет, 0-белый цвет
  display.drawBitmap(LCDWIDTH/2-8, LCDHEIGHT/2-8, logoBmp, 24, 16, BLACK); // x, y, logo, w, h, color 
  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
  delay(1000);

  // Выведем текст
  display.print("Zelectro");
  delay(3000);

  display.clearDisplay();
  display.display();
  delay(5000);
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
