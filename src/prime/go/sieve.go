package main

import "fmt"

func printPrime(prime int) {
    fmt.Println(prime, "is prime")
}

func main() {
    max_num := 1073741824
    numbers := make([]bool, max_num)

    printPrime(1)
    for i := 2; i < max_num; i++ {
        if numbers[i] == false {
            printPrime(i)
        }

        for j := i; j < max_num; j += i {
            numbers[j] = true
        }
    }
}
