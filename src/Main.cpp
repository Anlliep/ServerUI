#include <iostream>

#include "serverUI.h"

int main() {
  try {
    myUI ui;
    ui.SetIsServerActive();
    ui.SetStatuses();
    ui.Run();
  } catch (const std::exception& ex) {
    std::cout << "There was an error:\n" << ex.what() << std::endl;
  }
  return 0;
}
