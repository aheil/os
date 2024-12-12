# Threads

## Lernziele

* Unterschied zwischen Prozessen und Threads **kennen lernen**
* Thread-API in C **nutzen können**

## Was sind Threads

**Wiederholung**

* Bisher gelernt
  * Virtualisierung einer CPU – also mit »Scheduling« den Anschein erwecken, als gäbe es viele CPUs
  * Virtualisierung von Speicher – also mittels »Adressräumen« den Anschein erwecken, als hätte jedes Programm seinen ganz eigenen Speicher
  * Dabei galt implizit immer die Annahme, dass ein Prozess aus einem einzigen Ausführungsstrang besteht

**Was sind Threads?**

* Anstelle nur eines Ausführungsstrangs besitzen »multi-threaded« Programme **mehrere Ausführungsstränge**
* Jeder dieser **Ausführungsstränge** (engl. **thread**) kann grundsätzlich wie ein Prozess verstanden werden
* Ein wichtiger Unterschied: Während jeder Prozess seinen ganz eigenen Adressraum besitzt, **teilen sich Threads einen Adressraum** und können auf dieselben Daten zugreifen

**Eigenschaften von Threads**

* Threads werden grundsätzlich wie Prozesse behandelt
  * Benötigen einen **Programm Counter (PC)** für die nächste Instruktion
  * Haben eigene **Register**
  * Laufen zwei Threads auf einer CPU, muss ein **Context-Switch** stattfinden
  * Für den Context Switch wird ein _**Thread Control Block**_**&#x20;(TCB)** analog zum Process Control Block (PCB) benötigt
  * Wichtiger Unterschied: Der **Adressraum bleibt** beim Context-Switch von Threads **der gleiche**

**Multi-Thread-Adressräume**

Threads laufen unabhängig, rufen eigene Routinen mit eigenen lokalen Variablen und Rücksprungadressen auf und benötigen daher **dedizierte Stacks** (engl. thread-local storage)

\
![](<../.gitbook/assets/os.08.multi_thread_address_space (2).png>)\
\
**Warum überhaupt Threads verwenden?**

**Offensichtlich:** Parallelisierung, sobald mehrere CPUs bereitstehen, können Aufgaben gleichzeitig durchgeführt werden

**Etwas subtiler:** Würde ein Prozess aufgrund einer I/O-Operation geblockt werden, könnte ein weitere Thread im Prozess weiterlaufen und somit das **blockieren des Prozesses vermeiden**

**Vorteil:** Threads greifen auf dieselben Daten innerhalb eines Prozesses zu.

**Threads und Determinismus**

* Threads laufen **nicht deterministisch**, da Routinen im Thread unabhängig vom Aufrufer laufen
* Was dann tatsächlich läuft wird durch den Scheduler gesteuert
* Problem verschärft sich noch, da Threads auf dieselben Daten zugreifen

Konsequenz: Nicht-deterministische Programmläufe und Ergebnisse

* OSTEP Kapitel 26.2 zum Erzeugen von Threads (in C)

## Scheduling Beispiel

**Hier: Erhöhen eines Counters um 1**

* Variable für Counter liegt bei Adresse 0x8049a1c
* Wert der Variable wird in Register eax geladen (Zeile 1)
* Wert wird um 1 erhöht (Zeile 2)
* Neuer Wert wird aus Register zurück an Andresse 0x8049a1c geschrieben

```
mov 0x8049a1c, %eax
add $0x1, %eax
mov %eax, 0x8049a1c
```

**Annahme: Es gbit twei Threads, T1 und T2**

* T1 führt den Code aus und erhöht den Wert (z.B. 50) um 1
* In Register eax steht nun 51
* Jetzt: Timer Interrupt, Betriebssystem speichert T1
  * Programm Counter
  * Register einschließlich eax
* Nun wird T2 ausgeführt
* T2 lädt den Wert von 0x8049a1c
* In eax steht nun 50 (Jeder Thread hat seine eigenen virtualisierten Register!!!1)
* eax wird erhöht und zurückgeschrieben
* Jetzt findet nochmal ein Context-Switch statt und T1 wird wieder ausgeführt
* Alle Register werden geladen
* In eax steht nun 51
* T1 führt nun noch Zeile 3 aus und schreibt 51 an 0x8049a1c
* Was sollte aber eigentlich in unserer Variable stehen?

➡ OSTEP Kapitel 26.3 für detaillierten Trace des Ablaufs

## Race Conditions

* Ergebnis abhängig vom Timing bei der Ausführung von Code: **Race Condition**
* Führt zu einem nicht-deterministischen Programmfehler, der in der Regel schwer zu finden ist (Ergebnis heißt auf engl. indeterminate)
* Code, der die Race Condition auslösen kann wird **kritischer Abschnitt** (engl. critical section) genannt
* Greift auf eine **gemeinsame Variable** (engl. shared variable) bzw. **gemeinsame Ressource** (engl. shared resource).
* Durch **wechselseitigen Ausschluss** (engl. mutual exclusion) wird erreicht, dass wenn sich ein Thread in einem kritischen Abschnitt befindet, kein anderer Thread auf diesen Code zugreifen kann…
* Aber wie?

## Thread API

Fragestellung: Welche Schnittstellen muss das Betriebssystem bereitstellen, um Threads zu erstellen und zu kontrollieren?

* Am Beispiel von POSIX (Portable Operating System Interface) Systemen:
  * Ein Thread erzeugen
  * Auf einen Thread warten
  * Locks = Ausschluss aus kritischem Abschnitt
  * Condition Variable (dt. Monitor = Synchronisationsmechanismus)

**4 Argumente** erforderlich: `thread`, `attr`, `start routine`, `arg`

```
#include <pthread.h>
int pthread_create(pthread_t *thread,
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void*),
                   void *arg); 
```

* `thread`
  * Pointer auf Struktur vom Typ `pthread_t`, wird genutzt um mit dem Thread zu interagieren
* `attr`
  * Spezifiziert die Attribute des Threads (Größe d. Stacks, Scheduling Priorität etc.)
* `start_routine`
  * Function Pointer auf Routine, die vom Thread ausgeführt werden soll
* `arg`
  * Argument, das an den Anfang des Threads übergeben wird

**Auf einen Thread warten**

```
int pthread_join(pthread_t thread, void **value_ptr);
```

* `pthread_t`
  * Spezifiziert den Thread, auf den gestartet wird
* `value_ptr`
  * Zeiger auf den Rückgabewert der Routine
* Einen Thread mittels `pthread_create` zu erzeugen und direkt danach mittels `pthread_join` auf dessen Beendigung zu warten ist natürlich nicht sonderlich sinnvoll
* In diesem Fall ist ein einfacher Prozeduraufruf einfacher.
* Wenn der Thread jedoch gestartet wird, und erst im späteren Verlauf des Programms auf seine Beendigung gewartet wird, macht es wieder Sinn

<figure><img src="../.gitbook/assets/os.08.thread.png" alt=""><figcaption></figcaption></figure>

Locks\



* Funktionen für den gegenseitigen Ausschluss
* **Mutex:** Mutual Exclusion Object

```
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&lock); 
x = x + 1; // critical section
pthread_mutex_unlock(&lock);
```

**Wie funktioniert ein Mutex?**

* Kritischer Abschnitt kann nur betreten werden, wenn man (der Code) in Besitz des Mutex ist
* Beispiel hier: `pthread_mutex_loc` setzt das Mutex, und kein anderer Thread (mit dem gleichen Code) kann diesen Code-Abschnitt betreten, bevor `pthread_mutex_unlock` aufgerufen wurde
* Wie wird das erreicht?
  * Ist der Lock gesetzt, kehrt die Routine `pthread_mutex_lock` erst zurück, nachdem das Mutex freigegeben wurde

**Condition Variables**

Verständigung zwischen Threads

* z.B. ein Thread wartet auf etwas von einem anderen Thread
* `pthread_cond_wait` lässt den Thread warten, bis er das entsprechende Signal bekommt, dass er weitermachen kann

```
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_signal(pthread_cond_t *cond);
```

**Zusammenfassung**

* Durch das Aufrufen einer `lock`-Routine wird versucht Zugriff auf ein ein Lock, also eine Art Token zu erhalten, die das alleinige Ausführungsrecht eines kritischen Abschnitts sicherstellt
* Ruft jemand anderes nun `lock` auf, kehrt diese Methode solange nicht zurück, wie das Lock nicht freigegeben wurde
* Erst wenn der ursprüngliche Aufrufer durch `unlock` das Lock wieder frei gibt, kehrt eine der bisher aufgerufene `lock`-Routinen zurück und lässt die Ausführung des kritischen Abschnitts zu\


**Anforderungen an ein Lock**

* **Mutual Exclusion:** Locks müssen gegenseitigen Ausschluss ermöglichen
* **Fairness:** Hat jeder Thread eine faire Chance das Lock zu erhalten, sobald es wieder freigegeben wird? Anders ausgedrückt: Es muss vermieden werden, dass ein Thread verhungert (engl. starve), wenn er „ewig“ auf das Lock wartet
* **Overhead:** Hält sich der Aufwand für einen einzelnen Thread in Grenzen, um das Lock zu erhalten und wieder freizugeben? Wie verhält es sich mit der Performance, wenn es viele Threads und ggf. mehrere CPUs gibt?

## Ansatz 1: Interrupts deaktivieren

Durch Ausschalten der Interrupts, wird Code in einem kritischen Abschnitt nicht mehr unterbrochen



```
void lock() {
  DisableInterrupts();
}
void unlock() {
  EnableInterrupts();
}
```

**Pro und Contra deaktivierte Interrupts**

Vorteil:

* Einfach

Nachteil(e):

* Privilegierte Operation könnte ggf. missbraucht werden, indem am Anfang vom ganzen Programm `lock und erst am Ende `` unlock` aufgerufen wird
* Funktioniert nicht auf Multi-CPU-Systemen
* Wird, wenn überhaupt, nur vom Betriebssystem intern genutzt, um auf eigene Datenstrukturen zuzugreifen oder um ungünstige Interrupt-Situationen vorzubeugen

Ansatz 2: Einfache Variable\



Weshalb kann die Verwendung einer einfachen Variablen als Lock nicht genügen?

Zur Erinnerung: Im POSIX-System verwenden wir auch eine Variable in Kombination mit `lock`- und `unlock`-Routinen. Wie funktioniert das?

Grundidee:

* Einfache Variable (ein Flag) als Lock nutzen
* Aufruf von `lock` testet ob Flag == 1 ist, falls nicht wird dieses auf 1 gesetzt und Thread hat jetzt das Lock
* Ruft nun ein zweiter Thread `lock` auf, wartet er solange bis das Flag wieder 0 ist

<figure><img src="../.gitbook/assets/os.08.simple_lock.de.png" alt=""><figcaption></figcaption></figure>

**Probleme**

* **Problem 1 (Korrektheit):** Durch ungünstige Context-Switches schaffen es beide Threads nun zu laufen, da beide Threads das Flag auf 1 setzen konnten
* **Problem 2 (Performance):** Während ein Thread auf das Lock wartet, prüft er ständig die Variable und verschwendet Rechenzeit des anderen Prozesses z.B. durch Context-Switch, Speicherzugriff, etc. (eng. spin-waiting)

## Ansatz 3: Spin Locks mit Test-and-Set

* Hardware-Unterstützung bereits in den 1960ern, die heute noch zum Einsatz kommt
* Test-and-Set bzw. Atomic Exchange
* Liefert den »alten« Wert an der Adresse von `old_ptr`
* Setzt gleichzeitig den Wert an `*old_ptr` auf den Inhalt von new
* Wichtig: Diese Operationen werden atomar aufgerufen!

```
int TestAndSet(int *old_ptr, int new) {
  int old = *old_ptr; // fetch old value at old_ptr 3 
  *old_ptr = new; // store ’new’ into old_ptr
  return old; // return the old value
}
```

**Es funktioniert weil...**

**Szenario 1**

* Annahme, Thread ruft `lock` auf und kein anderer Thread hält das Lock (flag==0)
* `TestAndSet` liefert 0, also wird der aufrufende Thread nicht weite prüfen (spinning)
* Das Flag wird außerdem auf 1 gesetzt (atomar!), d.h. kein anderer Thread kann das Lock erhalten
* Ist der Thread fertig, ruft er `unlock` auf und das Flag wird wieder auf 0 gesetzt

**Szenario 2**

* Annahme, Ein anderer Thread hält das Lock (flag==1)
* `TestAndSet` liefert 1, also wird der aufrufende Thread nicht weite prüfen (spinning)
* Das Flag wird außerdem auf 1 gesetzt (atomar!), das ist aber nicht schlimm
* Solange der andere Thread das Lock besitzt, liefert `TestAndSet` immer 1

**Spinning Lock**

* Wir haben das erste funktionierende Lock gebaut
* Spin Lock: Einfachste Variante, dreht sich (engl. to spin) solang unter der Verwendung von CPU Cycles bis das Lock verfügbar wird
* Benötigt einen Preemtive Scheduler (Sie erinnern sich!?), der Threads via Timer unterbricht, damit ein anderer Thread laufen kann  ohne Preemtive Scheduling machen Spin Locks auf Systemen mit nur einer CPU keinen Sinn, da der Thread sonst nie die CPU freigeben würde

> In x86 z.B. via `xchg` realisiert (x86 Instruction Set Reference1): „... This instruction is useful for implementing semaphores or similar data structures for process synchronization.”

**Spin Locks: Kurze Betrachtung**

**Korrektheit**

* Wenn, wie zuvor umgesetzt, stellen Spin Locks sicher, dass nur ein einziger Thread einen kritischen Abschnitt ausführen kann

**Fairness**

* Nope, es kann sein, dass der wartende Thread niemals in den kritischen Abschnitt kommt, und können somit zum „verhungern“ eines anderen Threads führen

**Performanz**

* Jeder wartende Thread nutzt eine Zeitscheibe zum Prüfen, daher auf 1-CPU-Systemen nicht sonderlich performant, auf Multi-CPU Systemen ganz OK, solange Anzahl der Threads \~ Anzahl der CPUs

## Ansatz 4: Compare-and-Swap

Wird ebenfalls in gängigen Systemen (x86 via `cmpxchg`) eingesetzt2

* Prüft, ob der Wert an Adresse von `ptr` dem Wert von `expected` entspricht
* Falls ja, wird der Wert von `new` gesetzt
* Dann wird der (unveränderte) Wert von `original`zurück geleifert
* Nutzung analog zu Test-and-Swap

```
int CompareAndSwap(int *ptr, int expected, int new) {
  int original = *ptr;
  if (original == expected)
    *ptr = new;
  return original;
}
```

Yield\



* Anstelle zu warten, kann ein Thread sich selbst mittels `yield` »entschedulen«, d.h. ein anderer Thread wird anstelle dessen geplant
* Funktioniert gut bei zwei Threads
* Was aber, wenn viele Threads um das Lock konkurrieren?
* Dann werden sehr viele Context Switches ausgeführt, und wie wir wissen, lohnen sich diese erst ab einer gewissen Laufzeit

```
void init() { 
  flag = 0;
}

void lock() {
  while (TestAndSet(&flag, 1) == 1)
    yield(); // give up the CPU
}

void unlock() {
  flag = 0;
} 
```

\


* Bisher wird alles mehr oder weniger dem Zufall überlassen.
* Der Scheduler entscheidet darüber, welcher Thread als nächstes ausgeführt wird
* Je nachdem welcher Thread ausgewählt wird, wartet dieser auf das Lock oder entschedult (mittels `yield`) sich, während ein anderer Thread hätte ausgeführt werden können
* In beiden Fällen ist dies reine Verschwendung von Rechenkapazität

Wie kann diese Situation verbessert werden? Hier hilft uns das Betriebssystem!

## Referenzen

1. [https://c9x.me/x86/html/file\_module\_x86\_id\_328.html](https://c9x.me/x86/html/file_module_x86_id_328.html)&#x20;
2. [https://c9x.me/x86/html/file\_module\_x86\_id\_41.html](https://c9x.me/x86/html/file_module_x86_id_41.html)&#x20;

## &#x20;           
