#include <iostream>
#include "AgendaUI.hpp"
#include <csignal>

using namespace std;

AgendaUI ui;
void signalHandler(int signum) {
    cout << "\n[saving] [waiting......]\n" << endl;
    ui.quitAgenda();
    cout << "[saving] success!" << endl;
	exit(signum);
}

int main() {
	signal(SIGINT, signalHandler);	
	ui.OperationLoop();
	return 0;
}