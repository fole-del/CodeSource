package main

import "fmt"

type Animal struct {
	name string
}

func (a Animal) Eat(Food string) {
	fmt.Printf("%s is Eating %s\n", a.name, Food)
}

func (a Animal) Run()  {
	fmt.Printf("%s is Running\n", a.name)
}

type Dog struct {
	Animal
	wang int
}

type Cat struct {
	Animal
	miao int
}

func (d Dog) Run()  {
	fmt.Printf("%s runed %d KM\n", d.name, d.wang)
}

type Behavior interface {
	Eat(food string)
	Run()
}


func main() {
	xiaohua := Cat{Animal{"xiaohua"},15}
	bai := Dog{Animal{"bai"},15}

	var i Behavior

	i = xiaohua
	i.Eat("猫粮")
	i.Run()

	i = bai
	i.Eat("狗粮")
	i.Run()
}