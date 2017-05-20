#include <Servo.h>

long* data; //данные чтения
int posData = 0; //позиция нынешней записи в массив
long srd = 0;  //значение среднего
long* drawGraphLCD;


const int window = 400; //размер окна
const int sizeLCD = 48; //ширина экранчика
const int PINREAD = 5;  //аналоговый пин чтения данных

const int SERVO_1 = 5;  //цифровой пин вывода сервы, действующей на бинарном методе
const int SERVO_2 = 6;  //цифровой пин вывода, действующей на 

Servo s;
Servo s2;

void setup() {
  Serial.begin(115200);
  data = new long[window];
  drawGraphLCD = new long[
  s.attach(SERVO_1);
  s2.attach(SERVO_2);
  for (int i = 0; i < window; i++)
  {
    *(data + i) = 0;
  }
}

int tick = 0;
int a = 0;

void loop() {
  *(data + posData) = analogRead(PINREAD);  //запись значения в массив
  if (++posData >= window)                  //перевод указателя в начало после конца заполнения массива
  {
    posData = 0;
  }

  getSrd(); //вычисление значения среднего
  
  actionServ(); //совершение действия

  //методы на отрисовку графиков
  
  //drawGame();
  //outDate();
  drawGraph();  
  //drawMyGraph();
}

long getSrd(){  //вычисление среднего по окну
  srd = 0;
  for (int i = 0; i < window; i++)
  {
    srd += *(data + i);
  }
  srd /= window;
  return srd;
}

int ACTION = 70;  //переменная для действия сжал/разжал
int servStart = 0;  //"сжаая серва" - значение минимального угла для управляющей сервы
int servEnd = 180;  //"разжатая серва" - значение минимального угла для управляющей сервы
int actionStart = 0;  //старт для периода "интерполяции сервы"
int actionEnd = 500;  //конец для периода "интерполяции сервы"

void actionServ(){  //действие сервы
  //действие да/нет
  if(srd > ACTION)
  {
    s.write(180);
  }
  if(srd < ACTION)
  {
    s.write(0);
  }
  
  //интерполяция
  if(srd > actionStart && srd < actionEnd)
  {
    s2.write(map(srd, actionStart, actionEnd, servStart, servEnd));
  }
}

int ticket = 0;//переменная для вывода данных через заданное количество действий
void drawGame(){  //метод на работу с Unity
//if (++ticket == 25)
  {
    Serial.println("$");

    //Serial.println(getSrd());

    Serial.println(sin(++ticket));
    
    Serial.println(";");
    //ticket = 0;
  }
}

void drawGraph(){ //метод на работу с SerialPorts
  if (++ticket == 25)
  {
    Serial.println("$");
    Serial.println(analogRead(PINREAD));
    Serial.println(" ");
    Serial.println(getSrd());
    Serial.println(" ");
    Serial.println(ACTION);//map(getSrd(), 0, 1023, 0, 180));
    Serial.println(";");
    ticket = 0;
  }
}

void drawMyGraph(){ //отрисовка "моего графика"
  Serial.print(analogRead(PINREAD));
  Serial.print(" ");
  Serial.print(getSrd());
  Serial.print(" ");
  Serial.print(map(getSrd(), 0, 1023, 0, 180));
}
