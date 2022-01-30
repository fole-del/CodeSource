## 方法

> **方法**是作用在自定义类型上的一类特殊函数，通常自定义类型的值会被传递给该函数，该值可能是以指针或者复制值的形式传递。定义方法和定义函数几乎相同，只是需要在 `func` 关键字和方法名之间必须写上接接受者。例如我们给类型 `Count` 定义了以下方法：
>
> ~~~go
> type Count int
> 
> func (count *Count) Increment() { *count++ }  // 接受者是一个 `Count` 类型的指针
> func (count *Count) Decrement() { *count-- }
> func (count Count) IsZero() bool { return count == 0 }
> ~~~
>
> 以上代码中，我们在内置类型 `int` 的基础上定义了自定义类型 `Count`，然后给该类型添加了 `Increment()`、`Decrement()` 和 `IsZero()` 方法，其中前两者的接受者为 `Count` 类型的指针，后一个方法接收 `Count` 类型的值。

Demo:

类型的方法集是指可以被该类型的值调用的所有方法的集合。

一个指向自定义类型的值的指针，它的方法集由该类型定义的所有方法组成，无论这些方法接受的是一个值还是一个指针。如果在指针上调用一个接受值的方法，Go 语言会聪明地将该指针解引用。

一个自定义类型值的方法集合则由该类型定义的接收者为值类型的方法组成，但是不包括那些接收者类型为指针的方法。

其实这些限制 Go 语言帮我们解决的非常好，结果就是我们可以在值类型上调用接收者为指针的方法。假如我们只有一个值，仍然可以调用一个接收者为指针类型的方法，这是因为 Go 语言会自动获取值的地址传递给该方法，前提是该值是可寻址的。

在以上定义的类型 `Count` 中，`*Count` 方法集是 `Increment()`, `Decrement()` 和 `IsZero()`，`Count` 的值的方法集是 `IsZero()`。但是因为 `Count` 类型的是可寻址的，所以我们可以使用 `Count` 的值调用全部的方法。

另外如果结构体的字段也有方法，我们也可以直接通过结构体访问字段中的方法。下面让我们练习下，创建源文件 `struct_t.go`，输入以下代码：

```go
package main

import "fmt"

type Count int // 创建自定义类型 Count

func (count *Count) Increment()  { *count++ } // Count类型的方法
func (count *Count) Decrement()  { *count-- }
func (count Count) IsZero() bool { return count == 0 }

type Part struct { // 基于结构体创建自定义类型 Part
    stat  string
    Count // 匿名字段
}

func (part Part) IsZero() bool { // 覆盖了匿名字段Count的IsZero()方法
    return part.Count.IsZero() && part.stat == "" // 调用了匿名字段的方法
}

func (part Part) String() string { // 定义String()方法，自定义了格式化指令%v的输出
    return fmt.Sprintf("<<%s, %d>>", part.stat, part.Count)
}

func main() {
    var i Count = -1
    fmt.Printf("Start \"Count\" test:\nOrigin value of count: %d\n", i)
    i.Increment()
    fmt.Printf("Value of count after increment: %d\n", i)
    fmt.Printf("Count is zero t/f? : %t\n\n", i.IsZero())
    fmt.Println("Start: \"Part\" test:")
    part := Part{"232", 0}
    fmt.Printf("Part: %v\n", part)
    fmt.Printf("Part is zero t/f? : %t\n", part.IsZero())
    fmt.Printf("Count in Part is zero t/f?: %t\n", part.Count.IsZero()) // 尽管覆盖了匿名字段的方法，单还是可以访问

}
```

以上代码中，我们创建了 `Count` 类型，然后在其基础上又创建了结构体类型 `Part`。我们为 `Count` 类型定义了 3 个方法，并在 `Part` 类型中创建了方法 `IsZero()` 覆盖了其匿名字段 `Count` 中 `IsZero()` 方法。但是我们还是可以二次访问到匿名字段中被覆盖的方法。执行代码，输出如下：

```bash
$ go run struct_t.go
Start "Count" test:


Origin value of count: -1
Value of count after increment: 0
Count is zero t/f? : true

Start: "Part" test:
Part: <<232, 0>>
Part is zero t/f? : false
Count in Part is zero t/f?: true
```