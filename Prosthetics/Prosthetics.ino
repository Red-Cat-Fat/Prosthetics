#include <Servo.h>

long* data;                   //данные чтения
int posData = 0;              //позиция нынешней записи в массив data
long srd = 0;                 //значение среднего

const int window = 400; //размер окна
const int sizeLCD = 48; //ширина экранчика
const int PINREAD = 5;  //аналоговый пин чтения данных

Servo s;  //бинарная серва-тест
Servo s2; //интерполяционная серва-тест

const int SERVO_1 = 5;  //цифровой пин вывода сервы, действующей на бинарном методе
const int SERVO_2 = 6;  //цифровой пин вывода, действующей на

///========================================================///
///================== Инициализация и цикл =================///
///========================================================///

void setup() {
  Serial.begin(115200);
  initialization(); //инициализируем переменные
}

void initialization() { //инициализация данных
  s.attach(SERVO_1);    //бинарная серва
  s2.attach(SERVO_2);   //интерполяционная серва

  data = new long[window];          //выдклил память под массив считываемых значений

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
  getSrd(); //вычисление значения среднего и занесение в массив
}
void zeroArrays() { //обнуление массивов на отриссовку
  //обнуляем значения даты
  for (int i = 0; i < window; i++)
  {
    *(data + i) = 0;
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

int ACTION = 70;  //переменная для действия сжал/разжал
int servStart = 0;  //"сжаая серва" - значение минимального угла для управляющей сервы
int servEnd = 180;  //"разжатая серва" - значение минимального угла для управляющей сервы
int actionStart = 0;  //старт для периода "интерполяции сервы"
int actionEnd = 500;  //конец для периода "интерполяции сервы"

void binaryAct(Servo servo){  //бинарный метод активации сервы
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
void interpolationAct(Servo servo){ //интерполяционный метод активации сервы
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

  //if (++ticket == 25)
  {
    //методы на отрисовку графиков
    //drawGame(); //метод на работу с Unity
    drawGraph();  //метод на работу с SerialPorts
    //drawMyGraph();  //отрисовка "моего графика"

    //ticket = 0;
  }
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
