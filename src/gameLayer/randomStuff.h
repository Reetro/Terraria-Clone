#pragma once
#include <random>

/**
* @brief Generates a random float
* @param rng Reference to a std::ranlux24_base random number generator.
 * Must be initialized (optionally seeded) before use.
* @param min Min float value
* @param max Max float value
* @return A random float
*/
float getRandomFloat(std::ranlux24_base &rng, float min, float max);

/**
* @brief Generates a random int
* @param rng Reference to a std::ranlux24_base random number generator.
 * Must be initialized (optionally seeded) before use.
* @param min Min int value
* @param max Max int value
* @return A random int
*/
int getRandomInt(std::ranlux24_base &rng, int min, int max);

/**
* @brief Generates a random bool
* @param chance The chance for it to return true
* @return A random bool
*/
bool getRandomChance(std::ranlux24_base &rng, float chance);