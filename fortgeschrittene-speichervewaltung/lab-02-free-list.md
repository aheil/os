# Lab 02: Free List

## Voraussetzungen

Nutzen Sie wenn möglich [gcc](https://gcc.gnu.org/).

Sie können _gcc_ unter Linux und/oder macOS direkt nutzen. Unter Linux installieren Sie _gcc_ über Ihren Paket-Manager, unter macOX über [Homebrew](https://brew.sh/).

Nutzen Sie Windows 10 können Sie entweder eine Linux in einer virtuellen Maschine (z.B. in [VirtualBox](https://www.virtualbox.org/)) nutzen, oder direkt das [Windows Subsystem for Linux (WSL 2.0)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) nutzen.

In der Wahl des Linux Systems sind sie frei, alle Beispiele in der Vorlesung werden unter Ubuntu (letzter stabiler Release) vorgestellt.

## Aufgabenstellung

1. Implementieren Sie eine einfache Datenstruktur, die Ihren Hauptspeicher repräsentiert. Ihr Hauptspeicher hat eine Größe von 1000.
2. Implementieren Sie eine Datenstruktur Free List, die jeweils die Position und die Größe freier Datenblöcke repräsentiert.
3. Auf Basis Ihres Codes wird ein Zufallsgenerator gestartet, der willkürlich Speicherblöcke anfordert und wieder freigibt. Bei jeder Änderung ist die Linked List zu aktualisieren, so dass darin alle freien Speicherbereiche enthalten sind.
4. Nutzen Sie eine Header-Datei (`freelist.h`) der Form:

```
int position(int size);
void insert(int position, int size);
void free_mem(int position);
void print_freelist();
```

5. Die Funktion `position()` liefert den die erste Position an der die geforderte Größe eingefügt werden kann. Wird keine passender Speicherblock gefunden liefert die Funktion den Rückgabewert -1.
6. Jedes Mal wenn ein Speicherbereich mit der Funktion `insert()` "reserviert" wird, schreiben Sie entsprechende Daten in Ihre Datenstruktur. Wird Speicher mittels der Funktion `free_mem()` freigegeben, werden die Daten gelöscht.
7. Die Funktion `print_freelist()` gibt den aktuellen Zustand Ihrer Free List auf der Konsole (stdout) aus. Implementieren Sie entsprechende Ausgaben auf der Konsole, so das der jeweilige Status der Free List erkennbar ist. Orientieren Sie sich hierbei an der Darstellung der Free List aus der Vorlesung.
8. Ihr Programm wird mind. 100 Iterationen durchlaufen.
9. Hinweis: Ihre Datei(en) dürfen keine `main()`-Methode enthalten. Sollten Sie dennoch eine `main()`-Methode erstellen wollen, tun Sie dies bitte nur in einer Datei **main.c**.



\
Abgabe
------

Die Bewertung Ihrer Abgabe findet automatisch statt. Stellen Sie hierzu folgende Punkte sicher:

* Ihre Lösung befindet sich im Ordner **aufgabe2**.
* Ihre Implementierung befindet sich in einer Datei mit dem Namen **freelist.c**.
* Sie nutzen eine Header-Datei in der die obigen Prototypen verwendet werden.
* Zur Abgabe erhalten Sie einen Zugang zum hochschulinternen [GitLab](https://git.it.hs-heilbronn.de/).
* Ihre Lösung checken Sie in Ihrem Repository ein.

## Bewertung

* Die Bewertung Ihrer Aufgabe findet anhand einer Reihe von Tests statt.
* Ihr Implementierung wird einer Reihe von Tests unterzogen, die Ihre Implementierung auf Korrektheit überprüfen.
* Abgaben, die nicht vollständig sind oder die Abgabekriterien nicht erfüllen, werden nicht bewertet.
* Abgaben, die nicht fristgerecht eingereicht werden, werden nicht bewertet.
* Nutzen Sie zur Abgabe ausschließlich die beschriebene Möglichkeit. Abgaben, die per E-Mail oder anderen Wegen eingereicht werden, werden nicht bewertet.
* Abgaben, die aufgrund eines Fehlers nicht durch die Tests laufen werden entsprechend mit weniger Punkten bewertet.
