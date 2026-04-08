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

```
top
```

und mit&#x20;

```
htop
```

**Hinweis:** `yes > /dev/null &` ist eine klassischer Scheduler‑Testbefehl. Er erzeugt maximale CPU‑Last, ohne Ausgaben zu erzeugen. `yes` schreibt endlos eine Zeichenkette auf die Standardausgabe, rechnet nichts, verbraucht aber so viel CPU wie möglich.  `> /dev/null` leitet die Standardausgabe nach _/dev/nu&#x6C;_&#x6C; um. _/dev/null_ ist ein spezielles Device (Gerät), das alle Daten, die dort hineingeschrieben werden, sofort und endültig verschwinden. In unserem Fall wird dadurch keine I/O verbraucht und Du generierst reine CPU-Last. Das  Ampersand-Symbol `&` startet den Prozess im Hintergrund. Sieh hierzu auch die [Übungsaufgauben zu Prozessen](../virtualization/prozesse-ubungsaufgaben.md).  Der Teil taskset -c 0 führt dazu, dass der Prozess auf CPU/Core 0 ausgeüfhrt wird. SOmit muss der Scheduler die CPU-Leiistung aufteilen.

Du wirst sehen, dass jede CPU ungefähr 33% CPU Leistung erhält. Stehen mehreer CPUs zur Verfügung sollte jeder Prozess 100% CPU-Leistung erhalten - achte in diesem Fall auf die Zeit, die die Prozesse verbraucht haben. Die Werte sollten nahe beieinander liegen. Führe die Übung später nochmal aus - lasse dann aber den Teil `taskset -c 0` weg.

Beende nun alle Prozesse mit dem Befehl

```
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

* **Jeder Benutzer darf seine Prozesse „netter“ machen** → also den _nice_‑Wert **erhöhen** (z. B. von 0 auf 10 oder 19)
* **Nur root darf Prozesse „unhöflicher“ machen** → also den _nice_‑Wert **senken** (z. B. von 0 auf −5)

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

```
renice 15 -p <pid>
```

Anstelle \<pid> gib die ermittelte Prozess-ID an, z.B. `renice 15 -p 1432` .

Beobachte nochmals den CPU-Verbrauch der Prozesse mittels `htop`. Du solltest nun beobachten können, dass der CPU-Verbrauch des Prozesses unter den mit dem _nice_-Wert 15 fällt.



