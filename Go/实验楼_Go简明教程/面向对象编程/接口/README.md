## 接口

在 Go 中，**接口**是一组方法签名。当一个类型为接口中的所有方法提供定义时，它被称为实现该接口。它与 oop 非常相似。接口指定类型应具有的方法，类型决定如何实现这些方法。

### 接口基础

之所以说 Go 语言的面向对象很灵活，很大一部分原因是由于接口的存在。接口是一个自定义类型，它声明了一个或者多个方法签名，任何实现了这些方法的类型都实现这个接口。`infterface{}` 类型是声明了空方法集的接口类型。任何一个值都满足 `interface{}` 类型，也就是说如果一个函数或者方法接收 `interface{}` 类型的参数，那么任意类型的参数都可以传递给该函数。接口是完全抽象的，不能实例化。接口能存储任何实现了该接口的类型。直接看例子吧，创建源文件 `interface_t.go`，输入以下代码：

~~~go
package main

import "fmt"

type Human struct { // 结构体
    name  string
    age   int
    phone string
}

//Human实现SayHi方法
func (h Human) SayHi() {
    fmt.Printf("Hi, I am %s you can call me on %s\n", h.name, h.phone)
}

//Human实现Sing方法
func (h Human) Sing(lyrics string) {
    fmt.Println("La la la la...", lyrics)
}

type Student struct {
    Human  //匿名字段
    school string
    loan   float32
}

type Employee struct {
    Human   //匿名字段
    company string
    money   float32
}

// Employee重载Human的SayHi方法
func (e Employee) SayHi() {
    fmt.Printf("Hi, I am %s, I work at %s. Call me on %s\n", e.name,
        e.company, e.phone)
}

// Interface Men被Human,Student和Employee实现
// 因为这三个类型都实现了这两个方法
type Men interface {
    SayHi()
    Sing(lyrics string)
}

func main() {
    mike := Student{Human{"Mike", 25, "222-222-XXX"}, "MIT", 0.00}
    paul := Student{Human{"Paul", 26, "111-222-XXX"}, "Harvard", 100}
    sam := Employee{Human{"Sam", 36, "444-222-XXX"}, "Golang Inc.", 1000}
    Tom := Employee{Human{"Tom", 37, "222-444-XXX"}, "Things Ltd.", 5000}

    //定义Men类型的变量i
    var i Men

    //i能存储Student
    i = mike
    fmt.Println("This is Mike, a Student:")
    i.SayHi()
    i.Sing("November rain")

    //i也能存储Employee
    i = Tom
    fmt.Println("This is Tom, an Employee:")
    i.SayHi()
    i.Sing("Born to be wild")

    //定义了slice Men
    fmt.Println("Let's use a slice of Men and see what happens")
    x := make([]Men, 3)
    //这三个都是不同类型的元素，但是他们实现了interface同一个接口
    x[0], x[1], x[2] = paul, sam, mike

    for _, value := range x {
        value.SayHi()
    }
}
~~~

### 接口变量值的类型`*`

> <span style="background: PaleVioletRed;border-radius:10px;padding:1px 5px;color:white;font-weight:900">接口类型声明的变量里能存储任何实现了该接口的类型的值。</span>
>
> 有的时候我们需要知道这个变量里的值的类型，那么需要怎么做呢？可以使用类型断言，或者是 `switch` 类型判断分支。以下的例子 `interface_t1.go` 我们使用了 `switch` 类型判断分支。

~~~go
/*使用了switch 类型判断分支*/

package main

import (
	"fmt"
	"strconv"
)

type Element interface{}
type List []Element

type Person struct {
	name string
	age  int
}

// 实现了fmt.Stringer接口
func (p Person) String() string {
	return "(name: " + p.name + "- age: " + strconv.Itoa(p.age) + " years)"
}

func main() {
	list := make(List, 4)
	list[0] = 1
	list[1] = "Hello"
	list[2] = Person{"Dennis", 70}
	list[3] = 1 + 2i

	for index, element := range list {
		switch value := element.(type) {
		case int:
			fmt.Printf("list[%d] is an int and its value is %d\n", index, value)
		case string:
			fmt.Printf("list[%d] is an int and its value is %s\n", index, value)
		case Person:
			fmt.Printf("list[%d] is an int and its value is %s\n", index, value)
		default:
			fmt.Printf("list[%d] is of a different type", index)
		}
	}
}
~~~

### 嵌入interface

结构体中可以嵌入匿名字段，其实在接口里也可以再嵌入接口。<span style="background: PaleVioletRed;border-radius:10px;padding:1px 5px;color:white;font-weight:900">如果一个 `interface1` 作为 `interface2` 的一个嵌入字段，那么 `interface2` 隐式的包含了 `interface1` 里的方法。</span>如下例子中，`Interface2` 包含了 `Interface1` 的所有方法。

~~~go
type Interface1 interface {
    Send()
    Receive()
}

type Interface2 interface {
    Interface1
    Close()
}
~~~