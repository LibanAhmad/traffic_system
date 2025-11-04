# 🧠 Utvecklingsfilosofi: Modularisering och OOP

Den största lärdomen i detta projekt var att framgången låg i att **jag delade upp det komplexa problemet** i mindre, modulära C++-klasser. Detta är grunden i Object-Oriented Programming (OOP), och det gjorde att jag kunde säkerställa att varje del hade ett enskilt ansvar (Single Responsibility Principle).

Jag gav varje klass en specifik roll:

* **Vehicle:** Bär endast data.
* **Roads:** Kapslar in och hanterar fordonsköerna (med std::queue).
* **TrafficLight:** Håller endast tillståndet (Röd, Gul, Grön) och saknar egen tidtagning.
* **Intersection:** Centraliserar all styrlogik, hanterar cykeln (State Machine) och konflikterna.

***

# ⚙️ Lärdomar om C++ (Den Stora Utmaningen)

Den största utmaningen i projektet var **inte själva problemlösningen** (trafikreglerna), utan att tvinga mig själv att tänka utifrån **hur C++:s strikta system fungerar**, jämfört med ett flexibelt språk som Python som jag är van vid.

Jag var tvungen att hantera följande:

1.  **Typ-säkerhet (`enum class`):** Jag bytte ut vaga strängar/heltal mot strikta `enum class`-typer, vilket tvingade mig att tänka säkrare från grunden.
2.  **Explicit Kontroll:** Jag insåg att jag måste vara explicit med referenser och tvinga fram tillstånd. Intersection måste tvinga fram färgen på TrafficLight via en Setter, istället för att låta ljuset sköta det automatiskt.
3.  **Containrar:** Att använda `std::queue` för köer visade sig vara det mest optimerade och strikta sättet att hantera fordonsflödet i C++.

**Slutsats:** Genom att bygga upp systemet modulärt, har jag skapat en robust och effektiv lösning, samtidigt som jag bemästrat de nödvändiga principerna för C++-utveckling.
