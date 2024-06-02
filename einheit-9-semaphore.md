# Einheit 9: Semaphore

## Lernziele

* Konzept hinter Semaphoren **verstehen** und grundlegende Anwendungsmöglichkeiten **kennen lernen**
* Typische Probleme und Fehlerursachen bei Nebenläufigkeit **verstehen** und mögliche Lösungsansätze **kennen lernen** und **verstehen**

## Semaphoren kurz erklärt

Rein technisch betrachtet:

> Eine Semaphore ist ein Objekt mit einem Integer-Wert und dazugehörigen Methoden (wir betrachten, wie schon zuvor, den POSIX Standard). Der initiale Wert der Semaphore bestimmt ihr Verhalten, weswegen sie initialisiert werden muss.

Eine Semaphore kann als **Lock für einen kritischen Abschnitt** benutzt werden. Gleichzeitig ermöglicht es uns eine Semaphore, **die Anzahl der gleichzeitigen Zugriffe zu regeln**.&#x20;

Eine Semaphore, die genau einen Zugriff erlaubt nennt sich **binäre Semaphore**.

\
Methoden für Semaphore
----------------------

* `sem_init()`
* `sem_wait()`
* `sem_post()`

## Semaphore initialisieren

```
#include <semaphore.h>
sem_t s;
sem_init(&s, 0, 1);
```

* Zunächst wird eine Semaphore `s` deklariert
* `s` wird an `sem_init()` übergeben und mit dem Wert 1 initialisiert
* Der zweite Parameter 0 gibt an, dass die Semaphore zwischen Threads im selben Prozess geteilt wird
* Semaphoren dienen also dazu, Threads zu synchronisieren
* Sempahoren können auch prozessübergreifend zur Synchronisation genutzt werden, dies wird von uns aber nicht weiter behandelt
* Der dritte Parameter gibt den Initialisierungswert der Semaphore an.&#x20;

## Funktionsweise von sem\_wait()

```
int sem_wait(sem_t *s) { 
  verringere den Wert der Semaphore s um 1
  warte falls der Wert der Semaphore s negative ist
}
```

* `sem_wait()` kehrt sofort zurück, falls der Wert **größer oder gleich 0** war
* Oder, der Aufrufer wartet auf die Rückkehr aus der Routine, wenn der **Wert negativ** war.
* Ein ähnliches Konzept mit dem Warten auf das Lock habe wir bei den Locks verwende, sie erinnern sich? Wir kommen darauf gleich nochmal zurück…&#x20;
* Wird `sem_wait()` von mehreren Threads aufgerufen und der Wert war oder wird negativ, müssen alle aufrufenden Threads warten.

## Funktionsweise von sem\_post()

```
int sem_post(sem_t *s) { 
  erhöhe den Wert der Semaphore s um 1
  falls ein oder mehr Threads warten, wecke einen davon auf
}
```

* `sem_post()` erhöht einfach den Wert um 1
* Ohne weitere Prüfung wird dann ein schlafender Thread aufgeweckt

Wie kann das funktionieren? Im Folgenden werden einige Beispiele gezeigt, an denen das Verhalten einer Semaphore klarer wird.

## **Nützliche Informationen**

* Der **negative Wert** einer Semaphore gibt die **Nummer der wartenden Threads** an.
* Der **Initialisierungswert** einer Semaphore gibt die **Anzahl an Ressourcen an**, die wir initial „hergeben“ möchten (oder können).

\




