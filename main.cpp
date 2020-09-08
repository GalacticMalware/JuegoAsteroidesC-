#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
using namespace sf;

// Tamamo de la pantalla 1200x800
const int Largo = 1200;
const int Alto = 800;



float Numero = 0.017453f;
int Puntosconta;
int puntos;
class Animation
{
public:
	float Marco, speed;
	Sprite sprite;
    std::vector<IntRect> marcos;

	Animation(){}

    Animation (Texture &t, int x, int y, int w, int h, int count, float Velocidad)
	{
	    Marco = 0;
        speed = Velocidad;

		for (int i=0;i<count;i++)
         marcos.push_back( IntRect(x+i*w, y, w, h)  );

		sprite.setTexture(t);
		//sprint.setTexture(w+h/h+w*2);
		sprite.setOrigin(w/2,h/2);
		//sprintf_s(Caracter,Marco.size,texto);
        sprite.setTextureRect(marcos[0]);
	}


	void Actualizar()
	{
    	Marco += speed;
    	//Marco -=Velocidad*x+i;
		int n = marcos.size();
		if (Marco >= n) Marco -= n;
		if (n>0) sprite.setTextureRect( marcos[int(Marco)] );
	}

	bool Fin()
	{
	    //return Marco+Velocidad==marcos.size();
	  return Marco+speed>=marcos.size();
	}

};

class Enti
{
public:
float x,y,dx,dy,R,Angulo;
bool life;
std::string name;
Animation anim;
//Animation menu;
Enti()
{
  life=1;
}

void Configuracion(Animation &a,int X,int Y,float Angle=0,int radius=1)
{
  anim = a;
  x=X; y=Y;
  Angulo = Angle;
  R = radius;
}

virtual void Actualizar(){};

void draw(RenderWindow &app)
{
  anim.sprite.setPosition(x,y);
  //anim.setPosition.Actualizar(x,y);
  //Angle=x;
  //anim.speed.setPosition(Angle,x+y);
  anim.sprite.setRotation(Angulo+90);
  app.draw(anim.sprite);
  CircleShape circulo(R);
  circulo.setFillColor(Color(255,0,0,170));
  //Color color(255,0,0,170);
  circulo.setPosition(x,y);
  circulo.setOrigin(R,R);
  //circulo.setRotation(radius*y/x);
  //app.draw(circle);
}

virtual ~Enti(){};
};


class Asteroide: public Enti
{
public:
  Asteroide()
  {
    //dx=radius()*Angulo-dy;
    //dy=radius()*Angulo-dx;
    dx=rand()%8-4;
    dy=rand()%8-4;
    name="Asteroide";
  }

void  Actualizar()
  {
   x+=dx;
   y+=dy;

   /*
   switch(x!>Largo && y!>Ancho)
   case x:
       Largo-=x;
       x=0;
       break;

       case y;
       Alto-=y;
       y=0;
       break;
   */
   if (x>Largo) x=0;  if (x<0) x=Largo;
   if (y>Alto) y=0;  if (y<0) y=Alto;
  }

};



class bala: public Enti
{
public:
  bala()
  {
    name="bala";
  }

void  Actualizar()
  {
   dx=cos(Angulo*Numero)*6;
   dy=sin(Angulo*Numero)*6;
  // angle+=rand()%6-3;
   x+=dx;
   y+=dy;

   if (x>Largo || x<0 || y>Alto || y<0) life=0;
  }

};


class jugador: public Enti
{
public:
   bool Corte;

   jugador()
   {
     name="jugador";
   }

   void Actualizar()
   {
     if (Corte)
      {

          dx+=cos(Angulo*Numero)*0.2;
        dy+=sin(Angulo*Numero)*0.2; }
     else
      { dx*=0.99;
        dy*=0.99; }

//control de movimiento(X*X+*Y*Y*) RAPIDES DE LA VANE
    int MaximaVelocidad=15;
    float Rapides = sqrt(dx*dx+dy*dy);
    if (Rapides>MaximaVelocidad)
     { dx *= MaximaVelocidad/Rapides;
       dy *= MaximaVelocidad/Rapides; }

    x+=dx;
    y+=dy;

    if (x>Largo) x=0; if (x<0) x=Largo;
    if (y>Alto) y=0; if (y<0) y=Alto;
   }

};


bool Chocar(Enti *a,Enti *b)
{

  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}


int vida=3;

int main()
{
    srand(time(0));

    //Iniciacion de los graficos de la libreria Graphics.hpp
    RenderWindow app(VideoMode(Largo, Alto), "Asteroide!");
    app.setFramerateLimit(60);

    //Textura de las imagenes
    Texture t1,t2,t3,t4,t5,t6,t7,t8,menu;
    menu.loadFromFile("images/menu.jpg");
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    t8.loadFromFile("images/corazon.png");

    menu.setSmooth(true);
    t1.setSmooth(true);
    t2.setSmooth(true);
    t8.setSmooth(true);

    Sprite cocoro(t8);
    Sprite cocoro1(t8);
    Sprite cocoro2(t8);
    Sprite fondo(t2);
    Sprite men(menu);


    //Animacion de las explociones y disparos del jeugo

    Animation sExplosiones(t3, 0,0,256,256, 48, 0.5);
    Animation sRoca(t4, 0,0,64,64, 16, 0.2);
    //Animation sPuntos(t4,0,0,0,900,182,0.5);
    //Animation sVidas(t8,0,0,0,75,18,0.5);
    Animation sRoca_pequena(t6, 0,0,64,64, 16, 0.2);
    Animation sBala(t5, 0,0,32,64, 16, 0.8);
    Animation sJugador(t1, 40,0,40,40, 1, 0);
    Animation bala_go(t1, 40,40,40,40, 1, 0);
    Animation sExplosion_nave(t7, 0,0,192,192, 64, 0.5);
    //Animation sCorazon(t7,182,180,40,40,0,0);




    std::list<Enti*> Entidades;

    for(int i=0;i<15;i++)
    {
      Asteroide *a = new Asteroide();
      a->Configuracion(sRoca, rand()%Largo, rand()%Alto, rand()%360, 25);
      Entidades.push_back(a);

    }

    jugador *p = new jugador();
    p->Configuracion(sJugador,200,200,0,20);
    Entidades.push_back(p);
    //p->Configuracion(sCorazon,200,200,200,200);


       Color color(255, 0, 0);
    Font Pixels; Pixels.loadFromFile("Pixeles.ttf");
    Text texto("Puntos: %i"+puntos,Pixels,25);
    texto.setColor(color);
    texto.setPosition(130,130);




     app.draw(cocoro);
   app.draw(texto);
    //Entidades.push_back(p);
    /////main/////
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
             if (event.key.code == Keyboard::Space)
              {
                bala *b = new bala();
                b->Configuracion(sBala,p->x,p->y,p->Angulo,10);
                Entidades.push_back(b);

              }
        }

        int boton =0;
        //CONTROLES

    if (Keyboard::isKeyPressed(Keyboard::Right)) p->Angulo+=3;
    if (Keyboard::isKeyPressed(Keyboard::Left))  p->Angulo-=3;
    if (Keyboard::isKeyPressed(Keyboard::Up)) p->Corte=true;
   // if (Keyboard::isKeyPressed(Keyboard::Enter)) boton=1;
    //if (Keyboard::isKeyPressed(Keyboard::Down)) p->;
    //
    else p->Corte=false;







// Colicion de quitar imgane del Ase
    for(auto a:Entidades)
     for(auto b:Entidades)
    {
      if (a->name=="Asteroide" && b->name=="bala")
       if ( Chocar(a,b) )
           {
               puntos++;
               printf("%i",puntos);

               //Puntosconta++;
            a->life=false;
            b->life=false;

            Enti *e = new Enti();
            e->Configuracion(sExplosiones,a->x,a->y);
            e->name="explosion";
            Entidades.push_back(e);


            for(int i=0;i<2;i++)
            {
             if (a->R==15) continue;
             Enti *e = new Asteroide();
             e->Configuracion(sRoca_pequena,a->x,a->y,rand()%360,15);
             Entidades.push_back(e);
            }

           }
//Colicion del arteroide con la nave, quitando la nave
      if (a->name=="jugador" && b->name=="Asteroide")
       if ( Chocar(a,b) )
           {
            b->life=false;


            vida--;
            Enti *e = new Enti();
            e->Configuracion(sExplosion_nave,a->x,a->y);
            e->name="explosion";
            Entidades.push_back(e);

            vida--;
            p->Configuracion(sJugador,Largo/2,Alto/2,0,20);
            p->dx=0; p->dy=0;
           }
    }


    if (p->Corte)  p->anim = bala_go;
    else   p->anim = sJugador;


    for(auto e:Entidades)
     if (e->name=="explosion")
      if (e->anim.Fin()) e->life=0;

    if (rand()%150==0)
     {
       Asteroide *a = new Asteroide();
       a->Configuracion(sRoca, 0,rand()%Alto, rand()%360, 25);
       Entidades.push_back(a);
     }

    for(auto i=Entidades.begin();i!=Entidades.end();)
    {
      Enti *e = *i;

      e->Actualizar();
      e->anim.Actualizar();

      if (e->life==false) {i=Entidades.erase(i); delete e;}
      else i++;
    }



    Color color(255, 0, 0);
    Font Pixels; Pixels.loadFromFile("Pixeles.ttf");
    Text texto("Puntos:"+puntos,Pixels,25);
    texto.setColor(color);
    texto.setPosition(700,10);




    //Window.display();


   //////Dibuja los componentes//////



cocoro.setPosition(600,10);
cocoro1.setPosition(550,10);
cocoro2.setPosition(500,10);


 //  if(menu==1){



        app.draw(men);
        //scanf("%d",&boton);
       // app.display();



// app.draw(sCorazon);
// app.draw(sBala);
// app.draw(sJugador);

//while(boton==1){

     app.draw(fondo);



    if (vida==3)
    {
        app.draw(cocoro2);
    }
if (vida==3|vida==2){
     app.draw(cocoro1);
}
if(vida==3|vida==2|vida==1){
    app.draw(cocoro);

/*
       Color color(255, 0, 0);
    Font Pixels; Pixels.loadFromFile("Pixeles.ttf");
    Text perder("Haz perdido",Pixels,100);
    perder.setColor(color);
    perder.setPosition(600,400);

    app.draw(perder);
*/
}


   app.draw(texto);

   for(auto i:Entidades)
     i->draw(app);
app.display();
//}


   // }


   }

    return 0;
}
