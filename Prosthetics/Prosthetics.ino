#include <Servo.h>

long* data; //данные чтения
int pos = 0; //позиция нынешней записи в массив
long srd = 0;

const int window = 400; //размер окна
const int PINREAD = 5;  //аналоговый пин чтения данных

const int SERVO_1 = 5;  //цифровой пин вывода
const int SERVO_2 = 6;  //цифровой пин вывода



Servo s;
Servo s2;
void setup() {
  Serial.begin(115200);
  data = new long[window];
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
  *(data + pos) = analogRead(PINREAD);  //запись значения в массив
  if (++pos >= window)                  //перевод указателя в начало после конца заполнения массива
  {
    pos = 0;
  }

  getSrd(); //
  
  actionServ();
  
  //drawGame();
  //outDate();
  drawGraph();
  //drawMyGraph();
}

long getSrd()
{
  srd = 0;
  for (int i = 0; i < window; i++)
  {
    srd += *(data + i);
  }
  srd /= window;
  return srd;
}

const int ACTION = 70;
const int servStart = 0;
const int servEnd = 180;
const int actionStart = 0;
const int actionEnd = 500;

void actionServ()
{
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

/*
  void outDate()
  {
  Serial.print("DateNow = ");
  Serial.println(analogRead(PINREAD));
  Serial.print("Sred = ");
  Serial.println(getSrd());
  if (tick++ == window - 1)
  {
  Serial.println("======================================================================");
  Serial.print("Sred = ");
  Serial.print(getSrd());
  Serial.print(" VS ");
  Serial.println(analogRead(PINREAD));
  for (int i = 0; i < window; i++)
  {
  Serial.print(" [");
  Serial.print(i);
  Serial.print("]=");
  Serial.println(*(data + i));
  }
  tick = 0;
  }
  delay(1000);
  }*/

int ticket = 0;

void drawGame()
{
//if (++ticket == 25)
  {
    Serial.println("$");

    //Serial.println(getSrd());

    Serial.println(sin(++ticket));
    
    Serial.println(";");
    ticket = 0;
  }
}

void drawGraph()
{
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


void drawMyGraph()
{
  Serial.print(analogRead(PINREAD));
  Serial.print(" ");
  Serial.print(getSrd());
  Serial.print(" ");
  Serial.print(map(getSrd(), 0, 1023, 0, 180));
}
