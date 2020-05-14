Systemy operacyjne 2
==============================================================
Symulator gry *Heroes of Might and Magic III*
--------------------------------------------------------------

## 1. Opis
Celem projektu jest stworzenie modelu gry *Heroes III*. Jest to strategiczna gra turowa. Rozgrywka polega na eksploracji mapy gry i rozbudowie zamku oraz zwiększaniu liczebności armii przy wykorzystaniu surowców produkowanych przez kopalnie.  
Dla uproszczenie jedynym typem surowców będzie złoto, a jednostki nie będą podzielone na odrębne rodzaje. Liczba siedlisk jednostek i kopalni będzie ustana przy rozpoczęciu symulacji. Wszystkie wątki *gracz* będą współdzieliły te same zasoby. 

## 2. Wątki
- **Gracz** – przemieszcza się po mapie gry w celu podnoszenia skarbów lub atakowania przeciwników. Do wykonania pierwszej operacji potrzebuje zasobu *mapa* oraz *skarbiec ze złotem*, natomiast do wykonania drugiej wymagane są zasoby *mapa* oraz *armia*. Dodatkowo gracz będzie pozostawał przez pewien czas w spoczynku - wątek będzie wtedy uśpiony.
- **Kopalnia złota** – kopalnie posiadają dwa typy aktywności. Pierwszą jest wydobywanie złota (stan podobny do rozmyślania w przypadku problemu ucztujących filozofów), drugą jest transport złota do skarbca. Druga aktywność wymaga zasobu *skarbiec ze złotem* i polega na zwiększeniu jego ilości o pewną wartość.
\newpage
- **Siedlisko stworzeń** – podobnie jak kopalnie, siedliska przez pewien czas produkują jednostki, a następnie, jeżeli w skarbcu gracz jest wystarczająco dużo złota, dodają wyprodukowane jednostki do armii. Wymagane zasoby to *armia* i *skarbiec ze złotem*.

## 3. Zasoby
- **Skarbiec ze złotem** – struktura z polem określającym ilość złota w skarbcu.
- **Armia** – struktura z polem określającym liczbę jednostek w armii.
- **Mapa gry** – struktura zawierająca prostokątną mapę gry. Przechowuje informacje o pozycji obiektów – skarbów i przeciwników – na mapie.

## 4. Zakleszczenia
Zakleszczenie może wystąpić, gdy jeden wątek *gracz* pozyska zasób *mapa* i będzie chciał pozyskać zasób *armia* (lub *skarbiec ze złotem*), a drugi wątek pozyska zasób *armia* (lub *skarbiec ze złotem*) i będzie potrzebował zasobu *mapa*.

## 5. Zagłodzenie wątków
Zagłodzenie może wystąpić w wielu miejscach. Na przykład, gdy czas wydobywania złota w wątku *kopalnia złota* jest zerowy, wątek ten cały czas trzymałby zasób *skarbiec ze złotem*. Co uniemożliwi podjęcie akcji na przykład wątkowi *siedlisko stworzeń*.  
Inną sytuacja, która mogłaby doprowadzić do głodzenia wątków wystąpiłaby, gdy jeden z wątków gracz miałby zerowy czas spoczynku. W takiej sytuacji ciągle podejmowałby on akcje potencjalnie blokując innym wątkom, korzystającym z tych samych zasobów, taką możliwość.