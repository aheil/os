# Einheit 2: C Programmierung

C ist eine Programmiersprache, die in der Betriebssystemprogrammierung aus verschiedenen Gründen weit verbreitet ist:

**Hardwarezugriff**: In C kann **Hardware direkt angesprochen** werden. Dies ist für Treiberentwicklung und Systemprogrammierung notwendig.

**Effizienz**: Mit C lässt sich effizient entwickeln. Es ermöglicht die **direkte Steuerung von Hardware** und bietet eine lässt sich leicht in Assemblercode umsetzen. Diese Effizienz ist relevant für Betriebssysteme, die mit beschränkten Ressourcen arbeiten.

**Portabilität**: C ist portabel und kann mit relativ wenig Aufwand **auf verschiedenen Plattformen eingesetzt** werden. Dies ist wichtig für Betriebssysteme, die unterschiedlichen Architekturen adressieren.

Warum sind sog. managed Sprachen wie Java oder .NET nicht für doe Betriebssystemprogrammierung geeignet?

**Bootstrapping**: Betriebssysteme müssen sich **selbst initialisieren**. Die Verwendung von Managed Sprachen für diesen Zweck ist problematisch, da sie auf eine bereits laufende Laufzeitumgebung angewiesen sind.

**Abhängigkeit von Ressourcen**: Betriebssysteme müssen auf **unterster Ebene** mit Hardware-Ressourcen interagieren. Managed Sprachen sind abstrahiert und verbergen viele Details. Dies kann zu Problemen bei der direkten Steuerung von Hardware führen.

**Vorhersagbarkeit**: Betriebssysteme müssen **deterministisch** und **vorhersagbar** sein. Managed Sprachen haben z.B. eine **Garbage Collection** und andere automatisierte Prozesse, die schwer vorhersehbar sind und damit die V**orhersagbarkeit** beeinträchtigen können.

**Performance**: Managed Sprachen wie **Java** oder .NET führen aufgrund ihrer **Laufzeitumgebung** (z. B. der **Java Virtual Machine** oder des **.NET Common Language Runtime**) eine zusätzliche Schicht und somit zusätzlichen Overhead ein. Das wirkt sich negativ auf die **Performance** aus und ist in Betriebssystemen, die eigentlich auf **Effizienz** angewiesen sind, problematisch.

\


\


\


\


\
