# Exercise: Process Creation

## **Aufgaben**

**Ziel**: Entwickeln Sie ein C-Programm, das die Erzeugung, Ausführung und das Monitoring von Prozessen auf der Konsole ermöglicht. Das Programm soll es dem Benutzer ermöglichen, neue Prozesse zu starten, den Status laufender Prozesse anzuzeigen und Prozesse zu beenden.

**Anforderungen**:

1. **Prozesserzeugung**: Das Programm sollte den Benutzer auffordern, den Namen eines ausführbaren Programms (z.B. ein anderes C-Programm, das bereits kompiliert wurde) einzugeben. Anschließend wird dieses Programm als neuer Prozess gestartet.
2. **Prozessüberwachung**: Nach der Erzeugung eines Prozesses soll das Programm dessen PID (Prozess-ID) auf der Konsole ausgeben. Der Benutzer kann eine Liste aller derzeit vom Prozessmanager verwalteten Prozesse anzeigen lassen, einschließlich ihrer PIDs.
3. **Prozessbeendigung**: Der Benutzer soll die Möglichkeit haben, einen Prozess durch Eingabe seiner PID zu beenden.

**Hinweise zur Implementierung:**

* Verwenden Sie die `fork()`- und `exec()` bzw. deren Varianten um neue Prozesse zu erstellen. `fork()` erzeugt einen neuen Prozess durch Duplizieren des aufrufenden Prozesses, und `exec()` ersetzt den Prozessspeicher mit einem neuen Programm. Für die Unterschiede lesen Sie z.B. den Stackoverflow-Artikel [Difference between exec, execvp, execl, execv?](https://stackoverflow.com/questions/55743496/difference-between-exec-execvp-execl-execv).
* Verwenden Sie `wait()` oder `waitpid()`-Systemaufrufe, um auf die Beendigung von Prozessen zu warten.&#x20;
* Für die Beendigung von Prozessen können Sie auch den `kill()`-Systemaufruf verwenden, der es ermöglicht, Signale an Prozesse zu senden (z.B. SIGKILL zum Beenden eines Prozesses).
* Um den Status von Prozessen zu überwachen, können Sie eine einfache Datenstruktur (z.B. eine Liste oder ein Array) verwenden, um die PIDs der gestarteten Prozesse zu speichern.

**Hinweise zur Gestaltung**:

Es genügt, wenn Sie eine einfache Abfrage über die Kommandozeile realisieren. Nutzen Sie hierfür z.B. `printf()` um ein kleines Menü auszugeben und `scanf()`und `fgets()` um die Eingaben der Anwender auszulesen:

Nach dem Start könnte das Programm folgende Ausgabe auf der Konsole zeigen:

```shell-session
Einfacher Prozessmanager
Optionen:
1. Neuen Prozess starten
2. Prozess beenden (PID)
3. Exit
Wählen Sie eine Option: 

```

Wenn Sie `1` eingeben und die Eingabetaste drücken, fragt das Programm nach dem Namen eines auszuführenden Programms:

```shell-session
Geben Sie den Befehl für den neuen Prozess ein: 
```

Sie könnten z.B. `vi` (einen Texteditor unter Linux) eingeben. Das Programm startet dann `vi` und zeigt dessen PID:

```
Prozess gestartet mit PID: 12345
```

etc.

**Hinweis zur Abgabe:**

Sie checken das Programm in Ihrem Git Repository unter einem ordern _process\_manager_ ein.
