# Prozesse: Übungsaufgaben

## **Aufgaben**

**Ziel**: Du lernst den grundlegenden Umgang mit Prozessen unter Linux/Unix.

## Aufgabe 1: Prozesse erzeugen und beobachten&#x20;

Starte in einem Terminal einen Prozess, der längere Zeit läuft. Möglich ist dies z.B: mit dem Befehl `sleep`.

```
sleep 120 &
```

Notiere die ausgegebene PID und beobachte den Prozess mit:

* `ps -f`
* `pstree`
* `top`

## Aufgabe 2: Prozesse in den Hintergrund schicken

Starte einen Prozess im Vordergrund:

```
ping 8.8.8.8
```

Pausiere nun den Prozess mit `CTRL-Z` bzw. `STRG-Z` .

Schicke den Prozess nun in den Hintergrund:&#x20;

```
bg
```

Hole den Prozess wieder in den Vordergrund:

```
fg
```

**Was macht** `CTRL+Z` in der Konsole?

`TRL+Z` **sendet das Signal** `SIGTSTP` an den aktuell laufenden Vordergrundprozess.

Das bewirkt:

1. Der Prozess wird angehalten (gestoppt)\
   Er läuft nicht weiter, sondern friert ein — aber der **Prozess wird nicht beendet.**
2.  Der Prozess wird in den Hintergrund verschoben\
    Die Shell meldet typischerweise:<br>

    ```
    [1]+ Stopped ping 8.8.8.8
    ```

&#x20;      Damit wird er zu einem **Job**, der im Hintergrund „wartet“.

3. Prüfe dies über den Befehl `jobs` . Du solltest die gleiche Ausgabe wie zuvor erhalten.&#x20;
4.  Starte nun einen weiteren Prozess `sleep 120 &` - führe nochmals den Befehl `jobs` aus.\
    Du solltest nun eine ähnliche Ausgabe wie folgt sehen:<br>

    ```
    [1]+  Stopped                 ping 8.8.8.8
    [2]-  Running                 sleep 120 &
    ```

Der Befehl jobs zeigt dir demnach alle laufenden und gestoppten Prozesse, die aus deiner Konsole (Shell) gestartet wurden. Ein Job ist dabei kein Prozess, sondern kann mehrer Prozesse enthalten, die von der Shell verwaltet werden. Die Nummer in Klammern gibt dabei die Shell-interne Job-Nummer an - **dabei handelt es sich** **nicht um die Prozess-ID** (pid)

