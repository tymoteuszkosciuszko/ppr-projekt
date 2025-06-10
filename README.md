# ppr-projekt
Zadanie indywidualne w ramach przedmiotu Podstawy Przetwarzania Rozproszonego na WAT

*Tymoteusz Kościuszko, WCY22KC2S1*

## Wybór technologii
- ⬇️ Proces 1 (wejście): **C**
- ⚙️ Mechanizm 1: **Socket/TCP**
- 🔄 Proces 2: **PHP**
- ⚙️ Mechanizm 2: **Socket/UDP**
- 🔄 Proces 3: **Python**
- ⚙️ Mechanizm 3: **XML-RPC**
- ⬇️ Proces 4 (wyjście): **Perl**

Numer w Excelu: 496

## Sposób uruchamiania
Programy są pozanywane względem kolejności jaką przebędą dane od wejścia do wyjścia. Po skompilowaniu programu C, należy uruchamiać skrypty w odwrotnej kolejności, tj. począwszy od skryptu Perl *4-xml-stdout.pl*. Jego wyjście można przekierować poprzez STDOUT do pliku oraz porównać go potem z plikiem oryginalnym.