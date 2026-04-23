# Threads: Hausaufgaben

## Übungsaufgaben: Threads in C

> **Lehrveranstaltung:** Betriebssysteme – 1. Semester Informatik\
> **Grundlage:** [prof.aheil.de/os/threads](https://prof.aheil.de/os/threads)\
> **Ziel:** Erste praktische Erfahrungen mit der POSIX-Thread-API (`pthreads`) sammeln

***

### Voraussetzungen

Alle Programme in diesem Dokument verwenden die POSIX-Thread-Bibliothek (`pthreads`). Zum Kompilieren muss die Flag `-lpthread` angegeben werden:

```bash
gcc -o programm programm.c -lpthread
```

> **Hinweis:** Auf manchen Systemen (z.B. neuere Linux-Versionen) ist `-lpthread` bereits implizit enthalten. Es schadet aber nie, es explizit anzugeben.

***

### Übersicht der wichtigsten Funktionen

| Funktion               | Kurzbeschreibung                                     |
| ---------------------- | ---------------------------------------------------- |
| `pthread_create`       | Erzeugt einen neuen Thread                           |
| `pthread_join`         | Wartet auf die Beendigung eines Threads              |
| `pthread_mutex_lock`   | Sperrt einen kritischen Abschnitt (Lock setzen)      |
| `pthread_mutex_unlock` | Gibt einen kritischen Abschnitt frei (Lock aufheben) |
| `pthread_cond_wait`    | Lässt einen Thread warten, bis ein Signal kommt      |
| `pthread_cond_signal`  | Schickt ein Signal an einen wartenden Thread         |

***

### Aufgabe 1: Einen ersten Thread erzeugen

#### Hintergrund

Ein Thread ist ein eigenständiger Ausführungsstrang innerhalb eines Prozesses. Im Gegensatz zu einem vollständigen Prozess teilt sich ein Thread den Adressraum (also Variablen, Heap, etc.) mit allen anderen Threads desselben Prozesses.

Um Threads in C zu erzeugen, wird die Funktion `pthread_create` verwendet.

#### Die Funktion `pthread_create`

```c
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void *),
                   void *arg);
```

**Parameter:**

| Parameter       | Typ                      | Bedeutung                                                                                                                                                                                |
| --------------- | ------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `thread`        | `pthread_t *`            | Zeiger auf eine Variable vom Typ `pthread_t`. Hier speichert das System die ID des neuen Threads. Über diese ID kann später mit dem Thread interagiert werden (z.B. per `pthread_join`). |
| `attr`          | `const pthread_attr_t *` | Optionale Thread-Attribute (z.B. Stack-Größe, Scheduling-Priorität). Für den Einstieg wird hier `NULL` übergeben – das System wählt dann Standardwerte.                                  |
| `start_routine` | `void *(*)(void *)`      | Ein Funktionszeiger (engl. _function pointer_) auf die Funktion, die der Thread ausführen soll. Die Funktion muss die Signatur `void *funktionsname(void *arg)` haben.                   |
| `arg`           | `void *`                 | Ein optionales Argument, das an `start_routine` übergeben wird. Wenn kein Argument benötigt wird, wird `NULL` übergeben.                                                                 |

**Rückgabewert:** `0` bei Erfolg, ein Fehlercode bei Misserfolg.

#### Beispielprogramm

```c
#include <stdio.h>
#include <pthread.h>

/* Diese Funktion wird vom neuen Thread ausgeführt.
   Sie muss immer die Signatur void *name(void *arg) haben. */
void *hallo_thread(void *arg) {
    printf("Hallo aus dem Thread!\n");
    return NULL; /* Threads geben einen void-Zeiger zurück, NULL = kein Rückgabewert */
}

int main() {
    pthread_t thread_id; /* Variable zur Speicherung der Thread-ID */

    /* Thread erzeugen:
       - &thread_id : Wo die Thread-ID gespeichert werden soll
       - NULL       : Standard-Attribute verwenden
       - hallo_thread : Die Funktion, die der Thread ausführen soll
       - NULL       : Kein Argument an die Thread-Funktion übergeben */
    pthread_create(&thread_id, NULL, hallo_thread, NULL);

    /* Kurze Pause im Hauptthread, damit der neue Thread Zeit hat zu laufen.
       ACHTUNG: Das ist keine gute Methode! Besser: pthread_join (siehe Aufgabe 2) */
    sleep(1);

    printf("Hallo aus dem Hauptprogramm!\n");
    return 0;
}
```

**Was passiert hier?**

1. Der Hauptthread (das `main`-Programm) ruft `pthread_create` auf.
2. Das Betriebssystem erzeugt einen neuen Thread, der die Funktion `hallo_thread` ausführt.
3. Beide Threads laufen nun gleichzeitig (oder wechselweise, je nach Scheduler).
4. Die Reihenfolge der Ausgaben ist **nicht garantiert** – das entscheidet der Scheduler.

**Aufgabe:**\
Ändere das Programm so, dass der Thread deinen eigenen Namen ausgibt. Kompiliere und führe das Programm mehrmals aus. Ändert sich die Reihenfolge der Ausgaben?

***

### Aufgabe 2: Auf einen Thread warten mit `pthread_join`

#### Hintergrund

In Aufgabe 1 haben wir `sleep(1)` verwendet, um dem Thread Zeit zu geben. Das ist keine verlässliche Methode. Die richtige Lösung heißt `pthread_join`: Damit wartet der aufrufende Thread so lange, bis der angegebene Thread beendet ist.

#### Die Funktion `pthread_join`

```c
int pthread_join(pthread_t thread, void **value_ptr);
```

**Parameter:**

| Parameter   | Typ         | Bedeutung                                                                                                                                          |
| ----------- | ----------- | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| `thread`    | `pthread_t` | Die ID des Threads, auf den gewartet werden soll. Diese ID wurde beim Aufruf von `pthread_create` gespeichert.                                     |
| `value_ptr` | `void **`   | Zeiger auf einen Zeiger, in dem der Rückgabewert des Threads gespeichert wird. Wird der Rückgabewert nicht benötigt, kann `NULL` übergeben werden. |

**Rückgabewert:** `0` bei Erfolg, ein Fehlercode bei Misserfolg.

#### Beispielprogramm

```c
#include <stdio.h>
#include <pthread.h>

/* Thread-Funktion: gibt eine Nachricht aus und gibt nichts zurück */
void *meine_aufgabe(void *arg) {
    printf("Thread: Ich führe meine Aufgabe durch...\n");
    printf("Thread: Fertig!\n");
    return NULL;
}

int main() {
    pthread_t thread_id; /* Thread-ID */

    printf("Hauptprogramm: Starte Thread...\n");

    /* Thread erzeugen */
    pthread_create(&thread_id, NULL, meine_aufgabe, NULL);

    printf("Hauptprogramm: Warte auf Thread...\n");

    /* Warten bis der Thread fertig ist.
       - thread_id : ID des Threads, auf den gewartet wird
       - NULL      : Rückgabewert des Threads interessiert uns nicht */
    pthread_join(thread_id, NULL);

    printf("Hauptprogramm: Thread ist beendet. Programm wird beendet.\n");
    return 0;
}
```

**Was passiert hier?**

1. Das Hauptprogramm startet den Thread und ruft dann `pthread_join` auf.
2. `pthread_join` blockiert das Hauptprogramm – es läuft erst weiter, wenn `meine_aufgabe` komplett ausgeführt wurde.
3. Die letzten beiden Ausgaben sind jetzt in der richtigen Reihenfolge garantiert.

#### Beispielprogramm: Argument an Thread übergeben

```c
#include <stdio.h>
#include <pthread.h>

/* Thread-Funktion: erwartet einen int-Wert als Argument */
void *zaehle_bis(void *arg) {
    /* Das Argument kommt als void* an und muss umgewandelt (gecastet) werden */
    int ziel = *((int *)arg); /* void* zu int* casten, dann dereferenzieren */

    for (int i = 1; i <= ziel; i++) {
        printf("Thread zählt: %d\n", i);
    }
    return NULL;
}

int main() {
    pthread_t thread_id;
    int zielwert = 5; /* Dieser Wert wird an den Thread übergeben */

    /* Zeiger auf zielwert als Argument übergeben */
    pthread_create(&thread_id, NULL, zaehle_bis, &zielwert);

    /* Auf Thread warten */
    pthread_join(thread_id, NULL);

    printf("Hauptprogramm: Thread hat gezählt.\n");
    return 0;
}
```

**Aufgabe:**\
Erstelle ein Programm mit **zwei Threads**. Übergib jedem Thread eine andere Zahl als Argument. Beide Threads sollen bis zu ihrer jeweiligen Zahl zählen. Verwende `pthread_join`, um auf beide Threads zu warten.

_Tipp: Du benötigst zwei Variablen vom Typ `pthread_t` und rufst `pthread_join` für jede auf._

***

### Aufgabe 3: Race Conditions beobachten

#### Hintergrund

Da Threads auf denselben Speicher zugreifen können, entstehen **Race Conditions** (dt. Wettlaufsituationen): Das Ergebnis eines Programms hängt von der Reihenfolge ab, in der die Threads ausgeführt werden – und diese Reihenfolge wird vom Betriebssystem (Scheduler) bestimmt, nicht vom Programmierer.

Ein klassisches Beispiel ist das gemeinsame Erhöhen eines Zählers.

#### Das Problem: gemeinsamer Zähler ohne Schutz

```c
#include <stdio.h>
#include <pthread.h>

/* Globale Variable, die von beiden Threads geteilt wird */
int zaehler = 0;

/* Thread-Funktion: erhöht den globalen Zähler viele Male */
void *erhoehe_zaehler(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        /* PROBLEM: Diese Operation ist NICHT atomar!
           Sie besteht intern aus drei Schritten:
           1. Wert aus Speicher lesen
           2. Wert um 1 erhöhen
           3. Wert zurückschreiben
           Zwischen diesen Schritten kann ein Context-Switch stattfinden! */
        zaehler = zaehler + 1;
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    /* Zwei Threads starten – beide erhöhen denselben Zähler */
    pthread_create(&thread1, NULL, erhoehe_zaehler, NULL);
    pthread_create(&thread2, NULL, erhoehe_zaehler, NULL);

    /* Auf beide Threads warten */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    /* Erwartetes Ergebnis: 2.000.000
       Tatsächliches Ergebnis: meistens WENIGER – warum? */
    printf("Endwert des Zählers: %d\n", zaehler);
    printf("Erwartet:            2000000\n");

    return 0;
}
```

**Was passiert hier?**

Der Ausdruck `zaehler = zaehler + 1` sieht wie eine einzelne Operation aus, besteht aber intern aus drei Schritten (Lesen → Erhöhen → Schreiben). Wenn zwei Threads diese Schritte gleichzeitig ausführen und ein **Context-Switch** zwischen den Schritten stattfindet, kann es passieren, dass eine Erhöhung "verloren geht":

```
Thread 1 liest Wert:  50
Thread 2 liest Wert:  50   ← liest auch 50, weil Thread 1 noch nicht zurückgeschrieben hat
Thread 1 schreibt:    51
Thread 2 schreibt:    51   ← überschreibt Ergebnis von Thread 1!
Ergebnis: 51 statt 52
```

Dieser Code-Bereich, in dem auf gemeinsame Daten zugegriffen wird, heißt **kritischer Abschnitt** (engl. _critical section_).

**Aufgabe:**\
Führe das Programm mehrmals aus. Ist das Ergebnis jedes Mal dasselbe? Was beobachtest du?

***

### Aufgabe 4: Locks (Mutexe) – kritische Abschnitte schützen

#### Hintergrund

Um Race Conditions zu verhindern, wird **wechselseitiger Ausschluss** (engl. _mutual exclusion_, kurz _Mutex_) eingesetzt. Ein Mutex ist ein "Token": Nur der Thread, der den Mutex besitzt, darf den kritischen Abschnitt betreten. Alle anderen Threads müssen warten, bis der Mutex wieder freigegeben wird.

#### Die wichtigsten Funktionen

**Mutex initialisieren:**

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
```

Oder dynamisch:

```c
pthread_mutex_t lock;
pthread_mutex_init(&lock, NULL);
```

**Mutex sperren:**

```c
int pthread_mutex_lock(pthread_mutex_t *mutex);
```

Wartet, bis der Mutex verfügbar ist, und sperrt ihn dann. Kein anderer Thread kann den Mutex sperren, während er bereits gesperrt ist.

**Mutex freigeben:**

```c
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

Gibt den Mutex wieder frei, sodass ein anderer wartender Thread ihn sperren kann.

#### Beispielprogramm: Zähler mit Mutex geschützt

```c
#include <stdio.h>
#include <pthread.h>

/* Globaler Zähler – wird von beiden Threads gemeinsam genutzt */
int zaehler = 0;

/* Mutex zur Absicherung des kritischen Abschnitts.
   PTHREAD_MUTEX_INITIALIZER ist die einfachste Initialisierung. */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Thread-Funktion: erhöht den Zähler – jetzt mit Mutex geschützt */
void *erhoehe_zaehler(void *arg) {
    for (int i = 0; i < 1000000; i++) {

        /* Kritischen Abschnitt betreten: Lock setzen.
           Falls ein anderer Thread das Lock bereits hält,
           wartet dieser Thread hier, bis es freigegeben wird. */
        pthread_mutex_lock(&lock);

        /* Kritischer Abschnitt: nur ein Thread gleichzeitig hier drin */
        zaehler = zaehler + 1;

        /* Kritischen Abschnitt verlassen: Lock freigeben.
           Ein anderer wartender Thread kann nun eintreten. */
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    /* Zwei Threads starten */
    pthread_create(&thread1, NULL, erhoehe_zaehler, NULL);
    pthread_create(&thread2, NULL, erhoehe_zaehler, NULL);

    /* Auf beide Threads warten */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    /* Jetzt sollte das Ergebnis immer 2.000.000 sein */
    printf("Endwert des Zählers: %d\n", zaehler);
    printf("Erwartet:            2000000\n");

    return 0;
}
```

**Wichtige Regel:** `pthread_mutex_lock` und `pthread_mutex_unlock` müssen **immer paarweise** aufgerufen werden. Ein nicht freigegebener Mutex führt zu einem **Deadlock** – alle Threads warten ewig und das Programm hängt.

**Aufgabe:**\
Vergleiche die Ausgabe dieses Programms mit dem Programm aus Aufgabe 3. Führe beide mehrfach aus. Was stellst du fest?

***

### Aufgabe 5: Rückgabewerte aus Threads

#### Hintergrund

Thread-Funktionen haben den Rückgabetyp `void *`. Damit können Ergebnisse zurückgegeben werden. `pthread_join` ermöglicht es, diesen Rückgabewert entgegenzunehmen.

#### Beispielprogramm

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Thread-Funktion: berechnet die Summe von 1 bis 100
   und gibt das Ergebnis als Zeiger zurück */
void *berechne_summe(void *arg) {
    /* Speicher für das Ergebnis auf dem Heap anlegen.
       Wichtig: NICHT den Stack verwenden (lokale Variable),
       da dieser nach dem Funktionsende nicht mehr gültig ist! */
    int *ergebnis = malloc(sizeof(int));
    *ergebnis = 0;

    for (int i = 1; i <= 100; i++) {
        *ergebnis += i;
    }

    /* Zeiger auf das Ergebnis zurückgeben */
    return (void *)ergebnis;
}

int main() {
    pthread_t thread_id;
    void *rueckgabe; /* Hier wird der Rückgabewert des Threads gespeichert */

    /* Thread starten */
    pthread_create(&thread_id, NULL, berechne_summe, NULL);

    /* Auf Thread warten und Rückgabewert in 'rueckgabe' speichern */
    pthread_join(thread_id, &rueckgabe);

    /* void* in int* umwandeln und Wert ausgeben */
    int *summe = (int *)rueckgabe;
    printf("Die Summe von 1 bis 100 ist: %d\n", *summe);

    /* Heap-Speicher wieder freigeben (vom Thread mit malloc angelegt) */
    free(summe);

    return 0;
}
```

**Aufgabe:**\
Ändere das Programm so, dass der Thread nicht die Summe von 1 bis 100, sondern von 1 bis zu einem Wert berechnet, der per Argument übergeben wird. Gib das Ergebnis im Hauptprogramm aus.

***

### Aufgabe 6: Mehrere Threads mit Argumenten – ein Array verarbeiten

#### Hintergrund

In der Praxis startet man oft mehrere Threads mit unterschiedlichen Argumenten. Um mehrere Werte an einen Thread zu übergeben, kann eine `struct` verwendet werden.

#### Beispielprogramm

```c
#include <stdio.h>
#include <pthread.h>

/* Struktur, um mehrere Argumente an einen Thread zu übergeben */
typedef struct {
    int thread_nummer; /* Nummer des Threads (für die Ausgabe) */
    int start;         /* Startwert des Bereichs */
    int ende;          /* Endwert des Bereichs */
} ThreadArgument;

/* Thread-Funktion: gibt die Zahlen im zugewiesenen Bereich aus */
void *verarbeite_bereich(void *arg) {
    /* Argument von void* in den richtigen Typ umwandeln */
    ThreadArgument *t = (ThreadArgument *)arg;

    printf("Thread %d: Verarbeite Bereich %d bis %d\n",
           t->thread_nummer, t->start, t->ende);

    for (int i = t->start; i <= t->ende; i++) {
        printf("  Thread %d: %d\n", t->thread_nummer, i);
    }

    printf("Thread %d: Fertig.\n", t->thread_nummer);
    return NULL;
}

int main() {
    pthread_t threads[3];          /* Array für 3 Thread-IDs */
    ThreadArgument argumente[3];   /* Array für 3 Argument-Strukturen */

    /* Argumente für jeden Thread festlegen */
    argumente[0].thread_nummer = 1; argumente[0].start = 1;  argumente[0].ende = 5;
    argumente[1].thread_nummer = 2; argumente[1].start = 6;  argumente[1].ende = 10;
    argumente[2].thread_nummer = 3; argumente[2].start = 11; argumente[2].ende = 15;

    /* Alle drei Threads starten */
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, verarbeite_bereich, &argumente[i]);
    }

    /* Auf alle drei Threads warten */
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Hauptprogramm: Alle Threads sind fertig.\n");
    return 0;
}
```

**Was passiert hier?**\
Jeder der drei Threads bekommt seinen eigenen Zahlenbereich und verarbeitet diesen unabhängig. Die Ausgaben der Threads können sich vermischen – das ist normal, da alle drei gleichzeitig laufen.

**Aufgabe:**\
Führe das Programm mehrmals aus. Beobachte die Reihenfolge der Ausgaben. Ist sie immer gleich? Warum (nicht)?

***

### Aufgabe 7: Condition Variables – Threads koordinieren

#### Hintergrund

Manchmal muss ein Thread warten, bis ein anderer Thread eine bestimmte Aufgabe erledigt hat. Das kann man zwar mit einem Mutex lösen (aktives Warten = _spin-waiting_), aber das verschwendet CPU-Zeit. Die elegantere Lösung sind **Condition Variables** (dt. _Bedingungsvariablen_).

#### Die wichtigsten Funktionen

```c
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

| Parameter | Bedeutung                                                                                                                                                                                                        |
| --------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `cond`    | Die Condition Variable, auf die gewartet wird.                                                                                                                                                                   |
| `mutex`   | Ein bereits gesperrter Mutex. `pthread_cond_wait` gibt den Mutex **atomar** frei und wartet dann auf das Signal. Wenn das Signal empfangen wird, wird der Mutex wieder gesperrt, bevor die Funktion zurückkehrt. |

```c
int pthread_cond_signal(pthread_cond_t *cond);
```

| Parameter | Bedeutung                                                                                      |
| --------- | ---------------------------------------------------------------------------------------------- |
| `cond`    | Die Condition Variable, an die ein Signal gesendet wird. Weckt **einen** wartenden Thread auf. |

#### Beispielprogramm: Produzent-Konsument

```c
#include <stdio.h>
#include <pthread.h>

/* Gemeinsame Daten */
int daten_verfuegbar = 0; /* Flag: 1 = Daten vorhanden, 0 = keine Daten */
int wert = 0;             /* Der eigentliche Datenwert */

/* Mutex und Condition Variable gemeinsam initialisieren */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  bedingung = PTHREAD_COND_INITIALIZER;

/* Produzent-Thread: erzeugt Daten und signalisiert dem Konsumenten */
void *produzent(void *arg) {
    printf("Produzent: Bereite Daten vor...\n");

    /* Simuliere zeitaufwändige Berechnung */
    sleep(2);
    wert = 42;

    /* Mutex sperren, bevor gemeinsame Daten geändert werden */
    pthread_mutex_lock(&mutex);
    daten_verfuegbar = 1; /* Daten sind jetzt bereit */

    /* Signal an wartende Threads senden (z.B. den Konsumenten) */
    pthread_cond_signal(&bedingung);

    /* Mutex freigeben */
    pthread_mutex_unlock(&mutex);

    printf("Produzent: Daten bereitgestellt (Wert = %d).\n", wert);
    return NULL;
}

/* Konsument-Thread: wartet auf Daten vom Produzenten */
void *konsument(void *arg) {
    printf("Konsument: Warte auf Daten...\n");

    /* Mutex sperren, um gemeinsame Daten zu lesen */
    pthread_mutex_lock(&mutex);

    /* Warten, solange noch keine Daten verfügbar sind.
       pthread_cond_wait gibt den Mutex frei und wartet auf das Signal.
       Nach dem Signal: Mutex wird automatisch wieder gesperrt! */
    while (daten_verfuegbar == 0) {
        pthread_cond_wait(&bedingung, &mutex);
    }

    /* Jetzt sind Daten verfügbar – kritischen Abschnitt verlassen */
    printf("Konsument: Daten empfangen! Wert = %d\n", wert);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t prod_thread, kons_thread;

    /* Beide Threads starten */
    pthread_create(&kons_thread, NULL, konsument, NULL);
    pthread_create(&prod_thread, NULL, produzent, NULL);

    /* Auf beide Threads warten */
    pthread_join(prod_thread, NULL);
    pthread_join(kons_thread, NULL);

    printf("Hauptprogramm: Fertig.\n");
    return 0;
}
```

**Warum `while` statt `if` beim Warten?**\
`pthread_cond_wait` kann in seltenen Fällen aufgeweckt werden, ohne dass wirklich ein Signal gesendet wurde (_spurious wakeup_). Deshalb prüft man die Bedingung in einer `while`-Schleife, um sicherzustellen, dass die Daten wirklich verfügbar sind.

**Aufgabe:**\
Ändere das Programm so, dass der Produzent **drei** Werte nacheinander erzeugt und der Konsument jeden einzeln empfängt und ausgibt.

***

### Zusammenfassung

| Konzept                       | Problem                                                     | Lösung                                            |
| ----------------------------- | ----------------------------------------------------------- | ------------------------------------------------- |
| Thread erzeugen               | —                                                           | `pthread_create`                                  |
| Auf Thread warten             | —                                                           | `pthread_join`                                    |
| Race Condition                | Mehrere Threads schreiben gleichzeitig auf gemeinsame Daten | Mutex (`pthread_mutex_lock`/`unlock`)             |
| Aktives Warten (spin-waiting) | Verschwendet CPU-Zeit                                       | Condition Variable (`pthread_cond_wait`/`signal`) |

#### Checkliste: Häufige Fehler vermeiden

* [ ] **Mutex immer freigeben:** Jedes `pthread_mutex_lock` muss ein entsprechendes `pthread_mutex_unlock` haben.
* [ ] **`pthread_join` nicht vergessen:** Ohne `join` kann das Hauptprogramm beendet werden, bevor Threads fertig sind.
* [ ] **Keine Stack-Zeiger zurückgeben:** Rückgabewerte aus Threads müssen auf dem Heap (`malloc`) gespeichert werden.
* [ ] **`while` statt `if` bei `cond_wait`:** Zum Schutz vor _spurious wakeups_.
* [ ] **Compiler-Flag nicht vergessen:** `-lpthread` beim Kompilieren angeben.

***



### Weiterführende Ressourcen

* [Vorlesungsfolien: Threads](https://prof.aheil.de/os/threads)
* OSTEP (Operating Systems: Three Easy Pieces) – Kapitel 26 (online verfügbar)
* `man pthread_create`, `man pthread_join`, `man pthread_mutex_lock` (in der Linux-Terminal-Hilfe)
