#include <stdio.h>
#include <stdlib.h>

void putstr(char* s) {
  while (*s) {
    putchar(*s);
    s += 1;
  }
}

void putint_aux(int n) {
  if (n <= -10) putint_aux(n / 10);
  putchar('0' - (n % 10));
}

void putint(int n) {
  if (n < 0) {
    putchar('-');
    putint_aux(n);
  } else {
    putint_aux(-n);
  }
}

typedef enum Direction {
  Up,
  Down,
  Left,
  Right,
} Direction;

// Anonymous
typedef enum {
  North,
  South,
  East,
  West,
} CardinalDirection;

// Typedef of named structure
typedef struct Rectangle {
  int w;
  int h;
} Rectangle;

// Typedef of anonymous structure
typedef struct Point {
  int x;
  int y;
} Point;

// Named structure declaration
typedef struct Shape {
  Point origin;
  Rectangle r;
} Shape;

void f(enum Direction dir, Direction dir2) {
  putstr("Direction: "); putint(dir); putstr(" "); putint(dir2); putchar('\n');
}

void test_enums() {
  Direction up = Up;
  const enum Direction down = Down;
  Direction left = Left, right = Right;

  putstr("# test_enums\n");

  // Enums still work
  f(up, down);
  f(left, right);
}

void test_stack_structs() {
  struct Point points_local[3];
  Point pt2;
  int i;

  putstr("# test_stack_structs\n");

  // Stack-allocated structs
  pt2.x = 15;
  pt2.y = 16;

  putstr("pt2: "); putint(pt2.x); putstr(" "); putint(pt2.y); putchar('\n');

  // Stack-allocated array of struct
  for (i = 0; i < 3; i++) {
    points_local[i].x = i;
    points_local[i].y = i * i;
  }

  for (i = 0; i < 3; i++) {
    putint(points_local[i].x);
    putstr(" ");
    putint(points_local[i].y);
    putchar('\n');
  }
}

void test_heap_structs() {
  // Rectangle can be with and without the struct keyword
  struct Rectangle *r1 = (Rectangle*) malloc(sizeof(struct Rectangle));
  Rectangle *r2 = (Rectangle*) malloc(sizeof(Rectangle));

  putstr("# test_heap_structs\n");

  r1->w = 5;
  r1->h = 6;

  r2->w = 7;
  r2->h = 8;

  putstr("r1: "); putint(r1->h); putstr(" "); putint(r1->w); putchar('\n');
  putstr("r2: "); putint(r2->h); putstr(" "); putint(r2->w); putchar('\n');
}

struct Point points_static[3];
struct Point point_static1;
struct Point point_static2;
void test_static_structs() {
  putstr("# test_static_structs\n");

  point_static1.x = 5;
  point_static1.y = 12;

  putstr("point_static1: "); putint(point_static1.x); putstr(" "); putint(point_static1.y); putchar('\n');
  putstr("point_static2: "); putint(point_static2.x); putstr(" "); putint(point_static2.y); putchar('\n');

  point_static2 = point_static1;

  putstr("point_static1: "); putint(point_static1.x); putstr(" "); putint(point_static1.y); putchar('\n');
  putstr("point_static2: "); putint(point_static2.x); putstr(" "); putint(point_static2.y); putchar('\n');
}

void test_struct_assignment() {
  struct Point pt1;
  struct Point pt2;
  struct Point *pts = (struct Point *) malloc(3 * sizeof(struct Point));
  Shape *shapes = (Shape*) malloc(3 * sizeof(Shape));
  int i;

  putstr("# test_struct_assignment\n");

  pt1.x = 5;
  pt1.y = 6;

  pt2 = pt1;

  putstr("pt1: "); putint(pt1.x); putstr(" "); putint(pt1.y); putchar('\n');
  putstr("pt2: "); putint(pt2.x); putstr(" "); putint(pt2.y); putchar('\n');

  // Test nested structure assignment
  for (i = 0; i < 3; i++) {
    pts[i].x = i;
    pts[i].y = i * i;
  }

  for (i = 0; i < 3; i++) {
    shapes[i].origin = pts[i];
  }

  for (i = 0; i < 3; i++) {
    putint(shapes[i].origin.x);
    putstr(" ");
    putint(shapes[i].origin.y);
    putchar('\n');
  }
}

void test_ptr_arith() {
  int i;
  struct Point *pts = (struct Point *) malloc(3 * sizeof(struct Point));
  struct Point **pts2 = (struct Point *) malloc(3 * sizeof(struct Point*));
  struct Point pts3[3];

  putstr("# test_ptr_arith\n");

  for (i = 0; i < 3; i++) {
    if (pts + i != &pts[i]) {
      putstr("Struct pointer arithmetic failed: ");
      putint((long) (pts + i));
      putstr(" ");
      putint((long) &pts[i]);
      putchar('\n');
      exit(-1);
    }
  }

  // On double indirections
  for (i = 0; i < 3; i++) {
    if (pts2 + i != &pts2[i]) {
      putstr("Struct pointer arithmetic failed: ");
      putint((long) (pts2 + i));
      putstr(" ");
      putint((long) &pts2[i]);
      putchar('\n');
      exit(-1);
    }
  }

  // On arrays
  for (i = 0; i < 3; i++) {
    if (pts3 + i != &pts3[i]) {
      putstr("Struct pointer arithmetic failed: ");
      putint((long) (pts3 + i));
      putstr(" ");
      putint((long) &pts3[i]);
      putchar('\n');
      exit(-1);
    }
  }
}

void test_nested_structs() {
  Shape *shapes = (Shape*) malloc(3 * sizeof(Shape));
  int i;

  putstr("# test_nested_structs\n");

  // Test nested structures
  for (i = 0; i < 3; i++) {
    shapes[i].origin.x = i + 2;
    shapes[i].origin.y = (i + 2) * (i + 2);
    shapes[i].r.w = i;
    shapes[i].r.h = i * i;
  }

  for (i = 0; i < 3; i++) {
    putint(shapes[i].origin.x);
    putstr(" ");
    putint(shapes[i].origin.y);
    putstr(" ");
    putint(shapes[i].r.w);
    putstr(" ");
    putint(shapes[i].r.h);
    putchar('\n');
  }
}

void pass_as_value(Point pt) {
  putstr("pass_as_value: Point: "); putint(pt.x); putstr(" "); putint(pt.y); putchar('\n');
  pt.x = 123;
  pt.y = 456;
  putstr("pass_as_value: Point: "); putint(pt.x); putstr(" "); putint(pt.y); putchar('\n');
}

void pass_as_ref(Point *pt) {
  putstr("pass_as_ref: Point: "); putint(pt->x); putstr(" "); putint(pt->y); putchar('\n');
  pt->x = 123;
  pt->y = 456;
  putstr("pass_as_ref: Point: "); putint(pt->x); putstr(" "); putint(pt->y); putchar('\n');
}

void test_passing_as_value() {
  Point pt;
  pt.x = 5;
  pt.y = 6;
  Shape *shape_heap = (Shape*) malloc(sizeof(Shape));
  Shape shape_stack;

  // And passing a struct as value. It should not change the original struct
  putstr("# test_passing_as_value\n");

  putstr("pt: "); putint(pt.x); putstr(" "); putint(pt.y); putchar('\n');
  pass_as_value(pt);
  putstr("pt: "); putint(pt.x); putstr(" "); putint(pt.y); putchar('\n');
  pass_as_ref(&pt);
  putstr("pt after pass_as_ref: "); putint(pt.x); putstr(" "); putint(pt.y); putchar('\n');

  shape_stack.origin.x = 5;
  shape_stack.origin.y = 6;
  shape_heap->origin.x = 7;
  shape_heap->origin.y = 8;

  putstr("shape_stack: "); putint(shape_stack.origin.x); putstr(" "); putint(shape_stack.origin.y); putchar('\n');
  pass_as_value(shape_stack.origin);
  putstr("shape_stack: "); putint(shape_stack.origin.x); putstr(" "); putint(shape_stack.origin.y); putchar('\n');
  pass_as_ref(&shape_stack.origin);
  putstr("shape_stack after pass_as_ref: "); putint(shape_stack.origin.x); putstr(" "); putint(shape_stack.origin.y); putchar('\n');
}

void test_casts() {
  void *words = malloc(5 * sizeof(struct Shape));
  struct Shape shape;
  int i;

  putstr("# test_casts\n");

  for (i = 0; i < 5; i++) {
    shape.origin.x = i * 13;
    shape.origin.y = i * 17;
    shape.r.w = i * 19;
    shape.r.h = i * 23;
    *((struct Shape *) words + i) = shape; // Casts on left side
  }

  struct Shape *shapes_arr = (struct Shape *) words; // Cast on right side

  for (i = 0; i < 5; i++) {
    putint(shapes_arr[i].origin.x);
    putstr(" ");
    putint(shapes_arr[i].origin.y);
    putstr(" ");
    putint(shapes_arr[i].r.w);
    putstr(" ");
    putint(shapes_arr[i].r.h);
    putchar('\n');
  }
}

void main() {
  test_enums();
  test_stack_structs();
  test_heap_structs();
  test_static_structs();
  test_struct_assignment();
  test_ptr_arith();
  test_nested_structs();
  test_passing_as_value();
  test_casts();
}
