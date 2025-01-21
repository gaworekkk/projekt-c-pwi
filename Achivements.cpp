#include "Achivements.h"

int range_distance(int totalDistance){
    if(totalDistance < 1000) return 1000;
    if(totalDistance < 10000) return 10000;
    return 100000;
}

int range_death(int deathCount){
    if(deathCount < 10) return 10;
    if(deathCount < 25) return 25;
    return 100;
}

int range_coins(int coinCount){
    if(coinCount < 100) return 100;
    if(coinCount < 250) return 250;
    return 1000;
}

int range_jump(int jumpCount){
    if(jumpCount < 50) return 50;
    if(jumpCount < 100) return 100;
    return 500;
}