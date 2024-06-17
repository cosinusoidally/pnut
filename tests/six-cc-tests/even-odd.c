void putnumber(int n) {
  if (n == 0) {
    putchar(48);
    return;
  }
  int acc = 0;
  int i = 0;
  int digits[10];
  while (n > 0) {
    digits[i] = n % 10;
    n = n / 10;
    i = i + 1;
  }
  i = i - 1;
  while (i >= 0) {
    putchar(digits[i] + 48);
    i = i - 1;
  }
}

int abs(int number)
{
  if(number < 0) return -number;
  return number;
}

int even(int number)
{
  int a; /* Local variable so that the function is not simple */
  if(number == 0) return 1;
  return odd(abs(number)-1);
}

int odd(int number)
{
  int a; /* Local variable so that the function is not simple */
  if( number == 0 ) return 0;
  return even(abs(number)-1);
}

void putstring(char *s) {
  while (*s) {
    putchar(*s);
    s = s + 1;
  }
}

int main() {
  int n1;
  int n2;
  n1 = even(10);
  n2 = odd(10);
  putstring("n1 = ");
  putnumber(n1);
  putchar('\n');
  putstring("n2 = ");
  putnumber(n2);
  putchar('\n');
  return 0;
}
