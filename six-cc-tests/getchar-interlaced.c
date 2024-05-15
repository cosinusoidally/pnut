/* input:six-cc-tests/getchar-interlaced.c */
/* Compare stdin with file, output matching prefix */
int main() {
  char c1;
  char c2;
  int f;
  f = fopen("six-cc-tests/getchar-interlaced.c", 0);
  while ((c1 = getchar()) && (c2 = fgetc(f)) && c1 == c2 && c1 != -1) {
    putchar(c1);
  }
  fclose(f);
  return 0;
}
