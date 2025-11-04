Utvecklingsfilosofi: Modularisering och OOP

Den viktigaste insikten i detta projekt var att framgången kom av att dela upp det komplexa problemet i mindre, modulära C++-klasser. Detta är kärnan i objektorienterad programmering (OOP), och det gjorde det möjligt att ge varje del ett tydligt och avgränsat ansvar enligt Single Responsibility Principle.

Varje klass fick en specifik roll:

Vehicle: Håller endast data.

Roads: Kapslar in och hanterar fordonsköer med std::queue.

TrafficLight: Håller endast tillståndet (Röd, Gul, Grön) och har ingen egen tidtagning.

Intersection: Centraliserar styrlogik, hanterar cykler (state machine) och konflikter.

Lärdomar om C++ (Den stora utmaningen)

Den största utmaningen i projektet handlade inte om trafiklogiken i sig, utan om att anpassa mitt sätt att tänka till C++ och dess mer strikta system jämfört med Python som jag är mer van vid.

Några centrala lärdomar:

Typ-säkerhet med enum class
Jag ersatte vaga strängar och heltal med strikta enum class-typer, vilket tvingade fram säkrare design från början.

Tydlig kontroll och ansvar
Jag behövde vara uttrycklig med referenser och tillstånd. Exempelvis måste Intersection aktivt sätta färg på TrafficLight via en setter i stället för att låta ljuset sköta allt självt.

Korrekt val av datastrukturer
Att använda std::queue för fordonsköer visade sig vara det mest logiska, strikta och optimerade sättet att hantera trafikflödet.

Slutsats: Genom att bygga systemet steg för steg med tydliga ansvar och strikt typkontroll kunde jag skapa en robust och skalbar trafik­simulering i C++.modulärt, har jag skapat en robust och effektiv lösning, samtidigt som jag bemästrat de nödvändiga principerna för C++-utveckling.
