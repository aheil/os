# Exkurs: Bugs durch Nebenläufigkeit

| Anwendung  | Typ          | Non-Deadlock | Deadlock |
| ---------- | ------------ | ------------ | -------- |
| MySQL      | Datenbank    | 14           | 9        |
| Apache     | Web Server   | 13           | 4        |
| Mozilla    | Wen Browser  | 41           | 16       |
| OpenOffice | Office Suite | 6            | 2        |

Basierend auf einer Studie \[1] lassen sie die Fehler zunächst nach **Deadlock und Nicht-Deadlock** Fehlern aufgliedern. 1/3 der Fehler lassen sich dabei auf Deadlocks zurückführen.

Warum ist das interessant? Zu verstehen, wo die Fehler herkommen, ermöglicht es uns bessere und robustere Software zu schreiben. Da gilt übrigens nicht nur für Betriebssysteme!

Noch einige Daten aus der Studie:

* Sucht man nach den nachfolgen genannten beiden Fehlern, findet man den größten Teil der Nicht-Deadlock-Fehler.
* **Die meisten Nebenläufigkeitsfehler entstehen durch 1 oder 2 Threads** (also nicht durch viele Threads)
* **Mehr als 20% der Deadlock-Fehler entstehen in einem einzigen Thread**
* **Die meisten Deadlock-Fehler entstehen durch den Zugriff auf nur 1 oder 2 Ressourcen**

## Atomic Violation und Order Violation

Zwei Fehlerklassen bei Nicht-Deadlocks sind für uns von besonderem Interesse:

* **Atomic Violation** (Zugriff auf eine geschützte Ressource)
* **Order Violation** (Einhaltung einer Reihenfolge)

\
Atomic Violation
----------------

> **Definition Atomic Violation**: The desired serializability among multiple memory accesses is violated (i.e. a code region is intended to be atomic, but the atomicity is not enforced during execution).” \[1]

Beispiel aus MySQL:

Thread 11

```
if (thd->proc_info) 
{ 
  fputs(thd->proc_info, ...); 
}
```

Thread 2:

```
thd->proc_info = NULL;
```

* Thread 1 prüft auf Nicht-Null, um dann einen Wert zu setzen
* Wird Thread 1 nach der Prüfung (also vor dem Setzen des Wertes unterbrochen) und Thread 2 setzt den Pointer auf Null, dann passiert was?
* Referenz ist auf Null gesetzt und wenn Thread 1 wieder läuft, wird das Programm vermutlich abstürzen…

**Lösung:** Den kritischen Abschnitt durch ein Lock (Mutex, binäre Semaphore) schützen.

## Order Violation

> **Definition: Order Violation:** The desired order between two (groups of) memory accesses is flipped (i.e., A should always be executed before B, but the order is not enforced during execution)” \[1]\
>

Thread 1:

```
void init() {
  mThread = PR_CreateThread(mMain, ...);
}
```

Thread 2:

```
void mMain(...) {
  mState = mThread->State;
}
```

Was passiert hier?

* Thread 2 geht davon aus, dass `mThread` bereits initialisiert ist (d.h. != Null)
* Wenn Thread 2 sofort läuft, nachdem er erzeugt wurde, konnte Thread 1 die Referenz aber noch nicht zuweisen
* Ist das klar warum? Thread 1 wurde dann ja direkt vor der Zuweisung unterbrochen…

## **Fazit Nicht-Deadlock-Fehler**

*
  * **97% der nicht auf Deadlocks basierenden Fehler sind die beiden zuvor behandelten Fehlertypen** (Atomic Violation bzw. Order Violation)
  * **Statische Code-Analyse** kann hier helfen die Fehler vorzubeugen
  * Die **Fehler sind oft nicht leicht zu finden**
  * Die **Fehlerbehebung erfordert oft das Refactoring größerer bzw. großer Mengen von Code** oder von Datenstrukturen

## Deadlocks

Wie entsteht eigentlich ein Deadlock?

Thread 1:&#x20;

```
pthread_mutex_lock(L1); 
pthread_mutex_lock(L2);
```

Thread 2:

```
pthread_mutex_lock(L2); 
pthread_mutex_lock(L1);
```

* Thread 1 hält Lock `L1`, wartet auf Lock `L2`, allerdings hält Thread 2 das Lock `L2` und wartet auf `L1`
* Problem: Der Deadlock muss nicht zwangsweise auftreten

<figure><img src="https://raw.githubusercontent.com/aheil/os/main/img/os.09_1.deadlock.png" alt=""><figcaption></figcaption></figure>

Ursachen für Deadlocks

* Komplexität von Programmen – am Beispiel Betriebssysteme und dem Zugriff auf Ressourcen vermutlich intuitiv sichtbar
* **Kapselung**: Versteckt Implementierungsdetails vor dem Aufrufer… aber auch andere Dinge. Beispiel Java Vector Klasse:
* Beispiel: Java Klasse Vector

```
Vector v1, v2;
v1.AddAll(v2); 
```

* Methode muss multi-thread-safe sein um Effekte wie zuvor beschrieben zu vermeiden
* Daher wird ein Lock für beide Variablen (`v1` und `v2`) angefordert
* Ruft ein anderer Thread nun `v2.AddAll(v1)` auf besteht Potential für ein Deadlock

## **Bedingungen für Deadlocks**

Damit ein Deadlock entstehen kann müssen **vier Bedingungen** entstehen:

* **Mutual Exclusion:** Threads fordern exklusiven Zugriff auf eine Ressource an
* **Hold-an-Wait:** Threads halten Ressourcen, die sie einmal erhalten haben (z.B. Locks) wenn Sie auf weitere Ressourcen (z.B. zusätzliche Locks) warten
* **No Preemtion:** Ressourcen können einem Thread nicht (gewaltsam) entzogen werden
* **Circular Wait** Es existiert eine zirkuläre Beziehung zwischen Threads, die eine oder mehrere Ressourcen blockieren, die von anderen Threads in dieser Kette angefordert werden (vgl. Grafik zuvor)

> Anders ausgedrückt: trifft eine der Bedingungen nicht zu, kann kein Deadlock entstehen, das kann helfen, wenn man versucht Deadlocks zu vermeiden. Wir müssen nämlich nur eine er vier Bedingungen auflösen!

## Aufgabe

Erstellen Sie ein Java Programm, das einen Deadlock provoziert:&#x20;

* Implementieren Sie eine Klasse für einen Sende-Empfänger.&#x20;
* Starten Sie für zwei Sende-Empfänger jeweils einen Thread.
* Empfange wird, indem die Sende-Methode in eine entsprechende Variable des anderen Empfängers schreibt\
  (Hinweis: Threads haben auf den gleichen Heap innerhalb des Prozesses Zugriff)
* Provozieren Sie nun einen Deadlock aufgrund folgenden Verhaltens: Ein Sender kann keine Nachricht empfangen, solange er senden möchte, bzw. empfängt ein Sender eine Nachricht, kann er keine Nachricht versenden.&#x20;
* Tipp: Nutzen Sie das `synchronized` Keyword in Java, um das gewünschte Verhalten zu provozieren.&#x20;

Checken Sie das Code-File in Ihrer Gruppe unter dem Ordner  sender\_receiver in Ihrem Repository ein.

\[1] “Learning from Mistakes — A Comprehensive Study on Real World Concurrency Bug Characteristics” by Shan Lu, Soyeon Park, Eunsoo Seo, Yuanyuan Zhou. ASPLOS ’08, March 2008, Seattle, Washington
