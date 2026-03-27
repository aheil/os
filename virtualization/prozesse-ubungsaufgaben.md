# Prozesse: Übungsaufgaben

## **Aufgaben**

**Ziel**: Du lernst den grundlegenden Umgang mit Prozessen unter Linux/Unix.

## Aufgabe 1: Prozesse erzeugen und beobachten&#x20;

Starte in einem Terminal einen Prozess, der längere Zeit läuft. Möglich ist dies z.B: mit dem Befehl `sleep`.

```
sleep 120 &
```

Notiere die ausgegebene PID und beobachte den Prozess mit:

* `ps -f`
* `pstree`
* `top`

## Aufgabe 2: Prozesse in den Hintergrund schicken

Starte einen Prozess im Vordergrund:

```
ping 8.8.8.8
```

Pausiere nun den Prozess mit `CTRL-Z` bzw. `STRG-Z` .

Schicke den Prozess nun in den Hintergrund:&#x20;

```
bg
```

Hole den Prozess wieder in den Vordergrund:

```
fg
```

**Was macht** `CTRL+Z` in der Konsole?

`TRL+Z` **sendet das Signal** `SIGTSTP` an den aktuell laufenden Vordergrundprozess.

Das bewirkt:

1. Der Prozess wird angehalten (gestoppt)\
   Er läuft nicht weiter, sondern friert ein — aber der **Prozess wird nicht beendet.**
2.  Der Prozess wird in den Hintergrund verschoben\
    Die Shell meldet typischerweise:<br>

    ```
    [1]+ Stopped ping 8.8.8.8
    ```

&#x20;      Damit wird er zu einem **Job**, der im Hintergrund „wartet“.

3. Prüfe dies über den Befehl `jobs` . Du solltest die gleiche Ausgabe wie zuvor erhalten.&#x20;
4.  Starte nun einen weiteren Prozess `sleep 120 &` - führe nochmals den Befehl `jobs` aus.\
    Du solltest nun eine ähnliche Ausgabe wie folgt sehen:<br>

    ```
    [1]+  Stopped                 ping 8.8.8.8
    [2]-  Running                 sleep 120 &
    ```

Der Befehl jobs zeigt dir demnach alle laufenden und gestoppten Prozesse, die aus deiner Konsole (Shell) gestartet wurden. Ein Job ist dabei kein Prozess, sondern kann mehrer Prozesse enthalten, die von der Shell verwaltet werden. Die Nummer in Klammern gibt dabei die Shell-interne Job-Nummer an - **dabei handelt es sich** **nicht um die Prozess-ID** (pid).

## Aufgabe 3: Signale senden

Starte einen Prozess:&#x20;

```
ping 8.8.8.8
```

1. Beende den Prozess mit `CTRL-C` bzw. `STRG-C` (SIGINT).
2.  Starte den Prozess erneut und beende ihn kontrolliert:<br>

    ```
    kill -SIGTERM <PID>
    ```
3.  Erzwinge das Beenden des Prozesses:<br>

    ```
    kill -9 <PID>
    ```

**SIGTERM - „Bitte beende dich.“ (Signal 15)**

* Ein **freundliches**, kontrolliertes Beenden
* Der Prozess bekommt die Chance:
  * Dateien zu schließen
  * Speicher aufzuräumen
  * Child-Prozesse zu beenden
  * Logs zu schreiben
* Programme können SIGTERM **abfangen** und darauf reagieren (z. B. „Beende dich sauber“)
* Die Kurzform ist `kill <PID>`

**SIGKILL (Signal 9) - "Sofort benden. Keine Kompromisse" (Signal 9)**

* Der Prozess wird **sofort** vom Kernel beendet
* Keine Aufräumarbeiten
* Keine Chance, Daten zu speichern
* Kann **nicht abgefangen** oder ignoriert werden
* Der Kernel entfernt den Prozess einfach aus der Prozessliste

Warum solltest Du SIGKILL nur ium Notfall nutzen?:

* Es kann **Datenverlust** verursachen kann (z. B. ungeschriebene Dateien, halbfertige Logs)
* Es kann **inkonsistente Zustände** hinterlassen kann (z. B. Lockfiles, offene Sockets)
* Es kann **Kindprozesse verwaisen** lassen kann
* Das Programm hat **keine Chance für einen sauberen Shutdown**
* Debugging erschwert (man sieht nicht, _warum_ der Prozess hängt)

## Aufgabe 4: Zombie Prozesse

Erstelle ein C-Programm:

```c
#include <unistd.h>
#include <stdlib.h>

int main() {
    if (fork() == 0) {
        exit(0); // Kindprozess beendet sich sofort
    }
    sleep(60); // Elternprozess wartet NICHT auf das Kind
    return 0;
}

```

Kompiliere das Programm wie folgt:&#x20;

```
gcc zombie.c -o zombie
```

Starte das Programm mit

```
./zombie &
```

Beobachte das Programm mit&#x20;

```
ps -l
```

und einmal mit&#x20;

```
top
```

Du solltest nun in der Lage sein, den Prozess als ZOmbie zu identifizieren und diesen auf Basis der vorherigen Übungen zu beenden.&#x20;

Versuche den C-Code zu verstehen - aus welchem Grund entsteht der Zombieprozess?

## Aufgabe 5: Verwaisten Prozess (Orphan-Prozess) erzeugen

Erstelle ein neues Programm `` orphan.c` ``

```c
#include <unistd.h>
#include <stdlib.h>

int main() {
    if (fork() == 0) {
        sleep(30); // Kind lebt länger als Elternprozess
        return 0;
    }
    exit(0); // Elternprozess beendet sich sofort
}
```

Compiliere den Prozess wie zuvor und starte ihn mit&#x20;

```
./orphan &
```

Nutze `ps -l`  und `pstree` um herauszufinden, welcher Prozess nun den verwaisten Prozess "adoptiert".

## Aufgabe 6: Analyse &#x20;

Auf einem Linux System läuft ein Prozess mit 100% CPU Nutzung.&#x20;

1.  Identifizeire den Prozess<br>

    ```
    top
    ```
2.  Finde heraus, welcher Befehl dahintersteckt.<br>

    ```
    ps -fp <PID>
    ```
3.  Beende den Prozess kontrolliert <br>

    ```
    kill <PID>
    ```
4.  Falls ein kontrolliertes Beenden nicht möglich ist, nutze die "Brechstange"<br>

    ```
    kill -9 <PID>
    ```

Der Befehl:

```
ps -fp <PID>
```

bedeutet:

* `ps` → Prozessliste anzeigen
* `-f` → **full format** (zeigt zusätzliche Spalten wie UID, PPID, Startzeit, CMD - also den Befehl auf der Kommandozeile)
* `-p <PID>` → **nur den Prozess mit dieser PID anzeigen**

Damit bekommst du eine **detaillierte Einzelansicht** eines bestimmten Prozesses und kannst heruasfinden, wie der Prozess gestartet wurde.\
\
Beispiel:&#x20;

```
UID          PID    PPID  C STIME TTY          TIME CMD
andreas     6269     392  0 07:27 pts/0    00:00:00 ./zombie
```

Bedeutung der Spalten: <br>

| Spalte    | Bedeutung                                     |
| --------- | --------------------------------------------- |
| **UID**   | Benutzer, dem der Prozess gehört              |
| **PID**   | Prozess‑ID                                    |
| **PPID**  | Parent‑Prozess‑ID                             |
| **C**     | CPU‑Nutzung                                   |
| **STIME** | Startzeit                                     |
| **TTY**   | Terminal, von dem der Prozess gestartet wurde |
| **TIME**  | bisher verbrauchte CPU‑Zeit                   |
| **CMD**   | der vollständige Befehl                       |
