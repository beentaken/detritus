package main

import "fmt"

func printPrime(prime int) {
    fmt.Println(prime, "is prime")
}

func main() {
    max_num := 1073741824
    numbers := make([]bool, max_num)
    num_primes := 0

    for i := 2; i < max_num; i++ {
        if numbers[i] == false {
            num_primes++
        }

        for j := i; j < max_num; j += i {
            numbers[j] = true
        }
    }

    fmt.Println(num_primes, "out of", max_num, "are primes.")
}
