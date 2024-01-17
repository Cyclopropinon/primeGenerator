#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

void write_primes_to_file(const std::string& filename)
{
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
            std::cout << i << '\r';
            file.write(reinterpret_cast<const char*>(&i), sizeof(uint32_t));    // write primes to file
            for (uint64_t j = i * i; j <= limit; j += i)                        // sieve out the numbers
            {
                sieve[j] = false;
            }
        }
    }

    file.flush();                                                               // flush for safety

    for (i; i <= limit; ++i)
    {
        if (sieve[i])
        {            
            file.write(reinterpret_cast<const char*>(&i), sizeof(uint32_t));    // write the rest of the primes to file
        }
        if (i % 32768 == 1)
        {
            std::cout << i << '\r';
        }

    }

    file.close();
}

int main()
{
    const std::string filename = "primes.bin";
    write_primes_to_file(filename);

    std::cout << "Prime numbers up to 2^32-1 written to " << filename << std::endl;

    return 0;
}
