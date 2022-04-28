#define pot A0
#define ip1 5
#define ip2 6
#define enA 2
#define enB 3
#define button 4

int dir;
float targetpos,targetrpm;
volatile long encoderCount=0;
float pos,posprev=0,rpm;
volatile unsigned long count=0;
unsigned long countprev,T,Tprev;
float Tdiff,V,Vmax=11.5;
//PID parameters
float epos,eprevpos=0,erpm,eprevrpm=0,eintegralpos=0,eintegralrpm=0;

void setup()
{
  Serial.begin(9600);
  pinMode(pot,INPUT);
  pinMode(ip1,OUTPUT);
  pinMode(ip2,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  pinMode(enA,INPUT_PULLUP);
  pinMode(enB,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(enA),readpos,RISING);

  cli();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  OCR1A=12499;  //Prescaler=64
  TCCR1B|=1<<WGM12;
  TCCR1B|=1<<CS11|1<<CS10;
  TIMSK1|=1<<OCIE1A;
  sei();
}

void loop()
{
  if(digitalRead(button)==HIGH)
  {
    int potip=analogRead(pot);
    targetpos=map(potip,0,1023,0,360);
    float kp=12,ki=0.08,kd=27.5;
    T=micros();
    pos=encoderCount;
    Tdiff=((float)(T-Tprev))/1000000;
    //PID Algorithm
    epos=(targetpos-((int)pos%360));
    float derivative=(epos-eprevpos)/Tdiff;
    eintegralpos+=epos*Tdiff;
    //Control Signal
    V=kp*epos+ki*eintegralpos+kd*derivative;
    if(V>0)
      dir=1;
    else if(V<0)
      dir=-1;
    else
      dir=0;
    V=fabs(V);
    if(V>Vmax)
      V=Vmax;
    setOutput(dir,V);
    Serial.print(targetpos);
    Serial.print(" ");
    Serial.println((int)pos%360);
    posprev=pos;
    countprev=count;
    Tprev=T;
    eprevpos=epos;
  }
  else
  {
    if(count>countprev)
    {
      int potip=analogRead(pot);
      T=micros();
      pos=encoderCount;
      Tdiff=((float)(T-Tprev))/1000000;
      targetrpm=map(potip,0,1023,-166,166);
      float kp=0.02,ki=0.15,kd=0;
      rpm=(pos-posprev)/Tdiff/6;
      //PID Algorithm
      erpm=targetrpm-rpm;
      float derivative=(erpm-eprevrpm)/Tdiff;
      eintegralrpm+=erpm*Tdiff;
      //Control Signal
      V=kp*erpm+ki*eintegralrpm+kd*derivative;
      if(V>0)
        dir=1;
      else if(V<0)
        dir=-1;
      else
        dir=0;
      V=fabs(V);
      if(V>Vmax)
        V=Vmax;
      setOutput(dir,V);
      Serial.print(targetrpm);
      Serial.print(" ");
      Serial.println(rpm);
      posprev=pos;
      countprev=count;
      Tprev=T;
      eprevrpm=erpm;
    }
  }
}

void readpos()
{
  bool b=digitalRead(enB);
  if(b==LOW)
    encoderCount++;
  else
    encoderCount--;
}

void setOutput(int dir,float V)
{
  int PWMval=(int)(255*V/Vmax);
  if(PWMval>255)
    PWMval=255;
  if(dir==1)
  {
    analogWrite(ip1,PWMval);
    analogWrite(ip2,0);
  }
  else if(dir==-1)
  {
    analogWrite(ip1,0);
    analogWrite(ip2,PWMval);
  }
  else
  {
    analogWrite(ip1,0);
    analogWrite(ip2,0);
  }
}

ISR(TIMER1_COMPA_vect)
{
  count++;
}
