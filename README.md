# Dino Game
## Opis
Inspiracja zaczerpnięta z gry przeglądarkowej Chrome Dino. "Dino Game" to dynamiczna gra platformowa, w której gracz wciela się w postać dinozaura przemierzającego niebezpieczne tereny pełne przeszkód. Celem gry jest pokonanie jak największej odległości, unikając kaktusów i ptaków, zbierając monety oraz zdobywając osiągnięcia.
### Główne cechy gry
- Intuicyjne sterowanie: Skacz, biegaj i lataj, używając prostych klawiszy, aby unikać przeszkód.
- Różnorodne przeszkody: Zmierz się z kaktusami i ptakami, które stanowią wyzwanie na Twojej drodze.
- Tryb powietrzny: Unieść się w przestworza i pokonaj nadlatującą chmarę ptaków
- Zbieranie monet: Zbieraj monety, aby zwiększyć swój wynik i odblokować nowe skórki dla dinozaura.
- Poziom trudności: Zmień poziom trudności, aby uczynić rozgrywkę łatwiejszą lub trudniejszą
- Osiągnięcia: Zdobywaj osiągnięcia za pokonanie określonych dystansów, wykonanie skoków i inne wyzwania.
- Statystyki: Śledź swoje postępy, takie jak liczba zdobytych monet, najlepszy wynik, całkowity dystans, liczba skoków i śmierci.

## Instalacja
### Windows x64
Wystarczy, że pobierzesz najnowszą wersję z zakładki Releases na GitHubie. Rozpakuj plik .zip i uruchom plik Dino Game.exe, aby uruchomić grę.
### Linux
Wymagania: Zainstalowany C++ wraz z kompilatorem oraz biblioteka SFML. Dla systemów opartych na Debianie możesz użyć następujących komend:
- C++ i narzędzia do kompilacji
```
sudo apt update
sudo apt install build-essential
```
- SFML
```
sudo apt update
sudo apt install libsfml-dev
```
Po zainstalowaniu następujących bibliotek można przejść do instalacji
1. Sklonuj repozytorium na swój komputer
2. W terminalu, przejdź do folderu, w którym znajduje się repozytorium
3. Skompiluj program. Użyj do tego komendy:
```
g++ *.cpp -o DinoGame.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```
4. Uruchom grę za pomocą komendy:
```
./DinoGame.exe
```

## Sterowanie
- Strzałki lewo, prawo - poruszanie Dinem
- Strzałka w górę lub Spacja - skok Dinem
- Strzałka w dół lub Lewy Control - kucanie Dinem
- Escape - wyjście do poprzedniego menu
- R - reset poziomu
- Q - wyjście z programu

## Do przyszłego zaimplementowania
- Achivementy (na razie tylko proof-of-the-concept)
- Animacja latania i kucania
- Nowe poziomy z nowymi sceneriami

## Skład zespołu:  
- Dominika Szymaniak  
- Ignacy Wereniuk  
- Krzysztof Ball  
- Krzysztof Kałańdziak  
- Mateusz Kucio  
- Tomasz Kruk  
- Wojciech Romanowski  
  
Język C++ 
