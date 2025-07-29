#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios termios_config;

    // Dateideskriptor für STDIN
    int fd = STDIN_FILENO; // Normalerweise 0

    // Aktuelle Terminal-Einstellungen lesen
    if (tcgetattr(fd, &termios_config) == -1) {
        perror("tcgetattr");
        return 1;
    }

    // Terminal in den Raw-Modus versetzen
    termios_config.c_lflag &= ~(ICANON | ECHO); // Kanonischen Modus und Echo ausschalten
    termios_config.c_cc[VMIN] = 5;  // Lesen nach einem Zeichen
    termios_config.c_cc[VTIME] = 0; // Kein Timeout

    // Änderungen sofort anwenden
    if (tcsetattr(fd, TCSANOW, &termios_config) == -1) {
        perror("tcsetattr");
        return 1;
    }

    printf("Terminal im Raw-Modus. Drücke eine Taste, um zu beenden.\n");

    // Warten auf ein Zeichen (blockierend)
    char c[5];
    read(fd, &c, 5);

    printf("Du hast '%s' gedrückt.\n", c);

    // Ursprüngliche Einstellungen wiederherstellen
    if (tcgetattr(fd, &termios_config) == -1) {
        perror("tcgetattr");
        return 1;
    }

    // Terminal zurücksetzen (z. B. Kanonischen Modus wieder aktivieren)
    termios_config.c_lflag |= (ICANON | ECHO);

    if (tcsetattr(fd, TCSANOW, &termios_config) == -1) {
        perror("tcsetattr");
        return 1;
    }

    printf("Terminal-Einstellungen wiederhergestellt.\n");
    return 0;
}