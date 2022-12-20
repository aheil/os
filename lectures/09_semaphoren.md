<!--

author:   Andreas Heil

email:    andreas.heil@hs-heilbronn.de

version:  0.1

language: de

narrator: DE German Male

tags: betriebssysteme, lecture, semaphoren

comment:  

-->


# Semaphoren

<!-- data-type="none" -->
| Parameter | Kursinformationen |
| --- | --- |
| **Veranstaltung:** | `262007 Betriebssysteme`|
| **Semester** | `SEB2` |
| **Hochschule:** | `Hochschule Heilbronn` |
| **Inhalte:** | `Semaphoren` |
| Startseite | [https://liascript.github.io/course/?https://raw.githubusercontent.com/aheil/os/master/README.md#1](https://liascript.github.io/course/?https://raw.githubusercontent.com/aheil/os/master/README.md#1) | 
| **Link auf den GitHub:** | [https://github.com/aheil/os/blob/main/lectures/09_semaphoren.md](https://github.com/aheil/os/blob/main/lectures/09_semaphoren.md) |
| **Autoren** | @author |

## Lernziele 

- Konzept hinter Semaphoren **verstehen** und grundlegende Anwendungsmöglichkeiten **kennen lernen**

- Typische Probleme und Fehlerursachen bei Nebenläufigkeit **verstehen** und mögliche Lösungsansätze **kennen lernen** und **verstehen** 

## Semaphoren kurz erklärt 

Rein technisch betrachtet: 

> Eine Semaphore ist ein Objekt mit einem Integer-Wert und dazugehörigen Methoden (wir betrachten, wie schon zuvor, den POSIX Standard). Der initiale Wert der Semaphore bestimmt ihr Verhalten, weswegen sie initialisiert werden muss.

Welche Methoden benötigen wir für eine Semaphore?

- `sem_wait()`
- `sem_post()`

{{1}}
************************************

**Semaphore initialisieren**

```c
#include <semaphore.h>
sem_t s;
sem_init(&s, 0, 1);
```

- Zunächst wird eine Semaphore `s` deklariert 
- `s` wird an `sem_init()` übergeben und mit dem Wert 1 initialisiert 
- Der zweite Parameter 0 gibt an, dass die Semaphore zwischen Threads im selben Prozess geteilt wird
- Semaphoren dienen also dazu, Threads zu synchronisieren 
- Sempahoren können auch prozessübergreifend zur Synchronisation genutzt werden, dies wird von uns aber nicht weiter behandelt 


************************************

{{2}}
************************************

**SEM\_WAIT und SEM\_POST** 

```c
int sem_wait(sem_t *s) { 
  verringere den Wert der Semaphore s um 1
  warte falls der Wert der Semaphore s negative ist
}
```

- `sem_wait()` kehrt sofort zurück, falls der Wert größer oder gleich 0 war 
- Oder, der Aufrufer wartet auf die Rückkehr aus der Routine, wenn der Wert negativ war.
- Ein ähnliches Konzept mit dem Warten auf das Lock habe wir bei den Locks verwende, sie erinnern sich? Wir kommen darauf gleich nochmal zurück… Bitte 
- Wird `sem_wait()` von mehreren Threads aufgerufen und der Wert war oder wird negativ, müssen alle aufrufenden Threads warten.


```c
int sem_post(sem_t *s) { 
  erhöhe den Wert der Semaphore s um 1
  falls ein oder mehr Threads warten, wecke einen davon auf
}
```

- `sem_post()` erhöht einfach den Wert um 1
- Ohne weitere Prüfung wird dann ein schlafender Thread aufgeweckt 

Wie kann das funktionieren?! Das schauen wir auf den folgenden Beispielen an!

************************************

{{3}}
************************************


**Nützliche Informationen:** 

- Der negative Wert einer Semaphore gibt die Nummer der wartenden Threads an. 
- Der Initialisierungswert einer Semaphore gibt die Anzahl an Ressourcen an, die wir initial „hergeben“ möchten (oder kkönnen) 

************************************

## Semaphore als Lock

**Verwendung der Semaphore als Lock:**

- Wir setzen den Wert der Semaphore initial auf 1

**Beispiel 1:**

- Thread T0 ruft `sem_wait()` auf
- Der Wert der Sempahore wird um 1 verringert (d.h. von 1 auf 0) 
- Thread T0 wartet nur, wenn der Wert nicht größer gleich 0 ist, also nur wenn der Wert negativ ist
- Da der Wert noch 0 ist, kehrt `sem_wait()` sofort zurück und Thread T0 kann den kritischen Abschnitt sofort ausführen 
- Sofern kein anderer Prozess versucht hat das Lock zu erhalten, ruft Thread T0 am Ende `sem_post()` auf und der Wert der Semaphore wird zurück auf 1 gesetzt

**Beispiel 2:** 

- Thread T0 hat `sem_post()` noch nicht aufgerufen (wurde vorher vom SCheduler unterbrochen oder T1 läuft auf einem anderen Kern) und Thread T1 versucht nun das Lock zu erhalten
- Thread T1 verringert den Wert der Semaphore auf -1 und wartet (-1 signalisiert, es ist keine Ressource mehr frei)
- Wenn Thread T0 irgendwann wieder ausgeführt wird und fertig wird, wird durch den Aufruf von `sem_post()` der Wert der Semaphore auf 0 erhöht und ein anderer Thread (in unserem Fall Thread T1) aufgeweckt
- Thread T1 kann das Lock nun erhalte (der Wert ist ja auf 0) und ruft nach Beendigung `sem_post()` auf und setzt somit den Wert der Semaphore wieder auf 1

{{1}}
************************************

**Binäre Semaphore**

- Mit dem vorherigen Beispiel haben wir eine Semaphore genutzt, um ein Lock zu erzeugen 
- Locks haben genau zwei Zustände: gesperrt oder nicht gesperrt bzw. engl. _held_ und _not held_ 
- Aufgrund der zwei Zustände heißt eine solche Semaphore auch **binäre Semaphore**


************************************

## Semaphoren zur Signalisierung

> Semaphoren können auch genutzt werden, um mehrer Threads zu synchronisieren

Prinzip: ein Thread wartet darauf, dass etwas passiert, ein anderer kümmert sich darum, dass etwas passiert und signalisiert am Ende etwas dem anderen Thread. 

Etwas konkreter: Ein Thread erzeugt einen anderen Thread und wartet auf dessen Beendigung.

{{1}}
************************************

**Parent-Child-Synchronisation**

- Parent Thread erzeugt den Child Thread
- In diesem Fall initialisieren wir den Wert der Semaphore mit **0** 
- Dieses Mal ruft der Parent Thread `sem_wait()` auf, Child Thread ruft `sem_post()` auf 

**Szenario 1:** 

- Child Thread wurde zwar erzeugt, läuft aber noch nicht
- Annahme: Parent Thread ruf `sem_wait()` auf bevor der Child Thread `sem_post()` aufruft 
- Parent läuft also, ruft `sem_wait()`, verringert den Wert der Semaphore auf -1 und schläft dann
- Erst wenn der Child Thread beendet ist, dieser dann ` sem_post()` aufruft, wird der Wert auf 0 gesetzt und der Parent Thread kann wieder weiter laufen 

************************************

{{2}}
************************************

**Producer/Consumer Problem**

Wir Bereits bei Conditional Variables behandelt (falls Sie sich nicht mehr erinnern, wiederholen Sie Kapitel 27/28 aus dem OSTEP Buch).

Klassisches Problem in der Prozesssynchronsiation, die den konkurrierenden Zugriff von Erzeugern (engl. producer) und Verbrauchern (engl. consumer) regelt

Grundidee: Consumer sollen keine Elemente aus einer Datenstruktur nehmen können, wenn dort keine enthalten sind und Producer sollen keine Elemente in die Datenstruktur schreiben können, wenn dort die maximale Kapazität erreicht ist



************************************

{{3}}
************************************

**Der Puffer**

```c
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

- Zunächst gehen wir davon aus, es gibt genau einen Puffer, d.h. `MAX = 1`
- Zwei Threads, Producer, Consumer und eine CPU
- Weiterhin gehen wir davon aus, dass derConsumer zuerst läuft  

************************************

{{4}}
************************************

**Consumer und Producer**

Zwei Semaphore 

- `empty`
- `full`

Der Consumer wartet bis der Puffer voll ist

```c
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

```c
void *producer(void *arg) {
  int i;
  for (i = 0; i < loops; i++) {
    sem_wait(&empty); // Line P1    
    put(i); // Line P2
    sem_post(&full); // Line P3
  }
}
```

************************************

{{5}}
************************************

**Versuch 1**

- Consumer startet und ruft `sem_wait(&full)` auf
- Da `full` mit 0 initialisiert war, wird `full` nun um 1 reduziert (d.h. neuer Wert ist nun -1)
- Consumer wird nun blockiert und wartet bis ein andere Thread `sem_post(&full)` aufruft
- Nun läuft der Producer und ruft `sem_wait(&empty)` auf
- `empty` wurde mit `MAX=1` initialisiert, daher läuft der Producer zunächst und `empty` wird auf 0 reduziert 

```c
int main(int argc, char *argv[]) {
  // ...
  sem_init(&empty, 0, MAX); // MAX are empty
  sem_init(&full, 0, 0); // 0 are full
  // ...
}
```

- Nun wird der Puffer befüllt und `sem_post(&full)` aufgerufen
- `full` wird von -1 auf 0 erhöht und der Consumer kann wieder laufen 

**Szenario 1:**

- Producer läuft noch weiter, versucht wieder `sem_wait(&empty)` aufzurufen und wird diesmal blockiert, `empty` steht nämlich derzeit auf 0

**Szenario 2:**

- Producer stoppt, Consumer startet, würde nun von `sem_wait(&full)` zurückkehren und `get()` aufrufen
Da der Producer `&full` inzwischen erhöht hatte, kann der Consumer wieder laufen

> Beide Fälle führen zum gewünschten Ergebnis! 


************************************

{{6}}
************************************

**Was wäre wenn?**

... MAX > 1 wäre? 

**Annahme: Mehrere Producer und mehrere Consumer**

- In unserem Beispiel gibt es eine _Race Condition_
- Sowohl in `put` als auch in `get` können zwei Threads auf den gleichen Zähler (`fill` bzw. `use`) zugreifen, wenn Sie durch ein Interrupt direkt nach dem Schreiben, aber vor dem Erhöhen unterbrochen werden

Was fehlt? Mutual Exclusion oder eben eine Lock!

************************************

{{7}}
************************************

**Mutex**

Nutzen wir eine binäre Semaphore als Lock für den kritischen Abschnitt:

```c 
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

```c
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
> Hinweis: Werden die Zeilen des Mutex mit der jeweiligen anderen Semaphore paarweise vertauscht (z.B. P1 mit P2) entsteht ein Deadlock


************************************

## Konotrollfragen

Was ist der Hauptzweck von Semaphoren?

-[(x)] Um den Zugriff auf gemeinsam genutzte Ressourcen zu synchronisieren
-[( )] Um den Zugriff auf nicht gemeinsam genutzte Ressourcen zu synchronisieren
-[( )] Um den Zugriff auf Dateien zu synchronisieren
-[( )] Um den Zugriff auf Netzwerkverbindungen zu synchronisieren

Welche der folgenden Aussagen beschreibt Semaphoren **am besten**?

-[( )] Semaphoren sind ein Mechanismus zur Synchronisierung von Threads innerhalb eines Prozesses
-[(x)] Semaphoren sind ein Mechanismus zur Synchronisierung von Prozessen innerhalb eines Systems
-[( )] Semaphoren sind ein Mechanismus zur Synchronisierung von Netzwerkverbindungen
-[( )] Semaphoren sind ein Mechanismus zur Synchronisierung von Dateizugriffen

Welches ist der richtige Weg, um eine Semaphore für Threads zu initialisieren?

-[(x)] sem_init(&sem, 0, 1);
-[( )] sem_init(sem, 0, 1);
-[( )] sem_init(sem, 1, 1);
-[( )] sem_init(&sem, 1, 0);


Was ist der Hauptunterschied zwischen Semaphoren und Mutexen?

-[( )] Semaphoren können von mehreren Threads gleichzeitig besetzt werden, während Mutexes nur von einem Thread besetzt werden können
-[( )] Semaphoren können von mehreren Prozessen gleichzeitig besetzt werden, während Mutexes nur von einem Prozess besetzt werden können
-[( )] Semaphoren können von mehreren Prozessen und Threads gleichzeitig besetzt werden, während Mutexes nur von einem Thread oder Prozess besetzt werden können
-[(x)] Semaphoren können von mehreren Threads gleichzeitig besetzt werden, während Mutexes nur von einem Thread oder Prozess besetzt werden können