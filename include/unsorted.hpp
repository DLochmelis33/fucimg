#pragma once
#include "common.hpp"

void zakaz1(vvpix& pixels);

void zakaz2(vvpix& pixels);

void bounds_avgsigm(vvpix& pixels, uint TH, int n = 5);

void mix_2kp1(vvpix& pixels, unsigned int k); // i.e. mix_5 for k=2

void color_k_means(vvpix& pixels, int k, double stopSensitivity = 3,
                   int stepsLimit = INT_MAX); // might loop indefinitely if stopSensitivity < 3
