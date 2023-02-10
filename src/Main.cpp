#include <iostream>

#include "serverUI.h"

int main() {
  myUI ui;
  ui.SetIsServerActive();
  ui.Run();
  return 0;
}
