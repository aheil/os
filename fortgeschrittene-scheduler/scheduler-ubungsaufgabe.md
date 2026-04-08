# Scheduler: Übungsaufgabe

## **Aufgaben**

**Ziel**: Du lernst den grundlegenden Umgang mit Schedulern unter Linux/Unix.

## Übung 1: Fair Scheduling (CFS-Grundidee)

**Ziel**: Verstehen, dass der **Completely Fair Scheduler** CPU‑Zeit möglichst gleich verteilt.

Starte mehrere CPU-lastige Prozesse und beobachte die CPU Verteilung. Führe hierzu in der Shell folgende drei Befehle aus:

```bash
taskset -c 0 yes > /dev/null &
taskset -c 0 yes > /dev/null &
taskset -c 0 yes > /dev/null &
```

Beobachte die Prozesse mit&#x20;

```bash
top
```

und mit&#x20;

```bash
htop
```

**Hinweis:** `yes > /dev/null &` ist eine klassischer Scheduler‑Testbefehl. Er erzeugt maximale CPU‑Last, ohne Ausgaben zu erzeugen. `yes` schreibt endlos eine Zeichenkette auf die Standardausgabe, rechnet nichts, verbraucht aber so viel CPU wie möglich.  `> /dev/null` leitet die Standardausgabe nach _/dev/nu&#x6C;_&#x6C; um. _/dev/null_ ist ein spezielles Device (Gerät), das alle Daten, die dort hineingeschrieben werden, sofort und endültig verschwinden. In unserem Fall wird dadurch keine I/O verbraucht und Du generierst reine CPU-Last. Das  Ampersand-Symbol `&` startet den Prozess im Hintergrund. Sieh hierzu auch die [Übungsaufgauben zu Prozessen](../virtualization/prozesse-ubungsaufgaben.md).  Der Teil taskset -c 0 führt dazu, dass der Prozess auf CPU/Core 0 ausgeüfhrt wird. SOmit muss der Scheduler die CPU-Leiistung aufteilen.

Du wirst sehen, dass jede CPU ungefähr 33% CPU Leistung erhält. Stehen mehreer CPUs zur Verfügung sollte jeder Prozess 100% CPU-Leistung erhalten - achte in diesem Fall auf die Zeit, die die Prozesse verbraucht haben. Die Werte sollten nahe beieinander liegen. Führe die Übung später nochmal aus - lasse dann aber den Teil `taskset -c 0` weg.

Beende nun alle Prozesse mit dem Befehl

```bash
killall yes
```

## Übung 2: Prioritäten mit nice

**Ziel:** Einfluss von Prioritäten auf den Scheduler verstehen.

Starte drei Prozesse mit unterschiedlichen _nice-_&#x57;erte&#x6E;**:**

```bash

taskset -c 0 yes > /dev/null &
taskset -c 0 nice -n 10 yes > /dev/null &
taskset -c 0 nice -n 19 yes > /dev/null &

```

**Dur wirst nun sehen, dass die Prozesse unterschiedlich viel Zeit erhalten:**

* `nice 0` **→ meiste CPU**
* `nice 10` → weniger CPU
* `nice 19` → deutlich weniger CPU

Beende nun wieder alle Prozesse mit `killall yes` .

**Hinweis:** Unter Linux gilt:

* **Jeder Benutzer** **darf** seine Prozesse „netter“ machen → also **den&#x20;**_**nice**_**‑Wert erhöhen** (z. B. von 0 auf 10 oder 19)
* **Nur&#x20;**_**root**_ **darf** Prozesse „unhöflicher“ machen → also den _**nice**_**‑Wert senken** (z. B. von 0 auf −5)

Niedriger _nice_‑Wert bedeutet:

* geringere `vruntime`
* Prozess bekommt **mehr CPU‑Zeit**
* andere Prozesse bekommen **weniger**

Das betrifft **alle anderen Benutzer und Prozesse**.

Mit dem Befehl

```bash
nice -n -20 mein_prozess
```

wäre es im Prinzip möglich alle anderen Prozesse verhungern zu lassen.

## Übung 3: Dynamische Änderung von Prioritäten

**Ziel:** Lernen, dass dass Scheduler-Verhalten zur Laufzeit angepasst werden können.

Starte wie in Übung 2 drei Prozesse mit unterschiedlichen nice-Werten (alle auf CPU/Core 0).

Beobachte den CPU-Verbrauch der drei Prozesse mittels `htop`.&#x20;

Finde nun die Prozess-ID (_pid_) des Prozesses mit dem _nice_-Wert 0. Ändere nun den _nice_-Wert wie folgt:

```bash
renice 15 -p <pid>
```

Anstelle \<pid> gib die ermittelte Prozess-ID an, z.B. `renice 15 -p 1432` .

Beobachte nochmals den CPU-Verbrauch der Prozesse mittels `htop`. Du solltest nun beobachten können, dass der CPU-Verbrauch des Prozesses unter den mit dem _nice_-Wert 10 fällt.

## Übung 4: CPU-bound vs. I/O-bound

**Ziel:** Verstehen warum interaktive Programme ansprechbar bleiben&#x20;

Öffne drei Terminals (Shells) bzw. unter Windows WSL drei Fenster mit der gleichen Linux-Instanz.&#x20;

In Terminal 1 starte

```bash
 taskset -c 0 yes > /dev/null
```

In Terminal 2 starte&#x20;

```bash
taskset -c 0 watch -n 1 date
```

In Terminal 3 starte&#x20;

```bash
htop
```

Hinweis:&#x20;

`watch -n 1 date` macht immer wieder:

1. `sleep` (Timer blockiert)
2. `date` ausführen (kurz CPU)
3. Ausgabe ins Terminal (I/O)
4. wieder schlafen

`watch -n 1 date` ist I/O‑intensiv, da es nur kurze CPU‑Bursts ausführt und den Großteil der Zeit auf Timer‑ und Ein-/Ausgabe‑Operationen wartet und somit blockiert. `yes` hingegen ist ein CPU‑lastiger Prozess, der permanent rechnet und nie blockiert.

**Hinweis:** An diesem Beispiel kannst Du die Funktionsweise von Preemtion erkennen. _date_ aktualisiert sich flüssig, obwohl _yes fast_ die gesamte CPU blockiert. _yes_ wird jedoch ständig unterbrochen um _date_ auszuführen.

## Übung 5: Echtzeitschedulung und Round Robin&#x20;

Starte nun&#x20;

```
sudo taskset -c 0  chrt -r 50  yes > /dev/null &
sudo taskset -c 0  chrt -r 50  yes > /dev/null & 
```

**Hinweis:** Der Befehl startet einen **Echtzeit‑Prozess mit Round‑Robin‑Scheduling**, Priorität **50**, der **endlos CPU verbraucht** und **im Hintergrund läuft**.

Mit `chrt`  (change real‑time scheduling) wird die Scheduler‑Klasse und die Echtzeit‑Priorität eines Prozesses gesetzt. `-r` steht für Round-Robund (SCHED\_RR) - diese Prozesse laufen for normalewn CFS-Prozessen. Mit 50 wird die Echtzeit-Priorität angegeben (1-99) - je grpßer die Zahl ist, desto höher ist die Priorität.

Als Faustrtegel gilt: Niedrige Real-Time-Prozesse verlieren gegenüber hohen Real-TimeProzessen, normales Scheduling verliert immer gegenüber Real-Time Prozessen,

## **Übung 6: Hart am Limit**

Führe diese Übung nur durch, wenn Du die vorherigen Übungen durchgearbeitet hast und dich auf der Konsole sicher fühlst.

**Ziel:** Dein System für kurze nahezu unbrauchbar machen.

Start ezunächst zwei Shelles (oder unter Widows WSL zwei linux-Fenster). Führe nun für jede CPU folgenden Befehl aus:

```bash
sudo chrt -f 80 taskset -c 0 yes > /dev/null &
```

Ersetze dabei bei jedem Aufruf `0` durch die nächste Ziffer (1, 2, 3 etc.) - abhängig davon wieviele CPUs in deinem Rechner verbaut sind. Hierdurch wird ein hoch-priorisierter Real-Time-Prozess auf der CPU gestartet.&#x20;

Nach dem Start auf der vorletzten bzw. letzten CPU sollte das Terminal beginnen sehr verzögert zu reagieren. Grund hierfür ist, dass die Real-Time-Prozesse nun (fast) alle CPUs blockiere und alle anderen Prozesse im Scheduling verlieren.

Beende nun alle Prozesse mit

```shellscript
sudo killall yes
```

Dein System sollte schlagartig wieder reagieren.
