#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPS (1e-12)


//struktura w ktorej zapisujemy dane naszych dzialan na kartkach
typedef struct kartka
{
  int ktore;// 0 to prostokat, 1 to kolo i 2 to zgiecie
  union
  {
    struct
    {
      double x1, x2, y1, y2;
    } prostokat;
    struct
    {
      double x, y, r;
    } kolo;
    struct
    {
      double k, x1, x2, y1, y2;
    } zgiecie;
  }dzialanie;
}kartka;


//sprawdzamy, czy liczby sa rowne
bool czy_rowne(double x, double y)
{
  if (fabs(x - y) < EPS) return true;
  return false;
}


// sprawdzamy, czy punkt lezy po prawej stronie prostej
bool czy_po_prawej (double x, double y, kartka prosta)
{
  if (czy_rowne(prosta.dzialanie.zgiecie.x1, prosta.dzialanie.zgiecie.x2))
  {
    if (prosta.dzialanie.zgiecie.y1 > prosta.dzialanie.zgiecie.y2 &&
        x < prosta.dzialanie.zgiecie.x1) return true;
    else if (prosta.dzialanie.zgiecie.y1 < prosta.dzialanie.zgiecie.y2 &&
             x > prosta.dzialanie.zgiecie.x1) return true;
  }
  else if (czy_rowne(prosta.dzialanie.zgiecie.y1, prosta.dzialanie.zgiecie.y2))
  {
    if (prosta.dzialanie.zgiecie.x1 < prosta.dzialanie.zgiecie.x2 &&
        y < prosta.dzialanie.zgiecie.y1) return true;
    else if (prosta.dzialanie.zgiecie.x1 > prosta.dzialanie.zgiecie.x2 &&
             y > prosta.dzialanie.zgiecie.y1) return true;
  }
  else
  {
    double a = (prosta.dzialanie.zgiecie.y1 - prosta.dzialanie.zgiecie.y2) /
               (prosta.dzialanie.zgiecie.x1 - prosta.dzialanie.zgiecie.x2);
    double b = prosta.dzialanie.zgiecie.y1 - a * prosta.dzialanie.zgiecie.x1;
    if (y - a * x < b && prosta.dzialanie.zgiecie.x2 > prosta.dzialanie.zgiecie.x1) return true;
    else if (y - a * x > b && prosta.dzialanie.zgiecie.x2 < prosta.dzialanie.zgiecie.x1) return true;
  }
  return false;
}


//wyznaczamy x-owa wspolrzedna punktu po odbiciu przez prosta
double odbiciex (double x, double y, kartka prosta)
{
  double a = 0, b = 0, xodp = 0;

  if (czy_rowne(prosta.dzialanie.zgiecie.x1, prosta.dzialanie.zgiecie.x2))
    xodp = 2 * prosta.dzialanie.zgiecie.x1 - x;
  else if (czy_rowne(prosta.dzialanie.zgiecie.y1, prosta.dzialanie.zgiecie.y2))
    xodp = x;
  else
	  {
    a = (prosta.dzialanie.zgiecie.y1 - prosta.dzialanie.zgiecie.y2) /
        (prosta.dzialanie.zgiecie.x1 - prosta.dzialanie.zgiecie.x2);
    b = prosta.dzialanie.zgiecie.y1 - a * prosta.dzialanie.zgiecie.x1;
    double a2 = -1 / a;
    double b2 = y - a2 * x;
    double xod = (b - b2) / (a2 - a);
    xodp = 2 * xod - x;
  }
  return xodp;
}


//analogicznie jak w odbiciex
double odbiciey (double x, double y, kartka prosta)
{
  double a = 0, b = 0, yodp = 0;

  if (czy_rowne(prosta.dzialanie.zgiecie.x1, prosta.dzialanie.zgiecie.x2))
    yodp = y;
  else if (czy_rowne(prosta.dzialanie.zgiecie.y1, prosta.dzialanie.zgiecie.y2))
    yodp = 2 * prosta.dzialanie.zgiecie.y1 - y;
  else
  {
    a = (prosta.dzialanie.zgiecie.y1 - prosta.dzialanie.zgiecie.y2) /
        (prosta.dzialanie.zgiecie.x1 - prosta.dzialanie.zgiecie.x2);
    b = prosta.dzialanie.zgiecie.y1 - a * prosta.dzialanie.zgiecie.x1;
    double a2 = -1 / a;
    double b2 = y - a2 * x;
    double xod = (b - b2) / (a2 - a);
    double yod = a * xod + b;
    yodp = 2 * yod - y;
  }
  return yodp;
}


//sprawdzamy, czy punkt lezy w prostakacie
int czy_w_figurze(double xx, double yy, kartka figura)
{
  if (czy_rowne(figura.ktore, 1))
  {
    if ((xx - figura.dzialanie.kolo.x) * (xx - figura.dzialanie.kolo.x) +
        (yy - figura.dzialanie.kolo.y) * (yy - figura.dzialanie.kolo.y) <
         figura.dzialanie.kolo.r * figura.dzialanie.kolo.r + EPS) return 1;
  }
  else
  {
    if (xx > figura.dzialanie.prostokat.x1 - EPS && xx < figura.dzialanie.prostokat.x2 + EPS &&
        yy > figura.dzialanie.prostokat.y1 - EPS && yy < figura.dzialanie.prostokat.y2 + EPS) return 1;
  }
  return 0;
}


//funkcja wyznaczajaca wszystkie punkty, ktore moga byc przebiciami kartki
//zgiecia to tablica dzialan na kartkach
int ile_przebic(unsigned int kk, double xx, double yy, kartka* zgiecia,  int odpowiedz)
{
  if (!czy_rowne(zgiecia[kk - 1].ktore, 2))
  {
    odpowiedz += czy_w_figurze(xx, yy, zgiecia[kk - 1]);
  }
  else
  {
    double nowyx = odbiciex (xx, yy, zgiecia[kk - 1]);
    double nowyy = odbiciey (xx, yy, zgiecia[kk - 1]);
    unsigned int nowe = (unsigned int) zgiecia[kk - 1].dzialanie.zgiecie.k;

    //jesli punkt lezy po prawej stronie nowego zgiecia, to wtedy nie zlozy nam sie w to miejsce
    if (!czy_po_prawej(xx, yy, zgiecia[kk - 1]))
    {
      if (czy_rowne(nowyx, xx) && czy_rowne(nowyy, yy))
        odpowiedz = ile_przebic(nowe, xx, yy, zgiecia, odpowiedz);
      else
      {
        odpowiedz = ile_przebic(nowe, xx, yy, zgiecia, odpowiedz);
        odpowiedz = ile_przebic(nowe, nowyx, nowyy, zgiecia, odpowiedz);
      }
    }
  }
  return odpowiedz;
}


//funckja do wczytywania kolejnych operacji P/K/Z
void tablica_dzialan(kartka* t, unsigned int n)
{
  for (unsigned int i = 0; i < n; i++)
  {
    char litera = 0;
    litera = (char)getchar();
    if (litera == 'P')
    {
      kartka nowy;

      scanf ("%lf %lf %lf %lf\n", &nowy.dzialanie.prostokat.x1,
             &nowy.dzialanie.prostokat.y1, &nowy.dzialanie.prostokat.x2, &nowy.dzialanie.prostokat.y2);

      nowy.ktore = 0;
      t[i] = nowy;
    }
    else if (litera == 'K')
    {
      kartka nowy;

      scanf ("%lf %lf %lf\n", &nowy.dzialanie.kolo.x, &nowy.dzialanie.kolo.y, &nowy.dzialanie.kolo.r);

      nowy.ktore = 1;
      t[i] = nowy;
    }
    else
    {
      kartka nowy;

      scanf ("%lf %lf %lf %lf %lf\n", &nowy.dzialanie.zgiecie.k, &nowy.dzialanie.zgiecie.x1,
             &nowy.dzialanie.zgiecie.y1, &nowy.dzialanie.zgiecie.x2, &nowy.dzialanie.zgiecie.y2);

      nowy.ktore = 2;
      t[i] = nowy;
    }
  }
  return;
}


int main()
{
  unsigned int n, q;

  scanf ("%u %u\n", &n, &q);

  kartka* dzialania = (kartka*) malloc(sizeof(kartka) * n);

  tablica_dzialan(dzialania, n);

  for (unsigned int i = 0; i < q; i++)
  {
    double xx, yy;
    unsigned int kk;

    scanf ("%u %lf %lf", &kk, &xx, &yy);

    int odp = 0;

    odp = ile_przebic(kk, xx, yy, dzialania, 0);

    printf("%d\n", odp);

  }

  //uwalniamy pamiec po zakonczeniu zadania
  free(dzialania);

  return 0;
}

