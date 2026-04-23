# Exkurs: Spurious Wakeups

**Spurious Wakeups** (auf deutsch: _scheinbare oder unbegründete Aktivbierung_) kommen bei **Threads** und **Condition Variables** vor.

**Idee:**\
Ein Thread kann aus dem Warten aufwachen, **ohne dass jemand ihn absichtlich geweckt hat** und **ohne dass die Bedingung erfüllt ist**.

#### Einfaches Beispiel

Ein Thread wartet:

```c
wait(condition);
```

Er wacht auf - **aber die Bedingung ist noch falsch**.

#### Warum passiert das?

* Betriebssystem-Optimierungen
* Mehrere Threads werden gleichzeitig geweckt
* Implementierungsdetails der Synchronisation

> **Wichtig:** **Das ist kein Fehler, sondern erlaubt!**

#### Konsequenz (Merksatz)

> **Nach dem Aufwachen immer prüfen, ob die Bedingung wirklich gilt.**

Darum schreibt man:

```c
while (!logicalCondition) { // logische Bedingung
  wait(wakeUpCondition);    // Synchronisationsmechanismus, zusammen mit einem Mutex
}
```

#### Hinweis:&#x20;

#### Da dies verwirrend sein kann:&#x20;

* **`logicalCondition`** → _logische Bedingung im Programm_ (z. B. `buffer_not_empty`)
* **`wakeUpCondition`** → _Conditional Variable_, nur ein **Weckmechanismus,** funktioniert nur zusammen mit einem Mutex

**Daher:** Ein _spurious wakeup_ ist ein unerwartetes Aufwachen eines Threads - deshalb **nie `if`, immer `while` beim Warten verwenden**.

