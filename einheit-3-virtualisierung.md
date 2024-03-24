# Einheit 3: Virtualisierung

## Lernziele und Kompetenzen

* **Verstehen** wie sich Prozesse zusammensetzen und Prozesse vom Betriebssystem verwaltet werden.
* **Verstehen** wie Prozesse im Betriebssystem gesteuert werden.

## Prozesse

**Was ist eigentlich ein Prozess?**

Einfache Antwort: Ein ausgeführtes bzw. laufendes Programm.

**Was ist dann überhaupt ein Programm?**

Es Besteht aus **Code** (Bits) und ggf. **statischen Daten,** wartet auf der Festplatte und tut eigentlich nichts. Erst durch die Ausführung wird ein Programm zum Prozess.

**Was benötigt ein Programm, um ausgeführt zu werden?**

Es wird **Speicher** benötigt, in den der auszuführende Code und die Daten geladen werden können.  Zur Ausführung außerdem eine **CPU** benötigt.&#x20;

<figure><img src=".gitbook/assets/os.02.taskmng1.png" alt=""><figcaption><p>Windows Task Manager zeigt die laufenden Prozesse</p></figcaption></figure>

In der Regel laufen mehr Prozesse auf dem PC, als CPUs vorhanden sind.&#x20;

**Wie kann die Illusion vieler CPUs geschaffen werden, wenn es nur eine (oder wenige) physikalische CPUs gibt?**\
\
Hierzu betrachten wir, wie Prozesse im Rechner abgelegt sind:&#x20;

* Programmanweisungen bzw. Instruktionen (engl. instructions), also der ausführbaren Maschinencode liegen im Speicher.
* Daten, die im Prozess gelesen oder geschrieben werden, liegen im Speichern.
* Der Prozess muss einen Teil des Speicher daher adressieren können (**adressierbarer Speicher** bzw. engl. address space).
* Die CPU liest die Instruktionen und Daten aus Registern und schreibt in diese Register. Hierzu gehören der **Program Counter** (Abk. PC) oder auch Instruction Counter (Abk. IC): Hier steht die Adresse der nächsten Anweisung, die ausgeführt werden soll.
* Dateien auf die der Prozess zugreift, um Daten daraus zu lesen oder zu schreiben, Liste der Dateien, die der Prozess aktuell geöffnet hat, also eine **Liste der Dateien, die der Prozess aktuell geöffnet hat.**
* Weiter werden Register für I/O-Informationen (Input/Output), Stack Pointer, Frame Pointer, Funktionsparameter, lokale Variablen und Rücksprungadressen (engl. return address) benötigt. - mehr dazu später.&#x20;

> Alle diese Informationen können jederzeit »weggespeichert« und »wiederhergestellt« werden



Übungsaufgaben:&#x20;

Jeder findet heraus, wieviele CPUs bzw. Kerne im eigenen Rechner vorhanden sind.&#x20;
