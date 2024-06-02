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

## Semaphore als Lock

**Verwendung der Semaphore als Lock:**

* Wir setzen den Wert der Semaphore initial auf 1

**Beispiel 1:**

* Thread T0 ruft `sem_wait()` auf
* Der Wert der Sempahore wird um 1 verringert (d.h. von 1 auf 0)
* Thread T0 wartet nur, wenn der Wert nicht größer gleich 0 ist, also nur wenn der Wert negativ ist
* Da der Wert noch 0 ist, kehrt `sem_wait()` sofort zurück und Thread T0 kann den kritischen Abschnitt sofort ausführen
* Sofern kein anderer Prozess versucht hat das Lock zu erhalten, ruft Thread T0 am Ende `sem_post()` auf und der Wert der Semaphore wird zurück auf 1 gesetzt

**Beispiel 2:**

* Thread T0 hat `sem_post()` noch nicht aufgerufen (wurde vorher vom SCheduler unterbrochen oder T1 läuft auf einem anderen Kern) und Thread T1 versucht nun das Lock zu erhalten
* Thread T1 verringert den Wert der Semaphore auf -1 und wartet (-1 signalisiert, es ist keine Ressource mehr frei)
* Wenn Thread T0 irgendwann wieder ausgeführt wird und fertig wird, wird durch den Aufruf von `sem_post()` der Wert der Semaphore auf 0 erhöht und ein anderer Thread (in unserem Fall Thread T1) aufgeweckt
* Thread T1 kann das Lock nun erhalte (der Wert ist ja auf 0) und ruft nach Beendigung `sem_post()` auf und setzt somit den Wert der Semaphore wieder auf 1

**Binäre Semaphore**

* Mit dem vorherigen Beispiel haben wir eine Semaphore genutzt, um ein Lock zu erzeugen
* Locks haben genau zwei Zustände: gesperrt oder nicht gesperrt bzw. engl. _held_ und _not held_
* Aufgrund der zwei Zustände heißt eine solche Semaphore auch **binäre Semaphore**

## Semaphoren zur Signalisierung

Semaphoren können auch genutzt werden, um mehrere Threads zu synchronisieren

Prinzip: ein Thread wartet darauf, dass etwas passiert, ein anderer kümmert sich darum, dass etwas passiert und signalisiert am Ende etwas dem anderen Thread.

Etwas konkreter: Ein Thread erzeugt einen anderen Thread und wartet auf dessen Beendigung.

**Parent-Child-Synchronisation**

* Parent Thread erzeugt den Child Thread
* In diesem Fall initialisieren wir den Wert der Semaphore mit **0**
* Dieses Mal ruft der Parent Thread `sem_wait()` auf, Child Thread ruft `sem_post()` auf

**Szenario 1**

* Child Thread wurde zwar erzeugt, läuft aber noch nicht
* Annahme: Parent Thread ruf `sem_wait()` auf bevor der Child Thread `sem_post()` aufruft
* Parent läuft also, ruft `sem_wait()`, verringert den Wert der Semaphore auf -1 und schläft dann
* Erst wenn der Child Thread beendet ist, dieser dann `sem_post()` aufruft, wird der Wert auf 0 gesetzt und der Parent Thread kann wieder weiter laufen

**Szenario 2**

* Child Thread wurde erzeugt und läuft bis zum Ende, bevor Parent Thread `sem_wait()` aufrufen konnte
* Child Thread ruft `sem_post()` auf
* Der Wert der Semaphore wird von 0 auf 1 erhöht (Parent Thread hatte ja noch keine Chance `sem_wait()` aufzurufen, und der Wert wurde noch nicht verringert)
* Wenn nun der Parent Thread wieder läuft und `sem_wait()` aufruft, wird der Wert der Semaphore zurück auf 0 gesetzt - ohne zu warten, da die Routine sofort beendet

## Producer / Consumer Problem

Wir Bereits bei Conditional Variables behandelt (vgl. Kapitel 27/28 aus dem OSTEP Buch).

Klassisches Problem in der Prozesssynchronsiation, die den konkurrierenden Zugriff von Erzeugern (engl. producer) und Verbrauchern (engl. consumer) regelt

Grundidee: Consumer sollen keine Elemente aus einer Datenstruktur nehmen können, wenn dort keine enthalten sind und Producer sollen keine Elemente in die Datenstruktur schreiben können, wenn dort die maximale Kapazität erreicht ist.

**Puffer**

```
int buffer[MAX];
int fill = 0;
int use = 0;

void put(int value) {
  buffer[fill] = value; // Line F1
  fill = (fill + 1) % MAX; // Line F2
}

int get() {
  int tmp = buffer[use]; // Line G1
  use = (use + 1) % MAX; // Line G2
  return tmp;
} 
```

* Zunächst gehen wir davon aus, es gibt genau einen Puffer, d.h. `MAX = 1`
* Zwei Threads, Producer, Consumer und eine CPU
* Weiterhin gehen wir davon aus, dass der Consumer zuerst läuft

**Producer und Consumer**

Zwei Semaphore

* `empty`
* `full`

Der Consumer wartet bis der Puffer voll ist

```
sem_t empty;
sem_t full;

void *consumer(void *arg) {
  int i, tmp = 0;
  while (tmp != -1) {
    sem_wait(&full);  // Line C1
    tmp = get();      // Line C2
    sem_post(&empty); // Line C3
    printf("%d\n", tmp);
  }
}
```

Der Producer wartet bis der Puffer leer ist

```
void *producer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    sem_wait(&empty); // Line P1    
    put(i); // Line P2
    sem_post(&full); // Line P3
  }
}
```

**Versuch 1**

* Consumer startet und ruft `sem_wait(&full)` auf
* Da `full` mit 0 initialisiert war, wird `full` nun um 1 reduziert (d.h. neuer Wert ist nun -1)
* Consumer wird nun blockiert und wartet bis ein andere Thread `sem_post(&full)` aufruft
* Nun läuft der Producer und ruft `sem_wait(&empty)` auf
* `empty` wurde mit `MAX=1` initialisiert, daher läuft der Producer zunächst und `empty` wird auf 0 reduziert

```
int main(int argc, char *argv[]) {
  // ...
  sem_init(&empty, 0, MAX); // MAX are empty
  sem_init(&full, 0, 0); // 0 are full
  // ...
}
```

* Nun wird der Puffer befüllt und `sem_post(&full)` aufgerufen
* `full` wird von -1 auf 0 erhöht und der Consumer kann wieder laufen

**Szenario 1:**

* Producer läuft noch weiter, versucht wieder `sem_wait(&empty)` aufzurufen und wird diesmal blockiert, `empty` steht nämlich derzeit auf 0

**Szenario 2:**

* Producer stoppt, Consumer startet, würde nun von `sem_wait(&full)` zurückkehren und `get()` aufrufen Da der Producer `&full` inzwischen erhöht hatte, kann der Consumer wieder laufen

> Beide Fälle führen zum gewünschten Ergebnis!

**Was wäre wenn?**

... MAX > 1 wäre?\


**Annahme: Mehrere Producer und mehrere Consumer**

Was fehlt? Mutual Exclusion oder eben eine Lock!

* In unserem Beispiel gibt es eine _Race Condition_
* Sowohl in `put` als auch in `get` können zwei Threads auf den gleichen Zähler (`fill` bzw. `use`) zugreifen, wenn Sie durch ein Interrupt direkt nach dem Schreiben, aber vor dem Erhöhen unterbrochen werden

**Mutex**

Nutzen wir eine binäre Semaphore als Lock für den kritischen Abschnitt:

```
void *producer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) 
    sem_wait(&empty); // Line P1
    sem_wait(&mutex); // Line P2 
    put(i);** // <== 
    sem_post(&mutex);
    sem_post(&full);
  }
}
```

```
void *consumer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    sem_wait(&full);
    sem_wait(&mutex);
    int tmp = get();  // <==
    sem_post(&mutex);
    sem_post(&empty); 
    printf("%d\n", tmp);
  }
}
```

**Hinweis**: Werden die Zeilen des Mutex mit der jeweiligen anderen Semaphore paarweise vertauscht (z.B. P1 mit P2) entsteht ein **Deadlock**\


## Unnützes Wissen

Sempahor ist eine Methode zur Kommunikation mittels Flaggen oder Lichtern, die über große Entfernung sichtbar sind und stammt ursprünglich aus der Schifffahrt.&#x20;

Durch die Positionierung von zwei Flaggen oder Lichtern kann somit ein Code erzeugt werden, der an einen Empfänger übermittelt werden kann.&#x20;



\


