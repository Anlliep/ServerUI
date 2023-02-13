#include <iostream>

#include "serverUI.h"

int main() {
  myUI ui;
  ui.SetIsServerActive();
  ui.SetStatuses();
  ui.Run();
  return 0;
}
