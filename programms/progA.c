/*
строгое соответствие типов(никакого приведения типов)
в цикле фор увеличение переменной строго на 1
строка формата в scanf и printf должна быть правильной
тело функции определяется при объявлении
разрешён толлько один инклуд #include <stdio.h> из которого доступны только scanf и printf
функция main определяется последней
аргументы main, а так же её тип и возвращаемое значение игнорируются
запрещён преждевременный выход из функции
запрещён switch
запрещены побитовые операции, в том числе и сдвиги
запрещены указатели
printf обязаткельно выводит перевод строки
*/
#include <stdio.h>

long long sqr(char b, char x, long long pull, double z)
{
  return b * b;
}

long long sqr2(char b, char x, long long pull, double z)
{
  return b * b;
}

int main()
{
  char a;
  short b;
  int c = 8;
  long long d = 10;
  float x;
  double y;
  scanf("%lld", &d);
  long long mas[12][12][12][12][12];
  mas[1][2][3][4][5] = sqr(d);
  if (c > 7)
    c = 8;
  else
  {
    c = 7;
  }
  for (int i = 0; i < 100; i++)
  {
    a = 0;
  }
  while (c > 0)
  {
    c -= 3;
  }
  do
  {
    c -= 3;
  } while (c > -1);
  return 0;
}