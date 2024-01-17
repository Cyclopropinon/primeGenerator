#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <chrono>

void write_primes_to_file(const std::string& filename)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    constexpr uint32_t limit = std::numeric_limits<uint32_t>::max();
    const uint32_t sqlimit = (uint32_t)std::sqrt(limit) + 1;
    std::cout << limit << '\t' << sqlimit << std::endl;
    std::vector<bool> sieve(limit, true);
    uint32_t i;

    for (i = 2; i <= sqlimit; ++i)
    {
        if (sieve[i])
        {
            file.write(reinterpret_cast<const char*>(&i), sizeof(uint32_t));    // write primes to file
            for (uint64_t j = i * i; j <= limit; j += i)                        // sieve out the numbers
            {
                sieve[j] = false;
            }
        }
    }

    auto sieve_finish_time = std::chrono::high_resolution_clock::now();
    std::cout << "Finished sieving. Now writing to file." << std::endl;
    file.flush();                                                               // flush for safety

    for (i; i <= limit && i > sqlimit; ++i)                                     // Overflow-safe
    {
        if (sieve[i])
        {
            file.write(reinterpret_cast<const char*>(&i), sizeof(uint32_t));    // write the rest of the primes to file
        }
    }

    file.close();

    auto finish_time = std::chrono::high_resolution_clock::now();
    auto sieve_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(sieve_finish_time - start_time).count();
    auto write_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - sieve_finish_time).count();
    auto total_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time - start_time).count();
    std::cout << "Prime numbers up to 2^32-1 written to \"" << filename
              << "\"\nSieving Time:\t" << sieve_duration
              << "ns\nWriting Time:\t" << write_duration
              << "ns\nTotal Time:\t" << total_duration << "ns" << std::endl;
}

int main()
{
    const std::string filename = "primes.bin";
    write_primes_to_file(filename);
    return 0;
}
