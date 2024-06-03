# Lab 03: Semaphore

## Voraussetzungen

Nutzen Sie hierfür [gcc](https://gcc.gnu.org/).

Sie können _gcc_ unter Linux und/oder macOS direkt verwenden. Unter Linux installieren Sie _gcc_ über Ihren Paket-Manager, unter macOX über [Homebrew](https://brew.sh/).

Nutzen Sie Windows 10 können Sie entweder eine Linux in einer virtuellen Maschine (z.B. in [VirtualBox](https://www.virtualbox.org/)) nutzen, oder direkt das [Windows Subsystem for Linux (WSL 2.0)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) nutzen. Alternativ ist es möglich _Windows Subsystem for Linux_ über den Windows App Store zu installieren.

In der Wahl der Linux Distribution sind sie frei, alle Beispiele in der Vorlesung werden jedoch unter Ubuntu (letzter stabiler Release) bewertet und eventuell vorgestellt.

## Aufgabenstellung

1. Implementieren Sie eine Semaphore auf Basis folgender Header-Datei (`sem.h`):



```c
typedef struct
{
    // ...
} hhn_sem_t;

int hhn_sem_wait(hhn_sem_t *s);
int hhn_sem_post(hhn_sem_t *__sem);
int hhn_sem_init(hhn_sem_t *__sem, int __pshared, int __value);
int hhn_sem_destroy(hhn_sem_t *__sem);
```

2. Ziel ist es die implementierung aus `semaphore.h` analog durch die implementierung aus `sem.h` zu ersetzen.



```c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void *thread_func(void *arg)
{
    int i;
    for (i = 0; i < 10; i++) {
        sem_wait(&sem);
        printf("Thread running\n");
        sem_post(&sem);
    }
    return NULL;
}

int main(void)
{
    pthread_t thread;
    sem_init(&sem, 0, 1);
    pthread_create(&thread, NULL, thread_func, NULL);
    int i;
    for (i = 0; i < 10; i++) {
        sem_wait(&sem);
        printf("Main thread running\n");
        sem_post(&sem);
    }
    pthread_join(thread, NULL);
    sem_destroy(&sem);
    return 0;
}
```

Dieses Programm erstellt eine Semaphore namens `sem` und initialisiert sie mit dem Wert 1. Es erstellt dann einen neuen Thread, der die Funktion `thread_func` ausführt. In der Hauptfunktion und im neuen Thread wird jeweils eine Schleife ausgeführt, in der die Semaphore mit der Funktion `sem_wait` gesperrt wird, um den Zugriff auf eine gemeinsam genutzte Ressource zu synchronisieren. Nachdem die gemeinsam genutzte Ressource verwendet wurde, wird die Semaphore mit der Funktion `sem_post` freigegeben. Am Ende des Programms wird die Semaphore mit der Funktion `sem_destroy` freigegeben.

3. Implementieren Sie eine Struktur `hhn_sem_t` und die, für das vorherige Programm erforderlichen Funktionen (`hhn_sem_wait`, `hhn_sem_post`, `hhn_sem_init`, `hhn_sem_destroy`).
4. Deklarieren Sie alle dafür erforderlichen Funktionen in einer Header Datei `sem.h`, tauschen Sie den Eintrag entsprechend in Ihrem Programm, so dass Ihre Implementierung der Semaphore verwendet wird.

```c
#include <stdio.h>
#include <pthread.h>
#include "sem.h" 
```

5. Zur Abgabe reichen Sie sowohl Ihr Hauptprogramm, das neue Header-File (`sem.h`) als auch die Datei mit der Implementierung Ihrer Semaphore ein.

> Hinweis: Ziel der Aufgabenstellung ist es, dass Sie die Grundfunktionalität einer Semaphore nachbilden können. Die Implementierung Ihrer Semaphore muss daher nicht Thread-sicher sein.

### Abgabe

Die Bewertung Ihrer Abgabe findet automatisch statt. Stellen Sie hierzu folgende Punkte sicher:

* Ihre Lösung befindet sich im Ordner **aufgabe3**.
* Ihre Implementierung befindet sich in einer Datei mit dem Namen **sem.c**.
* Sie nutzen eine Header-Datei in der der obige Header hinterlegt ist.
* Zur Abgabe erhalten Sie einen Zugang zum hochschulinternen [GitLab](https://git.it.hs-heilbronn.de/).
* Ihre Lösung checken Sie in Ihrem Repository ein.

### Bewertung

* Die Bewertung Ihrer Aufgabe findet anhand einer Reihe von automatisierten Tests statt.
* Hierzu ist es erforderlich, dass die obigen Schritte exakt eingehalten werden.
* Ihr Implementierung wird einer Reihe von automatisierten Tests unterzogen, die Ihre Implementierung auf Korrektheit überprüfen.
* Abgaben, die nicht vollständig sind oder die Abgabekriterien nicht erfüllen werden nicht bewertet.
* Abgaben, die nicht fristgerecht eingereicht werden, werden nicht bewertet.
* Nutzen Sie zur Abgabe ausschließlich das beschriebene Verfahren. Abgaben, die per E-Mail oder anderen Wegen eingereicht werden, werden nicht bewertet.
* Abgaben, die aufgrund eines Fehlers nicht durch die Tests laufen, werden entsprechend mit weniger Punkten bewertet.
