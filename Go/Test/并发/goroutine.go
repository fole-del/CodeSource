package main

import (
	"fmt"
	"sync"
	"time"
)

func main() {
	var wg sync.WaitGroup
	var limit = make(chan int, 3)
	var works []func()

	for i := 0; i < 100; i++ {
		works = append(works, func() {
			fmt.Println("hi")
		})
	}

	for _, w := range works {
		work := w
		wg.Add(1)
		go func() { // 以协程的方式运行
			defer wg.Done()
			limit <- 1
			work()
			time.Sleep(time.Second)
			<-limit
		}()
	}
	wg.Wait()
}
