---
description: >-
  Diese Übung verbindet die theoretischen Konzepte der Speicherverwaltung –
  Segmentierung, Paging, Page Tables, Fragmentierung, Free Lists, Copy‑on‑Write
  mit praktischen Linux‑Konsolenübungen.
---

# Fortg. Speicherverwaltung: Übungsaufgaben

Alle Aufgaben sind so gestaltet, dass sie ohne Root‑Zugriff (außer pagemap‑Lesen) und ohne spezielle Kernelmodule lösbar sind.

## Übung 1: Speicherbelegung & Fragmentierung sichtbar machen

#### Lernziel

Verstehen, wie Heap‑Wachstum, Allokationsmuster und Fragmentierung im User-Space entstehen.

#### Aufgabe

Schreibe ein C‑Programm, das:

* wiederholt Speicherblöcke unterschiedlicher Größe mit `malloc()` anfordert
* einige dieser Blöcke wieder freigibt
* nach jedem Schritt die aktuelle Speicherbelegung über `/proc/self/maps` und `/proc/self/smaps` ausliest
* die Ergebnisse protokolliert

#### Hinweise

Kompilieren:

Code

```bash
gcc -O0 -g frag.c -o frag
```

Beobachten:

Code

```bash
cat /proc/<PID>/maps
cat /proc/<PID>/smaps | grep -E "Size|Rss|Pss"
pmap <PID>
```

#### Erwartete Erkenntnisse

* Der Heap wächst nicht bei jeder Allokation.
* Fragmentierung entsteht durch freie Lücken zwischen Blöcken.
* `sbrk()`‑ähnliches Verhalten wird sichtbar.

#### **Hinweis**

Hier die Bedeutung der einzelnen Paramter, ie beim Aufruf von `gcc` in dieser und dne folgenden Übungen verwendet werden:&#x20;

* **-O0** - _keine Optimierung_. Wichtig für Lehrzwecke, weil der Compiler sonst Code umsortiert oder entfernt, was Speicherverhalten verfälschen würde.
* **-g** - Debug‑Symbole einfügen. Damit kannst du das Programm mit `gdb`, `pmap`, `perf` usw. sauber analysieren.
* **frag.c** - die Eingabedatei.
* **-o frag** - Name der erzeugten ausführbaren Datei.

## Übung 2: Virtuelle → physikalische Adressen untersuchen

#### Lernziel

Paging und Address Translation praktisch nachvollziehen.

#### Aufgabe

Schreibe ein C‑Programm, das:

* ein großes Array (z. B. 200 MB) allokiert
* auf jedes 4096. Byte zugreift
* die virtuelle Adresse eines Elements ausgibt
* die physikalische Adresse über `/proc/<pid>/pagemap` bestimmt

#### Hinweise

Kompilieren:

Code

```bash
gcc -O0 -g pagemap.c -o pagemap
```

PFN auslesen:

Code

```bash
sudo cat /proc/<PID>/pagemap | hexdump
```

Physikalische Adresse berechnen:

Code

```c
phys_addr = PFN * 4096 + offset
```

#### Erwartete Erkenntnisse

* Virtuelle Adressen sind nicht linear im physikalischen Speicher.
* Page Frames werden dynamisch zugeordnet.
* `pagemap` zeigt die tatsächliche PFN.

#### Tipp:&#x20;

Wie kann das Array erzeugt werden? 200 MB muss man nicht im Kopf ausrechnen, sondern kann dies direkt im Code erledigen:&#x20;

```c
    size_t size = 200 * 1024 * 1024;   // 200 MB
    char *arr = malloc
```

Der Stack ist unter Linux typischerweise 8MB groß. Das 200 MB Array sollte daher tatsächlich auf dem Heap dynamisch allokiert werden.&#x20;

Führe unter Linux folgenden Befehl aus um dies zu überprüfen:

```bash
cat /proc/$$/limits | grep -i stack
```

Due solltest eine Ausgabe der Art&#x20;

```
Max stack size    8388608    unlimited    bytes
```

erhalten.&#x20;

In deinem Fall kann man daraus ablesen:

* **Soft Limit: 8388608 Bytes:** Das ist **8 MB** → Das ist der tatsächlich wirksame Stack‑Grenzwert für diesen Prozess.
* **Hard Limit: unlimited:** → Der Benutzer _dürfte theoretisch_ das Soft Limit erhöhen, weil das Hard Limit keine Obergrenze setzt.

erhalten.&#x20;

## Übung 3: Copy‑on‑Write (COW) sichtbar machen

#### Lernziel

Verstehen, wie Prozesse Speicher teilen und wann Kopien erzeugt werden.

#### Aufgabe

Schreiben Sie ein C‑Programm, das:

* ein großes Array allokiert
* `fork()` aufruft
* im Kindprozess ein einzelnes Byte verändert
* vor und nach dem Schreibzugriff die PFNs vergleicht

#### Hinweise

Beobachten:

Code

```bash
pmap <PID>
sudo cat /proc/<PID>/pagemap
watch -n 0.5 "sudo cat /proc/<PID>/pagemap"
```

#### Erwartete Erkenntnisse

* Vor dem Schreiben teilen Eltern‑ und Kindprozess dieselben PFNs.
* Nach dem Schreiben entstehen neue PFNs - dies wird als _Copy‑on‑Write_ bezeichnet.

#### Hinweis

Wenn ein Prozess unter Linux `fork()` aufruft, passiert **nicht** das, was man iuntuitiv denkt, dass der komplette Prozess kopiert wird.

> Es wird _nicht_ der komplette Speicher kopiert.

Stattdessen nutzt Linux ein extrem effizientes Verfahren: Copy‑on‑Write (COW)

* Eltern‑ und Kindprozess **teilen sich denselben Speicherbereich** (also dieselben physikalischen Pages)
* Beide Prozesse sehen _identische virtuelle Adressen_
* Der Kernel markiert die Pages als **read‑only** und **shared**
* Erst wenn einer der Prozesse **schreibt**, wird die betroffene Page kopiert

Das spart:

* Zeit
* Speicher
* Page‑Faults
* CPU‑Last

Tipp:

Falls Du nicht sicher bist, kannst Du kannst folgendes Code Snippet verwenden um die Aufgabe zu lösen.&#x20;

```c
  // Beispiel: 1 MB allokieren
    size_t size = 1024 * 1024;
    char *arr = malloc(size);

    // Initialisierung, damit Pages wirklich angelegt werden
    arr[0] = 1;

    pid_t pid = fork();

    if (pid == 0) {
        // Kindprozess
        arr[0] = 42;   // EIN Byte ändern → löst Copy-on-Write aus
        printf("Kind: Byte geändert.\n");
        sleep(5);      // Zeit zum Beobachten
    } else {
        // Elternprozess
        printf("Elternprozess wartet...\n");
        sleep(10);
    }
```

Warum die Zeile `pid == 0` ? Hierfür lohnt sich ein Blick auf die Rückgabewerte von _fork()_

| Kindprozess   | **0**                            | „Ich bin das Kind“                                           |
| ------------- | -------------------------------- | ------------------------------------------------------------ |
| Elternprozess | **PID des Kindes** (z. B. 12345) | „Ich bin der Elternprozess, und mein Kind hat die PID 12345“ |
| Fehler        | -1                               | fork() ist fehlgeschlagen                                    |

Der Abschnitt&#x20;

```c
pid_t pid = fork();

if (pid == 0) {
    // Kindprozess
} else ...
```

bedeutet demnach führe diesen Block **nur im Kindprozess** aus. Der Elternprozess geht in den `else`‑Teil.

Warum ist das so implementiert?

Weil `fork()` **den kompletten Prozess dupliziert**:

* gleicher Code
* gleiche Daten
* gleiche virtuelle Adressen
* gleicher Program Counter (PC)

→ aber **zwei getrennte Prozesse**.

Damit beide wissen, _wer sie sind_, gibt `fork()` unterschiedliche Werte zurück.\
\
Gleicher Program Counter“ – wie kann das sein, wenn jeder Prozess seinen eigenen hat wie wir es in der Vorlesung gelernt haben?

Kurzfassung:

> **Ja, jeder Prozess hat seinen eigenen Program Counter (PC).** Aber **direkt nach** `fork()` haben beide Prozesse **denselben PC‑Wert**, weil sie **an exakt derselben Stelle im Code weiterlaufen**.

Das ist kein Widerspruch sondern genau der Trick von `fork()`.

## Übung 4: Swapping & Page Faults provozieren

#### Lernziel

Verstehen, wie das System reagiert, wenn mehr Speicher benötigt wird als verfügbar ist.

#### Aufgabe

Schreibe ein Programm, das:

* mehr Speicher allokiert als RAM verfügbar ist - nutze die Variante aus der Aufgabe zuvor für die Berechnung
* den Speicher sequenziell also fortlaufend beschreibt
* die Laufzeit misst

#### Hinweise

Parallel beobachten:

```bash
vmstat 1
dmesg | grep -i oom
free -h
```

#### Erwartete Erkenntnisse

* `vmstat` zeigt steigende Page‑Fault‑Raten.
* Swap‑Verbrauch steigt sichtbar.
* Der OOM‑Killer kann Prozesse beenden.

#### Hinweis

Der **OOM‑Killer** („Out Of Memory Killer“) ist ein Sicherheitsmechanismus des Linux‑Kernels. Er springt ein, **wenn der physikalische RAM + Swap komplett voll sind** und das System sonst stehenbleiben würde.

Normalerweise versucht Linux:

1. RAM zu nutzen
2. ungenutzte Pages zu droppen
3. Pages in den Swap auszulagern
4. Page Cache zu verkleinern

Wenn **alles voll** ist und ein Prozess trotzdem mehr Speicher anfordert, steht der Kernel vor einem Problem:

> „Ich kann die Anforderung nicht erfüllen - aber ich darf auch nicht einfach stehenbleiben.“

Ein System ohne freien Speicher würde **komplett einfrieren**. Dann greift der OOM‑Killer ein. Der Kernel entscheidet:

> „Ich muss einen Prozess beenden, um Speicher freizugeben.“

Er sucht sich den „besten“ Kandidaten — also den Prozess, der:

* viel Speicher verbraucht
* wenig wichtig erscheint
* eine hohe OOM‑Score‑Bewertung hat

und **tötet ihn sofort**.

Windows verwendet übrigens kein OOM-Killer sondern vergibt schlicht keinen Speicher mehr. Viele Programme, die mehr Speicher anfordern obwohl kein Speicher mehr verfügbar ist stürzen dadurch ab - Windows killt die Prozesse allerdings nicht aktiv. Das kann den Eindruck erwecken, dass Windows nicht so "zuverlässig" sei - dabei versucht Windows vorher verschieden Maßnahmen um Speicher freizugeben, indem es Pages trimmet, sog. Working Sets (also die Teile des virtuellen Speichers eines Prozesses, der aktuell im RAM liegt) verkleinern, Pagefile zu nutzt und Cache reduziert. Als Folge wird Windows langsam, läuft aber stabil weiter.&#x20;

**Linux:**

* Hat einen echten OOM‑Killer
* Kernel entscheidet aktiv, welchen Prozess es tötet
* Ziel: System am Leben halten

**Windows:**

* Hat _keinen_ OOM‑Killer
* Verhindert Allokationen, komprimiert Speicher, reduziert Working Sets
* Programme crashen eher selbst
* System kann einfrieren, aber Windows „opfert“ nicht gezielt Prozesse

## &#x20;Übung 5: Free‑List‑Allocator erweitern

#### Lernziel

Verstehen, wie Speicherverwaltung ohne Paging funktioniert.

#### Aufgabe

Erweitere deinen Userspace‑Allocator aus der Hausaufgabe, der Speicherblöcke in einer Free List verwaltet und nun folgende Strategien unterstützen soll:

* First Fit (also den ersten Speicherplatz nehmen, der groß genug ist)
* Best Fit (den Speicherbereich nehmen, der am besten passt)
* Worst Fit (den Speicherbereich nehmen, der am schlechtesten passt)&#x20;

und für die einzelnen Strategien die&#x20;

* Fragmentierung misst (z. B. Anzahl Lücken, größte Lücke, kleinste Lücke)

#### Hinweise

Kompilieren:

```
gcc freelist.c -o freelist
```

Teste das Programm und die versch. Strategien mit zufälligen Allokationsmustern.

#### Erwartete Erkenntnisse

* Unterschiedliche Strategien erzeugen unterschiedliche Fragmentierung.
* Free‑List‑Verwaltung ist fehleranfällig.
* Paging in Betriebssystemen löst viele dieser Probleme, die in dem Versuch auftreten können.
