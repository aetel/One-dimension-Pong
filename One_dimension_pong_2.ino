/*Codigo para One dimension Pong
La idea es que detras de una unica tira de leds se mueva la pelota, a lo largo del eje o de la tira, y que tenga que ser golpeada en cada extremo en el momemento justo
Este codigo esta muy chapucero... como todos mis codigos, pero es una prueba de concepto... como todos mis codigos. No te suicides. Lo hice en un bus, de camino a casa a las 11 de la noche
y probablemnte lo siga haciendolo mañana, de camino a la universidad, antes de presentarlo sin haberlo probado
Tadaaaa ;) ;) ;) como es ese emoji con lagrima y sonrisa... o mejor.... lagrima y sonrisa poker... sep ese. no existe, pero me lo imagino. es mi hijo
Terminado a las 10:10. todavia por probar... funcionara? es muy pero que muy simple
*/
//Librerias
#include <NeoPixelBus.h>    //La del ws2812b
#include <NeoPixelAnimator.h> 
const uint16_t PixelCount = 110;  //La cantidad de pixeles que tiene la tira. Creo que tiene 120, pero ni idea
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount);   //Inicializamos el objeto de la tira, con el DMA del esp8266
//#define Boton1 D0   //Todavia tengo que poner los pines correctamente, que esto con el wemos es una cosa...
//#define Boton2 D1
const int8_t Boton1= D3;  //D3 es GPIO0, que esta soldado al pixel mas lejano (110)
const int8_t Boton2= D4;  //D4 es GPIO2, que esta soldado al pixel ,mas cercano
byte puntos[2]={0,0};  //El array que mantendra los puntos
RgbColor mem[30];
int j=0;
bool modus=0;
bool perdioderecho=false;
void setup() {
  // put your setup code here, to run once:
  SetRandomSeed();
  pinMode(Boton1,INPUT_PULLUP); //Inicializamos los botones
  pinMode(Boton2,INPUT_PULLUP);
  //Serial.begin(115200);
  strip.Begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(puntos[0]==0 && puntos[1]==0){ //Juego nuevo
    while(digitalRead(Boton1)||digitalRead(Boton2)){
      yield();
      if(j==0){
        rellenarolas();
      }
      strip.RotateLeft(1);
      j--;
      strip.SetPixelColor(PixelCount/2,mem[j]); //mem contiene los colores de una ola
      for(int h=(PixelCount/2); h>0; h--){
        strip.SetPixelColor(PixelCount-h,strip.GetPixelColor(h));
      }
      strip.Show();
      delay(50);
    }//Cuando salga de aqui, va a empezar el juego. Ponemos las "barreras" y iniciamos a la derecha
    vaciar();
    strip.SetPixelColor(10,RgbColor(255,0,0));
    strip.SetPixelColor(100, RgbColor(255,0,0));
    j=1;
    
  }
  nuevojuego(perdioderecho,1);
  j++;
  

}
void nuevojuego(bool perdedor, int nivel){
  //Si perdedor =1, izquierda saca, sino derecha saca
  int tiempo=70-10*nivel;
  vaciar();
  strip.Show();
  RgbColor inicio;
  RgbColor colafinal;
  switch(nivel){
    case 1:
    inicio=RgbColor(0,255,0);
    colafinal=RgbColor(0,0,255);
    break;
    case 2:
    inicio=RgbColor(60,255,0);
    colafinal=RgbColor(0,0,255);
    break;
    case 3:
    inicio=RgbColor(80,160,0);
    colafinal=RgbColor(0,255,0);
    break;
    case 4:
    inicio=RgbColor(255,0,0);
    colafinal=RgbColor(0,255,0);
    break;
    default:
    inicio=RgbColor(255,255,255);
    colafinal=RgbColor(0,0,0);
    break;
  }
  RgbColor bola[15];
  for(int h=0;h<15;h++){
    bola[h]=RgbColor::LinearBlend(inicio,colafinal,(float)h/15.0);
  }
  yield();
  bool gool=false;
  if(perdedor){
    int m=0;
    while(!gool){
      for(int h=0; h<15;h++){ //Esta parte mete la "bola" en el campo
        activarmarcadores(true);
        strip.SetPixelColor(11,bola[h]);
        strip.Show();
        activarmarcadores(false);
        strip.RotateRight(1);
        delay(tiempo);
      }//Una vez dentro, la movemos
      while(digitalRead(Boton1)&&m<90){
        activarmarcadores(false);
        strip.RotateRight(1);
        activarmarcadores(true);
        delay(tiempo);
        m++;
        strip.Show();
      }
      if(m>=90||m<86){  //Solo hay un espacio critico de 4 pixeles
        //Si=tr entro aqui, perdio el otro
        gool=true;
        puntos[0]=puntos[0]+1;
        perdioderecho=false;
      }
      else{
        for(int h=0; h<15;h++){ //Esta parte mete la "bola" en el campo
          activarmarcadores(true);
          strip.SetPixelColor(99,bola[h]);
          strip.Show();
          activarmarcadores(false);
          strip.RotateLeft(1);
          delay(tiempo);
      }//Una vez dentro, la movemos
        m=0;
        while(digitalRead(Boton2)&&m<90){
          activarmarcadores(false);
        strip.RotateLeft(1);
        activarmarcadores(true);
        delay(tiempo);
        m++;
        strip.Show();
        }
      }
      if(m>=90||m<86){  //Solo hay un espacio critico de 4 pixeles
        //Si=tr entro aqui, perdio el otro
        gool=true;
        puntos[1]=puntos[1]+1;
        perdioderecho=true;
      }
      else{
        m=0;
      }
      tiempo=tiempo-5;
    }
  }
  else{
    int m=0;
    while(!gool){
      for(int h=0; h<15;h++){ //Esta parte mete la "bola" en el campo
        activarmarcadores(true);
        strip.SetPixelColor(99,bola[h]);
        strip.Show();
        activarmarcadores(false);
        strip.RotateLeft(1);
        delay(tiempo);
      }//Una vez dentro, la movemos
      while(digitalRead(Boton2)&&m<90){
        activarmarcadores(false);
        strip.RotateLeft(1);
        activarmarcadores(true);
        delay(tiempo);
        m++;
        strip.Show();
      }
      if(m>=90||m<86){  //Solo hay un espacio critico de 4 pixeles
        //Si=tr entro aqui, perdio el otro
        gool=true;
        puntos[1]=puntos[1]+1;
        perdioderecho=true;
      }
      else{
        for(int h=0; h<15;h++){ //Esta parte mete la "bola" en el campo
          activarmarcadores(true);
          strip.SetPixelColor(11,bola[h]);
          strip.Show();
          activarmarcadores(false);
          strip.RotateRight(1);
          delay(tiempo);
      }//Una vez dentro, la movemos
        m=0;
        while(digitalRead(Boton1)&&m<90){
          activarmarcadores(false);
        strip.RotateRight(1);
        activarmarcadores(true);
        delay(tiempo);
        m++;
        strip.Show();
        }
      }
      if(m>=90||m<86){  //Solo hay un espacio critico de 4 pixeles
        //Si=tr entro aqui, perdio el otro
        gool=true;
        puntos[0]=puntos[0]+1;
        perdioderecho=false;
      }
      else{
        m=0;
      }
      tiempo=tiempo-5;
    }
  }
}
void mover(bool derecha, int pos){
  if(derecha){  
    strip.RotateRight(pos);
  }
  else{
    strip.RotateLeft(pos);
  }
}
void rellenarolas(){
  RgbColor ColorInicial=mem[0];
  RgbColor ColorFinal=RgbColor(random(150),random(150),random(150));
//  if(modus){
//    ColorInicial=RgbColor(0,255,0);
//    ColorFinal=RgbColor(0,0,255);
//  }
//  else{
//    ColorInicial=RgbColor(0,0,255);
//    ColorFinal=RgbColor(0,255,0);
//  }
  float divisor=0.0;
  for(int h=0; h<30;h++){
    divisor=(float)h/30.0;
    mem[29-h]=RgbColor::LinearBlend(ColorInicial, ColorFinal, divisor);
  }
  j=29;
  //modus=!modus;
}
void SetRandomSeed(){
  pinMode(A0, INPUT);
    uint32_t seed;
    seed = analogRead(A0);
    delay(1);
    for (int shifts = 3; shifts < 31; shifts += 3){
        seed ^= analogRead(A0) << shifts;
        delay(1);
    }
    randomSeed(seed);
}
void activarmarcadores(bool activo){
  if(activo){
    strip.SetPixelColor(10,RgbColor(255,0,0));
    strip.SetPixelColor(100,RgbColor(255,0,0));
    for(int h=puntos[0];h>0;h--){
      strip.SetPixelColor(h,RgbColor(10*(puntos[0]-puntos[1]),10*puntos[0],5*puntos[1]));
    }
    for(int h=PixelCount-puntos[1];h<PixelCount;h++){
      strip.SetPixelColor(h,RgbColor(10*(puntos[1]-puntos[0]),10*puntos[1],5*puntos[0]));
    }
  }
  else{
    for(int h=0;h<11;h++){
      strip.SetPixelColor(h,RgbColor(0,0,0));
    }
    //strip.SetPixelColor(10,RgbColor(0,0,0));
    //strip.SetPixelColor(100,RgbColor(0,0,0));
    for(int h=100; h<PixelCount;h++){
      strip.SetPixelColor(h,RgbColor(0,0,0));
    }
  }
}
void vaciar(){
  for(int h=0; h<PixelCount; h++){
    strip.SetPixelColor(h,RgbColor(0,0,0));
  }
}
