# Dino Game
## Opis

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

## Skład zespołu:  
- Dominika Szymaniak  
- Ignacy Wereniuk  
- Krzysztof Ball  
- Krzysztof Kałańdziak  
- Mateusz Kucio  
- Tomasz Kruk  
- Wojciech Romanowski  
  
Język C++ 
